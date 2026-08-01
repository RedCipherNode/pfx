cmake_minimum_required(VERSION 3.21)

project(
    PFX
    VERSION 0.1.0
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(third_party/argon2)

# Engine Library
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

# Applications
add_executable(
    pfx
    src/main.cpp
)

target_link_libraries(
    pfx
    PRIVATE
    pfx_engine
)

# Regression Tools
add_executable(
    pfx-regression
    tests/tools/pfx-regression.cpp
    tests/tools/regression.cpp
)

target_link_libraries(
    pfx-regression
    PRIVATE
    pfx_engine
)

add_executable(
    pfx-verify
    tests/tools/pfx-verify.cpp
)

target_link_libraries(
    pfx-verify
    PRIVATE
    pfx_engine
)

add_executable(
    pfx-diff
    tests/tools/pfx-diff.cpp
)

target_link_libraries(
    pfx-diff
    PRIVATE
    pfx_engine
)

add_executable(
    pfx-analysis
    tests/tools/pfx-analysis.cpp
)

target_link_libraries(
    pfx-analysis
    PRIVATE
    pfx_engine
)