#include "map_refresh_postpass8.h"
#include "standalone_data.h"
#include "postpass8_records.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
#define CHECK(c) do{if(!(c)){fprintf(stderr,"postpass8 line%d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t p[20],words[500000],offsets[100000],lengths[100000],used,total,abi[7];
static const uint32_t regions[8][2]={{0xacf640,0xd8700},{0xbc9730,0xc00},{0xd04f10,0x33c60},{0x6f4000,0x400},{0x78d79c,0x30},{0xf56b40,0x810},{0x5ca648,0x320},{0x5cbfa8,0x1c0}};
static HCRYPTPROV provider;
static const uint8_t old_one[10]={0,0,0,0,0,0,0,0xc0,0xff,0x3f},old_two[10]={0,0,0,0,0,0,0,0x80,0,0xc0};
static uint8_t after_one[10],after_two[10];
static void append(uint32_t *e,uint32_t n){CHECK(used<100000&&total+n<=500000);offsets[used]=total;memcpy(words+total,e,n*4);lengths[used++]=n;total+=n;}
uint32_t map_post8_read(uint32_t a,uint32_t n){uint32_t v=n==1?*(uint8_t *)(uintptr_t)a:*(uint32_t *)(uintptr_t)a,e[5]={0,a,n,v,0};append(e,5);return v;}
void map_post8_write(uint32_t a,uint32_t n,uint32_t v){uint32_t e[5]={1,a,n,v& (n==1?255u:0xffffffffu),0};append(e,5);if(n==1)*(uint8_t *)(uintptr_t)a=(uint8_t)v;else *(uint32_t *)(uintptr_t)a=v;}
uint32_t map_post2_read(uint32_t a,uint32_t n){return map_post8_read(a,n);}
void map_post2_write(uint32_t a,uint32_t n,uint32_t v){map_post8_write(a,n,v);}
uint32_t map_neighbors_read(uint32_t a,uint32_t n){return map_post8_read(a,n);}
void map_neighbors_write(uint32_t a,uint32_t v){map_post8_write(a,4,v);}
void map_post8_helper(uint32_t a,uint32_t phase,uint32_t x,uint32_t y,uint32_t result){uint32_t i,e[15]={2,a,phase,x,y,result,*(uint32_t *)0xbc9768};for(i=0;i<8;++i)e[7+i]=*(uint32_t *)(uintptr_t)(0xbc97c0u+4u*i);append(e,15);}
static void put(uint32_t a,uint32_t v){*(uint32_t *)(uintptr_t)a=v;}
static void prepare(void)
{
    uint32_t i,base,tile,value,address;int32_t left;DWORD previous;static const int32_t around[8]={-228,-227,1,229,228,227,-1,-229};
    static const uint32_t graphics[11]={0xf5722c,0xf56bd4,0xf56c34,0xf57234,0xf57264,0xf56c9c,0xf57224,0xf57324,0xf56d24,0xf5721c,0xf56b64};
    for(i=0;i<8;++i){CHECK(VirtualProtect((void *)(uintptr_t)regions[i][0],regions[i][1],PAGE_READWRITE,&previous));memset((void *)(uintptr_t)regions[i][0],(int)((p[16]+37u*i)&255u),regions[i][1]);}
    memset((void *)0xaf518c,0,53008u*4u);memset((void *)0xd04f20,0,53008u*4u);
    memcpy((void *)0x5ca658,primary_records,sizeof(primary_records));memcpy((void *)0x5cbfb8,secondary_records,sizeof(secondary_records));
    /* Counter seeds are actual recordBYTE fields; animation>=0 does not use primary cycles. */
    for(i=0;i<26;++i)*(uint8_t *)(uintptr_t)(0x5cbfb8u+16u*i+14u)=(uint8_t)(p[16]+i);
    put(0xbca2d0,p[0]);put(0xbca2d4,p[1]);FUN_0046f7b0();
    base=((228u-p[1])/2u)*228u+(228u-p[0])/2u;put(0xbca2d8,base);put(0xbca2dc,228u-p[0]);put(0xbca2e0,p[5]);put(0xbc9768,p[17]);
    for(i=0;i<8;++i)put(0xbc97c0u+4u*i,p[16]^(i*0x11335577u));
    tile=base+p[3]*228u+p[2];
    for(i=0;i<8;++i){value=p[18]|((p[6]>>i&1u)?4u:0u)|((p[7]>>i&1u)?0x10000u:0u)|((p[8]>>i&1u)?0x100u:0u)|((p[9]>>i&1u)?0x4000000u:0u);put(0xaf598cu+(tile+(uint32_t)around[i])*4u,value);address=0x6f4000u+i*64u;*(uint8_t *)(uintptr_t)(address+24u)=(uint8_t)((p[11]>>i&1u)?0:0x7f);put(0xd05720u+(tile+(uint32_t)around[i])*4u,(p[12]>>i&1u)?0:address);}
    put(0xaf598cu+tile*4u,p[10]);
    for(i=0;i<51984;++i)*(uint8_t *)(uintptr_t)(0xacf85cu+i)=(uint8_t)p[13];
    left=(int32_t)(p[2]-1u);if(left<0)left=0;
    put(0x78d7b0,p[14]?1u:0u);put(0x78d7ac,p[14]==1?1001u:p[14]==2?1u+2u*(228u-(uint32_t)left):1u);
    for(i=0;i<11;++i){value=10000u+i*1000u;if(p[15]==1)value=0x80000000u;else if(p[15]==2&&(i==2||i==3))value=0x80000000u;else if(p[15]==3&&(i==0||i==1||i==4||i==5))value=0x80000000u;else if(p[15]==4)value=0xfffffff0u;put(graphics[i],value);}
    used=total=0;
}
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
        mov ecx,0badc0deh
        push 0baadf00dh
        push dword ptr [p+16]
        push dword ptr [p+12]
        push dword ptr [p+8]
        call FUN_0047a220
        add esp,12
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
static void digest(char hex[65])
{
    HCRYPTHASH h;DWORD n=32;uint8_t bytes[32];uint32_t i;const char *digits="0123456789abcdef";CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));for(i=0;i<8;++i){CHECK(CryptHashData(h,(BYTE *)regions[i],8,0));CHECK(CryptHashData(h,(BYTE *)(uintptr_t)regions[i][0],regions[i][1],0));}CHECK(CryptGetHashParam(h,HP_HASHVAL,bytes,&n,0));CHECK(CryptDestroyHash(h));for(i=0;i<32;++i){hex[2*i]=digits[bytes[i]>>4];hex[2*i+1]=digits[bytes[i]&15];}hex[64]=0;
}
int main(int argc,char **argv)
{
    uint32_t i,j;uint16_t old_cw,wanted,after;char hex[65];SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){puts("{\"fields\":20}");fflush(stdout);for(;;){for(i=0;i<20;++i)if(scanf("%u",p+i)!=1)goto done;CHECK(p[0]<=228&&p[1]<=228&&p[13]<=255);prepare();wanted=(uint16_t)p[19];
        __asm {fnstcw old_cw}
        __asm {fldcw wanted}
        __asm {fld tbyte ptr [old_one]}
        __asm {fld tbyte ptr [old_two]}
        invoke();
        __asm {fnstcw after}
        __asm {fstp tbyte ptr [after_two]}
        __asm {fstp tbyte ptr [after_one]}
        __asm {fldcw old_cw}
        CHECK(after==wanted&&!memcmp(old_one,after_one,10)&&!memcmp(old_two,after_two,10));CHECK(abi[0]==0x11111111&&abi[1]==0x22222222&&abi[2]==0x33333333&&abi[3]==0x44444444&&abi[4]==abi[5]&&abi[6]==0xbaadf00d);digest(hex);
        printf("{\"result\":0,\"cw\":%u,\"memory\":\"%s\",\"events\":[",(uint32_t)after,hex);for(i=0;i<used;++i){printf("%s[",i?",":"");for(j=0;j<lengths[i];++j)printf("%s%u",j?",":"",words[offsets[i]+j]);putchar(']');}puts("]}");fflush(stdout);
    }}else puts("whole eighth map postpass ready");
done:CryptReleaseContext(provider,0);restore_data_close();return 0;
}
