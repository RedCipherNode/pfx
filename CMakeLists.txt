cmake_minimum_required(VERSION 3.21)

project(
    PFX
    VERSION 0.1.0
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(third_party/argon2)
add_subdirectory(third_party/fltk)

# Engine
add_library(
    pfx_engine
    src/engine.cpp
    src/clipboard.cpp
)

target_include_directories(
    pfx_engine
    PUBLIC
    include
)

target_link_libraries(
    pfx_engine
    PUBLIC
    argon2
)

# CLI
add_executable(
    pfx
    apps/cli/main.cpp
    apps/cli/cli.cpp
    apps/cli/commands.cpp
    apps/cli/console.cpp
)

target_link_libraries(
    pfx
    PRIVATE
    pfx_engine
)

# GUI
add_executable(
    pfx-gui
    apps/gui/main.cpp
    apps/gui/window.cpp
    app.rc
)

target_link_libraries(
    pfx-gui
    PRIVATE
    pfx_engine
    fltk
)

# Tools
add_executable(
    pfx-regression
    tests/tools/pfx-regression.cpp
    tests/tools/regression.cpp
)

add_executable(
    pfx-verify
    tests/tools/pfx-verify.cpp
)

add_executable(
    pfx-diff
    tests/tools/pfx-diff.cpp
)

add_executable(
    pfx-analysis
    tests/tools/pfx-analysis.cpp
)

target_link_libraries(
    pfx-regression
    PRIVATE
    pfx_engine
)

target_link_libraries(
    pfx-verify
    PRIVATE
    pfx_engine
)

target_link_libraries(
    pfx-diff
    PRIVATE
    pfx_engine
)

target_link_libraries(
    pfx-analysis
    PRIVATE
    pfx_engine
)