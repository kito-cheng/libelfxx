#include <ElfGNULibList.h>
#include <ElfGNULib.h>
#include <sstream>

namespace libelfxx {

template <class Elf_Shdr,
          class Elf_Lib>
static void
read(Elf_Shdr *shdr,
     Elf_Shdr *strtab,
     uint8_t *rawData,
     ElfGNULibList::GNULibs &gnulibs) {
  size_t size = shdr->sh_size / shdr->sh_entsize;
  const char *strs = reinterpret_cast<const char*>(
                       rawData + strtab->sh_offset);
  Elf_Lib *elflibs = reinterpret_cast<Elf_Lib *>(
                       rawData + shdr->sh_offset);
  for (size_t i=0;i<size;++i) {
    Elf_Lib *elflib = elflibs + i;
    gnulibs.push_back(ElfGNULib(strs + elflib->l_name,
                                elflib->l_time_stamp,
                                elflib->l_checksum,
                                elflib->l_version,
                                elflib->l_flags));
  }
}

ElfGNULibList::ElfGNULibList(const std::string &name,
                             Elf32_Shdr *shdr,
                             Elf32_Shdr *strtab,
                             uint8_t *rawData)
  : ElfSection(name, shdr, rawData)
{
  read<Elf32_Shdr, Elf32_Lib>(shdr, strtab, rawData, _libs);
}

ElfGNULibList::ElfGNULibList(const std::string &name,
                             Elf64_Shdr *shdr,
                             Elf64_Shdr *strtab,
                             uint8_t *rawData)
  : ElfSection(name, shdr, rawData)
{
  read<Elf64_Shdr, Elf32_Lib>(shdr, strtab, rawData, _libs);
}

ElfGNULibList::~ElfGNULibList() {
}

ElfGNULibList::iterator ElfGNULibList::begin() {
  return _libs.begin();
}

ElfGNULibList::iterator ElfGNULibList::end() {
  return _libs.end();
}

ElfGNULibList::const_iterator ElfGNULibList::begin() const {
  return _libs.begin();
}

ElfGNULibList::const_iterator ElfGNULibList::end() const {
  return _libs.end();
}

size_t ElfGNULibList::size() const {
  return _libs.size();
}

ElfGNULib ElfGNULibList::get(size_t i) const {
  return _libs[i];
}

ElfGNULib ElfGNULibList::operator[](size_t i) const {
  return _libs[i];
}

};
