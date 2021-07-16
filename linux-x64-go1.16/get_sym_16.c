#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<elf.h>

FILE *fp;
size_t gopclntab_offset;
size_t gopclntab_size;

int getfilesize(char *filename){
    int fd,filesize;
    fd = open(filename,O_RDONLY);
    filesize = lseek(fd,SEEK_SET,SEEK_END);
    close(fd);
    return filesize;
}

int main(int argc, char const *argv[])
{
    if (argc < 2)
	{
		printf("invalid arguments\n");
		printf("usage: ./get_sym [filename]\n");
		exit(0);
	}
    fp = fopen( argv[1], "rb+" );
    if(fp==NULL)
    {
        puts("open file error!");
        exit(0);
    }
    // printf("%d",getfilesize(filename));
    
    //read elf header
    Elf64_Ehdr *elf_head=malloc(sizeof(Elf64_Ehdr));
    size_t read_sign;
    read_sign = fread(elf_head,1,sizeof(Elf64_Ehdr),fp);
    // printf("%d,%d",read_sign,sizeof(Elf64_Ehdr));
    //check header
    if(elf_head->e_ident[0]!= 0x7f || elf_head->e_ident[1]!='E' || elf_head->e_ident[2]!='L' || elf_head->e_ident[3]!='F' )
    {
        puts("file format error!");
        exit(0);
    }
    
    //analysis section
    Elf64_Shdr *shdr = malloc(sizeof(Elf64_Shdr)*elf_head->e_shnum);
    read_sign = fseek(fp, elf_head->e_shoff, SEEK_SET); 
    read_sign = fread(shdr,1,sizeof(Elf64_Shdr)* elf_head->e_shnum,fp);
    rewind(fp);
    fseek(fp, shdr[elf_head->e_shstrndx].sh_offset, SEEK_SET);
    char shstrtab[shdr[elf_head->e_shstrndx].sh_size];
	char *temp = shstrtab;
    fread(shstrtab, shdr[elf_head->e_shstrndx].sh_size, 1, fp);
    // printf("%d\n",elf_head->e_shnum);
    int i ;
    for (int i = 0; i < elf_head->e_shnum; i++)
	{
		temp = shstrtab;
		temp = temp + shdr[i].sh_name;
        if (strcmp(temp, ".gopclntab") == 0){
            // printf("%s\t\t", temp);
            // printf("%x\t\t", shdr[i].sh_offset);
            // printf("%x\t\t", shdr[i].sh_size);
            // printf("%x\n", shdr[i].sh_addr);
            // puts(".gopclntab found!");
            gopclntab_offset = (size_t)shdr[i].sh_offset;
            gopclntab_size = (size_t)shdr[i].sh_size;
            break;
        }
        // uint8_t *sign_data=(uint8_t*)malloc(sizeof(uint8_t)*shdr[i].sh_size);
		// 依据此段在文件中的偏移读取出
		// fseek(fp, shdr[i].sh_offset, SEEK_SET);
		// fread(sign_data, sizeof(uint8_t)*shdr[i].sh_size, 1, fp);
		// // 显示读取的内容
		// uint8_t *p = sign_data;
		// int j = 0;
		// for (j=0; j<shdr[i].sh_size; j++)
		// {
		//     printf("%x", *p);
        //     p++;
		// }
	 }
     if(gopclntab_offset==0)
     {
         puts(".gopclntab not found!");
         exit(0);
     }
    rewind(fp);
    
    size_t *data = malloc(gopclntab_size);
    read_sign = fseek(fp,gopclntab_offset,SEEK_SET);
    // printf("%d\n",gopclntab_size);
    read_sign =  fread(data,gopclntab_size,1,fp);
    size_t func_num = data[1];
    size_t filepath_num = data[2];
    size_t func_name_offset = data[3];
    size_t func_struct_offset = data[7];
    // printf("function number: %x\n",func_num);
    // printf("filepath_num: %x\n",filepath_num);
    // printf("func_name_offset: %x\n",func_name_offset);
    // printf("func_struct_offset: %x\n",func_struct_offset);

    size_t *func_struct_point = (char *)data+func_struct_offset;
    
    int index;
    // printf("%p,%p,%p",&data,(char*)data+1,(char*)data+2);
    FILE *data_p;
    char data_file[] = "data.py";
    data_p = fopen(data_file, "w+");
    fputs("#coding:utf-8\n",data_p);
    fputs("data = {",data_p);
    for(index=0;index<func_num;index++)
    {
        int *tmp = (char *)data+func_struct_offset+func_struct_point[index*2+1];
        fprintf(data_p,"\"sub_%x\":\"%s\",\t",func_struct_point[index*2],(char *)data + func_name_offset +tmp[2]);
    }
    fputs("}",data_p);

    /*

    //source path
    int path_offset =data[func_num*2+3];
    int path_num = data[data[func_num*2+3]/8];
    printf("%x,%x\n",path_offset,path_num);
    int *p = (char *)data+path_offset;
    for(index=0;index<path_num;index++)
    {
        printf("%d:%x, %s\n",index,p[index+1],(char*)data+p[index+1]);
    }
    */
    return 0;
    
}
