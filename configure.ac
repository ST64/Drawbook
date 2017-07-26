AC_PREREQ([2.68])
AC_INIT([Drawbook],[0.1.0],[drawbook.productions@gmail.com])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR([src/drawbook2.cpp])
AC_CONFIG_HEADERS([config.h])

AC_PROG_CXX
AC_PROG_CC

AX_CHECK_COMPILE_FLAG([-std=c++11], [CXXFLAGS="$CXXFLAGS -std=c++11"])
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
