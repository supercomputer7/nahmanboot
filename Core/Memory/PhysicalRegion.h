/*
 * Copyright (c) 2021, Liav A. <liavalb@hotmail.co.il>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/NonnullOwnPtr.h>
#include <AK/Types.h>
#include <Core/Images/Multiboot.h>
#include <Core/Memory/PhysicalAddress.h>

namespace Core {
class PhysicalRegion {
public:
    // Note: This adopts e820 entry types as the "standard".
    // Multiboot protocol 1 (as well as the linux boot protocol for providing memory map) also adopts this convention.
    enum class Type {
        Usable = 1,
        Reserved = 2,
        ACPIReclaimable = 3,
        ACPINVS = 4,
        BadMemory = 5,
    };

public:
    static NonnullOwnPtr<PhysicalRegion> create(PhysicalAddress, size_t, Multiboot::MemoryEntryType);

    PhysicalAddress base_address() const { return m_base_address; }
    size_t length() const { return m_length; }
    Type type() const { return m_type; }

private:
    PhysicalRegion(PhysicalAddress, size_t, Type);

    const PhysicalAddress m_base_address;
    const size_t m_length;
    const Type m_type;
};
}
