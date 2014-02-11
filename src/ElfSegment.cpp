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

#include <ElfSegment.h>

namespace libelfxx {


ElfSegment::ElfSegment(Elf32_Phdr *phdr)
  :_type(phdr->p_type)
  ,_offset(phdr->p_offset)
  ,_vaddr(phdr->p_vaddr)
  ,_paddr(phdr->p_paddr)
  ,_filesz(phdr->p_filesz)
  ,_memsz(phdr->p_memsz)
  ,_flags(phdr->p_flags)
  ,_align(phdr->p_align)
{

}

ElfSegment::ElfSegment(Elf64_Phdr *phdr)
  :_type(phdr->p_type)
  ,_offset(phdr->p_offset)
  ,_vaddr(phdr->p_vaddr)
  ,_paddr(phdr->p_paddr)
  ,_filesz(phdr->p_filesz)
  ,_memsz(phdr->p_memsz)
  ,_flags(phdr->p_flags)
  ,_align(phdr->p_align)
{
}

uint32_t ElfSegment::type() const {
  return _type;
}
uint64_t ElfSegment::offset() const {
  return _offset;
}
uint64_t ElfSegment::vaddr() const {
  return _vaddr;
}
uint64_t ElfSegment::paddr() const {
  return _paddr;
}
uint32_t ElfSegment::filesz() const {
  return _filesz;
}
uint32_t ElfSegment::memsz() const {
  return _memsz;
}
uint32_t ElfSegment::flags() const {
  return _flags;
}
uint32_t ElfSegment::align() const {
  return _align;
}
};
