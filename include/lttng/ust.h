/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (C) 2011-2012 Mathieu Desnoyers <mathieu.desnoyers@efficios.com>
 */

#ifndef _LTTNG_UST_H
#define _LTTNG_UST_H

#include <signal.h>

#ifdef __cplusplus
extern "C" {
#endif

// FIXME: This header should be renamed

// This is ABI for liblttng-ust-fork


// Owned by liblttng-ust.so, no extern
void ust_before_fork(sigset_t *save_sigset);
void ust_after_fork_parent(sigset_t *restore_sigset);
void ust_after_fork_child(sigset_t *restore_sigset);
void ust_after_setns(void);
void ust_after_unshare(void);
void ust_after_setuid(void);
void ust_after_setgid(void);
void ust_after_seteuid(void);
void ust_after_setegid(void);
void ust_after_setreuid(void);
void ust_after_setregid(void);
void ust_after_setresuid(void);
void ust_after_setresgid(void);

#ifdef __cplusplus
}
#endif

#endif /* _LTTNG_UST_H */
