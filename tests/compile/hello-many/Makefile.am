# SPDX-License-Identifier: LGPL-2.1-only

AM_CPPFLAGS += -I$(srcdir)

noinst_PROGRAMS = hello-many
hello_many_SOURCES = hello-many.c tp.c ust_tests_hello_many.h
hello_many_CFLAGS = -Werror=old-style-definition $(AM_CFLAGS)
hello_many_LDADD = \
	$(top_builddir)/src/lib/lttng-ust/liblttng-ust.la \
	$(DL_LIBS)

EXTRA_DIST = README
