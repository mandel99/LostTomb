#include "refresh_menu_dispatch.h"
#include "sound_streams.h"
#include "video_player.h"
#include "standalone_data.h"
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
#include "fixture_schema.h"
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define W(a) (*(uint16_t *)(uintptr_t)(a))
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Refresh fixture line%d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t p[RF_FIELDS],events[256][RF_EVENT_FIELDS],used;
static HCRYPTPROV provider;
static uint32_t global_value(uint32_t index){CHECK(index<RF_GLOBALS);return rf_globals[index][1]==1?B(rf_globals[index][0]):U(rf_globals[index][0]);}
static void global_put(uint32_t index,uint32_t value){CHECK(index<RF_GLOBALS);if(rf_globals[index][1]==1)B(rf_globals[index][0])=(uint8_t)value;else U(rf_globals[index][0])=value;}
static void mutate(uint32_t token,uint32_t ordinal,uint32_t phase)
{
 if(p[RF_MUTATION_PHASE]!=phase||(p[RF_MUTATION_TOKEN]&&p[RF_MUTATION_TOKEN]!=token)||(p[RF_MUTATION_ORDINAL]&&p[RF_MUTATION_ORDINAL]!=ordinal))return;
 global_put(p[RF_MUTATION_FIELD],p[RF_MUTATION_VALUE]);
}
void refresh_fixture_event(uint32_t token,uint32_t count,const uint32_t *values)
{
 uint32_t i,*e;CHECK(used<256&&count<=6);mutate(token,used+1,1);e=events[used++];memset(e,0,sizeof(events[0]));e[0]=token;e[1]=count;
 for(i=0;i<count;++i)e[2+i]=values[i];for(i=0;i<RF_GLOBALS;++i)e[8+i]=global_value(i);mutate(token,used,2);
}
uint32_t refresh_fixture_result(uint32_t token,uint32_t first)
{
 uint32_t i,value=0;for(i=0;i<RF_RETURNS;++i)if(rf_returns[i]==token){value=p[RF_REPEATS+1+i];break;}CHECK(i<RF_RETURNS);
 if(token==0x5013a0u&&!(p[RF_QUERY_MASK]&(1u<<(first&31u))))value=0;return value;
}
#include "boundaries.inc"
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) static void checked_call(uint32_t out[6])
{
 __asm{
  push ebp
  push ebx
  push esi
  push edi
  mov ecx,[esp+20]
  mov [ecx+20],esp
  mov ebx,011111111h
  mov ebp,022222222h
  mov esi,033333333h
  mov edi,044444444h
  call FUN_004d75d0
  mov ecx,[esp+20]
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
 uint32_t i,j;DWORD old;
 for(i=0;i<sizeof(rf_regions)/sizeof(rf_regions[0]);++i){CHECK(VirtualProtect((void *)(uintptr_t)rf_regions[i][0],rf_regions[i][1],PAGE_READWRITE,&old));for(j=0;j<rf_regions[i][1];++j)B(rf_regions[i][0]+j)=(uint8_t)(p[RF_SEED]+i*29u+j*37u);}
 for(i=0;i<8u*0x24ecu;++i)W(0xea4096u+i*2u)=(uint16_t)p[RF_TRAINING_TAG];
 for(i=0;i<40;++i)W(0x4ac24096u+i*2u)=(uint16_t)p[RF_SYNTHETIC_TAG];
 for(i=0;i<257;++i)W(0x7999e8u+i*264u)=(uint16_t)p[RF_ACTOR_TAG];W(0x7999e8u+256u*264u)=(uint16_t)p[RF_ACTOR_HIGH_TAG];
 for(i=0;i<8;++i)B(0x77f748u+i*98u)=(uint8_t)p[RF_RECORD_BYTE];
 for(i=0;i<36;++i)U(0xf53e70u+i*4u)=0xa0000000u+i;
 for(i=0;i<RF_GLOBALS;++i)global_put(i,p[i]);used=0;
 CHECK(p[RF_REPEATS]>0&&p[RF_REPEATS]<=3);
}
static void run(void)
{
 uint32_t i,out[6];prepare();for(i=0;i<p[RF_REPEATS];++i){checked_call(out);CHECK(out[0]==0x11111111&&out[1]==0x22222222&&out[2]==0x33333333&&out[3]==0x44444444&&out[4]==out[5]);}
}
static void output(void)
{
 HCRYPTHASH hash;DWORD n=32;uint8_t bytes[32];uint32_t i,j;static const char digits[]="0123456789abcdef";
 CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&hash));for(i=0;i<sizeof(rf_regions)/sizeof(rf_regions[0]);++i){CHECK(CryptHashData(hash,(const BYTE *)rf_regions[i],8,0));CHECK(CryptHashData(hash,(const BYTE *)(uintptr_t)rf_regions[i][0],rf_regions[i][1],0));}CHECK(CryptGetHashParam(hash,HP_HASHVAL,bytes,&n,0));CHECK(CryptDestroyHash(hash));
 printf("{\"memory\":\"");for(i=0;i<32;++i){putchar(digits[bytes[i]>>4]);putchar(digits[bytes[i]&15]);}printf("\",\"events\":[");
 for(i=0;i<used;++i){printf("%s[",i?",":"");for(j=0;j<RF_EVENT_FIELDS;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);
}
int main(int argc,char **argv)
{
 uint32_t i;CHECK(restore_data_open());SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(VirtualAlloc((void *)0x4ac24000u,0x2000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE));CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
 if(argc==2&&!strcmp(argv[1],"--oracle-batch")){printf("{\"fields\":%u}\n",(uint32_t)RF_FIELDS);fflush(stdout);for(;;){for(i=0;i<RF_FIELDS;++i)if(scanf("%u",p+i)!=1)goto done;run();output();}}
 else{
  memcpy(p,rf_defaults,sizeof(p));run();CHECK(used>10&&B(0xe38dc9)==0);p[RF_G_00E38DDA]=15;run();CHECK(events[0][0]==0x530d70u);p[RF_G_00E38DDA]=61;run();CHECK(events[0][0]==0x425730u&&events[0][7]==0&&events[0][5]==4);puts("Whole refresh candidate: city/popup/mission branches and actual getters ABI smoke.");
 }
done:CHECK(CryptReleaseContext(provider,0));restore_data_close();return 0;
}
