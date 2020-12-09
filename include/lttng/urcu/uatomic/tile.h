#ifndef _LTTNG_UST_ARCH_UATOMIC_TILE_H
#define _LTTNG_UST_ARCH_UATOMIC_TILE_H

/*
 * Copyright (c) 1991-1994 by Xerox Corporation.  All rights reserved.
 * Copyright (c) 1996-1999 by Silicon Graphics.  All rights reserved.
 * Copyright (c) 1999-2004 Hewlett-Packard Development Company, L.P.
 * Copyright (c) 2009-2015 Mathieu Desnoyers
 * Copyright (c) 2010      Paul E. McKenney, IBM Corporation
 *			   (Adapted from uatomic_arch_ppc.h)
 *
 * THIS MATERIAL IS PROVIDED AS IS, WITH ABSOLUTELY NO WARRANTY EXPRESSED
 * OR IMPLIED.  ANY USE IS AT YOUR OWN RISK.
 *
 * Permission is hereby granted to use or copy this program
 * for any purpose,  provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 * Code inspired from libuatomic_ops-1.2, inherited in part from the
 * Boehm-Demers-Weiser conservative garbage collector.
 */

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

#endif /* _LTTNG_UST_ARCH_UATOMIC_TILE_H */
