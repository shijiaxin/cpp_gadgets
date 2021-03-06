#include <unistd.h> //fread,fwrite,fopen
#include <fcntl.h>  //fread,fwrite,fopen
#include <stdio.h>  //printf
#include <dirent.h> //dirent

#define  uint64_t unsigned long long 
#define item_type int
//#define  uint64_t int
typedef struct element{
	item_type src;
	item_type dst;
	double value;
	void set(item_type a, item_type b,double c){
		src=a;
		dst=b;
		value=c;
	}
} element;
void read_dirent(){
        DIR* dir;
        dirent* pdir;
        dir = opendir(".");
		printf("start reading dirent ... ...\n");
        while (pdir = readdir(dir)) {
			printf("%s\n",pdir->d_name);
        }
		printf("end reading dirent ... ...\n");
}

/*
binary format:
[ 0, 7] : "_HEADER_"
[ 8,15] : size of each element
[16,23] : number of elements
[24,..] : elements
*/
void write_header(FILE * fp,uint64_t size,uint64_t n){
	char header[8+8*2]="_HEADER_";
	*((uint64_t *)(header+8))=size;
	*((uint64_t *)(header+8+8))=n;
	fwrite(header,sizeof(header[0]),sizeof(header)/sizeof(header[0]),fp);
}

void write_binary(){
    FILE *fp=fopen("output.bin.out","wb");
	write_header(fp,2*sizeof(item_type)+sizeof(double),3);	
    element array [3];
    array[0].set(1,2,1.0);
    array[1].set(3,4,2.0);
    array[2].set(5,6,3.0);
    fwrite(array,sizeof(array[0]),sizeof(array)/sizeof(array[0]),fp);
    fclose(fp);
}
void read_binary(){
    FILE *fp=fopen("output.bin.out","rb");
    char header[8+8*2];
    int elem_size;
    int elem_num;
    fread(header,sizeof(header[0]),sizeof(header)/sizeof(header[0]),fp);
    elem_size=*((uint64_t *)(header+8));
    elem_num=*((uint64_t *)(header+16));
    header[8]=0;
    printf("%s\n",header);
    printf("size=%d,num=%d\n",elem_size,elem_num);
    element tmp;
    for(int i=0;i<elem_num;i++){
        fread(&tmp,sizeof(element),1,fp);
        printf("%d,%d,%f\n",tmp.src,tmp.dst,tmp.value);
    }
    fclose(fp);    
}
void overwrite_binary(){
	write_binary();
	FILE *fp=fopen("output.bin.out","r+b"); // "wb" mode  will clear old data
	write_header(fp,2*sizeof(item_type)+sizeof(double),2);
	fclose(fp);
	read_binary();
}

int main(){
    write_binary();
    read_binary();
	//read_dirent();
	overwrite_binary();
}
