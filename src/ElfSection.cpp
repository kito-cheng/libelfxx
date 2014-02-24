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

#include <ElfSection.h>
#include <Debug.h>

#include <stdio.h>
#include <inttypes.h>

#include <algorithm>

namespace libelfxx {

template <class Elf_Shdr>
static uint8_t *initContent(Elf_Shdr *shdr, uint8_t *rawData) {
  uint8_t *content = new uint8_t[shdr->sh_size];
  uint8_t *begin = rawData + shdr->sh_offset;
  uint8_t *end = begin + shdr->sh_size;
  if (!(shdr->sh_type & SHT_NOBITS)) {
    std::copy(begin, end, content);
  } else {
    std::fill(content, content+shdr->sh_size, 0);
  }
  return content;
}

ElfSection::ElfSection(const char *name, Elf32_Shdr *shdr, uint8_t *rawData)
  : _nameStr(name)
  , _name(shdr->sh_name)
  , _type(shdr->sh_type)
  , _flags(shdr->sh_flags)
  , _addr(shdr->sh_addr)
  , _offset(shdr->sh_offset)
  , _size(shdr->sh_size)
  , _link(shdr->sh_link)
  , _info(shdr->sh_info)
  , _addralign(shdr->sh_addralign)
  , _entsize(shdr->sh_entsize)
  , _content(initContent(shdr, rawData))
{
}

ElfSection::ElfSection(const char *name, Elf64_Shdr *shdr, uint8_t *rawData)
  : _nameStr(name)
  , _name(shdr->sh_name)
  , _type(shdr->sh_type)
  , _flags(shdr->sh_flags)
  , _addr(shdr->sh_addr)
  , _offset(shdr->sh_offset)
  , _size(shdr->sh_size)
  , _link(shdr->sh_link)
  , _info(shdr->sh_info)
  , _addralign(shdr->sh_addralign)
  , _entsize(shdr->sh_entsize)
  , _content(initContent(shdr, rawData))
{
}

ElfSection::~ElfSection() {
}

void ElfSection::print(FILE *fp) const {
  fprintf(fp, "Section `%s`:\n"
              "  sh_name : %" PRIu32 "\n"
              "  sh_type : 0x%" PRIx32 "\n"
              "  sh_flags : 0x%" PRIx64 "\n"
              "  sh_addr : 0x%" PRIx64 "\n"
              "  sh_offset : 0x%" PRIx64 "\n"
              "  sh_size : 0x%" PRIu64 "\n"
              "  sh_link : %" PRIu32 "\n"
              "  sh_info : 0x%" PRIx32 "\n"
              "  sh_addralign : %" PRIu64 "\n"
              "  sh_entsize : %" PRIu64 "\n",
              _nameStr,
              _name,
              _type,
              _flags,
              _addr,
              _offset,
              _size,
              _link,
              _info,
              _addralign,
              _entsize);
}

const uint8_t *ElfSection::content() const {
  return _content;
}

const char *ElfSection::nameStr() const {
  return _nameStr;
}

uint32_t ElfSection::name() const {
  return _name;
}

uint32_t ElfSection::type() const {
  return _type;
}

uint64_t ElfSection::flags() const {
  return _flags;
}

uint64_t ElfSection::addr() const {
  return _addr;
}

uint64_t ElfSection::offset() const {
  return _offset;
}

uint64_t ElfSection::size() const {
  return _size;
}

uint32_t ElfSection::link() const {
  return _link;
}

uint32_t ElfSection::info() const {
  return _info;
}

uint64_t ElfSection::addralign() const {
  return _addralign;
}

uint64_t ElfSection::entsize() const {
  return _entsize;
}

bool ElfSection::RELA() const {
  return _type == SHT_RELA;
}

bool ElfSection::PROGBITS() const {
  return _type == SHT_PROGBITS;
}

bool ElfSection::SYMTAB() const {
  return _type == SHT_SYMTAB;
}

bool ElfSection::STRTAB() const {
  return _type == SHT_STRTAB;
}

bool ElfSection::HASH() const {
  return _type == SHT_HASH;
}

bool ElfSection::DYNAMIC() const {
  return _type == SHT_DYNAMIC;
}

bool ElfSection::NOBITS() const {
  return _type == SHT_NOBITS;
}

bool ElfSection::REL() const {
  return _type == SHT_REL;
}

bool ElfSection::DYNSYM() const {
  return _type == SHT_DYNSYM;
}

bool ElfSection::GROUP() const {
  return _type == SHT_GROUP;
}

};
