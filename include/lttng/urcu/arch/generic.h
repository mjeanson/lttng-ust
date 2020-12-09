#ifndef _URCU_ARCH_GENERIC_H
#define _URCU_ARCH_GENERIC_H

/*
 * arch_generic.h: common definitions for multiple architectures.
 *
 * Copyright (c) 2010 Paolo Bonzini <pbonzini@redhat.com>
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

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LTTNG_UST_CACHE_LINE_SIZE
#define LTTNG_UST_CACHE_LINE_SIZE	64
#endif

#if !defined(lttng_ust_mc) && !defined(lttng_ust_rmc) && !defined(lttng_ust_wmc)
/*
 * Architectures with cache coherency must _not_ define lttng_ust_mc/lttng_ust_rmc/lttng_ust_wmc.
 *
 * For them, lttng_ust_mc/lttng_ust_rmc/lttng_ust_wmc are implemented with a simple
 * compiler barrier; in addition, we provide defaults for lttng_ust_mb (using
 * GCC builtins) as well as lttng_ust_rmb and lttng_ust_wmb (defaulting to lttng_ust_mb).
 */

#ifndef lttng_ust_mb
#define lttng_ust_mb()    __sync_synchronize()
#endif

#ifndef lttng_ust_rmb
#define lttng_ust_rmb()	lttng_ust_mb()
#endif

#ifndef lttng_ust_wmb
#define lttng_ust_wmb()	lttng_ust_mb()
#endif

#define lttng_ust_mc()	lttng_ust_barrier()
#define lttng_ust_rmc()	lttng_ust_barrier()
#define lttng_ust_wmc()	lttng_ust_barrier()
#else
/*
 * Architectures without cache coherency need something like the following:
 *
 * #define lttng_ust_mc()	arch_cache_flush()
 * #define lttng_ust_rmc()	arch_cache_flush_read()
 * #define lttng_ust_wmc()	arch_cache_flush_write()
 *
 * Of these, only lttng_ust_mc is mandatory. lttng_ust_rmc and lttng_ust_wmc default to
 * lttng_ust_mc. lttng_ust_mb/lttng_ust_rmb/lttng_ust_wmb use these definitions by default:
 *
 * #define lttng_ust_mb()	lttng_ust_mc()
 * #define lttng_ust_rmb()	lttng_ust_rmc()
 * #define lttng_ust_wmb()	lttng_ust_wmc()
 */

#ifndef lttng_ust_mb
#define lttng_ust_mb()	lttng_ust_mc()
#endif

#ifndef lttng_ust_rmb
#define lttng_ust_rmb()	lttng_ust_rmc()
#endif

#ifndef lttng_ust_wmb
#define lttng_ust_wmb()	lttng_ust_wmc()
#endif

#ifndef lttng_ust_rmc
#define lttng_ust_rmc()	lttng_ust_mc()
#endif

#ifndef lttng_ust_wmc
#define lttng_ust_wmc()	lttng_ust_mc()
#endif
#endif

/* Nop everywhere except on alpha. */
#ifndef lttng_ust_read_barrier_depends
#define lttng_ust_read_barrier_depends()
#endif

#ifndef lttng_ust_smp_mb
#define lttng_ust_smp_mb()	lttng_ust_mb()
#endif
#ifndef lttng_ust_smp_rmb
#define lttng_ust_smp_rmb()	lttng_ust_rmb()
#endif
#ifndef lttng_ust_smp_wmb
#define lttng_ust_smp_wmb()	lttng_ust_wmb()
#endif
#ifndef lttng_ust_smp_mc
#define lttng_ust_smp_mc()	lttng_ust_mc()
#endif
#ifndef lttng_ust_smp_rmc
#define lttng_ust_smp_rmc()	lttng_ust_rmc()
#endif
#ifndef cmm_smp_wmc
#define cmm_smp_wmc()	lttng_ust_wmc()
#endif
#ifndef cmm_smp_read_barrier_depends
#define cmm_smp_read_barrier_depends()	lttng_ust_read_barrier_depends()
#endif

#ifndef caa_cpu_relax
#define caa_cpu_relax()		lttng_ust_barrier()
#endif

#ifdef __cplusplus
}
#endif

#endif /* _URCU_ARCH_GENERIC_H */
