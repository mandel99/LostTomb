#include "file_list.h"
#include "standalone_data.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define BASE 0x30000000u
static unsigned int files,index,allocation,checks,event_count;
static uint32_t events[4096][2];
#define REQUIRE(x) do{++checks;if(!(x)){fprintf(stderr,"File list failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
static void event(uint32_t kind,uint32_t value) {REQUIRE(event_count<4096);events[event_count][0]=kind;events[event_count++][1]=value;}
static void name(WIN32_FIND_DATAA *entry) {memset(entry,0,sizeof(*entry));sprintf_s(entry->cFileName,MAX_PATH,"Family%03u.dat",files-index-1);}
HANDLE restore_file_find_first(LPCSTR pattern,WIN32_FIND_DATAA *entry)
{REQUIRE(!strcmp(pattern,"Save\\*.dat"));index=0;event(1,files);if(!files)return INVALID_HANDLE_VALUE;name(entry);return (HANDLE)0x1234;}
BOOL restore_file_find_next(HANDLE handle,WIN32_FIND_DATAA *entry)
{REQUIRE(handle==(HANDLE)0x1234);++index;event(2,index);if(index>=files)return FALSE;name(entry);return TRUE;}
BOOL restore_file_find_close(HANDLE handle) {REQUIRE(handle==(HANDLE)0x1234);event(3,0x1234);return TRUE;}
void *FUN_00561a1d(unsigned int bytes)
{void *result=(void *)(uintptr_t)(BASE+0x40000+allocation);event(4,bytes);allocation=(allocation+bytes+63)&~63u;REQUIRE(allocation<0x10000);return result;}
void FUN_00561a2b(void *pointer)
{
    unsigned int i;
    for(i=0;i<event_count;++i)REQUIRE(events[i][0]!=5||events[i][1]!=(uint32_t)(uintptr_t)pointer);
    event(5,(uint32_t)(uintptr_t)pointer);
}
static int compare(const char *left,const char *right) {return _stricmp(left,right);}
static unsigned int run(const uint32_t p[6])
{
    RestoreFileList *list=(RestoreFileList *)BASE;unsigned int i,result=0;
    memset((void *)BASE,0xcc,0x50000);files=p[0];index=0;allocation=0;event_count=0;
    list->legacy_vtable=0x56f548;list->offsets=p[1]?(uint32_t *)(BASE+0x1000):0;list->count=p[5];list->capacity=p[1];
    list->strings=p[2]?(char *)(BASE+0x5000):0;list->string_capacity=p[2];list->used=p[5]*6;
    for(i=0;i<p[5];++i){list->offsets[i]=i*6;sprintf_s(list->strings+i*6,6,"old%02u",i);}
    *(char **)0x7027dcu=0;
    *(int32_t *)0x704db0u=0;
    if(p[4]==1) {int bytes,count;result=FUN_0041c8f0(list,"Save\\*.dat",&bytes,&count);event(7,bytes);event(8,count);}
    else {result=FUN_0041c740(list,"Save\\*.dat",(char)p[3]);if(p[4]==2){FUN_0041ca10(list,compare);result=0;}}
    return result;
}
static void output(unsigned int result)
{
    static const uint32_t regions[][2]={{BASE,28},{BASE+0x1000,2048},{BASE+0x5000,16384},{BASE+0x40000,8192},{0x7027dc,4}};
    unsigned int i,j;printf("{\"result\":%u,\"bytes\":\"",result);
    for(i=0;i<5;++i)for(j=0;j<regions[i][1];++j)printf("%02x",*(uint8_t *)(uintptr_t)(regions[i][0]+j));
    printf("\",\"events\":[");for(i=0;i<event_count;++i)printf("%s[%u,%u]",i?",":"",events[i][0],events[i][1]);
    puts("]}");fflush(stdout);
}
int main(int argc,char **argv)
{
    uint32_t p[6]={260,0,0,0,0,0};unsigned int i;
    REQUIRE(restore_data_open());REQUIRE(VirtualAlloc((void *)BASE,0x50000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")) {
        puts("{\"fields\":6}");fflush(stdout);
        for(;;){for(i=0;i<6;++i)if(scanf("%u",p+i)!=1)goto done;output(run(p));}
    } else {
        REQUIRE(run(p)==1);REQUIRE(((RestoreFileList *)BASE)->count==260);
        p[4]=2;run(p);REQUIRE(!strcmp(FUN_0041e010((RestoreFileList *)BASE,0),"Family000.dat"));
        REQUIRE(!strcmp(FUN_0041e010((RestoreFileList *)BASE,259),"Family259.dat"));
        REQUIRE(FUN_0041e010((RestoreFileList *)BASE,-1)==0&&FUN_0041e010((RestoreFileList *)BASE,260)==0);
        p[0]=0;p[4]=0;REQUIRE(run(p)==0&&((RestoreFileList *)BASE)->count==0);
        p[0]=260;p[1]=1;p[2]=10000;REQUIRE(run(p)==1&&((RestoreFileList *)BASE)->count==260);
        printf("File list: %u checks; explicit receivers, growth beyond 255 entries, sorting and empty enumeration.\n",checks);
    }
done:VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;
}
