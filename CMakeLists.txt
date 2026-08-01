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

# CLI
add_executable(
    pfx
    src/main.cpp
)

target_link_libraries(
    pfx
    PRIVATE
    pfx_engine
)

# Regression Tool
add_executable(
    pfx-regression
    tests/regression/pfx-regression.cpp
)

target_link_libraries(
    pfx-regression
    PRIVATE
    pfx_engine
)