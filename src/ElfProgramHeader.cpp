#include <ElfProgramHeader.h>
#include <ElfSegment.h>

namespace libelfxx {

template <class Elf_Ehdr,
          class Elf_Phdr>
static std::vector<ElfSegment*> getSections(Elf_Ehdr *ehdr, uint8_t *rawData) {
  std::vector<ElfSegment*> elfSegments;
  uint64_t phoff = ehdr->e_phoff;
  Elf_Phdr *phdrs = reinterpret_cast<Elf_Phdr*>(rawData + phoff);
  for (int i = 0;i < ehdr->e_phnum; ++i) {
    Elf_Phdr *phdr = &phdrs[i];
    ElfSegment *elfSegment = new ElfSegment(phdr);
    elfSegments.push_back(elfSegment);
  }
  return elfSegments;
}

ElfProgramHeader::ElfProgramHeader(Elf32_Ehdr *ehdr, uint8_t *rawData)
  : _segments(getSections<Elf32_Ehdr, Elf32_Phdr>(ehdr, rawData))
{
}

ElfProgramHeader::ElfProgramHeader(Elf64_Ehdr *ehdr, uint8_t *rawData)
  : _segments(getSections<Elf64_Ehdr, Elf64_Phdr>(ehdr, rawData))
{

}

ElfSegment *ElfProgramHeader::getSegment(unsigned i) {
  if (i < _segments.size()) {
    return _segments[i];
  } else {
    return nullptr;
  }
}

size_t ElfProgramHeader::getSegmentNum() const {
  return _segments.size();
}

ElfProgramHeader::iterator ElfProgramHeader::begin() {
  return _segments.begin();
}

ElfProgramHeader::iterator ElfProgramHeader::end() {
  return _segments.end();
}

ElfProgramHeader::const_iterator ElfProgramHeader::begin() const {
  return _segments.begin();
}

ElfProgramHeader::const_iterator ElfProgramHeader::end() const {
  return _segments.end();
}

};
