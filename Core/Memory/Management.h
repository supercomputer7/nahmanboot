/*
 * Copyright (c) 2021, Liav A. <liavalb@hotmail.co.il>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/NonnullOwnPtrVector.h>
#include <AK/Types.h>
#include <Core/Memory/PhysicalAddress.h>
#include <Core/Memory/PhysicalRegion.h>

namespace Core {
class MemoryManagement {
public:
    static MemoryManagement& the();
    bool initialize(PhysicalAddress multiboot_memory_map_physical_address, size_t entries_count);

    MemoryManagement();

private:
    bool m_was_initialized { false };
    NonnullOwnPtrVector<PhysicalRegion> m_regions;
};
}
