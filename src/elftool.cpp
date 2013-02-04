#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

#include <ElfImage.h>
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
machine2str(uint16_t mach)
{
  switch (mach) {
    case EM_NONE:        return "Unknown machine";
    case EM_M32:         return "AT&T WE32100";
    case EM_SPARC:       return "Sun SPARC";
    case EM_386:         return "Intel 80386";
    case EM_68K:         return "Motorola 68000";
    case EM_88K:         return "Motorola 88000";
    case EM_860:         return "Intel i860";
    case EM_MIPS:        return "MIPS R3000 Big-Endian only";
    case EM_S370:        return "IBM System/370";
    case EM_MIPS_RS3_LE: return "MIPS R3000 Little-Endian";
    case EM_PARISC:      return "HP PA-RISC";
    case EM_VPP500:      return "Fujitsu VPP500";
    case EM_SPARC32PLUS: return "SPARC v8plus";
    case EM_960:         return "Intel 80960";
    case EM_PPC:         return "PowerPC 32-bit";
    case EM_PPC64:       return "PowerPC 64-bit";
    case EM_S390:        return "IBM System/390";
    case EM_V800:        return "NEC V800";
    case EM_FR20:        return "Fujitsu FR20";
    case EM_RH32:        return "TRW RH-32";
    case EM_RCE:         return "Motorola RCE";
    case EM_ARM:         return "ARM";
    case EM_SH:          return "Hitachi SH";
    case EM_SPARCV9:     return "SPARC v9 64-bit";
    case EM_TRICORE:     return "Siemens TriCore embedded processor";
    case EM_ARC:         return "Argonaut RISC Core";
    case EM_H8_300:      return "Hitachi H8/300";
    case EM_H8_300H:     return "Hitachi H8/300H";
    case EM_H8S:         return "Hitachi H8S";
    case EM_H8_500:      return "Hitachi H8/500";
    case EM_IA_64:       return "Intel IA-64 Processor";
    case EM_MIPS_X:      return "Stanford MIPS-X";
    case EM_COLDFIRE:    return "Motorola ColdFire";
    case EM_68HC12:      return "Motorola M68HC12";
    case EM_MMA:         return "Fujitsu MMA";
    case EM_PCP:         return "Siemens PCP";
    case EM_NCPU:        return "Sony nCPU";
    case EM_NDR1:        return "Denso NDR1 microprocessor";
    case EM_STARCORE:    return "Motorola Star*Core processor";
    case EM_ME16:        return "Toyota ME16 processor";
    case EM_ST100:       return "STMicroelectronics ST100 processor";
    case EM_TINYJ:       return "Advanced Logic Corp. TinyJ processor";
    case EM_X86_64:      return "Advanced Micro Devices x86-64";
    case EM_PDSP:        return "Sony DSP Processor";
    case EM_FX66:        return "Siemens FX66 microcontroller";
    case EM_ST9PLUS:     return "STMicroelectronics ST9+ 8/16 microcontroller";
    case EM_ST7:         return "STmicroelectronics ST7 8-bit microcontroller";
    case EM_68HC16:      return "Motorola MC68HC16 microcontroller";
    case EM_68HC11:      return "Motorola MC68HC11 microcontroller";
    case EM_68HC08:      return "Motorola MC68HC08 microcontroller";
    case EM_68HC05:      return "Motorola MC68HC05 microcontroller";
    case EM_SVX:         return "Silicon Graphics SVx";
    case EM_ST19:        return "STMicroelectronics ST19 8-bit mc";
    case EM_VAX:         return "Digital VAX";
    case EM_CRIS:        return "Axis Communications 32-bit embedded processor";
    case EM_JAVELIN:     return "Infineon Tech. 32bit embedded processor";
    case EM_FIREPATH:    return "Element 14 64-bit DSP Processor";
    case EM_ZSP:         return "LSI Logic 16-bit DSP Processor";
    case EM_MMIX:        return "Donald Knuth's educational 64-bit proc";
    case EM_HUANY:       return "Harvard University MI object files";
    case EM_PRISM:       return "SiTera Prism";
    case EM_AVR:         return "Atmel AVR 8-bit microcontroller";
    case EM_FR30:        return "Fujitsu FR30";
    case EM_D10V:        return "Mitsubishi D10V";
    case EM_D30V:        return "Mitsubishi D30V";
    case EM_V850:        return "NEC v850";
    case EM_M32R:        return "Mitsubishi M32R";
    case EM_MN10300:     return "Matsushita MN10300";
    case EM_MN10200:     return "Matsushita MN10200";
    case EM_PJ:          return "picoJava";
    case EM_OPENRISC:    return "OpenRISC 32-bit embedded processor";
    case EM_ARC_A5:      return "ARC Cores Tangent-A5";
    case EM_XTENSA:      return "Tensilica Xtensa Architecture";
    default:             return "Unknown Machine!";
  }
}

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
  const char *machineStr = machine2str(image->getMachine());

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
  for (int i = optind;
       i < argc;
       ++i) {
    libelfxx::ElfImage *image = libelfxx::ElfImage::create(argv[optind]);
    if (optionFlags[FILE_HEADER]) {
      printFileHeader(image);
    }
  }

  return 0;
}
