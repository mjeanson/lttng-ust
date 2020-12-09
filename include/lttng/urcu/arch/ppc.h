#ifndef _LTTNG_UST_ARCH_PPC_H
#define _LTTNG_UST_ARCH_PPC_H

/*
 * arch_ppc.h: trivial definitions for the powerpc architecture.
 *
 * Copyright (c) 2009 Paul E. McKenney, IBM Corporation.
 * Copyright (c) 2009 Mathieu Desnoyers <mathieu.desnoyers@efficios.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <lttng/urcu/compiler.h>
#include <lttng/ust-config.h>
#include <sys/syscall.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Include size of POWER5+ L3 cache lines: 256 bytes */
#define LTTNG_UST_CACHE_LINE_SIZE	256

#ifdef __NO_LWSYNC__
#define LTTNG_UST_LWSYNC_OPCODE	"sync\n"
#else
#define LTTNG_UST_LWSYNC_OPCODE	"lwsync\n"
#endif

/*
 * Use sync for all lttng_ust_mb/rmb/wmb barriers because lwsync does not
 * preserve ordering of cacheable vs. non-cacheable accesses, so it
 * should not be used to order with respect to MMIO operations.  An
 * eieio+lwsync pair is also not enough for lttng_ust_rmb, because it will
 * order cacheable and non-cacheable memory operations separately---i.e.
 * not the latter against the former.
 */
#define lttng_ust_mb()         __asm__ __volatile__ ("sync":::"memory")

/*
 * lwsync orders loads in cacheable memory with respect to other loads,
 * and stores in cacheable memory with respect to other stores.
 * Therefore, use it for barriers ordering accesses to cacheable memory
 * only.
 */
#define lttng_ust_smp_rmb()    __asm__ __volatile__ (LTTNG_UST_LWSYNC_OPCODE:::"memory")
#define lttng_ust_smp_wmb()    __asm__ __volatile__ (LTTNG_UST_LWSYNC_OPCODE:::"memory")

/*
 * On Linux, define the membarrier system call number if not yet available in
 * the system headers.
 */
#if (defined(__linux__) && !defined(__NR_membarrier))
#define __NR_membarrier		365
#endif

#ifdef __cplusplus
}
#endif

#include <lttng/urcu/arch/generic.h>

#endif /* _LTTNG_UST_ARCH_PPC_H */
