# 开发与复用规则

生效范围为整个独立项目；自动化修改入口见根目录 [AGENTS.md](../AGENTS.md)。本文件约束后续实现和来源维护；当前基础 Co-TDMA 的能力与状态契约见 BASIC_NATIVE_COTDMA.md。

## 模块边界

|层|唯一职责/现有位置|约束|
|---|---|---|
|引擎/代码生成|`Code/COST/`、`Code/compcpp/`|作为基础依赖；协议补丁不改引擎|
|公共数据|`Code/structures/`、`Code/list_of_macros.h`|Notification/TxInfo、FIFO、Wlan、配置、状态、统计统一定义；新增字段兼容现有模式|
|PHY 通用计算|`Code/methods/phy/`|功率换算、路径损耗、阵列增益、带宽、MCS、帧时长统一实现|
|MAC 通用计算|`Code/methods/mac/`、`Code/methods/frames/`|NAV、退避、PER 判决、通知和聚合算法复用|
|节点事件/状态|`Code/main/node.h`、`Code/methods/node/`|事件调度、控制帧、超时、ACK 提交等仅保留一套驱动；模式差异以显式条件或小函数表达|
|输入及报告|`Code/methods/utils/`|共用配置解析/校验、统计和输出；不在三个模式各建 CSV 解析器或 KPI 公式|
|运行工具|`scripts/`、`examples/`|负责输入、选择规则和结果归档；不在 Python 重写 PHY/MAC|

“公共”表示同一职责有唯一实现，不表示每个头都能随处 include。公共算法头、类型头与 Node/Komondor 实现片段的包含规则不同，详见 [共享头文件目录](SHARED_HEADERS.md)。

## 新功能落点的判定顺序

1. 查 [全头文件目录](HEADER_CATALOG.md) 和 `rg`，确认已有类型、公式、事件入口及使用者。
2. 已有公共职责：在原实现修复，并检查全部调用方；禁止建立 `*_sr_copy.h`、`*_bf_v2.h` 或复制整套 Node 以绕过公共修改。
3. 模式专属策略：把新增选择/可行性计算限制在清晰的小函数；实际发送、接收、出队、NAV/超时仍调用公共路径。
4. 出现第二个真正调用方才提炼新公共模块；先划定语义、单位和状态所有权，不为未来假设建立空壳框架。
5. 新增头文件登记职责、适用模式、包含入口、上游/本地来源、验证；共用接口只有一个定义位置。

当前 `ComputeCoSRTxPowers()` 实现位于 `node_packet_methods.h`，BF 数值实现位于 `beamforming_methods.h`。`mapc_methods.h` 没有工作中的 peer 选择/功率实现；`node_spatial_reuse_methods.h` 主要负责 OBSS/PD SR，不可按文件名直接等同于 Co-SR。后续若拆分 Co-SR 策略，只迁移实现及更新调用，不留下两份同名公式。

## 接口约定

- 单位必须明确：时间 s、预算另有 us 时显式转换；dBm/dB 与线性 pW 不混用；区分发射总功率、每信道功率和含波束增益的接收功率。遵循既有 API 的实际单位，不进行无声明单位迁移。
- BF 矩阵必须写明行列数、行跨度、复数实虚部、共轭与归一化规则；缓冲区构造和求解器使用同一布局。
- 通知中的协作模式与实际执行模式必须区分；Co-SR 回退后的 DATA、ACK、padding、NAV 和超时均按实际执行方式处理。
- 队列仅有一个真实所有者；发送暂存/重传不得提前出队，ACK 经共同事务验证后提交；统计使用同一事件事实，不各自推导“成功”。
- 公共类型不依赖 Node/Komondor 实现片段；可独立的数值函数不调仿真时钟、日志或队列；状态驱动可调用这些底层函数，反向依赖禁止新增。
- 新类型头须有 include guard；已有含非 inline 自由函数的实现头不视为通用多翻译单元库。维持 COST 当前单生成入口，新增包含关系需确认无重复定义。

## 变更必须交付的记录

|内容|维护位置|
|---|---|
|触发问题、最终行为、受影响模式、验证及限制|`docs/CHANGES.md`|
|每个修改/新增原生文件的职责及修改内容|`provenance/file_annotations.json`|
|文件官方身份、当前 SHA256、修改层次|`make inventory` 自动生成的 FILE_INVENTORY / file_inventory|
|新增共享接口职责、复用方式及模式边界|`docs/SHARED_HEADERS.md` 与自动头文件目录|
|未解决问题及计划|`docs/ROADMAP.md`|

历史 comparison 是固定官方提交与历史后端的逐文件证据，不是独立版当前清单。当前清单用官方 SHA256 判定未改/已改，用导入 SHA256 区分历史本地改动与独立化后改动；不能仅因文件头有官方版权就认定文件未被修改。

`make inventory-check` 检查当前清单是否与文件内容及包含关系一致，并要求本地修改/新增的原生文件有说明，同时检查 Node/Komondor 实现片段的唯一直接包含入口。它不能自动判断两段实现语义重复，代码评审仍须核查复用规则。
