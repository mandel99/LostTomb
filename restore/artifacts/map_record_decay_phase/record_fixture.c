#include <windows.h>
#include <wincrypt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map_record_decay.h"
#include "standalone_data.h"
#define CHECK(x) do{if(!(x)){fprintf(stderr,"check:%u\n",__LINE__);exit(2);}}while(0)
static uint32_t p[16],abi[8],used;
static uint32_t words[50000];
static const uint32_t regions[][2]={{0x77f738,0x6cf4}};
static HCRYPTPROV provider;
static void put(uint32_t a,uint32_t v,uint32_t n){memcpy((void *)(uintptr_t)a,&v,n);}
static void event(uint32_t k,uint32_t a,uint32_t n,uint32_t v){CHECK(used+4<=50000);words[used++]=k;words[used++]=a;words[used++]=n;words[used++]=v;}
#ifdef MAP_RECORD_DECAY_TRACE
uint32_t map_decay_read(uint32_t a,uint32_t n){uint32_t v=0;memcpy(&v,(void *)(uintptr_t)a,n);event(0,a,n,v);return v;}
void map_decay_write(uint32_t a,uint32_t n,uint32_t v){v&=n==1?255u:65535u;event(1,a,n,v);put(a,v,n);}
#endif
static void entry(uint32_t i,uint32_t type,uint32_t active,uint32_t state,uint32_t timer)
{
    uint32_t r=0x77f748u+i*98u;put(r,type,1);put(r+1,active,1);put(r+24,state,1);put(r+51,timer,1);
}
static void prepare(void)
{
    uint32_t i;memset((void *)(uintptr_t)regions[0][0],p[8]&255,regions[0][1]);
    for(i=0;i<200;++i){
        if(p[2]==1)entry(i,1,1,p[6],p[7]);
        else if(p[2]==2)entry(i,i%4,i%3,i%7,i%5);
        else entry(i,1,0,p[6],p[7]);
    }
    entry(p[3],p[4],p[5],p[6],p[7]);
    if(p[9]<200)entry(p[9],p[10],p[11],p[12],p[13]);
    put(0x784f10,p[1],4);used=0;
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
        cmp dword ptr [p],1
        je second
        call FUN_00442ef0
        cmp dword ptr [p],2
        jne finished
second:
        call FUN_00442f80
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
            CHECK(p[0]<=2&&p[2]<=2&&p[3]<200);
            prepare();invoke();CHECK(abi[0]==0x11111111&&abi[1]==0x22222222&&abi[2]==0x33333333&&abi[3]==0x44444444&&abi[4]==abi[5]&&abi[6]==0xbaadf00d);
            digest(memory,0);digest(trace,1);printf("{\"result\":%u,\"memory\":\"%s\",\"trace\":\"%s\",\"events\":%u}\n",0u,memory,trace,used/4);fflush(stdout);
        }
    }else puts("record decay C ready");
done:CryptReleaseContext(provider,0);restore_data_close();return 0;
}
