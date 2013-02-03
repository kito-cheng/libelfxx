#ifndef _LIBELFXX_ELF_SYMBOL_H_
#define _LIBELFXX_ELF_SYMBOL_H_

#include <elf.h>
#include <stdint.h>
#include <stdio.h>

namespace libelfxx {

class ElfSymbol {
  public:
    enum class Type {
      NOTYPE,
      OBJECT,
      FUNCTION,
      SECTION,
      FILE,
      OTHER,
    };
    enum class Bind {
      LOCAL,
      GLOBAL,
      WEAK,
      OTHER,
    };

    ElfSymbol(const char *name, Elf32_Sym *sym);

    bool isLocal() const;
    bool isGlobal() const;
    bool isWeak() const;
    Bind getBind() const;

    bool isNoType() const;
    bool isObject() const;
    bool isFunction() const;
    bool isSection() const;
    bool isFile() const;
    Type getType() const;

    void print(FILE *fp);
  private:
    const char *_nameStr;
    uint32_t _name;
    uint8_t _info;
    uint8_t _other;
    uint16_t _shndx;
    uint64_t _value;
    uint64_t _size;
    Bind _bind;
    Type _type;
};

};

#endif
