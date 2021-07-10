add_compile_options(-Og)

set(CORE_SOURCES
    StdLib.cpp
    init.cpp
    boot.S
)

set(AK_SOURCES
)

set(SOURCES
    ${CORE_SOURCES}
    ${AK_SOURCES}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-warning-option -Wvla -Wnull-dereference -std=c++20")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -ffreestanding -fbuiltin")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mno-80387 -mno-mmx -mno-sse -mno-sse2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-asynchronous-unwind-tables")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstack-protector-strong")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -nostdlib -nostdinc -nostdinc++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")

include_directories(${TOOLCHAIN_ROOT}/lib/gcc/${CPU_ARCH}/${GCC_VERSION}/include/)
link_directories(${TOOLCHAIN_ROOT}/lib/gcc/${CPU_ARCH}/${GCC_VERSION}/)
include_directories(${CMAKE_SOURCE_DIR}/)
link_directories(${CMAKE_SOURCE_DIR}/AK/)

add_compile_definitions(CORE)

add_link_options(LINKER:-T ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld -nostdlib)

if (NOT EXISTS ${CMAKE_C_COMPILER})
    message(FATAL_ERROR "Toolchain appears to be missing! Please run: Toolchain/build.sh")
endif()

set(CORE_TARGET image)
add_executable(${CORE_TARGET} ${SOURCES})
