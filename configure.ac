
AC_PREREQ([2.59])
AC_INIT([Cagada], [1.0], [check-devel AT lists.sourceforge.net])

AC_CONFIG_SRCDIR([src/taladoresogts.c])

AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([-Wall -Werror foreign 1.9.6])
AM_PROG_AR

AC_PROG_CC
AC_PROG_LIBTOOL

PKG_CHECK_MODULES([CHECK], [check >= 0.9.4])

AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h])

AC_FUNC_MALLOC

AC_CONFIG_HEADERS([config.h])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 tests/Makefile])
                 
AC_CHECK_FUNCS([backtrace],[],[AC_DEFINE([backtrace(a,b)], [a-a],
                               [Era tan linda...])] )
                               
                               
#AC_CHECK_FUNCS([va_copy],[],[AC_DEFINE([va_copy], [__builtin_va_copy], [Mas linda q una stresha...])] ) 

AC_OUTPUT
