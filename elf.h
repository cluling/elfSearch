#ifndef ELF_H
#define ELF_H
//#include <stdint.h>
#include <elf.h>

typedef struct Elf{
    int fd;
    struct Elf32_Ehdr *elf_hdr;
    struct Elf32_Shdr *shtab;
    const char *shstrtab;
}Elf;

Elf *open_elf(int fd);
void close_elf(Elf *efile);
int find_dynsym(Elf *efile, char *sym_name);
#endif
