#include <ElfSection.h>
#include <Debug.h>

#include <stdio.h>
#include <inttypes.h>

namespace libelfxx {

ElfSection::ElfSection(const char *name, Elf32_Shdr *shdr)
  : _nameStr(name)
  , _name(shdr->sh_name)
  , _flags(shdr->sh_flags)
  , _addr(shdr->sh_addr)
  , _offset(shdr->sh_offset)
  , _size(shdr->sh_size)
  , _link(shdr->sh_link)
  , _info(shdr->sh_info)
  , _addralign(shdr->sh_addralign)
  , _entsize(shdr->sh_entsize)
{
}

void ElfSection::print(FILE *fp) {
  PRINT("Section `%s`:\n"
        "  sh_name : %" PRIu32 "\n"
        "  sh_flags : 0x%" PRIx64 "\n"
        "  sh_addr : 0x%" PRIx64 "\n"
        "  sh_offset : 0x%" PRIx64 "\n"
        "  sh_size : 0x%" PRIu64 "\n"
        "  sh_link : %" PRIu32 "\n"
        "  sh_info : 0x%" PRIx32 "\n"
        "  sh_addralign : %" PRIu64 "\n"
        "  sh_entsize : %" PRIu64 "\n",
        _nameStr,
       _name,
       _flags,
       _addr,
       _offset,
       _size,
       _link,
       _info,
       _addralign,
       _entsize);
}

};
