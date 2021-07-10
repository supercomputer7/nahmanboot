add_compile_options(-Og)

set(CORE_SOURCES
    init.c
    boot.S
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unknown-warning-option -Wvla -Wnull-dereference")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffreestanding -fbuiltin")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mno-80387 -mno-mmx -mno-sse -mno-sse2")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-asynchronous-unwind-tables")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fstack-protector-strong")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -nostdlib -nostdinc")

include_directories(${TOOLCHAIN_ROOT}/lib/gcc/${CPU_ARCH}/${GCC_VERSION}/include/)
link_directories(${TOOLCHAIN_ROOT}/lib/gcc/${CPU_ARCH}/${GCC_VERSION}/)

add_compile_definitions(CORE)

add_link_options(LINKER:-T ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld -nostdlib)

if (NOT EXISTS ${CMAKE_C_COMPILER})
    message(FATAL_ERROR "Toolchain appears to be missing! Please run: toolchain/build.sh")
endif()

set(CORE_TARGET image)
add_executable(${CORE_TARGET} ${CORE_SOURCES})
