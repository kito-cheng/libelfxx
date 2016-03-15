/*
 * Copyright (C) 2012-2014 Kito Cheng (kito@0xlab.org)
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

#ifndef _LIBELFXX_ELF_RELOCATION_TABLE_H_
#define _LIBELFXX_ELF_RELOCATION_TABLE_H_

#include <vector>
#include <libelf++/ElfSection.h>
#include <libelf++/ElfRelocation.h>
#include <string>

namespace libelfxx {

class ElfRelocationTable : public ElfSection {
  public:
    typedef std::vector<ElfRelocation> Relocations;
    typedef Relocations::iterator iterator;
    typedef Relocations::const_iterator const_iterator;

    ElfRelocationTable(Elf32_Shdr *shdrs,
                       int shnum,
                       const char *shstrtab,
                       const std::string &name,
                       Elf32_Shdr *shdr,
                       Elf32_Shdr *symtab,
                       Elf32_Shdr *strtab,
                       uint8_t *rawData);

    ElfRelocationTable(Elf64_Shdr *shdrs,
                       int shnum,
                       const char *shstrtab,
                       const std::string &name,
                       Elf64_Shdr *shdr,
                       Elf64_Shdr *symtab,
                       Elf64_Shdr *strtab,
                       uint8_t *rawData);

    virtual ~ElfRelocationTable();

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    size_t size() const;

  private:
    Relocations _relocations;
};

};

#endif
