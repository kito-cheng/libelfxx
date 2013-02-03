#ifndef _LIBELFXX_ELF_SYMBOL_TABLE_H_
#define _LIBELFXX_ELF_SYMBOL_TABLE_H_

#include <ElfSymbol.h>
#include <elf.h>
#include <map>

namespace libelfxx {

class ElfSymbolTable {
  public:
    typedef std::map<const char *, ElfSymbol> SymbolTable;
    typedef SymbolTable::iterator iterator;
    typedef SymbolTable::const_iterator const_iterator;

    ElfSymbolTable(Elf32_Ehdr *symtab, Elf32_Ehdr *strtab);
    ElfSymbolTable(Elf64_Ehdr *symtab, Elf64_Ehdr *strtab);

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
  private:
    SymbolTable _symtab;
};

};

#endif
