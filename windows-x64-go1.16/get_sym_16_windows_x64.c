#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
FILE *fp;
size_t gopclntab_offset;
size_t gopclntab_size;

int getfilesize(const char *filename){
    int fd,filesize;
    fd = open(filename,O_RDONLY);
    filesize = lseek(fd,SEEK_SET,SEEK_END);
    close(fd);
    return filesize;
}

//int StringFind(char *pSrc,int S_len,const char *pDst,int D_len)
//{
//	int i,ret;
//	for(i=0;i<S_len;i++)
//	{
//		ret = strncmp(pSrc+i,pDst,D_len);
//		if(ret==0)
//		{
//			return i;
//		}
//	}
//	return -1;
//}
int FindPclntab(char *pSrc,int S_len)
{
	int i,ret;
	for(i=0;i<S_len/8;i++)
	{
		if (*(size_t *)(pSrc+i*8) == 0x08010000fffffffa){
			return i*8;
		}
//		printf("%p\n",*(size_t *)(pSrc+i*8));
	}
	return -1;
}


int main(int argc, char const *argv[])
{
	int file_size;
	if (argc < 2)
	{
		printf("invalid arguments\n");
		printf("usage: ./tools [filename]\n");
		exit(0);
	}
    fp = fopen( argv[1], "rb+" );
    if(fp==NULL)
    {
        puts("open file error!");
        exit(0);
    }
    file_size = getfilesize(argv[1]);
    printf("file size: %d\n",file_size);
    char *file_data = malloc(file_size);
    size_t read_sign;
    read_sign = fread(file_data,1,getfilesize(argv[1]),fp);
    printf("read_sign: %d\n",read_sign);
    gopclntab_offset = FindPclntab(file_data,file_size);
    printf("pclntab offset:%p\n",gopclntab_offset);
    // printf("%d\n",gopclntab_size);
    size_t *data = (size_t *)(file_data+gopclntab_offset);
    size_t func_num = data[1];
    size_t filepath_num = data[2];
    size_t func_name_offset = data[3];
    size_t func_struct_offset = data[7];
    printf("function number: %x\n",func_num);
    printf("filepath_num: %x\n",filepath_num);
    printf("func_name_offset: %x\n",func_name_offset);
    printf("func_struct_offset: %x\n",func_struct_offset);
//	exit(0);
    size_t *func_struct_point = (char *)data+func_struct_offset;
    int index;
    //printf("%p,%p,%p",&data,(char*)data+1,(char*)data+2);
    FILE *data_p;
    char data_file[] = "data.py";
    data_p = fopen(data_file, "w+");
    fputs("#coding:utf-8\n",data_p);
    fputs("data = {",data_p);
    for(index=0;index<func_num;index++)
    {
        int *tmp = (char *)data+func_struct_offset+func_struct_point[index*2+1];
        fprintf(data_p,"\"%x\":\"%s\",\t",func_struct_point[index*2],(char *)data + func_name_offset +tmp[2]);
    }
    fputs("}",data_p);
	return 0;
}
