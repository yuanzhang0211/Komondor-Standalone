# 公共头文件与复用边界

以下为现有代码的职责目录。所有头文件的来源和直接包含者见自动生成的 [HEADER_CATALOG.md](HEADER_CATALOG.md)；“公共”不等于本地新增，精确身份以 [FILE_INVENTORY.md](FILE_INVENTORY.md) 为准。

## 三模式共用：优先复用这些实现

下表路径均以 `Code/` 为根。

|唯一实现位置|公共职责|接入规则/适用范围|
|---|---|---|
|`list_of_macros.h`|帧类型、状态、单位及公共参数标识|沿用统一常量，不在 SR/BF 各自定义同义 ID|
|`structures/notification.h`|Notification、TxInfo、协作元数据|三模式共用空口通知结构；新增字段须初始化，不能复制成 SR/BF 通知结构|
|`structures/FIFO.h`|真实包队列及按目的操作|同一真实队列；策略只查询/选择，确认后统一提交|
|`structures/mpdu_transaction.h`|发送 MPDU 身份、ACK 事务及重复/守恒检测|本地新增公共机制；所有模式保留事务语义，不能绕过创建第二套 ACK 账本|
|`structures/wlan.h`、`node_parameters.h`、`node_configuration.h`、`sim_configuration.h`|BSS/协作组、节点和仿真配置|复用当前每节点/每组状态；不要以全局变量保存协作轮次|
|`structures/packet_exchange_sequence.h`、`channel_access_state.h`、`spatial_reuse_state.h`|交换顺序、竞争及 SR 状态|状态语义须区分 OBSS/PD SR 与 Co-SR；不混用模式特有标记|
|`methods/phy/power_channel_methods.h`|ConvertPower、路径损耗、功率分配、接收功率及干扰|三模式统一换算/缓存口径；已间接包含 beamforming，不再重复引入另一阵列实现|
|`methods/phy/beamforming_methods.h`|ULA、ZF、阵列增益及复数求解|公共 PHY 计算；仅 Co-BF 自动设置跨 BSS 零陷；已知矩阵步长问题待修|
|`methods/phy/frame_duration_methods.h`|RTS/CTS/DATA/ACK 及交换时长|三模式与预算检查统一使用，禁止复制速率/时长公式|
|`methods/phy/mcs_methods.h`、`channel_bonding_methods.h`|MCS 与动态带宽计算|带宽改变后重算真实帧时长，不只复用旧预算|
|`methods/mac/packet_loss_methods.h`、`nack_methods.h`|接收判决和失败原因|实际帧接收统一走 SINR/PER 逻辑；策略预计值不能代替接收事件|
|`methods/mac/nav_methods.h`|普通 NAV 计算|三模式共用；轮次 CF-End 预约是另一个受限扩展|
|`methods/mac/channel_access/*`|EDCA/DCF 及退避算法|复用现有参数/退避，不在每个模式实现一份|
|`methods/frames/notification_methods.h`、`packet_aggregation_methods.h`|构造通知、聚合计算|模式可以传不同元数据，公共帧构造与聚合规则统一|
|`structures/node_statistics.h`、`performance.h`、`simulation_stats.h`、`logger.h`|计数、KPI、系统统计及日志对象|一套事实与报告口径；无样本不能当成功包时延|

这些头存在单入口顺序依赖，有些含函数定义；只通过既有包含路径或经验证的独立测试使用，不新增“全公共头”重复收集全部实现。

## 公共状态驱动：只能从统一入口接入

|入口/实现片段|共享职责|包含约束|
|---|---|---|
|`main/node.h`|Node 组件声明及共享状态、计时器/端口|主入口接入，不复制 Node 做三套模式|
|`methods/node/node_impl.h`|聚合 Node 方法实现|仅在 node.h 末尾包含一次|
|`node_fsm_methods.h`、`node_packet_methods.h`|控制帧/数据/ACK 状态机、发送计划、Co-SR 功率策略及回退|只由 node_impl.h 包含；三模式共享驱动，局部模式分支|
|`node_backoff_methods.h`、`node_timeout_methods.h`、`node_mcs_methods.h`|竞争、超时、MCS/业务到达事件|同上，不能被测试/其他模式直接 include|
|`node_spatial_reuse_methods.h`、`node_config_methods.h`、`node_statistics_methods.h`|OBSS/PD SR、配置应用、节点统计|同上；OBSS/PD 不是 Co-SR 功率选择的替代入口|
|`methods/utils/input_methods/input_loader.h`、`input_validator.h`|Komondor 绑定的输入加载和校验实现|在 Komondor 定义后按现有主入口顺序接入；不当成独立工具头|
|`methods/utils/output_generation_methods.h`、logging/print helpers|公共结果生成与日志辅助|保留调用前置依赖；新模式复用既有输出路径|

## 现有模式专用扩展：暂不提升为三模式公共库

|位置|当前用途|后续复用规则|
|---|---|---|
|`structures/tdma_pilot.h`|早期在线 TDMA 计划/队列观测、STA 和预算选择|其中有外部计划入口，不作为新 SR/BF 或 RL 入口；需要通用队列快照时先剥离策略语义|
|`structures/paper_replay.h`|工程应用 trace 与分片|可作为未来通用业务输入候选；当前 replay 与 DL TDMA 语义耦合，入口对 SR/BF 不启用|
|`structures/paper_edca.h`|DL replay 的 VO/VI/BE 虚拟竞争|先核对普通流量路径与 EDCA 接入，再扩展其他模式，禁止复制一份 BF/SR EDCA|
|`structures/dl_cfend.h`、`methods/node/node_dl_cfend_methods.h`|DL 轮次预约、受限 CF-End 与释放|前者是状态/辅助算法，后者是 Node 实现片段；不是已完成的三模式标准双 NAV|

## 保留依赖与占位

`COST/` 和 `compcpp/` 是引擎/代码生成器依赖；`main/agent.h`、`central_controller.h`、`learning_modules/` 是上游保留的构建依赖，当前不启用。不得为“清理学习”盲目删除被包含的组件，也不在新修复中使用它们接入学习。

`methods/mapc/mapc_methods.h` 当前没有直接包含者，`selectMapcPeer()` 为空实现，`calculateCoSrPower()` 仅声明。它是官方已有占位文件。若后续将协作策略抽到该目录，须明确迁移真实实现、调用关系和测试，不能按文件名假定已有能力。

## 当前每 AP 只读观测

`structures/ap_observation.h` 提供每个 AP 独占的 JSONL 写入器、序号、有效标记和该 AP 已收到的 ICR 报告缓存；`methods/node/node_observation_methods.h` 是其 Node 绑定实现片段，仅由 node_impl.h 接入。没有可变全局协作状态或全网状态表。不得通过其他 AP 组件、全局队列镜像或未来到达计算观测，也不在写日志时调用随机数或插入仿真事件。契约见 BASIC_NATIVE_COTDMA.md。
