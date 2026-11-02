#include "chunk_io.h"
#include "standalone_data.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BASE 0x30000000u
#define OUTPUT ((uint8_t *)(BASE+0x20000))
#define INPUT ((uint8_t *)(BASE+0x40000))
#define HEAP ((uint8_t *)(BASE+0x60000))
#define U(a) (*(uint32_t *)(uintptr_t)(a))
static uint32_t events[2048][5],count,position,fail_alloc;
static char hex[200001];
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"Chunk IO failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
static void event(uint32_t a,uint32_t b,uint32_t c,uint32_t d){REQUIRE(count<2048);events[count][0]=a;events[count][1]=b;events[count][2]=c;events[count][3]=d;events[count][4]=U(0xe945c0);++count;}
int FUN_005660ed(int file,void *out,uint32_t n){REQUIRE(file==37&&position+n<=100000);event(0x5660ed,file,(uint32_t)out,n);memcpy(out,INPUT+position,n);position+=n;return n;}
uint32_t FUN_0055bcf0(int file,void *out,uint32_t n){REQUIRE(file==37&&position+n<=100000);event(0x55bcf0,file,(uint32_t)out,n);memcpy(out,INPUT+position,n);position+=n;return 1;}
void *FUN_00561a1d(uint32_t n){event(0x561a1d,n,0,0);return fail_alloc?0:HEAP;}
void FUN_00561a2b(void *p){REQUIRE(p==HEAP);event(0x561a2b,(uint32_t)p,0,0);}
void *FUN_00562880(void *out,const void *in,uint32_t n){event(0x562880,(uint32_t)out,(uint32_t)in,n);return memmove(out,in,n);}
void FUN_004cc110(int g,int i){event(0x4cc110,g,i,0);U(0xe20768)=BASE+0x100;}
void FUN_004cc160(int g,int i){event(0x4cc160,g,i,0);U(0xe20764)=BASE+0x200;}
int FUN_00538cd0(const void *a,const void *b,int flags){event(0x538cd0,(uint32_t)a,(uint32_t)b,flags);return 1;}
/* op,index,current_size,current_count,stored_size,stored_count,codec,
   stored_codec,cutoff,version,declared_count,fail_or_old,seed,migration_id */
static uint32_t run(const uint32_t p[14])
{
    uint32_t i;RestoreChunkDescriptor *descriptors=(RestoreChunkDescriptor *)(BASE+0x1000),*saved=(RestoreChunkDescriptor *)0xe92e50u;RestoreChunkMigration *migration=(RestoreChunkMigration *)(BASE+0x8000);
    REQUIRE(p[0]<3&&p[1]<300&&p[2]*p[3]<=50000&&p[4]*p[5]<=50000);position=count=0;fail_alloc=p[11];
    memset(descriptors,0,6000);memset(saved,0,6000);memset(OUTPUT,0xa5,100000);memset(HEAP,0xcc,100000);memset(migration,0,32);
    for(i=0;i<100000;++i)INPUT[i]=(uint8_t)(p[12]+i*29+i/251);
    U(0xe945c0)=p[9];U(0xe945c4)=p[10];U(0xe20764)=0x11111111;U(0xe20768)=0x22222222;
    descriptors[p[1]].codec=p[6];descriptors[p[1]].destination=(uint32_t)OUTPUT;descriptors[p[1]].size=p[2];descriptors[p[1]].count=p[3];
    saved[p[1]].codec=p[7];saved[p[1]].size=p[4];saved[p[1]].count=p[5];
    if(p[8]){descriptors[p[1]].migration=(uint32_t)migration;migration->size=p[4];migration->count=p[5];migration->cutoff=p[8];migration->callback=p[13]==1?0x4d0d40:0x4d0c60;}
    if(p[0]){memcpy(OUTPUT,INPUT,20000);if(p[0]==1)FUN_004d0d40(OUTPUT,p[11]);else FUN_004d0c60(OUTPUT,p[11]);return 0;}
    return FUN_004d0e10(p[1],descriptors,37);
}
static void output(uint32_t result)
{
    uint32_t i,j;static const char digits[]="0123456789abcdef";
    for(i=0;i<50000;++i){hex[i*2]=digits[OUTPUT[i]>>4];hex[i*2+1]=digits[OUTPUT[i]&15];}hex[100000]=0;
    printf("{\"result\":%u,\"position\":%u,\"labels\":[%u,%u],\"events\":[",result,position,U(0xe20764),U(0xe20768));
    for(i=0;i<count;++i){printf("%s[",i?",":"");for(j=0;j<5;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}printf("],\"output\":\"%s\"}\n",hex);fflush(stdout);
}
int main(int argc,char **argv)
{
    uint32_t p[14]={0,14,1592,1,1592,1,0,0,0,147,175,0,7,0},i,result;
    SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX|SEM_NOOPENFILEERRORBOX);
    _set_abort_behavior(0,_WRITE_ABORT_MSG|_CALL_REPORTFAULT);
    REQUIRE(restore_data_open());REQUIRE(VirtualAlloc((void *)BASE,0x100000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){puts("{\"fields\":14}");fflush(stdout);for(;;){for(i=0;i<14;++i)if(scanf("%u",p+i)!=1)goto done;result=run(p);output(result);}}
    else{REQUIRE(run(p)==1&&position==1592&&OUTPUT[1592]==0xa5);p[9]=114;REQUIRE(run(p)==0);p[0]=1;p[11]=1;run(p);REQUIRE(*(uint32_t *)(OUTPUT+4)==*(uint16_t *)(INPUT+6));puts("Chunk IO: native schemas, full-width versions/counts, resizing, codec lengths and migration contracts.");}
done:VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;
}
