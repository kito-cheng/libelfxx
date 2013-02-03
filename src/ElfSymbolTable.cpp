#include <ElfSymbolTable.h>
#include <ElfSection.h>

namespace libelfxx {


template<class Elf_Sym,
         class Elf_Word>
static
ElfSymbolTable::SymbolTable initSymTab(ElfSection *symtab,
                                       ElfSection *strtab,
                                       uint8_t *rawData) {
  Elf_Sym *syms = (Elf32_Sym*)(rawData + symtab->getOffset());
  Elf_Word numSym = (symtab->getSize() / symtab->getEntsize());
  ElfSymbolTable::SymbolTable symbolTable;

  for (Elf_Word i=0;i<numSym;++i) {
    Elf_Sym *sym = syms+i;
    const char *symName = (const char *)(rawData +
                                         strtab->getOffset() +
                                         sym->st_name);
    ElfSymbol *symbol = new ElfSymbol(symName, &syms[i]);
    symbolTable.insert(std::make_pair(symName, symbol));

  }
  return symbolTable;
}

ElfSymbolTable::ElfSymbolTable(ElfSection *symtab,
                               ElfSection *strtab,
                               uint8_t *rawData,
                               bool isElf32) {
  if (isElf32) {
    _symtab = initSymTab<Elf32_Sym,
                         Elf32_Word>(symtab, strtab, rawData);
  } else {
    // TODO: ELF64
  }
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
