cmake_minimum_required(VERSION 3.16)
project(nahmanboot C CXX ASM)

set(TOOLCHAIN_ROOT ${CMAKE_SOURCE_DIR}/Toolchain/local/)
set(TOOLCHAIN_PATH ${TOOLCHAIN_ROOT}/bin)
set(CPU_ARCH "i686-elf")
set(TOOLCHAIN_PREFIX ${TOOLCHAIN_PATH}/${CPU_ARCH}-)

add_compile_options(-Wno-literal-suffix)

add_subdirectory(AK)
add_subdirectory(Core)

add_custom_target(run
    COMMAND ${CMAKE_SOURCE_DIR}/Meta/test.sh
    USES_TERMINAL
)
