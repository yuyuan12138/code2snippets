# Program binary
bin_PROGRAMS = code2snippet

# Source files
code2snippet_SOURCES = \
    main.cpp \
    config.cpp

# Header files (included in distribution)
noinst_HEADERS = \
    config.h

# Compiler flags (add -std=c++17 for C++17 support)
code2snippet_CXXFLAGS = -Wall -Wextra -std=c++17

# Linker flags
code2snippet_LDADD = $(FILESYSTEM_LIB)
code2snippet_LDFLAGS =
