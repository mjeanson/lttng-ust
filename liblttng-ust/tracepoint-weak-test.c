/*
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (C) 2016 Mathieu Desnoyers <mathieu.desnoyers@efficios.com>
 */

#include "ust-helper.h"

/* Test compiler support for weak symbols with hidden visibility. */
LTTNG_HIDDEN
int __tracepoint_test_symbol1;

LTTNG_HIDDEN
void *__tracepoint_test_symbol2;

LTTNG_HIDDEN
struct {
	char a[24];
} __tracepoint_test_symbol3;

void *lttng_ust_tp_check_weak_hidden1(void)
{
	return &__tracepoint_test_symbol1;
}

void *lttng_ust_tp_check_weak_hidden2(void)
{
	return &__tracepoint_test_symbol2;
}

void *lttng_ust_tp_check_weak_hidden3(void)
{
	return &__tracepoint_test_symbol3;
}
