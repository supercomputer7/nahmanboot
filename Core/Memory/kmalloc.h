/*
 * Copyright (c) 2021, Liav A. <liavalb@hotmail.co.il>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Types.h>

void kmalloc_init();

void* krealloc(void*, size_t);
void kfree(void*);
void* kmalloc(size_t);
