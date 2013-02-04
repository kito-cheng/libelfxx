quiet-command = $(if $(V),$1,$(if $(2),@echo $2 && $1, @$1))

LINK_SO = $(call quiet-command,$(CXX) $(CXXFLAGS) $(LDFLAGS) -shared \
            -o $@ $(sort $(1)) $(2) $(LIBS),"  LINK  $(TARGET_DIR)$@")

LINK_AR = $(call quiet-command,$(AR) $(AR_FLAGS) $(LDFLAGS) \
            -o $@ $(sort $(1)) $(2) $(LIBS),"  LINK  $(TARGET_DIR)$@")

LINK_EXE = $(call quiet-command,$(CXX) $(CXXFLAGS) $(LDFLAGS) \
             -o $@ $(sort $(1)) $(2) $(LIBS),"  LINK  $(TARGET_DIR)$@")

release/%.o: src/%.cpp
	@mkdir -p release/
	$(call quiet-command,$(CXX) $(CXXFLAGS) $(RELEASE_CXXFLAGS) \
                             -c -o $@ $<,"  CXX    $(TARGET_DIR)$@")

debug/%.o: src/%.cpp
	@mkdir -p debug/
	$(call quiet-command,$(CXX) $(CXXFLAGS) $(DEBUG_CXXFLAGS) \
                             -c -o $@ $<,"  CXX    $(TARGET_DIR)$@")
