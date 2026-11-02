#include "map_neighbors_phase.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
#define U(a) (*(uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(uint32_t)(a))
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Map neighbors check line %d\n",__LINE__);ExitProcess(1);}}while(0)
void FUN_0047db00(uint32_t);
static HCRYPTPROV provider;
static uint32_t regions[4][2],events[128][4],event_count,before_slots[8];
static void event(uint32_t kind,uint32_t a,uint32_t width,uint32_t value)
{
    uint32_t *e;CHECK(event_count<128);e=events[event_count++];e[0]=kind;e[1]=a;e[2]=width;e[3]=value;
}
uint32_t map_neighbors_read(uint32_t a,uint32_t width)
{
    uint32_t value=width==1?B(a):U(a);event(0,a,width,value);return value;
}
void map_neighbors_write(uint32_t a,uint32_t value) {event(1,a,4,value);U(a)=value;}
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) static void call_abi(uint32_t op,uint32_t a,uint32_t b,uint32_t out[7])
{
    __asm {
        push ebp
        push ebx
        push esi
        push edi
        mov eax,[esp+32]
        mov [eax+24],esp
        mov eax,[esp+28]
        push eax
        mov eax,[esp+28]
        push eax
        mov ebx,011111111h
        mov ebp,022222222h
        mov esi,033333333h
        mov edi,044444444h
        mov ecx,0badc0deh
        mov eax,[esp+28]
        cmp eax,0
        je all_mask
        cmp eax,1
        je filtered
        cmp eax,3
        je secondary
        cmp eax,4
        je cardinal
        call FUN_0047ddd0
        jmp complete
all_mask: call FUN_0047dbc0
        jmp complete
filtered: call FUN_0047dc80
        jmp complete
secondary: call FUN_0047dee0
        jmp complete
cardinal: call FUN_0047e000
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
static uint32_t run(const uint32_t p[30])
{
    uint32_t tile=p[1],i,out[7];DWORD previous;
    regions[0][0]=0xaf598cu+tile*4u-932u;regions[0][1]=1868;
    regions[1][0]=0xb285ccu+tile-245u;regions[1][1]=492;
    regions[2][0]=0xbc9758;regions[2][1]=36;
    regions[3][0]=0xbc97b0;regions[3][1]=64;
    for(i=0;i<4;++i){CHECK(VirtualProtect((void *)(uintptr_t)regions[i][0],regions[i][1],PAGE_READWRITE,&previous));memset((void *)(uintptr_t)regions[i][0],(int)((p[4]+i*41u)&255u),regions[i][1]);}
    U(0xbc9768)=tile;
    for(i=0;i<8;++i){U(mn_terrain(tile,i))=p[6+i];U(mn_slot(i))=p[14+i];B(mn_secondary(tile,i))=(uint8_t)p[22+i];}
    CHECK(VirtualProtect((void *)(uintptr_t)regions[0][0],regions[0][1],PAGE_READONLY,&previous));
    CHECK(VirtualProtect((void *)(uintptr_t)regions[1][0],regions[1][1],PAGE_READONLY,&previous));
    if(p[0]==5)FUN_0047db00(p[5]);
    for(i=0;i<8;++i)before_slots[i]=U(mn_slot(i));event_count=0;
    call_abi(p[0],p[2],p[3],out);
    CHECK(out[1]==0x11111111&&out[2]==0x22222222&&out[3]==0x33333333&&out[4]==0x44444444&&out[5]==out[6]);
    CHECK(VirtualProtect((void *)(uintptr_t)regions[0][0],regions[0][1],PAGE_READWRITE,&previous));
    CHECK(VirtualProtect((void *)(uintptr_t)regions[1][0],regions[1][1],PAGE_READWRITE,&previous));
    if(p[0]==4){CHECK(out[0]<=1);return out[0];}return 0;
}
static void digest(char hex[65])
{
    HCRYPTHASH h;DWORD n=32;uint8_t bytes[32];uint32_t i;const char *digits="0123456789abcdef";
    CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));
    for(i=0;i<4;++i){CHECK(CryptHashData(h,(BYTE *)regions[i],8,0));CHECK(CryptHashData(h,(BYTE *)(uintptr_t)regions[i][0],regions[i][1],0));}
    CHECK(CryptGetHashParam(h,HP_HASHVAL,bytes,&n,0));CHECK(CryptDestroyHash(h));
    for(i=0;i<32;++i){hex[2*i]=digits[bytes[i]>>4];hex[2*i+1]=digits[bytes[i]&15];}hex[64]=0;
}
int main(int argc,char **argv)
{
    uint32_t p[30],i,j,result;char hex[65];
    SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){
        puts("{\"fields\":30}");fflush(stdout);
        for(;;){for(i=0;i<30;++i)if(scanf("%u",p+i)!=1)goto done;CHECK(p[0]<6);result=run(p);digest(hex);
            printf("{\"result\":%u,\"memory\":\"%s\",\"before_slots\":[",result,hex);
            for(i=0;i<8;++i)printf("%s%u",i?",":"",before_slots[i]);printf("],\"events\":[");
            for(i=0;i<event_count;++i){printf("%s[",i?",":"");for(j=0;j<4;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);
        }
    }else puts("Map neighbor fixture ready: full DWORD slots, byte grid, ordered events and composed original helper.");
done:CryptReleaseContext(provider,0);restore_data_close();return 0;
}
