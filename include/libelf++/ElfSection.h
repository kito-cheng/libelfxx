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

#ifndef _LIBELFXX_ELF_SECTION_H_
#define _LIBELFXX_ELF_SECTION_H_

#include <elf.h>
#include <stdint.h>
#include <stdio.h>

namespace libelfxx {

class ElfSection {
  public:
    ElfSection(const char *name, Elf32_Shdr *shdr, uint8_t *rawData);
    ElfSection(const char *name, Elf64_Shdr *shdr, uint8_t *rawData);
    virtual ~ElfSection();

    const char *nameStr() const;
    uint32_t name() const;
    uint32_t type() const;
    uint64_t flags() const;
    uint64_t addr() const;
    uint64_t offset() const;
    uint64_t size() const;
    uint32_t link() const;
    uint32_t info() const;
    uint64_t addralign() const;
    uint64_t entsize() const;

    bool RELA() const;
    bool REL() const;
    bool DYNAMIC() const;

    bool PROGBITS() const;
    bool SYMTAB() const;
    bool STRTAB() const;
    bool HASH() const;
    bool NOBITS() const;
    bool DYNSYM() const;
    bool GROUP() const;

    const uint8_t *content() const;

    virtual void print(FILE *fp) const;
  private:
    const char *_nameStr;
    uint32_t _name;
    uint32_t _type;
    uint64_t _flags;
    uint64_t _addr;
    uint64_t _offset;
    uint64_t _size;
    uint32_t _link;
    uint32_t _info;
    uint64_t _addralign;
    uint64_t _entsize;

    uint8_t *_content;
};

};
#endif
