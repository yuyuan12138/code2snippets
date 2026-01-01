#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([code2snippet], [1.0.0], [https://github.com/yuyuan12138/code2snippets/issues])
AM_INIT_AUTOMAKE([1.14 foreign subdir-objects])
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_INSTALL
AM_PROG_AR

# Check for C++17 support manually
AC_MSG_CHECKING([for C++17 support])
AC_LANG_PUSH([C++])
ac_saved_cxxflags="$CXXFLAGS"
CXXFLAGS="$CXXFLAGS -std=c++17"
AC_COMPILE_IFELSE([AC_LANG_SOURCE([
#include <filesystem>
int main() {
    std::filesystem::path p;
    return 0;
}
])], [
    AC_MSG_RESULT([yes])
    CXXFLAGS="$ac_saved_cxxflags"
    AC_DEFINE([HAVE_CXX17], [1], [Define to 1 if C++17 is supported])
], [
    AC_MSG_ERROR([C++17 is required but not supported])
])
AC_LANG_POP([C++])

# Checks for libraries.
# Try to link with libstdc++fs, but don't fail if not found
# (newer GCC versions have std::filesystem in libstdc++)
AC_CHECK_LIB([stdc++fs], [main],
    [FILESYSTEM_LIB=-lstdc++fs],
    [FILESYSTEM_LIB=])

AC_SUBST(FILESYSTEM_LIB)

# Checks for library functions.

# Config files
AC_CONFIG_FILES([
    Makefile
    src/Makefile
])

AC_OUTPUT

# Print summary
echo "
  $PACKAGE_NAME $VERSION

  Configuration:
    Compiler:           ${CXX}
    CXXFLAGS:           ${CXXFLAGS}
    LDFLAGS:            ${LDFLAGS}
    Filesystem lib:     ${FILESYSTEM_LIB:-none}

  Installation paths:
    Prefix:             ${prefix}
    Executables:        ${bindir}
    Config files:       ${sysconfdir}/code2snippet
"
