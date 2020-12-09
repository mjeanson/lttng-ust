/*
 * urcu/arch.h
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

#ifndef _LTTNG_UST_ARCH_H
#define _LTTNG_UST_ARCH_H

/*
 * Architecture detection using compiler defines.
 *
 * The following defines are used internally for architecture specific code.
 *
 * LTTNG_UST_ARCH_X86 : All x86 variants 32 and 64 bits
 *   LTTNG_UST_ARCH_I386 : Specific to the i386
 *   LTTNG_UST_ARCH_AMD64 : All 64 bits x86 variants
 *   LTTNG_UST_ARCH_K1OM : Specific to the Xeon Phi / MIC
 *
 * LTTNG_UST_ARCH_PPC : All PowerPC variants 32 and 64 bits
 *   LTTNG_UST_ARCH_PPC64 : Specific to 64 bits variants
 *
 * LTTNG_UST_ARCH_S390 : All IBM s390 / s390x variants
 *
 * LTTNG_UST_ARCH_SPARC64 : All Sun SPARC variants
 *
 * LTTNG_UST_ARCH_ALPHA : All DEC Alpha variants
 * LTTNG_UST_ARCH_IA64 : All Intel Itanium variants
 * LTTNG_UST_ARCH_ARM : All ARM 32 bits variants
 * LTTNG_UST_ARCH_AARCH64 : All ARM 64 bits variants
 * LTTNG_UST_ARCH_MIPS : All MIPS variants
 * LTTNG_UST_ARCH_NIOS2 : All Intel / Altera NIOS II variants
 * LTTNG_UST_ARCH_TILE : All Tilera TILE variants
 * LTTNG_UST_ARCH_HPPA : All HP PA-RISC variants
 * LTTNG_UST_ARCH_M68K : All Motorola 68000 variants
 * LTTNG_UST_ARCH_RISCV : All RISC-V variants
 */

#if (defined(__INTEL_OFFLOAD) || defined(__TARGET_ARCH_MIC) || defined(__MIC__))

#define LTTNG_UST_ARCH_X86 1
#define LTTNG_UST_ARCH_AMD64 1
#define LTTNG_UST_ARCH_K1OM 1
#include <lttng/urcu/arch/x86.h>

#elif (defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64))

#define LTTNG_UST_ARCH_X86 1
#define LTTNG_UST_ARCH_AMD64 1
#include <lttng/urcu/arch/x86.h>

#elif (defined(__i486__) || defined(__i586__) || defined(__i686__))

#define LTTNG_UST_ARCH_X86 1
#include <lttng/urcu/arch/x86.h>

#elif (defined(__i386__) || defined(__i386))

#define LTTNG_UST_ARCH_X86 1
#define LTTNG_UST_ARCH_I386 1
#include <lttng/urcu/arch/x86.h>

#elif (defined(__powerpc64__) || defined(__ppc64__))

#define LTTNG_UST_ARCH_PPC 1
#define LTTNG_UST_ARCH_PPC64 1
#include <lttng/urcu/arch/ppc.h>

#elif (defined(__powerpc__) || defined(__powerpc) || defined(__ppc__))

#define LTTNG_UST_ARCH_PPC 1
#include <lttng/urcu/arch/ppc.h>

#elif (defined(__s390__) || defined(__s390x__) || defined(__zarch__))

#define LTTNG_UST_ARCH_S390 1
#include <lttng/urcu/arch/s390.h>

#elif (defined(__sparc__) || defined(__sparc) || defined(__sparc64__))

#define LTTNG_UST_ARCH_SPARC64 1
#include <lttng/urcu/arch/sparc64.h>

#elif (defined(__alpha__) || defined(__alpha))

#define LTTNG_UST_ARCH_ALPHA 1
#include <lttng/urcu/arch/alpha.h>

#elif (defined(__ia64__) || defined(__ia64))

#define LTTNG_UST_ARCH_IA64 1
#include <lttng/urcu/arch/ia64.h>

#elif (defined(__arm__) || defined(__arm))

#define LTTNG_UST_ARCH_ARM 1
#include <lttng/urcu/arch/arm.h>

#elif defined(__aarch64__)

#define LTTNG_UST_ARCH_AARCH64 1
#include <lttng/urcu/arch/aarch64.h>

#elif (defined(__mips__) || defined(__mips))

#define LTTNG_UST_ARCH_MIPS 1
#include <lttng/urcu/arch/mips.h>

#elif (defined(__nios2__) || defined(__nios2))

#define LTTNG_UST_ARCH_NIOS2 1
#include <lttng/urcu/arch/nios2.h>

#elif defined(__tilegx__)
/*
 * URCU has only been tested on the TileGx architecture. For other Tile*
 * architectures, please run the tests first and report the results to the
 * maintainer so that proper support can be added.
 */

#define LTTNG_UST_ARCH_TILE 1
#include <lttng/urcu/arch/tile.h>

#elif (defined(__hppa__) || defined(__HPPA__) || defined(__hppa))

#define LTTNG_UST_ARCH_HPPA 1
#include <lttng/urcu/arch/hppa.h>

#elif defined(__m68k__)

#define LTTNG_UST_ARCH_M68K 1
#include <lttng/urcu/arch/m68k.h>

#elif defined(__riscv)

#define LTTNG_UST_ARCH_RISCV 1
#include <lttng/urcu/arch/riscv.h>

#else
#error "Cannot build: unrecognized architecture, see <lttng/urcu/arch.h>."
#endif


#endif /* _LTTNG_UST_ARCH_H */
