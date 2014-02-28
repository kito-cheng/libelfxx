#include <ElfNote.h>
#include <sstream>

namespace libelfxx {

#ifndef GNU_ABI_TAG_LINUX
#define GNU_ABI_TAG_LINUX       0
#define GNU_ABI_TAG_HURD        1
#define GNU_ABI_TAG_SOLARIS     2
#define GNU_ABI_TAG_FREEBSD     3
#define GNU_ABI_TAG_NETBSD      4
#endif

template <class T>
T align_power(T addr, T align) {
  return ((addr) + (1 << align) - 1) & static_cast<T>(-1) << (align);
}

template <class Elf_Shdr,
          class Elf_Nhdr>
static void
read(Elf_Shdr *noteshr,
     uint8_t *rawData,
     std::string &note) {
  const Elf_Nhdr *nhdr = reinterpret_cast<const Elf_Nhdr *>(
                     rawData + noteshr->sh_offset);
  uintptr_t end = reinterpret_cast<uintptr_t>(
                    rawData + noteshr->sh_offset + noteshr->sh_size);
  while (reinterpret_cast<uintptr_t>(nhdr) < end) {
    const char *name = reinterpret_cast<const char*>(nhdr) + sizeof(Elf_Nhdr);
    // XXX: align_power is 3 in readelf but it will display wrong info?
    const uint8_t *desc = reinterpret_cast<const uint8_t *>(name +
                      align_power<size_t>(nhdr->n_namesz, 2));
    switch (nhdr->n_type) {
      case NT_GNU_ABI_TAG:
        {
          std::ostringstream oss;
          uint32_t os, major, minor, subminor;
          const char *osname = nullptr;
          os = *reinterpret_cast<const uint32_t*>(desc);
          major = *reinterpret_cast<const uint32_t*>(desc + 4);
          minor = *reinterpret_cast<const uint32_t*>(desc + 8);
          subminor = *reinterpret_cast<const uint32_t*>(desc + 12);
          switch (os) {
            case GNU_ABI_TAG_LINUX:   osname = "Linux"; break;
            case GNU_ABI_TAG_HURD:    osname = "Hurd"; break;
            case GNU_ABI_TAG_SOLARIS: osname = "Solaris"; break;
            case GNU_ABI_TAG_FREEBSD: osname = "FreeBSD"; break;
            case GNU_ABI_TAG_NETBSD:  osname = "NetBSD"; break;
            default:                  osname = "Unknown"; break;
          }
          oss << "GNU ABI TAG " << osname << " "
              << major << "." << minor << "." << subminor;
          note = oss.str();
        }
        break;
      case NT_GNU_HWCAP:
        note = "GNU HWCAP";
        break;
      case NT_GNU_BUILD_ID:
        {
          std::ostringstream oss;
          oss << "GNU BUILD ID ";
          for (size_t i=0;i<nhdr->n_descsz;++i) {
            char buf[32];
            sprintf (buf, "%02x", desc[i] & 0xff);
            oss << buf;
          }
          note = oss.str();
        }
        break;
      case NT_GNU_GOLD_VERSION:
        note = "GNU GOLD VERSION";
        break;
      default:
        {
          std::ostringstream oss;
          oss << "UNKNOWN NOTE " << nhdr->n_type;
          note = oss.str();
        }
        break;
    }
    nhdr = reinterpret_cast<const Elf_Nhdr *>(
             desc + align_power<size_t>(nhdr->n_descsz, 3));
    break;
  }
}

ElfNote::ElfNote(const std::string &name,
                 Elf32_Shdr *shdr,
                 uint8_t *rawData)
  : ElfSection(name, shdr, rawData)
{
  read<Elf32_Shdr, Elf32_Nhdr>(shdr, rawData, _note);
}

ElfNote::ElfNote(const std::string &name,
                 Elf64_Shdr *shdr,
                 uint8_t *rawData)
  : ElfSection(name, shdr, rawData)
{
  read<Elf64_Shdr, Elf64_Nhdr>(shdr, rawData, _note);
}

ElfNote::~ElfNote() {
}

const std::string &ElfNote::note() const {
  return _note;
}

};
