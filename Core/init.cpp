/*
 * Copyright (c) 2021, Liav A. <liavalb@hotmail.co.il>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Array.h>
#include <AK/Format.h>
#include <AK/Types.h>
#include <Core/Bus/PCI/Management.h>
#include <Core/Memory/kmalloc.h>

extern u32 __stack_chk_guard;
u32 __stack_chk_guard;

namespace Core {

extern "C" void* multiboot_info_ptr;
extern "C" [[noreturn]] void core_init() __attribute__((used));

void core_init()
{
    kmalloc_init();

    if (!PCI::Management::the().initialize())
        VERIFY_NOT_REACHED();
    VERIFY_NOT_REACHED();
}

extern "C" {
void* multiboot_info_ptr;
}

// Define some Itanium C++ ABI methods to stop the linker from complaining.
// If we actually call these something has gone horribly wrong
void* __dso_handle __attribute__((visibility("hidden")));

}
