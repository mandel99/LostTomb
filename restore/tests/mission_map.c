#include "mission_loaders.h"
#include "mission_routes.h"
#include "chunk_io.h"
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
#define REQUIRE(x) do { if(!(x)) {fprintf(stderr,"Mission map failure at %d\n",__LINE__);ExitProcess(1);} } while(0)
static uint8_t *stream;
static uint32_t length,position,reads,bump=BASE+0x800000,allocation_count;
static struct {uint32_t address,size,live;} allocations[128];
static HCRYPTPROV provider;
int FUN_005660ed(uint32_t fd,void *out,uint32_t n)
{uint32_t count=length-position;REQUIRE(fd==37);++reads;if(count>n)count=n;if(count)memcpy(out,stream+position,count);position+=count;return count;}
void FUN_004ce6e0(const char *message,int first,int second)
{(void)message;(void)first;(void)second;REQUIRE(0);}
void *FUN_00561a1d(uint32_t n)
{
    uint32_t address=bump;REQUIRE(n<=0x100000&&allocation_count<128&&n+32<0x32800000u-bump);
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
/* These branches must be skipped for the v160 fixtures. */
#define LEGACY(id) void FUN_##id(void){REQUIRE(0);}
LEGACY(004d17e0) LEGACY(004d19e0) LEGACY(004d1a70) LEGACY(004d32d0)
LEGACY(004d3d80) LEGACY(004d4080) LEGACY(004d40a0) LEGACY(004d40c0)
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
    RestoreChunkDescriptor *schema=(RestoreChunkDescriptor *)0x5e17d8u;char hex[65];uint32_t i,nonempty=0,points=0;
    for(i=0;i<50;++i){uint32_t n=U(0x7770e4u+i*324);if(n){++nonempty;points+=n;}}
    printf("{\"version\":%u,\"declared\":%u,\"position\":%u,\"reads\":%u,\"nonempty_routes\":%u,\"route_points\":%u,\"chunks\":[",U(0xe945c0),U(0xe945c4),position,reads,nonempty,points);
    for(i=0;i<24;++i) {uint32_t n=(uint32_t)schema[i].size*schema[i].count;digest(schema[i].destination,n,hex);printf("%s{\"index\":%u,\"length\":%u,\"sha256\":\"%s\"}",i?",":"",i,n,hex);}
    printf("],\"allocations\":[");
    for(i=0;i<allocation_count;++i) {digest(allocations[i].address,allocations[i].size+32,hex);printf("%s{\"address\":%u,\"size\":%u,\"live\":%u,\"sha256_with_guard\":\"%s\"}",i?",":"",allocations[i].address,allocations[i].size,allocations[i].live,hex);}
    puts("]}");
}
static void synthetic(void)
{
    RestoreChunkDescriptor *schema=(RestoreChunkDescriptor *)0x5e17d8u;uint32_t i,n;
    stream=(uint8_t *)calloc(1,0x200000);REQUIRE(stream!=NULL);
    memcpy(stream,(const void *)0x5e17ccu,4);*(uint32_t *)(stream+4)=160;*(uint32_t *)(stream+8)=24;
    memcpy(stream+12,schema,24*20);length=6012;
    for(i=4;i<24;++i){n=(uint32_t)schema[i].size*schema[i].count;length+=schema[i].codec?4:n;REQUIRE(length<0x200000);}
}
int main(int argc,char **argv)
{
    FILE *file;long bytes;
    SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX|SEM_NOOPENFILEERRORBOX);
    REQUIRE(restore_data_open());REQUIRE(VirtualAlloc((void *)BASE,0x02800000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);
    REQUIRE(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
    if(argc==3&&!strcmp(argv[1],"--map")) {
        file=fopen(argv[2],"rb");REQUIRE(file!=NULL);REQUIRE(!fseek(file,0,SEEK_END));bytes=ftell(file);REQUIRE(bytes>=6012&&bytes<=0x2000000);
        length=(uint32_t)bytes;stream=(uint8_t *)malloc(length);REQUIRE(stream!=NULL);REQUIRE(!fseek(file,0,SEEK_SET));REQUIRE(fread(stream,1,length,file)==length);REQUIRE(!fclose(file));
    } else {REQUIRE(argc==1);synthetic();}
    REQUIRE(*(uint32_t *)(stream+4)==160&&*(uint32_t *)(stream+8)==24);U(0xe92e1c)=BASE;
    FUN_004d4ae0(37);REQUIRE(position==length&&U(0xe945c0)==160&&U(0xe945c4)==24);
    if(argc==3)emit();else puts("Mission map: complete v160 C controller, chunk decoding and route reconstruction.");
    free(stream);REQUIRE(CryptReleaseContext(provider,0));VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;
}
