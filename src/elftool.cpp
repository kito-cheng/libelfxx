#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

#include <ElfImage.h>
#include <Debug.h>

enum Option {
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

static struct option options[NUMBER_OF_OPTION] =
{
  [ALL]            = {"all",                   no_argument, 0, 'a'},
  [FILE_HEADER]    = {"file-header",           no_argument, 0, 'f'},
  [PROGRAM_HEADER] = {"program-header",        no_argument, 0, 'h'},
  [SEGMENTS]       = {"segments",              no_argument, 0, 'h'},
  [SECTION_HEADER] = {"section-headers",       no_argument, 0, 'S'},
  [SECTIONS]       = {"sections",              no_argument, 0, 'S'},
  [SECTION_GROUPS] = {"section-groups",        no_argument, 0, 'g'},
  [SECTION_DETAILS]= {"section-details",       no_argument, 0, 't'},
  [HEADERS]        = {"headers",               no_argument, 0, 'e'},
  [SYMBOLS]        = {"symbols",               no_argument, 0, 's'},
  [SYMS]           = {"syms",                  no_argument, 0, 's'},
  [DYN_SYMS]       = {"dyn-syms",              no_argument, 0,   0},
  [NOTES]          = {"notes",                 no_argument, 0, 'n'},
  [RELOCS]         = {"relocs",                no_argument, 0, 'r'},
  [UNWIND]         = {"unwind",                no_argument, 0, 'u'},
  [DYNAMIC]        = {"dynamic",               no_argument, 0, 'd'},
  [VERSION_INFO]   = {"version-info",          no_argument, 0, 'V'},
  [ARCH_SPECIFIC]  = {"arch-specific",         no_argument, 0, 'A'},
  [ARCHIVE_INDEX]  = {"archive-index",         no_argument, 0, 'c'},
  [USE_DYNAMIC]    = {"use-dynamic",           no_argument, 0, 'D'},
  [HEX_DUMP]       = {"hex-dump",        required_argument, 0, 'x'},
  [STRING_DUMP]    = {"string-dump",     required_argument, 0, 'p'},
  [RELOCATED_DUMP] = {"relocated-dump",  required_argument, 0, 'R'},
  [DEBUG_DUMP]     = {"debug-dump",      required_argument, 0, 'w'},
  [DWARF_DEPTH]    = {"dwarf-depth",     required_argument, 0,   0},
  [DWARF_START]    = {"dwarf-start",     required_argument, 0,   0},
  [HISTOGRAM]      = {"histogram",             no_argument, 0, 'I'},
  [WIDE]           = {"wide",                  no_argument, 0, 'W'},
  [HELP]           = {"help",                  no_argument, 0, 'H'},
  [VERSION]        = {"version",               no_argument, 0, 'v'},
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

void printProgramHeader(libelfxx::ElfImage *image) {
  printf("ELF Header:\n");
  printf("  Magic:   \n");

}

int main(int argc, char * const *argv) {
  int c;
  while (1) {
    int option_index = 0;
    c = getopt_long(argc, argv, "afhSgtesnrudVAcDx:p:R:w:IWHv",
                    options, &option_index);
    if (c == -1) {
      break;
    }
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
    if (optionFlags[PROGRAM_HEADER]) {
      printProgramHeader(image);
    }
  }

  return 0;
}
