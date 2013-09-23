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
class ElfSymbolTable;
class ElfProgramHeader;

class ElfImage {
  public:
    ~ElfImage();
    enum Type {
      ELFINVALID,
      ELF32,
      ELF64,
    };
    enum Endian {
      ELF_NONE_ENDIAN,
      ELF_LITTLE_ENDIAN,
      ELF_BIG_ENDIAN,
      ELF_UNKNOWN_ENDIAN,
    };
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
    const_iterator cbegin() const;
    const_iterator cend() const;

    const uint8_t *getIdent() const;
    uint16_t getType() const;
    const char *getMachineString() const;
    uint16_t getMachine() const;
    uint32_t getVersion() const;
    uint64_t getEntry() const;
    uint64_t getPhoff() const;
    uint64_t getShoff() const;
    uint32_t getFlags() const;
    uint16_t getEhsize() const;
    uint16_t getPhentsize() const;
    uint16_t getPhnum() const;
    uint16_t getShentsize() const;
    uint16_t getShnum() const;
    uint16_t getShstrndx() const;

    size_t getSegmentNum() const;
    size_t getSectionNum() const;
    ElfSection *getSection(unsigned idx);
    ElfSection *getSection(const char *name);
    ElfProgramHeader *getProgramHeader();
    Type getElfType() const;

    const std::string &getInterpreter() const;

    Endian getEndian() const;
    bool isLittleEndian() const;
    bool isBigEndian() const;

  private:
    ElfImage(Elf64_Ehdr *ehdr,
             uint8_t *rawData,
             Sections *sections,
             SectionMap *sectionMap,
             ElfSymbolTable *symbolTable,
             ElfSymbolTable *dynSymbolTable,
             ElfProgramHeader *programHeader);
    ElfImage(Elf32_Ehdr *ehdr,
             uint8_t *rawData,
             Sections *sections,
             SectionMap *sectionMap,
             ElfSymbolTable *symbolTable,
             ElfSymbolTable *dynSymbolTable,
             ElfProgramHeader *programHeader);
    uint8_t _ident[EI_NIDENT];
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
    uint8_t *_rawData;

    std::string _interpreter;

    Sections *_sections;
    SectionMap *_sectionMap;
    ElfSymbolTable *_symbolTable;
    ElfSymbolTable *_dynSymbolTable;
    ElfProgramHeader *_programHeader;
    Type _elfType;
};

};

#endif /* _ELF_IMAGE_H_ */
