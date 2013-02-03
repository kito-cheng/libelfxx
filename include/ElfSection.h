#ifndef _LIBELFXX_ELF_SECTION_H_
#define _LIBELFXX_ELF_SECTION_H_

#include <elf.h>
#include <stdint.h>
#include <stdio.h>

namespace libelfxx {

class ElfSection {
  public:
    ElfSection(const char *name, Elf32_Shdr *shdr);
    ElfSection(const char *name, Elf64_Shdr *shdr);

    const char *getNameStr() const;
    uint32_t getName() const;
    uint64_t getFlags() const;
    uint64_t getAddr() const;
    uint64_t getOffset() const;
    uint64_t getSize() const;
    uint32_t getLink() const;
    uint32_t getInfo() const;
    uint64_t getAddralign() const;
    uint64_t getEntsize() const;

    void print(FILE *fp);
  private:
    const char *_nameStr;
    uint32_t _name;
    uint64_t _flags;
    uint64_t _addr;
    uint64_t _offset;
    uint64_t _size;
    uint32_t _link;
    uint32_t _info;
    uint64_t _addralign;
    uint64_t _entsize;
};

};
#endif
