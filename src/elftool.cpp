#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

#include <ElfImage.h>
#include <ElfProgramHeader.h>
#include <ElfSegment.h>
#include <ElfSection.h>
#include <Debug.h>

enum Option {
  DUMMY,
  ALL,
  FILE_HEADER,
  PROGRAM_HEADER,
  SEGMENTS,
  SECTION_HEADER,
  SECTIONS,
  SECTION_GROUPS,
  SECTION_DETAILS,
  HEADERS,
  SYMBOLS,
  SYMS,
  DYN_SYMS,
  NOTES,
  RELOCS,
  UNWIND,
  DYNAMIC,
  VERSION_INFO,
  ARCH_SPECIFIC,
  ARCHIVE_INDEX,
  USE_DYNAMIC,
  HEX_DUMP,
  STRING_DUMP,
  RELOCATED_DUMP,
  DEBUG_DUMP,
  DWARF_DEPTH,
  DWARF_START,
  HISTOGRAM,
  WIDE,
  HELP,
  VERSION,
  NUMBER_OF_OPTION,
  MIN_ARG_OPTION = HEX_DUMP,
  MAX_ARG_OPTION = DWARF_START,
};

static struct option options[] =
{
  [DUMMY]            = {"",                      no_argument, 0, ' '},
  [ALL]              = {"all",                   no_argument, 0, 'a'},
  [FILE_HEADER]      = {"file-header",           no_argument, 0, 'h'},
  [PROGRAM_HEADER]   = {"program-header",        no_argument, 0, 'l'},
  [SEGMENTS]         = {"segments",              no_argument, 0, 'l'},
  [SECTION_HEADER]   = {"section-headers",       no_argument, 0, 'S'},
  [SECTIONS]         = {"sections",              no_argument, 0, 'S'},
  [SECTION_GROUPS]   = {"section-groups",        no_argument, 0, 'g'},
  [SECTION_DETAILS]  = {"section-details",       no_argument, 0, 't'},
  [HEADERS]          = {"headers",               no_argument, 0, 'e'},
  [SYMBOLS]          = {"symbols",               no_argument, 0, 's'},
  [SYMS]             = {"syms",                  no_argument, 0, 's'},
  [DYN_SYMS]         = {"dyn-syms",              no_argument, 0,   0},
  [NOTES]            = {"notes",                 no_argument, 0, 'n'},
  [RELOCS]           = {"relocs",                no_argument, 0, 'r'},
  [UNWIND]           = {"unwind",                no_argument, 0, 'u'},
  [DYNAMIC]          = {"dynamic",               no_argument, 0, 'd'},
  [VERSION_INFO]     = {"version-info",          no_argument, 0, 'V'},
  [ARCH_SPECIFIC]    = {"arch-specific",         no_argument, 0, 'A'},
  [ARCHIVE_INDEX]    = {"archive-index",         no_argument, 0, 'c'},
  [USE_DYNAMIC]      = {"use-dynamic",           no_argument, 0, 'D'},
  [HEX_DUMP]         = {"hex-dump",        required_argument, 0, 'x'},
  [STRING_DUMP]      = {"string-dump",     required_argument, 0, 'p'},
  [RELOCATED_DUMP]   = {"relocated-dump",  required_argument, 0, 'R'},
  [DEBUG_DUMP]       = {"debug-dump",      required_argument, 0, 'w'},
  [DWARF_DEPTH]      = {"dwarf-depth",     required_argument, 0,   0},
  [DWARF_START]      = {"dwarf-start",     required_argument, 0,   0},
  [HISTOGRAM]        = {"histogram",             no_argument, 0, 'I'},
  [WIDE]             = {"wide",                  no_argument, 0, 'W'},
  [HELP]             = {"help",                  no_argument, 0, 'H'},
  [VERSION]          = {"version",               no_argument, 0, 'v'},
  [NUMBER_OF_OPTION] = { NULL,                              0, 0,   0}
};

static const char *usageStr =
  "Usage: elftool <option(s)> elf-file(s)\n"
  " Display information about the contents of ELF format files\n"
  " Options are:\n"
  "  -a --all               Equivalent to: -h -l -S -s -r -d -V -A -I\n"
  "  -h --file-header       Display the ELF file header\n"
  "  -l --program-headers   Display the program headers\n"
  "     --segments          An alias for --program-headers\n"
  "  -S --section-headers   Display the sections' header\n"
  "     --sections          An alias for --section-headers\n"
  "  -g --section-groups    Display the section groups\n"
  "  -t --section-details   Display the section details\n"
  "  -e --headers           Equivalent to: -h -l -S\n"
  "  -s --syms              Display the symbol table\n"
  "     --symbols           An alias for --syms\n"
  "  --dyn-syms             Display the dynamic symbol table\n"
  "  -n --notes             Display the core notes (if present)\n"
  "  -r --relocs            Display the relocations (if present)\n"
  "  -u --unwind            Display the unwind info (if present)\n"
  "  -d --dynamic           Display the dynamic section (if present)\n"
  "  -V --version-info      Display the version sections (if present)\n"
  "  -A --arch-specific     Display architecture specific "
  "                         information (if any)\n"
  "  -c --archive-index     Display the symbol/file index in an archive\n"
  "  -D --use-dynamic       Use the dynamic section info when "
  "                         displaying symbols\n"
  "  -x --hex-dump=<number|name>\n"
  "                         Dump the contents of section <number|name>"
  "                         as bytes\n"
  "  -p --string-dump=<number|name>\n"
  "                         Dump the contents of section <number|name>"
  "                         as strings\n"
  "  -R --relocated-dump=<number|name>\n"
  "                         Dump the contents of section <number|name>"
  "                         as relocated bytes\n"
  "  -w[lLiaprmfFsoRt] or\n"
  "  --debug-dump[=rawline,=decodedline,=info,=abbrev,=pubnames,=aranges,\n"
  "               =macro,=frames,=frames-interp,=str,=loc,=Ranges,=pubtypes,\n"
  "               =gdb_index,=trace_info,=trace_abbrev,=trace_aranges]\n"
  "                         Display the contents of DWARF2 debug sections\n"
  "  --dwarf-depth=N        Do not display DIEs at depth N or greater\n"
  "  --dwarf-start=N        Display DIEs starting with N, at the same depth\n"
  "                         or deeper\n"
  "  -I --histogram         Display histogram of bucket list lengths\n"
  "  -W --wide              Allow output width to exceed 80 characters\n"
  "  @<file>                Read options from <file>\n"
  "  -H --help              Display this information\n"
  "  -v --version           Display the version number of readelf\n";

static void usage() {
  puts(usageStr);
}

static std::string optionArgs[MAX_ARG_OPTION - MIN_ARG_OPTION];

static bool optionFlags[NUMBER_OF_OPTION] ={
  [DUMMY]          = false,
  [ALL]            = false,
  [FILE_HEADER]    = false,
  [PROGRAM_HEADER] = false,
  [SEGMENTS]       = false,
  [SECTION_HEADER] = false,
  [SECTIONS]       = false,
  [SECTION_GROUPS] = false,
  [SECTION_DETAILS]= false,
  [HEADERS]        = false,
  [SYMBOLS]        = false,
  [SYMS]           = false,
  [DYN_SYMS]       = false,
  [NOTES]          = false,
  [RELOCS]         = false,
  [UNWIND]         = false,
  [DYNAMIC]        = false,
  [VERSION_INFO]   = false,
  [ARCH_SPECIFIC]  = false,
  [ARCHIVE_INDEX]  = false,
  [USE_DYNAMIC]    = false,
  [HEX_DUMP]       = false,
  [STRING_DUMP]    = false,
  [RELOCATED_DUMP] = false,
  [DEBUG_DUMP]     = false,
  [DWARF_DEPTH]    = false,
  [DWARF_START]    = false,
  [HISTOGRAM]      = false,
  [WIDE]           = false,
  [HELP]           = false,
  [VERSION]        = false,
};

static const char *
osabi2str(uint8_t abi)
{
  switch(abi) {
  case ELFOSABI_SYSV:       return "UNIX - System V";
  case ELFOSABI_HPUX:       return "HPUS";
  case ELFOSABI_NETBSD:     return "NetBSD";
  case ELFOSABI_GNU:        return "GNU";
  case ELFOSABI_SOLARIS:    return "Solaris";
  case ELFOSABI_AIX:        return "AIX";
  case ELFOSABI_IRIX:       return "IRIX";
  case ELFOSABI_FREEBSD:    return "FreeBSD";
  case ELFOSABI_TRU64:      return "TRU64";
  case ELFOSABI_MODESTO:    return "MODESTO";
  case ELFOSABI_OPENBSD:    return "OpenBSD";
  case ELFOSABI_ARM:        return "ARM";
  case ELFOSABI_STANDALONE: return "StandAlone";
  default:                  return "Unknown ABI!";
  }
}

static const char *
elftype2str(uint16_t type)
{
  switch (type) {
  case ET_NONE: return "NONE (None)";
  case ET_REL: return  "REL (Relocatable file)";
  case ET_EXEC: return "EXEC (Executable file)";
  case ET_DYN: return  "DYN (Shared object file)";
  case ET_CORE: return "CORE (Core file)";
  default:
    {
      if (type >= ET_LOPROC) {
        return "ELF TYPE PROC";
      } else if (type >= ET_LOOS && type <= ET_HIOS) {
        return "ELF TYPE OS";
      } else {
        return "ELF TYPE UNKNOWN";
      }
    }
  }
}

static const char*
type2str(libelfxx::ElfImage::Type type) {
 switch (type) {
    case libelfxx::ElfImage::ELF32:      return "ELF32";
    case libelfxx::ElfImage::ELF64:      return "ELF64";
    case libelfxx::ElfImage::ELFINVALID: return "ELFNONE";
    default:                             return "ELF???";
  }
}

const char *
endian2str(uint8_t endian) {
  switch (endian) {
    case ELFDATANONE: return "none";
    case ELFDATA2LSB: return "2's complement, little endian";
    case ELFDATA2MSB: return "2's complement, big endian";
    default:          return "Unknown endian!";
  }
}

static const char *
phdrType2str(uint32_t phdrType) {
  switch (phdrType) {
    case PT_NULL:         return "NULL";
    case PT_LOAD:         return "LOAD";
    case PT_DYNAMIC:      return "DYNAMIC";
    case PT_INTERP:       return "INTERP";
    case PT_NOTE:         return "NOTE";
    case PT_SHLIB:        return "SHLIB";
    case PT_PHDR:         return "PHDR";
    case PT_TLS:          return "TLS";
    case PT_GNU_EH_FRAME: return "GNU_EH_FRAME";
    case PT_GNU_STACK:    return "GNU_STACK";
    case PT_GNU_RELRO:    return "GNU_RELRO";
    default:
      if (phdrType >= PT_LOPROC && phdrType <= PT_HIPROC){
        return "PROC";
      } else if (phdrType >= PT_LOOS && phdrType <= PT_HIOS) {
        return "OS";
      } else {
        return "UNKNOWN";
      }
  }
}

void printFileHeader(libelfxx::ElfImage *image) {
  const uint8_t *ident = image->getIdent();

  uint8_t endian = ident[EI_DATA];
  uint8_t osabi = ident[EI_OSABI];
  uint8_t abiVersion = ident[EI_ABIVERSION];
  const char *currentStr = "";
  if (image->getVersion() == EV_CURRENT) {
    currentStr = " (current)";
  }
  const char *endianStr = endian2str(endian);
  const char *typeStr = type2str(image->getElfType());
  const char *elftypeStr = elftype2str(image->getType());
  const char *osabiStr = osabi2str(osabi);
  const char *machineStr = image->getMachineString();

  printf("ELF Header:\n");
  printf("  Magic:   ");
  for (int i = 0;i<EI_NIDENT;++i) {
    printf("%02x ", ident[i]);
  }
  printf("\n");
  printf("  Class:                             %s\n",
         typeStr);
  printf("  Data:                              %s\n", endianStr);
  printf("  Version:                           %" PRIu8 "%s\n",
         image->getVersion(), currentStr);
  printf("  OS/ABI:                            %s\n",
         osabiStr);
  printf("  ABI Version:                       %" PRIu8 "\n",
         abiVersion);
  printf("  Type:                              %s\n",
         elftypeStr);
  printf("  Machine:                           %s\n",
         machineStr);
  printf("  Version:                           0x%" PRIx32 "\n",
         image->getVersion());
  printf("  Entry point address:               0x%" PRIx64 "\n",
         image->getEntry());
  printf("  Start of program headers:          %" PRIu64 " (bytes into file)\n",
         image->getPhoff());
  printf("  Start of section headers:          %" PRIu64 " (bytes into file)\n",
         image->getShoff());
  printf("  Flags:                             0x%" PRIx32 "\n",
         image->getFlags());
  printf("  Size of this header:               %" PRId16 " (bytes)\n",
         image->getEhsize());
  printf("  Size of program headers:           %" PRId16 " (bytes)\n",
         image->getPhentsize());
  printf("  Number of program headers:         %" PRId16 "\n",
         image->getPhnum());
  printf("  Size of section headers:           %" PRId16 " (bytes)\n",
         image->getShentsize());
  printf("  Number of section headers:         %" PRId16 "\n",
         image->getShnum());
  printf("  Section header string table index: %" PRId16 "\n",
         image->getShstrndx());
}

static void
printProgramHeaders(libelfxx::ElfImage *image){
  const char *elftypeStr = elftype2str(image->getType());
  libelfxx::ElfProgramHeader *programHeader = image->getProgramHeader();
  printf("\n");
  printf("Elf file type is %s\n", elftypeStr);
  printf("Entry point 0x%" PRIx64 "\n", image->getEntry());
  printf("There are %" PRId16 " program headers, starting at "
         "offset %" PRIu64 "\n",
         image->getPhnum(),
         image->getPhoff());
  printf("\nProgram Headers:\n");
  switch (image->getElfType()) {
    case libelfxx::ElfImage::ELF32:
       {
          printf("  Type           Offset   VirtAddr   PhysAddr   "
                 "FileSiz MemSiz  Flg Align\n");
       }
       break;
    case libelfxx::ElfImage::ELF64:
       {
          printf("  Type           Offset             VirtAddr"
                 "           PhysAddr\n"
                 "                 FileSiz            MemSiz"
                 "              Flags  Align\n");
       }
       break;
    default:
       FATAL("Unknown Elf Type!\n");
  }

  for (auto *segment : *programHeader) {
    uint64_t offset = segment->getOffset();
    uint64_t vaddr = segment->getVaddr();
    uint64_t paddr = segment->getPaddr();
    uint32_t filesz = segment->getFilesz();
    uint32_t memsz = segment->getMemsz();
    uint32_t flags = segment->getFlags();
    uint32_t align = segment->getAlign();
    const char *phdrType = phdrType2str(segment->getType());
    switch (image->getElfType()) {
      case libelfxx::ElfImage::ELF32:
         {
            printf("  %-14.14s 0x%6.6" PRIx64 " 0x%8.8" PRIx64
                   " 0x%8.8" PRIx64 " 0x%5.5" PRIx32 " 0x%5.5" PRIx32
                   " %c%c%c %#" PRIx32 "\n",
                   phdrType, offset, vaddr, paddr, filesz, memsz,
                   flags & PF_R ? 'R' : ' ',
                   flags & PF_W ? 'W' : ' ',
                   flags & PF_X ? 'E' : ' ',
                   align);
         }
         break;
      case libelfxx::ElfImage::ELF64:
         {
            printf("  %-14.14s 0x%16.16" PRIx64 " 0x%16.16" PRIx64
                   " 0x%16.16" PRIx64 " 0x%16.16" PRIx32 " 0x%16.16" PRIx32
                   " %c%c%c %#" PRIx32 "\n",
                   phdrType, offset, vaddr, paddr, filesz, memsz,
                   flags & PF_R ? 'R' : ' ',
                   flags & PF_W ? 'W' : ' ',
                   flags & PF_X ? 'E' : ' ',
                   align);
         }
         break;
      default:
         FATAL("Unknown Elf Type!\n");
    }
    if (segment->getType() == PT_INTERP) {
      printf("      [Requesting program interpreter: %s]\n",
             image->getInterpreter().c_str());
    }
  }
  printf("\n");
  printf(" Section to Segment mapping:\n");
  printf("  Segment Sections...\n");
  for (unsigned i = 0;
       i<programHeader->getSegmentNum();
       ++i) {
    libelfxx::ElfSegment *segment = programHeader->getSegment(i);
    printf("   %2.2u     ", i);
    uint64_t beginAddr = segment->getPaddr();
    uint64_t endAddr =  beginAddr + segment->getMemsz();
    for (unsigned j = 1; /* Skip NULL section */
         j<image->getSectionNum();
         ++j) {
      libelfxx::ElfSection *section = image->getSection(j);
      uint64_t addr = section->getAddr();
      if (addr >= beginAddr && addr < endAddr) {
        printf("%s ", section->getNameStr());
      }
    }
    printf("\n");
  }
}

typedef void (*OptionHandler)(libelfxx::ElfImage*);
static OptionHandler optionHandler[NUMBER_OF_OPTION] ={
  [DUMMY]          = nullptr,
  [ALL]            = nullptr,
  [FILE_HEADER]    = printFileHeader,
  [PROGRAM_HEADER] = printProgramHeaders,
  [SEGMENTS]       = printProgramHeaders,
  [SECTION_HEADER] = nullptr,
  [SECTIONS]       = nullptr,
  [SECTION_GROUPS] = nullptr,
  [SECTION_DETAILS]= nullptr,
  [HEADERS]        = nullptr,
  [SYMBOLS]        = nullptr,
  [SYMS]           = nullptr,
  [DYN_SYMS]       = nullptr,
  [NOTES]          = nullptr,
  [RELOCS]         = nullptr,
  [UNWIND]         = nullptr,
  [DYNAMIC]        = nullptr,
  [VERSION_INFO]   = nullptr,
  [ARCH_SPECIFIC]  = nullptr,
  [ARCHIVE_INDEX]  = nullptr,
  [USE_DYNAMIC]    = nullptr,
  [HEX_DUMP]       = nullptr,
  [STRING_DUMP]    = nullptr,
  [RELOCATED_DUMP] = nullptr,
  [DEBUG_DUMP]     = nullptr,
  [DWARF_DEPTH]    = nullptr,
  [DWARF_START]    = nullptr,
  [HISTOGRAM]      = nullptr,
  [WIDE]           = nullptr,
  [HELP]           = nullptr,
  [VERSION]        = nullptr,
};

int main(int argc, char * const *argv) {
  int c;
  while (1) {
    int option_index = 0;
    c = getopt_long(argc, argv, "ahlSgtesnrudVAcDx:p:R:w:IWHv",
                    options, &option_index);
    if (option_index == DUMMY && c == '?') {
      usage();
      break;
    }
    if (c == -1) {
      break;
    }
    if (option_index == DUMMY){
      for (int i=0;i<NUMBER_OF_OPTION;++i) {
        if (c == options[i].val){
          option_index = i;
          break;
        }
      }
    }
    DEBUG("c=%c(%d) option_index=%d\n", c, c, option_index);
    optionFlags[option_index] = true;
    if (option_index > MIN_ARG_OPTION &&
        option_index < MAX_ARG_OPTION) {
      optionArgs[option_index - MIN_ARG_OPTION] = optarg;
    }
  }
  if (optionFlags[HELP]) {
    usage();
  }
  if (optionFlags[VERSION]) {
    printf("libelfxx elftool version 0.01\n");
  }

  if (optionFlags[ALL]) {
    std::fill(optionFlags, optionFlags+NUMBER_OF_OPTION, true);
  }

  for (int i = optind;
       i < argc;
       ++i) {
    libelfxx::ElfImage *image = libelfxx::ElfImage::create(argv[optind]);
    for (int optidx = 0;optidx<NUMBER_OF_OPTION;++optidx) {
      if (optionFlags[optidx] && optionHandler[optidx] != nullptr) {
        optionHandler[optidx](image);
      }
    }
  }

  return 0;
}
