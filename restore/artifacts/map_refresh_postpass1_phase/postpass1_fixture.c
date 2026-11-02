#include "map_refresh_postpass1.h"
#include "city_prepare.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
#define CHECK(c) do{if(!(c)){fprintf(stderr,"postpass1 line%d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t p[18],events[100000][5],lengths[100000],used,abi[6];
static const uint32_t regions[11][2]={{0xa509ac,0xcb30},{0xa5d4bc,0xcb30},{0xacf84c,0xcb30},{0xae8e6c,0xcb30},{0xaf597c,0x32c60},{0xb285bc,0xcb30},{0xb67d0c,0xcb30},{0xb7481c,0x32c60},{0xbc9750,0xbe0},{0xf56c50,0x120},{0x56f560,0x40}};
static HCRYPTPROV provider;
static const uint8_t old_one[10]={0,0,0,0,0,0,0,0xc0,0xff,0x3f},old_two[10]={0,0,0,0,0,0,0,0x80,0,0xc0};
static uint8_t after_one[10],after_two[10];
static void append(uint32_t *e,uint32_t n){CHECK(used<100000&&n<=5);memcpy(events[used],e,n*4);lengths[used++]=n;}
uint64_t map_post1_read(uint32_t a,uint32_t n){uint64_t v=n==1?*(uint8_t *)(uintptr_t)a:n==4?*(uint32_t *)(uintptr_t)a:*(uint64_t *)(uintptr_t)a;uint32_t e[5]={0,a,n,(uint32_t)v,(uint32_t)(v>>32)};append(e,5);return v;}
void map_post1_write(uint32_t a,uint32_t n,uint32_t v){uint32_t e[5]={1,a,n,n==1?v&255u:v,0};append(e,5);if(n==1)*(uint8_t *)(uintptr_t)a=(uint8_t)v;else *(uint32_t *)(uintptr_t)a=v;}
void map_post1_fp(uint32_t a,MRFloat v){uint8_t raw[10];uint32_t e[5]={2,a,0,0,0};mr_fp_raw80(v,raw);memcpy(e+2,raw,8);e[4]=(uint32_t)raw[8]|((uint32_t)raw[9]<<8);append(e,5);}
void map_post1_bounds(int32_t x,int32_t y,uint32_t value){uint32_t e[4]={5,(uint32_t)x,(uint32_t)y,value};append(e,4);}
static void prepare(void)
{
    uint32_t i,index,base;int32_t dx,dy,nx,ny;DWORD previous;
    for(i=0;i<11;++i){CHECK(VirtualProtect((void *)(uintptr_t)regions[i][0],regions[i][1],PAGE_READWRITE,&previous));memset((void *)(uintptr_t)regions[i][0],(int)((p[14]+i*37u)&255u),regions[i][1]);}
    memset((void *)0xa509bc,(int)(p[11]&255u),0xcb10);memset((void *)0xa5d4cc,(int)(p[10]&255u),0xcb10);memset((void *)0xacf85c,(int)(p[12]&255u),0xcb10);memset((void *)0xaf598c,0,0x32c40);
    *(uint32_t *)0xbca2d0=p[1];*(uint32_t *)0xbca2d4=p[2];base=((228u-p[2])/2u)*228u+(228u-p[1])/2u;*(uint32_t *)0xbca2d8=base;*(uint32_t *)0xbca2dc=228u-p[1];FUN_0046f7b0();
    for(dy=-2;dy<=2;++dy)for(dx=-2;dx<=2;++dx){nx=(int32_t)p[16]+dx;ny=(int32_t)p[17]+dy;if(nx<0||ny<0||nx>=(int32_t)p[1]||ny>=(int32_t)p[2])continue;index=base+(uint32_t)ny*228u+(uint32_t)nx;*(uint32_t *)(uintptr_t)(0xaf598cu+index*4u)=p[15]?p[9]:0;}
    if(p[16]<p[1]&&p[17]<p[2]){index=base+p[17]*228u+p[16];*(uint32_t *)(uintptr_t)(0xaf598cu+index*4u)=p[8];}
    *(uint64_t *)0x56f588=UINT64_C(0x3f84afd6a052bf5b);*(uint64_t *)0x56f580=UINT64_C(0x4008000000000000);*(uint64_t *)0x56f570=UINT64_C(0x4058000000000000);*(uint64_t *)0x56f578=UINT64_C(0x4028000000000000);
    *(uint32_t *)0xf56c64=p[14];*(uint32_t *)0xf56d4c=p[14]+0x10000u;*(uint32_t *)0xf56cf4=p[14]+0x20000u;*(uint32_t *)0xf56cec=p[14]+0x30000u;used=0;
}
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) static void invoke(void)
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
        jne rectangle
        push dword ptr [p+28]
        push dword ptr [p+16]
        push dword ptr [p+12]
        mov ecx,0badc0deh
        call FUN_00479b70
        add esp,12
        jmp finished
rectangle:
        push dword ptr [p+24]
        push dword ptr [p+20]
        push dword ptr [p+16]
        push dword ptr [p+12]
        mov ecx,0badc0deh
        call FUN_00477500
        add esp,16
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
    HCRYPTHASH h;DWORD n=32;uint8_t bytes[32];uint32_t i;const char *digits="0123456789abcdef";CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));for(i=0;i<11;++i){CHECK(CryptHashData(h,(BYTE *)regions[i],8,0));CHECK(CryptHashData(h,(BYTE *)(uintptr_t)regions[i][0],regions[i][1],0));}CHECK(CryptGetHashParam(h,HP_HASHVAL,bytes,&n,0));CHECK(CryptDestroyHash(h));for(i=0;i<32;++i){hex[2*i]=digits[bytes[i]>>4];hex[2*i+1]=digits[bytes[i]&15];}hex[64]=0;
}
int main(int argc,char **argv)
{
    uint32_t i,j;uint16_t old_cw,wanted,after;char hex[65];SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){
        puts("{\"fields\":18}");fflush(stdout);
        for(;;){for(i=0;i<18;++i)if(scanf("%u",p+i)!=1)goto done;CHECK(p[0]<2&&p[1]<=228&&p[2]<=228);prepare();wanted=(uint16_t)p[13];
            __asm {fnstcw old_cw}
            __asm {fldcw wanted}
            __asm {fld tbyte ptr [old_one]}
            __asm {fld tbyte ptr [old_two]}
            invoke();
            __asm {fnstcw after}
            __asm {fstp tbyte ptr [after_two]}
            __asm {fstp tbyte ptr [after_one]}
            __asm {fldcw old_cw}
            CHECK(after==wanted&&!memcmp(old_one,after_one,10)&&!memcmp(old_two,after_two,10));CHECK(abi[0]==0x11111111&&abi[1]==0x22222222&&abi[2]==0x33333333&&abi[3]==0x44444444&&abi[4]==abi[5]);digest(hex);
            printf("{\"cw\":%u,\"memory\":\"%s\",\"events\":[",(uint32_t)after,hex);for(i=0;i<used;++i){printf("%s[",i?",":"");for(j=0;j<lengths[i];++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);
        }
    }else puts("complete first map postpass cluster ready");
done:CryptReleaseContext(provider,0);restore_data_close();return 0;
}
