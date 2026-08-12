# QElectroTech — agent guide

Qt5 C++17 CMake CAD/CAE app for electrical schematics. GPLv2.

## User preferences

- Language: Chinese (Chain of Thought and respond in Chinese unless technical terms require English)
- 不要反复读取文件，充分利用上下文
- 当我要求你对项目的功能进行分析、设计新的方案时：
  - 将内容以 Markdown 格式，保存到项目的 `/design/plan/YYYYMMDD-<主题简述>.md`（例如：`20260624-用户认证模块设计.md`）
  - 方案文档中的代码片段必须与项目实际代码风格完全一致
  - **禁止**在代码片段旁添加风格说明类无效文本；纯说明文字仅保留设计理由与操作信息

## Build

**通过 qt-creator MCP 构建/调试/运行，不要直接在 WSL shell 里跑 cmake。**

### Qt Creator MCP 工具

| 操作 | MCP 工具 |
|---|---|
| 构建（当前项目） | `qt-creator_build` |
| 运行 | `qt-creator_run_project` |
| 调试（启动调试会话） | `qt-creator_debug`，配合 `qt-creator_get_*`/`qt-creator_debugger_*` 系列 |
| 测试 | `qt-creator_run_tests`（构建+执行），详见 Tests |
| 查看错误/警告 | `qt-creator_list_issues` |

### 构建环境注意事项

- 项目在 Windows 侧 Qt Creator 打开，源码位于 WSL UNC 路径 `\\wsl.localhost\Ubuntu-26.04\home\zmz\qelectrotech-source-mirror`，工具链为 Windows 侧 Qt 官方 MinGW（CMake `D:/QT/Tools/CMake_64`、编译器 `D:/QT/Tools/mingw810_64`、Qt `D:/QT/5.15.2/mingw81_64`、Ninja `C:/msys64/mingw64`）。
- **构建目录固定为 `build/debug`
- Legacy `qelectrotech.pro` (qmake) still exists — **prefer CMake**.
- Dependencies: Qt5 (Widgets, Svg, Sql, Xml, PrintSupport, Concurrent, Network), SQLite3, pugixml (bundled via git submodule), SingleApplication (v3.2.0, bundled via FetchContent).
- KF5 (kcoreaddons, kwidgetsaddons) fetched at tag `v5.77.0` from KDE Git when `BUILD_WITH_KF5=ON`. Set `BUILD_KF5=OFF` to use system packages instead.
- Use `-DBUILD_WITH_KF5=OFF` on systems without KF5.
- CI forces `-DCMAKE_DISABLE_FIND_PACKAGE_Qt6=ON` and removes Qt6 cmake paths to avoid Qt6/Qt5 conflicts.
- Binary: `qelectrotech` (or `qelectrotech.exe` on Windows with `/SUBSYSTEM:WINDOWS`).

## Tests

Controlled by `PACKAGE_TESTS` (not standard `BUILD_TESTING`). Three test frameworks:

| Dir | Framework | Target |
|---|---|---|
| `tests/catch/` | Catch2 v2.13.10 (fetched) | `C_unittests` |
| `tests/googletest/` | GoogleTest (bundled) | — |
| `tests/qttest/` | QtTest | `qt_unittests` |

**通过 Qt Creator MCP 运行测试：** `qt-creator_run_tests`（构建+执行所有已发现测试）。可选 `scope`/`names` 参数缩小范围。失败详情用 `qt-creator_get_test_details`，测试清单用 `qt-creator_list_tests`。不要用 `ctest`（本地工具链无法在 WSL UNC 路径下工作）。

`BUILD_TESTING=OFF` is set in CI release builds — tests are **not** compiled by default with `-DPACKAGE_TESTS=OFF` (default is ON in CMakeLists.txt but OFF in CI).

## Key architecture

- Entrypoint: `sources/main.cpp:169` — handles CLI export mode **before** SingleApplication init.
- CLI export (`--export`): runs a `QApplication`, disables crash-recovery backups, exits. GUI path uses `SingleApplication` (prevents duplicate instances, forwards args to running instance).
- `QT_HASH_SEED=0` is hardcoded in `main.cpp` for deterministic XML output.
- Logs written to `dataDir/yyyyMMdd.log`, auto-cleaned after 7 days.
- UI `.ui` files live in `sources/ui/` and subdirectories; `CMAKE_AUTOUIC_SEARCH_PATHS` points there.
- Translations: `lang/*.ts` → compiled `.qm` via `qt5_add_translation`. Set 

## Code style

- **Tabs for indentation**, interpreted as 8 spaces. `UseTab: Always` in `.clang-format`.
- 80-column limit (`ColumnLimit: '80'`).
- Allman brace style (`BreakBeforeBraces: Allman`).
- Pointer alignment: left (`PointerAlignment: Left`).
- Run `clang-format` before committing (config at `.clang-format`).

## CMake options & defines

| Option | Default | Effect |
|---|---|---|
| `BUILD_WITH_KF5` | ON | Link KDE Frameworks 5 (kcoreaddons, kwidgetsaddons) |
| `PACKAGE_TESTS` | ON | Build test subdirectories |
| `BUILD_PUGIXML` | YES | Build bundled pugixml vs system |

Always-on defines: `QET_ALLOW_OVERRIDE_CED_OPTION`, `QET_ALLOW_OVERRIDE_CD_OPTION`, `QET_ALLOW_OVERRIDE_DD_OPTION`, `QET_ALLOW_OVERRIDE_CTBTD_OPTION`, `QT_DEPRECATED_WARNINGS`, `QT_MESSAGELOGCONTEXT`. Uncomment `QT_DISABLE_DEPRECATED_BEFORE=0x060000` in `cmake/developer_options.cmake` to error on Qt5-deprecated APIs.

## Git submodules

```
pugixml/       — XML parser (https://github.com/zeux/pugixml)
SingleApplication/ — single-instance lock (https://github.com/itay-grudev/SingleApplication)
elements/      — element collection (https://github.com/qelectrotech/qelectrotech-elements)
doxygen-awesome-css/ — Doxygen theme
```

Clone with `git submodule update --init --recursive`.

## CI notes

- All workflows are **schedule + workflow_dispatch only** (no push/PR triggers).
- Windows-only builds (MSYS2 MinGW). No Linux/macOS CI.
- Version extracted from `sources/qetversion.cpp` at build time.
- Nightly release published to `nightly` tag (`.exe` installer + `.zip` portable).
- Packaging scripts exist for AppImage, Flatpak, Debian, Windows NSIS.

## CLI arguments

The app accepts runtime overrides for data/config directories (compile-time opt-in):
- `--common-elements-dir <path>` (requires `QET_ALLOW_OVERRIDE_CED_OPTION`)
- `--common-tbt-dir <path>` (requires `QET_ALLOW_OVERRIDE_CTBTD_OPTION`)
- `--config-dir <path>` (requires `QET_ALLOW_OVERRIDE_CD_OPTION`)
- `--data-dir <path>` (requires `QET_ALLOW_OVERRIDE_DD_OPTION`)
- `--export <project.qet> --output <file> --format <pdf|png|svg>` (headless CLI export)

## graphify

This project has a knowledge graph at graphify-out/ with god nodes, community structure, and cross-file relationships.

When the user types `/graphify`, use the installed graphify skill or instructions before doing anything else.

Rules:
- For codebase questions, first run `graphify query "<question>"` when graphify-out/graph.json exists. Use `graphify path "<A>" "<B>"` for relationships and `graphify explain "<concept>"` for focused concepts. These return a scoped subgraph, usually much smaller than GRAPH_REPORT.md or raw grep output.
- Dirty graphify-out/ files are expected after hooks or incremental updates; dirty graph files are not a reason to skip graphify. Only skip graphify if the task is about stale or incorrect graph output, or the user explicitly says not to use it.
- If graphify-out/wiki/index.md exists, use it for broad navigation instead of raw source browsing.
- Read graphify-out/GRAPH_REPORT.md only for broad architecture review or when query/path/explain do not surface enough context.
- After modifying code, run `graphify update .` to keep the graph current (AST-only, no API cost).
