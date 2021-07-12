/*
 * Copyright (c) 2021, Liav A. <liavalb@hotmail.co.il>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Singleton.h>
#include <AK/kstdio.h>
#include <Core/Images/Multiboot.h>
#include <Core/Memory/Management.h>

namespace Core {

static AK::Singleton<MemoryManagement> s_the;

MemoryManagement::MemoryManagement()
{
}

MemoryManagement& MemoryManagement::the()
{
    return *s_the;
}

bool MemoryManagement::initialize(PhysicalAddress multiboot_memory_map_physical_address, size_t entries_count)
{
    VERIFY(!multiboot_memory_map_physical_address.is_null());
    if (!multiboot_memory_map_physical_address.is_contained_in_32_bits_space())
        return false;
    VERIFY(!m_was_initialized);
    m_was_initialized = true;
    // FIXME: Try to find a way to not use a reinterpret_cast...
    auto* entry = reinterpret_cast<Multiboot::MemoryMapEntry*>(multiboot_memory_map_physical_address.as_ptr());
    for (size_t index = 0; index < entries_count; index++) {
        auto range_base_addr = ((u64)entry->addr_low + ((u64)entry->addr_high << 32));
        auto length = (u64)entry->len_low + ((u64)entry->len_high << 32);
        auto range_end_addr = range_base_addr + length;
        auto type = (Multiboot::MemoryEntryType)entry->type;
        dbgln("multiboot-e820: range {:016x} - {:016x}, {}", range_base_addr, range_end_addr, Multiboot::parse_entry_type(type));
        m_regions.append(PhysicalRegion::create(PhysicalAddress(range_base_addr), length, type));
        entry++;
    }
    return true;
}

}
