#include <ElfSymbolTable.h>
#include <ElfSection.h>
#include <ElfImage.h>
#include <ElfSymbol.h>
#include <Debug.h>

namespace libelfxx {


template<class Elf_Shdr,
         class Elf_Sym,
         class Elf_Word>
static
ElfSymbolTable::SymbolTable initSymTab(Elf_Shdr *shdr,
                                       Elf_Shdr *strtab,
                                       uint8_t *rawData) {
  Elf_Sym *syms = reinterpret_cast<Elf_Sym*>(rawData + shdr->sh_offset);
  Elf_Word numSym = (shdr->sh_size / shdr->sh_entsize);
  ElfSymbolTable::SymbolTable symbolTable;

  for (Elf_Word i=0;i<numSym;++i) {
    Elf_Sym *sym = syms+i;
    const char *symName = (const char *)(rawData +
                                         strtab->sh_offset +
                                         sym->st_name);
    ElfSymbol *symbol = new ElfSymbol(symName, &syms[i]);
    symbolTable.insert(std::make_pair(symName, symbol));

  }
  return symbolTable;
}

ElfSymbolTable::ElfSymbolTable(const std::string &name,
                               Elf32_Shdr *shdr,
                               Elf32_Shdr *strtab,
                               uint8_t *rawData)
  : ElfSection(name, shdr, rawData)
{
  _symtab = initSymTab<Elf32_Shdr,
                       Elf32_Sym,
                       Elf32_Word>(shdr, strtab, rawData);
}

ElfSymbolTable::ElfSymbolTable(const std::string &name,
                               Elf64_Shdr *shdr,
                               Elf64_Shdr *strtab,
                               uint8_t *rawData)
  : ElfSection(name, shdr, rawData)
{
  _symtab = initSymTab<Elf64_Shdr,
                       Elf64_Sym,
                       Elf64_Word>(shdr, strtab, rawData);
}

ElfSymbolTable::~ElfSymbolTable() {
}

void ElfSymbolTable::print(FILE *fp) const {
  fprintf(fp, "SymbolTable\n");
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
        address >= sym->value() &&
        address < sym->value() + sym->size()) {
      return sym;
    }
  }
  if (findNearIfPossible) {
    uint64_t nearestDistance = (uint64_t)-1;
    ElfSymbol *nearestSym = NULL;
    for (auto &sym_:symTab) {
      auto &sym = sym_.second;
      if ((sym->isObject() || sym->isFunction()) &&
          sym->size() == 0 &&
          address >= sym->value() &&
          (address - sym->value()) <= nearestDistance) {
        nearestDistance = address - sym->value();
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
