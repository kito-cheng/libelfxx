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

#ifndef _LIBELFXX_ELF_GNU_LIB_H_
#define _LIBELFXX_ELF_GNU_LIB_H_

#include <vector>
#include <libelf++/ElfSection.h>
#include <string>

namespace libelfxx {

class ElfGNULib {
  public:
    ElfGNULib(const std::string &name,
              time_t timestamp,
              uint32_t checksum,
              uint32_t version,
              uint32_t flags);

    const std::string &name() const;
    time_t timestamp() const;
    uint32_t checksum() const;
    uint32_t version() const;
    uint32_t flags() const;

  private:
    std::string _name;
    time_t _timestamp;
    uint32_t _checksum;
    uint32_t _version;
    uint32_t _flags;
};

};

#endif
