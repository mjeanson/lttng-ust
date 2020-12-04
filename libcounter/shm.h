#ifndef _LIBCOUNTER_SHM_H
#define _LIBCOUNTER_SHM_H

/*
 * libcounter/shm.h
 *
 * Copyright (C) 2011-2012 Mathieu Desnoyers <mathieu.desnoyers@efficios.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; only
 * version 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include <usterr-signal-safe.h>
#include <lttng/urcu/compiler.h>
#include "shm_types.h"

/* lttng_counter_handle_create - for UST. */
extern
struct lttng_counter_shm_handle *lttng_counter_handle_create(void *data,
				uint64_t memory_map_size, int wakeup_fd);
/* lttng_counter_handle_add_cpu - for UST. */
extern
int lttng_counter_handle_add_cpu(struct lttng_counter_shm_handle *handle,
		int shm_fd, uint32_t cpu_nr,
		uint64_t memory_map_size);
unsigned int lttng_counter_handle_get_nr_cpus(struct lttng_counter_shm_handle *handle);

/*
 * Pointer dereferencing. We don't trust the shm_ref, so we validate
 * both the index and offset with known boundaries.
 *
 * "shmp" and "shmp_index" guarantee that it's safe to use the pointer
 * target type, even in the occurrence of shm_ref modification by an
 * untrusted process having write access to the shm_ref. We return a
 * NULL pointer if the ranges are invalid.
 */
static inline
char *_lttng_counter_shmp_offset(struct lttng_counter_shm_object_table *table,
		struct lttng_counter_shm_ref *ref,
		size_t idx, size_t elem_size)
{
	struct lttng_counter_shm_object *obj;
	size_t objindex, ref_offset;

	objindex = (size_t) ref->index;
	if (lttng_ust_unlikely(objindex >= table->allocated_len))
		return NULL;
	obj = &table->objects[objindex];
	ref_offset = (size_t) ref->offset;
	ref_offset += idx * elem_size;
	/* Check if part of the element returned would exceed the limits. */
	if (lttng_ust_unlikely(ref_offset + elem_size > obj->memory_map_size))
		return NULL;
	return &obj->memory_map[ref_offset];
}

#define lttng_counter_shmp_index(handle, ref, index)			\
	({								\
		__typeof__((ref)._type) ____ptr_ret;			\
		____ptr_ret = (__typeof__(____ptr_ret)) _lttng_counter_shmp_offset((handle)->table, &(ref)._ref, index, sizeof(*____ptr_ret));	\
		____ptr_ret;						\
	})

#define lttng_counter_shmp(handle, ref)	lttng_counter_shmp_index(handle, ref, 0)

static inline
void _lttng_counter_set_shmp(struct lttng_counter_shm_ref *ref, struct lttng_counter_shm_ref src)
{
	*ref = src;
}

#define lttng_counter_set_shmp(ref, src)	_lttng_counter_set_shmp(&(ref)._ref, src)

struct lttng_counter_shm_object_table *lttng_counter_shm_object_table_create(size_t max_nb_obj);
struct lttng_counter_shm_object *lttng_counter_shm_object_table_alloc(struct lttng_counter_shm_object_table *table,
			size_t memory_map_size,
			enum lttng_counter_shm_object_type type,
			const int cpu_fd,
			int cpu);
struct lttng_counter_shm_object *lttng_counter_shm_object_table_append_shm(struct lttng_counter_shm_object_table *table,
			int shm_fd, size_t memory_map_size);
/* mem ownership is passed to lttng_counter_shm_object_table_append_mem(). */
struct lttng_counter_shm_object *lttng_counter_shm_object_table_append_mem(struct lttng_counter_shm_object_table *table,
			void *mem, size_t memory_map_size);
void lttng_counter_shm_object_table_destroy(struct lttng_counter_shm_object_table *table, int consumer);

/*
 * lttng_counter_zalloc_shm - allocate memory within a shm object.
 *
 * Shared memory is already zeroed by shmget.
 * *NOT* multithread-safe (should be protected by mutex).
 * Returns a -1, -1 tuple on error.
 */
struct lttng_counter_shm_ref lttng_counter_zalloc_shm(struct lttng_counter_shm_object *obj, size_t len);
void lttng_counter_align_shm(struct lttng_counter_shm_object *obj, size_t align);

static inline
int lttng_counter_shm_get_shm_fd(struct lttng_counter_shm_handle *handle, struct lttng_counter_shm_ref *ref)
{
	struct lttng_counter_shm_object_table *table = handle->table;
	struct lttng_counter_shm_object *obj;
	size_t index;

	index = (size_t) ref->index;
	if (lttng_ust_unlikely(index >= table->allocated_len))
		return -EPERM;
	obj = &table->objects[index];
	return obj->shm_fd;
}


static inline
int lttng_counter_shm_get_shm_size(struct lttng_counter_shm_handle *handle, struct lttng_counter_shm_ref *ref,
		uint64_t *size)
{
	struct lttng_counter_shm_object_table *table = handle->table;
	struct lttng_counter_shm_object *obj;
	size_t index;

	index = (size_t) ref->index;
	if (lttng_ust_unlikely(index >= table->allocated_len))
		return -EPERM;
	obj = &table->objects[index];
	*size = obj->memory_map_size;
	return 0;
}

#endif /* _LIBCOUNTER_SHM_H */
