#include "elf.h"
#include "utilities.h"
#include <elf.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static int check_hdr(Elf32_Ehdr *hdr)
{
    if(hdr->e_ident[EI_MAG0] != ELFMAG0){
        return 0;
    }
    if(hdr->e_ident[EI_MAG1] != ELFMAG1){
        return 0;
    }
    if(hdr->e_ident[EI_MAG2] != ELFMAG2){
        return 0;
    }
    if(hdr->e_ident[EI_MAG3] != ELFMAG3){
        return 0;
    }
    return 1;
}

static int check_supported(Elf32_Ehdr *hdr)
{
    if(!check_hdr(hdr)){
        return 0;
    }
    if(hdr->e_ident[EI_CLASS] != ELFCLASS32){
        return 0;
    }
    if(hdr->e_ident[EI_DATA] != ELFDATA2LSB){
        return 0;
    }
    if(hdr->e_machine != EM_ARM){
        return 0;
    }
    if(hdr->e_type != ET_EXEC && hdr->e_type != ET_DYN){
        return 0;
    }
    return 1;
}

Elf *open_elf(int elf_fd)
{
    Elf *elf;
    Elf32_Ehdr *ehdr;
    Elf32_Shdr *section_table;
    char *str_table;

    ehdr = malloc(sizeof(Elf32_Ehdr));
    read(elf_fd, ehdr, sizeof(Elf32_Ehdr));    
    if(!check_supported(ehdr)){
        free(ehdr);
        return NULL;
    }
    //load section header table
    section_table = malloc(ehdr->e_shnum * ehdr->e_shentsize);
    lseek(elf_fd, ehdr->e_shoff, SEEK_SET);
    read(elf_fd, section_table, ehdr->e_shnum * ehdr->e_shentsize);
    //load section header string table
    if(ehdr->e_shstrndx == SHN_UNDEF){
        free(ehdr);
        free(section_table);
        return NULL;
    }
    Elf32_Shdr *shdr = section_table + ehdr->e_shstrndx;
    str_table = malloc(shdr->sh_size);
    lseek(elf_fd, shdr->sh_offset, SEEK_SET);
    read(elf_fd, str_table, shdr->sh_size);

    //allocate and populate ELF struct and return a reference to it.
    elf = malloc(sizeof(Elf));
    elf->fd = elf_fd;
    elf->elf_hdr = ehdr;
    elf->shtab = section_table;
    elf->shstrtab = str_table; 
    return elf;
}

void close_elf(Elf *elf)
{
    close(elf->fd);
    free(elf->elf_hdr);
    free(elf->shtab);
    free(elf->shstrtab);
    free(elf);
}

int find_dynsym(Elf *elf, char *sym_name)
{
    Elf32_Sym *dynsym_table;
    Elf32_Sym *victim;
    Elf32_Shdr *dynsym_table_hdr;
    Elf32_Shdr *dynstr_table_hdr;
    char *dynstr_table;
    int i;
    int flag;

    i = 1;
    flag = 0;
    dynsym_table_hdr = &elf->shtab[1];
    while(i < elf->elf_hdr->e_shnum){
        if(dynsym_table_hdr->sh_name != SHN_UNDEF && !strcmp(elf->shstrtab + dynsym_table_hdr->sh_name,  ".dynsym")){
            flag = 1;
            break;
        }
        i++;
        dynsym_table_hdr++;
    }
    if(!flag){
        return 0;
    }
    //load dynamic symbol table
    dynsym_table = malloc(dynsym_table_hdr->sh_size);
    lseek(elf->fd, dynsym_table_hdr->sh_offset, SEEK_SET);
    read(elf->fd, dynsym_table, dynsym_table_hdr->sh_size);
    //load dynamic symbol string table
    dynstr_table_hdr = elf->shtab + dynsym_table_hdr->sh_link;
    dynstr_table = malloc(dynstr_table_hdr->sh_size);
    lseek(elf->fd, dynstr_table_hdr->sh_offset, SEEK_SET);
    read(elf->fd, dynstr_table, dynstr_table_hdr->sh_size); 

    //search for the supplied symbol name in the dynamic symbol
    i = 1;
    flag = 0;
    victim = &dynsym_table[1];
    while(i < dynsym_table_hdr->sh_size / dynsym_table_hdr->sh_entsize){
        if(victim->st_name != SHN_UNDEF && !strcmp(dynstr_table + victim->st_name, sym_name)){
            flag = 1;
            break;
        }
        i++;
        victim++;
    }
    if(!flag){
        free(dynsym_table);
        free(dynstr_table);
        return 0;
    }
    free(dynsym_table);
    free(dynstr_table);
    return 1;
}
