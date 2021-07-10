/*
 * Copyright (c) 2018-2020, Andreas Kling <kling@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// FIXME: Add include to <Core/kstdio.h> when implemented
#ifdef Core
#endif
extern "C" {
void dbgputstr(const char*, size_t);
int sprintf(char* buf, const char* fmt, ...) __attribute__((format(printf, 2, 3)));
int snprintf(char* buffer, size_t, const char* fmt, ...) __attribute__((format(printf, 3, 4)));
}

inline void dbgputstr(const char* characters, size_t length)
{
    fwrite(characters, 1, length, stderr);
}
template<size_t N>
inline void dbgputstr(const char (&array)[N])
{
    return ::dbgputstr(array, N);
}
