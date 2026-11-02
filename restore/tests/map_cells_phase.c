#include "map_cells_phase.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
#define U(a) (*(uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(uint32_t)(a))
#define CHECK(x) do {if(!(x)){fprintf(stderr,"Map cells check line %d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t regions[16][2],region_count,reads[50000][3],read_count;
static HCRYPTPROV provider;
uint32_t map_cells_phase_read(uint32_t a,uint32_t width)
{
    uint32_t v=width==1?B(a):U(a);CHECK(read_count<50000);
    reads[read_count][0]=a;reads[read_count][1]=width;reads[read_count++][2]=v;return v;
}
static void region(uint32_t a,uint32_t n)
{
    DWORD previous;CHECK(region_count<16);CHECK(VirtualProtect((void *)(uintptr_t)a,n,PAGE_READWRITE,&previous));
    regions[region_count][0]=a;regions[region_count++][1]=n;memset((void *)(uintptr_t)a,0,n);
}
static void digest(char hex[65])
{
    HCRYPTHASH h;DWORD length=32;uint8_t b[32];uint32_t i;static const char digits[]="0123456789abcdef";
    CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));
    for(i=0;i<region_count;++i){CHECK(CryptHashData(h,(const BYTE *)regions[i],8,0));CHECK(CryptHashData(h,(const BYTE *)(uintptr_t)regions[i][0],regions[i][1],0));}
    CHECK(CryptGetHashParam(h,HP_HASHVAL,b,&length,0));CHECK(CryptDestroyHash(h));
    for(i=0;i<32;++i){hex[i*2]=digits[b[i]>>4];hex[i*2+1]=digits[b[i]&15];}hex[64]=0;
}
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) static void call_abi(uint32_t op,uint32_t a,uint32_t b,uint32_t c,uint32_t d,uint32_t out[7])
{
    __asm {
        push ebp
        push ebx
        push esi
        push edi
        mov eax,[esp+40]
        mov [eax+24],esp
        mov eax,[esp+36]
        push eax
        mov eax,[esp+36]
        push eax
        mov eax,[esp+36]
        push eax
        mov eax,[esp+36]
        push eax
        mov ebp,022222222h
        mov ebx,011111111h
        mov esi,033333333h
        mov edi,044444444h
        mov ecx,0badc0deh
        mov eax,[esp+36]
        cmp eax,0
        je ring
        cmp eax,1
        je banks
        cmp eax,2
        je maskpair
        cmp eax,3
        je rawgrowth
        call FUN_00487390
        jmp complete
ring:   call FUN_00486d30
        jmp complete
banks:  call FUN_00487260
        jmp complete
maskpair: call FUN_004872c0
        jmp complete
rawgrowth: call FUN_00487300
complete:
        add esp,16
        mov ecx,[esp+40]
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
static uint32_t run(uint32_t p[5],uint32_t *raw)
{
    uint32_t t=p[0]==4?p[1]+p[2]:p[1],out[7],i;DWORD previous;
    region_count=read_count=0;
    if(p[0]==0){region(0xaf598c-16,228*228*4+32);region(0xbca2c0,48);}
    else {
        region(0xaf598cu+t*4u-928u,1860);region(0xd05720u+t*4u-928u,1860);
        region(0xa509bcu+t-16u,36);region(0xa509bcu+t*4u-16u,36);
        region(0xe00000,256);
    }
    /* Fixture writes are supplied by the independent oracle after regions reset. */
    {
        uint32_t n,a,w,v;CHECK(scanf("%u",&n)==1);
        while(n--){CHECK(scanf("%u %u %u",&a,&w,&v)==3);if(w==1)B(a)=(uint8_t)v;else U(a)=v;}
    }
    for(i=0;i<region_count;++i)CHECK(VirtualProtect((void *)(uintptr_t)regions[i][0],regions[i][1],PAGE_READONLY,&previous));
    call_abi(p[0],p[1],p[2],p[3],p[4],out);
    for(i=0;i<region_count;++i)CHECK(VirtualProtect((void *)(uintptr_t)regions[i][0],regions[i][1],PAGE_READWRITE,&previous));
    CHECK(out[1]==0x11111111&&out[2]==0x22222222&&out[3]==0x33333333&&out[4]==0x44444444&&out[5]==out[6]);
    *raw=out[0];return p[0]==1?out[0]&255u:out[0];
}
int main(int argc,char **argv)
{
    uint32_t p[5],i,result,raw;char hex[65];
    SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")) {
        puts("{\"fields\":5}");fflush(stdout);
        for(;;){for(i=0;i<5;++i)if(scanf("%u",p+i)!=1)goto done;result=run(p,&raw);digest(hex);
            printf("{\"result\":%u,\"raw\":%u,\"memory\":\"%s\",\"reads\":[",result,raw,hex);
            for(i=0;i<read_count;++i)printf("%s[%u,%u,%u]",i?",":"",reads[i][0],reads[i][1],reads[i][2]);puts("]}");fflush(stdout);
        }
    }else puts("Map cells verifier ready; full deterministic fixtures are driven by the native oracle.");
done:CryptReleaseContext(provider,0);restore_data_close();return 0;
}
