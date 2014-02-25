/*
 * Copyright (C) 2012-2013 Kito Cheng (kito@0xlab.org)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _LIBELFXX_ELF_SYMBOL_TABLE_H_
#define _LIBELFXX_ELF_SYMBOL_TABLE_H_

#include <libelf++/ElfImage.h>
#include <libelf++/ElfSection.h>
#include <elf.h>
#include <stdio.h>
#include <map>
#include <string>

namespace libelfxx {

class ElfSymbol;
class ElfSection;

class ElfSymbolTable : public ElfSection {
  public:
    typedef std::map<std::string, ElfSymbol*> SymbolTable;
    typedef SymbolTable::iterator iterator;
    typedef SymbolTable::const_iterator const_iterator;

    ElfSymbolTable(const std::string &name,
                   Elf32_Shdr *shdr,
                   Elf32_Shdr *strtab,
                   uint8_t *rawData);
    ElfSymbolTable(const std::string &name,
                   Elf64_Shdr *shdr,
                   Elf64_Shdr *strtab,
                   uint8_t *rawData);
    virtual ~ElfSymbolTable();

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    virtual void print(FILE *fp) const;

    iterator find(const std::string &name);
    const_iterator find(const std::string &name) const;

    ElfSymbol *find(uint64_t address,
                    bool findNearIfPossible = false);
    const ElfSymbol *find(uint64_t address,
                          bool findNearIfPossible = false) const;
  private:
    SymbolTable _symtab;
};

};

#endif
