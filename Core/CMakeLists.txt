add_compile_options(-Og)

project(core)
set(CORE_TARGET image)

set(CORE_SOURCES
    StdLib.cpp
    Assertions.cpp
    Memory/kmalloc.cpp
    kprintf.cpp
    init.cpp
    boot.S
)

set(AK_SOURCES
    ../AK/FlyString.cpp
    ../AK/GenericLexer.cpp
    ../AK/Hex.cpp
    ../AK/String.cpp
    ../AK/StringBuilder.cpp
    ../AK/StringImpl.cpp
    ../AK/StringUtils.cpp
    ../AK/StringView.cpp
    ../AK/Format.cpp
    ../AK/UUID.cpp
)

set(SOURCES
    ${CORE_SOURCES}
    ${AK_SOURCES}
)

set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_LINKER ${TOOLCHAIN_PREFIX}ld)
set(CMAKE_RANLIB ${TOOLCHAIN_PREFIX}gcc-ranlib)
set(CMAKE_STRIP ${TOOLCHAIN_PREFIX}strip)
set(CMAKE_AR ${TOOLCHAIN_PREFIX}gcc-ar)
set(GCC_VERSION 11.1.0)

add_executable(${CORE_TARGET} ${SOURCES})
target_link_libraries(image "/home/liav/git-projects/nahman/Toolchain//local/lib/gcc/i686-elf/11.1.0/libgcc.a")

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

#add_link_options(LINKER:-T ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld -nostdlib)
add_link_options(LINKER:-nostdlib)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${CMAKE_SOURCE_DIR}/Core/linker.ld")

if (NOT EXISTS ${CMAKE_C_COMPILER})
    message(FATAL_ERROR "Toolchain appears to be missing! Please run: Toolchain/build.sh")
endif()

#set(LINKER_SCRIPT "/home/liav/git-projects/nahman/Core/linker.ld")
#set_target_properties(${CORE_TARGET} PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})

#add_library(libgcc STATIC "/home/liav/git-projects/nahman/Toolchain//local/lib/gcc/i686-elf/11.1.0/libgcc.a")

