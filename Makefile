#
# Copyright (C) 2012-2013 Kito Cheng (kito@0xlab.org)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

include config.mak
include rules.mak

OBJS=ElfImage.o ElfSection.o ElfSymbol.o ElfSymbolTable.o \
     ElfSegment.o ElfProgramHeader.o
ELFTOOL_OBJS=elftool.o
LIBELFPP_so=debug/libelf++.so
LIBELFPP_a=debug/libelf++.a
ELFTOOL=debug/elftool

all: $(LIBELFPP_so) $(ELFTOOL)

$(LIBELFPP_so): $(addprefix debug/,$(OBJS))
	$(call LINK_SO,$^)

$(LIBELFPP_a): $(addprefix debug/,$(OBJS))
	$(call LINK_AR,$^)

$(ELFTOOL): $(addprefix debug/,$(ELFTOOL_OBJS)) $(LIBELFPP_a)
	$(call LINK_EXE,$^)

clean:
	rm debug/ -rf
	rm release/ -rf

doxygen:
	doxygen docs/doxyfile.cfg

-include $(wildcard debug/*.d)
-include $(wildcard release/*.d)
