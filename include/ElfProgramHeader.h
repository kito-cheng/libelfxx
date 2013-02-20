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

#ifndef _LIBELFXX_ELF_PROGRAM_HEADER_H_
#define _LIBELFXX_ELF_PROGRAM_HEADER_H_

#include <vector>

#include <stdint.h>
#include <stddef.h>
#include <elf.h>

namespace libelfxx {

class ElfSegment;

class ElfProgramHeader {
  public:
    typedef std::vector<ElfSegment *> Segments;
    typedef Segments::iterator iterator;
    typedef Segments::const_iterator const_iterator;
    ElfProgramHeader(Elf32_Ehdr *ehdr, uint8_t *rawData);
    ElfProgramHeader(Elf64_Ehdr *ehdr, uint8_t *rawData);

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    size_t getSegmentNum() const;
    ElfSegment *getSegment(unsigned i);

  private:
    std::vector<ElfSegment *> _segments;
};

};
#endif
