/*
 * Copyright (c) 2021, Liav A. <liavalb@hotmail.co.il>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Format.h>
#include <Core/Assertions.h>

void __assertion_failed(const char* msg, const char* file, unsigned line, const char* func)
{
    asm volatile("cli");
    dmesgln("ASSERTION FAILED: {}", msg);
    dmesgln("{}:{} in {}", file, line, func);

    abort();
}
[[noreturn]] void abort()
{
    asm volatile("cli; hlt");
    __builtin_unreachable();
}
