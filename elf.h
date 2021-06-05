#ifndef ELF_H
#define ELF_H
#include <elf.h>

typedef struct Elf{
    int fd;
    Elf32_Ehdr *elf_hdr;
    Elf32_Shdr *shtab;
    char *shstrtab;
}Elf;

Elf *open_elf(int fd);
void close_elf(Elf *efile);
int find_dynsym(Elf *efile, char *sym_name);
#endif
