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

bool ElfRelocation::isRela() const {
  return _rela;
}

uint64_t ElfRelocation::info() const {
  return _info;
}

uint64_t ElfRelocation::offset() const {
  return _offset;
}

uint64_t ElfRelocation::addend() const {
  return _addend;
}

const std::string &ElfRelocation::symbol() const {
  return _symbol;
}

uint64_t ElfRelocation::type() const {
  return _type;
}

};
