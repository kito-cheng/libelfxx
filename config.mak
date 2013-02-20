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
CXXFLAGS=-Wall -fPIC -I include $(CXX11STDFLAG) -D__STDC_FORMAT_MACROS
