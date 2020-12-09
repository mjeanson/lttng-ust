#ifndef _URCU_ARCH_X86_H
#define _URCU_ARCH_X86_H

/*
 * arch_x86.h: trivial definitions for the x86 architecture.
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

#define LTTNG_UST_CACHE_LINE_SIZE	128

/*
 * For now, using lock; addl compatibility mode even for i686, because the
 * Pentium III is seen as a i686, but lacks mfence instruction.  Only using
 * fence for x86_64.
 *
 * k1om (__MIC__) is the name for the Intel MIC family (Xeon Phi). It is an
 * x86_64 variant but lacks fence instructions.
 */
#if (defined(URCU_ARCH_AMD64) && !defined(URCU_ARCH_K1OM))

#define lttng_ust_mb()    __asm__ __volatile__ ("mfence":::"memory")

/*
 * Define lttng_ust_rmb/lttng_ust_wmb to "strict" barriers that may be needed when
 * using SSE or working with I/O areas.  lttng_ust_smp_rmb/cmm_smp_wmb are
 * only compiler barriers, which is enough for general use.
 */
#define lttng_ust_rmb()     __asm__ __volatile__ ("lfence":::"memory")
#define lttng_ust_wmb()     __asm__ __volatile__ ("sfence"::: "memory")
#define lttng_ust_smp_rmb() lttng_ust_barrier()
#define cmm_smp_wmb() lttng_ust_barrier()

#else

/*
 * We leave smp_rmb/smp_wmb as full barriers for processors that do not have
 * fence instructions.
 *
 * An empty lttng_ust_smp_rmb() may not be enough on old PentiumPro multiprocessor
 * systems, due to an erratum.  The Linux kernel says that "Even distro
 * kernels should think twice before enabling this", but for now let's
 * be conservative and leave the full barrier on 32-bit processors.  Also,
 * IDT WinChip supports weak store ordering, and the kernel may enable it
 * under our feet; cmm_smp_wmb() ceases to be a nop for these processors.
 */
#if (LTTNG_UST_BITS_PER_LONG == 32)
#define lttng_ust_mb()    __asm__ __volatile__ ("lock; addl $0,0(%%esp)":::"memory")
#define lttng_ust_rmb()    __asm__ __volatile__ ("lock; addl $0,0(%%esp)":::"memory")
#define lttng_ust_wmb()    __asm__ __volatile__ ("lock; addl $0,0(%%esp)":::"memory")
#else
#define lttng_ust_mb()    __asm__ __volatile__ ("lock; addl $0,0(%%rsp)":::"memory")
#define lttng_ust_rmb()    __asm__ __volatile__ ("lock; addl $0,0(%%rsp)":::"memory")
#define lttng_ust_wmb()    __asm__ __volatile__ ("lock; addl $0,0(%%rsp)":::"memory")
#endif
#endif

#define caa_cpu_relax()	__asm__ __volatile__ ("rep; nop" : : : "memory")

/*
 * On Linux, define the membarrier system call number if not yet available in
 * the system headers.
 */
#if (defined(__linux__) && !defined(__NR_membarrier))
#if (LTTNG_UST_BITS_PER_LONG == 32)
#define __NR_membarrier		375
#else
#define __NR_membarrier		324
#endif
#endif

#ifdef __cplusplus
}
#endif

#include <lttng/urcu/arch/generic.h>

#endif /* _URCU_ARCH_X86_H */
