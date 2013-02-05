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

-include $(wildcard debug/*.d)
-include $(wildcard release/*.d)
