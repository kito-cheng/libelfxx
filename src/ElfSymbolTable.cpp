#include <ElfSymbolTable.h>
#include <ElfSection.h>
#include <ElfImage.h>
#include <ElfSymbol.h>
#include <Debug.h>

namespace libelfxx {


template<class Elf_Sym,
         class Elf_Word>
static
ElfSymbolTable::SymbolTable initSymTab(ElfSection *symtab,
                                       ElfSection *strtab,
                                       uint8_t *rawData) {
  Elf_Sym *syms = reinterpret_cast<Elf_Sym*>(rawData + symtab->offset());
  Elf_Word numSym = (symtab->size() / symtab->entsize());
  ElfSymbolTable::SymbolTable symbolTable;

  for (Elf_Word i=0;i<numSym;++i) {
    Elf_Sym *sym = syms+i;
    const char *symName = (const char *)(rawData +
                                         strtab->offset() +
                                         sym->st_name);
    ElfSymbol *symbol = new ElfSymbol(symName, &syms[i]);
    symbolTable.insert(std::make_pair(symName, symbol));

  }
  return symbolTable;
}

ElfSymbolTable::ElfSymbolTable(ElfSection *symtab,
                               ElfSection *strtab,
                               uint8_t *rawData,
                               ElfImage::Type elfType) {
  switch (elfType) {
    case ElfImage::ELF32:
      _symtab = initSymTab<Elf32_Sym,
                           Elf32_Word>(symtab, strtab, rawData);
      break;
    case ElfImage::ELF64:
      _symtab = initSymTab<Elf64_Sym,
                           Elf64_Word>(symtab, strtab, rawData);
      break;
    default:
      error("OMG, what's the hell?");
      abort();
      break;
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

ElfSymbolTable::const_iterator ElfSymbolTable::cbegin() const {
  return _symtab.begin();
}

ElfSymbolTable::const_iterator ElfSymbolTable::cend() const {
  return _symtab.end();
}

ElfSymbolTable::iterator ElfSymbolTable::find(const std::string &name) {
  return _symtab.find(name);
}

ElfSymbolTable::const_iterator
ElfSymbolTable::find(const std::string &name) const {
  return _symtab.find(name);
}

template<typename SymTab, typename Sym>
static Sym *
findSymbol(SymTab &symTab, uint64_t address, bool findNearIfPossible) {
  for (auto &sym_:symTab) {
    auto &sym = sym_.second;
    if ((sym->isObject() || sym->isFunction()) &&
        address >= sym->getValue() &&
        address < sym->getValue() + sym->getSize()) {
      return sym;
    }
  }
  if (findNearIfPossible) {
    uint64_t nearestDistance = (uint64_t)-1;
    ElfSymbol *nearestSym = NULL;
    for (auto &sym_:symTab) {
      auto &sym = sym_.second;
      if ((sym->isObject() || sym->isFunction()) &&
          sym->getSize() == 0 &&
          address >= sym->getValue() &&
          (address - sym->getValue()) <= nearestDistance) {
        nearestDistance = address - sym->getValue();
        nearestSym = sym;
      }
    }
    return nearestSym;
  }
  return nullptr;
}

ElfSymbol *ElfSymbolTable::find(uint64_t address,
                                bool findNearIfPossible) {
  return findSymbol<ElfSymbolTable, ElfSymbol>(
           *this, address, findNearIfPossible);
}

const ElfSymbol *ElfSymbolTable::find(uint64_t address,
                                      bool findNearIfPossible) const {
  return findSymbol<const ElfSymbolTable, const ElfSymbol>(
           *this, address, findNearIfPossible);
}


};
