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
