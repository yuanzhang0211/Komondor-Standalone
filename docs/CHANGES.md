# 本地修改与来源

当前版本：原生输入下的基础规则 Co-TDMA、每 AP 独立只读状态，一 AP 对应一个未来 Agent。当前未实现 Agent 动作、需求感知或原生多 AC 重构。当前能力与检查结果统一见 [BASIC_NATIVE_COTDMA.md](BASIC_NATIVE_COTDMA.md)。

官方固定参照为 `a40896d881bfb377c07a9ccef249cdab52849445`；这是内容比较参照，不声称是最新版本或本地原始 checkout 祖先。官方原有协作状态机、Co-SR 四种功率策略、ULA/ZF、EDCA/DCB 不算本地新增。逐文件官方/本地身份见 [FILE_INVENTORY.md](FILE_INVENTORY.md)。

## 当前基础原生 Co-TDMA

新增两个头文件：`structures/ap_observation.h`（每 AP 独立 IO 和已收报告缓存）、`methods/node/node_observation_methods.h`（Node 绑定的本地只读快照）。修改 12 个已有原生文件，完整清单与 SHA256 在 [basic_native_implementation.json](../provenance/basic_native_implementation.json)，相对该轮开始前的补丁在 [basic_native_changes.patch](../provenance/basic_native_changes.patch)。

主要修复：单目标 ICR 的多 AP 延迟、按组 peer 轮询、固定八 peer 容量、预填队列不足的聚合越界、单包超授权、满缓存补包/源刷新守恒、固定间隔流量零时间循环及未初始化诊断字段。状态仅取本 AP 和实际收到的报告；观测不调用随机数或改变调度，预算未知状态显式标识。

原生 CLI 新增 `--state-dir`，运行器新增 `--observe`，单 AP 读取器为 `scripts/read_ap_state.py`。默认使用 native；历史 replay 显式选择，原生 --policy 不再静默忽略。多 AP 示例和专项测试保留在 examples/multi_ap_native 与 tests/basic_native_tdma.py。

## 底层修复：三种模式共享

|修改|行为及位置|边界|
|---|---|---|
|随机种子|`Code/main/komondor_main.cc` 将 CLI seed 交给 COST，在 Setup 前初始化随机流|改变随机轨迹，旧结果不可直接混用|
|MPDU/ACK 事务|`Code/structures/mpdu_transaction.h` 记录实际 DATA 的 MPDU ID；ACK 来源、ID、队列前缀校验；确认后提交出队，重复确认及守恒检查|不是单纯新增日志|
|控制帧解码|节点 FSM/packet 路径为 ICR、TF、MU-RTS/TXS 增补 SINR/PER 接收判断，等待/接收状态纳入干扰计算|依赖现有简化 PER 模型|
|NAV 延长|目标 AP 已在 NAV 时，重复发给它的 ICF 不再无限续长原 NAV|没有直接绕过原 NAV|
|Co-SR 时分回退|回退信息传给 peer；等待状态、padding、ACK 和后续 MU-RTS 识别实际时分模式|不代表四种功率算法已经验证|
|功率缓存|按实际发射功率刷新接收功率并初始化总功率元数据|仍需跨模式功率回归|
|BF 保护|跨 BSS 自动零陷仅在 Co-BF 使用；期望方向与零陷方向退化时保留期望链路|未修复 Gram 矩阵步长问题|
|传播 model 13|`Code/methods/phy/power_channel_methods.h` 等提供 TGn D 同层确定性中值路径损耗|不是完整随机衰落/CSI 模型|
|统计稳定性|KPI 脱离日志开关，修正重复归一化、除零、log(0)、Jain 分母；标识无吞吐/无时延样本 WLAN|无样本聚合延迟占位为仿真时长，不是真实成功时延|
|兼容与诊断|backoff 移除 ML_ACCESS 占位分支；保留 AUDIT/FIX/LINK 诊断|部分诊断针对固定节点 ID|

历史底层逐文件补丁：[official_to_local_base.patch](../provenance/official_to_local_base.patch)。[comparison.json](../provenance/comparison.json) 是历史完整审计，包含未纳入本次独立版的学习后端信息，仅作来源证据。

## 保留的历史非学习扩展（默认不启用）

独立版继续包含原 `tdma-online` 与 `paper-dl` 的非学习代码。相对修复基底的实际源码差异见 [base_to_standalone.patch](../provenance/base_to_standalone.patch)，无需在构建时应用。

|功能|关键文件|当前含义|
|---|---|---|
|Trace 回放与在线观测|`structures/tdma_pilot.h`、node/traffic_generator|按目的 STA 队列选择、速率/队列观测、双方预算和授权、逐 MPDU 记录|
|DL 应用回放与分片|`structures/paper_replay.h`|确定性应用输入、VO/VI/BE、1472B 有效载荷分片|
|多 AC 竞争|`structures/paper_edca.h`|各 AC 虚拟退避与内部碰撞，工程队列容量|
|规则需求感知共享|node 的预算选择和 mapc 路径|equal_txs / dl_ll_demand，不训练模型|
|预算与带宽保护|node/PHY 帧时长逻辑|DCB 带宽改变后重算，检查单包可容纳性、PPDU/TXOP 时长|
|CF-End 与轮次 NAV|`structures/dl_cfend.h`、`methods/node/node_dl_cfend_methods.h`|仅匹配轮次及合法发送者释放；丢失 CF-End 后自然超时；保护其他预约|

这些扩展当前是 SU DL 工程模型：没有完整 DL MU/RU、触发 UL、标准双 NAV、动态协作发现/生命周期或真实 CSI sounding/反馈老化/多径 MIMO。已有 EDCA、Co-SR 四种功率策略、ULA/ZF 和协作状态机来自上游，不能列为本地新发明。

## 独立化与官方材料

首次从已带修复的非学习后端提取，保留初始 [import_manifest.json](../provenance/import_manifest.json)。随后按固定官方提交补回 753 个配套文件，保留 Apps、Documentation、官方输入、验证/批量脚本、版本说明、许可和 IDE 配套；文件身份由 [official_materials.json](../provenance/official_materials.json) 与当前台账区分。详情见 [DIRECTORY_LAYOUT.md](DIRECTORY_LAYOUT.md)。

构建不依赖旧 RL 项目或补丁链。共享 build.mk、参数 stamp、make doctor 与构建环境约定见 [BUILD_ENVIRONMENT.md](BUILD_ENVIRONMENT.md)；来源/包含检查由 make inventory / inventory-check 管理。未导入 H-MAB、UCB、训练、模型冻结或奖励/信用归因框架。

## 文档与生成物整理（2026-11-03）

删除已过期的首次精简提取说明 EXTRACTION.md，将独特来源信息并入 DIRECTORY_LAYOUT.md；删除重复的 BASIC_NATIVE_COTDMA_REPORT.md，将验证汇总并入当前 BASIC_NATIVE_COTDMA.md。更新本地 README、路线图、开发规则和构建状态，保留官方 Markdown。

runs 清除 13 组未被当前报告引用的中间/重复/失败记录，保留 4 组有效证据。根目录忽略规则按路径整理，保留官方 Apps 分发资源及来源 JSON/补丁；详细清单见 [maintenance_cleanup.json](../provenance/maintenance_cleanup.json)。同时修正自动清单中特殊文件名（如 #、空格）的链接转义。本轮没有改动原生仿真代码和运行行为。
