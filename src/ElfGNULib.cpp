#include <ElfGNULib.h>
#include <sstream>

namespace libelfxx {

ElfGNULib::ElfGNULib(const std::string &name,
                     time_t timestamp,
                     uint32_t checksum,
                     uint32_t version,
                     uint32_t flags)
  : _name(name)
  , _timestamp(timestamp)
  , _checksum(checksum)
  , _version(version)
  , _flags(flags)
{
}

const std::string &ElfGNULib::name() const {
  return _name;
}

time_t ElfGNULib::timestamp() const {
  return _timestamp;
}

uint32_t ElfGNULib::checksum() const {
  return _checksum;
}

uint32_t ElfGNULib::version() const {
  return _version;
}

uint32_t ElfGNULib::flags() const {
  return _flags;
}

};
