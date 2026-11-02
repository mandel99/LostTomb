#include <windows.h>
#include <wincrypt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map_hover_geometry.h"
#include "standalone_data.h"
#define CHECK(x) do{if(!(x)){fprintf(stderr,"check:%u\n",__LINE__);exit(2);}}while(0)
static uint32_t p[16],abi[8],used;
static uint32_t words[50000];
static const uint32_t regions[][2]={{0x70e3e8,0xac+24*231*4+16},{0xe38e20,40}};
static HCRYPTPROV provider;
static void put(uint32_t a,uint32_t v,uint32_t n){memcpy((void *)(uintptr_t)a,&v,n);}
static void event(uint32_t k,uint32_t a,uint32_t n,uint32_t v){CHECK(used+4<=50000);words[used++]=k;words[used++]=a;words[used++]=n;words[used++]=v;}
#ifdef MAP_HOVER_GEOMETRY_TRACE
uint32_t map_hover_read(uint32_t a){uint32_t v;memcpy(&v,(void *)(uintptr_t)a,4);event(0,a,4,v);return v;}
void map_hover_write(uint32_t a,uint32_t v){event(1,a,4,v);put(a,v,4);}
void map_hover_enter(uint32_t v){event(2,0x430790,v,0);}
#endif
static void prepare(void)
{
 uint32_t i,j,v;
 for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)memset((void *)(uintptr_t)regions[i][0],(p[13]+37*i)&255,regions[i][1]);
 put(0xe38e34,p[0],4);put(0xe38e30,p[1],4);
 put(0x70e490,p[2],4);put(0x70e48c,p[3],4);put(0x70e458,p[4],4);put(0x70e45c,p[5],4);put(0x70e44c,p[6],4);put(0x70e460,p[7],4);put(0x70e464,p[8],4);put(0x70e474,p[9],4);put(0x70e478,p[10],4);
 for(i=0;i<24;++i)for(j=0;j<231;++j){v=p[12]+i*228+j%228;if(p[11]==1)v=0;if(p[11]==2)v=0x0fff0000;if(p[11]==3)v=0x0ffeffff;if(p[11]==4)v=0x80000000;if(p[11]==5)v=0xffffffff;put(0x70e494+(i*231+j)*4,v,4);}
 used=0;
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
        push dword ptr [p+56]
        call FUN_00430790
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
            CHECK(p[5]<0x7fffffff&&p[6]>0&&p[6]<=0x7fffffff&&p[11]<=5);
            prepare();invoke();CHECK(abi[0]==0x11111111&&abi[1]==0x22222222&&abi[2]==0x33333333&&abi[3]==0x44444444&&abi[4]==abi[5]&&abi[6]==0xbaadf00d);
            digest(memory,0);digest(trace,1);printf("{\"result\":%u,\"memory\":\"%s\",\"trace\":\"%s\",\"events\":%u}\n",abi[7],memory,trace,used/4);fflush(stdout);
        }
    }else puts("map hover geometry C ready");
done:CryptReleaseContext(provider,0);restore_data_close();return 0;
}
