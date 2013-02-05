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

uint32_t ElfSegment::getType() const {
  return _type;
}
uint64_t ElfSegment::getOffset() const {
  return _offset;
}
uint64_t ElfSegment::getVaddr() const {
  return _vaddr;
}
uint64_t ElfSegment::getPaddr() const {
  return _paddr;
}
uint32_t ElfSegment::getFilesz() const {
  return _filesz;
}
uint32_t ElfSegment::getMemsz() const {
  return _memsz;
}
uint32_t ElfSegment::getFlags() const {
  return _flags;
}
uint32_t ElfSegment::getAlign() const {
  return _align;
}
};
