#include "camera_bounds.h"
#include "restore_types.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
#define U(a) (*(uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(uint32_t)(a))
#define CHECK(x) do { if(!(x)) { fprintf(stderr,"Camera bounds failure at %d\n",__LINE__);ExitProcess(1); } } while(0)
#define FIELDS 21
static const uint32_t regions[][2]={{0x70e450,64},{0x70e484,231*457*4+32},{0xbca2b0,96},{0xe94db0,48},{0x6fdc40,32},{0x605a30,32},{0xe38dc0,32}};
static HCRYPTPROV provider;
static uint32_t p[FIELDS];
extern bool FUN_0047e830(int32_t);
static uint32_t grid_value(uint32_t index,uint32_t *r)
{
    *r^=*r<<13;*r^=*r>>17;*r^=*r<<5;
    switch(p[2]) {
    case 0:return 114u+114u*228u;
    case 1:return (uint32_t)-100000;
    case 2:return *r;
    case 3:return index;
    case 4:return index%231u+(index/231u/2u)*228u;
    case 5:return index%2?p[20]:114u+114u*228u;
    default:return p[20];
    }
}
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) static void call_abi(uint32_t op,uint32_t out[6])
{
    __asm {
        push ebp
        push ebx
        push esi
        push edi
        mov ecx,[esp+24]
        mov [ecx+20],esp
        mov ebp,022222222h
        mov ebx,011111111h
        mov esi,033333333h
        mov edi,044444444h
        cmp dword ptr[esp+20],1
        je clamp_only
        call FUN_004ddf90
        cmp dword ptr[esp+20],0
        je complete
clamp_only:
        call FUN_004de080
complete:
        mov ecx,[esp+24]
        mov [ecx],ebx
        mov [ecx+4],ebp
        mov [ecx+8],esi
        mov [ecx+12],edi
        mov [ecx+16],esp
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}
#pragma warning(pop)
static void run(const uint32_t *q)
{
    uint32_t i,r,out[6];DWORD old;
    memcpy(p,q,sizeof(p));r=p[1]?p[1]:1;
    for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i) {
        CHECK(VirtualProtect((void *)(uintptr_t)regions[i][0],regions[i][1],PAGE_READWRITE,&old));
        memset((void *)(uintptr_t)regions[i][0],(p[1]+i*29u)&255u,regions[i][1]);
    }
    for(i=0;i<231u*457u;++i) U(0x70e494+i*4)=grid_value(i,&r);
    U(0xbca2e8)=p[3];U(0xbca2f0)=p[4];U(0xbca2f4)=p[5];U(0xbca2f8)=p[6];U(0xbca2fc)=p[7];
    U(0x70e464)=p[8];U(0x70e460)=p[9];U(0xbca2bc)=p[10];U(0xbca2c0)=p[11];
    U(0xe94dd0)=p[12];U(0xe94dc8)=p[13];U(0xe94dcc)=p[14];U(0xe94dc4)=p[15];
    U(0x6fdc50)=p[16];U(0x605a3c)=p[17];B(0xe38dca)=(uint8_t)p[18];
    call_abi(p[0],out);
    CHECK(out[0]==0x11111111&&out[1]==0x22222222&&out[2]==0x33333333&&out[3]==0x44444444&&out[4]==out[5]);
}
static void digest(char hex[65])
{
    HCRYPTHASH h;DWORD length=32;uint8_t b[32];uint32_t i;static const char d[]="0123456789abcdef";
    CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));
    for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i) {
        CHECK(CryptHashData(h,(const BYTE *)regions[i],8,0));
        CHECK(CryptHashData(h,(const BYTE *)(uintptr_t)regions[i][0],regions[i][1],0));
    }
    CHECK(CryptGetHashParam(h,HP_HASHVAL,b,&length,0));CHECK(CryptDestroyHash(h));
    for(i=0;i<32;++i){hex[i*2]=d[b[i]>>4];hex[i*2+1]=d[b[i]&15];}hex[64]=0;
}
int main(int argc,char **argv)
{
    uint32_t q[FIELDS]={0},i;char hex[65];
    SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());
    CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")) {
        puts("{\"fields\":21}");fflush(stdout);
        for(;;) {
            for(i=0;i<FIELDS;++i)if(scanf("%u",q+i)!=1)goto done;
            run(q);digest(hex);printf("{\"memory\":\"%s\"}\n",hex);fflush(stdout);
        }
    } else {
        q[0]=1;q[8]=14;q[9]=30;q[10]=1000;q[11]=1001;q[12]=10;q[13]=12;q[14]=300;q[15]=400;q[17]=1;q[18]=0;
        run(q);CHECK(U(0xbca2bc)==286&&U(0xbca2c0)==370&&B(0xe38dca)==1);
        q[0]=0;q[2]=1;run(q);CHECK(U(0xe94dd0)==231&&U(0xe94dcc)==0xffffffffu);
        puts("Camera bounds: DWORD lookup, signed camera clamps, parity mask, real geometry helpers and guard bytes.");
    }
done:
    CryptReleaseContext(provider,0);restore_data_close();return 0;
}
