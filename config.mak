CXX=clang++
CXX11STDFLAG= `echo "" |$(CXX) -x c++ - -std=c++11 - -E &>/dev/null; \
               if [ $$? == 0 ] ; \
                 then echo "-std=c++11" ; \
               else \
                 echo "-std=c++0x"; \
               fi;`
DEBUG_CXXFLAGS=-MMD -MP -MT $@ -MF debug/$(*F).d -g3 -O0
RELEASE_CXXFLAGS=-MMD -MP -MT $@ -MF release/$(*F).d -O2
AR=ar
AR_FLAGS=rcs
CXXFLAGS=-Wall -fPIC -I include $(CXX11STDFLAG) -D__STDC_FORMAT_MACROS
