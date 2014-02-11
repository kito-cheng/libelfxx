ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
lib_LTLIBRARIES = libelf++.la
libelf___la_SOURCES = \
  ElfImage.cpp \
  ElfSection.cpp \
  ElfSymbol.cpp \
  ElfProgramHeader.cpp \
  ElfSegment.cpp \
  ElfSymbolTable.cpp \
  Debug.cpp

AM_CPPFLAGS = -I$(top_srcdir)/include \
              -I$(top_srcdir)/include/libelf++ \
              -D__STDC_FORMAT_MACROS

AM_CXXFLAGS = -Wall -Wextra -std=c++11 -Werror
