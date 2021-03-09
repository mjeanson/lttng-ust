/*
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (C) 2011 Mathieu Desnoyers <mathieu.desnoyers@efficios.com>
 */

#ifndef _LTTNG_TRACEPOINT_INTERNAL_H
#define _LTTNG_TRACEPOINT_INTERNAL_H

#include <urcu/list.h>
#include <lttng/tracepoint-types.h>
#include <lttng/ust-events.h>

#include "ust-helper.h"

#define TRACE_DEFAULT	TRACE_DEBUG_LINE

struct tracepoint_lib {
	struct cds_list_head list;	/* list of registered libs */
	struct lttng_ust_tracepoint * const *tracepoints_start;
	int tracepoints_count;
	struct cds_list_head callsites;
};

/* FIXME: No callers, drop them?
LTTNG_HIDDEN
extern int tracepoint_probe_register_noupdate(const char *name,
		void (*callback)(void), void *priv,
		const char *signature);

LTTNG_HIDDEN
extern int tracepoint_probe_unregister_noupdate(const char *name,
		void (*callback)(void), void *priv);
LTTNG_HIDDEN
extern void tracepoint_probe_update_all(void);
*/

// FIXME: ABI between liblttng-ust-tracepoint and liblttng-ust
// Should probably be namespaced under 'lttng_ust_tp_'
int __tracepoint_probe_register_queue_release(const char *name,
		void (*func)(void), void *data, const char *signature);
int __tracepoint_probe_unregister_queue_release(const char *name,
		void (*func)(void), void *data);
void __tracepoint_probe_prune_release_queue(void);

void lttng_ust_tp_init(void);
void lttng_ust_tp_exit(void);

/* Test compiler support for weak symbols with hidden visibility. */
LTTNG_HIDDEN
void *lttng_ust_tp_check_weak_hidden1(void);
LTTNG_HIDDEN
void *lttng_ust_tp_check_weak_hidden2(void);
LTTNG_HIDDEN
void *lttng_ust_tp_check_weak_hidden3(void);

#endif /* _LTTNG_TRACEPOINT_INTERNAL_H */
