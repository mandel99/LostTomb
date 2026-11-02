#include "chunk_io.h"
#include "compression.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BASE 0x30000000u
#define STAGE ((uint8_t *)BASE)
#define HEAP ((uint8_t *)(BASE+0x200000))
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define REQUIRE(x) do { if(!(x)) { fprintf(stderr,"Mission chunks failure at %d\n",__LINE__); ExitProcess(1); } } while(0)
static uint8_t *stream;
static uint32_t length,position,errors,reads,allocated;
static HCRYPTPROV provider;

int FUN_005660ed(uint32_t file,void *out,uint32_t count)
{
    uint32_t available=length-position;
    REQUIRE(file==37);++reads;
    if(count>available)count=available;
    if(count)memcpy(out,stream+position,count);
    position+=count;return (int)count;
}
void FUN_004ce6e0(const char *message,int first,int second)
{ REQUIRE(!first&&!second);fprintf(stderr,"Codec error %08x\n",(unsigned)message);++errors; }
void *FUN_00561a1d(uint32_t count)
{ REQUIRE(!allocated&&count<=0x200000);allocated=1;memset(HEAP,0xcc,0x200000);return HEAP; }
void FUN_00561a2b(void *p) { REQUIRE(p==HEAP&&allocated);allocated=0; }
void *FUN_00562880(void *out,const void *in,uint32_t count) { return memmove(out,in,count); }
void FUN_004cc110(int group,int item) { (void)group;(void)item;++errors; }
void FUN_004cc160(int group,int item) { (void)group;(void)item;++errors; }
int FUN_00538cd0(const void *first,const void *second,int flags)
{ (void)first;(void)second;(void)flags;++errors;return 0; }

static void digest(const void *data,uint32_t size,char hex[65])
{
    HCRYPTHASH hash;DWORD used=32;uint8_t bytes[32];uint32_t i;
    static const char digits[]="0123456789abcdef";
    REQUIRE(CryptCreateHash(provider,CALG_SHA_256,0,0,&hash));
    REQUIRE(CryptHashData(hash,(const BYTE *)data,size,0));
    REQUIRE(CryptGetHashParam(hash,HP_HASHVAL,bytes,&used,0)&&used==32);
    REQUIRE(CryptDestroyHash(hash));
    for(i=0;i<32;++i) { hex[i*2]=digits[bytes[i]>>4];hex[i*2+1]=digits[bytes[i]&15]; }
    hex[64]=0;
}
static uint32_t load(unsigned limit,int emit)
{
    RestoreChunkDescriptor *schema=(RestoreChunkDescriptor *)0x5e09a0u;
    uint32_t index,completed=0;char hex[65];
    position=errors=reads=allocated=0;U(0xe945c0)=160;U(0xe945c4)=0;U(0xe92e1c)=(uint32_t)STAGE;
    for(index=0;index<limit;++index) {
        uint32_t count=(uint32_t)schema[index].size*schema[index].count;
        uint32_t result=FUN_004d0e10(index,schema,37);
        REQUIRE(!allocated&&count<=1200000);
        digest((const void *)(uintptr_t)schema[index].destination,count,hex);
        ++completed;
        if(emit) {
            printf("{\"index\":%u,\"result\":%u,\"position\":%u,\"version\":%u,\"declared\":%u,\"length\":%u,\"reads\":%u,\"errors\":%u,\"sha256\":\"%s\"}\n",
                index,result,position,U(0xe945c0),U(0xe945c4),count,reads,errors,hex);fflush(stdout);
        }
        if(!result||(index==174&&(int32_t)U(0xe945c0)<154))break;
    }
    return completed;
}
static void export_map(const char *path)
{
    RestoreChunkDescriptor *schema=(RestoreChunkDescriptor *)0x5e17d8u;
    FILE *file;uint32_t i,bytes,marker;
    REQUIRE(U(0xe945c0)==160);
    memcpy((void *)0x5e17c4u,(const void *)0x5e17ccu,4);U(0xe945c4)=24;
    memset((void *)0xe92e50u,0,6000);memcpy((void *)0xe92e50u,schema,24*20);
    file=fopen(path,"wb");REQUIRE(file!=NULL);
    for(i=0;i<24;++i) {
        bytes=(uint32_t)schema[i].size*schema[i].count;
        if(schema[i].codec) {marker=bytes?0x80000000u:0;REQUIRE(fwrite(&marker,4,1,file)==1);}
        if(bytes)REQUIRE(fwrite((const void *)(uintptr_t)schema[i].destination,1,bytes,file)==bytes);
    }
    REQUIRE(!fclose(file));
}
int main(int argc,char **argv)
{
    FILE *file;long end;uint32_t completed;
    REQUIRE(restore_data_open());
    REQUIRE(VirtualAlloc(STAGE,0x400000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==STAGE);
    REQUIRE(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
    if((argc==3&&!strcmp(argv[1],"--stream"))||(argc==4&&!strcmp(argv[1],"--export-map"))) {
        file=fopen(argv[2],"rb");REQUIRE(file!=NULL);
        REQUIRE(!fseek(file,0,SEEK_END));end=ftell(file);REQUIRE(end>=6012&&end<=0x2000000);
        length=(uint32_t)end;stream=(uint8_t *)malloc(length);REQUIRE(stream!=NULL);
        REQUIRE(!fseek(file,0,SEEK_SET));REQUIRE(fread(stream,1,length,file)==length);REQUIRE(!fclose(file));
        completed=load(181,1);REQUIRE(completed==((int32_t)U(0xe945c0)<154?175u:181u));
        REQUIRE(position==length&&!errors);free(stream);
        if(argc==4)export_map(argv[3]);
    } else {
        REQUIRE(argc==1);length=6012;stream=(uint8_t *)calloc(1,length);REQUIRE(stream!=NULL);
        *(uint32_t *)(stream+4)=160;*(uint32_t *)(stream+8)=181;
        memcpy(stream+12,(const void *)0x5e09a0u,181*20);
        REQUIRE(load(7,0)==7&&position==6012&&U(0xe945c0)==160&&U(0xe945c4)==181&&!errors);
        free(stream);puts("Mission chunks: real schema header, native version/count ABI and complete C codec pipeline.");
    }
    REQUIRE(CryptReleaseContext(provider,0));VirtualFree(STAGE,0,MEM_RELEASE);restore_data_close();return 0;
}
