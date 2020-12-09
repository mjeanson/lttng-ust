#ifndef _URCU_ARCH_ALPHA_H
#define _URCU_ARCH_ALPHA_H

/*
 * arch_alpha.h: trivial definitions for the Alpha architecture.
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

#define lttng_ust_mb()			__asm__ __volatile__ ("mb":::"memory")
#define lttng_ust_wmb()			__asm__ __volatile__ ("wmb":::"memory")
#define lttng_ust_read_barrier_depends()	__asm__ __volatile__ ("mb":::"memory")

#ifdef __cplusplus
}
#endif

#include <lttng/urcu/arch/generic.h>

#endif /* _URCU_ARCH_ALPHA_H */
