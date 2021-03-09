/*
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (C) 2016  Mathieu Desnoyers <mathieu.desnoyers@efficios.com>
 */

#define _LGPL_SOURCE
#include <limits.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <ust-fd.h>
#include <dlfcn.h>
#include <errno.h>

#include <ust-helper.h>

static int (*__lttng_ust_fd_plibc_close)(int fd) = NULL;
static int (*__lttng_ust_fd_plibc_fclose)(FILE *stream) = NULL;

/*
 * Use dlsym to find the original libc close() symbol and store it in
 * __lttng_ust_fd_plibc_close.
 *
 * Void since we have no way to recover if dlsym fails.
 */
static
void _lttng_ust_fd_init_plibc_close(void)
{
	__lttng_ust_fd_plibc_close = dlsym(RTLD_NEXT, "close");
	if (__lttng_ust_fd_plibc_close == NULL) {
		fprintf(stderr, "%s\n", dlerror());
	}
}

/*
 * Use dlsym to find the original libc fclose() symbol and store it in
 * __lttng_ust_fd_plibc_fclose.
 *
 * Void since we have no way to recover if dlsym fails.
 */
static
void _lttng_ust_fd_init_plibc_fclose(void)
{
	__lttng_ust_fd_plibc_fclose = dlsym(RTLD_NEXT, "fclose");
	if (__lttng_ust_fd_plibc_fclose == NULL) {
		fprintf(stderr, "%s\n", dlerror());
	}
}

/*
 * Initialize the function pointers to the original libc symbols in the
 * constructor since close() has to stay async-signal-safe and so can't call
 * dlsym().
 */
static __attribute__((constructor))
void _lttng_ust_fd_init(void)
{
	_lttng_ust_fd_init_plibc_close();
	_lttng_ust_fd_init_plibc_fclose();
}

/*
 * Override the libc close symbol with our own allowing applications to close
 * arbitrary file descriptors. We check if it is owned by lttng-ust, and return
 * -1, errno=EBADF instead of closing it if it is the case.
 */
int close(int fd)
{
	/*
	 * We can't retry dlsym here since close is async-signal-safe.
	 */
	if (__lttng_ust_fd_plibc_close == NULL) {
		errno = ENOSYS;
		return -1;
	}

	return lttng_ust_safe_close_fd(fd, __lttng_ust_fd_plibc_close);
}

/*
 * Override the libc fclose symbol with our own allowing applications to close
 * arbitrary streams. We check if it is owned by lttng-ust, and return -1,
 * errno=EBADF instead of closing it if it is the case.
 *
 * Note: fcloseall() is not an issue because it fcloses only the
 * streams it knows about, which differs from the problems caused by
 * gnulib close_stdout(), which does an explicit fclose(stdout).
 */
int fclose(FILE *stream)
{
	/*
	 * FIXME: Do the same thing as close() or retry dlsym()?
	 */
	if (__lttng_ust_fd_plibc_fclose == NULL) {
		errno = EINVAL;
		return -1;
	}

	return lttng_ust_safe_fclose_stream(stream,
			__lttng_ust_fd_plibc_fclose);
}

#if defined(__sun__) || defined(__FreeBSD__)
/* Solaris and FreeBSD. */
void closefrom(int lowfd)
{
	/*
	 * We can't retry dlsym here since close is async-signal-safe.
	 */
	if (__lttng_ust_fd_plibc_close == NULL) {
		return;
	}

	(void) lttng_ust_safe_closefrom_fd(lowfd, __lttng_ust_fd_plibc_close);
}
#elif defined(__NetBSD__) || defined(__OpenBSD__)
/* NetBSD and OpenBSD. */
int closefrom(int lowfd)
{
	/*
	 * We can't retry dlsym here since close is async-signal-safe.
	 */
	if (__lttng_ust_fd_plibc_close == NULL) {
		errno = ENOSYS;
		return -1;
	}

	return lttng_ust_safe_closefrom_fd(lowfd, __lttng_ust_fd_plibc_close);
}
#else
/* As far as we know, this OS does not implement closefrom. */
#endif
