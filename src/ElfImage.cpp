#include <ElfImage.h>
#include <ElfSection.h>
#include <ElfSymbolTable.h>
#include <ElfProgramHeader.h>
#include <Debug.h>

#include <stdio.h>
#include <stdint.h>
#include <elf.h>

#include <string>

namespace libelfxx {

static std::string initInterpreter(libelfxx::ElfImage::SectionMap *sectionMap) {
  auto itr = sectionMap->find(".interp");
  if (itr != sectionMap->end()) {
    return std::string(reinterpret_cast<char*>(itr->second->getContent()));
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
                   ElfProgramHeader *programHeader)
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
                   ElfProgramHeader *programHeader)
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
  , _elfType(Type::ELF32)
{
  std::copy(ehdr->e_ident, ehdr->e_ident+EI_NIDENT, _ident);
}


ElfImage *ElfImage::create(const char *path) {
  FILE *fp = fopen(path, "rb");
  DEBUG("Open file `%s`...\n", path);
  if (fp) {
    ElfImage *image = create(fp);
    fclose(fp);
    return image;
  } else {
    ERROR("Fail to open file %s\n", path);
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
      ERROR("Read data fail!\n");
      break;
    }
    Elf_Ehdr *ehdr = reinterpret_cast<Elf_Ehdr*>(rawData);
    if (ehdr->e_shstrndx == SHN_XINDEX) {
      ERROR("Section string table not found!\n");
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
    ERROR("Input file is not valid elf file!\n");
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
                              data.programHeader);
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
                              data.programHeader);
        }
      }
      break;
    case 0: /* Invalid elf type? */
    default:
      ERROR("Invalid ELFCLASS! type = %x\n", elfIdent[EI_CLASS]);
      break;
  }
  return nullptr;
}


const uint8_t *ElfImage::getIdent() const {
  return _ident;
}

uint16_t ElfImage::getType() const {
  return _type;
}

uint16_t ElfImage::getMachine() const {
  return _machine;
}

uint32_t ElfImage::getVersion() const {
  return _version;
}

uint64_t ElfImage::getEntry() const {
  return _entry;
}

uint64_t ElfImage::getPhoff() const {
  return _phoff;
}

uint64_t ElfImage::getShoff() const {
  return _shoff;
}

uint32_t ElfImage::getFlags() const {
  return _flags;
}

uint16_t ElfImage::getEhsize() const {
  return _ehsize;
}

uint16_t ElfImage::getPhentsize() const {
  return _phentsize;
}

uint16_t ElfImage::getPhnum() const {
  return _phnum;
}

uint16_t ElfImage::getShentsize() const {
  return _shentsize;
}

uint16_t ElfImage::getShnum() const {
  return _shnum;
}

uint16_t ElfImage::getShstrndx() const {
  return _shstrndx;
}

size_t ElfImage::getSectionNum() const {
  return _shnum;
}

size_t ElfImage::getSegmentNum() const {
  return _phnum;
}

ElfSection *ElfImage::getSection(unsigned idx) {
  if (idx < _shnum) {
    return (*_sections)[idx];
  } else {
    return nullptr;
  }
}

ElfSection *ElfImage::getSection(const char *name) {
  auto itr = _sectionMap->find(name);
  if (itr == _sectionMap->end()) {
    return nullptr;
  } else {
    return itr->second;
  }
}

ElfProgramHeader *ElfImage::getProgramHeader() {
  return _programHeader;
}

ElfImage::Type ElfImage::getElfType() const {
  return _elfType;
}

const std::string &ElfImage::getInterpreter() const {
  return _interpreter;
}

bool ElfImage::isElf32() const {
  return _elfType == ELF32;
}
bool ElfImage::isElf64() const {
  return _elfType == ELF64;
}

};
