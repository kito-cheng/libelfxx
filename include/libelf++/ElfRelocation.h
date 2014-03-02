/*
 * Copyright (C) 2012-2014 Kito Cheng (kito@0xlab.org)
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

#ifndef _LIBELFXX_ELF_RELOCATION_H_
#define _LIBELFXX_ELF_RELOCATION_H_

#include <elf.h>
#include <stdint.h>
#include <string>

namespace libelfxx {

class ElfRelocation {
  public:
    ElfRelocation(uint64_t info, uint64_t offset,
                  uint64_t type, const std::string &symbol);
    ElfRelocation(uint64_t info, uint64_t offset,
                  uint64_t type, const std::string &symbol,
                  uint64_t addend);

    bool isRela() const;
    uint64_t info() const;
    uint64_t addr() const;
    uint64_t addend() const;

    const std::string &symbol() const;
    uint64_t type() const;

  private:
    uint64_t _info;
    uint64_t _offset;
    uint64_t _addend;
    uint64_t _type;
    std::string _symbol;
    bool _rela;

};

};

#endif
