#include "remaining_menu_actions.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
#define U(a) (*(uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(uint32_t)(a))
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Remaining menu check line%d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t p[30],regions[12][2],events[128][32],lengths[128],used,calls;
static HCRYPTPROV provider;
static void append(const uint32_t *v,uint32_t n){CHECK(used<128&&n<=32);memcpy(events[used],v,n*4);lengths[used++]=n;}
uint32_t remaining_menu_read(uint32_t a,uint32_t n){uint32_t v=n==1?B(a):U(a),e[4]={0,a,n,0};e[3]=v;append(e,4);return v;}
void remaining_menu_write(uint32_t a,uint32_t n,uint32_t v){uint32_t e[4]={1,a,n,n==1?v&255u:v};append(e,4);if(n==1)B(a)=(uint8_t)v;else U(a)=v;}
void remaining_menu_context(uint32_t a,uintptr_t c){uint32_t e[3]={3,a,(uint32_t)c};append(e,3);}
static void endpoint(uint32_t a,uint32_t self,uint32_t n,const uint32_t *args)
{
    static const uint32_t state[]={0xe38dda,0xe38dca,0x605a3c,0xbca2d0,0xbca2d4,0xd3b198,0xeaac28,0x605a44,0xe38ea4,0xe38ebc,0xf53f48,0xd3b17c,0xd3b180};
    uint32_t e[32]={2,a,self,n},i,k=4;
    for(i=0;i<n;++i)e[k++]=args[i];
    for(i=0;i<13;++i)e[k++]=i<2?B(state[i]):U(state[i]);append(e,k);
    if(p[10]&(1u<<calls)){
        if(p[11]&1){U(0xbca2d0)=p[12];U(0xbca2d4)=p[13];}
        if(p[11]&2)U(0xd3b198)=p[14];if(p[11]&4)U(0xeaac28)=p[15];
        if(p[11]&8)B(0xe38dda)=(uint8_t)p[16];if(p[11]&16)U(0xe38ebc)=p[17];
        if(p[11]&32){U(0xd3b17c)=p[18];U(0xd3b180)=p[19];}
        if(p[11]&64)U(0x605a3c)=p[20];
        if(p[11]&128){B(0xe38dca)=0xa6;U(0xf53f48)=0xa5a55a5a;}
    }
    ++calls;
}
/* Controlled boundaries only in this private verifier. Production targets
   link the actual existing C callees instead of this fixture. */
void FUN_00478420(void){endpoint(0x478420,0,0,NULL);}
void FUN_00486e70(void){endpoint(0x486e70,0,0,NULL);}
void FUN_004dea50(void){endpoint(0x4dea50,0,0,NULL);}
void FUN_004be160(void *world,int32_t x0,int32_t y0,int32_t x1,int32_t y1){uint32_t a[]={x0,y0,x1,y1};endpoint(0x4be160,(uint32_t)(uintptr_t)world,4,a);}
void FUN_00488e90(int32_t x0,int32_t y0,int32_t x1,int32_t y1,char refresh){uint32_t a[]={x0,y0,x1,y1,(uint8_t)refresh};endpoint(0x488e90,0,5,a);}
void FUN_004e2970(int message){uint32_t a[]={(uint32_t)message};endpoint(0x4e2970,0,1,a);}
void FUN_0040a160(void){endpoint(0x40a160,0,0,NULL);}
void FUN_00539170(void){endpoint(0x539170,0,0,NULL);}
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) static void invoke(uint32_t which,uint32_t out[6])
{
    __asm {
        push ebp
        push ebx
        push esi
        push edi
        mov ecx,[esp+24]
        mov [ecx+20],esp
        mov ebx,011111111h
        mov ebp,022222222h
        mov esi,033333333h
        mov edi,044444444h
        mov eax,[esp+20]
        mov ecx,0badc0deh
        test eax,eax
        jnz selected
        call FUN_00406090
        jmp done
selected:call FUN_0040a0e0
done:
        mov ecx,[esp+24]
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
static void prepare(void)
{
    static const uint32_t fixed[][2]={{0x605a2c,48},{0xbca2c0,80},{0xd3a6d8,0xae0},{0xe38dc0,0x110},{0xeaac18,32},{0xf53f38,32},{0xd05710,48},{0xbca340,48}};
    uint32_t i;DWORD previous;
    memcpy(regions,fixed,sizeof(fixed));regions[8][0]=0x6fdc88u+p[4]-16u;regions[8][1]=33;
    regions[9][0]=0x6fdc88u+p[14]-16u;regions[9][1]=33;
    regions[10][0]=0xea623cu+p[5]*0x49d8u-16u;regions[10][1]=33;
    regions[11][0]=0xea623cu+p[15]*0x49d8u-16u;regions[11][1]=33;
    for(i=0;i<12;++i){CHECK(VirtualProtect((void *)(uintptr_t)regions[i][0],regions[i][1],PAGE_READWRITE,&previous));memset((void *)(uintptr_t)regions[i][0],(int)((p[9]+i*37u)&255u),regions[i][1]);}
    B(0xe38dda)=(uint8_t)p[1];U(0x605a3c)=p[2];U(0xe38ebc)=p[3];U(0xd3b198)=p[4];U(0xeaac28)=p[5];
    B(0x6fdc88u+p[4])=(uint8_t)p[6];B(0x6fdc88u+p[14])=(uint8_t)p[28];
    U(0xbca2d0)=p[7];U(0xbca2d4)=p[8];U(0xd3b17c)=p[21];U(0xd3b180)=p[22];
    B(0xe38dca)=(uint8_t)p[23];U(0xf53f48)=p[24];
    B(0xea623cu+p[5]*0x49d8u)=(uint8_t)p[26];B(0xea623cu+p[15]*0x49d8u)=(uint8_t)p[27];
    used=calls=0;
}
static void digest(char hex[65])
{
    HCRYPTHASH h;DWORD n=32;uint8_t bytes[32];uint32_t i;const char *digits="0123456789abcdef";
    CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));
    for(i=0;i<12;++i){CHECK(CryptHashData(h,(BYTE *)regions[i],8,0));CHECK(CryptHashData(h,(BYTE *)(uintptr_t)regions[i][0],regions[i][1],0));}
    CHECK(CryptGetHashParam(h,HP_HASHVAL,bytes,&n,0));CHECK(CryptDestroyHash(h));
    for(i=0;i<32;++i){hex[2*i]=digits[bytes[i]>>4];hex[2*i+1]=digits[bytes[i]&15];}hex[64]=0;
}
int main(int argc,char **argv)
{
    uint32_t i,j,out[6];char hex[65];
    SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){
        puts("{\"fields\":30}");fflush(stdout);
        for(;;){for(i=0;i<30;++i)if(scanf("%u",p+i)!=1)goto done;CHECK(p[0]<2);prepare();
            if(p[29])CHECK(restore_remaining_menu_action(p[0]?0x40a0e0:0x406090)==1);
            else{invoke(p[0],out);CHECK(out[0]==0x11111111&&out[1]==0x22222222&&out[2]==0x33333333&&out[3]==0x44444444&&out[4]==out[5]);}
            digest(hex);printf("{\"memory\":\"%s\",\"events\":[",hex);
            for(i=0;i<used;++i){printf("%s[",i?",":"");for(j=0;j<lengths[i];++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);
        }
    }else{CHECK(restore_remaining_menu_action(0xdeadbeef)==0);puts("Remaining menu private controlled-boundary fixture ready.");}
done:CryptReleaseContext(provider,0);restore_data_close();return 0;
}
