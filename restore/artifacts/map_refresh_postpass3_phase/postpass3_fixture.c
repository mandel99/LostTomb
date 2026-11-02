#include "map_refresh_postpass3.h"
#include "selector_native_data.h"
#include "city_prepare.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
#define CHECK(c) do{if(!(c)){fprintf(stderr,"postpass3 line%d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t p[26],words[500000],offsets[100000],lengths[100000],used,total,abi[6];
static const uint32_t regions[15][2]={{0xa509ac,0xcb30},{0xa5d4bc,0xcb30},{0xacf84c,0xcb30},{0xae8e6c,0xcb30},{0xaf557c,0x33460},{0xb285bc,0xcb30},{0xb67d0c,0xcb30},{0xb7481c,0x32c60},{0xbc9730,0xc00},{0xf56c24,0x630},{0x56f580,0x30},{0xd05710,0x32c60},{0x5cac48,0xc20},{0x6f4000,0x1000},{0x78d79c,0x30}};
static HCRYPTPROV provider;
static const uint8_t old_one[10]={0,0,0,0,0,0,0,0xc0,0xff,0x3f},old_two[10]={0,0,0,0,0,0,0,0x80,0,0xc0};
static uint8_t after_one[10],after_two[10];
static void append(uint32_t *e,uint32_t n){CHECK(used<100000&&total+n<=500000);offsets[used]=total;memcpy(words+total,e,n*4);lengths[used++]=n;total+=n;}
uint64_t map_post3_read(uint32_t a,uint32_t n){uint64_t v=n==1?*(uint8_t *)(uintptr_t)a:n==4?*(uint32_t *)(uintptr_t)a:*(uint64_t *)(uintptr_t)a;uint32_t e[5]={0,a,n,(uint32_t)v,(uint32_t)(v>>32)};append(e,5);return v;}
void map_post3_write(uint32_t a,uint32_t n,uint32_t v){uint32_t e[5]={1,a,n,n==1?v&255u:v,0};append(e,5);if(n==1)*(uint8_t *)(uintptr_t)a=(uint8_t)v;else *(uint32_t *)(uintptr_t)a=v;}
void map_post3_fp(uint32_t a,MRFloat v){uint8_t raw[10];uint32_t e[5]={2,a,0,0,0};mr_fp_raw80(v,raw);memcpy(e+2,raw,8);e[4]=(uint32_t)raw[8]|((uint32_t)raw[9]<<8);append(e,5);}
void map_post3_bounds(int32_t x,int32_t y,uint32_t v){uint32_t e[4]={4,(uint32_t)x,(uint32_t)y,v};append(e,4);}
void map_post3_water(int32_t x,int32_t y,uint32_t v){uint32_t e[4]={5,(uint32_t)x,(uint32_t)y,*(int32_t *)0x78d7b0>0?v:v&255u};append(e,4);}
void map_post3_helper(uint32_t a,uint32_t phase,uint32_t arg){uint32_t i,e[13]={3,a,phase,arg,*(uint32_t *)0xbc9768};for(i=0;i<8;++i)e[5+i]=*(uint32_t *)(uintptr_t)(0xbc97c0u+i*4u);append(e,13);}
uint32_t map_post2_read(uint32_t a,uint32_t n){return (uint32_t)map_post3_read(a,n);}
void map_post2_write(uint32_t a,uint32_t n,uint32_t v){map_post3_write(a,n,v);}
uint32_t map_neighbors_read(uint32_t a,uint32_t n){return (uint32_t)map_post3_read(a,n);}
void map_neighbors_write(uint32_t a,uint32_t v){map_post3_write(a,4,v);}
static void prepare(void)
{
    uint32_t i,index,base,anchor;int32_t dx,dy,nx,ny;DWORD previous;static const int32_t neighbors[8]={-228,-227,1,229,228,227,-1,-229};
    for(i=0;i<15;++i){CHECK(VirtualProtect((void *)(uintptr_t)regions[i][0],regions[i][1],PAGE_READWRITE,&previous));memset((void *)(uintptr_t)regions[i][0],(int)((p[14]+i*37u)&255u),regions[i][1]);}
    memset((void *)0xa5d4cc,(int)(p[10]&255u),0xcb10);memset((void *)0xacf85c,(int)(p[11]&255u),0xcb10);memset((void *)0xaf598c,0,0x32c40);memset((void *)0xd05720,0,0x32c40);
    *(uint32_t *)0xbca2d0=p[1];*(uint32_t *)0xbca2d4=p[2];base=((228u-p[2])/2u)*228u+(228u-p[1])/2u;*(uint32_t *)0xbca2d8=base;*(uint32_t *)0xbca2dc=228u-p[1];FUN_0046f7b0();*(uint32_t *)0xbca2e0=p[18];
    anchor=(base+p[25])%51984u;*(uint32_t *)0x6f4000=0x6f4800;*(uint32_t *)0x6f4008=anchor;*(uint8_t *)0x6f4018=p[12]==1?0:p[12]==3?128:1;*(uint32_t *)0x6f4804=p[21];
    for(dy=-2;dy<=2;++dy)for(dx=-2;dx<=2;++dx){nx=(int32_t)p[16]+dx;ny=(int32_t)p[17]+dy;if(nx<0||ny<0||nx>=(int32_t)p[1]||ny>=(int32_t)p[2])continue;index=base+(uint32_t)ny*228u+(uint32_t)nx;*(uint32_t *)(uintptr_t)(0xaf598cu+index*4u)=p[15]==1?p[9]:0;*(uint32_t *)(uintptr_t)(0xd05720u+index*4u)=p[12]?0x6f4000:0;}
    if(p[16]<p[1]&&p[17]<p[2]){index=base+p[17]*228u+p[16];if(p[15]==2)for(i=0;i<8;++i)*(uint32_t *)(uintptr_t)(0xaf598cu+(index+(uint32_t)neighbors[i])*4u)=((p[24]>>i)&1u)?p[9]:0;*(uint32_t *)(uintptr_t)(0xaf598cu+index*4u)=p[8];}
    memcpy((void *)0x5cac58,post2_main_records,sizeof post2_main_records);for(i=0;i<192;++i){*(uint8_t *)(uintptr_t)(0x5cac58u+i*16u+13u)=(uint8_t)p[22];*(uint8_t *)(uintptr_t)(0x5cac58u+i*16u+14u)=(uint8_t)p[23];}
    *(uint64_t *)0x56f598=UINT64_C(0x3f847ae147ae147b);*(uint64_t *)0x56f590=UINT64_C(0x4020000000000000);*(uint32_t *)0xf56c34=p[14];*(uint32_t *)0xf57234=p[14]+0x10000u;*(uint32_t *)0x78d7ac=p[19];*(uint32_t *)0x78d7b0=p[20];used=total=0;
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
        call FUN_00479e70
        add esp,12
        jmp finished
rectangle:
        push dword ptr [p+24]
        push dword ptr [p+20]
        push dword ptr [p+16]
        push dword ptr [p+12]
        mov ecx,0badc0deh
        call FUN_004777c0
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
    HCRYPTHASH h;DWORD n=32;uint8_t bytes[32];uint32_t i;const char *digits="0123456789abcdef";CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));for(i=0;i<15;++i){CHECK(CryptHashData(h,(BYTE *)regions[i],8,0));CHECK(CryptHashData(h,(BYTE *)(uintptr_t)regions[i][0],regions[i][1],0));}CHECK(CryptGetHashParam(h,HP_HASHVAL,bytes,&n,0));CHECK(CryptDestroyHash(h));for(i=0;i<32;++i){hex[2*i]=digits[bytes[i]>>4];hex[2*i+1]=digits[bytes[i]&15];}hex[64]=0;
}
int main(int argc,char **argv)
{
    uint32_t i,j;uint16_t old_cw,wanted,after;char hex[65];SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){
        puts("{\"fields\":26}");fflush(stdout);
        for(;;){for(i=0;i<26;++i)if(scanf("%u",p+i)!=1)goto done;CHECK(p[0]<2&&p[1]<=228&&p[2]<=228);prepare();wanted=(uint16_t)p[13];
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
            printf("{\"cw\":%u,\"memory\":\"%s\",\"events\":[",(uint32_t)after,hex);for(i=0;i<used;++i){printf("%s[",i?",":"");for(j=0;j<lengths[i];++j)printf("%s%u",j?",":"",words[offsets[i]+j]);putchar(']');}puts("]}");fflush(stdout);
        }
    }else puts("complete third map postpass cluster ready");
done:CryptReleaseContext(provider,0);restore_data_close();return 0;
}
