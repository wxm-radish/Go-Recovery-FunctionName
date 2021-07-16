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

int StringFind(char *pSrc,int S_len,const char *pDst,int D_len)
{
	int i,ret;
	for(i=0;i<S_len;i++)
	{
		ret = strncmp(pSrc+i,pDst,D_len);
		if(ret==0)
		{
			return i;
		}
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
    gopclntab_offset = StringFind(file_data,file_size,"\xfb\xff\xff\xff\x00\x00\x01\x08",8);
    printf("pclntab offset:%d\n",gopclntab_offset);
    size_t *data = (size_t *)(file_data+gopclntab_offset);
    size_t func_num = data[1];
    printf("function number: %x\n",func_num);
    int index;
    FILE *data_p;
    char data_file[] = "data.py";
    data_p = fopen(data_file, "w+");
    fputs("#coding:utf-8\n",data_p);
    fputs("data = {",data_p);

    for(index=0;index<func_num;index++)
    {
        
        fprintf(data_p,"\"%x\":\"%s\",\t",data[index*2+2],(char*)data+(data[data[index*2+3]/8+1]&0xffffffff));
    }
    fputs("}",data_p);
	return 0;
}
