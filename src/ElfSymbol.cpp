#include <ElfSymbol.h>
#include <inttypes.h>

namespace libelfxx {

static ElfSymbol::Bind info2bind32 (uint8_t info) {
  switch (ELF32_ST_BIND(info)) {
    case STB_LOCAL:  return ElfSymbol::Bind::LOCAL;
    case STB_GLOBAL: return ElfSymbol::Bind::GLOBAL;
    case STB_WEAK:   return ElfSymbol::Bind::WEAK;
    default:         return ElfSymbol::Bind::OTHER;
  }
}

static ElfSymbol::Type info2type32 (uint8_t info) {
  switch (ELF32_ST_TYPE(info)) {
    case STT_NOTYPE:  return ElfSymbol::Type::NOTYPE;
    case STT_OBJECT:  return ElfSymbol::Type::OBJECT;
    case STT_FUNC:    return ElfSymbol::Type::FUNCTION;
    case STT_SECTION: return ElfSymbol::Type::SECTION;
    case STT_FILE:    return ElfSymbol::Type::FILE;
    default:          return ElfSymbol::Type::OTHER;
  }
}

static ElfSymbol::Bind info2bind64 (uint8_t info) {
  switch (ELF64_ST_BIND(info)) {
    case STB_LOCAL:  return ElfSymbol::Bind::LOCAL;
    case STB_GLOBAL: return ElfSymbol::Bind::GLOBAL;
    case STB_WEAK:   return ElfSymbol::Bind::WEAK;
    default:         return ElfSymbol::Bind::OTHER;
  }
}

static ElfSymbol::Type info2type64 (uint8_t info) {
  switch (ELF64_ST_TYPE(info)) {
    case STT_NOTYPE:  return ElfSymbol::Type::NOTYPE;
    case STT_OBJECT:  return ElfSymbol::Type::OBJECT;
    case STT_FUNC:    return ElfSymbol::Type::FUNCTION;
    case STT_SECTION: return ElfSymbol::Type::SECTION;
    case STT_FILE:    return ElfSymbol::Type::FILE;
    default:          return ElfSymbol::Type::OTHER;
  }
}


ElfSymbol::ElfSymbol(const char *name, Elf32_Sym *sym)
  : _nameStr(name)
  , _name(sym->st_name)
  , _info(sym->st_info)
  , _other(sym->st_other)
  , _shndx(sym->st_shndx)
  , _value(sym->st_value)
  , _size(sym->st_size)
  , _bind(info2bind32(sym->st_info))
  , _type(info2type32(sym->st_info))
{
}

ElfSymbol::ElfSymbol(const char *name, Elf64_Sym *sym)
  : _nameStr(name)
  , _name(sym->st_name)
  , _info(sym->st_info)
  , _other(sym->st_other)
  , _shndx(sym->st_shndx)
  , _value(sym->st_value)
  , _size(sym->st_size)
  , _bind(info2bind64(sym->st_info))
  , _type(info2type64(sym->st_info))
{
}


void ElfSymbol::print(FILE *fp) {
  fprintf(fp, "Symbol `%s`:"
              "  st_name : %" PRIu32 "\n"
              "  st_info : %" PRIx8 "\n"
              "  st_other : %" PRIx8 "\n"
              "  st_shndx : %" PRIu16 "\n"
              "  st_value : %" PRIx64 "\n"
              "  st_size : %" PRIx64 "\n",
              _nameStr,
              _name,
              _info,
              _other,
              _shndx,
              _value,
              _size);
}

bool ElfSymbol::isLocal() const {
  return _bind == Bind::LOCAL;
}

bool ElfSymbol::isGlobal() const {
  return _bind == Bind::GLOBAL;
}

bool ElfSymbol::isWeak() const {
  return _bind == Bind::WEAK;
}

ElfSymbol::Bind ElfSymbol::getBind() const {
  return _bind;
}

bool ElfSymbol::isNoType() const {
  return _type == Type::NOTYPE;
}

bool ElfSymbol::isObject() const {
  return _type == Type::OBJECT;
}

bool ElfSymbol::isFunction() const {
  return _type == Type::FUNCTION;
}

bool ElfSymbol::isSection() const {
  return _type == Type::SECTION;
}

bool ElfSymbol::isFile() const {
  return _type == Type::FILE;
}

ElfSymbol::Type ElfSymbol::getType() const {
  return _type;
}

};
