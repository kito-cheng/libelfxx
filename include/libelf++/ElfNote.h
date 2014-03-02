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

#ifndef _LIBELFXX_ELF_NOTE_H_
#define _LIBELFXX_ELF_NOTE_H_

#include <libelf++/ElfSection.h>
#include <libelf++/ElfRelocation.h>
#include <string>

namespace libelfxx {

class ElfNote : public ElfSection {
  public:
    ElfNote(const std::string &name,
            Elf32_Shdr *shdr,
            uint8_t *rawData);

    ElfNote(const std::string &name,
            Elf64_Shdr *shdr,
            uint8_t *rawData);

    virtual ~ElfNote();

    const std::string &note() const;

  private:
    std::string _note;
};

};

#endif
