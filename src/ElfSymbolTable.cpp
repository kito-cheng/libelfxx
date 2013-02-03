#include <ElfSymbolTable.h>

namespace libelfxx {


ElfSymbolTable::ElfSymbolTable(Elf32_Ehdr *symtab, Elf32_Ehdr *strtab) {
}

ElfSymbolTable::iterator ElfSymbolTable::begin() {
  return _symtab.begin();
}

ElfSymbolTable::iterator ElfSymbolTable::end() {
  return _symtab.end();
}

ElfSymbolTable::const_iterator ElfSymbolTable::begin() const {
  return _symtab.begin();
}

ElfSymbolTable::const_iterator ElfSymbolTable::end() const {
  return _symtab.end();
}

};
