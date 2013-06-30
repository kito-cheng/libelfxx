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

quiet-command = $(if $(V),$1,$(if $(2),@echo $2 && $1, @$1))

LINK_SO = $(call quiet-command,$(CXX) $(CXXFLAGS) $(LDFLAGS) -shared \
            -o $@ $(sort $(1)) $(2) $(LIBS),"  LINK   $(TARGET_DIR)$@")

LINK_AR = $(call quiet-command,$(AR) $(AR_FLAGS) $(LDFLAGS) \
            -o $@ $(sort $(1)) $(2) $(LIBS),"  LINK   $(TARGET_DIR)$@")

LINK_EXE = $(call quiet-command,$(CXX) $(CXXFLAGS) $(LDFLAGS) \
             -o $@ $(sort $(1)) $(2) $(LIBS),"  LINK   $(TARGET_DIR)$@")

release/%.o: src/%.cpp
	@mkdir -p release/
	$(call quiet-command,$(CXX) $(CXXFLAGS) $(RELEASE_CXXFLAGS) \
                             -c -o $@ $<,"  CXX    $(TARGET_DIR)$@")

debug/%.o: src/%.cpp
	@mkdir -p debug/
	$(call quiet-command,$(CXX) $(CXXFLAGS) $(DEBUG_CXXFLAGS) \
                             -c -o $@ $<,"  CXX    $(TARGET_DIR)$@")
