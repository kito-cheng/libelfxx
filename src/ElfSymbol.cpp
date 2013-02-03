#include <ElfSymbol.h>

namespace libelfxx {

ElfSymbol::ElfSymbol(const char *name, Elf32_Sym *sym)
  : _nameStr(name)
  , _name(sym->st_name)
  , _info(sym->st_info)
  , _other(sym->st_other)
  , _shndx(sym->st_shndx)
  , _value(sym->st_value)
  , _size(sym->st_size)
{
}

};
