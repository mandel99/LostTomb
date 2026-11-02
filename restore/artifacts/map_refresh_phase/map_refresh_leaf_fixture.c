#include "map_refresh.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
#define CHECK(c) do{if(!(c)){fprintf(stderr,"map refresh leaf line%d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t p[10],events[8][3],used;
static const uint32_t regions[3][2]={{0x5c9b98,0xa00},{0x70be2c,0x180},{0xbca2c0,48}};
static HCRYPTPROV provider;
uint32_t map_refresh_read(uint32_t address,uint32_t width)
{
    uint32_t value=width==1?*(uint8_t *)(uintptr_t)address:*(uint32_t *)(uintptr_t)address;
    CHECK(used<8);events[used][0]=address;events[used][1]=width;events[used++][2]=value;return value;
}
/* Independent test preparation matrix, never an expected-result dispatcher. */
static uint32_t selected_address(void)
{
    static const uint32_t t[36]={0x70bf7c,0x70bf5c,0x70bf2c,0x70beec,0x70be9c,0x70be3c,0x5c9ba8,0x5c9bc0,0x5c9be8,0x5c9c20,0x5c9c68,0x5c9cc0,0x5c9d28,0x5c9d48,0x5c9d78,0x5c9db8,0x5c9e08,0x5c9e68,0x5c9ed8,0x5c9f00,0x5c9f38,0x5c9f80,0x5c9fd8,0x5ca040,0x5ca0b8,0x5ca0e8,0x5ca128,0x5ca178,0x5ca1d8,0x5ca248,0x5ca2c8,0x5ca300,0x5ca348,0x5ca3a0,0x5ca408,0x5ca480};
    uint32_t row=p[2]-1u,col;
    if(row<5){col=p[3]-1u;if(col>=5)col=5;}else{row=5;col=p[3];if(col>=5)col=5;}
    return t[row*6+col]+p[4]*2u;
}
static void prepare(void)
{
    uint32_t i,j,a;DWORD previous;
    for(i=0;i<3;++i){CHECK(VirtualProtect((void *)(uintptr_t)regions[i][0],regions[i][1],PAGE_READWRITE,&previous));for(j=0;j<regions[i][1];++j)*(uint8_t *)(uintptr_t)(regions[i][0]+j)=(uint8_t)((p[7]+j*13u+i*37u)%17u-8u);}
    *(uint32_t *)0xbca2d0=p[5];*(uint32_t *)0xbca2d4=p[6];
    if(p[8]){a=selected_address();*(uint8_t *)(uintptr_t)a=(uint8_t)p[9];*(uint8_t *)(uintptr_t)(a+1u)=(uint8_t)(p[9]>>8);}
    used=0;
}
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) static uint32_t invoke(uint32_t out[6])
{
    __asm {
        push ebp
        push ebx
        push esi
        push edi
        mov ecx,[esp+20]
        mov [ecx+20],esp
        mov ebx,011111111h
        mov ebp,022222222h
        mov esi,033333333h
        mov edi,044444444h
        push dword ptr [p+16]
        push dword ptr [p+12]
        push dword ptr [p+8]
        push dword ptr [p+4]
        push dword ptr [p]
        mov ecx,0badc0deh
        mov edx,0fefefefeh
        call FUN_00481aa0
        add esp,20
        mov ecx,[esp+20]
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
static void digest(char hex[65])
{
    HCRYPTHASH h;DWORD n=32;uint8_t bytes[32];uint32_t i;const char *digits="0123456789abcdef";
    CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));
    for(i=0;i<3;++i){CHECK(CryptHashData(h,(BYTE *)regions[i],8,0));CHECK(CryptHashData(h,(BYTE *)(uintptr_t)regions[i][0],regions[i][1],0));}
    CHECK(CryptGetHashParam(h,HP_HASHVAL,bytes,&n,0));CHECK(CryptDestroyHash(h));
    for(i=0;i<32;++i){hex[i*2]=digits[bytes[i]>>4];hex[i*2+1]=digits[bytes[i]&15];}hex[64]=0;
}
int main(int argc,char **argv)
{
    uint32_t i,result,out[6];char hex[65];
    SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){
        puts("{\"fields\":10}");fflush(stdout);
        for(;;){for(i=0;i<10;++i)if(scanf("%u",p+i)!=1)goto done;CHECK(p[4]<68);prepare();result=invoke(out);CHECK(out[0]==0x11111111&&out[1]==0x22222222&&out[2]==0x33333333&&out[3]==0x44444444&&out[4]==out[5]);digest(hex);
            printf("{\"result\":%u,\"memory\":\"%s\",\"reads\":[",result,hex);
            for(i=0;i<used;++i)printf("%s[%u,%u,%u]",i?",":"",events[i][0],events[i][1],events[i][2]);puts("]}");fflush(stdout);
        }
    }else puts("map refresh boundary leaf fixture ready");
done:CryptReleaseContext(provider,0);restore_data_close();return 0;
}
