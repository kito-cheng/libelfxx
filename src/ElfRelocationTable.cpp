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

static uint64_t
Elf32_st_type(unsigned char st_info)
{
  return ELF32_ST_TYPE (st_info);
}

static uint64_t
Elf64_st_type(unsigned char st_info)
{
  return ELF64_ST_TYPE (st_info);
}

template <class Elf_Shdr_t,
          class Elf_Rel_t,
          class Elf_Sym,
          typename Elf_r_sym_t,
          typename Elf_r_type_t,
          typename Elf_st_type_t>
static void
proc_rel(Elf_Shdr_t *shdrs,
         int shnum,
         const char *shstrtab,
         Elf_Rel_t *rel,
         uint64_t addend,
         Elf_Sym *syms,
         const char *strs,
         ElfRelocationTable::Relocations &relocations,
         Elf_r_sym_t Elf_r_sym,
         Elf_r_type_t Elf_r_type,
         Elf_st_type_t Elf_st_type) {
  uint64_t type = Elf_r_type(rel->r_info);
  uint64_t symIdx = Elf_r_sym(rel->r_info);
  Elf_Sym *sym = syms + symIdx;

  const char *symname = strs + sym->st_name;

  if (Elf_st_type(sym->st_info) == STT_GNU_IFUNC) {
    relocations.push_back(
      ElfRelocation(rel->r_info, 0, type, symname, addend));
  } else {
    if (sym->st_name == 0) {
      if (Elf_st_type(sym->st_info) == STT_SECTION) {
        const char *section_name = nullptr;
        if (sym->st_shndx < shnum) {
          section_name = shstrtab + shdrs[sym->st_shndx].sh_name;
        } else {
          switch (sym->st_shndx) {
            case SHN_ABS:    section_name = "ABS";
            case SHN_COMMON: section_name = "COMMON";
            default:         section_name = "<null>";
          }
        }
        relocations.push_back(
          ElfRelocation(rel->r_info, rel->r_offset, type, section_name, addend));
      } else {
        relocations.push_back(
          ElfRelocation(rel->r_info, rel->r_offset, type, "", addend));
      }
    } else {
      relocations.push_back(
        ElfRelocation(rel->r_info, rel->r_offset, type, symname, addend));
    }
  }
}

template <class Elf_Shdr,
          class Elf_Rel,
          class Elf_Rela,
          class Elf_Sym,
          typename Elf_r_sym_t,
          typename Elf_r_type_t,
          typename Elf_st_type_t>
static void
read(
  Elf_Shdr *shdrs,
  int shnum,
  const char *shstrtab,
  Elf_Shdr *reltab,
  Elf_Shdr *symtab,
  Elf_Shdr *strtab,
  ElfRelocationTable::Relocations &relocations,
  uint8_t *rawData,
  Elf_r_sym_t Elf_r_sym,
  Elf_r_type_t Elf_r_type,
  Elf_st_type_t Elf_st_type) {
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
      proc_rel<Elf_Shdr, Elf_Rel, Elf_Sym>(
        shdrs, shnum, shstrtab,
        rel, 0, syms, strs, relocations,
        Elf_r_sym, Elf_r_type, Elf_st_type);
    }
  } else { /* reltab->sh_tab == SHT_RELA */
    Elf_Rela *relas = reinterpret_cast<Elf_Rela *>(
                      rawData + reltab->sh_offset);
    for (size_t i=0;i<size;++i) {
      Elf_Rela *rela = relas + i;
      proc_rel<Elf_Shdr, Elf_Rela, Elf_Sym>(
        shdrs, shnum, shstrtab,
        rela, rela->r_addend, syms, strs, relocations,
        Elf_r_sym, Elf_r_type, Elf_st_type);
    }

  }
}

ElfRelocationTable::ElfRelocationTable(
  Elf32_Shdr *shdrs,
  int shnum,
  const char *shstrtab,
  const std::string &name,
  Elf32_Shdr *shdr,
  Elf32_Shdr *symtab,
  Elf32_Shdr *strtab,
  uint8_t *rawData)
  : ElfSection(name, shdr, rawData)
{
  read<Elf32_Shdr, Elf32_Rel,
       Elf32_Rela, Elf32_Sym>(
    shdrs, shnum, shstrtab,
    shdr, symtab, strtab, _relocations, rawData,
    Elf32_r_sym, Elf32_r_type, Elf32_st_type);
}

ElfRelocationTable::ElfRelocationTable(
  Elf64_Shdr *shdrs,
  int shnum,
  const char *shstrtab,
  const std::string &name,
  Elf64_Shdr *shdr,
  Elf64_Shdr *symtab,
  Elf64_Shdr *strtab,
  uint8_t *rawData)
  : ElfSection(name, shdr, rawData)
{
  read<Elf64_Shdr, Elf64_Rel,
       Elf64_Rela, Elf64_Sym>(
    shdrs, shnum, shstrtab,
    shdr, symtab, strtab, _relocations, rawData,
    Elf64_r_sym, Elf64_r_type, Elf64_st_type);
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
