/* Reusable single-WORD framebuffer fixture for independent leaf batches. */
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
#define U(a) (*(uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(uint32_t)(a))
#define ALLOCATION 0x30000000u
#define BUFFER 0x30001000u
#define CAPACITY 65536u
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Pixel fixture check line %d\n",__LINE__);ExitProcess(1);}}while(0)
void FUN_0040fa40(int32_t,int32_t,uint16_t);
void FUN_0040fa70(int32_t,int32_t,uint16_t);
void FUN_0040fae0(int32_t,int32_t,uint16_t);
static const uint32_t regions[][2]={{BUFFER,CAPACITY},{0xe38d84,36},{0xe92e08,36}};
static HCRYPTPROV provider;
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) static void call_abi(uint32_t op,uint32_t x,uint32_t y,uint32_t mask,uint32_t out[7])
{
    __asm {
        push ebp
        push ebx
        push esi
        push edi
        mov eax,[esp+36]
        mov [eax+20],esp
        mov eax,[esp+32]
        push eax
        mov eax,[esp+32]
        push eax
        mov eax,[esp+32]
        push eax
        mov ebp,022222222h
        mov ebx,011111111h
        mov esi,033333333h
        mov edi,044444444h
        mov ecx,0badc0deh
        mov eax,[esp+32]
        cmp eax,0
        je store_word
        cmp eax,1
        je and_word
        call FUN_0040fae0
        jmp complete
store_word: call FUN_0040fa40
        jmp complete
and_word: call FUN_0040fa70
complete:
        mov ecx,[esp+48]
        mov dword ptr [ecx+24],1
        mov eax,[esp]
        cmp eax,[esp+36]
        jne arguments_changed
        mov eax,[esp+4]
        cmp eax,[esp+40]
        jne arguments_changed
        mov eax,[esp+8]
        cmp eax,[esp+44]
        jne arguments_changed
        jmp arguments_checked
arguments_changed:
        mov dword ptr [ecx+24],0
arguments_checked:
        add esp,12
        mov ecx,[esp+36]
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
static void run(const uint32_t p[9])
{
    uint32_t i,out[7],target=BUFFER+p[6],expected;DWORD previous;
    CHECK(p[0]<3&&p[6]+2<=CAPACITY);
    CHECK(p[4]+p[1]*2u+p[2]*p[3]==target);
    for(i=0;i<CAPACITY;++i)B(BUFFER+i)=(uint8_t)(i*37u+p[7]);
    memset((void *)(uintptr_t)regions[1][0],0xa5,regions[1][1]);
    memset((void *)(uintptr_t)regions[2][0],0x5a,regions[2][1]);
    W(target)=(uint16_t)p[8];U(0xe38d94)=p[3];U(0xe92e18)=p[4];
    CHECK(VirtualProtect((void *)0xe38000,0x2000,PAGE_READONLY,&previous));
    CHECK(VirtualProtect((void *)0xe92000,0x2000,PAGE_READONLY,&previous));
    call_abi(p[0],p[1],p[2],p[5],out);
    CHECK(VirtualProtect((void *)0xe38000,0x2000,PAGE_READWRITE,&previous));
    CHECK(VirtualProtect((void *)0xe92000,0x2000,PAGE_READWRITE,&previous));
    CHECK(out[0]==0x11111111&&out[1]==0x22222222&&out[2]==0x33333333&&out[3]==0x44444444&&out[4]==out[5]&&out[6]==1);
    expected=p[0]==0?p[5]:p[0]==1?(p[8]&p[5]):(p[8]|p[5]);CHECK(W(target)==(uint16_t)expected);
}
static void digest(char hex[65])
{
    HCRYPTHASH hash;DWORD length=32;uint8_t bytes[32];uint32_t i;const char *digits="0123456789abcdef";
    CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&hash));
    for(i=0;i<3;++i)CHECK(CryptHashData(hash,(const BYTE *)(uintptr_t)regions[i][0],regions[i][1],0));
    CHECK(CryptGetHashParam(hash,HP_HASHVAL,bytes,&length,0));CHECK(CryptDestroyHash(hash));
    for(i=0;i<32;++i){hex[i*2]=digits[bytes[i]>>4];hex[i*2+1]=digits[bytes[i]&15];}hex[64]=0;
}
int main(int argc,char **argv)
{
    uint32_t p[9],i;char hex[65];
    SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
    CHECK(VirtualAlloc((void *)ALLOCATION,CAPACITY+8192,MEM_RESERVE,PAGE_NOACCESS)==(void *)ALLOCATION);
    CHECK(VirtualAlloc((void *)BUFFER,CAPACITY,MEM_COMMIT,PAGE_READWRITE)==(void *)BUFFER);
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){
        puts("{\"fields\":9}");fflush(stdout);
        for(;;){for(i=0;i<9;++i)if(scanf("%u",p+i)!=1)goto done;run(p);digest(hex);printf("{\"memory\":\"%s\"}\n",hex);fflush(stdout);}
    }else puts("Guarded WORD framebuffer fixture ready.");
done:VirtualFree((void *)ALLOCATION,0,MEM_RELEASE);CryptReleaseContext(provider,0);restore_data_close();return 0;
}
