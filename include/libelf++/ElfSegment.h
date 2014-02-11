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

    uint32_t type() const;
    uint64_t offset() const;
    uint64_t vaddr() const;
    uint64_t virtualAddress() const {return vaddr();}
    uint64_t paddr() const;
    uint64_t physicalAddress() const {return paddr();}
    uint32_t filesz() const;
    uint64_t fileSize() const {return filesz();}
    uint32_t memsz() const;
    uint64_t memorySize() const {return memsz();}
    uint32_t flags() const;
    uint32_t align() const;
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
