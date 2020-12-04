#ifndef _LTTNG_UST_URCU_POINTER_H
#define _LTTNG_UST_URCU_POINTER_H

/*
 * lttng/urcu/pointer.h
 *
 * Userspace RCU header. Operations on pointers.
 *
 * Copyright (c) 2009 Mathieu Desnoyers <mathieu.desnoyers@efficios.com>
 * Copyright (c) 2009 Paul E. McKenney, IBM Corporation.
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
 *
 * IBM's contributions to this file may be relicensed under LGPLv2 or later.
 */

#include <lttng/urcu/compiler.h>
#include <lttng/urcu/arch.h>
#include <lttng/urcu/uatomic.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_LGPL_SOURCE) || defined(LTTNG_UST_URCU_INLINE_SMALL_FUNCTIONS)

#include <lttng/urcu/static/pointer.h>

/*
 * lttng_ust_rcu_dereference(ptr)
 *
 * Fetch a RCU-protected pointer. Typically used to copy the variable ptr to a
 * local variable.
 */
#define lttng_ust_rcu_dereference		_lttng_ust_rcu_dereference

/*
 * type *lttng_ust_rcu_cmpxchg_pointer(type **ptr, type *new, type *old)
 * type *lttng_ust_rcu_xchg_pointer(type **ptr, type *new)
 * void lttng_ust_rcu_set_pointer(type **ptr, type *new)
 *
 * RCU pointer updates.
 * @ptr: address of the pointer to modify
 * @new: new pointer value
 * @old: old pointer value (expected)
 *
 * return: old pointer value
 */
#define lttng_ust_rcu_cmpxchg_pointer	_lttng_ust_rcu_cmpxchg_pointer
#define lttng_ust_rcu_xchg_pointer	_lttng_ust_rcu_xchg_pointer
#define lttng_ust_rcu_set_pointer	_lttng_ust_rcu_set_pointer

#else /* !(defined(_LGPL_SOURCE) || defined(LTTNG_UST_URCU_INLINE_SMALL_FUNCTIONS)) */

extern void *lttng_ust_rcu_dereference_sym(void *p);
#define lttng_ust_rcu_dereference(p)					     \
	__extension__							     \
	({								     \
		__typeof__(p) _________p1 =	LTTNG_UST_URCU_FORCE_CAST(__typeof__(p), \
			lttng_ust_rcu_dereference_sym(LTTNG_UST_URCU_FORCE_CAST(void *, p))); \
		(_________p1);						     \
	})

extern void *lttng_ust_rcu_cmpxchg_pointer_sym(void **p, void *old, void *_new);
#define lttng_ust_rcu_cmpxchg_pointer(p, old, _new)			     \
	__extension__							     \
	({								     \
		__typeof__(*(p)) _________pold = (old);			     \
		__typeof__(*(p)) _________pnew = (_new);		     \
		__typeof__(*(p)) _________p1 = LTTNG_UST_URCU_FORCE_CAST(__typeof__(*(p)), \
			lttng_ust_rcu_cmpxchg_pointer_sym(LTTNG_UST_URCU_FORCE_CAST(void **, p), \
						_________pold,		     \
						_________pnew));	     \
		(_________p1);						     \
	})

extern void *lttng_ust_rcu_xchg_pointer_sym(void **p, void *v);
#define lttng_ust_rcu_xchg_pointer(p, v)				     \
	__extension__							     \
	({								     \
		__typeof__(*(p)) _________pv = (v);		             \
		__typeof__(*(p)) _________p1 = LTTNG_UST_URCU_FORCE_CAST(__typeof__(*(p)), \
			lttng_ust_rcu_xchg_pointer_sym(LTTNG_UST_URCU_FORCE_CAST(void **, p), \
					     _________pv));		     \
		(_________p1);						     \
	})

/*
 * Note: lttng_ust_rcu_set_pointer_sym returns @v because we don't want to break
 * the ABI. At the API level, lttng_ust_rcu_set_pointer() now returns void. Use of
 * the return value is therefore deprecated, and will cause a build
 * error.
 */
extern void *lttng_ust_rcu_set_pointer_sym(void **p, void *v);
#define lttng_ust_rcu_set_pointer(p, v)					     \
	do {								     \
		__typeof__(*(p)) _________pv = (v);		             \
		(void) lttng_ust_rcu_set_pointer_sym(LTTNG_UST_URCU_FORCE_CAST(void **, p), \
					    _________pv);		     \
	} while (0)

#endif /* !(defined(_LGPL_SOURCE) || defined(LTTNG_UST_URCU_INLINE_SMALL_FUNCTIONS)) */

/*
 * void lttng_ust_rcu_assign_pointer(type *ptr, type *new)
 *
 * Same as lttng_ust_rcu_set_pointer, but takes the pointer to assign to rather than its
 * address as first parameter. Provided for compatibility with the Linux kernel
 * RCU semantic.
 */
#define lttng_ust_rcu_assign_pointer(p, v)	lttng_ust_rcu_set_pointer((&p), (v))

#ifdef __cplusplus
}
#endif

#endif /* _LTTNG_UST_URCU_POINTER_H */
