/*
 * Copyright (c) 2021, Liav A. <liavalb@hotmail.co.il>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Format.h>
#include <Core/Assertions.h>
#include <Core/Memory/kmalloc.h>
#include <Core/StdLib.h>
#include <Core/kstdio.h>

void __assertion_failed(const char* msg, const char* file, unsigned line, const char* func)
{
    // Note: If we assert, we can't be sure the regular kmalloc will work, so let's use the eternal kmalloc instead.
    declare_emergency_kmalloc();
    asm volatile("cli");
    dmesgln("ASSERTION FAILED: {}", msg);
    dmesgln("{}:{} in {}", file, line, func);

    abort();
}

void __panic_kmalloc()
{
    asm volatile("cli");
    kernelputstr("PANIC: Out of memory in critical memory allocation section", strlen("PANIC: Out of memory in critical memory allocation section"));
    abort();
}

[[noreturn]] void abort()
{
    asm volatile("cli; hlt");
    __builtin_unreachable();
}
