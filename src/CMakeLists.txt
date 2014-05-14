add_library(elf++ STATIC
  Debug.cpp
  ElfDynamicInfo.cpp
  ElfGNULib.cpp
  ElfGNULibList.cpp
  ElfImage.cpp
  ElfNote.cpp
  ElfProgramHeader.cpp
  ElfRelocation.cpp
  ElfRelocationTable.cpp
  ElfSection.cpp
  ElfSegment.cpp
  ElfSymbol.cpp
  ElfSymbolTable.cpp
)

add_executable(elftool elftool.cpp)
target_link_libraries(elftool elf++)
