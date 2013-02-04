#ifndef _LIBELFXX_ELF_SYMBOL_TABLE_H_
#define _LIBELFXX_ELF_SYMBOL_TABLE_H_

#include <ElfSymbol.h>
#include <ElfImage.h>
#include <elf.h>
#include <stdio.h>
#include <map>
#include <string>

namespace libelfxx {

class ElfSection;

class ElfSymbolTable {
  public:
    typedef std::map<std::string, ElfSymbol*> SymbolTable;
    typedef SymbolTable::iterator iterator;
    typedef SymbolTable::const_iterator const_iterator;

    ElfSymbolTable(ElfSection *symtab,
                   ElfSection *strtab,
                   uint8_t *rawData,
                   ElfImage::Type elfType);

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    void print(FILE *fp);
  private:
    SymbolTable _symtab;
};

};

#endif
