#include "map_refresh_postpass2.h"
#include "selector_native_data.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
#define CHECK(c) do{if(!(c)){fprintf(stderr,"selector leaf line%d\n",__LINE__);ExitProcess(1);}}while(0)
#define TABLE 0x6f2000u
static uint32_t p[16],events[2048][4],used,abi[6];
static const uint32_t regions[3][2]={{TABLE-16,0x1820},{0xbc9730,0xc0},{0xbca2d0,0x40}};
static HCRYPTPROV provider;
static const uint8_t old_one[10]={0,0,0,0,0,0,0,0xc0,0xff,0x3f},old_two[10]={0,0,0,0,0,0,0,0x80,0,0xc0};
static uint8_t after_one[10],after_two[10];
uint32_t map_post2_read(uint32_t a,uint32_t n){uint32_t v=n==1?*(uint8_t *)(uintptr_t)a:*(uint32_t *)(uintptr_t)a;CHECK(used<2048);events[used][0]=0;events[used][1]=a;events[used][2]=n;events[used++][3]=v;return v;}
void map_post2_write(uint32_t a,uint32_t n,uint32_t v){CHECK(used<2048);events[used][0]=1;events[used][1]=a;events[used][2]=n;events[used++][3]=n==1?v&255u:v;if(n==1)*(uint8_t *)(uintptr_t)a=(uint8_t)v;else *(uint32_t *)(uintptr_t)a=v;}
static uint32_t context(uint32_t i){return ((p[12]>>i)&1u)|(((p[13]>>i)&1u)<<1)|(p[14]&~3u);}
static uint8_t truth(uint32_t i){return p[0]==1?(uint8_t)(context(i)&1u):(uint8_t)(context(i)!=0);}
static void prepare(void)
{
    uint32_t i,j,g,row,n,a;uint8_t *record;DWORD previous;
    for(i=0;i<3;++i){CHECK(VirtualProtect((void *)(uintptr_t)regions[i][0],regions[i][1],PAGE_READWRITE,&previous));memset((void *)(uintptr_t)regions[i][0],(int)((p[15]+i*37u)&255u),regions[i][1]);}
    for(i=0;i<8;++i)*(uint32_t *)(uintptr_t)(0xbc97c0u+i*4u)=context(i);
    *(uint32_t *)0xbca2e0=p[5];
    if(p[1]){
        const uint8_t *data=p[1]==1?post2_main_records:post2_secondary_records;uint32_t bytes=p[1]==1?sizeof post2_main_records:sizeof post2_secondary_records;
        memcpy((void *)TABLE,data,bytes);for(i=0;i<bytes/16u;++i)*(uint8_t *)(uintptr_t)(TABLE+i*16u+14u)=(uint8_t)p[9];
    }else{
        n=(int32_t)p[2]>0?p[2]:0;CHECK(n<=48);
        for(g=0;g<8;++g)for(row=0;row<n;++row){
            a=TABLE+(g*n+row)*16u;record=(uint8_t *)(uintptr_t)a;
            for(i=0;i<8;++i){if(p[7]==0)record[i]=2;else if(p[7]==2&&i%3u==0)record[i]=2;else record[i]=truth(i)?(p[7]==3?0x80u:1u):0;}
            if(row!=p[6]){i=row%8u;record[i]=truth(i)?0:1;}
            for(j=0;j<4;++j)record[8u+j]=(uint8_t)(p[10]+g*17u+row*3u+j*37u);
            record[12]=(uint8_t)(p[11]+g*11u+row);record[13]=(uint8_t)p[8];record[14]=(uint8_t)p[9];record[15]=(uint8_t)(0xa5u^g^row);
        }
    }
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
        mov eax,dword ptr [p]
        test eax,eax
        jnz not_first
        push dword ptr [p+16]
        push dword ptr [p+12]
        push dword ptr [p+8]
        push TABLE
        mov ecx,0badc0deh
        call FUN_00484be0
        add esp,16
        jmp finished
not_first:
        cmp eax,1
        jne invert
        push dword ptr [p+8]
        push TABLE
        mov ecx,0badc0deh
        call FUN_00484d70
        add esp,8
        jmp finished
invert:
        call FUN_00484f00
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
    HCRYPTHASH h;DWORD n=32;uint8_t bytes[32];uint32_t i;const char *digits="0123456789abcdef";CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));for(i=0;i<3;++i){CHECK(CryptHashData(h,(BYTE *)regions[i],8,0));CHECK(CryptHashData(h,(BYTE *)(uintptr_t)regions[i][0],regions[i][1],0));}CHECK(CryptGetHashParam(h,HP_HASHVAL,bytes,&n,0));CHECK(CryptDestroyHash(h));for(i=0;i<32;++i){hex[2*i]=digits[bytes[i]>>4];hex[2*i+1]=digits[bytes[i]&15];}hex[64]=0;
}
int main(int argc,char **argv)
{
    uint32_t i,result;uint16_t old_cw,wanted=0x37f,after_cw;char hex[65];SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){
        puts("{\"fields\":16}");fflush(stdout);
        for(;;){for(i=0;i<16;++i)if(scanf("%u",p+i)!=1)goto done;CHECK(p[0]<3&&p[1]<3&&(int32_t)p[2]<=48);prepare();
            __asm {fnstcw old_cw}
            __asm {fldcw wanted}
            __asm {fld tbyte ptr [old_one]}
            __asm {fld tbyte ptr [old_two]}
            result=invoke();
            __asm {fnstcw after_cw}
            __asm {fstp tbyte ptr [after_two]}
            __asm {fstp tbyte ptr [after_one]}
            __asm {fldcw old_cw}
            CHECK(after_cw==wanted&&!memcmp(old_one,after_one,10)&&!memcmp(old_two,after_two,10));CHECK(abi[0]==0x11111111&&abi[1]==0x22222222&&abi[2]==0x33333333&&abi[3]==0x44444444&&abi[4]==abi[5]);if(p[0]==2)result=0;digest(hex);
            printf("{\"result\":%u,\"cw\":%u,\"memory\":\"%s\",\"events\":[",result,(uint32_t)after_cw,hex);for(i=0;i<used;++i)printf("%s[%u,%u,%u,%u]",i?",":"",events[i][0],events[i][1],events[i][2],events[i][3]);puts("]}");fflush(stdout);
        }
    }else puts("complete map selector leaves ready");
done:CryptReleaseContext(provider,0);restore_data_close();return 0;
}
