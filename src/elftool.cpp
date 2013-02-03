#include <stdio.h>
#include <ElfImage.h>
#include <Debug.h>

static void usage() {
  DEBUG("usage\n");
}

int main(int argc, const char *argv[]) {
  DEBUG("argc=%d\n", argc);
  if (argc < 2) {
    usage();
  } else {
    libelfxx::ElfImage *image = libelfxx::ElfImage::create(argv[1]);
  }
  return 0;
}
