/*
 * Copyright (c) 2020, Andreas Kling <kling@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <Core/Memory/PhysicalAddress.h>

namespace Core {

template<typename T>
struct TypedMapping {
    const T* ptr() const { return reinterpret_cast<const T*>(m_base_address.offset(offset).as_ptr()); }
    T* ptr() { return reinterpret_cast<T*>(m_base_address.offset(offset).as_ptr()); }
    const T* operator->() const { return ptr(); }
    T* operator->() { return ptr(); }
    const T& operator*() const { return *ptr(); }
    T& operator*() { return *ptr(); }
    PhysicalAddress m_base_address;
    size_t offset { 0 };
};

template<typename T>
static TypedMapping<T> map_typed(PhysicalAddress paddr)
{
    TypedMapping<T> table;
    table.m_base_address = paddr;
    table.offset = paddr.offset_in_page();
    return table;
}

}
