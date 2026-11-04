#include "settings_callbacks.h"
#include "settings_case.h"
#include "standalone_data.h"
#include "media_bindings.h"
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
#define U(a) (*(uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(uint32_t)(a))
#define ALT 0x31000000u
#define SOUND 0xe92bf8u
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Settings failure at line %d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t p[P_FIELDS],events[2048][32],writes[2048][3],used,stored,seen,plain_mode,active_self;
static HCRYPTPROV provider;
static const uint32_t regions[][2]={{0x605790,0x2c0},{0xd3b188,0x30},{0xe38db0,0x300},{0x6de500,0x30},{0xe39130,0x30},{0xe92be8,0x224},{ALT,0x224},{0xea186c,4*0x49d8+0x30},{0xeaac18,0x30},{0x5d1950,0x40},{0x7848e8,0x30}};
#define DECLARE(v) \
void settings_##v##_00402c10(void),settings_##v##_00402c40(void),settings_##v##_00402cc0(void),settings_##v##_00402e50(void),settings_##v##_00402f70(void),settings_##v##_00403060(void),settings_##v##_00403130(void),settings_##v##_004031a0(void),settings_##v##_00403210(void),settings_##v##_00403270(void),settings_##v##_004032f0(void),settings_##v##_00403320(void),settings_##v##_00403390(void),settings_##v##_004ea760(void),settings_##v##_0051f780(void); \
void settings_##v##_00416ba0(uint32_t); \
void settings_##v##_0055f32f(void *,int32_t),settings_##v##_0055f3eb(void *,int32_t),settings_##v##_0055f38d(RestoreSound *,int32_t),settings_##v##_0055f561(RestoreSound *,int),settings_##v##_0055f639(RestoreSound *,int,int32_t)
DECLARE(trace);DECLARE(plain);
static uint32_t flags(void){return B(0xe38e63)|((uint32_t)B(0xe38e64)<<8)|((uint32_t)B(0xe38e65)<<16)|((uint32_t)B(0xe38ec0)<<24);}
static void put_flags(uint32_t value){B(0xe38e63)=(uint8_t)value;B(0xe38e64)=(uint8_t)(value>>8);B(0xe38e65)=(uint8_t)(value>>16);B(0xe38ec0)=(uint8_t)(value>>24);}
static void observe(uint32_t fn,uint32_t phase,uint32_t a,uint32_t b,uint32_t c,uint32_t d,uint32_t result)
{
 uint32_t *e,n,s=active_self;CHECK(used<2048);e=events[used++];e[0]=fn;e[1]=phase;e[2]=a;e[3]=b;e[4]=c;e[5]=d;e[6]=result;
 e[7]=U(0xd3b198);e[8]=B(0xe38dda);e[9]=B(0xe38dca);e[10]=U(0xe39080);e[11]=U(0xeaac28);e[12]=U(0xe38ec8);e[13]=U(0xe38ecc);e[14]=U(0xe38ec4);e[15]=U(0xe38ed0);e[16]=flags();e[17]=U(0xe38e6c);e[18]=U(0xe38e70);e[19]=U(0x605a3c);e[20]=U(0x6de510);e[21]=U(0xe38ef0);e[22]=U(s+0x200);e[23]=U(s+0x1d8);e[24]=U(s+0x1dc);e[25]=U(s+0x1e0);e[26]=U(s+0x14c);e[27]=U(s+0x154);e[28]=U(s+0x158);e[29]=U(0x7848f8);e[30]=U(0xe39140);e[31]=s;
 if(fn==p[P_MFN]&&phase==p[P_MPHASE]&&++seen==p[P_MOCC]){
  n=p[P_MBITS];if(n&1u)U(0xd3b198)=p[P_NEWSELECTED];if(n&2u){B(0xe38dda)=(uint8_t)p[P_NEWMODE];B(0xe38dca)=(uint8_t)p[P_NEWDIRTY];}if(n&4u)U(0xe39080)=p[P_NEWDIFFICULTY];if(n&8u)U(0xeaac28)=p[P_NEWPLAYER];
  if(n&16u){U(0xe38ec8)=p[P_NEWVOL0];U(0xe38ecc)=p[P_NEWVOL1];U(0xe38ec4)=p[P_NEWVOL2];U(0xe38ed0)=p[P_NEWVOL3];put_flags(p[P_NEWFLAGS]);}
  if(n&32u){U(s+0x200)=p[P_NEWDISABLED];U(s+0x1d8)=p[P_NEWSTREAM0];U(s+0x1dc)=p[P_NEWSTREAM1];U(s+0x1e0)=p[P_NEWSTREAM2];}
  if(n&64u){U(s+0x14c)=p[P_NEWVOL0];U(s+0x154)=p[P_NEWVOL1];U(s+0x158)=p[P_NEWVOL3];}
  if(n&128u)U(0x6de510)=p[P_NEWPOPUP];if(n&256u){U(0xe38e6c)=p[P_NEWSPEED];U(0xe38e70)=p[P_NEWSCROLL];U(0x605a3c)=p[P_NEWLOCK];}
  if(n&512u){U(0x6057bc)=p[P_NEWVOL0];U(0x6057b4)=p[P_NEWVOL1];U(0x6057ac)=p[P_NEWVOL2];U(0x6057a4)=p[P_NEWVOL3];B(0x6057c0)=(uint8_t)(p[P_NEWFLAGS]>>8);B(0x6057b8)=(uint8_t)(p[P_NEWFLAGS]>>16);B(0x6057b0)=(uint8_t)p[P_NEWFLAGS];B(0x6057a8)=(uint8_t)(p[P_NEWFLAGS]>>24);}
  if(n&1024u)U(0x7848f8)=p[P_NEWQUERYARG];if(n&2048u)U(0xe39140)=p[P_NEWWINDOW];
 }
}
#define OBS(f,ph,a,b,c,d,r) observe(f,ph,(uint32_t)(a),(uint32_t)(b),(uint32_t)(c),(uint32_t)(d),(uint32_t)(r))
void restore_settings_write(uint32_t address,uint32_t width,uint32_t value)
{
 uint32_t j;for(j=0;j<sizeof(regions)/sizeof(regions[0]);++j)if(address>=regions[j][0]&&address+width<=regions[j][0]+regions[j][1])break;CHECK(j<sizeof(regions)/sizeof(regions[0]));CHECK(stored<2048);writes[stored][0]=address;writes[stored][1]=width;writes[stored++][2]=width==1?(value&255u):value;if(width==1)B(address)=(uint8_t)value;else{CHECK(width==4);U(address)=value;}
}
#define WRAP0(a) void FUN_##a(void){OBS(0x##a,0,0,0,0,0,0);if(plain_mode)settings_plain_##a();else settings_trace_##a();OBS(0x##a,1,0,0,0,0,0);}
WRAP0(00402c10) WRAP0(00402c40) WRAP0(00402cc0) WRAP0(00402e50) WRAP0(00402f70) WRAP0(00403060) WRAP0(00403130) WRAP0(004031a0) WRAP0(00403210) WRAP0(00403270) WRAP0(004032f0) WRAP0(00403320) WRAP0(00403390) WRAP0(004ea760) WRAP0(0051f780)
void FUN_00416ba0(uint32_t selection){OBS(0x416ba0,0,selection,0,0,0,0);if(plain_mode)settings_plain_00416ba0(selection);else settings_trace_00416ba0(selection);OBS(0x416ba0,1,selection,0,0,0,0);}
#define MEMBER_ONE(name,code,trace,plain,type) void name(type self,int32_t argument){uint32_t old=active_self;active_self=(uint32_t)(uintptr_t)self;OBS(code,0,(uintptr_t)self,argument,0,0,0);if(plain_mode)plain(self,argument);else trace(self,argument);OBS(code,1,(uintptr_t)self,argument,0,0,0);active_self=old;}
MEMBER_ONE(restore_settings_sound_55f32f,0x55f32f,settings_trace_0055f32f,settings_plain_0055f32f,void *)
MEMBER_ONE(FUN_0055f38d,0x55f38d,settings_trace_0055f38d,settings_plain_0055f38d,RestoreSound *)
MEMBER_ONE(restore_settings_sound_55f3eb,0x55f3eb,settings_trace_0055f3eb,settings_plain_0055f3eb,void *)
MEMBER_ONE(FUN_0055f561,0x55f561,settings_trace_0055f561,settings_plain_0055f561,RestoreSound *)
void FUN_0055f639(RestoreSound *self,int channel,int32_t volume){uint32_t old=active_self;active_self=(uint32_t)(uintptr_t)self;OBS(0x55f639,0,(uintptr_t)self,channel,volume,0,0);if(plain_mode)settings_plain_0055f639(self,channel,volume);else settings_trace_0055f639(self,channel,volume);OBS(0x55f639,1,(uintptr_t)self,channel,volume,0,0);active_self=old;}
static BOOL WINAPI post(HWND window,UINT message,WPARAM key,LPARAM detail){OBS(0x56f174,0,(uintptr_t)window,message,key,detail,0);OBS(0x56f174,1,(uintptr_t)window,message,key,detail,p[P_RESULT]);return(BOOL)p[P_RESULT];}
static const RestoreMenuActionApi api={NULL,NULL,NULL,post};
const RestoreMenuActionApi *restore_menu_action_platform(void){return &api;}
int __cdecl _AIL_stream_status_4(uintptr_t stream){OBS(0x56f298,0,stream,0,0,0,0);OBS(0x56f298,1,stream,0,0,0,p[P_STATUS]);return(int)p[P_STATUS];}
int __cdecl _AIL_set_stream_volume_8(uintptr_t stream,uintptr_t volume){OBS(0x56f29c,0,stream,volume,0,0,0);OBS(0x56f29c,1,stream,volume,0,0,p[P_RESULT]);return(int)p[P_RESULT];}
int __cdecl _AIL_close_stream_4(uintptr_t stream){OBS(0x56f2d0,0,stream,0,0,0,0);OBS(0x56f2d0,1,stream,0,0,0,p[P_RESULT]);return(int)p[P_RESULT];}
#define DEEP0(a) void FUN_##a(void){OBS(0x##a,0,0,0,0,0,0);OBS(0x##a,1,0,0,0,0,0);}
DEEP0(00540c90) DEEP0(00540ed0) DEEP0(00541160) DEEP0(004bc720)
int FUN_004cde40(int first,int second){OBS(0x4cde40,0,first,second,0,0,0);OBS(0x4cde40,1,first,second,0,0,p[P_RESULT]);return(int)p[P_RESULT];}
static void (*const callbacks[])(void)={FUN_00402c10,FUN_00402c40,FUN_00402cc0,FUN_00402e50,FUN_00402f70,FUN_00403060,FUN_00403130,FUN_004031a0,FUN_00403210,FUN_00403270,FUN_004032f0,FUN_00403320,FUN_00403390,FUN_004ea760,FUN_0051f780};
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) static void checked_call(uint32_t op,const uint32_t *q,uint32_t out[6])
{
 __asm{
  push ebp
  push ebx
  push esi
  push edi
  mov ecx,[esp+28]
  mov [ecx+20],esp
  mov ebx,011111111h
  mov ebp,022222222h
  mov esi,033333333h
  mov edi,044444444h
  mov eax,[esp+20]
  mov ecx,[esp+24]
  cmp eax,15
  je speed_caller
  jg member_call
  call dword ptr[callbacks+eax*4]
  jmp complete
speed_caller:push dword ptr[ecx+P_ARGUMENT*4]
  call FUN_00416ba0
  add esp,4
  jmp complete
member_call:mov edx,0e92bf8h
  cmp dword ptr[ecx+P_SELF*4],0
  je receiver_ready
  mov edx,031000010h
receiver_ready:cmp eax,20
  je dynamic_volume
  push dword ptr[ecx+P_ARGUMENT*4]
  push edx
  cmp eax,16
  je volume_one
  cmp eax,17
  je volume_zero
  cmp eax,18
  je volume_two
  call FUN_0055f561
  jmp one_member_done
volume_one:call restore_settings_sound_55f32f
  jmp one_member_done
volume_zero:call FUN_0055f38d
  jmp one_member_done
volume_two:call restore_settings_sound_55f3eb
one_member_done:add esp,8
  jmp complete
dynamic_volume:push dword ptr[ecx+P_VOL0*4]
  push dword ptr[ecx+P_ARGUMENT*4]
  push edx
  call FUN_0055f639
  add esp,12
complete:mov ecx,[esp+28]
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
static void prepare(const uint32_t *input)
{
 uint32_t j,a;DWORD old;memcpy(p,input,sizeof(p));used=stored=seen=0;active_self=SOUND;
 for(j=0;j<sizeof(regions)/sizeof(regions[0]);++j){CHECK(VirtualProtect((void *)(uintptr_t)regions[j][0],regions[j][1],PAGE_READWRITE,&old));memset((void *)(uintptr_t)regions[j][0],(uint8_t)(p[P_SEED]+j*29u),regions[j][1]);}
 U(0xd3b198)=p[P_SELECTED];B(0xe38dda)=(uint8_t)p[P_MODE];B(0xe38dca)=(uint8_t)p[P_DIRTY];U(0x6de510)=p[P_POPUP];U(0xe38ef0)=p[P_SAVED];U(0xe38ec8)=p[P_VOL0];U(0xe38ecc)=p[P_VOL1];U(0xe38ec4)=p[P_VOL2];U(0xe38ed0)=p[P_VOL3];put_flags(p[P_FLAGS]);U(0xe38e6c)=p[P_SPEED];U(0xe38e70)=p[P_SCROLL];U(0x605a3c)=p[P_LOCK];U(0xe39080)=p[P_DIFFICULTY];U(0xeaac28)=p[P_PLAYER];U(0x7848f8)=p[P_QUERYARG];U(0xe39140)=p[P_WINDOW];
 U(0x6057bc)=p[P_DEF0];U(0x6057b4)=p[P_DEF1];U(0x6057ac)=p[P_DEF2];U(0x6057a4)=p[P_DEF3];B(0x6057c0)=(uint8_t)(p[P_DEFFLAGS]>>8);B(0x6057b8)=(uint8_t)(p[P_DEFFLAGS]>>16);B(0x6057b0)=(uint8_t)p[P_DEFFLAGS];B(0x6057a8)=(uint8_t)(p[P_DEFFLAGS]>>24);U(0x6057a0)=p[P_DEFSPEED];U(0x60579c)=p[P_DEFSCROLL];
 for(j=0;j<2;++j){a=j?ALT+16u:SOUND;U(a+0x200)=p[P_DISABLED];U(a+0x1d8)=p[P_STREAM0];U(a+0x1dc)=p[P_STREAM1];U(a+0x1e0)=p[P_STREAM2];U(a+0x14c)=0xabc00000u+j;U(a+0x154)=0xabc00001u+j;U(a+0x158)=0xabc00002u+j;}
 for(j=0;j<5;++j)U(0x5d1960+j*4)=10007u+j*10009u;
}
static void run(const uint32_t *input)
{uint32_t j,out[6];prepare(input);for(j=0;j<p[P_REPEATS];++j){checked_call(p[P_OP],p,out);CHECK(out[0]==0x11111111&&out[1]==0x22222222&&out[2]==0x33333333&&out[3]==0x44444444&&out[4]==out[5]);}}
static void digest(char hex[65])
{HCRYPTHASH h;DWORD n=32;uint8_t bytes[32];uint32_t j;static const char digits[]="0123456789abcdef";CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));for(j=0;j<sizeof(regions)/sizeof(regions[0]);++j){CHECK(CryptHashData(h,(const BYTE *)regions[j],8,0));CHECK(CryptHashData(h,(const BYTE *)(uintptr_t)regions[j][0],regions[j][1],0));}CHECK(CryptGetHashParam(h,HP_HASHVAL,bytes,&n,0));CHECK(CryptDestroyHash(h));for(j=0;j<32;++j){hex[j*2]=digits[bytes[j]>>4];hex[j*2+1]=digits[bytes[j]&15];}hex[64]=0;}
int main(int argc,char **argv)
{
 uint32_t q[P_FIELDS]={0},j,k;char hex[65];SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(VirtualAlloc((void *)(uintptr_t)ALT,4096,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE));CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
 if(argc==2&&(!strcmp(argv[1],"--oracle-batch")||!strcmp(argv[1],"--oracle-plain-batch"))){plain_mode=!strcmp(argv[1],"--oracle-plain-batch");printf("{\"fields\":%u}\n",(uint32_t)P_FIELDS);fflush(stdout);for(;;){for(j=0;j<P_FIELDS;++j)if(scanf("%u",q+j)!=1)goto done;run(q);digest(hex);printf("{\"memory\":\"%s\",\"events\":[",hex);for(j=0;j<used;++j){if(j)putchar(',');putchar('[');for(k=0;k<32;++k){if(k)putchar(',');printf("%u",events[j][k]);}putchar(']');}printf("],\"writes\":[");for(j=0;j<stored;++j){if(j)putchar(',');printf("[%u,%u,%u]",writes[j][0],writes[j][1],writes[j][2]);}puts("]}");fflush(stdout);}}
 else{q[P_SEED]=1;q[P_REPEATS]=1;q[P_SPEED]=95;q[P_OP]=11;run(q);CHECK(U(0xe38e6c)==100&&B(0xe38dca)==1);q[P_OP]=2;q[P_SELECTED]=13;q[P_POPUP]=0xa5a50101u;run(q);CHECK(U(0xe38ef0)==0xa5a50101u&&B(0xe38dda)==1);q[P_OP]=7;q[P_SELECTED]=1;q[P_VOL1]=100;q[P_DISABLED]=1;run(q);CHECK(U(SOUND+0x154)==99);plain_mode=1;run(q);CHECK(U(SOUND+0x154)==99&&stored==0);puts("Settings callbacks: DWORD speed, popup confirmation and explicit sound receiver, traced and plain.");}
done:CHECK(CryptReleaseContext(provider,0));restore_data_close();return 0;
}

/* The shared stream module also defines open/get-volume helpers. This older
   settings corpus never reaches those backend calls; make unexpected use fail
   explicitly, while keeping both Debug and Release source-only links complete. */
int __cdecl _AIL_open_stream_12(uintptr_t a,uintptr_t b,uintptr_t c)
{(void)a;(void)b;(void)c;CHECK(0);return 0;}
int __cdecl _AIL_set_stream_loop_count_8(uintptr_t a,uintptr_t b)
{(void)a;(void)b;CHECK(0);return 0;}
int __cdecl _AIL_start_stream_4(uintptr_t a)
{(void)a;CHECK(0);return 0;}
int __cdecl _AIL_stream_volume_4(uintptr_t a)
{(void)a;CHECK(0);return 0;}
