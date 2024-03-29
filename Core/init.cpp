/*
 * Copyright (c) 2021, Liav A. <liavalb@hotmail.co.il>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Array.h>
#include <AK/Format.h>
#include <AK/Types.h>
#include <Core/Bus/PCI/Management.h>
#include <Core/Images/Multiboot.h>
#include <Core/Memory/Management.h>
#include <Core/Memory/PhysicalAddress.h>
#include <Core/Memory/kmalloc.h>

extern u32 __stack_chk_guard;
u32 __stack_chk_guard;

namespace Core {

extern "C" Multiboot::ProvidedInfo* multiboot_info_ptr;
extern "C" [[noreturn]] void core_init() __attribute__((used));

void core_init()
{
    kmalloc_eternal_init();
    // Note: For now, just assert if the pointer to multiboot provided info is
    // null, because we can't do anything useful yet without it.
    // FIXME: Remove this once we have our own bootloader stub at the MBR to
    // load this code and provide the memory map to to the bootloader.
    VERIFY(multiboot_info_ptr != nullptr);

    if (multiboot_info_ptr->mmap_addr == 0) {
        VERIFY_NOT_REACHED();
    }
    MemoryManagement::the().initialize(PhysicalAddress(multiboot_info_ptr->mmap_addr), multiboot_info_ptr->mmap_length / sizeof(Multiboot::MemoryMapEntry));
    kmalloc_init();

    if (!PCI::Management::the().initialize())
        VERIFY_NOT_REACHED();
    kmalloc_stats stats;
    get_kmalloc_stats(stats);
    dbgln("kmalloc stats, bytes allocated {}, bytes eternal {}", stats.bytes_allocated, stats.bytes_eternal);

    VERIFY_NOT_REACHED();
}

extern "C" {
Multiboot::ProvidedInfo* multiboot_info_ptr;
}

// Define some Itanium C++ ABI methods to stop the linker from complaining.
// If we actually call these something has gone horribly wrong
void* __dso_handle __attribute__((visibility("hidden")));

}
