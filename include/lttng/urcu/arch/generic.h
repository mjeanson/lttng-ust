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

#if !defined(lttng_ust_mc) && !defined(cmm_rmc) && !defined(cmm_wmc)
/*
 * Architectures with cache coherency must _not_ define lttng_ust_mc/cmm_rmc/cmm_wmc.
 *
 * For them, lttng_ust_mc/cmm_rmc/cmm_wmc are implemented with a simple
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
#define cmm_rmc()	lttng_ust_barrier()
#define cmm_wmc()	lttng_ust_barrier()
#else
/*
 * Architectures without cache coherency need something like the following:
 *
 * #define lttng_ust_mc()	arch_cache_flush()
 * #define cmm_rmc()	arch_cache_flush_read()
 * #define cmm_wmc()	arch_cache_flush_write()
 *
 * Of these, only lttng_ust_mc is mandatory. cmm_rmc and cmm_wmc default to
 * lttng_ust_mc. lttng_ust_mb/lttng_ust_rmb/lttng_ust_wmb use these definitions by default:
 *
 * #define lttng_ust_mb()	lttng_ust_mc()
 * #define lttng_ust_rmb()	cmm_rmc()
 * #define lttng_ust_wmb()	cmm_wmc()
 */

#ifndef lttng_ust_mb
#define lttng_ust_mb()	lttng_ust_mc()
#endif

#ifndef lttng_ust_rmb
#define lttng_ust_rmb()	cmm_rmc()
#endif

#ifndef lttng_ust_wmb
#define lttng_ust_wmb()	cmm_wmc()
#endif

#ifndef cmm_rmc
#define cmm_rmc()	lttng_ust_mc()
#endif

#ifndef cmm_wmc
#define cmm_wmc()	lttng_ust_mc()
#endif
#endif

/* Nop everywhere except on alpha. */
#ifndef cmm_read_barrier_depends
#define cmm_read_barrier_depends()
#endif

#ifndef cmm_smp_mb
#define cmm_smp_mb()	lttng_ust_mb()
#endif
#ifndef cmm_smp_rmb
#define cmm_smp_rmb()	lttng_ust_rmb()
#endif
#ifndef cmm_smp_wmb
#define cmm_smp_wmb()	lttng_ust_wmb()
#endif
#ifndef cmm_smp_mc
#define cmm_smp_mc()	lttng_ust_mc()
#endif
#ifndef cmm_smp_rmc
#define cmm_smp_rmc()	cmm_rmc()
#endif
#ifndef cmm_smp_wmc
#define cmm_smp_wmc()	cmm_wmc()
#endif
#ifndef cmm_smp_read_barrier_depends
#define cmm_smp_read_barrier_depends()	cmm_read_barrier_depends()
#endif

#ifndef caa_cpu_relax
#define caa_cpu_relax()		lttng_ust_barrier()
#endif

#ifdef __cplusplus
}
#endif

#endif /* _URCU_ARCH_GENERIC_H */
