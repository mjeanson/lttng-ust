/*
 * Atomic exchange operations for the m68k architecture. Let GCC do it.
 *
 * Copyright (c) 2017 Michael Jeanson <mjeanson@efficios.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef _LTTNG_UST_ARCH_UATOMIC_M68K_H
#define _LTTNG_UST_ARCH_UATOMIC_M68K_H

#include <lttng/urcu/compiler.h>
#include <lttng/urcu/system.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LTTNG_UST_HAS_ATOMIC_BYTE
#define LTTNG_UST_HAS_ATOMIC_SHORT

#ifdef __cplusplus
}
#endif

#include <lttng/urcu/uatomic/generic.h>

#endif /* _LTTNG_UST_ARCH_UATOMIC_M68K_H */
