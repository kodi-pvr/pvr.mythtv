
bin_PROGRAMS = cppmythtest

cppmythtest_SOURCES = src/test.cpp

AM_CPPFLAGS = -I@abs_top_srcdir@/cppmyth/src

cppmythtest_LDFLAGS = -static

cppmythtest_LDADD   = \
	@abs_top_builddir@/cppmyth/libcppmyth.la \
	@abs_top_builddir@/jansson/src/libjansson.la
