#ifndef _LIBELFXX_ELF_SYMBOL_H_
#define _LIBELFXX_ELF_SYMBOL_H_

#include <elf.h>
#include <stdint.h>

namespace libelfxx {

class ElfSymbol {
  public:
    ElfSymbol(const char *name, Elf32_Sym *sym);
  private:
    const char *_nameStr;
    uint32_t _name;
    uint8_t _info;
    uint8_t _other;
    uint16_t _shndx;
    uint64_t _value;
    uint64_t _size;
};

};

#endif
