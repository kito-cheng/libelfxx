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

CXX_preferred=clang++

define get_cxx
    $(if $(shell which $(CXX_preferred)),$(CXX_preferred),"c++")
endef

define get_cxx_standard
    $(if $(shell echo | $(CXX) -x c++ - -std=c++11 - -E >/dev/null; \
                 expr $$? = 0),"-std=c++11","-std=c++0x")
endef

CXX=$(call get_cxx)
CXX11STDFLAG=$(call get_cxx_standard)
DEBUG_CXXFLAGS=-MMD -MP -MT $@ -MF debug/$(*F).d -g3 -O0
RELEASE_CXXFLAGS=-MMD -MP -MT $@ -MF release/$(*F).d -O2
AR=ar
AR_FLAGS=rcs
CXXFLAGS=-Wall -fPIC -I include $(CXX11STDFLAG) -D__STDC_FORMAT_MACROS -Wextra
