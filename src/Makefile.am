ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
lib_LTLIBRARIES = libelf++.la
libelf___la_SOURCES = \
  ElfImage.cpp \
  ElfSection.cpp \
  ElfSymbol.cpp \
  ElfProgramHeader.cpp \
  ElfSegment.cpp \
  ElfSymbolTable.cpp \
  ElfDynamicInfo.cpp \
  ElfRelocationTable.cpp \
  ElfRelocation.cpp \
  ElfNote.cpp \
  ElfGNULibList.cpp \
  ElfGNULib.cpp \
  Debug.cpp

noinst_PROGRAMS = elftool

elftool_SOURCES = elftool.cpp
elftool_LDADD = libelf++.la

AM_CPPFLAGS = -I$(top_srcdir)/include \
              -I$(top_srcdir)/include/libelf++ \
              -D__STDC_FORMAT_MACROS

AM_CXXFLAGS = -Wall -Wextra -std=c++11
