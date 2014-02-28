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

#ifndef _LIBELF_PP_H_
#define _LIBELF_PP_H_

#include<libelf++/ElfDynamicInfo.h>
#include<libelf++/ElfGNULib.h>
#include<libelf++/ElfGNULibList.h>
#include<libelf++/ElfImage.h>
#include<libelf++/ElfNote.h>
#include<libelf++/ElfProgramHeader.h>
#include<libelf++/ElfRelocation.h>
#include<libelf++/ElfRelocationTable.h>
#include<libelf++/ElfSection.h>
#include<libelf++/ElfSegment.h>
#include<libelf++/ElfSymbol.h>
#include<libelf++/ElfSymbolTable.h>

namespace libelfxx {
  void setDebug(bool);
};

#endif /* _LIBELF_PP_H_ */
