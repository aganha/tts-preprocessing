#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])

AC_INIT([SeparaOraciones], [0.1], [esaudino@yahoo.com])
AC_CONFIG_SRCDIR([main.cpp])

AC_CONFIG_AUX_DIR([.])
AC_REQUIRE_AUX_FILE ([splitter.dat])
AC_REQUIRE_AUX_FILE ([tokenizar.dat])

AM_INIT_AUTOMAKE([SeparaOraciones], [0.1])


# Checks for programs.
AC_PROG_CXX

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lfreeling':
#AC_CHECK_LIB([freeling], [main])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL

# Checks for library functions.
AC_CHECK_FUNCS([strcasecmp])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT


