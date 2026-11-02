#include "map_refresh_postpass4.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
#define CHECK(c) do{if(!(c)){fprintf(stderr,"postpass4 line%d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t p[26],events[2048][4],used,abi[6];
static uint32_t regions[8][2]={{0xaf557c,0x33460},{0xb4e4ec,0x19a40},{0xbc9730,0xc0},{0x6f5ff0,0x30}};
static HCRYPTPROV provider;
static const uint8_t old_one[10]={0,0,0,0,0,0,0,0xc0,0xff,0x3f},old_two[10]={0,0,0,0,0,0,0,0x80,0,0xc0};
static uint8_t after_one[10],after_two[10];
uint32_t map_post4_read(uint32_t a,uint32_t n){uint32_t v=n==1?*(uint8_t *)(uintptr_t)a:n==2?*(uint16_t *)(uintptr_t)a:*(uint32_t *)(uintptr_t)a;CHECK(used<2048);events[used][0]=0;events[used][1]=a;events[used][2]=n;events[used++][3]=v;return v;}
void map_post4_write(uint32_t a,uint32_t n,uint32_t v){CHECK(used<2048);events[used][0]=1;events[used][1]=a;events[used][2]=n;events[used++][3]=n==1?v&255u:n==2?v&65535u:v;if(n==1)*(uint8_t *)(uintptr_t)a=(uint8_t)v;else if(n==2)*(uint16_t *)(uintptr_t)a=(uint16_t)v;else *(uint32_t *)(uintptr_t)a=v;}
static void prepare(void)
{
    static const int32_t offsets[8]={-228,-227,1,229,228,227,-1,-229};
    uint32_t i,a,id;DWORD previous;
    for(i=0;i<4;++i){id=(uint32_t)(int32_t)(int16_t)p[22]+i*4u;CHECK((int32_t)id>=-32768&&(int32_t)id<=32767);regions[4+i][0]=0x7999d8u+id*264u;regions[4+i][1]=0x128;}
    for(i=0;i<8;++i){CHECK(VirtualProtect((void *)(uintptr_t)regions[i][0],regions[i][1],PAGE_READWRITE,&previous));memset((void *)(uintptr_t)regions[i][0],(int)((p[23]+i*37u)&255u),regions[i][1]);}
    *(uint32_t *)0xbc9768=p[1];*(uint32_t *)0x6f6000=p[3];
    for(i=0;i<8;++i){*(uint32_t *)(uintptr_t)(0xbc97c0u+i*4u)=p[24]^(i*0x11335577u);*(uint32_t *)(uintptr_t)(0xaf598cu+(p[1]+(uint32_t)offsets[i])*4u)=p[5+i];}
    *(uint32_t *)(uintptr_t)(0xaf598cu+p[1]*4u)=p[4];
    for(i=0;i<4;++i){id=(uint32_t)(int32_t)(int16_t)p[22]+i*4u;a=0x7999e8u+id*264u;*(uint16_t *)(uintptr_t)(0xb4e6fcu+(p[1]+(uint32_t)offsets[i*2u])*2u)=(uint16_t)id;*(uint16_t *)(uintptr_t)a=(uint16_t)p[13+i];*(uint16_t *)(uintptr_t)(a+2u)=(uint16_t)p[17+i];*(uint8_t *)(uintptr_t)(a+154u)=(uint8_t)(p[21]>>(i*8u));}
    used=0;
}
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) static uint32_t invoke(void)
{
    __asm {
        push ebp
        push ebx
        push esi
        push edi
        mov dword ptr [abi+20],esp
        mov ebx,011111111h
        mov ebp,022222222h
        mov esi,033333333h
        mov edi,044444444h
        cmp dword ptr [p],0
        jne not_first
        call FUN_0047e5c0
        jmp finished
not_first:
        cmp dword ptr [p],1
        jne combined
        push 06f6000h
        push dword ptr [p+8]
        call FUN_0047eb60
        add esp,8
        jmp finished
combined:
        call FUN_0047e870
finished:
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
#pragma warning(pop)
static void digest(char hex[65])
{
    HCRYPTHASH h;DWORD n=32;uint8_t bytes[32];uint32_t i;const char *digits="0123456789abcdef";CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));for(i=0;i<8;++i){CHECK(CryptHashData(h,(BYTE *)regions[i],8,0));CHECK(CryptHashData(h,(BYTE *)(uintptr_t)regions[i][0],regions[i][1],0));}CHECK(CryptGetHashParam(h,HP_HASHVAL,bytes,&n,0));CHECK(CryptDestroyHash(h));for(i=0;i<32;++i){hex[2*i]=digits[bytes[i]>>4];hex[2*i+1]=digits[bytes[i]&15];}hex[64]=0;
}
int main(int argc,char **argv)
{
    uint32_t i;uint16_t old_cw,wanted=0x37f,after_cw;char hex[65];SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){
        puts("{\"fields\":26}");fflush(stdout);
        for(;;){for(i=0;i<26;++i)if(scanf("%u",p+i)!=1)goto done;CHECK(p[0]<3&&p[1]<51984&&p[2]<8);prepare();wanted=(uint16_t)p[25];
            __asm {fnstcw old_cw}
            __asm {fldcw wanted}
            __asm {fld tbyte ptr [old_one]}
            __asm {fld tbyte ptr [old_two]}
            invoke();
            __asm {fnstcw after_cw}
            __asm {fstp tbyte ptr [after_two]}
            __asm {fstp tbyte ptr [after_one]}
            __asm {fldcw old_cw}
            CHECK(after_cw==wanted&&!memcmp(old_one,after_one,10)&&!memcmp(old_two,after_two,10));CHECK(abi[0]==0x11111111&&abi[1]==0x22222222&&abi[2]==0x33333333&&abi[3]==0x44444444&&abi[4]==abi[5]);digest(hex);
            printf("{\"cw\":%u,\"memory\":\"%s\",\"events\":[",(uint32_t)after_cw,hex);for(i=0;i<used;++i)printf("%s[%u,%u,%u,%u]",i?",":"",events[i][0],events[i][1],events[i][2],events[i][3]);puts("]}");fflush(stdout);
        }
    }else puts("complete fourth map postpass cluster ready");
done:CryptReleaseContext(provider,0);restore_data_close();return 0;
}
