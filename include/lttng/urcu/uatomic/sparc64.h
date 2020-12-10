#ifndef _LTTNG_UST_ARCH_UATOMIC_SPARC64_H
#define _LTTNG_UST_ARCH_UATOMIC_SPARC64_H

/*
 * Copyright (c) 1991-1994 by Xerox Corporation.  All rights reserved.
 * Copyright (c) 1996-1999 by Silicon Graphics.  All rights reserved.
 * Copyright (c) 1999-2003 by Hewlett-Packard Company. All rights reserved.
 * Copyright (c) 2009      Mathieu Desnoyers
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

/* cmpxchg */

static inline __attribute__((always_inline))
unsigned long _lttng_ust_uatomic_cmpxchg(void *addr, unsigned long old,
			      unsigned long _new, int len)
{
	switch (len) {
	case 4:
	{
		__asm__ __volatile__ (
			"membar #StoreLoad | #LoadLoad\n\t"
                        "cas [%1],%2,%0\n\t"
                        "membar #StoreLoad | #StoreStore\n\t"
                        : "+&r" (_new)
                        : "r" (addr), "r" (old)
                        : "memory");

		return _new;
	}
#if (LTTNG_UST_BITS_PER_LONG == 64)
	case 8:
	{
		__asm__ __volatile__ (
			"membar #StoreLoad | #LoadLoad\n\t"
                        "casx [%1],%2,%0\n\t"
                        "membar #StoreLoad | #StoreStore\n\t"
                        : "+&r" (_new)
                        : "r" (addr), "r" (old)
                        : "memory");

		return _new;
	}
#endif
	}
	__builtin_trap();
	return 0;
}


#define uatomic_cmpxchg(addr, old, _new)				       \
	((__typeof__(*(addr))) _lttng_ust_uatomic_cmpxchg((addr),			       \
						lttng_ust_cast_long_keep_sign(old),  \
						lttng_ust_cast_long_keep_sign(_new), \
						sizeof(*(addr))))

#ifdef __cplusplus
}
#endif

#include <lttng/urcu/uatomic/generic.h>

#endif /* _LTTNG_UST_ARCH_UATOMIC_PPC_H */
