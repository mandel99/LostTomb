#include "map_refresh_arithmetic.h"
#include "standalone_data.h"
#include "map_ring_fixture_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
#define CHECK(c) do{if(!(c)){fprintf(stderr,"weighted terrain line%d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t p[12],events[2048][5],lengths[2048],used,abi[6];
static const uint32_t regions[7][2]={{0xaf558c,0x33440},{0x70ba9c,0x500},{0x70c8dc,0x500},{0x5c9af0,0x90},{0x56f460,0x150},{0xbc9758,0x30},{0xbca2c0,0x30}};
static HCRYPTPROV provider;
static const uint8_t old_one[10]={0,0,0,0,0,0,0,0xc0,0xff,0x3f},old_two[10]={0,0,0,0,0,0,0,0x80,0,0xc0};
static uint8_t after_one[10],after_two[10];
static void append(uint32_t *e,uint32_t n){CHECK(used<2048&&n<=5);memcpy(events[used],e,n*4);lengths[used++]=n;}
uint64_t map_refresh_arithmetic_read(uint32_t a,uint32_t n)
{
    uint64_t v=n==1?*(uint8_t *)(uintptr_t)a:n==2?*(uint16_t *)(uintptr_t)a:n==4?*(uint32_t *)(uintptr_t)a:*(uint64_t *)(uintptr_t)a;
    uint32_t e[5]={0,a,n,(uint32_t)v,(uint32_t)(v>>32)};append(e,5);return v;
}
uint32_t map_refresh_read(uint32_t a,uint32_t n){return (uint32_t)map_refresh_arithmetic_read(a,n);}
void map_refresh_arithmetic_write(uint32_t a,uint32_t v){uint32_t e[5]={1,a,4,v,0};append(e,5);*(uint32_t *)(uintptr_t)a=v;}
void map_refresh_arithmetic_fp(uint32_t instruction,MRFloat v)
{
    uint8_t raw[10];uint32_t e[5]={2,instruction,0,0,0};mr_fp_raw80(v,raw);memcpy(e+2,raw,8);e[4]=(uint32_t)raw[8]|((uint32_t)raw[9]<<8);append(e,5);
}
void map_refresh_arithmetic_spill(uint32_t instruction,uint64_t bits){uint32_t e[4]={3,instruction,(uint32_t)bits,(uint32_t)(bits>>32)};append(e,4);}
void map_refresh_arithmetic_compare(uint32_t instruction,uint32_t c0){uint32_t e[3]={4,instruction,c0};append(e,3);}
static void prepare(void)
{
    uint32_t i,index,hash,flags;int32_t dx,dy,nx,ny;DWORD previous;
    for(i=0;i<7;++i){CHECK(VirtualProtect((void *)(uintptr_t)regions[i][0],regions[i][1],PAGE_READWRITE,&previous));memset((void *)(uintptr_t)regions[i][0],(int)((p[9]+i*37u)&255u),regions[i][1]);}
    memcpy((void *)0x70baac,mr_native_ring_xy,sizeof mr_native_ring_xy);memcpy((void *)0x70c8ec,mr_native_ring_linear,sizeof mr_native_ring_linear);
    memcpy((void *)0x5c9b00,mr_native_ring_counts,sizeof mr_native_ring_counts);memcpy((void *)0x5c9b54,mr_native_ring_offsets,sizeof mr_native_ring_offsets);
    *(uint64_t *)0x56f470=UINT64_C(0x4059000000000000);*(uint64_t *)0x56f520=UINT64_C(0x3ff0000000000000);*(uint64_t *)0x56f5a0=0;
    *(uint32_t *)0xbca2d0=p[5];*(uint32_t *)0xbca2d4=p[6];*(uint32_t *)0xbca2d8=p[7];
    for(dy=-4;dy<=4;++dy)for(dx=-4;dx<=4;++dx){
        nx=(int32_t)p[1]+dx;ny=(int32_t)p[2]+dy;
        if(nx< -1||ny< -1||nx>(int32_t)p[5]||ny>(int32_t)p[6])continue;
        index=p[7]+(uint32_t)nx+(uint32_t)ny*228u;hash=(index^(p[9]*0x9e3779b9u))*1664525u+1013904223u;flags=hash&~0x800u;
        switch(p[8]){case 0:break;case 1:flags|=0x800u;break;case 2:if(((uint32_t)nx+(uint32_t)ny)&1u)flags|=0x800u;break;case 3:if((uint32_t)nx&1u)flags|=0x800u;break;case 4:if(hash&0x10000u)flags|=0x800u;break;case 5:if((hash>>16)%100u<p[11])flags|=0x800u;break;default:CHECK(0);}
        CHECK(0xaf598cu+index*4u>=regions[0][0]&&0xaf598cu+index*4u+4u<=regions[0][0]+regions[0][1]);*(uint32_t *)(uintptr_t)(0xaf598cu+index*4u)=flags;
    }
    used=0;
}
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) static int32_t invoke(void)
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
        jne wrapper
        push dword ptr [p+16]
        push dword ptr [p+12]
        push dword ptr [p+8]
        push dword ptr [p+4]
        mov ecx,0badc0deh
        call FUN_00481540
        add esp,16
        jmp finished
wrapper:
        push dword ptr [p+12]
        push dword ptr [p+8]
        push dword ptr [p+4]
        mov ecx,0badc0deh
        call FUN_00481690
        add esp,12
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
    HCRYPTHASH h;DWORD n=32;uint8_t bytes[32];uint32_t i;const char *digits="0123456789abcdef";
    CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));for(i=0;i<7;++i){CHECK(CryptHashData(h,(BYTE *)regions[i],8,0));CHECK(CryptHashData(h,(BYTE *)(uintptr_t)regions[i][0],regions[i][1],0));}
    CHECK(CryptGetHashParam(h,HP_HASHVAL,bytes,&n,0));CHECK(CryptDestroyHash(h));for(i=0;i<32;++i){hex[i*2]=digits[bytes[i]>>4];hex[i*2+1]=digits[bytes[i]&15];}hex[64]=0;
}
int main(int argc,char **argv)
{
    uint32_t i,j,result;uint16_t old_cw,wanted,after;char hex[65];
    SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){
        puts("{\"fields\":12}");fflush(stdout);
        for(;;){for(i=0;i<12;++i)if(scanf("%u",p+i)!=1)goto done;CHECK(p[0]<2&&p[3]<=4&&p[5]<=228&&p[6]<=228&&p[1]<=p[5]&&p[2]<=p[6]);prepare();wanted=(uint16_t)p[10];
            __asm {fnstcw old_cw}
            __asm {fldcw wanted}
            __asm {fld tbyte ptr [old_one]}
            __asm {fld tbyte ptr [old_two]}
            result=(uint32_t)invoke();
            __asm {fnstcw after}
            __asm {fstp tbyte ptr [after_two]}
            __asm {fstp tbyte ptr [after_one]}
            __asm {fldcw old_cw}
            CHECK(after==wanted&&memcmp(old_one,after_one,10)==0&&memcmp(old_two,after_two,10)==0);CHECK(abi[0]==0x11111111&&abi[1]==0x22222222&&abi[2]==0x33333333&&abi[3]==0x44444444&&abi[4]==abi[5]);digest(hex);
            printf("{\"result\":%u,\"cw\":%u,\"memory\":\"%s\",\"events\":[",result,(uint32_t)after,hex);for(i=0;i<used;++i){printf("%s[",i?",":"");for(j=0;j<lengths[i];++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);
        }
    }else puts("weighted terrain full native/C fixture ready");
done:CryptReleaseContext(provider,0);restore_data_close();return 0;
}
