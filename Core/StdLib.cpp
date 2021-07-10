/*
 * Copyright (c) 2018-2020, Andreas Kling <kling@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// Functions that are automatically called by the C++ compiler.
// Declare them first, to tell the silly compiler that they are indeed being used.
[[noreturn]] void __stack_chk_fail_local() __attribute__((used));
extern "C" int __cxa_atexit(void (*)(void*), void*, void*);
[[noreturn]] void __cxa_pure_virtual();

extern "C" [[noreturn]] void __stack_chk_fail() __attribute__((used));
[[noreturn]] void __stack_chk_fail()
{
    asm volatile("cli; hlt");
    __builtin_unreachable();
}

[[noreturn]] void __stack_chk_fail_local()
{
    asm volatile("cli; hlt");
    __builtin_unreachable();
}

extern "C" int __cxa_atexit(void (*)(void*), void*, void*)
{
    return 0;
}

[[noreturn]] void __cxa_pure_virtual()
{
    asm volatile("cli; hlt");
    __builtin_unreachable();
}
