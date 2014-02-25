/*
 * Copyright (C) 2012-2013 Kito Cheng (kito@0xlab.org)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <ElfDynamicInfo.h>
#include <ElfSection.h>
#include <ElfImage.h>
#include <Debug.h>
#include <inttypes.h>
#include <assert.h>

namespace libelfxx {

template <class Elf_Shdr,
          class Elf_Dyn>
static void
read(Elf_Shdr *dynsec,
     Elf_Shdr *dynstr,
     ElfDynamicInfo::DynamicTable *dyntab,
     ElfDynamic **rawDyns,
     ElfDynamicInfo::NeededList &neededList,
     const uint8_t *rawData) {
  std::vector<ElfDynamic> vRawDyns;
  const Elf_Dyn *dynamic =
    reinterpret_cast<const Elf_Dyn *>(rawData + dynsec->sh_offset);
  const Elf_Dyn *d;
  for (d = dynamic; d->d_tag != DT_NULL; ++d) {
    switch(d->d_tag) {
      case DT_NULL:
      case DT_NEEDED:
      case DT_PLTRELSZ:
      case DT_PLTGOT:
      case DT_HASH:
        break;
      case DT_STRTAB:
        break;
      case DT_SYMTAB:
      case DT_RELA:
      case DT_RELASZ:
      case DT_RELAENT:
      case DT_STRSZ:
      case DT_SYMENT:
      case DT_INIT:
      case DT_FINI:
      case DT_SONAME:
      case DT_RPATH:
      case DT_SYMBOLIC:
      case DT_REL:
      case DT_RELSZ:
      case DT_RELENT:
      case DT_PLTREL:
      case DT_DEBUG:
      case DT_TEXTREL:
      case DT_JMPREL:
      case DT_BIND_NOW:
      case DT_INIT_ARRAY:
      case DT_FINI_ARRAY:
      case DT_INIT_ARRAYSZ:
      case DT_FINI_ARRAYSZ:
      case DT_RUNPATH:
      case DT_FLAGS:
      case DT_PREINIT_ARRAY:
      case DT_PREINIT_ARRAYSZ:

      case DT_VALRNGLO:
      case DT_GNU_PRELINKED:
      case DT_GNU_CONFLICTSZ:
      case DT_GNU_LIBLISTSZ:
      case DT_CHECKSUM:
      case DT_PLTPADSZ:
      case DT_MOVEENT:
      case DT_MOVESZ:
      case DT_FEATURE_1:
      case DT_POSFLAG_1:
      case DT_SYMINSZ:
      case DT_SYMINENT:
      case DT_GNU_HASH:
      case DT_TLSDESC_PLT:
      case DT_TLSDESC_GOT:
      case DT_GNU_CONFLICT:
      case DT_GNU_LIBLIST:
      case DT_CONFIG:
      case DT_DEPAUDIT:
      case DT_AUDIT:
      case DT_PLTPAD:
      case DT_MOVETAB:
      case DT_SYMINFO:
      case DT_VERSYM:
      case DT_RELACOUNT:
      case DT_RELCOUNT:
      case DT_FLAGS_1:
      case DT_VERDEF:
      case DT_VERDEFNUM:
      case DT_VERNEED:
      case DT_VERNEEDNUM:
      case DT_AUXILIARY:
      case DT_FILTER:

      /* MIPS only */
      case DT_MIPS_RLD_VERSION:
      case DT_MIPS_TIME_STAMP:
      case DT_MIPS_ICHECKSUM:
      case DT_MIPS_IVERSION:
      case DT_MIPS_FLAGS:
      case DT_MIPS_BASE_ADDRESS:
      case DT_MIPS_MSYM:
      case DT_MIPS_CONFLICT:
      case DT_MIPS_LIBLIST:
      case DT_MIPS_LOCAL_GOTNO:
      case DT_MIPS_CONFLICTNO:
      case DT_MIPS_LIBLISTNO:
      case DT_MIPS_SYMTABNO:
      case DT_MIPS_UNREFEXTNO:
      case DT_MIPS_GOTSYM:
      case DT_MIPS_HIPAGENO:
      case DT_MIPS_RLD_MAP:
      case DT_MIPS_DELTA_CLASS:
      case DT_MIPS_DELTA_CLASS_NO:
      case DT_MIPS_DELTA_INSTANCE:
      case DT_MIPS_DELTA_INSTANCE_NO:
      case DT_MIPS_DELTA_RELOC:
      case DT_MIPS_DELTA_RELOC_NO:
      case DT_MIPS_DELTA_SYM:
      case DT_MIPS_DELTA_SYM_NO:
      case DT_MIPS_DELTA_CLASSSYM:
      case DT_MIPS_DELTA_CLASSSYM_NO:
      case DT_MIPS_CXX_FLAGS:
      case DT_MIPS_PIXIE_INIT:
      case DT_MIPS_SYMBOL_LIB:
      case DT_MIPS_LOCALPAGE_GOTIDX:
      case DT_MIPS_LOCAL_GOTIDX:
      case DT_MIPS_HIDDEN_GOTIDX:
      case DT_MIPS_PROTECTED_GOTIDX:
      case DT_MIPS_OPTIONS:
      case DT_MIPS_INTERFACE:
      case DT_MIPS_DYNSTR_ALIGN:
      case DT_MIPS_INTERFACE_SIZE:
      case DT_MIPS_RLD_TEXT_RESOLVE_ADDR:
      case DT_MIPS_PERF_SUFFIX:
      case DT_MIPS_COMPACT_SIZE:
      case DT_MIPS_GP_VALUE:
      case DT_MIPS_AUX_DYNAMIC:
      case DT_MIPS_PLTGOT:
      case DT_MIPS_RWPLT:
      default:
        break;
    }
    vRawDyns.push_back(ElfDynamic(d->d_tag, d->d_un.d_val));
  }
  vRawDyns.push_back(ElfDynamic(d->d_tag, d->d_un.d_val));

  *rawDyns = new ElfDynamic[vRawDyns.size()];
  std::copy(vRawDyns.begin(), vRawDyns.end(), *rawDyns);
  for (size_t i=0;i<vRawDyns.size();++i) {
    dyntab->insert(std::make_pair((*rawDyns)[i].tag(), &(*rawDyns)[i]));
  }
  for (d = dynamic; d->d_tag != DT_NULL; ++d) {
    if (d->d_tag == DT_NEEDED) {
      const char *needed =
        reinterpret_cast<const char*>(rawData + dynstr->sh_offset)
        + d->d_un.d_val;
      neededList.push_back(needed);
    }
  }
}

ElfDynamicInfo::ElfDynamicInfo(const std::string &name,
                               Elf32_Shdr *shdr,
                               Elf32_Shdr *strtab,
                               uint8_t *rawData)
  : ElfSection(name, shdr, rawData)
{
  read<Elf32_Shdr, Elf32_Dyn>(
    shdr, strtab, &_dyns, &_rawDyns, _neededList, rawData);
}

ElfDynamicInfo::ElfDynamicInfo(const std::string &name,
                               Elf64_Shdr *shdr,
                               Elf64_Shdr *strtab,
                               uint8_t *rawData)
  : ElfSection(name, shdr, rawData)
{
  read<Elf64_Shdr, Elf64_Dyn>(
    shdr, strtab, &_dyns, &_rawDyns, _neededList, rawData);
}

const ElfDynamicInfo::NeededList &ElfDynamicInfo::needed() const
{
  return _neededList;
}

ElfDynamicInfo::DynamicList ElfDynamicInfo::find(uint64_t tag) const
{
  auto upper_bound = _dyns.upper_bound(tag);
  auto lower_bound = _dyns.lower_bound(tag);
  DynamicList rv;
  for (auto itr = lower_bound;itr != upper_bound;++itr) {
    rv.push_back(*itr->second);
  }
  return rv;
}

const ElfDynamic *ElfDynamicInfo::entries() const
{
  return _rawDyns;
}

ElfDynamic *ElfDynamicInfo::entries()
{
  return _rawDyns;
}

ElfDynamic::ElfDynamic()
  : _tag(DT_NULL)
  , _val(0)
{
}

ElfDynamic::ElfDynamic(uint64_t tag, uint64_t value)
  : _tag(tag)
  , _val(value)
{
}

};
