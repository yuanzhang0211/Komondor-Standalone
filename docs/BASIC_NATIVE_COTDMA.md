# 原生输入、基础规则 Co-TDMA 与每 AP 独立状态

## 本轮范围

按最新要求，本轮只完善基础规则 Co-TDMA 和只读状态输出。需求感知、多 AC 重构、CF-End 扩展接入原生流量、学习/奖励/Agent 动作接口不在本轮范围。已有 replay/pilot 研究代码保留，但必须显式启用；默认运行入口已改为原生流量。

系统面向多 AP、多 Agent：每个 AP 有自己的状态文件，未来每个 Agent 只读取对应 AP 文件。没有全网状态表、没有集中式观测对象，没有跨 AP 直接读取队列/业务/模型状态。

## 原生输入及规则

继续使用官方的节点 CSV、MAPC CSV 和 config_models，不要求增加 trace、plan 或新列。MAPC 示例：

```text
GroupID;Method;CoordinatedBSSIds;ExtraParams
1;CO_TDMA;A,B,C,D;
```

竞争成功的 AP 按规则轮换协作组，在当前组内轮换选择一个对端 AP；ICR 确认对端有数据后按原有等分规则分配 DATA 预算。STA 的选择保持原生方式，未改成 Agent 或需求调度。

网络可以有多个 AP，每个 AP 都能竞争成为协调者或被其他 AP 邀请。当前协议每轮是“协调 AP + 一个受邀 AP”的顺序共享，并非一轮给组内全部 AP 分配连续时隙。每 AP 最多属于 8 个协作组，沿用上游限制；对端列表改为动态容量，已实际覆盖单组 10 AP / 9 个 peer。

官方旧 high_density CSV 仍使用历史字段布局，本轮不转换，不与当前原生输入混称兼容。读取失败必须检查返回码及 run.json 的 simulation_completed。

## 运行与单 AP 读取

```bash
make doctor
make build
make smoke                 # 四 AP、多 STA，原生规则与独立状态输出
make basic-test            # 本轮专项回归
make test                  # 原有回归、搬迁构建及本轮专项回归
```

使用原始官方输入：

```bash
python3 scripts/run.py --mode tdma --traffic native --observe \
  --nodes Code/input/examples/mapc_example/input_nodes_mapc_example.csv \
  --mapc Code/input/examples/mapc_example/mapc_cotdma_example.csv \
  --config Code/config_models --time 0.2
```

使用多 AP 输入：

```bash
python3 scripts/run.py --mode tdma --traffic native --observe \
  --nodes examples/multi_ap_native/nodes.csv \
  --mapc examples/multi_ap_native/mapc.csv \
  --config examples/multi_ap_native/config_models --time 0.3
```

每次运行创建新 runs 目录，其 `states/` 下只有 AP 独立文件。例如四 AP 示例的原生节点编号为 0、2、5、7，对应 `ap_0.jsonl`、`ap_2.jsonl`、`ap_5.jsonl`、`ap_7.jsonl`。编号是节点 CSV 数据行的零基索引，不是连续的 AP 序号，也不是协作组号。

Agent 对应 AP 5 时只读取：

```bash
python3 scripts/read_ap_state.py runs/本次运行/states --ap 5
# --all-events 也只输出 AP 5 自己的事件历史
```

Python 接口（scripts 在导入路径上时）：

```python
from read_ap_state import latest_ap_state, iter_ap_states
state = latest_ap_state(run_dir / "states", ap_id=5)
```

读取函数只打开指定 AP 的文件，不遍历其他 AP 文件；支持读取完整行并忽略末尾尚未写完的一行。每次写入均 fflush，因此运行过程中也能读到已完成事件。没有等待 Agent 的屏障或动作回传，不能将文件观测称为同步交互训练环境。

原生 CLI 也提供 `--state-dir <新目录>`，不依赖 Python。目录及每个 AP 文件均拒绝覆盖。默认关闭观测；原生 CLI 必须使用与原工具一致的 cwd/config_models 布局，推荐由 scripts/run.py 隔离执行。当前状态接口仅面向原生 Co-TDMA/普通 CSMA，拒绝与 replay、pilot 或 Agents 混用。

## AP 状态契约

JSONL schema 标识为 `komondor.ap_state.v1`。每行一个事件快照，sequence 仅在该 AP 内从 0 递增，time_s 是已发生事件的仿真时间；没有跨 AP 的全局序号或状态聚合。

|字段|含义及来源|
|---|---|
|ap_id、wlan_code|本 AP 的原生节点 ID 与 BSS 标识|
|event、sequence、time_s|本 AP 事件类型、顺序及仿真秒数|
|state_id、state_name、role|本 AP FSM 状态及当前协调者/参与者/竞争/等待角色|
|configured_group_ids、associated_sta_ids|本 AP 的本地配置：所属组和关联 STA，不含其他 BSS 状态|
|group_id、coordinator_id、selected_peer_id|当前本 AP 交换的组/角色/受邀对象；无活动交换时为 null|
|queue_packets、selected_queue_packets、queue_hol_age_s|本 AP 真实队列数量、当前过滤队列数量、整个队列最老包年龄（秒）|
|generated_mpdus、overflow_mpdus、source_replaced_mpdus、acked_mpdus、ledger_balance|本 AP 生成、溢出、饱和源刷新替换、唯一 ACK 与守恒余额|
|in_flight_mpdus|本 AP 当前 DATA/WAIT_ACK/RX_ACK 状态下的事务 MPDU 数量；不代表远端已收到|
|cw_min、cw_max、backoff_remaining_s、nav_until_s|本地竞争窗口、退避和 NAV；无 NAV 计时器时为 null|
|primary_channel、channel_left、channel_right、mcs|本地当前/最近选用的信道和零基 MCS；尚未选择宽度时 MCS 为 null，不是全网 CSI|
|channel_power_pw、sensitivity_pw、tx_power_total_pw|本 AP 观测的主信道功率、感知门限与发射总功率；线性 pW，不是 dBm|
|own_data_budget_valid、own_data_budget_s、peer_slot_active|本轮本地预算是否已确定、秒数及是否安排对端时隙；预算未知时 valid=false 且数值为 0，不能将旧轮次预算用于决策|
|peer_report|只有本 AP 成功接收的 ICR 报告：source_id、received_at_s、has_data；未知时为 null，不输出远端队列、期限或未来业务|
|frame|本 AP 即将发出的帧或实际收到的 ICR/授权帧摘要；包含方向、类型、时长、聚合数及 DATA MCS|

主要事件包括 ready、arrival、access_won、icr_received、rule_allocation、peer_grant_received、tx、tx_complete、ack_commit、ACK/CTS/DATA/NAV timeout、timeout_restart/restart、solo_fallback/no_fit、final。事件驱动快照不是固定周期采样，也不承诺记录每个内部变量赋值。rule_allocation 在规则已计算后记录；未来接入动作时须另定义因果边界，不能离线改写本轮状态。

原生队列的数据包通常在发送时选目的 STA，不能将 queue_packets 解释为已按 STA 绑定的队列；本轮未虚构逐 STA 队列或导出集中式队列镜像。

## 基础流程修复

1. Co-TDMA 每轮只有一个目标回复 ICR，因此回复固定在 SIFS 后，不再因其在多 AP 组内的排列位置延迟。
2. 对端轮询按 AP 自己的每个组分别计数，避免多个组共用计数器造成固定成员被漏选；对端列表使用动态容量。
3. ICR 与 MU-RTS/TXS 检查目标、来源和组，接收方不接受其他组/其他协调者的报告或授权。
4. ICF 已预填充满缓存队列时，DATA 根据实际还在队列中的 MPDU 数聚合，避免切换 STA/MCS 后读取不存在的包。
5. 原生短队列的半份预算容不下一个 MPDU时，协调者在已保留预算内退回独占；参与者仍放不下则保留包重试。不能额外创造空口时间，也不能发送超预算 DATA。
6. 满缓存保留原有源刷新行为：补包计入 generated，被源刷新替换的未确认包单独记为 source_replaced，不混作 ACK 或队列溢出。守恒为 generated = overflow + source_replaced + unique_acked + queued；不是声称刷新包均已成功传送。
7. 固定间隔原生流量使用浮点 1.0/traffic_load，修正整数除法导致的零时间循环。
8. 默认原生运行不接受 replay 的 --policy 参数，避免参数被静默忽略；进程返回 0 但未进入/完成仿真也视为失败。关闭 BF 时的未初始化诊断字段已初始化。

## 验证汇总与证据

当前交付修改 12 个已有原生文件，新增 `ap_observation.h` 和 `node_observation_methods.h`。具体职责见 [CHANGES.md](CHANGES.md) 和 [FILE_INVENTORY.md](FILE_INVENTORY.md)，当前原生文件哈希及变更补丁保存在 provenance。

|检查|结果及范围|
|---|---|
|原生 C++ 单元检查|4 项通过，覆盖已有机制不变量和观测编码|
|既有运行回归|7 组回放/原生三模式短时运行通过|
|基础原生专项|16 组通过，覆盖四类业务、多 seed、空 peer、丢帧、带宽不对称、4 AP 非连续 ID/多 STA、重叠组、10 AP/9 peers|
|边界与读取|非法 MAPC 组失败路径、每 AP 守恒/预算、单 AP reader 和不完整尾行检查通过|
|独立性|官方材料台账、无链接、搬迁后干净构建、头文件依赖检查通过|
|观测中立性|同 seed 开/关观测的无线、控制帧、ACK 和规则轨迹一致；同 seed 状态可重复|
|Sanitizer|ASan/UBSan 下 16 组专项通过；关闭继承分配生命周期的 leak 检查|
|改动前后参照|官方 Co-TDMA 输入、满缓存、seed 1、0.05 s 下 154 个协议事件一致；生成/源刷新统计按修复口径记录|

证据与当前代码身份：

- [原生实现清单与 SHA256](../provenance/basic_native_implementation.json)
- [原生改动补丁](../provenance/basic_native_changes.patch)
- [专项逐例结果](../provenance/basic_native_validation.json)
- [既有回归与搬迁检查](../provenance/validation.json)
- [Sanitizer 配置与逐例结果](../provenance/basic_native_sanitizer.json)
- [改动前后协议参照](../provenance/basic_native_reference_check.json)

`runs/` 保留当前报告引用的原始输入、日志、指标和每 AP 状态；未引用的重复/调试运行可清理，规则见 [README.local.md](../README.local.md)。运行不依赖已有 runs，新测试会创建新目录。

本轮不宣称完成标准全覆盖、Co-SR/Co-BF PHY 修复、历史 CSV 转换或在线 Agent 控制。读取接口是事件快照，没有动作提交或仿真步进屏障；后续 Agent 接入须继续每 AP 独立观测，并另定义动作生效时点。
