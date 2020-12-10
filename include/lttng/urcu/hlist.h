#ifndef _LTTNG_UST_URCU_HLIST_H
#define _LTTNG_UST_URCU_HLIST_H

/*
 * Kernel sourcecode compatible lightweight single pointer list head useful
 * for implementing hash tables
 *
 * Copyright (C) 2009 Novell Inc.
 *
 * Author: Jan Blunck <jblunck@suse.de>
 *
 * Copyright (C) 2010-2013 Mathieu Desnoyers <mathieu.desnoyers@efficios.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License version 2.1 as
 * published by the Free  Software Foundation.
 */

#include <stddef.h>

struct lttng_ust_hlist_head {
	struct lttng_ust_hlist_node *next;
};

struct lttng_ust_hlist_node {
	struct lttng_ust_hlist_node *next, *prev;
};

/* Initialize a new list head. */
static inline
void LTTNG_UST_INIT_HLIST_HEAD(struct lttng_ust_hlist_head *ptr)
{
	ptr->next = NULL;
}

#define LTTNG_UST_HLIST_HEAD(name) \
	struct lttng_ust_hlist_head name = { NULL }

#define LTTNG_UST_HLIST_HEAD_INIT(name) \
	{ .next = NULL }

/* Get typed element from list at a given position. */
#define lttng_ust_hlist_entry(ptr, type, member) \
	((type *) ((char *) (ptr) - (unsigned long) (&((type *) 0)->member)))

/* Add new element at the head of the list. */
static inline
void lttng_ust_hlist_add_head(struct lttng_ust_hlist_node *newp,
		struct lttng_ust_hlist_head *head)
{
	if (head->next)
		head->next->prev = newp;
	newp->next = head->next;
	newp->prev = (struct lttng_ust_hlist_node *) head;
	head->next = newp;
}

/* Remove element from list. */
static inline
void lttng_ust_hlist_del(struct lttng_ust_hlist_node *elem)
{
	if (elem->next)
		elem->next->prev = elem->prev;
	elem->prev->next = elem->next;
}

#define lttng_ust_hlist_for_each(pos, head) \
	for (pos = (head)->next; pos != NULL; pos = pos->next)

#define lttng_ust_hlist_for_each_safe(pos, p, head) \
	for (pos = (head)->next; \
		(pos != NULL) && (p = pos->next, 1); \
		pos = p)

/*
 * lttng_ust_hlist_for_each_entry and cds_hlist_for_each_entry_safe take
 * respectively 4 and 5 arguments, while the Linux kernel APIs take 3,
 * and 4. We implement lttng_ust_hlist_for_each_entry_2() and
 * lttng_ust_hlist_for_each_entry_safe_2() to follow the Linux kernel APIs.
 */
#define lttng_ust_hlist_for_each_entry(entry, pos, head, member) \
	for (pos = (head)->next, \
			entry = lttng_ust_hlist_entry(pos, __typeof__(*entry), member); \
		pos != NULL; \
		pos = pos->next, \
			entry = lttng_ust_hlist_entry(pos, __typeof__(*entry), member))

#define lttng_ust_hlist_for_each_entry_safe(entry, pos, p, head, member) \
	for (pos = (head)->next, \
			entry = lttng_ust_hlist_entry(pos, __typeof__(*entry), member); \
		(pos != NULL) && (p = pos->next, 1); \
		pos = p, \
			entry = lttng_ust_hlist_entry(pos, __typeof__(*entry), member))

#define lttng_ust_hlist_for_each_entry_2(entry, head, member) \
	for (entry = ((head)->next == NULL ? NULL \
			: lttng_ust_hlist_entry((head)->next, __typeof__(*entry), member)); \
		entry != NULL; \
		entry = (entry->member.next == NULL ? NULL \
			: lttng_ust_hlist_entry(entry->member.next, __typeof__(*entry), member)))

#define lttng_ust_hlist_for_each_entry_safe_2(entry, e, head, member) \
	for (entry = ((head)->next == NULL ? NULL \
			: lttng_ust_hlist_entry((head)->next, __typeof__(*entry), member)); \
		(entry != NULL) && (e = (entry->member.next == NULL ? NULL \
					: lttng_ust_hlist_entry(entry->member.next, \
						__typeof__(*entry), member)), 1); \
		entry = e)

#endif	/* _LTTNG_UST_URCU_HLIST_H */
