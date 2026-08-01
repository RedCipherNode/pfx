cmake_minimum_required(VERSION 3.21)

project(PFX VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(third_party/argon2)

add_executable(
    pfx
    src/main.cpp
    src/engine.cpp
    src/clipboard.cpp
)

target_link_libraries(
    pfx
    PRIVATE
    argon2
)

cmake_minimum_required(VERSION 3.21)

project(
    PFX
    VERSION 0.1.0
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

target_include_directories(
    pfx
    PRIVATE
    include
)
target_include_directories(
    pfx
    PRIVATE
    include
)
