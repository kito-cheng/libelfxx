#include <ElfRelocation.h>

namespace libelfxx {

ElfRelocation::ElfRelocation(uint64_t info, uint64_t offset,
                             uint64_t type, const std::string &symbol)
  : _info(info)
  , _offset(offset)
  , _addend(0)
  , _type(type)
  , _symbol(symbol)
  , _rela(false)
{
}

ElfRelocation::ElfRelocation(uint64_t info, uint64_t offset,
                             uint64_t type, const std::string &symbol,
                             uint64_t addend)
  : _info(info)
  , _offset(offset)
  , _addend(addend)
  , _type(type)
  , _symbol(symbol)
  , _rela(true)
{
}

};
