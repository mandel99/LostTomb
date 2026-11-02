#include <windows.h>
#include <wincrypt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map_menu_scroll.h"
#include "standalone_data.h"
#define CHECK(x) do{if(!(x)){fprintf(stderr,"check:%u\n",__LINE__);exit(2);}}while(0)
static uint32_t p[16],abi[8],used;
static uint32_t words[50000];
static const uint32_t regions[][2]={{0x7758fc,40},{0x5d2a00,48},{0xe38d88,40}};
static HCRYPTPROV provider;
static void put(uint32_t a,uint32_t v,uint32_t n){memcpy((void *)(uintptr_t)a,&v,n);}
static void event(uint32_t k,uint32_t a,uint32_t n,uint32_t v){CHECK(used+4<=50000);words[used++]=k;words[used++]=a;words[used++]=n;words[used++]=v;}
#ifdef MAP_MENU_SCROLL_TRACE
uint32_t map_scroll_read(uint32_t a){uint32_t v;memcpy(&v,(void *)(uintptr_t)a,4);event(0,a,4,v);return v;}
void map_scroll_write(uint32_t a,uint32_t v){event(1,a,4,v);put(a,v,4);}
void map_scroll_enter(void){event(2,0x442800,0,0);}
#endif
static void prepare(void)
{
    uint32_t i;
    for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)memset((void *)(uintptr_t)regions[i][0],(p[11]+37*i)&255,regions[i][1]);
    put(0x77590c,p[2],4);put(0x775910,p[3],4);
    for(i=0;i<5;++i)put(0x5d2a10+i*4,p[4+i],4);
    put(0xe38d98,p[9],4);put(0xe38d9c,p[10],4);used=0;
}
__declspec(naked) static void invoke(void)
{
    __asm{
        push ebp
        push ebx
        push esi
        push edi
        mov dword ptr [abi+20],esp
        mov ebx,011111111h
        mov ebp,022222222h
        mov esi,033333333h
        mov edi,044444444h
        mov ecx,0badc0deh
        mov edx,0fefefefeh
        push 0baadf00dh
        cmp dword ptr [p],0
        jne whole
        push dword ptr [p+4]
        call FUN_00442730
        add esp,4
        jmp finished
whole:
        call FUN_00442800
        mov dword ptr [abi+28],0
finished:
        pop eax
        mov dword ptr [abi+24],eax
        mov dword ptr [abi],ebx
        mov dword ptr [abi+4],ebp
        mov dword ptr [abi+8],esi
        mov dword ptr [abi+12],edi
        mov dword ptr [abi+16],esp
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}
static void digest(char out[65],int trace)
{
    HCRYPTHASH h;DWORD n=32;uint8_t b[32];uint32_t i;const char *d="0123456789abcdef";
    CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));
    if(trace){if(used)CHECK(CryptHashData(h,(BYTE *)words,used*4,0));}
    else for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i){CHECK(CryptHashData(h,(BYTE *)regions[i],8,0));CHECK(CryptHashData(h,(BYTE *)(uintptr_t)regions[i][0],regions[i][1],0));}
    CHECK(CryptGetHashParam(h,HP_HASHVAL,b,&n,0));CryptDestroyHash(h);
    for(i=0;i<32;++i){out[2*i]=d[b[i]>>4];out[2*i+1]=d[b[i]&15];}out[64]=0;
}
int main(int argc,char **argv)
{
    uint32_t i;char memory[65],trace[65];CHECK(restore_data_open());CHECK(CryptAcquireContext(&provider,0,0,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){
        puts("{\"fields\":16}");fflush(stdout);
        for(;;){
            for(i=0;i<16;++i)if(scanf("%u",p+i)!=1)goto done;
            CHECK(p[0]<=1);
            prepare();invoke();CHECK(abi[0]==0x11111111&&abi[1]==0x22222222&&abi[2]==0x33333333&&abi[3]==0x44444444&&abi[4]==abi[5]&&abi[6]==0xbaadf00d);
            digest(memory,0);digest(trace,1);printf("{\"result\":%u,\"memory\":\"%s\",\"trace\":\"%s\",\"events\":%u}\n",0u,memory,trace,used/4);fflush(stdout);
        }
    }else puts("map menu scroll C ready");
done:CryptReleaseContext(provider,0);restore_data_close();return 0;
}
