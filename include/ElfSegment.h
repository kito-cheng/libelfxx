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
