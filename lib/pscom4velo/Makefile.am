#
# ParaStation
#
# Copyright (C) 2012,2013 ParTec Cluster Competence Center GmbH, Munich
#
# This file may be distributed under the terms of the Q Public License
# as defined in the file LICENSE.QPL included in the packaging of this
# file.
#
# Authors:      Thomas Moschny <moschny@par-tec.com>
#               Jens Hauke <hauke@par-tec.com>
#
include $(top_srcdir)/common.am

AM_LDFLAGS += -avoid-version
AM_CPPFLAGS += -DLIBDIR=\"$(libdir)\" -I$(top_srcdir)/lib/pscom

lib_LTLIBRARIES = libpscom4velo.la

libpscom4velo_la_SOURCES = pscom_extoll.c pscom_extoll.h psextoll.c psextoll.h
libpscom4velo_la_CPPFLAGS = $(AM_CPPFLAGS) $(EXTOLL_CPPFLAGS)
libpscom4velo_la_LDFLAGS = $(AM_LDFLAGS) $(EXTOLL_LDFLAGS)
libpscom4velo_la_LIBADD = -lrma2 -lvelo2 $(top_builddir)/lib/pscom/libpscom.la
