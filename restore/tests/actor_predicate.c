#include "actor_predicate.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define W(a) (*(uint16_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define CHECK(x) do { if (!(x)) {fprintf(stderr,"Actor predicate failure at %d\n",__LINE__); ExitProcess(1);} } while(0)
/* op,seed,mode,actor,type,subtype,action,marker,actorBuilding,actorBuildingType,
   actorFlags,profile,active,selected,selectedType,selectedStatus,nextProfile,
   nextSelected,nextType,nextStatus,controlled,result5120a,result5120b,result5120c,
   result52d0,mutateAtCallMask,mutationBits,directDesired,actorNoAccess. */
static uint32_t p[29], regions[12][2],region_count,reads[32][3],read_count;
static uint32_t events[8][7],event_count,category_calls;
static HCRYPTPROV provider;
uint32_t actor_predicate_actual_5120(int32_t);
uint8_t actor_predicate_actual_52d0(int32_t,uint32_t);

uint32_t restore_actor_predicate_trace_read(uint32_t address,uint32_t width)
{
    uint32_t value=width==1?B(address):width==2?W(address):U(address);
    CHECK(read_count<32);reads[read_count][0]=address;reads[read_count][1]=width;reads[read_count++][2]=value;
    return value;
}
static void endpoint(uint32_t address,uint32_t arg0,uint32_t arg1)
{
    uint32_t *e,profile=p[11]*0x49d8u;
    CHECK(event_count<8);e=events[event_count++];e[0]=address;e[1]=arg0;e[2]=arg1;
    e[3]=U(0xe94e0c);e[4]=U(0xeaac28);e[5]=W(0xea5ce8+profile);e[6]=U(0xea5f9c+profile);
    if (p[25]&(1u<<(event_count-1))) {
        if(p[26]&1)U(0xeaac28)=p[16];
        if(p[26]&2)U(0xea5f9c+profile)=p[17];
        if(p[26]&4)W(0xea5ce8+profile)=0;
        if(p[26]&8)W(0xbcd652+p[3]*0x184u)=(uint16_t)p[17];
        if(p[26]&16)U(0xe94e0c)=0xdeadbeef;
        if(p[26]&32)W(0x7999e8+p[13]*0x108u)=68;
        if(p[26]&64)B(0x799a81+p[17]*0x108u)=3;
    }
}
uint32_t FUN_004c5120(int32_t building)
{
    uint32_t result;
    endpoint(0x4c5120,(uint32_t)building,0);
    CHECK(category_calls<3);
    result=p[20]?p[21+category_calls]:actor_predicate_actual_5120(building);
    ++category_calls;return result;
}
uint8_t FUN_004c52d0(int32_t building,uint32_t desired)
{
    endpoint(0x4c52d0,(uint32_t)building,desired);
    return p[20]?(uint8_t)p[24]:actor_predicate_actual_52d0(building,desired);
}

/* Inspect the actual compiler-produced call ABI, with adversarial ECX and
   sentinel callee-saved registers, without relying on C's register allocation. */
#pragma warning(push)
#pragma warning(disable:4100) /* Naked arguments are accessed through ESP. */
__declspec(naked) static void call_abi(uint32_t op,uint32_t arg,uint32_t desired,uint32_t out[7])
{
    __asm {
        push ebp
        push ebx
        push esi
        push edi
        mov ecx,[esp+32]
        mov [ecx+24],esp
        mov ebp,022222222h
        mov ebx,011111111h
        mov esi,033333333h
        mov edi,044444444h
        mov eax,[esp+20]
        mov edx,[esp+24]
        mov ecx,[esp+28]
        push ecx
        push edx
        mov ecx,0badc0deh
        cmp eax,1
        je category
        cmp eax,2
        je status
        call FUN_004387d0
        jmp complete
category:
        call FUN_004c5120
        jmp complete
status:
        call FUN_004c52d0
complete:
        add esp,8
        mov ecx,[esp+32]
        mov [ecx],eax
        mov [ecx+4],ebx
        mov [ecx+8],ebp
        mov [ecx+12],esi
        mov [ecx+16],edi
        mov [ecx+20],esp
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}
#pragma warning(pop)
static void region(uint32_t address,uint32_t size)
{
    uint32_t i;DWORD previous;
    for(i=0;i<region_count;++i)if(regions[i][0]==address)return;
    CHECK(region_count<12);CHECK(VirtualProtect((void *)(uintptr_t)address,size,PAGE_READWRITE,&previous));
    regions[region_count][0]=address;regions[region_count++][1]=size;
    memset((void *)(uintptr_t)address,(int)((p[1]+region_count-1)&255),size);
}
static uint32_t run(const uint32_t *params,uint32_t *raw)
{
    uint32_t actor,building,profile,next_profile,out[7],i;DWORD previous=0,unused;
    memcpy(p,params,sizeof(p));CHECK(p[0]<=2);read_count=event_count=category_calls=region_count=0;
    actor=p[3]*0x184u;building=(uint32_t)(int32_t)(int16_t)p[8];profile=p[11]*0x49d8u;next_profile=p[16]*0x49d8u;
    region(0xe94e08,12);region(0xeaac20,20);region(0xbcd600+actor-8,0x184+16);
    region(0x7999d8+building*0x108u-8,0x108+16);region(0x7999d8+p[13]*0x108u-8,0x108+16);region(0x7999d8+p[17]*0x108u-8,0x108+16);
    region(0xea5ce0+profile,0x2c8);region(0xea5ce0+next_profile,0x2c8);
    U(0xe94e0c)=p[2];U(0xeaac28)=p[11];B(0xbcd602+actor)=(uint8_t)p[4];B(0xbcd603+actor)=(uint8_t)p[5];
    B(0xbcd630+actor)=(uint8_t)p[6];B(0xbcd65e+actor)=(uint8_t)p[7];W(0xbcd652+actor)=(uint16_t)p[8];
    W(0x7999e8+building*0x108u)=(uint16_t)p[9];U(0x799ad8+building*0x108u)=p[10];
    W(0xea5ce8+profile)=(uint16_t)p[12];U(0xea5f9c+profile)=p[13];
    W(0xea5ce8+next_profile)=1;U(0xea5f9c+next_profile)=p[17];
    W(0x7999e8+p[13]*0x108u)=(uint16_t)p[14];B(0x799a81+p[13]*0x108u)=(uint8_t)p[15];
    W(0x7999e8+p[17]*0x108u)=(uint16_t)p[18];B(0x799a81+p[17]*0x108u)=(uint8_t)p[19];
    if(p[28])CHECK(VirtualProtect((void *)(uintptr_t)((0xbcd600+actor)&~4095u),4096,PAGE_NOACCESS,&previous));
    call_abi(p[0],p[0]?p[13]:p[3],p[27],out);
    if(p[28])CHECK(VirtualProtect((void *)(uintptr_t)((0xbcd600+actor)&~4095u),4096,previous,&unused));
    CHECK(out[1]==0x11111111&&out[2]==0x22222222&&out[3]==0x33333333&&out[4]==0x44444444&&out[5]==out[6]);
    *raw=out[0];i=p[0]==1?out[0]:out[0]&255u;
    if(p[0]==0)CHECK(i<=1);
    return i;
}
static void digest(char hex[65])
{
    HCRYPTHASH h;DWORD length=32;uint8_t b[32];uint32_t i;static const char digits[]="0123456789abcdef";
    CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));
    for(i=0;i<region_count;++i){CHECK(CryptHashData(h,(const BYTE *)regions[i],8,0));CHECK(CryptHashData(h,(const BYTE *)(uintptr_t)regions[i][0],regions[i][1],0));}
    CHECK(CryptGetHashParam(h,HP_HASHVAL,b,&length,0));CHECK(CryptDestroyHash(h));
    for(i=0;i<32;++i){hex[i*2]=digits[b[i]>>4];hex[i*2+1]=digits[b[i]&15];}hex[64]=0;
}
int main(int argc,char **argv)
{
    uint32_t params[29]={0,1,10,257,27,0,0,0,7,61,0,1,1,9,61,1,2,11,63,1,0,1,1,1,1,0,0,2,0},i,j,result,raw;char hex[65];
    SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){
        puts("{\"fields\":29}");fflush(stdout);
        for(;;){for(i=0;i<29;++i)if(scanf("%u",params+i)!=1)goto done;result=run(params,&raw);digest(hex);
            printf("{\"result\":%u,\"raw\":%u,\"memory\":\"%s\",\"reads\":[",result,raw,hex);
            for(i=0;i<read_count;++i)printf("%s[%u,%u,%u]",i?",":"",reads[i][0],reads[i][1],reads[i][2]);printf("],\"events\":[");
            for(i=0;i<event_count;++i){printf("%s[",i?",":"");for(j=0;j<7;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);
        }
    }else{
        CHECK(run(params,&raw)==1);CHECK(event_count==4&&events[0][1]==9&&events[1][1]==7&&events[2][1]==9&&events[3][2]==1);
        params[25]=4;params[26]=1|4|8|16;CHECK(run(params,&raw)==1);CHECK(events[2][1]==9&&events[3][1]==11&&events[3][2]==1);
        params[25]=0;params[26]=0;params[2]=49;params[9]=63;CHECK(run(params,&raw)==0);params[15]=3;CHECK(run(params,&raw)==1);
        params[2]=51;params[28]=1;CHECK(run(params,&raw)==1);params[2]=0x10000000;CHECK(run(params,&raw)==0);
        puts("Actor predicate: complete overlays, ordered reads, real helpers, profile reloads and C ABI.");
    }
done:CryptReleaseContext(provider,0);restore_data_close();return 0;
}
