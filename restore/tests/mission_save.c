#include "mission_loaders.h"
#include "mission_routes.h"
#include "mission_compat.h"
#include "audio_state.h"
#include "chunk_io.h"
#include "mission_schema.h"
/* Synthetic native-loader fixture: C-only save recovery is checked separately. */
void restore_mission_schema_recover_loaded(void){}
#include "compression.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BASE 0x30000000u
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define REQUIRE(x) do { if(!(x)) {fprintf(stderr,"Mission save failure at %d\n",__LINE__);ExitProcess(1);} } while(0)
static uint8_t *stream;
static uint32_t length,position,reads,bump=BASE+0x800000,allocation_count;
static struct {uint32_t address,size,live;} allocations[128];
static HCRYPTPROV provider;
static uint32_t random_state=1,random_calls;
uint32_t FUN_00562348(void){++random_calls;random_state=random_state*214013u+2531011u;return (random_state>>16)&32767u;}
static const uint32_t extra_regions[][2]={{0x580000,0x68000},{0x6fdcbc,80},{0x9763b8,8512},{0xbcba32,7240},{0xd05640,196},{0xea184c,0xbd00},{0xeaaf5c,4}};
int FUN_005660ed(uint32_t fd,void *out,uint32_t n)
{uint32_t count=length-position;REQUIRE(fd==37);++reads;if(count>n)count=n;if(count)memcpy(out,stream+position,count);position+=count;return count;}
void FUN_004ce6e0(const char *message,int first,int second)
{(void)message;(void)first;(void)second;REQUIRE(0);}
void *FUN_00561a1d(uint32_t n)
{
    uint32_t address=bump;REQUIRE(n<=0x200000&&allocation_count<128&&n+32<0x32800000u-bump);
    allocations[allocation_count].address=address;allocations[allocation_count].size=n;allocations[allocation_count++].live=1;
    memset((void *)(uintptr_t)address,0xcc,n+32);bump+=(n+32+63)&~63u;return (void *)(uintptr_t)address;
}
void FUN_00561a2b(void *p)
{uint32_t i;for(i=0;i<allocation_count;++i)if(allocations[i].address==(uint32_t)p&&allocations[i].live){allocations[i].live=0;return;}REQUIRE(0);}
void *FUN_00562880(void *out,const void *in,uint32_t n){return memmove(out,in,n);}
int FUN_00562370(const char *a,const char *b){return strcmp(a,b);}
void FUN_004cc110(int g,int i){(void)g;(void)i;REQUIRE(0);}
void FUN_004cc160(int g,int i){(void)g;(void)i;REQUIRE(0);}
int FUN_00538cd0(const void *a,const void *b,int flags){(void)a;(void)b;(void)flags;REQUIRE(0);return 0;}
/* Versions before 147 are outside this controller proof. Reaching one fails. */
#define LEGACY(id) void FUN_##id(void){REQUIRE(0);}
LEGACY(004d17e0) LEGACY(004d1840) LEGACY(004d1860) LEGACY(004d1880)
LEGACY(004d18b0) LEGACY(004d18f0) LEGACY(004d1ad0) LEGACY(004d2fa0)
LEGACY(004d3200) LEGACY(004bd5d0) LEGACY(004d32d0) LEGACY(004d32a0)
LEGACY(004d3c20) LEGACY(004d3cc0) LEGACY(004de140) LEGACY(004d3d80)
LEGACY(004d3da0) LEGACY(004d3f40)
static void digest(uint32_t address,uint32_t n,char hex[65])
{
    HCRYPTHASH hash;DWORD count=32;uint8_t bytes[32];uint32_t i;static const char digits[]="0123456789abcdef";
    REQUIRE(CryptCreateHash(provider,CALG_SHA_256,0,0,&hash));
    REQUIRE(CryptHashData(hash,(const BYTE *)(uintptr_t)address,n,0));
    REQUIRE(CryptGetHashParam(hash,HP_HASHVAL,bytes,&count,0)&&count==32);REQUIRE(CryptDestroyHash(hash));
    for(i=0;i<32;++i){hex[i*2]=digits[bytes[i]>>4];hex[i*2+1]=digits[bytes[i]&15];}hex[64]=0;
}
static void emit(void)
{
    RestoreChunkDescriptor *schema=(RestoreChunkDescriptor *)0x5e09a0u;char hex[65];uint32_t i,nonempty=0,points=0;
    for(i=0;i<50;++i){uint32_t n=U(0x7770e4u+i*324);if(n){++nonempty;points+=n;}}
    printf("{\"version\":%u,\"declared\":%u,\"position\":%u,\"reads\":%u,\"nonempty_routes\":%u,\"route_points\":%u,\"chunks\":[",U(0xe945c0),U(0xe945c4),position,reads,nonempty,points);
    for(i=0;i<181;++i) {uint32_t n=(uint32_t)schema[i].size*schema[i].count;digest(schema[i].destination,n,hex);printf("%s{\"index\":%u,\"length\":%u,\"sha256\":\"%s\"}",i?",":"",i,n,hex);}
    printf("],\"allocations\":[");
    for(i=0;i<allocation_count;++i) {digest(allocations[i].address,allocations[i].size+32,hex);printf("%s{\"address\":%u,\"size\":%u,\"live\":%u,\"sha256_with_guard\":\"%s\"}",i?",":"",allocations[i].address,allocations[i].size,allocations[i].live,hex);}
    printf("],\"random_calls\":%u,\"extra_hashes\":[",random_calls);
    for(i=0;i<sizeof(extra_regions)/sizeof(extra_regions[0]);++i){digest(extra_regions[i][0],extra_regions[i][1],hex);printf("%s\"%s\"",i?",":"",hex);}puts("]}");
}
static void synthetic(void)
{
    RestoreChunkDescriptor *schema=(RestoreChunkDescriptor *)0x5e09a0u;uint32_t i,n;
    stream=(uint8_t *)calloc(1,0x200000);REQUIRE(stream!=NULL);
    *(uint32_t *)(stream+4)=160;*(uint32_t *)(stream+8)=181;
    memcpy(stream+12,schema,181*20);length=6012;
    for(i=7;i<181;++i){n=(uint32_t)schema[i].size*schema[i].count;length+=schema[i].codec?4:n;REQUIRE(length<0x200000);}
}
/* seed, kind, row count, negative row (or -1), choices, sample index,
   sample count, flag, state. Isolated scratch records preserve output guards. */
static void state_case(const uint32_t p[9])
{
    uint32_t i,j,table=0x5be9b8u;char hex[65];
    const uint32_t object=BASE+0x1000,entries=BASE+0x2000,samples=BASE+0x3000;
    REQUIRE(p[2]<=3&&p[4]<=4&&p[6]<=4);
    for(i=0;i<64;++i)*(uint8_t *)(object+i)=(uint8_t)(p[0]+i*29);
    for(i=0;i<160;++i)*(uint8_t *)(entries+i)=(uint8_t)(p[0]+i*29);
    for(i=0;i<192;++i)*(uint8_t *)(samples+i)=(uint8_t)(p[0]+i*29);
    U(object)=p[1];
    if(p[1]<253)table+=p[1]*40;
    for(i=0;i<40;++i)*(uint8_t *)(table+i)=(uint8_t)(p[0]+i*29);
    U(table+16)=samples;U(table+20)=p[6];U(table+24)=entries;U(table+28)=p[2];
    for(i=0;i<4;++i){U(entries+i*40)=i==p[3]?0xffffffffu:i;
        for(j=0;j<4;++j){U(entries+i*40+8+j*8)=j<p[4]?p[5]+j:0xffffffffu;*(uint8_t *)(entries+i*40+12+j*8)=(uint8_t)(p[7]+j);}}
    random_state=p[0];random_calls=0;FUN_0041fca0((void *)object,(uint16_t)p[8]);
    digest(object,64,hex);printf("{\"random_calls\":%u,\"hashes\":[\"%s\"",random_calls,hex);
    digest(table,40,hex);printf(",\"%s\"",hex);digest(entries,160,hex);printf(",\"%s\"",hex);
    digest(samples,192,hex);printf(",\"%s\"]}\n",hex);fflush(stdout);
}
int main(int argc,char **argv)
{
    FILE *file;long bytes;
    SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX|SEM_NOOPENFILEERRORBOX);
    REQUIRE(restore_data_open());REQUIRE(VirtualAlloc((void *)BASE,0x02800000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);
    REQUIRE(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
    if(argc==2&&!strcmp(argv[1],"--state-batch")) {
        uint32_t p[9],i;puts("{\"fields\":9}");fflush(stdout);
        for(;;){for(i=0;i<9;++i)if(scanf("%u",p+i)!=1)goto done;state_case(p);}
    }
    if(argc==3&&!strcmp(argv[1],"--save")) {
        file=fopen(argv[2],"rb");REQUIRE(file!=NULL);REQUIRE(!fseek(file,0,SEEK_END));bytes=ftell(file);REQUIRE(bytes>=6012&&bytes<=0x2000000);
        length=(uint32_t)bytes;stream=(uint8_t *)malloc(length);REQUIRE(stream!=NULL);REQUIRE(!fseek(file,0,SEEK_SET));REQUIRE(fread(stream,1,length,file)==length);REQUIRE(!fclose(file));
    } else {REQUIRE(argc==1);synthetic();}
    REQUIRE((*(uint32_t *)(stream+4)==147||*(uint32_t *)(stream+4)==160)&&(*(uint32_t *)(stream+8)==175||*(uint32_t *)(stream+8)==181));U(0xe92e1c)=BASE;
    FUN_004d41e0(37);REQUIRE(position==length&&U(0xe945c0)==*(uint32_t *)(stream+4)&&U(0xe945c4)==*(uint32_t *)(stream+8));
    if(argc==3)emit();else puts("Mission save: complete C controller, native compatibility, histories and post-load resets.");
    free(stream);
done:
    REQUIRE(CryptReleaseContext(provider,0));VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;
}
