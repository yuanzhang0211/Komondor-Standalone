# Komondor Standalone 本地指南

当前完善原生输入下的基础规则 Co-TDMA，并为每个 AP 输出独立只读状态，供后续一 AP 一 Agent 使用。当前不做需求感知、多 AC 重构或 Agent 动作控制。

项目保留官方 `Code/`、`Apps/`、`Documentation/`、输入、验证脚本及许可；根 [README.md](README.md) 保留官方说明。本地规则与能力以本指南和下列文档为准。

## 构建与运行

当前验证环境为 Linux x86_64、GCC 11.4、GNU Make 4.3、Python 3.10，C++98；核心辅助脚本只用标准库，不需要原 RL 项目、Conda 或 pip 包。完整约定见 [BUILD_ENVIRONMENT.md](docs/BUILD_ENVIRONMENT.md)。

```bash
cd /home/yuanzhang/Project/Komondor-Standalone
make doctor
make build
make smoke       # 四 AP 原生规则与各 AP 独立状态
make basic-test  # 原生 Co-TDMA 专项检查
make test        # 单元、既有回归、搬迁构建和专项检查
```

使用自己的原生 CSV：

```bash
python3 scripts/run.py --mode tdma --traffic native --observe \
  --nodes /path/nodes.csv --mapc /path/mapc.csv \
  --config /path/config_models --time 0.3
```

程序打印新运行目录。读取其中 AP 5 的本地状态：

```bash
python3 scripts/read_ap_state.py runs/本次运行/states --ap 5
```

AP 编号是节点 CSV 数据行的零基索引，可能不连续。各 AP 单独写 `ap_<id>.jsonl`，不汇总其他 AP 私有状态。当前单轮邀请一个 peer，不是一轮给全组 AP 分配时隙。字段、规则与验证结果见 [BASIC_NATIVE_COTDMA.md](docs/BASIC_NATIVE_COTDMA.md)。

## 文档与开发入口

|文档|用途|
|---|---|
|[AGENTS.md](AGENTS.md)|必须遵守的开发约定|
|[BASIC_NATIVE_COTDMA.md](docs/BASIC_NATIVE_COTDMA.md)|当前规则、原生输入、状态契约、验证汇总|
|[DEVELOPMENT_RULES.md](docs/DEVELOPMENT_RULES.md)|公共代码复用、接口和修改记录要求|
|[SHARED_HEADERS.md](docs/SHARED_HEADERS.md)|共享头文件职责与包含边界|
|[HEADER_CATALOG.md](docs/HEADER_CATALOG.md)|自动生成的直接包含关系|
|[FILE_INVENTORY.md](docs/FILE_INVENTORY.md)|自动生成的官方/本地逐文件身份|
|[CHANGES.md](docs/CHANGES.md)|已有修复、当前改动及来源|
|[BUILD_ENVIRONMENT.md](docs/BUILD_ENVIRONMENT.md)|工具链、构建配置、产物约定|
|[DIRECTORY_LAYOUT.md](docs/DIRECTORY_LAYOUT.md)|官方与本地目录职责、来源和历史输入边界|
|[ROADMAP.md](docs/ROADMAP.md)|尚未完成的后续工作|

修改后维护 `provenance/file_annotations.json`，执行 `make inventory` 和 `make inventory-check`。来源补丁和哈希是审阅证据，构建不依赖旧项目或应用补丁链。

## runs 与生成物

`runs/` 是输入副本、日志、指标和逐 AP 状态等运行证据，不是编译/运行依赖，不提交 Git。当前保留被 `provenance/` 报告引用的四组证据，重复和失败的中间运行已清除。删除这些证据不会破坏程序，但会失去对应报告的原始日志；保留规则及本次删除清单见 `provenance/maintenance_cleanup.json`。

新运行仍写新目录，不覆盖旧结果。后续整理可删除未被当前报告引用的临时运行；清理被引用的证据时须同时明确更新报告的证据状态。`build/` 是本机报告、临时编译副本和工具输出，也不提交 Git。`Code/output/` 只跟踪空目录标记。

官方工具随附的 JAR、class、properties、图片以及嵌套 build 资源继续保留，不能用全局扩展名忽略规则误排除。`.gitignore` 不会自动取消已跟踪文件的跟踪。

## 保留的扩展与限制

历史 replay、DL 多 AC、需求感知和 CF-End 研究扩展仍在源码中；必须显式使用 `--traffic replay` 等配置，不在当前默认路径启用。Co-SR/Co-BF 保留后续开发入口，尚未完成 PHY 正确性验证。旧 high_density CSV 使用历史字段布局，当前不能直接读取，原始文件保留。

上游学习组件因构建依赖继续存在，默认不启用；官方辅助 Apps 的 Java、Matlab、绘图依赖与核心构建分开。许可证与引用文件继续保留在原位置。
