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

#ifndef _LIBELFXX_ELF_SEGMENT_H_
#define _LIBELFXX_ELF_SEGMENT_H_

#include <vector>

#include <stdint.h>
#include <stddef.h>
#include <elf.h>

namespace libelfxx {

class ElfSegment {
  public:
    ElfSegment(Elf32_Phdr *phdr);
    ElfSegment(Elf64_Phdr *phdr);

    uint32_t getType() const;
    uint64_t getOffset() const;
    uint64_t getVaddr() const;
    uint64_t getPaddr() const;
    uint32_t getFilesz() const;
    uint32_t getMemsz() const;
    uint32_t getFlags() const;
    uint32_t getAlign() const;
  private:
    uint32_t _type;
    uint64_t _offset;
    uint64_t _vaddr;
    uint64_t _paddr;
    uint32_t _filesz;
    uint32_t _memsz;
    uint32_t _flags;
    uint32_t _align;
};

};
#endif
