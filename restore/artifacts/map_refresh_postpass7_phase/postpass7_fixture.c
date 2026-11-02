#include "map_refresh_postpass7.h"
#include "city_prepare.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
#define CHECK(c) do{if(!(c)){fprintf(stderr,"postpass7 line%d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t p[20],words[2000000],offsets[400000],lengths[400000],used,total,abi[6];
static const uint32_t regions[2][2]={{0xaf517c,0x33c60},{0xbc9730,0xc00}};
static uint32_t depth,max_depth;
static HCRYPTPROV provider;
static const uint8_t old_one[10]={0,0,0,0,0,0,0,0xc0,0xff,0x3f},old_two[10]={0,0,0,0,0,0,0,0x80,0,0xc0};
static uint8_t after_one[10],after_two[10];
static void append(uint32_t *e,uint32_t n){CHECK(used<400000&&total+n<=2000000);offsets[used]=total;memcpy(words+total,e,n*4);lengths[used++]=n;total+=n;}
uint32_t map_post7_read(uint32_t a,uint32_t n){uint32_t v=*(uint32_t *)(uintptr_t)a,e[5]={0,a,n,v,0};append(e,5);return v;}
void map_post7_write(uint32_t a,uint32_t n,uint32_t v){uint32_t e[5]={1,a,n,v,0};append(e,5);*(uint32_t *)(uintptr_t)a=v;}
void map_post6_helper(uint32_t a,uint32_t phase,uint32_t arg){uint32_t i,e[13]={3,a,phase,arg,*(uint32_t *)0xbc9768};for(i=0;i<8;++i)e[5+i]=*(uint32_t *)(uintptr_t)(0xbc97c0u+i*4u);append(e,13);}
void map_post6_recursion(uint32_t phase,int32_t x,int32_t y){uint32_t e[5]={4,phase,(uint32_t)x,(uint32_t)y,0};if(phase){CHECK(depth);--depth;}e[4]=depth;append(e,5);if(!phase){++depth;if(depth>max_depth)max_depth=depth;}}
uint32_t map_post6_read(uint32_t a,uint32_t n){return map_post7_read(a,n);}
void map_post6_write(uint32_t a,uint32_t n,uint32_t v){map_post7_write(a,n,v);}
void map_post7_phase(uint32_t tag)
{
    HCRYPTHASH hash;DWORD n=32;uint32_t i,e[10]={5,tag};CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&hash));
    for(i=0;i<2;++i){CHECK(CryptHashData(hash,(BYTE *)regions[i],8,0));CHECK(CryptHashData(hash,(BYTE *)(uintptr_t)regions[i][0],regions[i][1],0));}
    CHECK(CryptGetHashParam(hash,HP_HASHVAL,(BYTE *)(e+2),&n,0));CHECK(CryptDestroyHash(hash));append(e,10);
}
static void field(uint32_t base,int32_t x,int32_t y,uint32_t value){int32_t tile=(int32_t)(base+(uint32_t)y*228u+(uint32_t)x);if(tile>=-512&&tile<52496)*(uint32_t *)(uintptr_t)(0xaf598cu+(uint32_t)tile*4u)=value;}
static void prepare(void)
{
    uint32_t i,base,value,bit,state;int32_t dx,dy,x=(int32_t)p[14],y=(int32_t)p[15],w=(int32_t)p[10],h=(int32_t)p[11];DWORD previous;static const uint32_t states[4]={0,4,0x80004,0x10004};
    for(i=0;i<2;++i){CHECK(VirtualProtect((void *)(uintptr_t)regions[i][0],regions[i][1],PAGE_READWRITE,&previous));memset((void *)(uintptr_t)regions[i][0],(int)((p[12]+i*37u)&255u),regions[i][1]);}
    for(i=0;i<53008;++i)*(uint32_t *)(uintptr_t)(0xaf518cu+i*4u)=p[7];
    base=((228u-p[2])/2u)*228u+(228u-p[1])/2u;*(uint32_t *)0xbca2d0=p[1];*(uint32_t *)0xbca2d4=p[2];*(uint32_t *)0xbca2d8=base;*(uint32_t *)0xbca2dc=228u-p[1];*(uint32_t *)0xbc9768=p[17];
    for(i=0;i<8;++i)*(uint32_t *)(uintptr_t)(0xbc97c0u+i*4u)=p[18]^(i*0x11335577u);
    if(p[9]==0){for(dy=-1;dy<=1;++dy)for(dx=-1;dx<=1;++dx){bit=(uint32_t)((dy+1)*3+dx+1);state=(p[19]>>(bit*2u))&3u;value=p[7]|(p[19]?states[state]:((p[8]>>bit&1u)?p[16]:0));field(base,x+dx,y+dy,value);}}
    else{for(dy=0;dy<h;++dy)for(dx=0;dx<w;++dx){value=p[7];if(p[9]==1||(p[9]==2&&((dx+dy)&1))||(p[9]==3&&(dx==w/2||dy==h/2)))value|=p[16];field(base,x+dx-w/2,y+dy-h/2,value);}}
    used=total=depth=max_depth=0;
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
        mov ecx,0badc0deh
        push dword ptr [p+24]
        push dword ptr [p+20]
        push dword ptr [p+16]
        push dword ptr [p+12]
        call FUN_004877f0
        add esp,16
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
    HCRYPTHASH h;DWORD n=32;uint8_t bytes[32];uint32_t i;const char *digits="0123456789abcdef";CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));for(i=0;i<2;++i){CHECK(CryptHashData(h,(BYTE *)regions[i],8,0));CHECK(CryptHashData(h,(BYTE *)(uintptr_t)regions[i][0],regions[i][1],0));}CHECK(CryptGetHashParam(h,HP_HASHVAL,bytes,&n,0));CHECK(CryptDestroyHash(h));for(i=0;i<32;++i){hex[2*i]=digits[bytes[i]>>4];hex[2*i+1]=digits[bytes[i]&15];}hex[64]=0;
}
int main(int argc,char **argv)
{
    uint32_t i,j,result;uint16_t old_cw,wanted,after;char hex[65];SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){
        puts("{\"fields\":20}");fflush(stdout);
        for(;;){for(i=0;i<20;++i)if(scanf("%u",p+i)!=1)goto done;CHECK(p[0]==0&&(p[1]<=228||(int32_t)p[1]<0)&&(p[2]<=228||(int32_t)p[2]<0)&&p[10]<=64&&p[11]<=64);prepare();wanted=(uint16_t)p[13];
            __asm {fnstcw old_cw}
            __asm {fldcw wanted}
            __asm {fld tbyte ptr [old_one]}
            __asm {fld tbyte ptr [old_two]}
            result=invoke();
            __asm {fnstcw after}
            __asm {fstp tbyte ptr [after_two]}
            __asm {fstp tbyte ptr [after_one]}
            __asm {fldcw old_cw}
            CHECK(after==wanted&&!memcmp(old_one,after_one,10)&&!memcmp(old_two,after_two,10));CHECK(abi[0]==0x11111111&&abi[1]==0x22222222&&abi[2]==0x33333333&&abi[3]==0x44444444&&abi[4]==abi[5]);CHECK(!depth);result=0;digest(hex);
            printf("{\"result\":%u,\"cw\":%u,\"memory\":\"%s\",\"events\":[",result,(uint32_t)after,hex);for(i=0;i<used;++i){printf("%s[",i?",":"");for(j=0;j<lengths[i];++j)printf("%s%u",j?",":"",words[offsets[i]+j]);putchar(']');}puts("]}");fflush(stdout);
        }
    }else puts("complete seventh map postpass cluster ready");
done:CryptReleaseContext(provider,0);restore_data_close();return 0;
}
