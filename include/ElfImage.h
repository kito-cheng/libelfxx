#ifndef _LIBELFXX_ELF_IMAGE_H_
#define _LIBELFXX_ELF_IMAGE_H_

#include <vector>
#include <map>
#include <string>

#include <stdio.h>
#include <stdint.h>
#include <elf.h>

namespace libelfxx {

class ElfSection;

class ElfImage {
  public:
    typedef std::vector<ElfSection*> Sections;
    typedef std::map<std::string, ElfSection*> SectionMap;
    typedef Sections::iterator iterator;
    typedef Sections::const_iterator const_iterator;

    static ElfImage *create(const char *path);
    static ElfImage *create(FILE *fp);
    bool isElf32() const;
    bool isElf64() const;


    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
  private:
    ElfImage(Elf64_Ehdr *ehdr);
    ElfImage(Elf32_Ehdr *ehdr);
    ~ElfImage();
    uint16_t _type;
    uint16_t _machine;
    uint32_t _version;
    uint64_t _entry;
    uint64_t _phoff;
    uint64_t _shoff;
    uint32_t _flags;
    uint16_t _ehsize;
    uint16_t _phentsize;
    uint16_t _phnum;
    uint16_t _shentsize;
    uint16_t _shnum;
    uint16_t _shstrndx;
    Sections _sections;
    SectionMap _sectionMap;
    uint8_t _rawData;
};

};

#endif /* _ELF_IMAGE_H_ */
