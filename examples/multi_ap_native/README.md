# 多 AP 原生基础 Co-TDMA 示例

四个 AP 的节点 ID 为 0、2、5、7，分别关联 1、2、1、3 个 STA；未假定 AP ID 连续。nodes.csv 为原生满缓存模型，MAPC 原生字段不增加列。mapc.csv 是同一组四 AP；mapc_groups.csv 是 A/B/C 与 A/C/D 两个重叠组。

`make smoke` 运行此示例。状态文件各自属于一个 AP，没有集中式状态文件；读取方式与限制见 ../../docs/BASIC_NATIVE_COTDMA.md。传播模型 13 为本地确定性 TGn D 中值模型，用于构造可相互接收控制帧的回归，不是性能论文场景。原官方输入仍在 Code/input 中原样保留。
