#ifndef _LTTNG_UST_ARCH_UATOMIC_X86_H
#define _LTTNG_UST_ARCH_UATOMIC_X86_H

/*
 * Copyright (c) 1991-1994 by Xerox Corporation.  All rights reserved.
 * Copyright (c) 1996-1999 by Silicon Graphics.  All rights reserved.
 * Copyright (c) 1999-2004 Hewlett-Packard Development Company, L.P.
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

#include <lttng/urcu/arch.h>
#include <lttng/ust-config.h>
#include <lttng/urcu/compiler.h>
#include <lttng/urcu/system.h>

#define LTTNG_UST_HAS_ATOMIC_BYTE
#define LTTNG_UST_HAS_ATOMIC_SHORT

#ifdef __cplusplus
extern "C" {
#endif

#if ((LTTNG_UST_BITS_PER_LONG != 64) && defined(LTTNG_UST_ARCH_I386))
#error "i386 lacks the cmpxchg instruction, build for i486 and up."
#endif

/*
 * Derived from AO_compare_and_swap() and AO_test_and_set_full().
 */

struct __lttng_ust_uatomic_dummy {
	unsigned long v[10];
};
#define __lttng_ust_hp(x)	((struct __lttng_ust_uatomic_dummy *)(x))

#define lttng_ust_uatomic_set(addr, v)	((void) LTTNG_UST_STORE_SHARED(*(addr), (v)))

/* cmpxchg */

static inline __attribute__((always_inline))
unsigned long __lttng_ust_cmpxchg(void *addr, unsigned long old,
			      unsigned long _new, int len)
{
	switch (len) {
	case 1:
	{
		unsigned char result = old;

		__asm__ __volatile__(
		"lock; cmpxchgb %2, %1"
			: "+a"(result), "+m"(*__lttng_ust_hp(addr))
			: "q"((unsigned char)_new)
			: "memory");
		return result;
	}
	case 2:
	{
		unsigned short result = old;

		__asm__ __volatile__(
		"lock; cmpxchgw %2, %1"
			: "+a"(result), "+m"(*__lttng_ust_hp(addr))
			: "r"((unsigned short)_new)
			: "memory");
		return result;
	}
	case 4:
	{
		unsigned int result = old;

		__asm__ __volatile__(
		"lock; cmpxchgl %2, %1"
			: "+a"(result), "+m"(*__lttng_ust_hp(addr))
			: "r"((unsigned int)_new)
			: "memory");
		return result;
	}
#if (LTTNG_UST_BITS_PER_LONG == 64)
	case 8:
	{
		unsigned long result = old;

		__asm__ __volatile__(
		"lock; cmpxchgq %2, %1"
			: "+a"(result), "+m"(*__lttng_ust_hp(addr))
			: "r"((unsigned long)_new)
			: "memory");
		return result;
	}
#endif
	}
	/*
	 * generate an illegal instruction. Cannot catch this with
	 * linker tricks when optimizations are disabled.
	 */
	__asm__ __volatile__("ud2");
	return 0;
}

#define uatomic_cmpxchg(addr, old, _new)				      \
	((__typeof__(*(addr))) __lttng_ust_cmpxchg((addr),		      \
						lttng_ust_cast_long_keep_sign(old), \
						lttng_ust_cast_long_keep_sign(_new),\
						sizeof(*(addr))))

/* xchg */

static inline __attribute__((always_inline))
unsigned long __lttng_ust_exchange(void *addr, unsigned long val, int len)
{
	/* Note: the "xchg" instruction does not need a "lock" prefix. */
	switch (len) {
	case 1:
	{
		unsigned char result;
		__asm__ __volatile__(
		"xchgb %0, %1"
			: "=q"(result), "+m"(*__lttng_ust_hp(addr))
			: "0" ((unsigned char)val)
			: "memory");
		return result;
	}
	case 2:
	{
		unsigned short result;
		__asm__ __volatile__(
		"xchgw %0, %1"
			: "=r"(result), "+m"(*__lttng_ust_hp(addr))
			: "0" ((unsigned short)val)
			: "memory");
		return result;
	}
	case 4:
	{
		unsigned int result;
		__asm__ __volatile__(
		"xchgl %0, %1"
			: "=r"(result), "+m"(*__lttng_ust_hp(addr))
			: "0" ((unsigned int)val)
			: "memory");
		return result;
	}
#if (LTTNG_UST_BITS_PER_LONG == 64)
	case 8:
	{
		unsigned long result;
		__asm__ __volatile__(
		"xchgq %0, %1"
			: "=r"(result), "+m"(*__lttng_ust_hp(addr))
			: "0" ((unsigned long)val)
			: "memory");
		return result;
	}
#endif
	}
	/*
	 * generate an illegal instruction. Cannot catch this with
	 * linker tricks when optimizations are disabled.
	 */
	__asm__ __volatile__("ud2");
	return 0;
}

#define uatomic_xchg(addr, v)						      \
	((__typeof__(*(addr))) __lttng_ust_exchange((addr),		      \
						lttng_ust_cast_long_keep_sign(v),   \
						sizeof(*(addr))))

/* uatomic_add_return */

static inline __attribute__((always_inline))
unsigned long __lttng_ust_add_return(void *addr, unsigned long val,
				 int len)
{
	switch (len) {
	case 1:
	{
		unsigned char result = val;

		__asm__ __volatile__(
		"lock; xaddb %1, %0"
			: "+m"(*__lttng_ust_hp(addr)), "+q" (result)
			:
			: "memory");
		return result + (unsigned char)val;
	}
	case 2:
	{
		unsigned short result = val;

		__asm__ __volatile__(
		"lock; xaddw %1, %0"
			: "+m"(*__lttng_ust_hp(addr)), "+r" (result)
			:
			: "memory");
		return result + (unsigned short)val;
	}
	case 4:
	{
		unsigned int result = val;

		__asm__ __volatile__(
		"lock; xaddl %1, %0"
			: "+m"(*__lttng_ust_hp(addr)), "+r" (result)
			:
			: "memory");
		return result + (unsigned int)val;
	}
#if (LTTNG_UST_BITS_PER_LONG == 64)
	case 8:
	{
		unsigned long result = val;

		__asm__ __volatile__(
		"lock; xaddq %1, %0"
			: "+m"(*__lttng_ust_hp(addr)), "+r" (result)
			:
			: "memory");
		return result + (unsigned long)val;
	}
#endif
	}
	/*
	 * generate an illegal instruction. Cannot catch this with
	 * linker tricks when optimizations are disabled.
	 */
	__asm__ __volatile__("ud2");
	return 0;
}

#define uatomic_add_return(addr, v)					    \
	((__typeof__(*(addr))) __lttng_ust_add_return((addr),		    \
						lttng_ust_cast_long_keep_sign(v), \
						sizeof(*(addr))))

/* uatomic_and */

static inline __attribute__((always_inline))
void __lttng_ust_and(void *addr, unsigned long val, int len)
{
	switch (len) {
	case 1:
	{
		__asm__ __volatile__(
		"lock; andb %1, %0"
			: "=m"(*__lttng_ust_hp(addr))
			: "iq" ((unsigned char)val)
			: "memory");
		return;
	}
	case 2:
	{
		__asm__ __volatile__(
		"lock; andw %1, %0"
			: "=m"(*__lttng_ust_hp(addr))
			: "ir" ((unsigned short)val)
			: "memory");
		return;
	}
	case 4:
	{
		__asm__ __volatile__(
		"lock; andl %1, %0"
			: "=m"(*__lttng_ust_hp(addr))
			: "ir" ((unsigned int)val)
			: "memory");
		return;
	}
#if (LTTNG_UST_BITS_PER_LONG == 64)
	case 8:
	{
		__asm__ __volatile__(
		"lock; andq %1, %0"
			: "=m"(*__lttng_ust_hp(addr))
			: "er" ((unsigned long)val)
			: "memory");
		return;
	}
#endif
	}
	/*
	 * generate an illegal instruction. Cannot catch this with
	 * linker tricks when optimizations are disabled.
	 */
	__asm__ __volatile__("ud2");
	return;
}

#define uatomic_and(addr, v)						   \
	(__lttng_ust_and((addr), lttng_ust_cast_long_keep_sign(v), sizeof(*(addr))))

/* uatomic_or */

static inline __attribute__((always_inline))
void __lttng_ust_or(void *addr, unsigned long val, int len)
{
	switch (len) {
	case 1:
	{
		__asm__ __volatile__(
		"lock; orb %1, %0"
			: "=m"(*__lttng_ust_hp(addr))
			: "iq" ((unsigned char)val)
			: "memory");
		return;
	}
	case 2:
	{
		__asm__ __volatile__(
		"lock; orw %1, %0"
			: "=m"(*__lttng_ust_hp(addr))
			: "ir" ((unsigned short)val)
			: "memory");
		return;
	}
	case 4:
	{
		__asm__ __volatile__(
		"lock; orl %1, %0"
			: "=m"(*__lttng_ust_hp(addr))
			: "ir" ((unsigned int)val)
			: "memory");
		return;
	}
#if (LTTNG_UST_BITS_PER_LONG == 64)
	case 8:
	{
		__asm__ __volatile__(
		"lock; orq %1, %0"
			: "=m"(*__lttng_ust_hp(addr))
			: "er" ((unsigned long)val)
			: "memory");
		return;
	}
#endif
	}
	/*
	 * generate an illegal instruction. Cannot catch this with
	 * linker tricks when optimizations are disabled.
	 */
	__asm__ __volatile__("ud2");
	return;
}

#define uatomic_or(addr, v)						   \
	(__lttng_ust_or((addr), lttng_ust_cast_long_keep_sign(v), sizeof(*(addr))))

/* uatomic_add */

static inline __attribute__((always_inline))
void __lttng_ust_add(void *addr, unsigned long val, int len)
{
	switch (len) {
	case 1:
	{
		__asm__ __volatile__(
		"lock; addb %1, %0"
			: "=m"(*__lttng_ust_hp(addr))
			: "iq" ((unsigned char)val)
			: "memory");
		return;
	}
	case 2:
	{
		__asm__ __volatile__(
		"lock; addw %1, %0"
			: "=m"(*__lttng_ust_hp(addr))
			: "ir" ((unsigned short)val)
			: "memory");
		return;
	}
	case 4:
	{
		__asm__ __volatile__(
		"lock; addl %1, %0"
			: "=m"(*__lttng_ust_hp(addr))
			: "ir" ((unsigned int)val)
			: "memory");
		return;
	}
#if (LTTNG_UST_BITS_PER_LONG == 64)
	case 8:
	{
		__asm__ __volatile__(
		"lock; addq %1, %0"
			: "=m"(*__lttng_ust_hp(addr))
			: "er" ((unsigned long)val)
			: "memory");
		return;
	}
#endif
	}
	/*
	 * generate an illegal instruction. Cannot catch this with
	 * linker tricks when optimizations are disabled.
	 */
	__asm__ __volatile__("ud2");
	return;
}

#define uatomic_add(addr, v)						   \
	(__lttng_ust_add((addr), lttng_ust_cast_long_keep_sign(v), sizeof(*(addr))))


/* uatomic_inc */

static inline __attribute__((always_inline))
void __lttng_ust_inc(void *addr, int len)
{
	switch (len) {
	case 1:
	{
		__asm__ __volatile__(
		"lock; incb %0"
			: "=m"(*__lttng_ust_hp(addr))
			:
			: "memory");
		return;
	}
	case 2:
	{
		__asm__ __volatile__(
		"lock; incw %0"
			: "=m"(*__lttng_ust_hp(addr))
			:
			: "memory");
		return;
	}
	case 4:
	{
		__asm__ __volatile__(
		"lock; incl %0"
			: "=m"(*__lttng_ust_hp(addr))
			:
			: "memory");
		return;
	}
#if (LTTNG_UST_BITS_PER_LONG == 64)
	case 8:
	{
		__asm__ __volatile__(
		"lock; incq %0"
			: "=m"(*__lttng_ust_hp(addr))
			:
			: "memory");
		return;
	}
#endif
	}
	/* generate an illegal instruction. Cannot catch this with linker tricks
	 * when optimizations are disabled. */
	__asm__ __volatile__("ud2");
	return;
}

#define uatomic_inc(addr)	(__lttng_ust_inc((addr), sizeof(*(addr))))

/* uatomic_dec */

static inline __attribute__((always_inline))
void __lttng_ust_dec(void *addr, int len)
{
	switch (len) {
	case 1:
	{
		__asm__ __volatile__(
		"lock; decb %0"
			: "=m"(*__lttng_ust_hp(addr))
			:
			: "memory");
		return;
	}
	case 2:
	{
		__asm__ __volatile__(
		"lock; decw %0"
			: "=m"(*__lttng_ust_hp(addr))
			:
			: "memory");
		return;
	}
	case 4:
	{
		__asm__ __volatile__(
		"lock; decl %0"
			: "=m"(*__lttng_ust_hp(addr))
			:
			: "memory");
		return;
	}
#if (LTTNG_UST_BITS_PER_LONG == 64)
	case 8:
	{
		__asm__ __volatile__(
		"lock; decq %0"
			: "=m"(*__lttng_ust_hp(addr))
			:
			: "memory");
		return;
	}
#endif
	}
	/*
	 * generate an illegal instruction. Cannot catch this with
	 * linker tricks when optimizations are disabled.
	 */
	__asm__ __volatile__("ud2");
	return;
}

#define uatomic_dec(addr)	(__lttng_ust_dec((addr), sizeof(*(addr))))

#define lttng_ust_smp_mb__before_uatomic_and()		lttng_ust_barrier()
#define lttng_ust_smp_mb__after_uatomic_and()		lttng_ust_barrier()

#define lttng_ust_smp_mb__before_uatomic_or()		lttng_ust_barrier()
#define lttng_ust_smp_mb__after_uatomic_or()		lttng_ust_barrier()

#define lttng_ust_smp_mb__before_uatomic_add()		lttng_ust_barrier()
#define lttng_ust_smp_mb__after_uatomic_add()		lttng_ust_barrier()

#define lttng_ust_smp_mb__before_uatomic_inc()		lttng_ust_barrier()
#define lttng_ust_smp_mb__after_uatomic_inc()		lttng_ust_barrier()

#define lttng_ust_smp_mb__before_uatomic_dec()		lttng_ust_barrier()
#define lttng_ust_smp_mb__after_uatomic_dec()		lttng_ust_barrier()

#ifdef __cplusplus
}
#endif

#include <lttng/urcu/uatomic/generic.h>

#endif /* _LTTNG_UST_ARCH_UATOMIC_X86_H */
