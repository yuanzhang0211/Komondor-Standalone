# Komondor Standalone 开发约定

## 范围与必读材料

当前工作范围是原生输入下的基础规则 Co-TDMA 和每 AP 独立只读状态；Co-SR/Co-BF 为后续任务，不接入 RL、MAB、训练或模型加载。
修改前阅读 `docs/DEVELOPMENT_RULES.md`、`docs/SHARED_HEADERS.md`、`docs/FILE_INVENTORY.md`，并在 `docs/HEADER_CATALOG.md` 中查找已有实现及直接使用者。

## 公共实现与复用

- 三种模式共用一套 Notification/TxInfo、FIFO、MPDU 事务、PHY 功率/帧时长、接收判决、退避、统计和协作状态驱动；禁止复制整个节点/PHY/队列实现另起模式分支。
- `structures/` 放数据类型及状态；`methods/phy/` 放物理计算；`methods/mac/` 放 MAC 通用算法；`methods/node/` 放绑定 Node 的事件/状态转换。先扩展现有公共实现，再考虑新增文件。
- `methods/node/node_*_methods.h` 是实现片段，只由 `node_impl.h` 统一接入；`node_impl.h` 仅在 `main/node.h` 尾部包含一次。不得给测试或模式文件直接 include 这些片段。
- `methods/utils/input_methods/input_loader.h` / `input_validator.h` 也是绑定 Komondor 的实现片段，不是可独立包含的公共库。
- 不新增三模式总括头，不把所有实现重复 include 到另一入口；维持 COST 单入口的包含顺序。COST/compcpp 不因协议修复而修改。
- `mapc_methods.h` 是现存上游占位文件，不能当作已工作的协作公共实现；实际 Co-SR 入口见 `node_packet_methods.h::ComputeCoSRTxPowers`。
- `paper_replay.h`、`paper_edca.h`、`tdma_pilot.h`、DL CF-End 是当前 DL/Co-TDMA 专用扩展。其通用价值需先提炼、验证，再供 SR/BF 使用，不能原样复制或无条件启用。
- 新公共 API 写明输入/输出单位、功率口径、数组布局、状态归属、失败/回退语义及调用前置条件；状态保存于节点/轮次，禁止引入可变全局协作状态。

## 来源及变更记录

- 保留上游版权/许可；官方原文件上的修改标为“官方原有、本地修改”，不得整文件称为本地原创。文件归属与算法归属分别说明。
- 历史官方参照为 `a40896d881bfb377c07a9ccef249cdab52849445`；不声称它是当前最新，也不声称是本地历史 checkout 祖先。
- 修改原生文件时，同步更新 `provenance/file_annotations.json` 的对应职责/变更说明和 `docs/CHANGES.md`。新增共享能力还要更新共享头文件目录。
- `provenance/comparison.json` 和 `import_manifest.json` 是冻结历史证据，不改写成新代码身份；`base_to_standalone.patch` 也是提取时快照。当前身份由 `file_inventory.json` 记录。
- 完成修改后运行 `make inventory`，再运行 `make inventory-check`；不得手工编辑自动生成的 FILE_INVENTORY / HEADER_CATALOG / file_inventory。
- 没有官方逐文件证据的导入资产明确标为“导入来源已知，官方身份未逐文件核验”，不得猜测其为官方未改文件。生成物和可执行工具不算本地原创源码。

## 验证

- 修改公共行为必须覆盖 Co-TDMA 参照及受影响模式。预算/守恒/超时/NAV 等验证机制不变量；BF 数值修改先做矩阵与功率参考测试。
- 能启动、吞吐改善均不能单独证明 SR/BF 正确。新增模式修复要记录通过的案例、未验证边界。
- `make test` 为现有原生不变量和搬迁运行检查；文档/清单变更用 `make inventory-check`，无需重复整套仿真。
- 不手改 `komondor_main.cxx`、`compcxx_komondor_main.h`；运行时不覆写既有 `runs/` 结果；用户明确要求整理时，可删除未被当前报告引用的临时/重复运行，保留删除清单并修复文档引用。独立构建不得依赖旧 RL 项目或外部链接目录。

## 编译环境

- 遵守 `docs/BUILD_ENVIRONMENT.md`；基准为 Ubuntu 22.04 x86_64、GCC 11.4、GNU Make 4.3、Python 3.10，保持 C++98。
- 仿真器构建参数仅定义在根 `build.mk`，默认 release；debug/sanitize 用 PROFILE 选择。测试沿用同一 CXX/flags，不另写工具链。
- 改构建前后执行 `make doctor`；改变编译器/flags 由 `.build-config` 触发重编译。同目录不并行构建不同配置。
- 默认无 Conda/pip 依赖；COST 随包工具只验证了 Linux x86_64。flex 仅用于重建生成器，缺失时不得声称重建验证通过。
- `build/environment.json` 是本机生成报告；`provenance/build_reference.json` 为冻结基准，不将两者混用或覆盖。

## 官方目录与配套材料

- 保留 Apps、Documentation、Code/input、Code/scripts_multiple_executions、Code/validate.sh 和官方根目录说明，禁止再按最小后端裁掉。目录职责见 docs/DIRECTORY_LAYOUT.md。
- README.md 保留官方正文及本地导航；本地使用说明在 README.local.md。配套材料身份以 official_materials.json 的固定提交哈希为证据，缺失会被 inventory 拦截。
- 官方示例/验证输入保持原样；新增本地场景放 examples 或明确的新文件，不改写历史基准。官方学习样例可以保留，但默认流程不执行学习。
- 官方生成物、旧仿真输出和 .git 历史不复制；Code/output 保留结构，原脚本会写固定输出。完整长时回归须单独执行/记录，不能用短时 smoke 代替。
- Apps 的 Java/Matlab/绘图依赖与核心构建依赖分开；未经实际验证不得声称辅助工具全部可用。

## 当前基础 Co-TDMA 与分布式观测契约

- 最新范围是原生输入的基础规则 Co-TDMA + 每 AP 独立只读观测；不增加需求感知、多 AC 重构或 Agent 动作。以 docs/BASIC_NATIVE_COTDMA.md 为当前入口，早期研究扩展仍需显式启用。
- 面向多 AP、多 Agent，每 Agent 对应一个 AP；状态只写 ap_<node_id>.jsonl，禁止新增全网状态汇总表/集中式观测对象，不得为观测读取其他 AP 私有队列。对端信息仅来自本 AP 实际收到的协议报告。
- ap_observation.h 为每 AP 独立 IO/已收报告缓存；node_observation_methods.h 仅经 node_impl.h 聚合，不能调 RNG、添加计时器或改变调度。
- 修改状态 schema/事件语义必须同步文档和单 AP 读取器，观测开关前后协议轨迹应一致。未知预算以 own_data_budget_valid 标识，禁止暴露旧轮次预算作当前预算。
- 多 AP 不等于单轮全体 AP 共享：目前仍是协调者邀请一个 peer；轮询按 AP/组本地保存。回归须覆盖非连续 AP ID、多 STA、多个组及超过八个 peer。
