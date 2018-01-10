AC_PREREQ([2.68])
AC_INIT([Drawbook],[0.1.0],[drawbook.productions@gmail.com])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR([src/drawbook2.cpp])
AC_CONFIG_HEADERS([config.h])

if test -z $CXXFLAGS; then
	CXXFLAGS='-O2 -flto'
fi
AC_PROG_CXX

AX_CXX_COMPILE_STDCXX_11
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
