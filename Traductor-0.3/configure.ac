#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
#

AC_PREREQ([2.68])
AC_INIT([Traductor], [0.1], [esaudino@yahoo.com])
AC_CONFIG_SRCDIR([main.cpp])

AC_CONFIG_AUX_DIR([./])
AC_REQUIRE_AUX_FILE ([splitter.dat])
AC_REQUIRE_AUX_FILE ([tokenizar.dat])

AM_INIT_AUTOMAKE([Traductor], [0.1])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stddef.h stdlib.h Avl.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([strcasecmp])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT



