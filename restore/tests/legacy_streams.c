#include "legacy_streams.h"
#include "host_allocator.h"
#include "standalone_data.h"
#include <stdlib.h>
#include <string.h>
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"Legacy stream failure at line %d\n",__LINE__);ExitProcess(1);}}while(0)
extern restore_legacy_FILE *FUN_00561a0a(const char *,const char *);
extern uint32_t FUN_00561994(restore_legacy_FILE *);
extern uint32_t FUN_00561ca1(void *,uint32_t,uint32_t,restore_legacy_FILE *);
extern int FUN_00561ea4(restore_legacy_FILE *,int,int);
extern int FUN_0056b1b6(int);
extern int FUN_0056b22e(restore_legacy_FILE *);
extern unsigned int FUN_004268c0(unsigned char *);
static void oracle(void)
{
    unsigned int count,free_index,seed,i;unsigned char *memory;
    restore_legacy_FILE **table=(restore_legacy_FILE **)(uintptr_t)0x30000000u;
    restore_legacy_FILE *records=(restore_legacy_FILE *)(uintptr_t)0x30001000u,*selected;
    REQUIRE(restore_data_open());memory=VirtualAlloc(table,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);REQUIRE(memory==(unsigned char *)table);
    puts("{\"table\":805306368,\"records\":805310464,\"record_bytes\":16384}");fflush(stdout);
    while(scanf("%u %u %u",&count,&free_index,&seed)==3) {
        REQUIRE((int32_t)count<=512);
        for(i=0;i<16384;++i)((unsigned char *)records)[i]=(unsigned char)(i*13u+seed);
        for(i=0;i<512;++i) {table[i]=records+i;records[i]._flag=(i&1)?0x83:1;}
        if(free_index<512)records[free_index]._flag=0x410;
        U(0x1266a90)=(uint32_t)(uintptr_t)table;U(0x1267aa0)=count;
        selected=FUN_00564a39();printf("{\"selected\":%u,\"after\":\"",(unsigned int)(uintptr_t)selected);
        for(i=0;i<16384;++i)printf("%02x",((unsigned char *)records)[i]);puts("\"}");fflush(stdout);
    }
    U(0x1266a90)=0;U(0x1267aa0)=0;VirtualFree(memory,0,MEM_RELEASE);restore_data_close();
}
int main(int argc,char **argv)
{
    char directory[MAX_PATH],file[MAX_PATH];
    unsigned char input[8193],output[8193];
    restore_legacy_FILE *streams[300],*stream;
    FILE *fixture;
    unsigned int i,j;
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")) {oracle();return 0;}
    REQUIRE(GetTempPathA(sizeof(directory),directory));REQUIRE(GetTempFileNameA(directory,"rst",0,file));
    fixture=fopen(file,"wb");REQUIRE(fixture);
    for(i=0;i<sizeof(input);++i)input[i]=(unsigned char)(i*37u);
    REQUIRE(fwrite(input,1,sizeof(input),fixture)==sizeof(input));REQUIRE(!fclose(fixture));
    REQUIRE(restore_data_open());REQUIRE(restore_legacy_streams_open());
    REQUIRE(U(0x1267aa0)==512&&U(0x1266a90)>255);
    for(i=0;i<300;++i) {
        stream=FUN_00561a0a(file,"rb");REQUIRE(stream);streams[i]=stream;
        for(j=0;j<i;++j)REQUIRE(streams[j]!=stream);
        REQUIRE(FUN_0056b1b6(FUN_0056b22e(stream))==sizeof(input));
        memset(output,0,sizeof(output));
        REQUIRE(FUN_00561ca1(output,1,sizeof(output),stream)==sizeof(input));
        REQUIRE(!memcmp(input,output,sizeof(input)));
        REQUIRE(!FUN_00561ea4(stream,4095,SEEK_SET));
        REQUIRE(FUN_00561ca1(output,1,23,stream)==23&&!memcmp(input+4095,output,23));
    }
    for(i=0;i<300;++i)REQUIRE(FUN_00561994(streams[i])==0);
    stream=FUN_00561a0a(file,"rb");REQUIRE(stream==streams[0]);REQUIRE(FUN_00561994(stream)==0);
    REQUIRE(FUN_004ce9e0()==IDCANCEL); /* No dialog, text selection or retry loop. */
    {unsigned char drive[4]={0x44,0x55,0x66,0x77};REQUIRE(FUN_004268c0(drive)==1);REQUIRE(!drive[0]&&drive[1]==0x55&&drive[2]==0x66&&drive[3]==0x77);}
    restore_legacy_streams_close();REQUIRE(U(0x1266a90)==0&&U(0x1267aa0)==0);
    REQUIRE(restore_legacy_streams_open());stream=FUN_00561a0a(file,"rb");REQUIRE(stream);REQUIRE(FUN_00561994(stream)==0);
    restore_legacy_streams_close();restore_data_close();REQUIRE(DeleteFileA(file));
    puts("Legacy streams: 300 simultaneous binary files, complete pointers/counts, exact bytes, seeking, close/reuse and bootstrap cleanup verified; CD retry returns Cancel without UI.");
    return 0;
}
