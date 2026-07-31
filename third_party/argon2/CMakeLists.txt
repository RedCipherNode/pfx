add_library(argon2 STATIC
    src/argon2.c
    src/core.c
    src/encoding.c
    src/thread.c
    src/opt.c
    src/blake2/blake2b.c
)

target_include_directories(argon2
    PUBLIC
    include
    src
    src/blake2
)

target_compile_definitions(
    argon2
    PRIVATE
    _CRT_SECURE_NO_WARNINGS
)