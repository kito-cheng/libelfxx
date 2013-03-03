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

#include <ElfProgramHeader.h>
#include <ElfSegment.h>

namespace libelfxx {

template <class Elf_Ehdr,
          class Elf_Phdr>
static std::vector<ElfSegment*> getSections(Elf_Ehdr *ehdr, uint8_t *rawData) {
  std::vector<ElfSegment*> elfSegments;
  uint64_t phoff = ehdr->e_phoff;
  Elf_Phdr *phdrs = reinterpret_cast<Elf_Phdr*>(rawData + phoff);
  for (int i = 0;i < ehdr->e_phnum; ++i) {
    Elf_Phdr *phdr = &phdrs[i];
    ElfSegment *elfSegment = new ElfSegment(phdr);
    elfSegments.push_back(elfSegment);
  }
  return elfSegments;
}

ElfProgramHeader::ElfProgramHeader(Elf32_Ehdr *ehdr, uint8_t *rawData)
  : _segments(getSections<Elf32_Ehdr, Elf32_Phdr>(ehdr, rawData))
{
}

ElfProgramHeader::ElfProgramHeader(Elf64_Ehdr *ehdr, uint8_t *rawData)
  : _segments(getSections<Elf64_Ehdr, Elf64_Phdr>(ehdr, rawData))
{

}

ElfSegment *ElfProgramHeader::getSegment(unsigned i) {
  if (i < _segments.size()) {
    return _segments[i];
  } else {
    return nullptr;
  }
}

size_t ElfProgramHeader::getSegmentNum() const {
  return _segments.size();
}

ElfProgramHeader::iterator ElfProgramHeader::begin() {
  return _segments.begin();
}

ElfProgramHeader::iterator ElfProgramHeader::end() {
  return _segments.end();
}

ElfProgramHeader::const_iterator ElfProgramHeader::begin() const {
  return _segments.begin();
}

ElfProgramHeader::const_iterator ElfProgramHeader::end() const {
  return _segments.end();
}

ElfProgramHeader::const_iterator ElfProgramHeader::cbegin() const {
  return _segments.begin();
}

ElfProgramHeader::const_iterator ElfProgramHeader::cend() const {
  return _segments.end();
}

};
