# 编译环境与构建约定

## 已验证基准与支持范围

基准核对日期：2026-11-03。原生仿真器与辅助脚本使用以下实际工具链完成构建/验证；这是当前可复现基准，不宣称其他版本已通过。

|项目|当前基准|约定|
|---|---|---|
|系统|Ubuntu 22.04.5 LTS，WSL2，x86_64 Linux|当前支持 Linux；宿主 WSL 内核版本不作为协议依赖|
|C++ 编译器|系统 `/usr/bin/g++`，GCC 11.4.0|默认 g++；显式 `CXX=/path/g++` 可覆盖。其他版本/编译器先 doctor，再完整 test|
|语言标准|C++98|不直接引入 C++11/17/20 特性；迁移语言标准须作为单独变更|
|构建工具|GNU Make 4.3，Bash|统一由根 Makefile 使用公共 build.mk；不维护第二套 IDE/Conda 专用编译参数|
|辅助脚本|系统 Python 3.10.12|要求 Python >=3.10，仅标准库，无 pip/Conda/RL 环境依赖|
|COST 生成器|项目内 `Code/COST/cxx`，Linux x86_64 ELF|依赖系统 glibc、libstdc++、libgcc、libm；不读取旧 RL 项目|
|生成器源码重建|`Code/compcpp/` + flex + g++ + make|只有重建生成器才需要 flex；当前机器未安装，未验证源码重建流程|

基准工具链快照见 `provenance/build_reference.json`。它是历史参考，不能在后续机器检查时覆写。`make doctor` 的当前机器报告另写 `build/environment.json`（生成物，不纳入来源清单）。doctor 会检查编译器、GNU make、COST 是否可启动，以及当前参数能否编译并运行小程序；它不代替仿真器测试。

Ubuntu 22.04 新机器可安装依赖后执行统一入口（这里列出操作方式，不会自动安装）：

```bash
sudo apt-get update
sudo apt-get install build-essential python3 flex
make doctor
make build
make inventory-check
make test
```

无需修改系统软链接、切换默认 Python 或激活原 RL Conda 环境。使用不同解释器时显式传 `PYTHON=/path/python3`；使用其他平台/架构须先重建 COST，再建立该平台的验证记录，当前不承诺原生 Windows/macOS 可直接构建。

## 唯一构建参数入口

配置集中在根 `build.mk`；根 Makefile 与 `Code/main/Makefile` 均包含它。默认 `PROFILE=release`。

|PROFILE|默认 CXXFLAGS|用途|
|---|---|---|
|release|`-O2 -g -Wall -std=c++98`|日常构建及当前回归基准|
|debug|`-O0 -g3 -Wall -std=c++98`|断点及状态机调试|
|sanitize|`-O1 -g -Wall -std=c++98 -fsanitize=address,undefined -fno-omit-frame-pointer`|定位内存/未定义行为；历史代码可能触发检查，不能以能编译视为通过|

```bash
make doctor
make build                       # release
make build PROFILE=debug
make build PROFILE=sanitize
make build PROFILE=release        # 回到基准配置
make test PROFILE=release
```

release 是当前回归基准；sanitize 已在独立副本通过 16 组原生专项（ASan/UBSan，detect_leaks=0），记录见 provenance/basic_native_sanitizer.json。debug 的完整运行矩阵尚未单独验证。不同配置的数值不能无条件混入性能比较。

允许显式覆盖 `CXX`、`PYTHON`、`CXXFLAGS`、`CPPFLAGS`、`LDFLAGS`。覆盖 CXXFLAGS 时是整组替换，须自行保留 `-std=c++98` 等约定，并记录原因；常规开发优先选择 PROFILE。环境中的自定义 CXXFLAGS 也会被采用，`make doctor` 中显示最终值。测试里的 C++ 编译和搬迁构建沿用同一组导出参数。

`Code/main/.build-config` 记录当前编译器命令/路径/版本和有效参数，仅内容变化时更新时间，作为二进制依赖。切换 profile、编译器或 flags 会触发重新链接编译；相同设置不会仅因检查 stamp 重编译。此文件不是人工维护源码，不手改、不归档为官方文件。COST 生成物仍由所有原生头文件/源码和生成器的变化驱动。

不默认使用 `-Werror`（保留的官方代码存在历史警告）、`-ffast-math` 或 `-march=native`。新增警告需分析；编译器升级、浮点优化选项或标准迁移不能顺带进入协议修复。

## 构建产物与并行约定

- 当前为单构建目录，release/debug/sanitize 共用 `Code/main/komondor_main`；不可同时在同一源码目录运行不同构建任务。需要并行配置时使用实际文件的独立副本。
- `Code/main/komondor_main.cxx`、`compcxx_komondor_main.h` 为 COST 生成物，禁止手改；`make clean` 清理原生二进制、生成源码及参数 stamp，不删除运行结果。
- `make rebuild-generator` 需要 flex；重建后生成器身份可能改变，须更新当前清单并跑完整测试。其继承的 `Code/compcpp/Makefile` 有自己的生成器编译参数，不使用仿真器 PROFILE，不因此修改冻结导入记录。
- 所有新运行写入新 `runs/` 目录；运行时不覆盖已有结果，显式整理可删除未引用的重复/临时证据并记录清单。命令与输入/二进制哈希保存在 run.json。涉及可比较结果时另保存本次 `build/environment.json` 和 `Code/main/.build-config`，确保编译环境也可追溯。

## 提交/交付前检查

修改编译入口、参数或环境脚本时执行 `make doctor`、实际重新构建和 `make test`；修改说明同步 `file_annotations.json` 与 `CHANGES.md`，最后更新/检查清单。只改文档可执行 `make inventory` 与 `make inventory-check`。不得把一次 doctor 通过写成三模式 PHY 已验证。

## 官方配套材料的工具链边界

已恢复的 Apps/Documentation/官方输入和脚本不改变核心的标准库 Python 约定。Java、Matlab、NumPy/Matplotlib 等仅为对应辅助工具的可选依赖；原始验证脚本另需 bc。官方 build_local 已转交同目录 make，保持构建参数统一。官方完整目录及使用方式见 DIRECTORY_LAYOUT.md。

## Git 跟踪边界

`.gitignore` 仅按实际生成物路径排除根 runs/build、Code/main 的二进制/生成源码/参数 stamp、compcpp 构建产物、Code/output 中除 .gitkeep 外的输出及工具缓存。不会全局排除 CSV、JSON/JSONL、Markdown、patch、JAR、class、properties 或嵌套 build 目录，避免漏掉官方分发材料。来源 JSON 和补丁保留跟踪；忽略规则不会自动移除已跟踪文件。
