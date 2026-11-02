#include <stdint.h>
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
void FUN_00484ee0(int32_t,int32_t);
int32_t FUN_00484f40(void);
uint32_t FUN_0047a1f0(int32_t);
#define CHECK(c) do{if(!(c)){fprintf(stderr,"map8leaf line%d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t p[10],abi[6];
static const uint32_t regions[3][2]={{0x6f1ff0,0x1020},{0xbc9730,0xc00},{0x78d79c,0x30}};
static HCRYPTPROV provider;
static const uint8_t old_one[10]={0,0,0,0,0,0,0,0xc0,0xff,0x3f},old_two[10]={0,0,0,0,0,0,0,0x80,0,0xc0};
static uint8_t after_one[10],after_two[10];
static void prepare(void){uint32_t i;DWORD old;for(i=0;i<3;++i){CHECK(VirtualProtect((void *)(uintptr_t)regions[i][0],regions[i][1],PAGE_READWRITE,&old));memset((void *)(uintptr_t)regions[i][0],(int)((p[8]+37u*i)&255u),regions[i][1]);}for(i=0;i<0x1020;++i)*(uint8_t *)(uintptr_t)(0x6f1ff0u+i)=(uint8_t)(p[8]+i*17u);for(i=0;i<8;++i)*(uint32_t *)(uintptr_t)(0xbc97c0u+i*4u)=((p[3]>>i)&1u)|(p[4]&~1u);*(uint32_t *)0x78d7ac=p[6];*(uint32_t *)0x78d7b0=p[7];}
__declspec(naked) static uint32_t invoke(void){__asm{
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
 jne count_slots
 mov eax,dword ptr [p+4]
 cmp eax,0ffffffffh
 jne buffer_address
 xor eax,eax
 jmp reset
buffer_address:
 add eax,06f2000h
reset:
 push dword ptr [p+8]
 push eax
 call FUN_00484ee0
 add esp,8
 jmp finished
count_slots:
 cmp eax,1
 jne water
 call FUN_00484f40
 jmp finished
water:
 push dword ptr [p+20]
 call FUN_0047a1f0
 add esp,4
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
}}
static void digest(char hex[65]){HCRYPTHASH h;DWORD n=32;uint32_t i;uint8_t bytes[32];const char *digits="0123456789abcdef";CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));for(i=0;i<3;++i){CHECK(CryptHashData(h,(BYTE *)regions[i],8,0));CHECK(CryptHashData(h,(BYTE *)(uintptr_t)regions[i][0],regions[i][1],0));}CHECK(CryptGetHashParam(h,HP_HASHVAL,bytes,&n,0));CHECK(CryptDestroyHash(h));for(i=0;i<32;++i){hex[2*i]=digits[bytes[i]>>4];hex[2*i+1]=digits[bytes[i]&15];}hex[64]=0;}
int main(int argc,char **argv){uint32_t i,result;uint16_t old_cw,wanted,after;char hex[65];SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));if(argc==2&&!strcmp(argv[1],"--oracle-batch")){puts("{\"fields\":10}");fflush(stdout);for(;;){for(i=0;i<10;++i)if(scanf("%u",p+i)!=1)goto done;CHECK(p[0]<3&&((p[1]<=31&&(int32_t)p[2]<=64)||(p[1]==0xffffffffu&&(int32_t)p[2]<=0)));prepare();wanted=(uint16_t)p[9];
 __asm{fnstcw old_cw}
 __asm{fldcw wanted}
 __asm{fld tbyte ptr [old_one]}
 __asm{fld tbyte ptr [old_two]}
 result=invoke();
 __asm{fnstcw after}
 __asm{fstp tbyte ptr [after_two]}
 __asm{fstp tbyte ptr [after_one]}
 __asm{fldcw old_cw}
 CHECK(after==wanted&&!memcmp(old_one,after_one,10)&&!memcmp(old_two,after_two,10));CHECK(abi[0]==0x11111111&&abi[1]==0x22222222&&abi[2]==0x33333333&&abi[3]==0x44444444&&abi[4]==abi[5]);if(p[0]==0)result=0;else if(p[0]==2&&(int32_t)p[7]<=0)result&=255u;digest(hex);printf("{\"result\":%u,\"cw\":%u,\"memory\":\"%s\"}\n",result,(uint32_t)after,hex);fflush(stdout);}}else puts("actual map8 leaf preflight ready");done:CryptReleaseContext(provider,0);restore_data_close();return 0;}
