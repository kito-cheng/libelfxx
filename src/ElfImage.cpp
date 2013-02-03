#include <ElfImage.h>
#include <ElfSection.h>
#include <ElfSymbolTable.h>
#include <Debug.h>

#include <stdio.h>
#include <stdint.h>
#include <elf.h>

#include <string>

namespace libelfxx {

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

static ElfImage *create32(FILE *fp) {
  uint8_t *rawData = nullptr;
  fseek(fp, 0l, SEEK_END);
  long int filesize = ftell(fp);
  fseek(fp, 0l, SEEK_SET);

  rawData = new uint8_t[filesize];
  do {
    ElfImage *image = nullptr;
    if(fread(rawData, filesize, 1, fp) < 1) {
      ERROR("Read data fail!\n");
      break;
    }
    Elf32_Ehdr *ehdr = (Elf32_Ehdr*)rawData;
    if (ehdr->e_shstrndx == SHN_XINDEX) {
      ERROR("Section string table not found!\n");
      break;
    }
    Elf32_Shdr *shdrs = (Elf32_Shdr*)(rawData + ehdr->e_shoff);
    Elf32_Shdr *shstrtab = &shdrs[ehdr->e_shstrndx];
    ElfImage::Sections sections;
    ElfImage::SectionMap sectionMap;
    int shnum = ehdr->e_shnum;
    for (int i=0;i<shnum;++i) {
       Elf32_Shdr *shdr = &shdrs[i];
       const char *sectionName = (const char*)(rawData +
                                               shstrtab->sh_offset +
                                               shdr->sh_name);
       DEBUG("Reading section `%s`...\n", sectionName);
       ElfSection *section = new ElfSection(sectionName, shdr);
       sections.push_back(section);
       sectionMap.insert(std::make_pair(sectionName, section));

       section->print(stdout);
    }

    auto symtabItr = sectionMap.find(".symtab");
    auto strtabItr = sectionMap.find(".strtab");
    DEBUG("Symtab = %d\n", symtabItr != sectionMap.end());
    DEBUG("Strtab = %d\n", strtabItr != sectionMap.end());
    if (symtabItr != sectionMap.end() && strtabItr != sectionMap.end()) {
      ElfSection *symtab = symtabItr->second;
      ElfSection *strtab = strtabItr->second;
      ElfSymbolTable *symbolTable =
        new ElfSymbolTable(symtab, strtab, rawData, true/* is elf32*/);
      for (auto symItr : *symbolTable) {
        ElfSymbol *sym = symItr.second;
        sym->print(stdout);
      }
    }
    return image;
  } while (0);
  /* Error handling... */
  delete []rawData;
  return nullptr;
}

static ElfImage *create64(FILE *fp) {
  ERROR("ELF 64 not support yet!\n");
  return nullptr;
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
      return create32(fp);
      break;
    case ELFCLASS64:
      return create64(fp);
      break;
    case 0: /* Invalid elf type? */
    default:
      ERROR("Invalid ELFCLASS! type = %x\n", elfIdent[EI_CLASS]);
      break;
  }
  return nullptr;
}

};
