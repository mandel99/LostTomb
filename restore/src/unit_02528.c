#include "sprite_libraries.h"
#include "host_allocator.h"
#include <stdio.h>
#include <string.h>
extern void FUN_0053f790(int *,char);
extern int FUN_004cea30(char *,int);
extern int FUN_004ceb60(const char *,void *,int);
extern void FUN_004cdf20(void *,int);
extern void FUN_004cdeb0(void *,int);
uint32_t FUN_0053f660(const char *name,uint8_t **pixels,uint8_t convert,uint32_t bytes,uint8_t compressed,int destination_offset,uint32_t file_offset)
{
    char path[260];
    if(snprintf(path,sizeof(path),"data\\%s.555",name)<0||strlen(name)>sizeof(path)-10)return 0;
    FUN_0053f790((int *)pixels,0);
    if(!bytes)bytes=(uint32_t)FUN_004cea30(path,(int)file_offset);
    if((int32_t)bytes>0) {
        *pixels=restore_heap_allocate(bytes);
        if(!*pixels)return 0;
        if(FUN_004ceb60(path,*pixels+destination_offset,(int)file_offset)==1&&convert) {
            if(compressed)FUN_004cdeb0(*pixels,(int)bytes);else FUN_004cdf20(*pixels,(int)bytes);
        }
    }
    return bytes;
}
