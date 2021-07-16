/*
 * Copyright (c) 2021, Liav A. <liavalb@hotmail.co.il>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Types.h>

namespace Core::PCI {

struct Device {
    const bool m_pci_express_connected;
    const u16 m_vendor_id;
    const u16 m_device_id;
    const u32 m_domain;
    const u8 m_bus;
    const u8 m_slot;
    const u8 m_function;
};
}
