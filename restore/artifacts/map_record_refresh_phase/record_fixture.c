#include <windows.h>
#include <wincrypt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map_record_refresh.h"
#include "standalone_data.h"
#define CHECK(x) do{if(!(x)){fprintf(stderr,"check:%u\n",__LINE__);exit(2);}}while(0)
static uint32_t p[16],abi[8],used;
static uint32_t words[50000];
static const uint32_t regions[][2]={{0x77f738,19632},{0x784b52,32},{0xf56434,0x540},{0x5d2a28,2080},{0x10d69fc,24000*64+32},{0x126516c,32}};
static HCRYPTPROV provider;
static void put(uint32_t a,uint32_t v,uint32_t n){memcpy((void *)(uintptr_t)a,&v,n);}
static void event(uint32_t k,uint32_t a,uint32_t n,uint32_t v){CHECK(used+4<=50000);words[used++]=k;words[used++]=a;words[used++]=n;words[used++]=v;}
#ifdef MAP_RECORD_REFRESH_TRACE
uint32_t map_record_read(uint32_t a,uint32_t n){uint32_t v=0;memcpy(&v,(void *)(uintptr_t)a,n);event(0,a,n,v);return v;}
void map_record_write16(uint32_t a,uint32_t v){event(1,a,2,v&65535);put(a,v,2);}
void map_record_enter(uint32_t i){event(2,i,0,0);}
#endif
static void prepare(void)
{
    uint32_t i,r,active,type,selector;
    for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)memset((void *)(uintptr_t)regions[i][0],(p[13]+37*i)&255,regions[i][1]);
    for(i=0;i<200;++i){
        r=0x77f748+i*98;active=0;type=1;
        if(p[12]==1)active=1;
        if(p[12]==2){active=i%3;type=i%4;}
        if(p[12]==3)active=i==p[1];
        if(p[12]==4)active=i==0||i==199;
        put(r,type,1);put(r+1,active,1);
        selector=p[14]==1?(p[2]+i)%7:p[14]==2?(p[2]+i)&255:p[2];
        put(r+24,selector,1);put(r+25,(p[3]+i*73)&255,1);
    }
    put(0x77f748+p[1]*98+24,p[2],1);put(0x77f748+p[1]*98+25,p[3],1);
    put(0x784b62,p[4],2);put(0xf56444,p[5],4);put(0xf56954,p[6],4);put(0xf5693c,p[7],4);put(0xf56964,p[8],4);
    for(i=0;i<256;++i)put(0x5d2a38+i*8,p[9]+i*p[10],4);
    for(i=0;i<24000;++i){r=0x10d6a0c+i*64;put(r,(i*313+17)^p[13],2);put(r+2,(i*701+0x8001)^p[13],2);}
    put(0x126517c,p[11],4);used=0;
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
        call FUN_004425c0
        add esp,4
        mov dword ptr [abi+28],eax
        jmp finished
whole:
        call FUN_004426c0
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
            CHECK(p[0]<=1&&p[1]<200&&p[2]<=255&&p[3]<=255&&p[12]<=4&&p[14]<=2);
            prepare();invoke();CHECK(abi[0]==0x11111111&&abi[1]==0x22222222&&abi[2]==0x33333333&&abi[3]==0x44444444&&abi[4]==abi[5]&&abi[6]==0xbaadf00d);
            digest(memory,0);digest(trace,1);printf("{\"result\":%u,\"memory\":\"%s\",\"trace\":\"%s\",\"events\":%u}\n",abi[7],memory,trace,used/4);fflush(stdout);
        }
    }else puts("record refresh C ready");
done:CryptReleaseContext(provider,0);restore_data_close();return 0;
}
