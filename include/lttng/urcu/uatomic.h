/*
 * urcu/uatomic.h
 *
 * Copyright (c) 2020 Michael Jeanson <michael.jeanson@efficios.com>
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

#ifndef _URCU_UATOMIC_H
#define _URCU_UATOMIC_H

#include <lttng/urcu/arch.h>

#if defined(LTTNG_UST_ARCH_X86)
#include <lttng/urcu/uatomic/x86.h>
#elif defined(LTTNG_UST_ARCH_PPC)
#include <lttng/urcu/uatomic/ppc.h>
#elif defined(LTTNG_UST_ARCH_S390)
#include <lttng/urcu/uatomic/s390.h>
#elif defined(LTTNG_UST_ARCH_SPARC64)
#include <lttng/urcu/uatomic/sparc64.h>
#elif defined(LTTNG_UST_ARCH_ALPHA)
#include <lttng/urcu/uatomic/alpha.h>
#elif defined(LTTNG_UST_ARCH_IA64)
#include <lttng/urcu/uatomic/ia64.h>
#elif defined(LTTNG_UST_ARCH_ARM)
#include <lttng/urcu/uatomic/arm.h>
#elif defined(LTTNG_UST_ARCH_AARCH64)
#include <lttng/urcu/uatomic/aarch64.h>
#elif defined(LTTNG_UST_ARCH_MIPS)
#include <lttng/urcu/uatomic/mips.h>
#elif defined(LTTNG_UST_ARCH_NIOS2)
#include <lttng/urcu/uatomic/nios2.h>
#elif defined(LTTNG_UST_ARCH_TILE)
#include <lttng/urcu/uatomic/tile.h>
#elif defined(LTTNG_UST_ARCH_HPPA)
#include <lttng/urcu/uatomic/hppa.h>
#elif defined(LTTNG_UST_ARCH_M68K)
#include <lttng/urcu/uatomic/m68k.h>
#elif defined(LTTNG_UST_ARCH_RISCV)
#include <lttng/urcu/uatomic/riscv.h>
#else
#error "Cannot build: unrecognized architecture, see <lttng/urcu/arch.h>."
#endif

#endif /* _URCU_UATOMIC_H */
