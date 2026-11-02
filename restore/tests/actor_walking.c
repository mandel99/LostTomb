#include "actor_walking.h"
#include "actor_walking_case.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
#define U(a) (*(uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(uint32_t)(a))
#define WALK_OUT 0x31000000u
#define CELLS 51984u
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Actor walking failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t p[P_FIELDS],events[8192][32],used,seen,regions[32][2],region_count;
static HCRYPTPROV provider;
static const int32_t deltas[8]={-228,-227,1,229,228,227,-1,-229};
static const uint32_t fixed[][2]={{0xbcd5f0,776008},{0xc8ad38,500000},{0xd04e58,2016},{0xd056e8,32},{0xbcd5d8,32},{0xbca2c8,64},{0xaf598c,CELLS*4},{0xb350dc,CELLS*2},{0xb4e6fc,CELLS*2},{0xadc36c,CELLS},{0xec7630,CELLS},{0xebab20,CELLS},{0xee0c50,CELLS},{0xed4140,CELLS},{0x78d7c0,32},{0xeaaf60,256*224+16},{WALK_OUT,32}};
void actor_walking_actual_schedule(int32_t,uint32_t),actor_walking_actual_step(int32_t,int32_t),actor_walking_actual_direction(int32_t),actor_walking_actual_gate(int32_t),actor_walking_actual_commit(int32_t),actor_walking_actual_link(int32_t),actor_walking_actual_unlink(int32_t),actor_walking_actual_pixel(int32_t),actor_walking_actual_release(int32_t),actor_walking_actual_service(int32_t,int32_t),actor_walking_actual_empty(int32_t);
uint32_t actor_walking_actual_heading(int32_t,int32_t,int32_t,int32_t),actor_walking_actual_queue_count(const void *),actor_walking_actual_queue_full(const void *),actor_walking_actual_queue_push(void *,const int32_t *);
static uint32_t actor(uint32_t id){return 0xbcd600u+id*388u;}
static uint32_t queue(uint32_t index){return 0xeaaf68u+index*224u;}
static uint32_t tag(const void *pointer){uint32_t a=(uint32_t)(uintptr_t)pointer;return WALK_OUT<=a&&a<WALK_OUT+32u?a:0;}
static void observe(uint32_t fn,uint32_t phase,uint32_t v0,uint32_t v1,uint32_t v2,uint32_t v3,uint32_t result,int32_t *point)
{
 uint32_t *e,a=actor(p[P_ID]),other=actor(p[P_ALT]),b=0x7999d8u+p[P_BUILD]*264u,q=queue(p[P_QIDX]),n;
 CHECK(used<8192);e=events[used++];e[0]=fn;e[1]=phase;e[2]=v0;e[3]=v1;e[4]=v2;e[5]=v3;e[6]=result;e[7]=U(0xd056f8);e[8]=U(0xbcd5e8);e[9]=U(0xbca2d8);
 e[10]=B(a+0x158);e[11]=B(a+0x31);e[12]=B(a+9);e[13]=W(a+0x32);e[14]=W(a+0x34);e[15]=W(a+0x36);e[16]=U(a+0x18);e[17]=W(a+0xc);e[18]=W(a+0xe);e[19]=B(a+0x30);e[20]=B(a+2);e[21]=B(a+6);e[22]=B(other+0x31);e[23]=B(other+9);e[24]=U(other+0x18);e[25]=W(b+0x10);e[26]=B(b+0xca);e[27]=U(q+4);e[28]=U(q+8);e[29]=U(q+12);e[30]=B(a+0x6f);e[31]=B(a+0x71);
 if(fn==p[P_MFN]&&phase==p[P_MPHASE]&&++seen==p[P_MOCC]){
  n=p[P_MBITS];if(n&1u)U(0xd056f8)=p[P_NEWCURRENT];if(n&2u)B(a+0x158)=(uint8_t)p[P_NEWCOUNTER];if(n&4u)B(a+0x31)=(uint8_t)p[P_NEWPHASE];if(n&8u)B(a+9)=(uint8_t)p[P_NEWDIR];
  if(n&16u){B(a+2)=(uint8_t)p[P_NEWTYPE];B(a+0x6a)=(uint8_t)p[P_NEWUSAGE];B(a+0x6e)=(uint8_t)p[P_NEWWATER];}
  if(n&32u){W(a+0x32)=(uint16_t)p[P_NEWROUTE];W(a+0x34)=(uint16_t)p[P_NEWPOS];W(a+0x36)=(uint16_t)p[P_NEWLEN];}
  if(n&64u){W(a+0xc)=(uint16_t)p[P_NEWX];W(a+0xe)=(uint16_t)p[P_NEWY];U(a+0x18)=p[P_NEWTILE];}
  if(n&128u){uint32_t t=p[P_TILE]+(uint32_t)deltas[p[P_DIR]&7u];U(0xaf598cu+t*4u)=p[P_NEWTERRAIN];W(0xb4e6fcu+t*2u)=(uint16_t)p[P_PAIR];}
  if(n&256u){W(b+0x10)=(uint16_t)p[P_NEWBKIND];U(b+0xc4)=p[P_NEWPAIR];W(b+0x40)=(uint16_t)p[P_NEWOCC];W(0x799a18u+p[P_PAIR]*264u)=(uint16_t)p[P_NEWPOCC];B(b+0xca)=(uint8_t)p[P_NEWQIDX];W(b+0x2a)=(uint16_t)p[P_NEWOWNER];}
  if(n&512u){if(fn==0x4f6630u||fn==0x4f6650u||fn==0x4f65b0u)q=v0;U(q+4)=p[P_NEWQHEAD];U(q+8)=p[P_NEWQTAIL];U(q+12)=p[P_NEWQCAP];}
  if((n&1024u)&&point)*point=(int32_t)p[P_NEWVALUE];
  if(n&2048u){B(a+0x30)=(uint8_t)p[P_NEWSTATE];B(a+6)=(uint8_t)p[P_NEWLIFE];}
  if(n&4096u)U(0xbcd5e8)=p[P_NEWGLOBAL];
 }
}
#define OBS(fn,ph,a,b,c,d,r) observe(fn,ph,(uint32_t)(a),(uint32_t)(b),(uint32_t)(c),(uint32_t)(d),(uint32_t)(r),NULL)
#define WRAP1(name,actual,fn) void name(int32_t id){OBS(fn,0,id,0,0,0,0);actual(id);OBS(fn,1,id,0,0,0,0);}
void FUN_004a5da0(int32_t id,uint32_t speed){OBS(0x4a5da0,0,id,speed,0,0,0);actor_walking_actual_schedule(id,speed);OBS(0x4a5da0,1,id,speed,0,0,0);}
void FUN_004b27e0(int32_t id,int32_t count){OBS(0x4b27e0,0,id,count,0,0,0);actor_walking_actual_step(id,count);OBS(0x4b27e0,1,id,count,0,0,0);}
WRAP1(FUN_004b33d0,actor_walking_actual_direction,0x4b33d0)
WRAP1(FUN_004b3530,actor_walking_actual_gate,0x4b3530)
WRAP1(FUN_004b3c60,actor_walking_actual_commit,0x4b3c60)
WRAP1(FUN_004b52f0,actor_walking_actual_link,0x4b52f0)
WRAP1(FUN_004b5420,actor_walking_actual_unlink,0x4b5420)
WRAP1(FUN_004b3f00,actor_walking_actual_pixel,0x4b3f00)
WRAP1(FUN_004b3250,actor_walking_actual_release,0x4b3250)
void restore_actor_walking_empty(int32_t id){OBS(0x51f780,0,id,0,0,0,0);actor_walking_actual_empty(id);OBS(0x51f780,1,id,0,0,0,0);}
void restore_actor_walking_service(int32_t id,int32_t unused){OBS(0x4baa10,0,id,unused,0,0,0);actor_walking_actual_service(id,unused);OBS(0x4baa10,1,id,unused,0,0,0);}
uint32_t FUN_004baa10(int32_t id){(void)id;return p[P_RETVAL];}
uint32_t FUN_0051cba0(int32_t x,int32_t y,int32_t tx,int32_t ty){uint32_t r;OBS(0x51cba0,0,x,y,tx,ty,0);r=actor_walking_actual_heading(x,y,tx,ty);OBS(0x51cba0,1,x,y,tx,ty,r);return r;}
uint32_t restore_actor_walking_queue_count(const void *self){uint32_t r;OBS(0x4f6630,0,(uintptr_t)self,0,0,0,0);r=actor_walking_actual_queue_count(self);OBS(0x4f6630,1,(uintptr_t)self,0,0,0,r);return r;}
uint32_t restore_actor_walking_queue_full(const void *self){uint32_t r;OBS(0x4f6650,0,(uintptr_t)self,0,0,0,0);r=actor_walking_actual_queue_full(self);OBS(0x4f6650,1,(uintptr_t)self,0,0,0,r);return r;}
uint32_t restore_actor_walking_queue_push(void *self,const int32_t *value){uint32_t r;observe(0x4f65b0,0,(uint32_t)(uintptr_t)self,tag(value),value?(uint32_t)*value:0,value==NULL,0,(int32_t *)value);r=actor_walking_actual_queue_push(self,value);observe(0x4f65b0,1,(uint32_t)(uintptr_t)self,tag(value),value?(uint32_t)*value:0,value==NULL,r,(int32_t *)value);return r;}
void FUN_004b2d20(int32_t id){uint32_t a=actor((uint32_t)id);OBS(0x4b2d20,0,id,0,0,0,0);W(a+0x32)=(uint16_t)p[P_BUILD_ROUTE];W(a+0x34)=(uint16_t)p[P_BUILD_POS];W(a+0x36)=(uint16_t)p[P_BUILD_LEN];if((int16_t)p[P_BUILD_ROUTE]>0)W(0xd04e58u+p[P_BUILD_ROUTE]*2u)=(uint16_t)id;OBS(0x4b2d20,1,id,0,0,0,0);}
uint32_t FUN_004ad880(int32_t *x,int32_t *y){CHECK(x&&y&&x!=y);OBS(0x4ad880,0,tag(x),tag(y),*x,*y,0);*x=(int32_t)p[P_RELOCX];*y=(int32_t)p[P_RELOCY];OBS(0x4ad880,1,tag(x),tag(y),*x,*y,p[P_RETVAL]);return p[P_RETVAL];}
uint32_t FUN_004b3470(int32_t id,int32_t building){OBS(0x4b3470,0,id,building,0,0,0);OBS(0x4b3470,1,id,building,0,0,p[P_LOOK]);return p[P_LOOK];}
void FUN_004b1210(int32_t id,int32_t other){OBS(0x4b1210,0,id,other,0,0,0);OBS(0x4b1210,1,id,other,0,0,0);}
void FUN_004690f0(int32_t tile,int32_t amount){OBS(0x4690f0,0,tile,amount,0,0,0);OBS(0x4690f0,1,tile,amount,0,0,0);}
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) static void checked_call(uint32_t op,const uint32_t *q,uint32_t out[7])
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
  cmp eax,9
  je heading_call
  cmp eax,10
  jge queue_call
  cmp eax,0
  je schedule_call
  cmp eax,1
  je step_call
  push dword ptr[ecx+P_ID*4]
  cmp eax,2
  je direction_call
  cmp eax,3
  je gate_call
  cmp eax,4
  je commit_call
  cmp eax,5
  je link_call
  cmp eax,6
  je unlink_call
  cmp eax,7
  je pixel_call
  call FUN_004b3250
  jmp one_void
direction_call:call FUN_004b33d0
  jmp one_void
gate_call:call FUN_004b3530
  jmp one_void
commit_call:call FUN_004b3c60
  jmp one_void
link_call:call FUN_004b52f0
  jmp one_void
unlink_call:call FUN_004b5420
  jmp one_void
pixel_call:call FUN_004b3f00
one_void:add esp,4
  jmp void_done
schedule_call:push dword ptr[ecx+P_SPEED*4]
  push dword ptr[ecx+P_ID*4]
  call FUN_004a5da0
  add esp,8
  jmp void_done
step_call:push dword ptr[ecx+P_STEPS*4]
  push dword ptr[ecx+P_ID*4]
  call FUN_004b27e0
  add esp,8
  jmp void_done
heading_call:push dword ptr[ecx+P_TY*4]
  push dword ptr[ecx+P_TX*4]
  push dword ptr[ecx+P_Y*4]
  push dword ptr[ecx+P_X*4]
  call FUN_0051cba0
  add esp,16
  jmp complete
queue_call:cmp eax,13
  je empty_call
  mov edx,[ecx+P_QIDX*4]
  imul edx,224
  add edx,0eaaf68h
  cmp eax,10
  je count_call
  cmp eax,11
  je full_call
  mov eax,031000000h
  cmp dword ptr[ecx+P_NULLVALUE*4],0
  je push_value
  xor eax,eax
push_value:push eax
  push edx
  call restore_actor_walking_queue_push
  add esp,8
  jmp complete
count_call:push edx
  call restore_actor_walking_queue_count
  add esp,4
  jmp complete
full_call:push edx
  call restore_actor_walking_queue_full
  add esp,4
  jmp complete
empty_call:push dword ptr[ecx+P_ID*4]
  call restore_actor_walking_empty
  add esp,4
void_done:xor eax,eax
complete:mov ecx,[esp+28]
  mov [ecx],ebx
  mov [ecx+4],ebp
  mov [ecx+8],esi
  mov [ecx+12],edi
  mov [ecx+16],esp
  mov [ecx+24],eax
  pop edi
  pop esi
  pop ebx
  pop ebp
  ret
 }
}
#pragma warning(pop)
static void region(uint32_t a,uint32_t n,uint8_t fill)
{DWORD old;uint32_t j;for(j=0;j<region_count;++j)if(regions[j][0]==a&&regions[j][1]==n)return;CHECK(region_count<32);regions[region_count][0]=a;regions[region_count][1]=n;++region_count;CHECK(VirtualProtect((void *)(uintptr_t)a,n,PAGE_READWRITE,&old));memset((void *)(uintptr_t)a,fill,n);}
static void record(uint32_t id,uint8_t fill)
{
 uint32_t a=actor(id);memset((void *)(uintptr_t)(a-8u),fill,388);W(a)=0;B(a+2)=(uint8_t)p[P_TYPE];B(a+6)=(uint8_t)p[P_LIFE];B(a+8)=0x72;B(a+9)=(uint8_t)p[P_DIR];B(a+10)=(uint8_t)p[P_PREV];W(a+0xc)=(uint16_t)p[P_X];W(a+0xe)=(uint16_t)p[P_Y];W(a+0x10)=(uint16_t)p[P_PREVX];W(a+0x12)=(uint16_t)p[P_PREVY];U(a+0x18)=p[P_TILE];W(a+0x1c)=(uint16_t)p[P_TX];W(a+0x1e)=(uint16_t)p[P_TY];W(a+0x2e)=0x1234;B(a+0x30)=(uint8_t)p[P_STATE];B(a+0x31)=(uint8_t)p[P_PHASE];W(a+0x32)=(uint16_t)p[P_ROUTE];W(a+0x34)=(uint16_t)p[P_POS];W(a+0x36)=(uint16_t)p[P_LENGTH];B(a+0x39)=1;W(a+0x42)=(uint16_t)p[P_DRAWX];W(a+0x44)=(uint16_t)p[P_DRAWY];B(a+0x5d)=(uint8_t)p[P_HS];B(a+0x6a)=(uint8_t)p[P_USAGE];B(a+0x6e)=(uint8_t)p[P_WATER];B(a+0x6f)=(uint8_t)p[P_HT];B(a+0x70)=(uint8_t)p[P_HC];B(a+0x71)=(uint8_t)p[P_HG];B(a+0x82)=0x55;B(a+0x158)=(uint8_t)p[P_COUNTER];
}
static uint32_t run(const uint32_t *input)
{
 uint32_t j,k,a,b,t,selected[3],out[7],neighbor;memcpy(p,input,sizeof(p));used=seen=region_count=0;
 for(j=0;j<17;++j)region(fixed[j][0],fixed[j][1],j==0||(j>=6&&j<=13)?0:(uint8_t)(p[P_SEED]+j*29u));
 selected[0]=p[P_BUILD];selected[1]=p[P_PAIR];selected[2]=p[P_NEWPAIR];for(j=0;j<3;++j)region(0x7999d0u+selected[j]*264u,280,(uint8_t)(0x91u+j*17u));
 record(p[P_ID],0xcc);if(p[P_ALT]!=p[P_ID])record(p[P_ALT],0xb9);if(p[P_OWNER]!=p[P_ID]&&p[P_OWNER]!=p[P_ALT])record(p[P_OWNER],0xd7);
 for(j=0;j<1000;++j){memset((void *)(uintptr_t)(0xc8ad38u+j*500u),(uint8_t)p[P_PATH],500);W(0xd04e58u+j*2u)=0;}
 if((int16_t)p[P_ROUTE]>0)W(0xd04e58u+(uint32_t)(int32_t)(int16_t)p[P_ROUTE]*2u)=(uint16_t)p[P_ID];
 b=0x7999d8u+p[P_BUILD]*264u;W(b+0x10)=(uint16_t)p[P_BKIND];U(b+0xc4)=p[P_PAIR];W(b+0x40)=(uint16_t)p[P_OCC];B(b+0xca)=(uint8_t)p[P_QIDX];W(b+0x2a)=(uint16_t)p[P_OWNER];W(0x799a18u+p[P_PAIR]*264u)=(uint16_t)p[P_POCC];W(0x7999e8u+p[P_PAIR]*264u)=0x38;W(0x799a18u+p[P_NEWPAIR]*264u)=(uint16_t)p[P_POCC];
 for(j=0;j<256;++j){a=queue(j);U(a+4)=p[P_QHEAD];U(a+8)=p[P_QTAIL];U(a+12)=p[P_QCAP];for(k=0;k<51;++k)U(a+20+k*4)=(j<<16)^k^0xabc00000u;}
 for(j=0;j<8;++j){t=p[P_TILE]+(uint32_t)deltas[j];if(t<CELLS){U(0xaf598cu+t*4u)=p[P_TERRAIN];W(0xb4e6fcu+t*2u)=(uint16_t)p[P_BUILD];B(0xadc36cu+t)=(uint8_t)p[P_HEIGHT];B(0xec7630u+t)=(uint8_t)p[P_EC];B(0xebab20u+t)=(uint8_t)p[P_EB];B(0xee0c50u+t)=(uint8_t)p[P_EE];B(0xed4140u+t)=(uint8_t)p[P_ED];}}
 U(0xd056f8)=p[P_ID];U(0xbcd5e8)=p[P_GLOBAL];U(0xbca2d8)=p[P_MAPBASE];B(0x78d7cc)=(uint8_t)p[P_MODE];U(WALK_OUT)=p[P_QVAL];
 a=actor(p[P_ID]);neighbor=p[P_TILE]+(uint32_t)deltas[p[P_DIR]&7u];
 if(p[P_TILE]<CELLS)W(0xb350dcu+p[P_TILE]*2u)=(uint16_t)(p[P_GRAPH]==4?0:p[P_ID]);W(a)=0;
 if(p[P_CHAIN]){
  CHECK(p[P_CHAIN]<257&&p[P_ID]>p[P_CHAIN]);
  for(j=1;j<=p[P_CHAIN];++j){W(actor(j))=(uint16_t)(j<p[P_CHAIN]?j+1u:0u);U(actor(j)+0x18)=p[P_GRAPH]==3?neighbor:p[P_TILE];}
  if(p[P_GRAPH]==3)W(0xb350dcu+neighbor*2u)=1;
  else if(p[P_GRAPH]==6){W(a)=1;}
  else{W(0xb350dcu+p[P_TILE]*2u)=1;if(p[P_GRAPH]!=2)W(actor(p[P_CHAIN]))=(uint16_t)p[P_ID];}
 }
 for(j=0;j<p[P_REPEATS];++j){checked_call(p[P_OP],p,out);CHECK(out[0]==0x11111111&&out[1]==0x22222222&&out[2]==0x33333333&&out[3]==0x44444444&&out[4]==out[5]);}return out[6];
}
static void digest(char hex[65])
{HCRYPTHASH h;DWORD n=32;uint8_t bytes[32];uint32_t j;static const char digits[]="0123456789abcdef";CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));for(j=0;j<region_count;++j){CHECK(CryptHashData(h,(const BYTE *)regions[j],8,0));CHECK(CryptHashData(h,(const BYTE *)(uintptr_t)regions[j][0],regions[j][1],0));}CHECK(CryptGetHashParam(h,HP_HASHVAL,bytes,&n,0));CHECK(CryptDestroyHash(h));for(j=0;j<32;++j){hex[j*2]=digits[bytes[j]>>4];hex[j*2+1]=digits[bytes[j]&15];}hex[64]=0;}
int main(int argc,char **argv)
{
 uint32_t q[P_FIELDS]={0},j,k,r;char hex[65];SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(VirtualAlloc((void *)(uintptr_t)WALK_OUT,4096,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE));CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
 if(argc==2&&!strcmp(argv[1],"--oracle-batch")){printf("{\"fields\":%u}\n",(uint32_t)P_FIELDS);fflush(stdout);for(;;){for(j=0;j<P_FIELDS;++j)if(scanf("%u",q+j)!=1)goto done;r=run(q);digest(hex);printf("{\"return\":%u,\"memory\":\"%s\",\"events\":[",r,hex);for(j=0;j<used;++j){if(j)putchar(',');putchar('[');for(k=0;k<32;++k){if(k)putchar(',');printf("%u",events[j][k]);}putchar(']');}puts("]}");fflush(stdout);}}
 else{
  q[P_OP]=1;q[P_SEED]=1;q[P_ID]=257;q[P_ALT]=1000;q[P_STEPS]=1;q[P_PHASE]=7;q[P_DIR]=q[P_PATH]=2;q[P_TYPE]=4;q[P_LIFE]=1;q[P_X]=q[P_Y]=100;q[P_TILE]=22900;q[P_TX]=110;q[P_TY]=100;q[P_ROUTE]=q[P_POS]=1;q[P_LENGTH]=16;q[P_DRAWX]=q[P_DRAWY]=1500;q[P_BUILD]=257;q[P_PAIR]=1000;q[P_NEWPAIR]=1500;q[P_OWNER]=1999;q[P_QIDX]=3;q[P_QCAP]=51;q[P_REPEATS]=1;
  run(q);CHECK(U(actor(257)+0x18)==22901&&W(actor(257)+0xc)==101&&W(0xb350dc+22900*2)==0&&W(0xb350dc+22901*2)==257&&W(actor(257)+0x42)==1501);
  q[P_OP]=3;q[P_TERRAIN]=8;q[P_BKIND]=0x88;q[P_LOOK]=1;q[P_OCC]=q[P_POCC]=1;run(q);CHECK(U(queue(3)+8)==1&&U(queue(3)+20)==257&&B(actor(257)+0x30)==7&&W(actor(257)+0x32)==0);
  puts("Actor walking: real tile commit/grid chains/subpixel and explicit-receiver queue composition.");
 }
done:CHECK(CryptReleaseContext(provider,0));restore_data_close();return 0;
}
