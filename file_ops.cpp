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
void write_binary(){
    FILE *fp=fopen("output.bin.out","wb");
    char header[8+8*2]="_HEADER_";
    *((uint64_t *)(header+8))=2*sizeof(item_type)+sizeof(double); // size of element
    *((uint64_t *)(header+8+8))=3;//number of element
    fwrite(header,sizeof(header[0]),sizeof(header)/sizeof(header[0]),fp);
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
int main(){
    write_binary();
    read_binary();
	read_dirent();
}
