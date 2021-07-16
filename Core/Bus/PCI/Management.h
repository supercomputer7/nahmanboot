/*
 * Copyright (c) 2021, Liav A. <liavalb@hotmail.co.il>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Types.h>
#include <AK/Vector.h>
#include <Core/Bus/PCI/Device.h>

namespace Core::PCI {

class Management {
    AK_MAKE_ETERNAL
public:
    static Management& the();
    bool initialize();

    Management();

private:
    bool probe_pci() const;
    void enumerate();

    Vector<Device> m_devices;
    bool m_was_initialized { false };
};
}
