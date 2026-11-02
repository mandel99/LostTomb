#include "raw_tile_raster.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
#define U(a) (*(uint32_t *)(uintptr_t)(uint32_t)(a))
#define BASE 0x30000000u
#define CAP 65536u
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Raw tile check failed at%d\n",__LINE__);ExitProcess(1);}}while(0)
static const uint32_t regions[][2]={{BASE,CAP},{0xd3b5d0,32},{0xe38d90,16},{0xe92e10,16}};
static HCRYPTPROV provider;
static void run(const uint32_t p[6])
{
    uint32_t i,state=p[5];
    CHECK(p[0]+512u<=CAP);
    for(i=0;i<CAP;++i){state=state*1664525u+1013904223u;*(uint8_t *)(BASE+i)=(uint8_t)(state>>24);}
    for(i=1;i<4;++i)memset((void *)(uintptr_t)regions[i][0],0x5a,regions[i][1]);
    U(0xe92e18)=BASE+p[1];U(0xd3b5e0)=p[3];U(0xd3b5dc)=p[4];U(0xe38d94)=p[2];
    FUN_00410fb0((const void *)(BASE+p[0]));
}
static void digest(char hex[65])
{
    HCRYPTHASH hash;DWORD length=32;uint8_t bytes[32];uint32_t i;const char *digits="0123456789abcdef";
    CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&hash));
    for(i=0;i<4;++i)CHECK(CryptHashData(hash,(const BYTE *)(uintptr_t)regions[i][0],regions[i][1],0));
    CHECK(CryptGetHashParam(hash,HP_HASHVAL,bytes,&length,0));CHECK(CryptDestroyHash(hash));
    for(i=0;i<32;++i){hex[i*2]=digits[bytes[i]>>4];hex[i*2+1]=digits[bytes[i]&15];}hex[64]=0;
}
int main(int argc,char **argv)
{
    uint32_t p[6]={4096,8192,1024,0,0,123},i;char hex[65];
    SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);
    CHECK(restore_data_open());CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
    CHECK(VirtualAlloc((void *)BASE,CAP,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){
        puts("{\"fields\":6}");fflush(stdout);
        for(;;){for(i=0;i<6;++i)if(scanf("%u",p+i)!=1)goto done;run(p);digest(hex);printf("{\"memory\":\"%s\"}\n",hex);fflush(stdout);}
    }else{
        run(p);for(i=0;i<16;++i)CHECK(!memcmp((void *)(BASE+4096+i*32),(void *)(BASE+8192+i*1024),32));
        CHECK(U(0xe38d94)==1024);puts("Opaque16x16 raster: all16 rows, full framebuffer address and pitch.");
    }
done:CryptReleaseContext(provider,0);restore_data_close();return 0;
}
