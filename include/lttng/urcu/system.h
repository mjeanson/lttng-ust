#ifndef _LTTNG_USTURCU_SYSTEM_H
#define _LTTNG_USTURCU_SYSTEM_H

/*
 * system.h
 *
 * System definitions.
 *
 * Copyright (c) 2009 Mathieu Desnoyers <mathieu.desnoyers@efficios.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 */

#include <lttng/urcu/compiler.h>
#include <lttng/urcu/arch.h>

/*
 * Identify a shared load. A lttng_ust_smp_rmc() or lttng_ust_smp_mc() should come
 * before the load.
 */
#define _LTTNG_UST_LOAD_SHARED(p)	       LTTNG_UST_ACCESS_ONCE(p)

/*
 * Load a data from shared memory, doing a cache flush if required.
 */
#define LTTNG_UST_LOAD_SHARED(p)			\
	__extension__			\
	({				\
		lttng_ust_smp_rmc();		\
		_LTTNG_UST_LOAD_SHARED(p);	\
	})

/*
 * Identify a shared store. A lttng_ust_smp_wmc() or lttng_ust_smp_mc() should
 * follow the store.
 */
#define _LTTNG_UST_STORE_SHARED(x, v)	__extension__ ({ LTTNG_UST_ACCESS_ONCE(x) = (v); })

/*
 * Store v into x, where x is located in shared memory. Performs the
 * required cache flush after writing. Returns v.
 */
#define LTTNG_UST_STORE_SHARED(x, v)						\
	__extension__							\
	({								\
		__typeof__(x) _v = _LTTNG_UST_STORE_SHARED(x, v);		\
		lttng_ust_smp_wmc();						\
		_v = _v;	/* Work around clang "unused result" */	\
	})

#endif /* _LTTNG_USTURCU_SYSTEM_H */
