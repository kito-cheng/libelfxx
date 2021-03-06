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

#ifndef _LIBELFXX_ELF_DYNAMIC_INFO_H_
#define _LIBELFXX_ELF_DYNAMIC_INFO_H_

#include <libelf++/ElfImage.h>
#include <libelf++/ElfSection.h>
#include <elf.h>
#include <stdint.h>
#include <map>
#include <vector>

namespace libelfxx {

class ElfDynamic {
  public:
    ElfDynamic();
    ElfDynamic(uint64_t tag, uint64_t value);
    uint64_t tag() const {return _tag;};
    uint64_t d_tag() const {return _tag;};
    uint64_t val() const {return _val;};
    uint64_t d_val() const {return _val;};
    uint64_t d_ptr() const {return _val;};
  private:
    uint64_t _tag;
    uint64_t _val;
};

class ElfDynamicInfo : public ElfSection {
  public:
    typedef std::multimap<uint64_t, ElfDynamic*> DynamicTable;
    typedef std::vector<ElfDynamic> DynamicList;
    typedef std::vector<std::string> NeededList;

    ElfDynamicInfo(const std::string &name,
                   Elf32_Shdr *strtab,
                   Elf32_Shdr *dynsec,
                   uint8_t *rawData);
    ElfDynamicInfo(const std::string &name,
                   Elf64_Shdr *strtab,
                   Elf64_Shdr *dynsec,
                   uint8_t *rawData);

    const NeededList &needed() const;
    DynamicList find(uint64_t tag) const;
    ElfDynamic *entries();
    const ElfDynamic *entries() const;

  private:
    DynamicTable _dyns;
    ElfDynamic *_rawDyns; /* Keep ElfDynamic as original order in .dynamic  */
    NeededList _neededList;
};

};

#endif
