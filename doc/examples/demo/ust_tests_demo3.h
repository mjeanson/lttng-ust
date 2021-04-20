/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (C) 2011 Mathieu Desnoyers <mathieu.desnoyers@efficios.com>
 */

#undef TRACEPOINT_PROVIDER
#define TRACEPOINT_PROVIDER ust_tests_demo3

#if !defined(_TRACEPOINT_UST_TESTS_DEMO3_H) || defined(LTTNG_UST_TRACEPOINT_HEADER_MULTI_READ)
#define _TRACEPOINT_UST_TESTS_DEMO3_H

#include <lttng/tracepoint.h>

LTTNG_UST_TRACEPOINT_EVENT(ust_tests_demo3, done,
	LTTNG_UST_TP_ARGS(int, value),
	LTTNG_UST_TP_FIELDS(
		ctf_integer(int, value, value)
	)
)
LTTNG_UST_TRACEPOINT_LOGLEVEL(ust_tests_demo3, done, LTTNG_UST_TRACEPOINT_LOGLEVEL_WARNING)

#endif /* _TRACEPOINT_UST_TESTS_DEMO3_H */

#undef TRACEPOINT_INCLUDE
#define TRACEPOINT_INCLUDE "./ust_tests_demo3.h"

/* This part must be outside ifdef protection */
#include <lttng/tracepoint-event.h>
