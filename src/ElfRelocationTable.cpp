#include <ElfRelocationTable.h>

namespace libelfxx {

static uint64_t
Elf32_r_sym(Elf32_Word r_info)
{
  return ELF32_R_SYM(r_info);
}

static uint64_t
Elf32_r_type(Elf32_Word r_info)
{
  return ELF32_R_TYPE(r_info);
}

static uint64_t
Elf64_r_sym(Elf64_Xword r_info)
{
  return ELF64_R_SYM(r_info);
}

static uint64_t
Elf64_r_type(Elf64_Xword r_info)
{
  return ELF64_R_TYPE(r_info);
}

template <class Elf_Shdr,
          class Elf_Rel,
          class Elf_Rela,
          class Elf_Sym,
          typename Elf_r_sym_t,
          typename Elf_r_type_t>
static void
read(Elf_Shdr *reltab,
     Elf_Shdr *symtab,
     Elf_Shdr *strtab,
     ElfRelocationTable::Relocations &relctions,
     uint8_t *rawData,
     Elf_r_sym_t Elf_r_sym,
     Elf_r_type_t Elf_r_type) {
  Elf_Sym *syms = reinterpret_cast<Elf_Sym *>(
                    rawData + symtab->sh_offset);
  const char *strs = reinterpret_cast<const char *>(
                       rawData + strtab->sh_offset);
  size_t size = reltab->sh_size / reltab->sh_entsize;
  if (reltab->sh_type == SHT_REL) {
    Elf_Rel *rels = reinterpret_cast<Elf_Rel *>(
                      rawData + reltab->sh_offset);
    for (size_t i=0;i<size;++i) {
      Elf_Rel *rel = rels + i;
      uint64_t type = Elf_r_type(rel->r_info);
      uint64_t symIdx = Elf_r_sym(rel->r_info);
      Elf_Sym *sym = syms + symIdx;
      const char *symname = strs + sym->st_name;
      relctions.push_back(
        ElfRelocation(rel->r_info, rel->r_offset, type, symname));
    }
  } else { /* reltab->sh_tab == SHT_RELA */
    Elf_Rela *relas = reinterpret_cast<Elf_Rela *>(
                      rawData + reltab->sh_offset);
    for (size_t i=0;i<size;++i) {
      Elf_Rela *rela = relas + i;
      uint64_t type = Elf_r_type(rela->r_info);
      uint64_t symIdx = Elf_r_sym(rela->r_info);
      Elf_Sym *sym = syms + symIdx;
      const char *symname = strs + sym->st_name;
      relctions.push_back(
        ElfRelocation(rela->r_info, rela->r_offset,
                      type, symname, rela->r_addend));
    }

  }
}

ElfRelocationTable::ElfRelocationTable(const std::string &name,
                                       Elf32_Shdr *shdr,
                                       Elf32_Shdr *symtab,
                                       Elf32_Shdr *strtab,
                                       uint8_t *rawData)
  : ElfSection(name, shdr, rawData)
{
  read<Elf32_Shdr, Elf32_Rel,
       Elf32_Rela, Elf32_Sym>(
    shdr, symtab, strtab, _relocations, rawData, Elf32_r_sym, Elf32_r_type);
}

ElfRelocationTable::ElfRelocationTable(const std::string &name,
                                       Elf64_Shdr *shdr,
                                       Elf64_Shdr *symtab,
                                       Elf64_Shdr *strtab,
                                       uint8_t *rawData)
  : ElfSection(name, shdr, rawData)
{
  read<Elf64_Shdr, Elf64_Rel,
       Elf64_Rela, Elf64_Sym>(
    shdr, symtab, strtab, _relocations, rawData, Elf64_r_sym, Elf64_r_type);
}

ElfRelocationTable::~ElfRelocationTable() {
}

ElfRelocationTable::iterator ElfRelocationTable::begin() {
  return _relocations.begin();
}

ElfRelocationTable::iterator ElfRelocationTable::end() {
  return _relocations.end();
}

ElfRelocationTable::const_iterator ElfRelocationTable::begin() const {
  return _relocations.begin();
}

ElfRelocationTable::const_iterator ElfRelocationTable::end() const {
  return _relocations.end();
}

size_t ElfRelocationTable::size() const {
  return _relocations.size();
}

};
