#include "map_refresh_postpass5.h"
#include "postpass5_record_data.h"
#include "city_prepare.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
#define CHECK(c) do{if(!(c)){fprintf(stderr,"postpass5 line%d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t p[40],words[2000000],offsets[400000],lengths[400000],used,total,abi[6];
static const uint32_t regions[13][2]={{0xac2d3c,0xcb30},{0xaf557c,0x3fb70},{0xb4e4ec,0x58f90},{0xbc9730,0xc00},{0xd05710,0x32c60},{0xf56b74,0x170},{0x978d64,0x30},{0x5cc258,0x530},{0x6f3ff0,0x120},{0x7999d8,0x128},{0x799df8,0x128},{0x79a218,0x128},{0x79a638,0x128}};
static HCRYPTPROV provider;
static const uint8_t old_one[10]={0,0,0,0,0,0,0,0xc0,0xff,0x3f},old_two[10]={0,0,0,0,0,0,0,0x80,0,0xc0};
static uint8_t after_one[10],after_two[10];
static void append(uint32_t *e,uint32_t n){CHECK(used<400000&&total+n<=2000000);offsets[used]=total;memcpy(words+total,e,n*4);lengths[used++]=n;total+=n;}
uint32_t map_post5_read(uint32_t a,uint32_t n){uint32_t v=n==1?*(uint8_t *)(uintptr_t)a:n==2?*(uint16_t *)(uintptr_t)a:*(uint32_t *)(uintptr_t)a;uint32_t e[5]={0,a,n,v,0};append(e,5);return v;}
void map_post5_write(uint32_t a,uint32_t n,uint32_t v){uint32_t e[5]={1,a,n,n==1?v&255u:n==2?v&65535u:v,0};append(e,5);if(n==1)*(uint8_t *)(uintptr_t)a=(uint8_t)v;else if(n==2)*(uint16_t *)(uintptr_t)a=(uint16_t)v;else *(uint32_t *)(uintptr_t)a=v;}
void map_post5_helper(uint32_t a,uint32_t phase,uint32_t arg){uint32_t i,e[13]={3,a,phase,arg,*(uint32_t *)0xbc9768};for(i=0;i<8;++i)e[5+i]=*(uint32_t *)(uintptr_t)(0xbc97c0u+i*4u);append(e,13);}
uint32_t map_post2_read(uint32_t a,uint32_t n){return map_post5_read(a,n);}
void map_post2_write(uint32_t a,uint32_t n,uint32_t v){map_post5_write(a,n,v);}
uint32_t map_post4_read(uint32_t a,uint32_t n){return map_post5_read(a,n);}
void map_post4_write(uint32_t a,uint32_t n,uint32_t v){map_post5_write(a,n,v);}
static void prepare(void)
{
    uint32_t i,index,base,a;int32_t dx,dy,nx,ny;DWORD previous;static const int32_t neighbors[8]={-228,-227,1,229,228,227,-1,-229};
    for(i=0;i<13;++i){CHECK(VirtualProtect((void *)(uintptr_t)regions[i][0],regions[i][1],PAGE_READWRITE,&previous));memset((void *)(uintptr_t)regions[i][0],(int)((p[14]+i*37u)&255u),regions[i][1]);}
    memset((void *)0xac2d4c,(int)(p[10]&255u),0xcb10);memset((void *)0xaf558c,0,0x33040);memset((void *)0xd05720,0,0x32c40);memset((void *)0xb4e4fc,0,0x19820);memset((void *)0xb285cc,(int)(p[19]&255u),0xcb10);
    *(uint32_t *)0xbca2d0=p[1];*(uint32_t *)0xbca2d4=p[2];base=((228u-p[2])/2u)*228u+(228u-p[1])/2u;*(uint32_t *)0xbca2d8=base;*(uint32_t *)0xbca2dc=228u-p[1];*(uint32_t *)0xbca2e0=p[18];
    for(i=0;i<51984;++i)*(uint32_t *)(uintptr_t)(0xb7482cu+i*4u)=p[14]+p[21];
    *(uint8_t *)0x6f4018=p[12]==1?0:p[12]==3?128:1;
    for(dy=-2;dy<=2;++dy)for(dx=-2;dx<=2;++dx){nx=(int32_t)p[16]+dx;ny=(int32_t)p[17]+dy;if(nx<0||ny<0||nx>=(int32_t)p[1]||ny>=(int32_t)p[2])continue;index=base+(uint32_t)ny*228u+(uint32_t)nx;*(uint32_t *)(uintptr_t)(0xaf598cu+index*4u)=p[15]==1?p[9]:0;*(uint32_t *)(uintptr_t)(0xd05720u+index*4u)=p[12]?0x6f4000:0;}
    if(p[16]<p[1]&&p[17]<p[2]){
        index=base+p[17]*228u+p[16];
        if(p[15]==2||p[15]==3)for(i=0;i<8;++i)*(uint32_t *)(uintptr_t)(0xaf598cu+(index+(uint32_t)neighbors[i])*4u)=p[15]==2?p[26+i]:((p[24]>>i)&1u)?p[9]:0;
        for(i=0;i<4;++i){int32_t cell=(int32_t)index+neighbors[i*2u];if(cell>=0&&cell<51984)*(uint16_t *)(uintptr_t)(0xb4e6fcu+(uint32_t)cell*2u)=(uint16_t)(i*4u);}
        *(uint32_t *)(uintptr_t)(0xaf598cu+index*4u)=p[8];*(uint16_t *)(uintptr_t)(0xb4e6fcu+index*2u)=(uint16_t)p[20];
    }
    *(uint32_t *)0xbc9768=base+p[17]*228u+p[16];for(i=0;i<8;++i)*(uint32_t *)(uintptr_t)(0xbc97c0u+i*4u)=((p[24]>>i)&1u)|(p[38]&~1u);
    for(i=0;i<4;++i){a=0x7999e8u+i*4u*264u;*(uint16_t *)(uintptr_t)a=(uint16_t)p[34];*(uint16_t *)(uintptr_t)(a+2u)=(uint16_t)(p[35]?p[35]:(i&1u?2u:1u));*(uint8_t *)(uintptr_t)(a+154u)=(uint8_t)(p[36]>>(i*8u));}
    memcpy((void *)0x5cc268,post5_records,sizeof post5_records);for(i=0;i<81;++i){a=0x5cc268u+i*16u;if(p[22])*(uint8_t *)(uintptr_t)(a+13u)=(uint8_t)p[22];*(uint8_t *)(uintptr_t)(a+14u)=(uint8_t)p[23];if(p[25])*(uint8_t *)(uintptr_t)(a+12u)=(uint8_t)(p[25]-1u);}
    *(uint32_t *)0xf56b84=p[14];*(uint32_t *)0xf56c34=p[14];*(uint32_t *)0xf56c44=p[14]+0x10000u;*(uint32_t *)0xf56c94=p[14]+0x20000u;*(uint32_t *)0xf56cd4=p[14]+0x30000u;*(uint32_t *)0x978d74=p[11];used=total=0;
}
static void fixture_normalize(void){p5_normalize();}
static void fixture_gather(void){p5_gather(p[39]);}
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
        mov eax,dword ptr [p]
        cmp eax,0
        jne rectangle
        call FUN_0047b670
        jmp finished
rectangle:
        cmp eax,1
        jne predicate
        push dword ptr [p+24]
        push dword ptr [p+20]
        push dword ptr [p+16]
        push dword ptr [p+12]
        call FUN_0047b590
        add esp,16
        jmp finished
predicate:
        cmp eax,2
        jne add21
        call FUN_0047b920
        jmp finished
add21:
        cmp eax,3
        jne convert
        push dword ptr [p+28]
        call FUN_0051c620
        add esp,4
        jmp finished
convert:
        cmp eax,4
        jne normalize
        push dword ptr [p+28]
        push dword ptr [p+148]
        call FUN_0051c5f0
        add esp,8
        jmp finished
normalize:
        cmp eax,5
        jne gather
        call fixture_normalize
        jmp finished
gather:
        call fixture_gather
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
    HCRYPTHASH h;DWORD n=32;uint8_t bytes[32];uint32_t i;const char *digits="0123456789abcdef";CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));for(i=0;i<13;++i){CHECK(CryptHashData(h,(BYTE *)regions[i],8,0));CHECK(CryptHashData(h,(BYTE *)(uintptr_t)regions[i][0],regions[i][1],0));}CHECK(CryptGetHashParam(h,HP_HASHVAL,bytes,&n,0));CHECK(CryptDestroyHash(h));for(i=0;i<32;++i){hex[2*i]=digits[bytes[i]>>4];hex[2*i+1]=digits[bytes[i]&15];}hex[64]=0;
}
int main(int argc,char **argv)
{
    uint32_t i,j,result;uint16_t old_cw,wanted,after;char hex[65];SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){
        puts("{\"fields\":40}");fflush(stdout);
        for(;;){for(i=0;i<40;++i)if(scanf("%u",p+i)!=1)goto done;CHECK(p[0]<7&&p[1]<=228&&p[2]<=228);prepare();wanted=(uint16_t)p[13];
            __asm {fnstcw old_cw}
            __asm {fldcw wanted}
            __asm {fld tbyte ptr [old_one]}
            __asm {fld tbyte ptr [old_two]}
            result=invoke();
            __asm {fnstcw after}
            __asm {fstp tbyte ptr [after_two]}
            __asm {fstp tbyte ptr [after_one]}
            __asm {fldcw old_cw}
            CHECK(after==wanted&&!memcmp(old_one,after_one,10)&&!memcmp(old_two,after_two,10));CHECK(abi[0]==0x11111111&&abi[1]==0x22222222&&abi[2]==0x33333333&&abi[3]==0x44444444&&abi[4]==abi[5]);if(p[0]==0||p[0]==1||p[0]>=5)result=0;digest(hex);
            printf("{\"result\":%u,\"cw\":%u,\"memory\":\"%s\",\"events\":[",result,(uint32_t)after,hex);for(i=0;i<used;++i){printf("%s[",i?",":"");for(j=0;j<lengths[i];++j)printf("%s%u",j?",":"",words[offsets[i]+j]);putchar(']');}puts("]}");fflush(stdout);
        }
    }else puts("complete fifth map postpass cluster ready");
done:CryptReleaseContext(provider,0);restore_data_close();return 0;
}
