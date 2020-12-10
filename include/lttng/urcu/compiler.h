#ifndef _LTTNG_UST_URCU_COMPILER_H
#define _LTTNG_UST_URCU_COMPILER_H

/*
 * compiler.h
 *
 * Compiler definitions.
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

#include <stddef.h>	/* for offsetof */

#define lttng_ust_likely(x)	__builtin_expect(!!(x), 1)
#define lttng_ust_unlikely(x)	__builtin_expect(!!(x), 0)

#define	lttng_ust_barrier()	__asm__ __volatile__ ("" : : : "memory")

/*
 * Instruct the compiler to perform only a single access to a variable
 * (prohibits merging and refetching). The compiler is also forbidden to reorder
 * successive instances of CMM_ACCESS_ONCE(), but only when the compiler is aware of
 * particular ordering. Compiler ordering can be ensured, for example, by
 * putting two CMM_ACCESS_ONCE() in separate C statements.
 *
 * This macro does absolutely -nothing- to prevent the CPU from reordering,
 * merging, or refetching absolutely anything at any time.  Its main intended
 * use is to mediate communication between process-level code and irq/NMI
 * handlers, all running on the same CPU.
 */
#define LTTNG_UST_ACCESS_ONCE(x)	(*(__volatile__  __typeof__(x) *)&(x))

#if defined(__SIZEOF_LONG__)
#define LTTNG_UST_BITS_PER_LONG	(__SIZEOF_LONG__ * 8)
#elif defined(_LP64)
#define LTTNG_UST_BITS_PER_LONG	64
#else
#define LTTNG_UST_BITS_PER_LONG	32
#endif

/*
 * lttng_ust_container_of - Get the address of an object containing a field.
 *
 * @ptr: pointer to the field.
 * @type: type of the object.
 * @member: name of the field within the object.
 */
#define lttng_ust_container_of(ptr, type, member)				\
	__extension__							\
	({								\
		const __typeof__(((type *) NULL)->member) * __ptr = (ptr); \
		(type *)((char *)__ptr - offsetof(type, member));	\
	})

#ifdef __cplusplus
#define LTTNG_UST_URCU_FORCE_CAST(type, arg)	(reinterpret_cast<type>(arg))
#else
#define LTTNG_UST_URCU_FORCE_CAST(type, arg)	((type) (arg))
#endif

/*
 * Cast to unsigned long, sign-extending if @v is signed.
 * Note: casting to a larger type or to same type size keeps the sign of
 * the expression being cast (see C99 6.3.1.3).
 */
#define lttng_ust_cast_long_keep_sign(v)	((unsigned long) (v))

#define LTTNG_UST_ARRAY_SIZE(x)	(sizeof(x) / sizeof((x)[0]))

#ifdef __GNUC__
# define LTTNG_UST_GCC_VERSION	(__GNUC__ * 10000 \
				+ __GNUC_MINOR__ * 100 \
				+ __GNUC_PATCHLEVEL__)
#endif

#endif /* _LTTNG_UST_URCU_COMPILER_H */
