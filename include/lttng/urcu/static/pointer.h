#ifndef _LTTNG_URCU_POINTER_STATIC_H
#define _LTTNG_URCU_POINTER_STATIC_H

/*
 * lttng/urcu/static/pointer.h
 *
 * Userspace RCU header. Operations on pointers.
 *
 * TO BE INCLUDED ONLY IN CODE THAT IS TO BE RECOMPILED ON EACH LIBURCU
 * RELEASE. See urcu.h for linking dynamically with the userspace rcu library.
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
#include <lttng/urcu/system.h>
#include <lttng/urcu/uatomic.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * _rcu_dereference - reads (copy) a RCU-protected pointer to a local variable
 * into a RCU read-side critical section. The pointer can later be safely
 * dereferenced within the critical section.
 *
 * This ensures that the pointer copy is invariant thorough the whole critical
 * section.
 *
 * Inserts memory barriers on architectures that require them (currently only
 * Alpha) and documents which pointers are protected by RCU.
 *
 * The compiler memory barrier in LTTNG_UST_LOAD_SHARED() ensures that value-speculative
 * optimizations (e.g. VSS: Value Speculation Scheduling) does not perform the
 * data read before the pointer read by speculating the value of the pointer.
 * Correct ordering is ensured because the pointer is read as a volatile access.
 * This acts as a global side-effect operation, which forbids reordering of
 * dependent memory operations. Note that such concern about dependency-breaking
 * optimizations will eventually be taken care of by the "memory_order_consume"
 * addition to forthcoming C++ standard.
 *
 * Should match rcu_assign_pointer() or rcu_xchg_pointer().
 *
 * This macro is less than 10 lines long.  The intent is that this macro
 * meets the 10-line criterion in LGPL, allowing this function to be
 * expanded directly in non-LGPL code.
 */
#define _lttng_ust_rcu_dereference(p)					\
				__extension__				\
				({					\
				__typeof__(p) _________p1 = LTTNG_UST_LOAD_SHARED(p); \
				lttng_ust_smp_read_barrier_depends();		\
				(_________p1);				\
				})

/**
 * _rcu_cmpxchg_pointer - same as rcu_assign_pointer, but tests if the pointer
 * is as expected by "old". If succeeds, returns the previous pointer to the
 * data structure, which can be safely freed after waiting for a quiescent state
 * using synchronize_rcu(). If fails (unexpected value), returns old (which
 * should not be freed !).
 *
 * lttng_ust_uatomic_cmpxchg() acts as both release and acquire barriers.
 *
 * This macro is less than 10 lines long.  The intent is that this macro
 * meets the 10-line criterion in LGPL, allowing this function to be
 * expanded directly in non-LGPL code.
 */
#define _lttng_ust_rcu_cmpxchg_pointer(p, old, _new)			\
	__extension__							\
	({								\
		__typeof__(*p) _________pold = (old);			\
		__typeof__(*p) _________pnew = (_new);			\
		lttng_ust_uatomic_cmpxchg(p, _________pold, _________pnew);	\
	})

/**
 * _rcu_xchg_pointer - same as rcu_assign_pointer, but returns the previous
 * pointer to the data structure, which can be safely freed after waiting for a
 * quiescent state using synchronize_rcu().
 *
 * lttng_ust_uatomic_xchg() acts as both release and acquire barriers.
 *
 * This macro is less than 10 lines long.  The intent is that this macro
 * meets the 10-line criterion in LGPL, allowing this function to be
 * expanded directly in non-LGPL code.
 */
#define _lttng_ust_rcu_xchg_pointer(p, v)		\
	__extension__					\
	({						\
		__typeof__(*p) _________pv = (v);	\
		lttng_ust_uatomic_xchg(p, _________pv);		\
	})


#define _lttng_ust_rcu_set_pointer(p, v)		\
	do {						\
		__typeof__(*p) _________pv = (v);	\
		if (!__builtin_constant_p(v) || 	\
		    ((v) != NULL))			\
			lttng_ust_wmb();			\
		lttng_ust_uatomic_set(p, _________pv);		\
	} while (0)

/**
 * _rcu_assign_pointer - assign (publicize) a pointer to a new data structure
 * meant to be read by RCU read-side critical sections. Returns the assigned
 * value.
 *
 * Documents which pointers will be dereferenced by RCU read-side critical
 * sections and adds the required memory barriers on architectures requiring
 * them. It also makes sure the compiler does not reorder code initializing the
 * data structure before its publication.
 *
 * Should match rcu_dereference().
 *
 * This macro is less than 10 lines long.  The intent is that this macro
 * meets the 10-line criterion in LGPL, allowing this function to be
 * expanded directly in non-LGPL code.
 */
#define _lttng_ust_rcu_assign_pointer(p, v)	_lttng_ust_rcu_set_pointer(&(p), v)

#ifdef __cplusplus
}
#endif

#endif /* _LTTNG_URCU_POINTER_STATIC_H */
