/*
 * Copyright (c) 2021, Liav A. <liavalb@hotmail.co.il>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/StdLibExtras.h>
#include <AK/Types.h>
#include <Core/StdLib.h>

static u8* s_next_eternal_ptr;
static u8* s_end_of_eternal_range;

// Note: 100 KiB seems like a reasonable amount of memory for a bootloader
// In any case, we can reduce the amount of eternal "heap" and use the e820 memory map
// to find new memory ranges to allocate from.
#define ETERNAL_RANGE_SIZE (100 * KiB)

__attribute__((section(".heap"))) static u8 kmalloc_eternal_heap[ETERNAL_RANGE_SIZE];

void kmalloc_init()
{
    // Zero out heap since it's placed after end_of_kernel_bss.
    memset(kmalloc_eternal_heap, 0, sizeof(kmalloc_eternal_heap));

    s_next_eternal_ptr = kmalloc_eternal_heap;
    s_end_of_eternal_range = s_next_eternal_ptr + sizeof(kmalloc_eternal_heap);
}

// Note: We only need a simple "bump allocator", so kmalloc_eternal will provide that for us.
// Note: We don't need a locking because there are no threads in a bootloader.
void* kmalloc_eternal(size_t size)
{
    size = round_up_to_power_of_two(size, sizeof(void*));
    void* ptr = s_next_eternal_ptr;
    s_next_eternal_ptr += size;
    // FIXME: Add an assertion to ensure we don't allocate too much in the bootloader.
    return ptr;
}

void* kmalloc(size_t size)
{
    return kmalloc_eternal(size);
}

void* krealloc(void*, size_t size)
{
    // Just reallocate everything again.
    return kmalloc_eternal(size);
}

void kfree(void*)
{
    // Note: We don't really want to mess with freeing resources, because this is a bootloader.
    // Just Don't do anything.
}
