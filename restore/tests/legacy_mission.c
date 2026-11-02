#include "mission_compat.h"
#include "town_menu.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BASE 0x30000000u
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define H(a) (*(int16_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define REQUIRE(x) do {if(!(x)){fprintf(stderr,"Legacy mission failure at %d\n",__LINE__);ExitProcess(1);}} while(0)
extern uint32_t FUN_004c5120(int);
extern void FUN_004d3c40(void),FUN_004d4040(void),FUN_004d4080(void),FUN_004d40a0(void),FUN_004d1a70(void),FUN_004d4180(void);
static const uint32_t regions[][2]={{0x77f748,19632},{0x785d70,18660},{0xbcd5f0,776032},{0x7999e0,1056032},
 {0xd05640,196},{0x9763b8,8512},{0x5d2d40,40},{0x5d2e60,40},{0x5d9c78,256},{0xbcba32,1024},
 {0x6fdcbc,80},{0xea184c,0xbd00},{BASE+0x1000,32},{BASE+0x2000,852},{BASE+0x10000,0xa000}};
static uint32_t params[10],events[32][2],event_count,bump,alloc_count;
static struct {uint32_t address,size,live;} allocations[16];
static HCRYPTPROV provider;
static void pattern(uint32_t address,uint32_t bytes,uint32_t seed)
{uint32_t i;for(i=0;i<bytes;++i)B(address+i)=(uint8_t)(seed+i*29+i/251);}
void *FUN_00561a1d(uint32_t n)
{
    uint32_t address=bump;REQUIRE(alloc_count<16&&n<=4000&&event_count<32);
    events[event_count][0]=0x561a1d;events[event_count++][1]=n;
    allocations[alloc_count].address=address;allocations[alloc_count].size=n;allocations[alloc_count++].live=1;
    memset((void *)(uintptr_t)address,0xcc,n+32);bump+=(n+32+63)&~63u;return (void *)(uintptr_t)address;
}
void FUN_00561a2b(void *p)
{REQUIRE(event_count<32);events[event_count][0]=0x561a2b;events[event_count++][1]=(uint32_t)p;}
static void history_setup(RestoreDynamicHistory *q,RestoreHistorySnapshot *s,uint32_t i,const uint32_t p[10])
{
    s->position=(int32_t)p[1]-3;s->capacity=p[7];s->step=(int32_t)p[1]-201;s->head=p[1]+259;s->flag=(uint8_t)(p[1]+i);
    q->flag=(uint8_t)(p[1]+i+3);q->step=(int32_t)p[1]-201;q->head=p[1]+259;q->position=(int32_t)p[1]-3;
    q->capacity=p[6];q->values=p[9]?(int32_t *)(BASE+0x10000+i*4096):NULL;
}
/* op,seed,town_count,walker_kind,frame_counter,building_type,
   current_history_capacity,stored_capacity,load_direction,has_buffer */
static uint32_t run(const uint32_t p[10])
{
    uint32_t i,j;static const int16_t values[]={-32768,-1,0,30,31,32,32764,32767};
    REQUIRE(p[0]<15&&p[6]<=258&&p[7]<=258&&p[9]<2);memcpy(params,p,sizeof(params));event_count=alloc_count=0;bump=BASE+0x80000;
    for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)pattern(regions[i][0],regions[i][1],p[1]+i);
    U(0x785d70)=p[2];
    for(i=0;i<150;++i) {uint32_t r=0x785d74+i*124;B(r+2)=(uint8_t)(i+p[1]);for(j=0;j<4;++j)H(r+4+j*2)=values[(i+j+p[1])%8];}
    for(i=0;i<256;++i)B(0x5d9c78+i)=i%3==0?0:(uint8_t)(i|1);
    for(i=0;i<4000;++i){H(0x7999e8+i*264)=(int16_t)p[5];B(0x799ae0+i*264)=1;H(0x799af0+i*264)=(int16_t)(0x29+i%3);B(0x799b8a+i*264)=(uint8_t)(p[1]+i);}
    for(i=1;i<2000;++i) {uint32_t r=i*388;B(0xbcd602+r)=(uint8_t)p[3];H(0xbcd652+r)=1+i%3998;H(0xbcd658+r)=i%6;}
    for(i=0;i<6;++i)B(0xbcba32+i*144)=(uint8_t)((p[1]+i)%6);
    for(i=0;i<41;++i)U(0xd05640+i*4)=p[4];
    for(i=0;i<10;++i)history_setup((RestoreDynamicHistory *)(0x9783c0+i*28),(RestoreHistorySnapshot *)(0x9763b8+i*820),i,p);
    memcpy((void *)(BASE+0x1000),(const void *)0x9783c0,28);memcpy((void *)(BASE+0x2000),(const void *)0x9763b8,820);
    U(0xeaac28)=p[2]&1;
    switch(p[0]) {
    case 0:FUN_004d19e0();break;case 1:FUN_004d40c0();break;case 2:FUN_004d32f0();break;
    case 3:case 4:FUN_004d0a40((uint8_t)p[8]);break;
    case 5:return FUN_004493c0((const RestoreTownRecord *)0x785d74u);
    case 6:return FUN_004c5120(257);
    case 7:FUN_004f67e0((RestoreDynamicHistory *)(BASE+0x1000),(const RestoreHistorySnapshot *)(BASE+0x2000));break;
    case 8:FUN_004f6810((RestoreDynamicHistory *)(BASE+0x1000),(RestoreHistorySnapshot *)(BASE+0x2000));break;
    case 9:FUN_004d3c40();break;case 10:FUN_004d4040();break;case 11:FUN_004d4080();break;
    case 12:FUN_004d40a0();break;case 13:FUN_004d1a70();break;case 14:FUN_004d4180();break;
    }
    return 0;
}
static void digest(uint32_t address,uint32_t n,char hex[65])
{
    HCRYPTHASH h;DWORD used=32;uint8_t bytes[32];uint32_t i;static const char digits[]="0123456789abcdef";
    REQUIRE(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));REQUIRE(CryptHashData(h,(const BYTE *)(uintptr_t)address,n,0));
    REQUIRE(CryptGetHashParam(h,HP_HASHVAL,bytes,&used,0)&&used==32);REQUIRE(CryptDestroyHash(h));
    for(i=0;i<32;++i){hex[i*2]=digits[bytes[i]>>4];hex[i*2+1]=digits[bytes[i]&15];}hex[64]=0;
}
static void output(uint32_t result)
{
    uint32_t i;char hex[65];printf("{\"result\":%u,\"hashes\":[",result);
    for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i){digest(regions[i][0],regions[i][1],hex);printf("%s\"%s\"",i?",":"",hex);}
    printf("],\"events\":[");for(i=0;i<event_count;++i)printf("%s[%u,%u]",i?",":"",events[i][0],events[i][1]);
    printf("],\"allocations\":[");for(i=0;i<alloc_count;++i){digest(allocations[i].address,allocations[i].size+32,hex);printf("%s[%u,%u,\"%s\"]",i?",":"",allocations[i].address,allocations[i].size,hex);}puts("]}");fflush(stdout);
}
int main(int argc,char **argv)
{
    uint32_t p[10]={0,7,6,3,256,60,3,7,1,1},i,result;
    SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX|SEM_NOOPENFILEERRORBOX);
    _set_abort_behavior(0,_WRITE_ABORT_MSG|_CALL_REPORTFAULT);
    REQUIRE(restore_data_open());REQUIRE(VirtualAlloc((void *)BASE,0x200000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);
    REQUIRE(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){puts("{\"fields\":10}");fflush(stdout);for(;;){for(i=0;i<10;++i)if(scanf("%u",p+i)!=1)goto done;result=run(p);output(result);}}
    else {REQUIRE(argc==1);run(p);REQUIRE(U(0x77f748+94)==0x02020202);p[0]=2;run(p);REQUIRE(U(0xbcd690+388)==255&&U(0xd056dc)==0);p[0]=3;run(p);REQUIRE(alloc_count==10&&((RestoreDynamicHistory *)0x9783c0u)->capacity==7);puts("Legacy mission: exact city conversion, town links, walker indexing and complete history snapshot ABI.");}
done:CryptReleaseContext(provider,0);VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;
}
