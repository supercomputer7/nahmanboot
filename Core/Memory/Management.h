/*
 * Copyright (c) 2021, Liav A. <liavalb@hotmail.co.il>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/NonnullOwnPtrVector.h>
#include <AK/Types.h>
#include <Core/Memory/PhysicalAddress.h>
#include <Core/Memory/PhysicalRange.h>
#include <Core/Memory/PhysicalRegion.h>

namespace Core {
class MemoryManagement {
    AK_MAKE_ETERNAL
public:
    static MemoryManagement& the();
    bool initialize(PhysicalAddress multiboot_memory_map_physical_address, size_t entries_count);
    static bool was_initialized();

    MemoryManagement();

    Optional<PhysicalRange> try_to_find_the_biggest_available_range() const;

private:
    bool m_was_initialized { false };
    NonnullOwnPtrVector<PhysicalRegion> m_regions;
};
}
