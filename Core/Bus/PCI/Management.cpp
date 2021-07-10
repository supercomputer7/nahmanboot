/*
 * Copyright (c) 2021, Liav A. <liavalb@hotmail.co.il>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Singleton.h>
#include <Core/Bus/PCI/Management.h>
#include <Core/IO.h>

namespace Core::PCI {

static AK::Singleton<Management> s_the;

Management& Management::the()
{
    return *s_the;
}

bool Management::initialize()
{
    VERIFY(!m_was_initialized);
    m_was_initialized = true;
    if (!probe_pci())
        return false;
    enumerate();
    return true;
}

void Management::enumerate()
{
    // Note: this is the most primitive type of PCI bus scan, but it seems to work
    // reliably for PCI devices. For PCIe enumeration, this is not really feasible
    // to scan for 4 billion PCI domains, so when we need to do that, we need something
    // clever to scan the PCIe bus.

    // FIXME: Find a better way to scan.
    // FIXME: Find a better way to represent a PCI address.
    // FIXME: Don't use so many hardcoded values.
    for (u8 bus = 0; bus < 0xFF; bus++) {
        for (u8 slot = 0; slot < 32; slot++) {
            for (u8 function = 0; function < 8; function++) {
                // Select vendor ID register in the PCI config space of the checked device address
                IO::out32(0xcf8, (1 << 31) | (bus << 16) | (slot << 11) | (function << 8));
                u16 vendor_id = IO::in16(0xcfc);
                // Select vendor ID register in the PCI config space of the checked device address
                IO::out32(0xcf8, (1 << 31) | (bus << 16) | (slot << 11) | (function << 8) | 2);
                u16 device_id = IO::in16(0xcfc);
                if (vendor_id == 0xFFFF || device_id == 0xFFFF)
                    continue;
                m_devices.append(Device { false, vendor_id, device_id, 0, bus, slot, function });
                dmesgln("PCI @ {:02x}:{:02x}.{:d} is {:x}:{:x}", bus, slot, function, vendor_id, device_id);
            }
        }
    }
}

bool Management::probe_pci() const
{
    u32 tmp = 0x80000000;
    // We try to probe the 0xcf8 (PCI address) port, to see if it responds with a reasonable value
    IO::out32(0xcf8, tmp);
    tmp = IO::in32(0xcf8);
    // Note: If we get the same value, this is a sign of PCI presence
    // We don't really have a better way to check if there's a PCI bus, so
    // we rely on this to work.
    return tmp == 0x80000000;
}

Management::Management()
{
}

}
