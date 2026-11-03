# 当前逐文件来源与修改清单

此文件由 `make inventory` 生成，禁止手工编辑。`make inventory-check` 检查是否过期。

固定官方参照：`a40896d881bfb377c07a9ccef249cdab52849445`；只对该历史参照判断，非最新版本声明，非本地 checkout 祖先声明。

文件身份与功能归属分开：官方文件内可有本地修改；本地修改不表示整份算法由我们实现。每个文件的当前/官方/导入 SHA256 和修改层次见 [机器清单](../provenance/file_inventory.json)。

## 原生源码统计

|身份|文件数|
|---|---:|
|本地新增原生文件|8|
|官方原有，本地修改|21|
|官方原有，当前未改|58|

统计仅包含 Code 下非生成 `.h/.cc/.cpp/.l`。构建文件、工具、配置、Python、测试与文档另列。

## 官方原有、本地修改 / 本地新增原生文件

|文件|身份|改动层次|当前说明|
|---|---|---|---|
|[Code/list_of_macros.h](<../Code/list_of_macros.h>)|官方原有，本地修改|simulator-fix → paper-dl|保留官方常量；本地新增传播/诊断及 DL CF-End 相关标识。|
|[Code/main/komondor_main.cc](<../Code/main/komondor_main.cc>)|官方原有，本地修改|simulator-fix → 独立化后|官方入口；本地在 Setup 前设置 COST 随机种子。 新增 --state-dir，仅原生规则运行启用独立 AP 状态文件；拒绝覆盖目录；清理继承开关并捕获原生异常。|
|[Code/main/node.h](<../Code/main/node.h>)|官方原有，本地修改|simulator-fix → tdma-online → paper-dl → 独立化后|官方 Node 组件；本地接入事务账本、回放、多 AC、轮次 CF-End 及诊断状态。 后续加入独立 AP 观测器、满缓存替换计数、按组 peer 轮询计数和动态 peer 列表；初始化空目的 ID。|
|[Code/main/traffic_generator.h](<../Code/main/traffic_generator.h>)|官方原有，本地修改|tdma-online → paper-dl → 独立化后|官方业务生成组件；本地接入在线 trace 和工程应用回放/分片。 后续修正原生固定间隔流量的整数除法零时间循环，使用 1.0/traffic_load；保留原流量模型选择。|
|[Code/methods/frames/notification_methods.h](<../Code/methods/frames/notification_methods.h>)|官方原有，本地修改|simulator-fix → tdma-online → 独立化后|官方通知构造；本地补充真实总功率、MPDU/在线预算等元数据。 后续初始化 BF 关闭时的 beam_use_zf 字段，避免未初始化诊断值。|
|[Code/methods/mac/channel_access/backoff_methods.h](<../Code/methods/mac/channel_access/backoff_methods.h>)|官方原有，本地修改|simulator-fix|官方退避算法；本地去除 ML_ACCESS 占位引用/分支。|
|[Code/methods/mac/packet_loss_methods.h](<../Code/methods/mac/packet_loss_methods.h>)|官方原有，本地修改|paper-dl|官方接收判决；本地扩展 DL CF-End 等新增帧接收路径。|
|[Code/methods/node/node_backoff_methods.h](<../Code/methods/node/node_backoff_methods.h>)|官方原有，本地修改|simulator-fix → tdma-online → paper-dl → 独立化后|官方竞争驱动；本地 NAV/在线选择、多 AC 退避及 CF-End 轮次处理。 后续在重启边界导出本 AP 状态，不引入新计时器或随机抽样。|
|[Code/methods/node/node_dl_cfend_methods.h](<../Code/methods/node/node_dl_cfend_methods.h>)|本地新增原生文件|paper-dl|本地新增：Node 绑定的 CF-End 发送/接收/超时与轮次 NAV 实现片段。|
|[Code/methods/node/node_fsm_methods.h](<../Code/methods/node/node_fsm_methods.h>)|官方原有，本地修改|simulator-fix → tdma-online → paper-dl → 独立化后|官方三模式状态机；本地控制帧解码、干扰状态、时分回退、NAV 及 DL 轮次保护。 后续修正 Co-TDMA 单目标 ICR 不按组内位置额外延迟，并按组推进 peer 轮询。|
|[Code/methods/node/node_impl.h](<../Code/methods/node/node_impl.h>)|官方原有，本地修改|paper-dl → 独立化后|官方 Node 聚合入口；本地追加 node_dl_cfend_methods.h。 后续追加独立的 node_observation_methods.h，仍只在 Node 定义后聚合一次。|
|[Code/methods/node/node_mcs_methods.h](<../Code/methods/node/node_mcs_methods.h>)|官方原有，本地修改|simulator-fix → tdma-online → paper-dl → 独立化后|官方 MCS/业务到达事件；本地回放、分片、MPDU/队列及观测接入。|
|[Code/methods/node/node_observation_methods.h](<../Code/methods/node/node_observation_methods.h>)|本地新增原生文件|—|本地新增 Node 实现片段：从本 AP 队列、计时器、竞争/PHY/协作状态生成只读快照；对端信息仅使用成功收到的 ICR has_data，不访问其他 AP 状态。|
|[Code/methods/node/node_packet_methods.h](<../Code/methods/node/node_packet_methods.h>)|官方原有，本地修改|simulator-fix → tdma-online → paper-dl → 独立化后|官方交换/发送驱动及 Co-SR 功率策略；本地 MPDU 事务、回退传播、功率/零陷保护、DL 共享预算和带宽检查。官方已有 ComputeCoSRTxPowers，不算本地新增算法。 后续补充原生只读观测事件、满缓存补包计数、保留队列的实际聚合上限、按组对端轮询/动态 peer 容量、ICR 来源校验；原生单包无法容纳时规则独占回退或保留重试，禁止越预算。|
|[Code/methods/node/node_statistics_methods.h](<../Code/methods/node/node_statistics_methods.h>)|官方原有，本地修改|simulator-fix → tdma-online → 独立化后|官方节点报告；本地守恒、逐 MPDU/回放统计与诊断。 后续将满缓存源刷新替换与溢出分开统计，补齐 generated，统一满缓存守恒检查。|
|[Code/methods/node/node_timeout_methods.h](<../Code/methods/node/node_timeout_methods.h>)|官方原有，本地修改|tdma-online → paper-dl → 独立化后|官方超时驱动；本地在线/回放状态与 DL CF-End 超时处理。 后续在 ACK/CTS/DATA/NAV 超时边界导出本 AP 状态，不改变超时策略。|
|[Code/methods/phy/beamforming_methods.h](<../Code/methods/phy/beamforming_methods.h>)|官方原有，本地修改|simulator-fix|官方 ULA/ZF 数值方法；本地期望/零陷退化保护，尚未修复矩阵步长问题。|
|[Code/methods/phy/power_channel_methods.h](<../Code/methods/phy/power_channel_methods.h>)|官方原有，本地修改|simulator-fix|官方功率/传播/干扰；本地 TGn D model 13、实际发射功率缓存刷新等修复。|
|[Code/methods/utils/input_methods/input_loader.h](<../Code/methods/utils/input_methods/input_loader.h>)|官方原有，本地修改|独立化后|官方输入加载器；本地新增 MAPC 组 ID 唯一性、未知/重复 BSS、至少两成员及每 BSS 组容量校验，防止零 peer 取模和静默残缺组。未改写官方 CSV。|
|[Code/methods/utils/output_generation_methods.h](<../Code/methods/utils/output_generation_methods.h>)|官方原有，本地修改|simulator-fix|官方 KPI 输出；本地稳定统计口径、除零/无样本保护和 Jain 分母修正。|
|[Code/structures/FIFO.h](<../Code/structures/FIFO.h>)|官方原有，本地修改|tdma-online → paper-dl|官方队列；本地按目的/AC 查询及事务/回放所需操作。|
|[Code/structures/ap_observation.h](<../Code/structures/ap_observation.h>)|本地新增原生文件|—|本地新增：每个 AP 独占的 JSONL 写入器、序号及本地收到的 ICR 报告缓存；无全局状态表、无随机数/调度控制，提供 JSON 转义和非有限数保护。|
|[Code/structures/dl_cfend.h](<../Code/structures/dl_cfend.h>)|本地新增原生文件|paper-dl|本地新增：轮次身份、预约、合法释放及预算可容纳性辅助方法。|
|[Code/structures/mpdu_transaction.h](<../Code/structures/mpdu_transaction.h>)|本地新增原生文件|simulator-fix|本地新增：实际 DATA MPDU 事务、ACK 身份/前缀、重复及守恒保护。|
|[Code/structures/notification.h](<../Code/structures/notification.h>)|官方原有，本地修改|simulator-fix → tdma-online → paper-dl|官方 Notification/TxInfo；本地 MPDU/预算、功率元数据、时分回退标记与 DL 轮次字段。|
|[Code/structures/paper_edca.h](<../Code/structures/paper_edca.h>)|本地新增原生文件|paper-dl|本地新增：DL replay 多 AC 虚拟退避与内部碰撞。|
|[Code/structures/paper_replay.h](<../Code/structures/paper_replay.h>)|本地新增原生文件|paper-dl|本地新增：确定性工程应用 trace、分片、DL 规则和队列容量配置。|
|[Code/structures/simulation_stats.h](<../Code/structures/simulation_stats.h>)|官方原有，本地修改|simulator-fix|官方系统统计；本地无吞吐/无时延样本与 KPI 稳定性处理。|
|[Code/structures/tdma_pilot.h](<../Code/structures/tdma_pilot.h>)|本地新增原生文件|tdma-online|本地新增：早期在线 TDMA 计划、队列/速率观测、STA/预算选择及计数。|

层次来自冻结的逐层比较；不是提交历史，也不是行级作者认定。完整已有修改见 [CHANGES.md](CHANGES.md)。

## 官方原有、当前源码与参照相同

|文件|身份|
|---|---|
|[Code/COST/config.h](<../Code/COST/config.h>)|官方原有，当前未改|
|[Code/COST/corsa_alloc.h](<../Code/COST/corsa_alloc.h>)|官方原有，当前未改|
|[Code/COST/cost.h](<../Code/COST/cost.h>)|官方原有，当前未改|
|[Code/COST/ether_addr.h](<../Code/COST/ether_addr.h>)|官方原有，当前未改|
|[Code/COST/path.h](<../Code/COST/path.h>)|官方原有，当前未改|
|[Code/COST/priority_q.h](<../Code/COST/priority_q.h>)|官方原有，当前未改|
|[Code/COST/sense.h](<../Code/COST/sense.h>)|官方原有，当前未改|
|[Code/compcpp/lexer.l](<../Code/compcpp/lexer.l>)|官方原有，当前未改|
|[Code/compcpp/parser.cpp](<../Code/compcpp/parser.cpp>)|官方原有，当前未改|
|[Code/compcpp/token.cpp](<../Code/compcpp/token.cpp>)|官方原有，当前未改|
|[Code/compcpp/token.h](<../Code/compcpp/token.h>)|官方原有，当前未改|
|[Code/learning_modules/action_space.h](<../Code/learning_modules/action_space.h>)|官方原有，当前未改|
|[Code/learning_modules/agent_methods.h](<../Code/learning_modules/agent_methods.h>)|官方原有，当前未改|
|[Code/learning_modules/external_model_client.h](<../Code/learning_modules/external_model_client.h>)|官方原有，当前未改|
|[Code/learning_modules/learning_algorithm.h](<../Code/learning_modules/learning_algorithm.h>)|官方原有，当前未改|
|[Code/learning_modules/ml_model.h](<../Code/learning_modules/ml_model.h>)|官方原有，当前未改|
|[Code/learning_modules/network_optimization_methods/centralized_action_banning.h](<../Code/learning_modules/network_optimization_methods/centralized_action_banning.h>)|官方原有，当前未改|
|[Code/learning_modules/network_optimization_methods/multi_armed_bandits.h](<../Code/learning_modules/network_optimization_methods/multi_armed_bandits.h>)|官方原有，当前未改|
|[Code/learning_modules/network_optimization_methods/rtot_algorithm.h](<../Code/learning_modules/network_optimization_methods/rtot_algorithm.h>)|官方原有，当前未改|
|[Code/learning_modules/pre_processor.h](<../Code/learning_modules/pre_processor.h>)|官方原有，当前未改|
|[Code/learning_modules/reward_function.h](<../Code/learning_modules/reward_function.h>)|官方原有，当前未改|
|[Code/main/agent.h](<../Code/main/agent.h>)|官方原有，当前未改|
|[Code/main/central_controller.h](<../Code/main/central_controller.h>)|官方原有，当前未改|
|[Code/methods/frames/packet_aggregation_methods.h](<../Code/methods/frames/packet_aggregation_methods.h>)|官方原有，当前未改|
|[Code/methods/mac/channel_access/channel_access_methods.h](<../Code/methods/mac/channel_access/channel_access_methods.h>)|官方原有，当前未改|
|[Code/methods/mac/channel_access/dcf_methods.h](<../Code/methods/mac/channel_access/dcf_methods.h>)|官方原有，当前未改|
|[Code/methods/mac/channel_access/deterministic_backoff_methods.h](<../Code/methods/mac/channel_access/deterministic_backoff_methods.h>)|官方原有，当前未改|
|[Code/methods/mac/channel_access/eca_methods.h](<../Code/methods/mac/channel_access/eca_methods.h>)|官方原有，当前未改|
|[Code/methods/mac/channel_access/edca_methods.h](<../Code/methods/mac/channel_access/edca_methods.h>)|官方原有，当前未改|
|[Code/methods/mac/channel_access/tokenized_backoff_methods.h](<../Code/methods/mac/channel_access/tokenized_backoff_methods.h>)|官方原有，当前未改|
|[Code/methods/mac/nack_methods.h](<../Code/methods/mac/nack_methods.h>)|官方原有，当前未改|
|[Code/methods/mac/nav_methods.h](<../Code/methods/mac/nav_methods.h>)|官方原有，当前未改|
|[Code/methods/mac/spatial_reuse_methods.h](<../Code/methods/mac/spatial_reuse_methods.h>)|官方原有，当前未改|
|[Code/methods/mapc/mapc_methods.h](<../Code/methods/mapc/mapc_methods.h>)|官方原有，当前未改|
|[Code/methods/node/node_config_methods.h](<../Code/methods/node/node_config_methods.h>)|官方原有，当前未改|
|[Code/methods/node/node_spatial_reuse_methods.h](<../Code/methods/node/node_spatial_reuse_methods.h>)|官方原有，当前未改|
|[Code/methods/phy/channel_bonding_methods.h](<../Code/methods/phy/channel_bonding_methods.h>)|官方原有，当前未改|
|[Code/methods/phy/frame_duration_methods.h](<../Code/methods/phy/frame_duration_methods.h>)|官方原有，当前未改|
|[Code/methods/phy/mcs_methods.h](<../Code/methods/phy/mcs_methods.h>)|官方原有，当前未改|
|[Code/methods/utils/auxiliary_methods.h](<../Code/methods/utils/auxiliary_methods.h>)|官方原有，当前未改|
|[Code/methods/utils/input_methods/input_validator.h](<../Code/methods/utils/input_methods/input_validator.h>)|官方原有，当前未改|
|[Code/methods/utils/logging_methods.h](<../Code/methods/utils/logging_methods.h>)|官方原有，当前未改|
|[Code/methods/utils/print_and_write_methods.h](<../Code/methods/utils/print_and_write_methods.h>)|官方原有，当前未改|
|[Code/structures/action.h](<../Code/structures/action.h>)|官方原有，当前未改|
|[Code/structures/agent_capabilities.h](<../Code/structures/agent_capabilities.h>)|官方原有，当前未改|
|[Code/structures/channel_access_state.h](<../Code/structures/channel_access_state.h>)|官方原有，当前未改|
|[Code/structures/controller_report.h](<../Code/structures/controller_report.h>)|官方原有，当前未改|
|[Code/structures/logger.h](<../Code/structures/logger.h>)|官方原有，当前未改|
|[Code/structures/logical_nack.h](<../Code/structures/logical_nack.h>)|官方原有，当前未改|
|[Code/structures/modulations.h](<../Code/structures/modulations.h>)|官方原有，当前未改|
|[Code/structures/node_configuration.h](<../Code/structures/node_configuration.h>)|官方原有，当前未改|
|[Code/structures/node_parameters.h](<../Code/structures/node_parameters.h>)|官方原有，当前未改|
|[Code/structures/node_statistics.h](<../Code/structures/node_statistics.h>)|官方原有，当前未改|
|[Code/structures/packet_exchange_sequence.h](<../Code/structures/packet_exchange_sequence.h>)|官方原有，当前未改|
|[Code/structures/performance.h](<../Code/structures/performance.h>)|官方原有，当前未改|
|[Code/structures/sim_configuration.h](<../Code/structures/sim_configuration.h>)|官方原有，当前未改|
|[Code/structures/spatial_reuse_state.h](<../Code/structures/spatial_reuse_state.h>)|官方原有，当前未改|
|[Code/structures/wlan.h](<../Code/structures/wlan.h>)|官方原有，当前未改|

## 官方配套材料及本地构建、工具、配置、测试与文档

|文件|身份|说明|
|---|---|---|
|[.cproject](<../.cproject>)|官方原有，当前未改||
|[.gitattributes](<../.gitattributes>)|官方原有，当前未改||
|[.gitignore](<../.gitignore>)|官方原有，本地修改|官方已有文件，本地按实际生成物重写：仅排除根 runs/build、原生/生成器构建产物、输出和缓存；移除重复与过宽规则，保留官方 Apps JAR/class/properties、嵌套 build 资源及 provenance。|
|[.settings/language.settings.xml](<../.settings/language.settings.xml>)|官方原有，当前未改||
|[AGENTS.md](<../AGENTS.md>)|本地工程/测试/文档||
|[Apps/BeamPlotting/input_mapc_cobf_1.csv](<../Apps/BeamPlotting/input_mapc_cobf_1.csv>)|官方原有，当前未改||
|[Apps/BeamPlotting/input_nodes_bf_standalone.csv](<../Apps/BeamPlotting/input_nodes_bf_standalone.csv>)|官方原有，当前未改||
|[Apps/BeamPlotting/input_nodes_cobf.csv](<../Apps/BeamPlotting/input_nodes_cobf.csv>)|官方原有，当前未改||
|[Apps/BeamPlotting/input_nodes_cobf_2.csv](<../Apps/BeamPlotting/input_nodes_cobf_2.csv>)|官方原有，当前未改||
|[Apps/BeamPlotting/plot_beams.py](<../Apps/BeamPlotting/plot_beams.py>)|官方原有，当前未改||
|[Apps/BeamPlotting/test_fig.png](<../Apps/BeamPlotting/test_fig.png>)|官方原有，当前未改||
|[Apps/BeamPlotting/test_fig1.png](<../Apps/BeamPlotting/test_fig1.png>)|官方原有，当前未改||
|[Apps/BeamPlotting/test_fig2.png](<../Apps/BeamPlotting/test_fig2.png>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/build/classes/.netbeans_automatic_build](<../Apps/CentralNodePostProcessing/build/classes/.netbeans_automatic_build>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/build/classes/.netbeans_update_resources](<../Apps/CentralNodePostProcessing/build/classes/.netbeans_update_resources>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/build/classes/KomondorLog.class](<../Apps/CentralNodePostProcessing/build/classes/KomondorLog.class>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/build/classes/Metrics.class](<../Apps/CentralNodePostProcessing/build/classes/Metrics.class>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/build/classes/Post_processer.class](<../Apps/CentralNodePostProcessing/build/classes/Post_processer.class>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/build/classes/Post_processer_primary.class](<../Apps/CentralNodePostProcessing/build/classes/Post_processer_primary.class>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/build/classes/Statistic.class](<../Apps/CentralNodePostProcessing/build/classes/Statistic.class>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/build.xml](<../Apps/CentralNodePostProcessing/build.xml>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/delay_comparison.csv](<../Apps/CentralNodePostProcessing/delay_comparison.csv>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/delay_comparison_sim.csv](<../Apps/CentralNodePostProcessing/delay_comparison_sim.csv>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/delay_load_similar.csv](<../Apps/CentralNodePostProcessing/delay_load_similar.csv>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/generated_info.csv](<../Apps/CentralNodePostProcessing/generated_info.csv>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/CHANGES](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/CHANGES>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/COPYING](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/COPYING>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/README](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/README>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/README.txt](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/README.txt>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/build.xml](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/build.xml>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/mysql-connector-java-5.1.46-bin.jar](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/mysql-connector-java-5.1.46-bin.jar>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/mysql-connector-java-5.1.46.jar](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/mysql-connector-java-5.1.46.jar>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/FabricCommunicationException.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/FabricCommunicationException.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/FabricConnection.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/FabricConnection.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/FabricStateResponse.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/FabricStateResponse.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/HashShardMapping.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/HashShardMapping.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/RangeShardMapping.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/RangeShardMapping.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/Response.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/Response.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/Server.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/Server.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/ServerGroup.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/ServerGroup.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/ServerMode.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/ServerMode.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/ServerRole.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/ServerRole.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/ShardIndex.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/ShardIndex.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/ShardMapping.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/ShardMapping.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/ShardMappingFactory.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/ShardMappingFactory.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/ShardTable.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/ShardTable.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/ShardingType.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/ShardingType.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/hibernate/FabricMultiTenantConnectionProvider.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/hibernate/FabricMultiTenantConnectionProvider.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/jdbc/ErrorReportingExceptionInterceptor.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/jdbc/ErrorReportingExceptionInterceptor.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/jdbc/FabricMySQLConnection.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/jdbc/FabricMySQLConnection.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/jdbc/FabricMySQLConnectionProperties.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/jdbc/FabricMySQLConnectionProperties.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/jdbc/FabricMySQLConnectionProxy.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/jdbc/FabricMySQLConnectionProxy.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/jdbc/FabricMySQLDataSource.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/jdbc/FabricMySQLDataSource.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/jdbc/FabricMySQLDriver.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/jdbc/FabricMySQLDriver.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/jdbc/JDBC4FabricMySQLConnection.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/jdbc/JDBC4FabricMySQLConnection.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/jdbc/JDBC4FabricMySQLConnectionProxy.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/jdbc/JDBC4FabricMySQLConnectionProxy.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/proto/xmlrpc/AuthenticatedXmlRpcMethodCaller.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/proto/xmlrpc/AuthenticatedXmlRpcMethodCaller.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/proto/xmlrpc/DigestAuthentication.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/proto/xmlrpc/DigestAuthentication.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/proto/xmlrpc/InternalXmlRpcMethodCaller.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/proto/xmlrpc/InternalXmlRpcMethodCaller.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/proto/xmlrpc/ResultSetParser.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/proto/xmlrpc/ResultSetParser.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/proto/xmlrpc/XmlRpcClient.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/proto/xmlrpc/XmlRpcClient.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/proto/xmlrpc/XmlRpcMethodCaller.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/proto/xmlrpc/XmlRpcMethodCaller.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/xmlrpc/Client.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/xmlrpc/Client.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/xmlrpc/base/Array.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/xmlrpc/base/Array.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/xmlrpc/base/Data.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/xmlrpc/base/Data.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/xmlrpc/base/Fault.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/xmlrpc/base/Fault.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/xmlrpc/base/Member.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/xmlrpc/base/Member.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/xmlrpc/base/MethodCall.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/xmlrpc/base/MethodCall.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/xmlrpc/base/MethodResponse.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/xmlrpc/base/MethodResponse.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/xmlrpc/base/Param.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/xmlrpc/base/Param.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/xmlrpc/base/Params.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/xmlrpc/base/Params.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/xmlrpc/base/ResponseParser.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/xmlrpc/base/ResponseParser.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/xmlrpc/base/Struct.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/xmlrpc/base/Struct.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/xmlrpc/base/Value.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/xmlrpc/base/Value.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/xmlrpc/exceptions/MySQLFabricException.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/fabric/xmlrpc/exceptions/MySQLFabricException.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/AbandonedConnectionCleanupThread.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/AbandonedConnectionCleanupThread.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/AssertionFailedException.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/AssertionFailedException.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/AuthenticationPlugin.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/AuthenticationPlugin.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/BalanceStrategy.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/BalanceStrategy.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/BestResponseTimeBalanceStrategy.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/BestResponseTimeBalanceStrategy.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/Blob.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/Blob.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/BlobFromLocator.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/BlobFromLocator.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/Buffer.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/Buffer.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/BufferRow.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/BufferRow.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ByteArrayRow.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ByteArrayRow.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/CacheAdapter.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/CacheAdapter.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/CacheAdapterFactory.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/CacheAdapterFactory.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/CachedResultSetMetaData.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/CachedResultSetMetaData.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/CallableStatement.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/CallableStatement.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/CharsetMapping.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/CharsetMapping.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/Charsets.properties](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/Charsets.properties>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/Clob.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/Clob.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/CommunicationsException.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/CommunicationsException.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/CompressedInputStream.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/CompressedInputStream.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/Connection.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/Connection.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ConnectionFeatureNotAvailableException.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ConnectionFeatureNotAvailableException.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ConnectionGroup.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ConnectionGroup.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ConnectionGroupManager.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ConnectionGroupManager.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ConnectionImpl.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ConnectionImpl.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ConnectionLifecycleInterceptor.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ConnectionLifecycleInterceptor.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ConnectionProperties.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ConnectionProperties.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ConnectionPropertiesImpl.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ConnectionPropertiesImpl.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ConnectionPropertiesTransform.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ConnectionPropertiesTransform.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/Constants.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/Constants.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/DatabaseMetaData.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/DatabaseMetaData.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/DatabaseMetaDataUsingInfoSchema.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/DatabaseMetaDataUsingInfoSchema.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/DocsConnectionPropsHelper.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/DocsConnectionPropsHelper.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/Driver.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/Driver.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/EscapeProcessor.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/EscapeProcessor.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/EscapeProcessorResult.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/EscapeProcessorResult.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/EscapeTokenizer.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/EscapeTokenizer.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ExceptionInterceptor.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ExceptionInterceptor.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ExportControlled.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ExportControlled.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/Extension.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/Extension.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/FailoverConnectionProxy.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/FailoverConnectionProxy.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/Field.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/Field.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/IterateBlock.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/IterateBlock.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC42CallableStatement.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC42CallableStatement.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC42Helper.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC42Helper.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC42PreparedStatement.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC42PreparedStatement.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC42ResultSet.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC42ResultSet.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC42ServerPreparedStatement.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC42ServerPreparedStatement.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC42UpdatableResultSet.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC42UpdatableResultSet.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4CallableStatement.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4CallableStatement.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4ClientInfoProvider.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4ClientInfoProvider.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4ClientInfoProviderSP.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4ClientInfoProviderSP.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4CommentClientInfoProvider.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4CommentClientInfoProvider.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4Connection.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4Connection.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4DatabaseMetaData.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4DatabaseMetaData.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4DatabaseMetaDataUsingInfoSchema.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4DatabaseMetaDataUsingInfoSchema.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4LoadBalancedMySQLConnection.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4LoadBalancedMySQLConnection.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4MultiHostMySQLConnection.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4MultiHostMySQLConnection.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4MySQLConnection.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4MySQLConnection.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4MysqlSQLXML.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4MysqlSQLXML.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4NClob.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4NClob.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4PreparedStatement.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4PreparedStatement.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4PreparedStatementHelper.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4PreparedStatementHelper.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4ReplicationMySQLConnection.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4ReplicationMySQLConnection.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4ResultSet.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4ResultSet.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4ServerPreparedStatement.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4ServerPreparedStatement.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4UpdatableResultSet.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/JDBC4UpdatableResultSet.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/LicenseConfiguration.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/LicenseConfiguration.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/LoadBalanceExceptionChecker.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/LoadBalanceExceptionChecker.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/LoadBalancedAutoCommitInterceptor.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/LoadBalancedAutoCommitInterceptor.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/LoadBalancedConnection.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/LoadBalancedConnection.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/LoadBalancedConnectionProxy.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/LoadBalancedConnectionProxy.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/LoadBalancedMySQLConnection.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/LoadBalancedMySQLConnection.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/LocalizedErrorMessages.properties](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/LocalizedErrorMessages.properties>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/Messages.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/Messages.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/MiniAdmin.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/MiniAdmin.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/MultiHostConnectionProxy.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/MultiHostConnectionProxy.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/MultiHostMySQLConnection.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/MultiHostMySQLConnection.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/MySQLConnection.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/MySQLConnection.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/MysqlDataTruncation.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/MysqlDataTruncation.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/MysqlDefs.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/MysqlDefs.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/MysqlErrorNumbers.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/MysqlErrorNumbers.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/MysqlIO.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/MysqlIO.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/MysqlParameterMetadata.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/MysqlParameterMetadata.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/MysqlSavepoint.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/MysqlSavepoint.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/NamedPipeSocketFactory.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/NamedPipeSocketFactory.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/NdbLoadBalanceExceptionChecker.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/NdbLoadBalanceExceptionChecker.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/NetworkResources.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/NetworkResources.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/NoSubInterceptorWrapper.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/NoSubInterceptorWrapper.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/NonRegisteringDriver.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/NonRegisteringDriver.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/NonRegisteringReplicationDriver.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/NonRegisteringReplicationDriver.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/NotImplemented.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/NotImplemented.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/NotUpdatable.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/NotUpdatable.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/OperationNotSupportedException.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/OperationNotSupportedException.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/OutputStreamWatcher.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/OutputStreamWatcher.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/PacketTooBigException.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/PacketTooBigException.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ParameterBindings.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ParameterBindings.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/PerConnectionLRUFactory.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/PerConnectionLRUFactory.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/PerVmServerConfigCacheFactory.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/PerVmServerConfigCacheFactory.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/PingTarget.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/PingTarget.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/PreparedStatement.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/PreparedStatement.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ProfilerEventHandlerFactory.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ProfilerEventHandlerFactory.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/RandomBalanceStrategy.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/RandomBalanceStrategy.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ReflectiveStatementInterceptorAdapter.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ReflectiveStatementInterceptorAdapter.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ReplicationConnection.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ReplicationConnection.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ReplicationConnectionGroup.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ReplicationConnectionGroup.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ReplicationConnectionGroupManager.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ReplicationConnectionGroupManager.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ReplicationConnectionProxy.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ReplicationConnectionProxy.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ReplicationDriver.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ReplicationDriver.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ReplicationMySQLConnection.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ReplicationMySQLConnection.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ResultSetImpl.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ResultSetImpl.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ResultSetInternalMethods.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ResultSetInternalMethods.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ResultSetMetaData.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ResultSetMetaData.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ResultSetRow.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ResultSetRow.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/RowData.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/RowData.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/RowDataCursor.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/RowDataCursor.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/RowDataDynamic.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/RowDataDynamic.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/RowDataStatic.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/RowDataStatic.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/SQLError.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/SQLError.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/Security.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/Security.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/SequentialBalanceStrategy.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/SequentialBalanceStrategy.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ServerAffinityStrategy.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ServerAffinityStrategy.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ServerPreparedStatement.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/ServerPreparedStatement.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/SingleByteCharsetConverter.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/SingleByteCharsetConverter.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/SocketFactory.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/SocketFactory.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/SocketMetadata.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/SocketMetadata.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/SocksProxySocketFactory.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/SocksProxySocketFactory.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/StandardLoadBalanceExceptionChecker.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/StandardLoadBalanceExceptionChecker.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/StandardSocketFactory.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/StandardSocketFactory.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/Statement.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/Statement.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/StatementImpl.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/StatementImpl.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/StatementInterceptor.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/StatementInterceptor.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/StatementInterceptorV2.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/StatementInterceptorV2.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/StreamingNotifiable.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/StreamingNotifiable.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/StringUtils.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/StringUtils.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/TimeUtil.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/TimeUtil.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/TimeZoneMapping.properties](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/TimeZoneMapping.properties>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/UpdatableResultSet.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/UpdatableResultSet.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/Util.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/Util.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/V1toV2StatementInterceptorAdapter.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/V1toV2StatementInterceptorAdapter.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/WatchableOutputStream.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/WatchableOutputStream.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/WatchableWriter.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/WatchableWriter.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/Wrapper.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/Wrapper.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/WriterWatcher.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/WriterWatcher.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/authentication/CachingSha2PasswordPlugin.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/authentication/CachingSha2PasswordPlugin.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/authentication/MysqlClearPasswordPlugin.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/authentication/MysqlClearPasswordPlugin.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/authentication/MysqlNativePasswordPlugin.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/authentication/MysqlNativePasswordPlugin.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/authentication/MysqlOldPasswordPlugin.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/authentication/MysqlOldPasswordPlugin.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/authentication/Sha256PasswordPlugin.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/authentication/Sha256PasswordPlugin.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/configs/3-0-Compat.properties](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/configs/3-0-Compat.properties>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/configs/5-0-Compat.properties](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/configs/5-0-Compat.properties>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/configs/clusterBase.properties](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/configs/clusterBase.properties>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/configs/coldFusion.properties](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/configs/coldFusion.properties>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/configs/fullDebug.properties](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/configs/fullDebug.properties>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/configs/maxPerformance.properties](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/configs/maxPerformance.properties>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/configs/solarisMaxPerformance.properties](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/configs/solarisMaxPerformance.properties>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/DeadlockTimeoutRollbackMarker.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/DeadlockTimeoutRollbackMarker.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/MySQLDataException.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/MySQLDataException.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/MySQLIntegrityConstraintViolationException.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/MySQLIntegrityConstraintViolationException.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/MySQLInvalidAuthorizationSpecException.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/MySQLInvalidAuthorizationSpecException.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/MySQLNonTransientConnectionException.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/MySQLNonTransientConnectionException.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/MySQLNonTransientException.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/MySQLNonTransientException.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/MySQLQueryInterruptedException.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/MySQLQueryInterruptedException.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/MySQLStatementCancelledException.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/MySQLStatementCancelledException.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/MySQLSyntaxErrorException.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/MySQLSyntaxErrorException.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/MySQLTimeoutException.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/MySQLTimeoutException.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/MySQLTransactionRollbackException.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/MySQLTransactionRollbackException.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/MySQLTransientConnectionException.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/MySQLTransientConnectionException.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/MySQLTransientException.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/MySQLTransientException.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/jdbc4/CommunicationsException.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/jdbc4/CommunicationsException.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/jdbc4/MySQLDataException.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/jdbc4/MySQLDataException.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/jdbc4/MySQLIntegrityConstraintViolationException.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/jdbc4/MySQLIntegrityConstraintViolationException.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/jdbc4/MySQLInvalidAuthorizationSpecException.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/jdbc4/MySQLInvalidAuthorizationSpecException.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/jdbc4/MySQLNonTransientConnectionException.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/jdbc4/MySQLNonTransientConnectionException.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/jdbc4/MySQLNonTransientException.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/jdbc4/MySQLNonTransientException.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/jdbc4/MySQLQueryInterruptedException.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/jdbc4/MySQLQueryInterruptedException.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/jdbc4/MySQLSyntaxErrorException.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/jdbc4/MySQLSyntaxErrorException.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/jdbc4/MySQLTimeoutException.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/jdbc4/MySQLTimeoutException.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/jdbc4/MySQLTransactionRollbackException.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/jdbc4/MySQLTransactionRollbackException.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/jdbc4/MySQLTransientConnectionException.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/jdbc4/MySQLTransientConnectionException.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/jdbc4/MySQLTransientException.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/exceptions/jdbc4/MySQLTransientException.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/integration/c3p0/MysqlConnectionTester.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/integration/c3p0/MysqlConnectionTester.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/integration/jboss/ExtendedMysqlExceptionSorter.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/integration/jboss/ExtendedMysqlExceptionSorter.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/integration/jboss/MysqlValidConnectionChecker.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/integration/jboss/MysqlValidConnectionChecker.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/interceptors/ResultSetScannerInterceptor.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/interceptors/ResultSetScannerInterceptor.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/interceptors/ServerStatusDiffInterceptor.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/interceptors/ServerStatusDiffInterceptor.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/interceptors/SessionAssociationInterceptor.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/interceptors/SessionAssociationInterceptor.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/CallableStatementWrapper.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/CallableStatementWrapper.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/ConnectionWrapper.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/ConnectionWrapper.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/JDBC42CallableStatementWrapper.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/JDBC42CallableStatementWrapper.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/JDBC42PreparedStatementWrapper.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/JDBC42PreparedStatementWrapper.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/JDBC4CallableStatementWrapper.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/JDBC4CallableStatementWrapper.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/JDBC4ConnectionWrapper.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/JDBC4ConnectionWrapper.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/JDBC4MysqlPooledConnection.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/JDBC4MysqlPooledConnection.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/JDBC4MysqlXAConnection.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/JDBC4MysqlXAConnection.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/JDBC4PreparedStatementWrapper.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/JDBC4PreparedStatementWrapper.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/JDBC4StatementWrapper.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/JDBC4StatementWrapper.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/JDBC4SuspendableXAConnection.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/JDBC4SuspendableXAConnection.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/MysqlConnectionPoolDataSource.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/MysqlConnectionPoolDataSource.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/MysqlDataSource.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/MysqlDataSource.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/MysqlDataSourceFactory.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/MysqlDataSourceFactory.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/MysqlPooledConnection.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/MysqlPooledConnection.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/MysqlXAConnection.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/MysqlXAConnection.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/MysqlXADataSource.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/MysqlXADataSource.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/MysqlXAException.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/MysqlXAException.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/MysqlXid.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/MysqlXid.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/PreparedStatementWrapper.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/PreparedStatementWrapper.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/StatementWrapper.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/StatementWrapper.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/SuspendableXAConnection.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/SuspendableXAConnection.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/WrapperBase.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jdbc2/optional/WrapperBase.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jmx/LoadBalanceConnectionGroupManager.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jmx/LoadBalanceConnectionGroupManager.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jmx/LoadBalanceConnectionGroupManagerMBean.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jmx/LoadBalanceConnectionGroupManagerMBean.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jmx/ReplicationGroupManager.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jmx/ReplicationGroupManager.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jmx/ReplicationGroupManagerMBean.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/jmx/ReplicationGroupManagerMBean.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/log/Jdk14Logger.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/log/Jdk14Logger.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/log/Log.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/log/Log.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/log/LogFactory.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/log/LogFactory.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/log/LogUtils.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/log/LogUtils.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/log/NullLogger.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/log/NullLogger.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/log/Slf4JLogger.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/log/Slf4JLogger.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/log/StandardLogger.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/log/StandardLogger.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/profiler/LoggingProfilerEventHandler.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/profiler/LoggingProfilerEventHandler.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/profiler/ProfilerEvent.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/profiler/ProfilerEvent.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/profiler/ProfilerEventHandler.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/profiler/ProfilerEventHandler.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/util/Base64Decoder.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/util/Base64Decoder.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/util/BaseBugReport.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/util/BaseBugReport.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/util/ErrorMappingsDocGenerator.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/util/ErrorMappingsDocGenerator.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/util/LRUCache.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/util/LRUCache.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/util/PropertiesDocGenerator.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/util/PropertiesDocGenerator.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/util/ReadAheadInputStream.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/util/ReadAheadInputStream.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/util/ResultSetUtil.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/util/ResultSetUtil.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/util/ServerController.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/util/ServerController.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/util/TimezoneDump.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/util/TimezoneDump.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/util/VersionFSHierarchyMaker.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/com/mysql/jdbc/util/VersionFSHierarchyMaker.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/demo/fabric/Client1_Fabric.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/demo/fabric/Client1_Fabric.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/demo/fabric/Employee.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/demo/fabric/Employee.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/demo/fabric/EmployeesDataSource.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/demo/fabric/EmployeesDataSource.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/demo/fabric/EmployeesJdbc.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/demo/fabric/EmployeesJdbc.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/demo/fabric/HibernateFabric.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/demo/fabric/HibernateFabric.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/demo/fabric/resources/com/mysql/fabric/demo/employee.hbm.xml](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/demo/fabric/resources/com/mysql/fabric/demo/employee.hbm.xml>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/doc/sources/pom.xml](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/doc/sources/pom.xml>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/lib/c3p0-0.9.1-pre6.jar](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/lib/c3p0-0.9.1-pre6.jar>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/lib/c3p0-0.9.1-pre6.src.zip](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/lib/c3p0-0.9.1-pre6.src.zip>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/lib/jboss-common-jdbc-wrapper-src.jar](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/lib/jboss-common-jdbc-wrapper-src.jar>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/lib/jboss-common-jdbc-wrapper.jar](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/lib/jboss-common-jdbc-wrapper.jar>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/lib/slf4j-api-1.6.1.jar](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/lib/slf4j-api-1.6.1.jar>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/org/gjt/mm/mysql/Driver.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/org/gjt/mm/mysql/Driver.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/BaseStatementInterceptor.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/BaseStatementInterceptor.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/BaseTestCase.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/BaseTestCase.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/UnreliableSocketFactory.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/UnreliableSocketFactory.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/fabric/BaseFabricTestCase.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/fabric/BaseFabricTestCase.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/fabric/SetupFabricTestsuite.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/fabric/SetupFabricTestsuite.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/fabric/TestAdminCommands.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/fabric/TestAdminCommands.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/fabric/TestDumpCommands.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/fabric/TestDumpCommands.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/fabric/TestResultSetParser.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/fabric/TestResultSetParser.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/fabric/TestShardMapping.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/fabric/TestShardMapping.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/fabric/TestXmlRpcCore.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/fabric/TestXmlRpcCore.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/fabric/jdbc/TestBasicConnection.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/fabric/jdbc/TestBasicConnection.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/fabric/jdbc/TestFabricMySQLConnectionSharding.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/fabric/jdbc/TestFabricMySQLConnectionSharding.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/fabric/jdbc/TestHABasics.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/fabric/jdbc/TestHABasics.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/fabric/jdbc/TestHashSharding.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/fabric/jdbc/TestHashSharding.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/fabric/jdbc/TestRegressions.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/fabric/jdbc/TestRegressions.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/perf/BasePerfTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/perf/BasePerfTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/perf/LoadStorePerfTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/perf/LoadStorePerfTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/perf/RetrievalPerfTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/perf/RetrievalPerfTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/BlobRegressionTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/BlobRegressionTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/CachedRowsetTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/CachedRowsetTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/CallableStatementRegressionTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/CallableStatementRegressionTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/CharsetRegressionTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/CharsetRegressionTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/ConnectionRegressionTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/ConnectionRegressionTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/DataSourceRegressionTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/DataSourceRegressionTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/EscapeProcessorRegressionTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/EscapeProcessorRegressionTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/MetaDataRegressionTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/MetaDataRegressionTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/MicroPerformanceRegressionTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/MicroPerformanceRegressionTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/NumbersRegressionTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/NumbersRegressionTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/PooledConnectionRegressionTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/PooledConnectionRegressionTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/ResultSetRegressionTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/ResultSetRegressionTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/StatementRegressionTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/StatementRegressionTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/StressRegressionTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/StressRegressionTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/StringRegressionTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/StringRegressionTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/SubqueriesRegressionTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/SubqueriesRegressionTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/SyntaxRegressionTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/SyntaxRegressionTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/UtilsRegressionTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/UtilsRegressionTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/jdbc4/ConnectionRegressionTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/jdbc4/ConnectionRegressionTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/jdbc4/ExceptionSubclassesTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/jdbc4/ExceptionSubclassesTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/jdbc4/MetaDataRegressionTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/jdbc4/MetaDataRegressionTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/jdbc4/StatementRegressionTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/jdbc4/StatementRegressionTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/jdbc42/ConnectionRegressionTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/jdbc42/ConnectionRegressionTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/jdbc42/ResultSetRegressionTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/jdbc42/ResultSetRegressionTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/jdbc42/StatementRegressionTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/regression/jdbc42/StatementRegressionTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/BlobTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/BlobTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/CallableStatementTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/CallableStatementTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/CharsetTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/CharsetTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/ConnectionTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/ConnectionTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/DataSourceTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/DataSourceTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/DateTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/DateTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/EscapeProcessingTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/EscapeProcessingTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/MetadataTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/MetadataTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/MiniAdminTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/MiniAdminTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/MultiHostConnectionTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/MultiHostConnectionTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/NumbersTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/NumbersTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/ReadOnlyCallableStatementTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/ReadOnlyCallableStatementTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/ResultSetTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/ResultSetTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/SSLTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/SSLTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/ServerControllerTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/ServerControllerTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/SimpleTransformer.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/SimpleTransformer.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/SplitDBdotNameTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/SplitDBdotNameTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/StatementsTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/StatementsTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/StringUtilsTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/StringUtilsTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/TestBug57662Logger.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/TestBug57662Logger.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/TestLifecycleInterceptor.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/TestLifecycleInterceptor.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/TransactionTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/TransactionTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/TraversalTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/TraversalTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/UpdatabilityTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/UpdatabilityTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/UtilsTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/UtilsTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/XATest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/XATest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/jdbc4/StatementsTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/jdbc4/StatementsTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/jdbc42/ConnectionTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/jdbc42/ConnectionTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/jdbc42/ResultSetTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/jdbc42/ResultSetTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/jdbc42/StatementsTest.java](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/jdbc42/StatementsTest.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/tb2-data.txt.gz](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/simple/tb2-data.txt.gz>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/ssl-test-certs/ca-cert.pem](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/ssl-test-certs/ca-cert.pem>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/ssl-test-certs/ca-key.pem](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/ssl-test-certs/ca-key.pem>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/ssl-test-certs/ca-truststore](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/ssl-test-certs/ca-truststore>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/ssl-test-certs/certs_howto.txt](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/ssl-test-certs/certs_howto.txt>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/ssl-test-certs/client-cert.pem](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/ssl-test-certs/client-cert.pem>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/ssl-test-certs/client-key.pem](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/ssl-test-certs/client-key.pem>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/ssl-test-certs/client-keystore](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/ssl-test-certs/client-keystore>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/ssl-test-certs/mykey.pem](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/ssl-test-certs/mykey.pem>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/ssl-test-certs/mykey.pub](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/ssl-test-certs/mykey.pub>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/ssl-test-certs/server-cert.pem](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/ssl-test-certs/server-cert.pem>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/ssl-test-certs/server-key.pem](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46/src/testsuite/ssl-test-certs/server-key.pem>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46.tar](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46.tar>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46.tar.gz](<../Apps/CentralNodePostProcessing/lib/mysql-connector-java-5.1.46.tar.gz>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/manifest.mf](<../Apps/CentralNodePostProcessing/manifest.mf>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/mysql-connector-java-5.1.46-bin.jar](<../Apps/CentralNodePostProcessing/mysql-connector-java-5.1.46-bin.jar>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/nbproject/build-impl.xml](<../Apps/CentralNodePostProcessing/nbproject/build-impl.xml>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/nbproject/genfiles.properties](<../Apps/CentralNodePostProcessing/nbproject/genfiles.properties>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/nbproject/private/private.properties](<../Apps/CentralNodePostProcessing/nbproject/private/private.properties>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/nbproject/private/private.xml](<../Apps/CentralNodePostProcessing/nbproject/private/private.xml>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/nbproject/project.properties](<../Apps/CentralNodePostProcessing/nbproject/project.properties>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/nbproject/project.xml](<../Apps/CentralNodePostProcessing/nbproject/project.xml>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/primary_study.csv](<../Apps/CentralNodePostProcessing/primary_study.csv>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/primary_study_processed.xlsx](<../Apps/CentralNodePostProcessing/primary_study_processed.xlsx>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/probability_throughput_load_similar.csv](<../Apps/CentralNodePostProcessing/probability_throughput_load_similar.csv>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/script_output.txt](<../Apps/CentralNodePostProcessing/script_output.txt>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/script_output_am_saturation_5April2018.txt](<../Apps/CentralNodePostProcessing/script_output_am_saturation_5April2018.txt>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/script_output_am_saturation_6April2018.txt](<../Apps/CentralNodePostProcessing/script_output_am_saturation_6April2018.txt>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/script_output_first100.txt](<../Apps/CentralNodePostProcessing/script_output_first100.txt>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/script_output_last200.txt](<../Apps/CentralNodePostProcessing/script_output_last200.txt>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/src/KomondorLog.java](<../Apps/CentralNodePostProcessing/src/KomondorLog.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/src/Metrics.java](<../Apps/CentralNodePostProcessing/src/Metrics.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/src/Post_processer.java](<../Apps/CentralNodePostProcessing/src/Post_processer.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/src/Post_processer_primary.java](<../Apps/CentralNodePostProcessing/src/Post_processer_primary.java>)|官方原有，当前未改||
|[Apps/CentralNodePostProcessing/src/Statistic.java](<../Apps/CentralNodePostProcessing/src/Statistic.java>)|官方原有，当前未改||
|[Apps/Input Generation/agents.csv](<../Apps/Input%20Generation/agents.csv>)|官方原有，当前未改||
|[Apps/Input Generation/appended_file.csv](<../Apps/Input%20Generation/appended_file.csv>)|官方原有，当前未改||
|[Apps/Input Generation/build/classes/Wlan.class](<../Apps/Input%20Generation/build/classes/Wlan.class>)|官方原有，当前未改||
|[Apps/Input Generation/build/classes/toy_scenario_3_brute_force.class](<../Apps/Input%20Generation/build/classes/toy_scenario_3_brute_force.class>)|官方原有，当前未改||
|[Apps/Input Generation/build.xml](<../Apps/Input%20Generation/build.xml>)|官方原有，当前未改||
|[Apps/Input Generation/input.txt](<../Apps/Input%20Generation/input.txt>)|官方原有，当前未改||
|[Apps/Input Generation/input_constructor/input_template_office_scenario_11ax.csv](<../Apps/Input%20Generation/input_constructor/input_template_office_scenario_11ax.csv>)|官方原有，当前未改||
|[Apps/Input Generation/input_constructor/input_template_random_students_projects_fgml5g.csv](<../Apps/Input%20Generation/input_constructor/input_template_random_students_projects_fgml5g.csv>)|官方原有，当前未改||
|[Apps/Input Generation/input_constructor/input_template_same_channel_office_scenario_11ax.csv](<../Apps/Input%20Generation/input_constructor/input_template_same_channel_office_scenario_11ax.csv>)|官方原有，当前未改||
|[Apps/Input Generation/input_constructor/input_template_single_office_scenario_11ax.csv](<../Apps/Input%20Generation/input_constructor/input_template_single_office_scenario_11ax.csv>)|官方原有，当前未改||
|[Apps/Input Generation/input_constructor/other/input_template_ten_wlans_legacy.csv](<../Apps/Input%20Generation/input_constructor/other/input_template_ten_wlans_legacy.csv>)|官方原有，当前未改||
|[Apps/Input Generation/input_constructor/other/input_template_ten_wlans_random_bss_color.csv](<../Apps/Input%20Generation/input_constructor/other/input_template_ten_wlans_random_bss_color.csv>)|官方原有，当前未改||
|[Apps/Input Generation/input_constructor/other/input_template_ten_wlans_random_srg.csv](<../Apps/Input%20Generation/input_constructor/other/input_template_ten_wlans_random_srg.csv>)|官方原有，当前未改||
|[Apps/Input Generation/input_constructor/other/input_template_two_wlans_max_power.csv](<../Apps/Input%20Generation/input_constructor/other/input_template_two_wlans_max_power.csv>)|官方原有，当前未改||
|[Apps/Input Generation/input_constructor/other/input_template_two_wlans_medium_power.csv](<../Apps/Input%20Generation/input_constructor/other/input_template_two_wlans_medium_power.csv>)|官方原有，当前未改||
|[Apps/Input Generation/input_constructor/other/input_template_two_wlans_min_power.csv](<../Apps/Input%20Generation/input_constructor/other/input_template_two_wlans_min_power.csv>)|官方原有，当前未改||
|[Apps/Input Generation/input_constructor/random_scenarios/input_template_random_dense_low.csv](<../Apps/Input%20Generation/input_constructor/random_scenarios/input_template_random_dense_low.csv>)|官方原有，当前未改||
|[Apps/Input Generation/input_constructor/random_scenarios/input_template_random_dense_medium.csv](<../Apps/Input%20Generation/input_constructor/random_scenarios/input_template_random_dense_medium.csv>)|官方原有，当前未改||
|[Apps/Input Generation/input_constructor/random_scenarios/input_template_random_semi_dense_low.csv](<../Apps/Input%20Generation/input_constructor/random_scenarios/input_template_random_semi_dense_low.csv>)|官方原有，当前未改||
|[Apps/Input Generation/input_constructor/random_scenarios/input_template_random_semi_dense_medium.csv](<../Apps/Input%20Generation/input_constructor/random_scenarios/input_template_random_semi_dense_medium.csv>)|官方原有，当前未改||
|[Apps/Input Generation/input_constructor/random_scenarios/input_template_random_sparse_low.csv](<../Apps/Input%20Generation/input_constructor/random_scenarios/input_template_random_sparse_low.csv>)|官方原有，当前未改||
|[Apps/Input Generation/input_constructor/random_scenarios/input_template_random_sparse_medium.csv](<../Apps/Input%20Generation/input_constructor/random_scenarios/input_template_random_sparse_medium.csv>)|官方原有，当前未改||
|[Apps/Input Generation/input_constructor/random_scenarios/input_template_random_ultra_dense_low.csv](<../Apps/Input%20Generation/input_constructor/random_scenarios/input_template_random_ultra_dense_low.csv>)|官方原有，当前未改||
|[Apps/Input Generation/input_constructor/random_scenarios/input_template_random_ultra_dense_medium.csv](<../Apps/Input%20Generation/input_constructor/random_scenarios/input_template_random_ultra_dense_medium.csv>)|官方原有，当前未改||
|[Apps/Input Generation/input_constructor/toy_scenarios/input_middle_in_grid.csv](<../Apps/Input%20Generation/input_constructor/toy_scenarios/input_middle_in_grid.csv>)|官方原有，当前未改||
|[Apps/Input Generation/input_constructor/toy_scenarios/input_template_toy_scenario_1.csv](<../Apps/Input%20Generation/input_constructor/toy_scenarios/input_template_toy_scenario_1.csv>)|官方原有，当前未改||
|[Apps/Input Generation/input_constructor/toy_scenarios/input_template_toy_scenario_2.csv](<../Apps/Input%20Generation/input_constructor/toy_scenarios/input_template_toy_scenario_2.csv>)|官方原有，当前未改||
|[Apps/Input Generation/input_template_rectangular.csv](<../Apps/Input%20Generation/input_template_rectangular.csv>)|官方原有，当前未改||
|[Apps/Input Generation/lib/jcommon-1.0.0-pre3.jar](<../Apps/Input%20Generation/lib/jcommon-1.0.0-pre3.jar>)|官方原有，当前未改||
|[Apps/Input Generation/lib/jcommon-1.0.17.jar](<../Apps/Input%20Generation/lib/jcommon-1.0.17.jar>)|官方原有，当前未改||
|[Apps/Input Generation/lib/jfreechart-0.9.16.jar](<../Apps/Input%20Generation/lib/jfreechart-0.9.16.jar>)|官方原有，当前未改||
|[Apps/Input Generation/lib/jfreechart-1.0.13.jar](<../Apps/Input%20Generation/lib/jfreechart-1.0.13.jar>)|官方原有，当前未改||
|[Apps/Input Generation/lib/jfreechart-1.0.19.jar](<../Apps/Input%20Generation/lib/jfreechart-1.0.19.jar>)|官方原有，当前未改||
|[Apps/Input Generation/lib/jfreechart-1.0.7-sources.jar](<../Apps/Input%20Generation/lib/jfreechart-1.0.7-sources.jar>)|官方原有，当前未改||
|[Apps/Input Generation/lib/jfreechart.jar](<../Apps/Input%20Generation/lib/jfreechart.jar>)|官方原有，当前未改||
|[Apps/Input Generation/manifest.mf](<../Apps/Input%20Generation/manifest.mf>)|官方原有，当前未改||
|[Apps/Input Generation/nbproject/build-impl.xml](<../Apps/Input%20Generation/nbproject/build-impl.xml>)|官方原有，当前未改||
|[Apps/Input Generation/nbproject/configs/file_path.properties](<../Apps/Input%20Generation/nbproject/configs/file_path.properties>)|官方原有，当前未改||
|[Apps/Input Generation/nbproject/configs/rectangular_scenario.properties](<../Apps/Input%20Generation/nbproject/configs/rectangular_scenario.properties>)|官方原有，当前未改||
|[Apps/Input Generation/nbproject/genfiles.properties](<../Apps/Input%20Generation/nbproject/genfiles.properties>)|官方原有，当前未改||
|[Apps/Input Generation/nbproject/private/config.properties](<../Apps/Input%20Generation/nbproject/private/config.properties>)|官方原有，当前未改||
|[Apps/Input Generation/nbproject/private/configs/file_path.properties](<../Apps/Input%20Generation/nbproject/private/configs/file_path.properties>)|官方原有，当前未改||
|[Apps/Input Generation/nbproject/private/configs/rectangular_scenario.properties](<../Apps/Input%20Generation/nbproject/private/configs/rectangular_scenario.properties>)|官方原有，当前未改||
|[Apps/Input Generation/nbproject/private/private.properties](<../Apps/Input%20Generation/nbproject/private/private.properties>)|官方原有，当前未改||
|[Apps/Input Generation/nbproject/private/private.xml](<../Apps/Input%20Generation/nbproject/private/private.xml>)|官方原有，当前未改||
|[Apps/Input Generation/nbproject/project.properties](<../Apps/Input%20Generation/nbproject/project.properties>)|官方原有，当前未改||
|[Apps/Input Generation/nbproject/project.xml](<../Apps/Input%20Generation/nbproject/project.xml>)|官方原有，当前未改||
|[Apps/Input Generation/src/AddNaiveColumn.java](<../Apps/Input%20Generation/src/AddNaiveColumn.java>)|官方原有，当前未改||
|[Apps/Input Generation/src/AppendLinesInFile.java](<../Apps/Input%20Generation/src/AppendLinesInFile.java>)|官方原有，当前未改||
|[Apps/Input Generation/src/Map_viewer.java](<../Apps/Input%20Generation/src/Map_viewer.java>)|官方原有，当前未改||
|[Apps/Input Generation/src/NEW_single_office_enterprise_scenario_11ax.java](<../Apps/Input%20Generation/src/NEW_single_office_enterprise_scenario_11ax.java>)|官方原有，当前未改||
|[Apps/Input Generation/src/Node.java](<../Apps/Input%20Generation/src/Node.java>)|官方原有，当前未改||
|[Apps/Input Generation/src/Wlan.java](<../Apps/Input%20Generation/src/Wlan.java>)|官方原有，当前未改||
|[Apps/Input Generation/src/enterprise_scenario_11ax.java](<../Apps/Input%20Generation/src/enterprise_scenario_11ax.java>)|官方原有，当前未改||
|[Apps/Input Generation/src/old/Nodes_deterministic_generator.java](<../Apps/Input%20Generation/src/old/Nodes_deterministic_generator.java>)|官方原有，当前未改||
|[Apps/Input Generation/src/old/Nodes_rectangular_central_generator.java](<../Apps/Input%20Generation/src/old/Nodes_rectangular_central_generator.java>)|官方原有，当前未改||
|[Apps/Input Generation/src/old/Nodes_rectangular_central_generator_no_load.java](<../Apps/Input%20Generation/src/old/Nodes_rectangular_central_generator_no_load.java>)|官方原有，当前未改||
|[Apps/Input Generation/src/old/Nodes_rectangular_central_generator_primary.java](<../Apps/Input%20Generation/src/old/Nodes_rectangular_central_generator_primary.java>)|官方原有，当前未改||
|[Apps/Input Generation/src/old/Nodes_rectangular_generator.java](<../Apps/Input%20Generation/src/old/Nodes_rectangular_generator.java>)|官方原有，当前未改||
|[Apps/Input Generation/src/old/Nodes_rectangular_generator_agents_test.java](<../Apps/Input%20Generation/src/old/Nodes_rectangular_generator_agents_test.java>)|官方原有，当前未改||
|[Apps/Input Generation/src/old/SR_RANDOM_1_bounded.java](<../Apps/Input%20Generation/src/old/SR_RANDOM_1_bounded.java>)|官方原有，当前未改||
|[Apps/Input Generation/src/old/SR_Scenario_2.java](<../Apps/Input%20Generation/src/old/SR_Scenario_2.java>)|官方原有，当前未改||
|[Apps/Input Generation/src/old/SR_Scenario_2b.java](<../Apps/Input%20Generation/src/old/SR_Scenario_2b.java>)|官方原有，当前未改||
|[Apps/Input Generation/src/old/SR_Scenario_2c.java](<../Apps/Input%20Generation/src/old/SR_Scenario_2c.java>)|官方原有，当前未改||
|[Apps/Input Generation/src/old/ThreeWLANsGenerator.java](<../Apps/Input%20Generation/src/old/ThreeWLANsGenerator.java>)|官方原有，当前未改||
|[Apps/Input Generation/src/old/TwoWLANsGenerator.java](<../Apps/Input%20Generation/src/old/TwoWLANsGenerator.java>)|官方原有，当前未改||
|[Apps/Input Generation/src/only_same_channel_enterprise_scenario_11ax.java](<../Apps/Input%20Generation/src/only_same_channel_enterprise_scenario_11ax.java>)|官方原有，当前未改||
|[Apps/Input Generation/src/random_scenarios.java](<../Apps/Input%20Generation/src/random_scenarios.java>)|官方原有，当前未改||
|[Apps/Input Generation/src/random_scenarios_ap_fixed.java](<../Apps/Input%20Generation/src/random_scenarios_ap_fixed.java>)|官方原有，当前未改||
|[Apps/Input Generation/src/random_scenarios_ap_fixed_random_action.java](<../Apps/Input%20Generation/src/random_scenarios_ap_fixed_random_action.java>)|官方原有，当前未改||
|[Apps/Input Generation/src/random_scenarios_magazine_aiml.java](<../Apps/Input%20Generation/src/random_scenarios_magazine_aiml.java>)|官方原有，当前未改||
|[Apps/Input Generation/src/random_scenarios_students_projects_fgml5g.java](<../Apps/Input%20Generation/src/random_scenarios_students_projects_fgml5g.java>)|官方原有，当前未改||
|[Apps/Input Generation/src/single_office_enterprise_scenario_11ax.java](<../Apps/Input%20Generation/src/single_office_enterprise_scenario_11ax.java>)|官方原有，当前未改||
|[Apps/Input Generation/src/toy_scenario_1_11ax_sr.java](<../Apps/Input%20Generation/src/toy_scenario_1_11ax_sr.java>)|官方原有，当前未改||
|[Apps/Input Generation/src/toy_scenario_1_brute_force.java](<../Apps/Input%20Generation/src/toy_scenario_1_brute_force.java>)|官方原有，当前未改||
|[Apps/Input Generation/src/toy_scenario_2.java](<../Apps/Input%20Generation/src/toy_scenario_2.java>)|官方原有，当前未改||
|[Apps/Input Generation/src/toy_scenario_2_11ax_sr.java](<../Apps/Input%20Generation/src/toy_scenario_2_11ax_sr.java>)|官方原有，当前未改||
|[Apps/Input Generation/src/toy_scenario_2_brute_force.java](<../Apps/Input%20Generation/src/toy_scenario_2_brute_force.java>)|官方原有，当前未改||
|[Apps/Input Generation/src/toy_scenario_2_free_tuning.java](<../Apps/Input%20Generation/src/toy_scenario_2_free_tuning.java>)|官方原有，当前未改||
|[Apps/Input Generation/src/toy_scenario_3_11ax_sr.java](<../Apps/Input%20Generation/src/toy_scenario_3_11ax_sr.java>)|官方原有，当前未改||
|[Apps/Input Generation/src/toy_scenario_3_brute_force.java](<../Apps/Input%20Generation/src/toy_scenario_3_brute_force.java>)|官方原有，当前未改||
|[Apps/Input Generation/src/toy_scenario_3_free_tuning.java](<../Apps/Input%20Generation/src/toy_scenario_3_free_tuning.java>)|官方原有，当前未改||
|[Apps/Matlab Files/chart.fig](<../Apps/Matlab%20Files/chart.fig>)|官方原有，当前未改||
|[Apps/Matlab Files/chart.png](<../Apps/Matlab%20Files/chart.png>)|官方原有，当前未改||
|[Apps/Matlab Files/generate_chart.m](<../Apps/Matlab%20Files/generate_chart.m>)|官方原有，当前未改||
|[Apps/Matlab Files/sutransmissiontime802_11ax/README.txt](<../Apps/Matlab%20Files/sutransmissiontime802_11ax/README.txt>)|官方原有，当前未改||
|[Apps/Matlab Files/sutransmissiontime802_11ax/ieee11axMACParams.m](<../Apps/Matlab%20Files/sutransmissiontime802_11ax/ieee11axMACParams.m>)|官方原有，当前未改||
|[Apps/Matlab Files/sutransmissiontime802_11ax/ieee11axPHYParams.m](<../Apps/Matlab%20Files/sutransmissiontime802_11ax/ieee11axPHYParams.m>)|官方原有，当前未改||
|[Apps/Matlab Files/sutransmissiontime802_11ax/ieee11axSUtransmission.m](<../Apps/Matlab%20Files/sutransmissiontime802_11ax/ieee11axSUtransmission.m>)|官方原有，当前未改||
|[Apps/Matlab Files/sutransmissiontime802_11ax/paperAPinitiatedMU11axK.pdf](<../Apps/Matlab%20Files/sutransmissiontime802_11ax/paperAPinitiatedMU11axK.pdf>)|官方原有，当前未改||
|[Apps/New Input Generator/input_constructor/input_template_random_dense_full.csv](<../Apps/New%20Input%20Generator/input_constructor/input_template_random_dense_full.csv>)|官方原有，当前未改||
|[Apps/New Input Generator/input_constructor/input_template_random_dense_full_sta_random.csv](<../Apps/New%20Input%20Generator/input_constructor/input_template_random_dense_full_sta_random.csv>)|官方原有，当前未改||
|[Apps/New Input Generator/input_constructor/input_template_random_dense_low.csv](<../Apps/New%20Input%20Generator/input_constructor/input_template_random_dense_low.csv>)|官方原有，当前未改||
|[Apps/New Input Generator/input_constructor/input_template_random_dense_mid.csv](<../Apps/New%20Input%20Generator/input_constructor/input_template_random_dense_mid.csv>)|官方原有，当前未改||
|[Apps/New Input Generator/main.py](<../Apps/New%20Input%20Generator/main.py>)|官方原有，当前未改||
|[Apps/New Input Generator/structures/Wlan.py](<../Apps/New%20Input%20Generator/structures/Wlan.py>)|官方原有，当前未改||
|[CITATION.cff](<../CITATION.cff>)|官方原有，当前未改||
|[CODE_OF_CONDUCT.md](<../CODE_OF_CONDUCT.md>)|官方原有，当前未改||
|[CONTRIBUTING.md](<../CONTRIBUTING.md>)|官方原有，当前未改||
|[Code/COST/cxx](<../Code/COST/cxx>)|官方原有，当前未改||
|[Code/compcpp/LICENSE](<../Code/compcpp/LICENSE>)|官方原有，当前未改||
|[Code/compcpp/Makefile](<../Code/compcpp/Makefile>)|官方原有，当前未改||
|[Code/compcpp/README.md](<../Code/compcpp/README.md>)|官方原有，当前未改||
|[Code/config_models](<../Code/config_models>)|官方原有，当前未改||
|[Code/input/examples/bandwidth_adaptation_example/input_nodes_bw_adapt.csv](<../Code/input/examples/bandwidth_adaptation_example/input_nodes_bw_adapt.csv>)|官方原有，当前未改||
|[Code/input/examples/basic_example/agents.csv](<../Code/input/examples/basic_example/agents.csv>)|官方原有，当前未改||
|[Code/input/examples/basic_example/input_nodes.csv](<../Code/input/examples/basic_example/input_nodes.csv>)|官方原有，当前未改||
|[Code/input/examples/basic_example/input_nodes_rts_cts.csv](<../Code/input/examples/basic_example/input_nodes_rts_cts.csv>)|官方原有，当前未改||
|[Code/input/examples/basic_example/input_nodes_spatial_reuse.csv](<../Code/input/examples/basic_example/input_nodes_spatial_reuse.csv>)|官方原有，当前未改||
|[Code/input/examples/dso_npca_example/input_nodes_dcf.csv](<../Code/input/examples/dso_npca_example/input_nodes_dcf.csv>)|官方原有，当前未改||
|[Code/input/examples/dso_npca_example/input_nodes_dso.csv](<../Code/input/examples/dso_npca_example/input_nodes_dso.csv>)|官方原有，当前未改||
|[Code/input/examples/dso_npca_example/input_nodes_dso_npca.csv](<../Code/input/examples/dso_npca_example/input_nodes_dso_npca.csv>)|官方原有，当前未改||
|[Code/input/examples/dso_npca_example/input_nodes_dso_test.csv](<../Code/input/examples/dso_npca_example/input_nodes_dso_test.csv>)|官方原有，当前未改||
|[Code/input/examples/dso_npca_example/input_nodes_npca.csv](<../Code/input/examples/dso_npca_example/input_nodes_npca.csv>)|官方原有，当前未改||
|[Code/input/examples/edca_example/input_nodes_edca_1_single_bss.csv](<../Code/input/examples/edca_example/input_nodes_edca_1_single_bss.csv>)|官方原有，当前未改||
|[Code/input/examples/edca_example/input_nodes_edca_2_ac_differentiation.csv](<../Code/input/examples/edca_example/input_nodes_edca_2_ac_differentiation.csv>)|官方原有，当前未改||
|[Code/input/examples/edca_example/input_nodes_edca_3_edca_vs_dcf.csv](<../Code/input/examples/edca_example/input_nodes_edca_3_edca_vs_dcf.csv>)|官方原有，当前未改||
|[Code/input/examples/external_model_example/agents_external.csv](<../Code/input/examples/external_model_example/agents_external.csv>)|官方原有，当前未改||
|[Code/input/examples/external_model_example/input_nodes.csv](<../Code/input/examples/external_model_example/input_nodes.csv>)|官方原有，当前未改||
|[Code/input/examples/mab_example/agents_egreedy.csv](<../Code/input/examples/mab_example/agents_egreedy.csv>)|官方原有，当前未改||
|[Code/input/examples/mab_example/agents_monitoring.csv](<../Code/input/examples/mab_example/agents_monitoring.csv>)|官方原有，当前未改||
|[Code/input/examples/mab_example/input_nodes.csv](<../Code/input/examples/mab_example/input_nodes.csv>)|官方原有，当前未改||
|[Code/input/examples/mapc_example/input_nodes_mapc_example.csv](<../Code/input/examples/mapc_example/input_nodes_mapc_example.csv>)|官方原有，当前未改||
|[Code/input/examples/mapc_example/mapc_cobf_example.csv](<../Code/input/examples/mapc_example/mapc_cobf_example.csv>)|官方原有，当前未改||
|[Code/input/examples/mapc_example/mapc_cosr_example.csv](<../Code/input/examples/mapc_example/mapc_cosr_example.csv>)|官方原有，当前未改||
|[Code/input/examples/mapc_example/mapc_cotdma_example.csv](<../Code/input/examples/mapc_example/mapc_cotdma_example.csv>)|官方原有，当前未改||
|[Code/input/examples/preamble_puncturing_example/input_nodes_pp.csv](<../Code/input/examples/preamble_puncturing_example/input_nodes_pp.csv>)|官方原有，当前未改||
|[Code/input/validation/basic_scenarios/input_nodes_scenario_1a.csv](<../Code/input/validation/basic_scenarios/input_nodes_scenario_1a.csv>)|官方原有，当前未改||
|[Code/input/validation/basic_scenarios/input_nodes_scenario_1a_aggregation.csv](<../Code/input/validation/basic_scenarios/input_nodes_scenario_1a_aggregation.csv>)|官方原有，当前未改||
|[Code/input/validation/basic_scenarios/input_nodes_scenario_1b.csv](<../Code/input/validation/basic_scenarios/input_nodes_scenario_1b.csv>)|官方原有，当前未改||
|[Code/input/validation/basic_scenarios/input_nodes_scenario_1b_aggregation.csv](<../Code/input/validation/basic_scenarios/input_nodes_scenario_1b_aggregation.csv>)|官方原有，当前未改||
|[Code/input/validation/channel_bonding_scenarios/input_nodes_0_pw_bw_validation_20MHz.csv](<../Code/input/validation/channel_bonding_scenarios/input_nodes_0_pw_bw_validation_20MHz.csv>)|官方原有，当前未改||
|[Code/input/validation/channel_bonding_scenarios/input_nodes_1_pw_bw_validation_40MHz.csv](<../Code/input/validation/channel_bonding_scenarios/input_nodes_1_pw_bw_validation_40MHz.csv>)|官方原有，当前未改||
|[Code/input/validation/channel_bonding_scenarios/input_nodes_2_pw_bw_validation_80MHz.csv](<../Code/input/validation/channel_bonding_scenarios/input_nodes_2_pw_bw_validation_80MHz.csv>)|官方原有，当前未改||
|[Code/input/validation/channel_bonding_scenarios/input_nodes_3_pw_bw_validation_160MHz.csv](<../Code/input/validation/channel_bonding_scenarios/input_nodes_3_pw_bw_validation_160MHz.csv>)|官方原有，当前未改||
|[Code/input/validation/complex_scenarios/input_nodes_scenario_2a.csv](<../Code/input/validation/complex_scenarios/input_nodes_scenario_2a.csv>)|官方原有，当前未改||
|[Code/input/validation/complex_scenarios/input_nodes_scenario_2a_aggregation.csv](<../Code/input/validation/complex_scenarios/input_nodes_scenario_2a_aggregation.csv>)|官方原有，当前未改||
|[Code/input/validation/complex_scenarios/input_nodes_scenario_2b.csv](<../Code/input/validation/complex_scenarios/input_nodes_scenario_2b.csv>)|官方原有，当前未改||
|[Code/input/validation/complex_scenarios/input_nodes_scenario_2b_aggregation.csv](<../Code/input/validation/complex_scenarios/input_nodes_scenario_2b_aggregation.csv>)|官方原有，当前未改||
|[Code/input/validation/complex_scenarios/input_nodes_scenario_2c.csv](<../Code/input/validation/complex_scenarios/input_nodes_scenario_2c.csv>)|官方原有，当前未改||
|[Code/input/validation/complex_scenarios/input_nodes_scenario_2c_aggregation.csv](<../Code/input/validation/complex_scenarios/input_nodes_scenario_2c_aggregation.csv>)|官方原有，当前未改||
|[Code/input/validation/complex_scenarios/input_nodes_scenario_2d.csv](<../Code/input/validation/complex_scenarios/input_nodes_scenario_2d.csv>)|官方原有，当前未改||
|[Code/input/validation/complex_scenarios/input_nodes_scenario_2d_aggregation.csv](<../Code/input/validation/complex_scenarios/input_nodes_scenario_2d_aggregation.csv>)|官方原有，当前未改||
|[Code/input/validation/high_density_scenarios/input_nodes_1.csv](<../Code/input/validation/high_density_scenarios/input_nodes_1.csv>)|官方原有，当前未改||
|[Code/input/validation/high_density_scenarios/input_nodes_10.csv](<../Code/input/validation/high_density_scenarios/input_nodes_10.csv>)|官方原有，当前未改||
|[Code/input/validation/high_density_scenarios/input_nodes_12.csv](<../Code/input/validation/high_density_scenarios/input_nodes_12.csv>)|官方原有，当前未改||
|[Code/input/validation/high_density_scenarios/input_nodes_14.csv](<../Code/input/validation/high_density_scenarios/input_nodes_14.csv>)|官方原有，当前未改||
|[Code/input/validation/high_density_scenarios/input_nodes_16.csv](<../Code/input/validation/high_density_scenarios/input_nodes_16.csv>)|官方原有，当前未改||
|[Code/input/validation/high_density_scenarios/input_nodes_18.csv](<../Code/input/validation/high_density_scenarios/input_nodes_18.csv>)|官方原有，当前未改||
|[Code/input/validation/high_density_scenarios/input_nodes_2.csv](<../Code/input/validation/high_density_scenarios/input_nodes_2.csv>)|官方原有，当前未改||
|[Code/input/validation/high_density_scenarios/input_nodes_20.csv](<../Code/input/validation/high_density_scenarios/input_nodes_20.csv>)|官方原有，当前未改||
|[Code/input/validation/high_density_scenarios/input_nodes_4.csv](<../Code/input/validation/high_density_scenarios/input_nodes_4.csv>)|官方原有，当前未改||
|[Code/input/validation/high_density_scenarios/input_nodes_5.csv](<../Code/input/validation/high_density_scenarios/input_nodes_5.csv>)|官方原有，当前未改||
|[Code/input/validation/high_density_scenarios/input_nodes_50.csv](<../Code/input/validation/high_density_scenarios/input_nodes_50.csv>)|官方原有，当前未改||
|[Code/input/validation/high_density_scenarios/input_nodes_6.csv](<../Code/input/validation/high_density_scenarios/input_nodes_6.csv>)|官方原有，当前未改||
|[Code/input/validation/high_density_scenarios/input_nodes_8.csv](<../Code/input/validation/high_density_scenarios/input_nodes_8.csv>)|官方原有，当前未改||
|[Code/input/validation/high_density_scenarios/input_system_conf.csv](<../Code/input/validation/high_density_scenarios/input_system_conf.csv>)|官方原有，当前未改||
|[Code/input/validation/spatial_reuse_scenarios/input_nodes_spatial_reuse_1.csv](<../Code/input/validation/spatial_reuse_scenarios/input_nodes_spatial_reuse_1.csv>)|官方原有，当前未改||
|[Code/input/validation/spatial_reuse_scenarios/input_nodes_spatial_reuse_2.csv](<../Code/input/validation/spatial_reuse_scenarios/input_nodes_spatial_reuse_2.csv>)|官方原有，当前未改||
|[Code/learning_modules/python_servers/ml_server_passthrough.py](<../Code/learning_modules/python_servers/ml_server_passthrough.py>)|官方原有，当前未改||
|[Code/learning_modules/python_servers/ml_server_pytorch.py](<../Code/learning_modules/python_servers/ml_server_pytorch.py>)|官方原有，当前未改||
|[Code/learning_modules/python_servers/ml_server_random.py](<../Code/learning_modules/python_servers/ml_server_random.py>)|官方原有，当前未改||
|[Code/main/Makefile](<../Code/main/Makefile>)|官方原有，本地修改|官方已有构建文件；独立化时重写依赖跟踪，修改子目录头文件触发 COST 重生成与编译。 后续统一引用 build.mk，并跟踪编译器/flags stamp，避免参数切换沿用旧二进制。|
|[Code/main/build_local](<../Code/main/build_local>)|官方原有，本地修改|官方已有入口；本地改为转交同目录 make，保持 PROFILE/CXX/flags 统一，移除硬编码 -Werror 编译。|
|[Code/output/.gitkeep](<../Code/output/.gitkeep>)|本地工程/测试/文档||
|[Code/scripts_multiple_executions/multiple_inputs_script.sh](<../Code/scripts_multiple_executions/multiple_inputs_script.sh>)|官方原有，当前未改||
|[Code/scripts_multiple_executions/multiple_inputs_script_agents.sh](<../Code/scripts_multiple_executions/multiple_inputs_script_agents.sh>)|官方原有，当前未改||
|[Code/scripts_multiple_executions/multiple_inputs_script_several_seeds.sh](<../Code/scripts_multiple_executions/multiple_inputs_script_several_seeds.sh>)|官方原有，当前未改||
|[Code/validate.sh](<../Code/validate.sh>)|官方原有，当前未改||
|[Documentation/Other/Images and resources/capture_effect.pptx](<../Documentation/Other/Images%20and%20resources/capture_effect.pptx>)|官方原有，当前未改||
|[Documentation/Other/Images and resources/cochannel_interference.pptx](<../Documentation/Other/Images%20and%20resources/cochannel_interference.pptx>)|官方原有，当前未改||
|[Documentation/Other/Images and resources/cost.pptx](<../Documentation/Other/Images%20and%20resources/cost.pptx>)|官方原有，当前未改||
|[Documentation/Other/Images and resources/dcf_operation.pptx](<../Documentation/Other/Images%20and%20resources/dcf_operation.pptx>)|官方原有，当前未改||
|[Documentation/Other/Images and resources/dcf_operation_rts_cts.pptx](<../Documentation/Other/Images%20and%20resources/dcf_operation_rts_cts.pptx>)|官方原有，当前未改||
|[Documentation/Other/Images and resources/example_execution_regression_test.png](<../Documentation/Other/Images%20and%20resources/example_execution_regression_test.png>)|官方原有，当前未改||
|[Documentation/Other/Images and resources/example_primary_channel_eps_greedy.png](<../Documentation/Other/Images%20and%20resources/example_primary_channel_eps_greedy.png>)|官方原有，当前未改||
|[Documentation/Other/Images and resources/friis_path_loss.png](<../Documentation/Other/Images%20and%20resources/friis_path_loss.png>)|官方原有，当前未改||
|[Documentation/Other/Images and resources/kom8ndor_logo.png](<../Documentation/Other/Images%20and%20resources/kom8ndor_logo.png>)|官方原有，当前未改||
|[Documentation/Other/Images and resources/komondor_flowchart.pptx](<../Documentation/Other/Images%20and%20resources/komondor_flowchart.pptx>)|官方原有，当前未改||
|[Documentation/Other/Images and resources/komondor_logo.png](<../Documentation/Other/Images%20and%20resources/komondor_logo.png>)|官方原有，当前未改||
|[Documentation/Other/Images and resources/komondor_visio.vsd](<../Documentation/Other/Images%20and%20resources/komondor_visio.vsd>)|官方原有，当前未改||
|[Documentation/Other/Images and resources/log_coding.ods](<../Documentation/Other/Images%20and%20resources/log_coding.ods>)|官方原有，当前未改||
|[Documentation/Other/Images and resources/modules_overview.png](<../Documentation/Other/Images%20and%20resources/modules_overview.png>)|官方原有，当前未改||
|[Documentation/Other/Images and resources/overview_agents.png](<../Documentation/Other/Images%20and%20resources/overview_agents.png>)|官方原有，当前未改||
|[Documentation/Other/Images and resources/traffic_models.pptx](<../Documentation/Other/Images%20and%20resources/traffic_models.pptx>)|官方原有，当前未改||
|[Documentation/Other/Validation/Komondor/basic_scenarios/.~lock.input_nodes_scenario_1a.csv#](<../Documentation/Other/Validation/Komondor/basic_scenarios/.~lock.input_nodes_scenario_1a.csv%23>)|官方原有，当前未改||
|[Documentation/Other/Validation/Komondor/basic_scenarios/.~lock.input_system_conf.csv#](<../Documentation/Other/Validation/Komondor/basic_scenarios/.~lock.input_system_conf.csv%23>)|官方原有，当前未改||
|[Documentation/Other/Validation/Komondor/basic_scenarios/input_nodes_scenario_1a.csv](<../Documentation/Other/Validation/Komondor/basic_scenarios/input_nodes_scenario_1a.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/Komondor/basic_scenarios/input_nodes_scenario_1b.csv](<../Documentation/Other/Validation/Komondor/basic_scenarios/input_nodes_scenario_1b.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/Komondor/basic_scenarios/input_system_conf.csv](<../Documentation/Other/Validation/Komondor/basic_scenarios/input_system_conf.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/Komondor/complex_scenarios/input_nodes_scenario_2a.csv](<../Documentation/Other/Validation/Komondor/complex_scenarios/input_nodes_scenario_2a.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/Komondor/complex_scenarios/input_nodes_scenario_2b.csv](<../Documentation/Other/Validation/Komondor/complex_scenarios/input_nodes_scenario_2b.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/Komondor/complex_scenarios/input_nodes_scenario_2c.csv](<../Documentation/Other/Validation/Komondor/complex_scenarios/input_nodes_scenario_2c.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/Komondor/complex_scenarios/input_nodes_scenario_2d.csv](<../Documentation/Other/Validation/Komondor/complex_scenarios/input_nodes_scenario_2d.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/Komondor/complex_scenarios/input_system_conf.csv](<../Documentation/Other/Validation/Komondor/complex_scenarios/input_system_conf.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/Komondor/high_density_scenarios/input_nodes_1.csv](<../Documentation/Other/Validation/Komondor/high_density_scenarios/input_nodes_1.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/Komondor/high_density_scenarios/input_nodes_10.csv](<../Documentation/Other/Validation/Komondor/high_density_scenarios/input_nodes_10.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/Komondor/high_density_scenarios/input_nodes_12.csv](<../Documentation/Other/Validation/Komondor/high_density_scenarios/input_nodes_12.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/Komondor/high_density_scenarios/input_nodes_14.csv](<../Documentation/Other/Validation/Komondor/high_density_scenarios/input_nodes_14.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/Komondor/high_density_scenarios/input_nodes_16.csv](<../Documentation/Other/Validation/Komondor/high_density_scenarios/input_nodes_16.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/Komondor/high_density_scenarios/input_nodes_18.csv](<../Documentation/Other/Validation/Komondor/high_density_scenarios/input_nodes_18.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/Komondor/high_density_scenarios/input_nodes_2.csv](<../Documentation/Other/Validation/Komondor/high_density_scenarios/input_nodes_2.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/Komondor/high_density_scenarios/input_nodes_20.csv](<../Documentation/Other/Validation/Komondor/high_density_scenarios/input_nodes_20.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/Komondor/high_density_scenarios/input_nodes_4.csv](<../Documentation/Other/Validation/Komondor/high_density_scenarios/input_nodes_4.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/Komondor/high_density_scenarios/input_nodes_5.csv](<../Documentation/Other/Validation/Komondor/high_density_scenarios/input_nodes_5.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/Komondor/high_density_scenarios/input_nodes_50.csv](<../Documentation/Other/Validation/Komondor/high_density_scenarios/input_nodes_50.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/Komondor/high_density_scenarios/input_nodes_6.csv](<../Documentation/Other/Validation/Komondor/high_density_scenarios/input_nodes_6.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/Komondor/high_density_scenarios/input_nodes_8.csv](<../Documentation/Other/Validation/Komondor/high_density_scenarios/input_nodes_8.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/Komondor/high_density_scenarios/input_system_conf.csv](<../Documentation/Other/Validation/Komondor/high_density_scenarios/input_system_conf.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/Validations Technical Report/Basic Operation/Scenario I/Bianchi/Bianchi_Scenario_I.m](<../Documentation/Other/Validation/Validations%20Technical%20Report/Basic%20Operation/Scenario%20I/Bianchi/Bianchi_Scenario_I.m>)|官方原有，当前未改||
|[Documentation/Other/Validation/Validations Technical Report/Old/Basic Scenarios/1a/output_logs_1a.txt](<../Documentation/Other/Validation/Validations%20Technical%20Report/Old/Basic%20Scenarios/1a/output_logs_1a.txt>)|官方原有，当前未改||
|[Documentation/Other/Validation/Validations Technical Report/Old/Basic Scenarios/1b/output_logs_1b.txt](<../Documentation/Other/Validation/Validations%20Technical%20Report/Old/Basic%20Scenarios/1b/output_logs_1b.txt>)|官方原有，当前未改||
|[Documentation/Other/Validation/Validations Technical Report/Old/Basic Scenarios/2a/output_logs_2a.txt](<../Documentation/Other/Validation/Validations%20Technical%20Report/Old/Basic%20Scenarios/2a/output_logs_2a.txt>)|官方原有，当前未改||
|[Documentation/Other/Validation/Validations Technical Report/Old/Basic Scenarios/2b/output_logs_2b.txt](<../Documentation/Other/Validation/Validations%20Technical%20Report/Old/Basic%20Scenarios/2b/output_logs_2b.txt>)|官方原有，当前未改||
|[Documentation/Other/Validation/Validations Technical Report/Old/Basic Scenarios/2c/output_logs_2c.txt](<../Documentation/Other/Validation/Validations%20Technical%20Report/Old/Basic%20Scenarios/2c/output_logs_2c.txt>)|官方原有，当前未改||
|[Documentation/Other/Validation/Validations Technical Report/Old/Hidden and Exposed node Scenarios/Exposed Node/output_logs_exposed.txt](<../Documentation/Other/Validation/Validations%20Technical%20Report/Old/Hidden%20and%20Exposed%20node%20Scenarios/Exposed%20Node/output_logs_exposed.txt>)|官方原有，当前未改||
|[Documentation/Other/Validation/Validations Technical Report/Old/Hidden and Exposed node Scenarios/Hidden node/output_logs_hidden.txt](<../Documentation/Other/Validation/Validations%20Technical%20Report/Old/Hidden%20and%20Exposed%20node%20Scenarios/Hidden%20node/output_logs_hidden.txt>)|官方原有，当前未改||
|[Documentation/Other/Validation/bianchi/11ax_times/ieee11axMACParams.m](<../Documentation/Other/Validation/bianchi/11ax_times/ieee11axMACParams.m>)|官方原有，当前未改||
|[Documentation/Other/Validation/bianchi/11ax_times/ieee11axPHYParams.m](<../Documentation/Other/Validation/bianchi/11ax_times/ieee11axPHYParams.m>)|官方原有，当前未改||
|[Documentation/Other/Validation/bianchi/11ax_times/ieee11axSUtransmission.m](<../Documentation/Other/Validation/bianchi/11ax_times/ieee11axSUtransmission.m>)|官方原有，当前未改||
|[Documentation/Other/Validation/bianchi/ieee11axBiancci.m](<../Documentation/Other/Validation/bianchi/ieee11axBiancci.m>)|官方原有，当前未改||
|[Documentation/Other/Validation/high-density/Komondor/input/multiple_inputs_script.sh](<../Documentation/Other/Validation/high-density/Komondor/input/multiple_inputs_script.sh>)|官方原有，当前未改||
|[Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_01.csv](<../Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_01.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_02.csv](<../Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_02.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_04.csv](<../Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_04.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_06.csv](<../Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_06.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_08.csv](<../Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_08.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_10.csv](<../Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_10.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_12.csv](<../Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_12.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_14.csv](<../Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_14.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_16.csv](<../Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_16.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_18.csv](<../Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_18.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_20.csv](<../Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_20.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_22.csv](<../Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_22.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_24.csv](<../Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_24.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_26.csv](<../Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_26.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_28.csv](<../Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_28.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_30.csv](<../Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_30.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_32.csv](<../Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_32.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_34.csv](<../Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_34.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_36.csv](<../Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_36.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_38.csv](<../Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_38.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_40.csv](<../Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_40.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_42.csv](<../Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_42.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_44.csv](<../Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_44.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_46.csv](<../Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_46.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_48.csv](<../Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_48.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_50.csv](<../Documentation/Other/Validation/high-density/Komondor/input/nodes/input_nodes_50.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/high-density/Komondor/input/system/input_system_conf.csv](<../Documentation/Other/Validation/high-density/Komondor/input/system/input_system_conf.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/high-density/Komondor/output/logs_console.txt](<../Documentation/Other/Validation/high-density/Komondor/output/logs_console.txt>)|官方原有，当前未改||
|[Documentation/Other/Validation/high-density/Komondor/output/script_output.txt](<../Documentation/Other/Validation/high-density/Komondor/output/script_output.txt>)|官方原有，当前未改||
|[Documentation/Other/Validation/high-density/Komondor/output/script_output_csv.csv](<../Documentation/Other/Validation/high-density/Komondor/output/script_output_csv.csv>)|官方原有，当前未改||
|[Documentation/Other/Validation/high-density/komondor_bianchi.xlsx](<../Documentation/Other/Validation/high-density/komondor_bianchi.xlsx>)|官方原有，当前未改||
|[Documentation/Other/Validation/ns-3/README_ns-3_modifications_komondor.txt](<../Documentation/Other/Validation/ns-3/README_ns-3_modifications_komondor.txt>)|官方原有，当前未改||
|[Documentation/Other/Validation/ns-3/ns-3-dev-November2018.zip](<../Documentation/Other/Validation/ns-3/ns-3-dev-November2018.zip>)|官方原有，当前未改||
|[Documentation/Other/Validation/sfctmn_scenarios/11ax_times/ieee11axMACParams.m](<../Documentation/Other/Validation/sfctmn_scenarios/11ax_times/ieee11axMACParams.m>)|官方原有，当前未改||
|[Documentation/Other/Validation/sfctmn_scenarios/11ax_times/ieee11axPHYParams.m](<../Documentation/Other/Validation/sfctmn_scenarios/11ax_times/ieee11axPHYParams.m>)|官方原有，当前未改||
|[Documentation/Other/Validation/sfctmn_scenarios/11ax_times/ieee11axSUtransmission.m](<../Documentation/Other/Validation/sfctmn_scenarios/11ax_times/ieee11axSUtransmission.m>)|官方原有，当前未改||
|[Documentation/Other/Validation/sfctmn_scenarios/scenario_1a.txt](<../Documentation/Other/Validation/sfctmn_scenarios/scenario_1a.txt>)|官方原有，当前未改||
|[Documentation/Other/Validation/sfctmn_scenarios/scenario_2a.txt](<../Documentation/Other/Validation/sfctmn_scenarios/scenario_2a.txt>)|官方原有，当前未改||
|[Documentation/Other/Validation/sfctmn_scenarios/scenario_2b.txt](<../Documentation/Other/Validation/sfctmn_scenarios/scenario_2b.txt>)|官方原有，当前未改||
|[Documentation/Other/Validation/sfctmn_scenarios/scenario_2c.txt](<../Documentation/Other/Validation/sfctmn_scenarios/scenario_2c.txt>)|官方原有，当前未改||
|[Documentation/Other/Validation/sfctmn_scenarios/scenario_2d.txt](<../Documentation/Other/Validation/sfctmn_scenarios/scenario_2d.txt>)|官方原有，当前未改||
|[Documentation/Other/Validation/sfctmn_scenarios/system_conf.m](<../Documentation/Other/Validation/sfctmn_scenarios/system_conf.m>)|官方原有，当前未改||
|[Documentation/Other/Validation/validation_report_v01.pdf](<../Documentation/Other/Validation/validation_report_v01.pdf>)|官方原有，当前未改||
|[Documentation/Papers/Kom8ndor paper (2026)/Kom8ndor__The_Wi_Fi_8_Simulator.zip](<../Documentation/Papers/Kom8ndor%20paper%20%282026%29/Kom8ndor__The_Wi_Fi_8_Simulator.zip>)|官方原有，当前未改||
|[Documentation/Papers/Komondor paper (2019)/Komondor_LaTeX_files.zip](<../Documentation/Papers/Komondor%20paper%20%282019%29/Komondor_LaTeX_files.zip>)|官方原有，当前未改||
|[Documentation/Presentations/agents_first_implementation.pptx](<../Documentation/Presentations/agents_first_implementation.pptx>)|官方原有，当前未改||
|[Documentation/Presentations/komondor_introductory_presentation.pdf](<../Documentation/Presentations/komondor_introductory_presentation.pdf>)|官方原有，当前未改||
|[Documentation/Presentations/komondor_introductory_presentation.pptx](<../Documentation/Presentations/komondor_introductory_presentation.pptx>)|官方原有，当前未改||
|[Documentation/Tutorials/Developer guide/Komondor_DEV_guide/komondor_dev_guide.pdf](<../Documentation/Tutorials/Developer%20guide/Komondor_DEV_guide/komondor_dev_guide.pdf>)|官方原有，当前未改||
|[Documentation/Tutorials/Developer guide/Komondor_DEV_guide/komondor_dev_guide.tex](<../Documentation/Tutorials/Developer%20guide/Komondor_DEV_guide/komondor_dev_guide.tex>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/bib.bib](<../Documentation/Tutorials/Tutorial%20%28OLD%29/bib.bib>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/images/ACK_issue.png](<../Documentation/Tutorials/Tutorial%20%28OLD%29/images/ACK_issue.png>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/images/Seminar3_2-eps-converted-to.pdf](<../Documentation/Tutorials/Tutorial%20%28OLD%29/images/Seminar3_2-eps-converted-to.pdf>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/images/Seminar3_2.eps](<../Documentation/Tutorials/Tutorial%20%28OLD%29/images/Seminar3_2.eps>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/images/agents_komondor.png](<../Documentation/Tutorials/Tutorial%20%28OLD%29/images/agents_komondor.png>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/images/ampdu.png](<../Documentation/Tutorials/Tutorial%20%28OLD%29/images/ampdu.png>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/images/capture_effect.pdf](<../Documentation/Tutorials/Tutorial%20%28OLD%29/images/capture_effect.pdf>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/images/cb_policy_flowchart.png](<../Documentation/Tutorials/Tutorial%20%28OLD%29/images/cb_policy_flowchart.png>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/images/channel_bonding_scenario1.png](<../Documentation/Tutorials/Tutorial%20%28OLD%29/images/channel_bonding_scenario1.png>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/images/channel_bonding_scenario2.png](<../Documentation/Tutorials/Tutorial%20%28OLD%29/images/channel_bonding_scenario2.png>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/images/channel_bonding_scenario3.png](<../Documentation/Tutorials/Tutorial%20%28OLD%29/images/channel_bonding_scenario3.png>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/images/channel_bonding_scenario4.png](<../Documentation/Tutorials/Tutorial%20%28OLD%29/images/channel_bonding_scenario4.png>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/images/channel_bonding_scenario5.png](<../Documentation/Tutorials/Tutorial%20%28OLD%29/images/channel_bonding_scenario5.png>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/images/channel_bonding_scenario6.png](<../Documentation/Tutorials/Tutorial%20%28OLD%29/images/channel_bonding_scenario6.png>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/images/channel_bonding_scenario7.png](<../Documentation/Tutorials/Tutorial%20%28OLD%29/images/channel_bonding_scenario7.png>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/images/channel_bonding_scenario8.png](<../Documentation/Tutorials/Tutorial%20%28OLD%29/images/channel_bonding_scenario8.png>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/images/cochannel_interference.pdf](<../Documentation/Tutorials/Tutorial%20%28OLD%29/images/cochannel_interference.pdf>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/images/cost.pdf](<../Documentation/Tutorials/Tutorial%20%28OLD%29/images/cost.pdf>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/images/dcb_dcf.png](<../Documentation/Tutorials/Tutorial%20%28OLD%29/images/dcb_dcf.png>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/images/dcf_operation.png](<../Documentation/Tutorials/Tutorial%20%28OLD%29/images/dcf_operation.png>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/images/general_statistics_example.png](<../Documentation/Tutorials/Tutorial%20%28OLD%29/images/general_statistics_example.png>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/images/komondor_files.png](<../Documentation/Tutorials/Tutorial%20%28OLD%29/images/komondor_files.png>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/images/komondor_flowchart.pdf](<../Documentation/Tutorials/Tutorial%20%28OLD%29/images/komondor_flowchart.pdf>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/images/nodes_statistics_example.png](<../Documentation/Tutorials/Tutorial%20%28OLD%29/images/nodes_statistics_example.png>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/images/rts_cts.jpg](<../Documentation/Tutorials/Tutorial%20%28OLD%29/images/rts_cts.jpg>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/images/rts_cts_mechanism.pdf](<../Documentation/Tutorials/Tutorial%20%28OLD%29/images/rts_cts_mechanism.pdf>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/images/scenario3_results_sfctmn_komondor.png](<../Documentation/Tutorials/Tutorial%20%28OLD%29/images/scenario3_results_sfctmn_komondor.png>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/images/scenario_I.png](<../Documentation/Tutorials/Tutorial%20%28OLD%29/images/scenario_I.png>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/images/scenario_II.png](<../Documentation/Tutorials/Tutorial%20%28OLD%29/images/scenario_II.png>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/images/scenario_III.png](<../Documentation/Tutorials/Tutorial%20%28OLD%29/images/scenario_III.png>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/images/state_diagram-eps-converted-to.pdf](<../Documentation/Tutorials/Tutorial%20%28OLD%29/images/state_diagram-eps-converted-to.pdf>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/images/state_diagram.eps](<../Documentation/Tutorials/Tutorial%20%28OLD%29/images/state_diagram.eps>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/images/traffic_models.pdf](<../Documentation/Tutorials/Tutorial%20%28OLD%29/images/traffic_models.pdf>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/komondor_tutorial.aux](<../Documentation/Tutorials/Tutorial%20%28OLD%29/komondor_tutorial.aux>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/komondor_tutorial.bbl](<../Documentation/Tutorials/Tutorial%20%28OLD%29/komondor_tutorial.bbl>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/komondor_tutorial.blg](<../Documentation/Tutorials/Tutorial%20%28OLD%29/komondor_tutorial.blg>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/komondor_tutorial.lof](<../Documentation/Tutorials/Tutorial%20%28OLD%29/komondor_tutorial.lof>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/komondor_tutorial.log](<../Documentation/Tutorials/Tutorial%20%28OLD%29/komondor_tutorial.log>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/komondor_tutorial.lot](<../Documentation/Tutorials/Tutorial%20%28OLD%29/komondor_tutorial.lot>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/komondor_tutorial.pdf](<../Documentation/Tutorials/Tutorial%20%28OLD%29/komondor_tutorial.pdf>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/komondor_tutorial.synctex.gz](<../Documentation/Tutorials/Tutorial%20%28OLD%29/komondor_tutorial.synctex.gz>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/komondor_tutorial.tex](<../Documentation/Tutorials/Tutorial%20%28OLD%29/komondor_tutorial.tex>)|官方原有，当前未改||
|[Documentation/Tutorials/Tutorial (OLD)/komondor_tutorial.toc](<../Documentation/Tutorials/Tutorial%20%28OLD%29/komondor_tutorial.toc>)|官方原有，当前未改||
|[Documentation/Tutorials/User guide (OLD)/LaTeX files/bib.bib](<../Documentation/Tutorials/User%20guide%20%28OLD%29/LaTeX%20files/bib.bib>)|官方原有，当前未改||
|[Documentation/Tutorials/User guide (OLD)/LaTeX files/images/example_1_console_output.png](<../Documentation/Tutorials/User%20guide%20%28OLD%29/LaTeX%20files/images/example_1_console_output.png>)|官方原有，当前未改||
|[Documentation/Tutorials/User guide (OLD)/LaTeX files/images/example_1_console_output_wlans.png](<../Documentation/Tutorials/User%20guide%20%28OLD%29/LaTeX%20files/images/example_1_console_output_wlans.png>)|官方原有，当前未改||
|[Documentation/Tutorials/User guide (OLD)/LaTeX files/images/example_2_console_output.png](<../Documentation/Tutorials/User%20guide%20%28OLD%29/LaTeX%20files/images/example_2_console_output.png>)|官方原有，当前未改||
|[Documentation/Tutorials/User guide (OLD)/LaTeX files/images/general_statistics_example.png](<../Documentation/Tutorials/User%20guide%20%28OLD%29/LaTeX%20files/images/general_statistics_example.png>)|官方原有，当前未改||
|[Documentation/Tutorials/User guide (OLD)/LaTeX files/images/komondor_files.png](<../Documentation/Tutorials/User%20guide%20%28OLD%29/LaTeX%20files/images/komondor_files.png>)|官方原有，当前未改||
|[Documentation/Tutorials/User guide (OLD)/LaTeX files/images/komondor_flowchart.pdf](<../Documentation/Tutorials/User%20guide%20%28OLD%29/LaTeX%20files/images/komondor_flowchart.pdf>)|官方原有，当前未改||
|[Documentation/Tutorials/User guide (OLD)/LaTeX files/images/nodes_statistics_example.png](<../Documentation/Tutorials/User%20guide%20%28OLD%29/LaTeX%20files/images/nodes_statistics_example.png>)|官方原有，当前未改||
|[Documentation/Tutorials/User guide (OLD)/LaTeX files/images/output_folder.png](<../Documentation/Tutorials/User%20guide%20%28OLD%29/LaTeX%20files/images/output_folder.png>)|官方原有，当前未改||
|[Documentation/Tutorials/User guide (OLD)/LaTeX files/komondor_user_guide.aux](<../Documentation/Tutorials/User%20guide%20%28OLD%29/LaTeX%20files/komondor_user_guide.aux>)|官方原有，当前未改||
|[Documentation/Tutorials/User guide (OLD)/LaTeX files/komondor_user_guide.bbl](<../Documentation/Tutorials/User%20guide%20%28OLD%29/LaTeX%20files/komondor_user_guide.bbl>)|官方原有，当前未改||
|[Documentation/Tutorials/User guide (OLD)/LaTeX files/komondor_user_guide.blg](<../Documentation/Tutorials/User%20guide%20%28OLD%29/LaTeX%20files/komondor_user_guide.blg>)|官方原有，当前未改||
|[Documentation/Tutorials/User guide (OLD)/LaTeX files/komondor_user_guide.lof](<../Documentation/Tutorials/User%20guide%20%28OLD%29/LaTeX%20files/komondor_user_guide.lof>)|官方原有，当前未改||
|[Documentation/Tutorials/User guide (OLD)/LaTeX files/komondor_user_guide.log](<../Documentation/Tutorials/User%20guide%20%28OLD%29/LaTeX%20files/komondor_user_guide.log>)|官方原有，当前未改||
|[Documentation/Tutorials/User guide (OLD)/LaTeX files/komondor_user_guide.lot](<../Documentation/Tutorials/User%20guide%20%28OLD%29/LaTeX%20files/komondor_user_guide.lot>)|官方原有，当前未改||
|[Documentation/Tutorials/User guide (OLD)/LaTeX files/komondor_user_guide.pdf](<../Documentation/Tutorials/User%20guide%20%28OLD%29/LaTeX%20files/komondor_user_guide.pdf>)|官方原有，当前未改||
|[Documentation/Tutorials/User guide (OLD)/LaTeX files/komondor_user_guide.pyg](<../Documentation/Tutorials/User%20guide%20%28OLD%29/LaTeX%20files/komondor_user_guide.pyg>)|官方原有，当前未改||
|[Documentation/Tutorials/User guide (OLD)/LaTeX files/komondor_user_guide.synctex.gz](<../Documentation/Tutorials/User%20guide%20%28OLD%29/LaTeX%20files/komondor_user_guide.synctex.gz>)|官方原有，当前未改||
|[Documentation/Tutorials/User guide (OLD)/LaTeX files/komondor_user_guide.tex](<../Documentation/Tutorials/User%20guide%20%28OLD%29/LaTeX%20files/komondor_user_guide.tex>)|官方原有，当前未改||
|[Documentation/Tutorials/User guide (OLD)/LaTeX files/komondor_user_guide.toc](<../Documentation/Tutorials/User%20guide%20%28OLD%29/LaTeX%20files/komondor_user_guide.toc>)|官方原有，当前未改||
|[Documentation/Validation/high-density/Komondor/input/nodes/input_nodes_42.csv](<../Documentation/Validation/high-density/Komondor/input/nodes/input_nodes_42.csv>)|官方原有，当前未改||
|[LICENSE](<../LICENSE>)|官方原有，当前未改||
|[Makefile](<../Makefile>)|本地工程/测试/文档||
|[README.local.md](<../README.local.md>)|本地工程/测试/文档||
|[README.md](<../README.md>)|官方原有，本地修改|恢复固定官方提交的完整 README 正文，前置本地导航；原独立版指南保存为 README.local.md。|
|[README_agents.md](<../README_agents.md>)|官方原有，当前未改||
|[build.mk](<../build.mk>)|本地工程/测试/文档||
|[docs/BASIC_NATIVE_COTDMA.md](<../docs/BASIC_NATIVE_COTDMA.md>)|本地工程/测试/文档||
|[docs/BUILD_ENVIRONMENT.md](<../docs/BUILD_ENVIRONMENT.md>)|本地工程/测试/文档||
|[docs/CHANGES.md](<../docs/CHANGES.md>)|本地工程/测试/文档||
|[docs/DEVELOPMENT_RULES.md](<../docs/DEVELOPMENT_RULES.md>)|本地工程/测试/文档||
|[docs/DIRECTORY_LAYOUT.md](<../docs/DIRECTORY_LAYOUT.md>)|本地工程/测试/文档||
|[docs/ROADMAP.md](<../docs/ROADMAP.md>)|本地工程/测试/文档||
|[docs/SHARED_HEADERS.md](<../docs/SHARED_HEADERS.md>)|本地工程/测试/文档||
|[examples/multi_ap_native/README.md](<../examples/multi_ap_native/README.md>)|本地工程/测试/文档||
|[examples/multi_ap_native/config_models](<../examples/multi_ap_native/config_models>)|本地工程/测试/文档||
|[examples/multi_ap_native/mapc.csv](<../examples/multi_ap_native/mapc.csv>)|本地工程/测试/文档||
|[examples/multi_ap_native/mapc_groups.csv](<../examples/multi_ap_native/mapc_groups.csv>)|本地工程/测试/文档||
|[examples/multi_ap_native/nodes.csv](<../examples/multi_ap_native/nodes.csv>)|本地工程/测试/文档||
|[examples/two_bss/config_models](<../examples/two_bss/config_models>)|本地工程/测试/文档||
|[examples/two_bss/mapc_bf.csv](<../examples/two_bss/mapc_bf.csv>)|本地工程/测试/文档||
|[examples/two_bss/mapc_sr.csv](<../examples/two_bss/mapc_sr.csv>)|本地工程/测试/文档||
|[examples/two_bss/mapc_tdma.csv](<../examples/two_bss/mapc_tdma.csv>)|本地工程/测试/文档||
|[examples/two_bss/nodes.csv](<../examples/two_bss/nodes.csv>)|本地工程/测试/文档||
|[examples/two_bss/nodes_bf.csv](<../examples/two_bss/nodes_bf.csv>)|本地工程/测试/文档||
|[examples/two_bss/traffic.csv](<../examples/two_bss/traffic.csv>)|本地工程/测试/文档||
|[provenance/base_to_standalone.patch](<../provenance/base_to_standalone.patch>)|冻结来源/历史证据||
|[provenance/basic_native_changes.patch](<../provenance/basic_native_changes.patch>)|冻结来源/历史证据||
|[provenance/basic_native_implementation.json](<../provenance/basic_native_implementation.json>)|冻结来源/历史证据||
|[provenance/basic_native_reference_check.json](<../provenance/basic_native_reference_check.json>)|冻结来源/历史证据||
|[provenance/basic_native_sanitizer.json](<../provenance/basic_native_sanitizer.json>)|冻结来源/历史证据||
|[provenance/basic_native_validation.json](<../provenance/basic_native_validation.json>)|冻结来源/历史证据||
|[provenance/build_reference.json](<../provenance/build_reference.json>)|冻结来源/历史证据||
|[provenance/comparison.json](<../provenance/comparison.json>)|冻结来源/历史证据||
|[provenance/file_annotations.json](<../provenance/file_annotations.json>)|本地工程/测试/文档||
|[provenance/import_manifest.json](<../provenance/import_manifest.json>)|冻结来源/历史证据||
|[provenance/maintenance_cleanup.json](<../provenance/maintenance_cleanup.json>)|冻结来源/历史证据||
|[provenance/official_materials.json](<../provenance/official_materials.json>)|冻结来源/历史证据||
|[provenance/official_to_local_base.patch](<../provenance/official_to_local_base.patch>)|冻结来源/历史证据||
|[provenance/restored_materials_validation.json](<../provenance/restored_materials_validation.json>)|冻结来源/历史证据||
|[provenance/validation.json](<../provenance/validation.json>)|本地工程/测试/文档||
|[release_notes.md](<../release_notes.md>)|官方原有，当前未改||
|[scripts/build_environment.py](<../scripts/build_environment.py>)|本地工程/测试/文档||
|[scripts/inventory.py](<../scripts/inventory.py>)|本地工程/测试/文档||
|[scripts/read_ap_state.py](<../scripts/read_ap_state.py>)|本地工程/测试/文档||
|[scripts/run.py](<../scripts/run.py>)|本地工程/测试/文档||
|[tests/basic_native_tdma.py](<../tests/basic_native_tdma.py>)|本地工程/测试/文档||
|[tests/check.py](<../tests/check.py>)|本地工程/测试/文档||
|[tests/cpp/test_ap_observation.cpp](<../tests/cpp/test_ap_observation.cpp>)|本地工程/测试/文档||
|[tests/cpp/test_dl_cfend.cpp](<../tests/cpp/test_dl_cfend.cpp>)|本地工程/测试/文档||
|[tests/cpp/test_paper_dl_budget.cpp](<../tests/cpp/test_paper_dl_budget.cpp>)|本地工程/测试/文档||
|[tests/cpp/test_paper_edca.cpp](<../tests/cpp/test_paper_edca.cpp>)|本地工程/测试/文档||

生成产物 `komondor_main.cxx` / `compcxx_komondor_main.h`、仿真二进制、运行结果及生成器编译中间文件不作为人工维护源码。三个自动清单也不记录自己的哈希，以避免循环身份。

官方原有功能包括协作状态机、Co-SR 四种功率策略、ULA/ZF、EDCA/DCB 等；文件改动不能将这些能力重新归为本地新增。未核验的导入文件也不能自动归为本地原创。
