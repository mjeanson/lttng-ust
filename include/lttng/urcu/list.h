/*
 * Copyright (C) 2002 Free Software Foundation, Inc.
 * (originally part of the GNU C Library)
 * Contributed by Ulrich Drepper <drepper@redhat.com>, 2002.
 *
 * Copyright (C) 2009 Pierre-Marc Fournier
 * Conversion to RCU list.
 * Copyright (C) 2010 Mathieu Desnoyers <mathieu.desnoyers@efficios.com>
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
 */

#ifndef _CDS_LIST_H
#define _CDS_LIST_H	1

/*
 * The definitions of this file are adopted from those which can be
 * found in the Linux kernel headers to enable people familiar with the
 * latter find their way in these sources as well.
 */

/* Basic type for the double-link list. */
struct lttng_ust_list_head {
	struct lttng_ust_list_head *next, *prev;
};

/* Define a variable with the head and tail of the list. */
#define LTTNG_UST_LIST_HEAD(name) \
	struct lttng_ust_list_head name = { &(name), &(name) }

/* Initialize a new list head. */
#define LTTNG_UST_INIT_LIST_HEAD(ptr) \
	(ptr)->next = (ptr)->prev = (ptr)

#define LTTNG_UST_LIST_HEAD_INIT(name) { .prev = &(name), .next = &(name) }

/* Add new element at the head of the list. */
static inline
void lttng_ust_list_add(struct lttng_ust_list_head *newp, struct lttng_ust_list_head *head)
{
	head->next->prev = newp;
	newp->next = head->next;
	newp->prev = head;
	head->next = newp;
}

/* Add new element at the tail of the list. */
static inline
void lttng_ust_list_add_tail(struct lttng_ust_list_head *newp, struct lttng_ust_list_head *head)
{
	head->prev->next = newp;
	newp->next = head;
	newp->prev = head->prev;
	head->prev = newp;
}

/* Remove element from list. */
static inline
void __lttng_ust_list_del(struct lttng_ust_list_head *prev, struct lttng_ust_list_head *next)
{
	next->prev = prev;
	prev->next = next;
}

/* Remove element from list. */
static inline
void lttng_ust_list_del(struct lttng_ust_list_head *elem)
{
	__lttng_ust_list_del(elem->prev, elem->next);
}

/* Remove element from list, initializing the element's list pointers. */
static inline
void lttng_ust_list_del_init(struct lttng_ust_list_head *elem)
{
	lttng_ust_list_del(elem);
	LTTNG_UST_INIT_LIST_HEAD(elem);
}

/* Delete from list, add to another list as head. */
static inline
void lttng_ust_list_move(struct lttng_ust_list_head *elem, struct lttng_ust_list_head *head)
{
	__lttng_ust_list_del(elem->prev, elem->next);
	lttng_ust_list_add(elem, head);
}

/* Replace an old entry. */
static inline
void lttng_ust_list_replace(struct lttng_ust_list_head *old, struct lttng_ust_list_head *_new)
{
	_new->next = old->next;
	_new->prev = old->prev;
	_new->prev->next = _new;
	_new->next->prev = _new;
}

/* Join two lists. */
static inline
void lttng_ust_list_splice(struct lttng_ust_list_head *add, struct lttng_ust_list_head *head)
{
	/* Do nothing if the list which gets added is empty. */
	if (add != add->next) {
		add->next->prev = head;
		add->prev->next = head->next;
		head->next->prev = add->prev;
		head->next = add->next;
	}
}

/* Get typed element from list at a given position. */
#define lttng_ust_list_entry(ptr, type, member) \
	((type *) ((char *) (ptr) - (unsigned long) (&((type *) 0)->member)))


/* Get first entry from a list. */
#define lttng_ust_list_first_entry(ptr, type, member) \
	lttng_ust_list_entry((ptr)->next, type, member)

/* Iterate forward over the elements of the list. */
#define cds_list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

/*
 * Iterate forward over the elements list. The list elements can be
 * removed from the list while doing this.
 */
#define cds_list_for_each_safe(pos, p, head) \
	for (pos = (head)->next, p = pos->next; \
		pos != (head); \
		pos = p, p = pos->next)

/* Iterate backward over the elements of the list. */
#define cds_list_for_each_prev(pos, head) \
	for (pos = (head)->prev; pos != (head); pos = pos->prev)

/*
 * Iterate backwards over the elements list. The list elements can be
 * removed from the list while doing this.
 */
#define cds_list_for_each_prev_safe(pos, p, head) \
	for (pos = (head)->prev, p = pos->prev; \
		pos != (head); \
		pos = p, p = pos->prev)

#define cds_list_for_each_entry(pos, head, member) \
	for (pos = lttng_ust_list_entry((head)->next, __typeof__(*pos), member); \
		&pos->member != (head); \
		pos = lttng_ust_list_entry(pos->member.next, __typeof__(*pos), member))

#define cds_list_for_each_entry_reverse(pos, head, member) \
	for (pos = lttng_ust_list_entry((head)->prev, __typeof__(*pos), member); \
		&pos->member != (head); \
		pos = lttng_ust_list_entry(pos->member.prev, __typeof__(*pos), member))

#define cds_list_for_each_entry_safe(pos, p, head, member) \
	for (pos = lttng_ust_list_entry((head)->next, __typeof__(*pos), member), \
			p = lttng_ust_list_entry(pos->member.next, __typeof__(*pos), member); \
		&pos->member != (head); \
		pos = p, p = lttng_ust_list_entry(pos->member.next, __typeof__(*pos), member))

/*
 * Same as cds_list_for_each_entry_safe, but starts from "pos" which should
 * point to an entry within the list.
 */
#define cds_list_for_each_entry_safe_from(pos, p, head, member) \
        for (p = lttng_ust_list_entry(pos->member.next, __typeof__(*pos), member); \
                &pos->member != (head); \
                pos = p, p = lttng_ust_list_entry(pos->member.next, __typeof__(*pos), member))

static inline
int cds_list_empty(struct lttng_ust_list_head *head)
{
	return head == head->next;
}

static inline
void lttng_ust_list_replace_init(struct lttng_ust_list_head *old,
		struct lttng_ust_list_head *_new)
{
	struct lttng_ust_list_head *head = old->next;

	lttng_ust_list_del(old);
	lttng_ust_list_add_tail(_new, head);
	LTTNG_UST_INIT_LIST_HEAD(old);
}

#endif	/* _CDS_LIST_H */
