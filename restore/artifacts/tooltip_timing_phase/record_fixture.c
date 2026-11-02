#include <windows.h>
#include <wincrypt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tooltip_timing.h"
#include "standalone_data.h"
#define CHECK(x) do{if(!(x)){fprintf(stderr,"check:%u\n",__LINE__);exit(2);}}while(0)
static uint32_t p[16],abi[8],used;
static uint32_t words[50000];
static const uint32_t regions[][2]={{0x576170,0x120},{0x5cf74c,16*120+32},{0x605780,32},{0x7056b4,0x38},{0xe92dfc,32}};
static HCRYPTPROV provider;
static void put(uint32_t a,uint32_t v,uint32_t n){memcpy((void *)(uintptr_t)a,&v,n);}
static void event(uint32_t k,uint32_t a,uint32_t n,uint32_t v){CHECK(used+4<=50000);words[used++]=k;words[used++]=a;words[used++]=n;words[used++]=v;}
#ifdef TOOLTIP_TIMING_TRACE
uint32_t tooltip_read(uint32_t a){uint32_t v;memcpy(&v,(void *)(uintptr_t)a,4);event(0,a,4,v);return v;}
void tooltip_write(uint32_t a,uint32_t v){event(1,a,4,v);put(a,v,4);}
void tooltip_enter(uint32_t a,uint32_t v){event(2,a,v,0);}
#endif
static void group_data(uint32_t g,uint32_t n){uint32_t i;for(i=0;i<30;++i)put(0x5cf75c+g*120+i*4,i<n?p[8]:i==n?p[7]:p[14],4);}
static void prepare(void)
{
 uint32_t i,g;
 for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)memset((void *)(uintptr_t)regions[i][0],(p[9]+37*i)&255,regions[i][1]);
 for(i=0;i<64;++i)put(0x576180+i*4,(i+3)&15,4);
 for(g=0;g<16;++g)group_data(g,(p[6]+g*7)%31);
 g=(p[0]?p[10]:p[5])&0x1fffffffu;CHECK(g<16);group_data(g,p[6]);
 CHECK((p[1]&0x3fffffffu)<64);put(0x576180+(p[1]&0x3fffffffu)*4,p[5],4);
 put(0x7056c8,p[1],4);put(0x7056c4,p[2],4);put(0xe92e0c,p[3],4);put(0x605790,p[4],4);put(0x7056dc,p[11],4);put(0x7056d8,p[12],4);put(0x7056d0,p[13],4);used=0;
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
        jne group
        call FUN_00409e40
        mov dword ptr [abi+28],eax
        jmp finished
group:
        push dword ptr [p+40]
        call FUN_00405170
        add esp,4
        mov dword ptr [abi+28],eax
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
            CHECK(p[0]<=1&&p[6]<=30);
            prepare();invoke();CHECK(abi[0]==0x11111111&&abi[1]==0x22222222&&abi[2]==0x33333333&&abi[3]==0x44444444&&abi[4]==abi[5]&&abi[6]==0xbaadf00d);
            digest(memory,0);digest(trace,1);printf("{\"result\":%u,\"memory\":\"%s\",\"trace\":\"%s\",\"events\":%u}\n",abi[7],memory,trace,used/4);fflush(stdout);
        }
    }else puts("tooltip timing C ready");
done:CryptReleaseContext(provider,0);restore_data_close();return 0;
}
