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

#include <ElfImage.h>
#include <ElfSection.h>
#include <ElfSymbolTable.h>
#include <ElfProgramHeader.h>
#include <ElfDynamicInfo.h>
#include <Debug.h>

#include <stdio.h>
#include <stdint.h>
#include <elf.h>

#include <string>

namespace libelfxx {

static std::string initInterpreter(libelfxx::ElfImage::SectionMap *sectionMap) {
  auto itr = sectionMap->find(".interp");
  if (itr != sectionMap->end()) {
    return std::string(reinterpret_cast<const char*>(itr->second->content()));
  } else {
    return std::string("");
  }
}


ElfImage::ElfImage(Elf64_Ehdr *ehdr,
                   uint8_t *rawData,
                   Sections *sections,
                   SectionMap *sectionMap,
                   ElfSymbolTable *symbolTable,
                   ElfSymbolTable *dynSymbolTable,
                   ElfProgramHeader *programHeader,
                   ElfDynamicInfo *dynamic)
  : _type(ehdr->e_type)
  , _machine(ehdr->e_machine)
  , _version(ehdr->e_version)
  , _entry(ehdr->e_entry)
  , _phoff(ehdr->e_phoff)
  , _shoff(ehdr->e_shoff)
  , _flags(ehdr->e_flags)
  , _ehsize(ehdr->e_ehsize)
  , _phentsize(ehdr->e_phentsize)
  , _phnum(ehdr->e_phnum)
  , _shentsize(ehdr->e_shentsize)
  , _shnum(ehdr->e_shnum)
  , _shstrndx(ehdr->e_shstrndx)
  , _rawData(rawData)
  , _interpreter(initInterpreter(sectionMap))
  , _sections(sections)
  , _sectionMap(sectionMap)
  , _symbolTable(symbolTable)
  , _dynSymbolTable(dynSymbolTable)
  , _programHeader(programHeader)
  , _dynamicInfo(dynamic)
  , _elfType(Type::ELF64)
{
  std::copy(ehdr->e_ident, ehdr->e_ident+EI_NIDENT, _ident);
}

ElfImage::ElfImage(Elf32_Ehdr *ehdr,
                   uint8_t *rawData,
                   Sections *sections,
                   SectionMap *sectionMap,
                   ElfSymbolTable *symbolTable,
                   ElfSymbolTable *dynSymbolTable,
                   ElfProgramHeader *programHeader,
                   ElfDynamicInfo *dynamic)
  : _type(ehdr->e_type)
  , _machine(ehdr->e_machine)
  , _version(ehdr->e_version)
  , _entry(ehdr->e_entry)
  , _phoff(ehdr->e_phoff)
  , _shoff(ehdr->e_shoff)
  , _flags(ehdr->e_flags)
  , _ehsize(ehdr->e_ehsize)
  , _phentsize(ehdr->e_phentsize)
  , _phnum(ehdr->e_phnum)
  , _shentsize(ehdr->e_shentsize)
  , _shnum(ehdr->e_shnum)
  , _shstrndx(ehdr->e_shstrndx)
  , _rawData(rawData)
  , _interpreter(initInterpreter(sectionMap))
  , _sections(sections)
  , _sectionMap(sectionMap)
  , _symbolTable(symbolTable)
  , _dynSymbolTable(dynSymbolTable)
  , _programHeader(programHeader)
  , _dynamicInfo(dynamic)
  , _elfType(Type::ELF32)
{
  std::copy(ehdr->e_ident, ehdr->e_ident+EI_NIDENT, _ident);
}

ElfImage::~ElfImage() {
  delete []_rawData;
  delete _sections;
  delete _sectionMap;
  delete _symbolTable;
  delete _dynSymbolTable;
  delete _programHeader;
}

ElfImage *ElfImage::create(const char *path) {
  FILE *fp = fopen(path, "rb");
  debug("Open file `%s`...\n", path);
  if (fp) {
    ElfImage *image = create(fp);
    fclose(fp);
    return image;
  } else {
    error("Fail to open file %s\n", path);
    return nullptr;
  }
}

static bool elfValid(unsigned char elfIdent[EI_NIDENT]) {
  if ((elfIdent[EI_MAG0] == ELFMAG0) &&
      (elfIdent[EI_MAG1] == ELFMAG1) &&
      (elfIdent[EI_MAG2] == ELFMAG2) &&
      (elfIdent[EI_MAG3] == ELFMAG3)) {
    return true;
  } else {
    return false;
  }
}

template<class Elf_Ehdr>
struct ElfImageData {
  Elf_Ehdr *ehdr;
  uint8_t *rawData;
  ElfImage::Sections *sections;
  ElfImage::SectionMap *sectionMap;
  ElfSymbolTable *symbolTable;
  ElfSymbolTable *dynSymbolTable;
  ElfProgramHeader *programHeader;
  ElfDynamicInfo *dynamicInfo;
};



template<class ElfImageData,
         class Elf_Ehdr,
         class Elf_Shdr>
static bool _create(FILE *fp, ElfImageData *data,
                    ElfImage::Type elfType) {
  uint8_t *rawData = nullptr;
  fseek(fp, 0l, SEEK_END);
  long int filesize = ftell(fp);
  fseek(fp, 0l, SEEK_SET);

  rawData = new uint8_t[filesize];
  do {
    if(fread(rawData, filesize, 1, fp) < 1) {
      error("Read data fail!\n");
      break;
    }
    Elf_Ehdr *ehdr = reinterpret_cast<Elf_Ehdr*>(rawData);
    if (ehdr->e_shstrndx == SHN_XINDEX) {
      error("Section string table not found!\n");
      break;
    }
    Elf_Shdr *shdrs = reinterpret_cast<Elf_Shdr*>(rawData + ehdr->e_shoff);
    Elf_Shdr *shstrtab = &shdrs[ehdr->e_shstrndx];
    ElfImage::Sections *sections = new ElfImage::Sections();
    ElfImage::SectionMap *sectionMap = new ElfImage::SectionMap();
    int shnum = ehdr->e_shnum;
    for (int i=0;i<shnum;++i) {
       Elf_Shdr *shdr = &shdrs[i];
       const char *sectionName = (const char*)(rawData +
                                               shstrtab->sh_offset +
                                               shdr->sh_name);
       ElfSection *section = new ElfSection(sectionName, shdr, rawData);
       sections->push_back(section);
       sectionMap->insert(std::make_pair(sectionName, section));
    }

    auto symtabItr = sectionMap->find(".symtab");
    auto strtabItr = sectionMap->find(".strtab");
    ElfSymbolTable *symbolTable = NULL;
    if (symtabItr != sectionMap->end() && strtabItr != sectionMap->end()) {
      ElfSection *symtab = symtabItr->second;
      ElfSection *strtab = strtabItr->second;
      symbolTable =
        new ElfSymbolTable(symtab, strtab, rawData, elfType);
    }

    symtabItr = sectionMap->find(".dynsym");
    strtabItr = sectionMap->find(".dynstr");
    ElfSymbolTable *dynSymbolTable = NULL;
    if (symtabItr != sectionMap->end() && strtabItr != sectionMap->end()) {
      ElfSection *symtab = symtabItr->second;
      ElfSection *strtab = strtabItr->second;
      dynSymbolTable =
        new ElfSymbolTable(symtab, strtab, rawData, elfType);
    }
    ElfProgramHeader *programHeader = new ElfProgramHeader(ehdr, rawData);
    data->ehdr = ehdr;
    data->rawData = rawData;
    data->sections = sections;
    data->sectionMap = sectionMap;
    data->symbolTable = symbolTable;
    data->dynSymbolTable = dynSymbolTable;
    data->programHeader = programHeader;
    auto dynSecItr = sectionMap->find(".dynamic");
    if (dynSecItr != sectionMap->end()) {
      ElfSection *dynSec = dynSecItr->second;
      data->dynamicInfo = new ElfDynamicInfo(*sectionMap, dynSec, elfType);
    } else {
      data->dynamicInfo = nullptr;
    }
    return true;
  } while (0);
  /* Error handling... */
  delete []rawData;
  return false;
}

ElfImage *ElfImage::create(FILE *fp) {
  if (fp == nullptr) {
    return nullptr;
  }
  unsigned char elfIdent[EI_NIDENT];

  fread(&elfIdent, EI_NIDENT, 1, fp);

  if (!elfValid(elfIdent)) {
    error("Input file is not valid elf file!\n");
    return nullptr;
  }

  switch (elfIdent[EI_CLASS]) {
    case ELFCLASS32:
      {
        ElfImageData<Elf32_Ehdr> data;
        if (_create<ElfImageData<Elf32_Ehdr>,
                    Elf32_Ehdr, Elf32_Shdr>(fp, &data, ELF32)){
          return new ElfImage(data.ehdr, data.rawData,
                              data.sections, data.sectionMap,
                              data.symbolTable, data.dynSymbolTable,
                              data.programHeader, data.dynamicInfo);
        }
      }
      break;
    case ELFCLASS64:
      {
        ElfImageData<Elf64_Ehdr> data;
        if (_create<ElfImageData<Elf64_Ehdr>,
                    Elf64_Ehdr, Elf64_Shdr>(fp, &data, ELF64)){
          return new ElfImage(data.ehdr, data.rawData,
                              data.sections, data.sectionMap,
                              data.symbolTable, data.dynSymbolTable,
                              data.programHeader, data.dynamicInfo);
        }
      }
      break;
    case 0: /* Invalid elf type? */
    default:
      error("Invalid ELFCLASS! type = %x\n", elfIdent[EI_CLASS]);
      break;
  }
  return nullptr;
}


const uint8_t *ElfImage::ident() const {
  return _ident;
}

uint16_t ElfImage::type() const {
  return _type;
}

uint16_t ElfImage::machine() const {
  return _machine;
}

uint32_t ElfImage::version() const {
  return _version;
}

uint64_t ElfImage::entry() const {
  return _entry;
}

uint64_t ElfImage::phoff() const {
  return _phoff;
}

uint64_t ElfImage::shoff() const {
  return _shoff;
}

uint32_t ElfImage::flags() const {
  return _flags;
}

uint16_t ElfImage::ehsize() const {
  return _ehsize;
}

uint16_t ElfImage::phentsize() const {
  return _phentsize;
}

uint16_t ElfImage::phnum() const {
  return _phnum;
}

uint16_t ElfImage::shentsize() const {
  return _shentsize;
}

uint16_t ElfImage::shnum() const {
  return _shnum;
}

uint16_t ElfImage::shstrndx() const {
  return _shstrndx;
}

size_t ElfImage::sectionNum() const {
  return _shnum;
}

size_t ElfImage::segmentNum() const {
  return _phnum;
}

ElfSection *ElfImage::section(size_t idx) {
  if (idx < _shnum) {
    return (*_sections)[idx];
  } else {
    return nullptr;
  }
}

ElfSection *ElfImage::section(const char *name) {
  auto itr = _sectionMap->find(name);
  if (itr == _sectionMap->end()) {
    return nullptr;
  } else {
    return itr->second;
  }
}

const ElfProgramHeader *ElfImage::programHeader() const {
  return _programHeader;
}

ElfProgramHeader *ElfImage::programHeader() {
  return _programHeader;
}

ElfImage::Type ElfImage::elfType() const {
  return _elfType;
}

const std::string &ElfImage::interpreter() const {
  return _interpreter;
}

bool ElfImage::isElf32() const {
  return _elfType == ELF32;
}
bool ElfImage::isElf64() const {
  return _elfType == ELF64;
}

static const char *
machine2str(uint16_t mach)
{
  switch (mach) {
    case EM_NONE:        return "Unknown machine";
    case EM_M32:         return "AT&T WE32100";
    case EM_SPARC:       return "Sun SPARC";
    case EM_386:         return "Intel 80386";
    case EM_68K:         return "Motorola 68000";
    case EM_88K:         return "Motorola 88000";
    case EM_860:         return "Intel i860";
    case EM_MIPS:        return "MIPS R3000 Big-Endian only";
    case EM_S370:        return "IBM System/370";
    case EM_MIPS_RS3_LE: return "MIPS R3000 Little-Endian";
    case EM_PARISC:      return "HP PA-RISC";
    case EM_VPP500:      return "Fujitsu VPP500";
    case EM_SPARC32PLUS: return "SPARC v8plus";
    case EM_960:         return "Intel 80960";
    case EM_PPC:         return "PowerPC 32-bit";
    case EM_PPC64:       return "PowerPC 64-bit";
    case EM_S390:        return "IBM System/390";
    case EM_V800:        return "NEC V800";
    case EM_FR20:        return "Fujitsu FR20";
    case EM_RH32:        return "TRW RH-32";
    case EM_RCE:         return "Motorola RCE";
    case EM_ARM:         return "ARM";
    case EM_SH:          return "Hitachi SH";
    case EM_SPARCV9:     return "SPARC v9 64-bit";
    case EM_TRICORE:     return "Siemens TriCore embedded processor";
    case EM_ARC:         return "Argonaut RISC Core";
    case EM_H8_300:      return "Hitachi H8/300";
    case EM_H8_300H:     return "Hitachi H8/300H";
    case EM_H8S:         return "Hitachi H8S";
    case EM_H8_500:      return "Hitachi H8/500";
    case EM_IA_64:       return "Intel IA-64 Processor";
    case EM_MIPS_X:      return "Stanford MIPS-X";
    case EM_COLDFIRE:    return "Motorola ColdFire";
    case EM_68HC12:      return "Motorola M68HC12";
    case EM_MMA:         return "Fujitsu MMA";
    case EM_PCP:         return "Siemens PCP";
    case EM_NCPU:        return "Sony nCPU";
    case EM_NDR1:        return "Denso NDR1 microprocessor";
    case EM_STARCORE:    return "Motorola Star*Core processor";
    case EM_ME16:        return "Toyota ME16 processor";
    case EM_ST100:       return "STMicroelectronics ST100 processor";
    case EM_TINYJ:       return "Advanced Logic Corp. TinyJ processor";
    case EM_X86_64:      return "Advanced Micro Devices x86-64";
    case EM_PDSP:        return "Sony DSP Processor";
    case EM_FX66:        return "Siemens FX66 microcontroller";
    case EM_ST9PLUS:     return "STMicroelectronics ST9+ 8/16 microcontroller";
    case EM_ST7:         return "STmicroelectronics ST7 8-bit microcontroller";
    case EM_68HC16:      return "Motorola MC68HC16 microcontroller";
    case EM_68HC11:      return "Motorola MC68HC11 microcontroller";
    case EM_68HC08:      return "Motorola MC68HC08 microcontroller";
    case EM_68HC05:      return "Motorola MC68HC05 microcontroller";
    case EM_SVX:         return "Silicon Graphics SVx";
    case EM_ST19:        return "STMicroelectronics ST19 8-bit mc";
    case EM_VAX:         return "Digital VAX";
    case EM_CRIS:        return "Axis Communications 32-bit embedded processor";
    case EM_JAVELIN:     return "Infineon Tech. 32bit embedded processor";
    case EM_FIREPATH:    return "Element 14 64-bit DSP Processor";
    case EM_ZSP:         return "LSI Logic 16-bit DSP Processor";
    case EM_MMIX:        return "Donald Knuth's educational 64-bit proc";
    case EM_HUANY:       return "Harvard University MI object files";
    case EM_PRISM:       return "SiTera Prism";
    case EM_AVR:         return "Atmel AVR 8-bit microcontroller";
    case EM_FR30:        return "Fujitsu FR30";
    case EM_D10V:        return "Mitsubishi D10V";
    case EM_D30V:        return "Mitsubishi D30V";
    case EM_V850:        return "NEC v850";
    case EM_M32R:        return "Mitsubishi M32R";
    case EM_MN10300:     return "Matsushita MN10300";
    case EM_MN10200:     return "Matsushita MN10200";
    case EM_PJ:          return "picoJava";
    case EM_OPENRISC:    return "OpenRISC 32-bit embedded processor";
    case EM_ARC_A5:      return "ARC Cores Tangent-A5";
    case EM_XTENSA:      return "Tensilica Xtensa Architecture";
    default:             return "Unknown Machine!";
  }
}

const char* ElfImage::machineString() const {
  return machine2str(_machine);
}

ElfImage::Endian ElfImage::endian() const {
  switch (_ident[EI_DATA]) {
    case ELFDATANONE: return ELF_NONE_ENDIAN;
    case ELFDATA2LSB: return ELF_LITTLE_ENDIAN;
    case ELFDATA2MSB: return ELF_BIG_ENDIAN;
    default:          return ELF_UNKNOWN_ENDIAN;
  }
}

bool ElfImage::isLittleEndian() const {
  return _ident[EI_DATA] == ELF_LITTLE_ENDIAN;
}

bool ElfImage::isBigEndian() const {
  return _ident[EI_DATA] == ELF_LITTLE_ENDIAN;
}

ElfImage::iterator ElfImage::begin() {
  return _sections->begin();
}

ElfImage::iterator ElfImage::end() {
  return _sections->end();
}

ElfImage::const_iterator ElfImage::begin() const {
  return (const_iterator)_sections->begin();
}

ElfImage::const_iterator ElfImage::end() const {
  return (const_iterator)_sections->end();
}

ElfImage::const_iterator ElfImage::cbegin() const {
  return (const_iterator)_sections->begin();
}

ElfImage::const_iterator ElfImage::cend() const {
  return (const_iterator)_sections->end();
}

template <typename SymbolTable>
static SymbolTable *
getSymbolTable(bool preferDyn, SymbolTable *dynSymTab, SymbolTable *symTab)
{
  if (preferDyn && dynSymTab)
    return dynSymTab;

  if (symTab)
    return symTab;
  else
    return dynSymTab;
}

const ElfSymbolTable *
ElfImage::symbolTable(bool preferDynamicSymbolTable) const {
  return getSymbolTable(
           preferDynamicSymbolTable, _dynSymbolTable, _symbolTable);
}

ElfSymbolTable *ElfImage::symbolTable(bool preferDynamicSymbolTable) {
  return getSymbolTable(
           preferDynamicSymbolTable, _dynSymbolTable, _symbolTable);
}

const ElfSymbol *
ElfImage::findSymbol(uint64_t address, bool findNearIfPossible) const {
  const ElfSymbolTable *symTab = symbolTable();
  if (symTab)
    return symTab->find(address, findNearIfPossible);
  else
    return nullptr;
}

};
