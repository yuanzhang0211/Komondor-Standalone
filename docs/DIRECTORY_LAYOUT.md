# 官方结构与本地维护目录

## 目录原则

以官方固定提交 `a40896d881bfb377c07a9ccef249cdab52849445` 的目录及配套材料为底，继续保留当前仿真器源码中的本地修复和非学习 Co-TDMA 扩展。官方内容不再按“最小可运行后端”裁掉。原始导入记录不改写，本次补全另有完整来源记录。

|位置|来源/用途|维护方式|
|---|---|---|
|`Code/main/`、`methods/`、`structures/`、`list_of_macros.h`|官方仿真器主体，含现有本地修改|三模式共用一套实现；按逐文件清单维护|
|`Code/COST/`、`Code/compcpp/`|官方引擎/生成器及依赖|保留；协议修复不顺带改引擎|
|`Code/input/examples/`|官方普通业务、EDCA、带宽、DSO/NPCA、MAPC 等示例|原位置、原字节保留；mab/external_model 等学习示例仅保留为配套材料，不自动执行|
|`Code/input/validation/`|官方 basic/complex/channel-bonding/spatial-reuse/high-density 输入|原位置保留；短时输入可加载检查与原始长时 KPI 回归分开记录|
|`Code/validate.sh`|官方原始回归脚本|原字节保留；预期值是官方历史基准，不保证修改版全部匹配|
|`Code/scripts_multiple_executions/`|官方批量脚本|原字节保留；其中 Agent 脚本不在默认流程启用|
|`Code/output/`|官方输出目录结构|只保留空目录标记；不迁入旧输出。直接执行原脚本可能重置其固定输出文件|
|`Apps/`|官方输入生成、后处理、Matlab、BeamPlotting 等工具及分发资源|完整保留配套文件，包括随上游分发的库/图示；不作为仿真核心的必需依赖|
|`Documentation/`|官方手册、教程、技术报告、历史验证资料、演示材料|原字节保留，与本地 docs 区分；资料可能面向历史版本|
|`README.md`|官方 README 正文 + 本地导航说明|正文保留官方内容；当前本地构建/验证以 README.local.md 为准|
|`README_agents.md`、`release_notes.md`、`CONTRIBUTING.md`、`CODE_OF_CONDUCT.md`|官方说明与项目治理材料|原字节保留，不表示当前工作范围改为学习|
|`.cproject`、`.settings/`、`.gitattributes`|官方 IDE/仓库配套|保留；统一实际构建仍使用 build.mk|
|`LICENSE`、`CITATION.cff`|官方许可与引用|原样保留；各工具内部许可也随原目录保留|
|`README.local.md`、`AGENTS.md`、`docs/`|本地使用指南、开发规则、改动清单和计划|本地维护；不替代官方 Documentation|
|根 `Makefile`、`build.mk`、`scripts/`|本地统一构建、运行、环境与来源检查|不在 Python 重写 PHY/MAC；不依赖旧 RL 项目|
|`examples/`、`tests/`|本地多 AP 主示例、双 BSS 扩展对照和机制检查|与官方输入/验证分开，避免重写官方历史样例|
|`provenance/`|历史比较、导入/恢复来源、文件身份与验证记录|保持冻结证据；当前台账由 inventory 更新|
|`build/`、`runs/`|本机环境报告与新运行证据|自动生成，不当作官方源文件|

## 官方材料恢复记录与来源边界

官方归档包含 848 个普通文件。逐一验证其 Git blob SHA1 与抓取的固定提交树一致，另记录各文件 SHA256。本次恢复 753 个此前缺失的文件，93 个同路径现存文件保留本地版本。仅不导入官方跟踪的两个生成产物：`Code/main/komondor_main` 和 `compcxx_komondor_main.h`；它们由本地构建产生。

`.git/` 不属于归档配套材料，没有复制本机旧仓库历史或其未提交修改。当前项目仍为独立实际文件，无符号/硬链接。完整记录见 [official_materials.json](../provenance/official_materials.json)。其 action 是恢复时的动作快照；当前身份以 [FILE_INVENTORY.md](FILE_INVENTORY.md) 为准。

官方 `build_local` 原来硬编码 g++/-Werror，本地保留入口但转交 `make`，统一 PROFILE 和编译器参数。官方 `.gitignore` 中残留的合并冲突标记已清理，并合并本地规则；随官方分发的 Apps 资源不能被本地构建目录忽略规则误排除。

## 使用官方示例

通过已有隔离运行入口加载官方 CSV，例如：

```bash
python3 scripts/run.py --mode none --traffic native \
  --nodes Code/input/examples/basic_example/input_nodes.csv \
  --config Code/config_models --time 0.05

python3 scripts/run.py --mode tdma --traffic native \
  --nodes Code/input/examples/mapc_example/input_nodes_mapc_example.csv \
  --mapc Code/input/examples/mapc_example/mapc_cotdma_example.csv \
  --config Code/config_models --time 0.05
```

结果仍写新 `runs/` 目录，不修改原 CSV，也不使用学习 Agent。按官方原有方式直接调用 `Code/main/komondor_main` 仍可用。`Code/main/build_local` 现在与根 make 使用同一工具链。

原始完整回归脚本入口是 `cd Code && bash validate.sh`，需要 Bash/bc 等 shell 工具，默认每场景 100 秒仿真并写固定 `Code/output/` 文件；应在专用副本或空输出目录执行，避免覆盖已有结果。本次只做材料完整性和短时运行检查，不把它们表述为原始长时 KPI 回归通过。

## 辅助应用依赖

核心构建、运行和台账工具继续只需要 C++ 工具链、make、Bash 和 Python 标准库。恢复 Apps 不会自动安装或启动工具：BeamPlotting 需要 NumPy/Matplotlib；Java 工具需要对应 JDK/构建环境；Matlab 脚本需要相应运行环境。其依赖与适用版本以各官方工具说明为准，当前不将全部辅助应用宣称为已运行验证。

## 官方材料恢复时的历史检查

恢复时完成了官方材料完整性、搬迁构建及当时的机制检查。当前版本的检查结果以 [BASIC_NATIVE_COTDMA.md](BASIC_NATIVE_COTDMA.md) 为准。另对 40 组官方输入各运行 0.01 秒：27 组完成，包括原始回归脚本四类中的 18 组输入及 9 组基础/EDCA/带宽/DSO/NPCA/MAPC 示例。

其余 13 组 `Code/input/validation/high_density_scenarios/input_nodes*.csv` 使用历史字段布局（含 destination_id、tpc_min/default/max、cca_min/default/max 等），与当前按位置读取的节点 CSV 布局不兼容，输入校验失败，未进入仿真。原样保留为历史资料，不能当作当前可直接运行的场景；后续如迁移需生成新文件并验证映射，不能覆写官方历史输入。

逐例输入、模式、结果及证据路径见 [restored_materials_validation.json](../provenance/restored_materials_validation.json)。本次没有运行原始每场景 100 秒的完整 KPI 对照，也没有运行全部 Apps 辅助应用。

## 首次提取来源及生成物维护

首次提取源为 `RL_Co-TDMA/third_party/komondor-paper-dl-src`，选用其底层修复与非学习回放扩展，未接入之后的训练/模型后端。初始源码哈希见 [import_manifest.json](../provenance/import_manifest.json)；后续补齐官方目录的哈希见 [official_materials.json](../provenance/official_materials.json)。这些是历史证据，当前身份由 FILE_INVENTORY 更新，不能推断本地历史 checkout 祖先。

编译、运行不访问旧 RL 项目，不应用历史补丁链。`runs/` 和 `build/` 是生成物，不提交 Git；当前 runs 保留四组被报告引用的证据，重复/失败的临时运行已清理。清理记录见 [maintenance_cleanup.json](../provenance/maintenance_cleanup.json)。官方配套材料、节点 CSV 和许可证继续保留，未随本次文档整理删除。
