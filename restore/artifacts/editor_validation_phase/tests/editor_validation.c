#include "editor_validation.h"
#include "editor_case.h"
#include "editor_points.h"
#include "standalone_data.h"
#include "restore_types.h"
#include <wincrypt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define U(a) (*(uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(uint32_t)(a))
#define ALT 0x31000000u
#define CELLS 51984u
#define CLASS_BASE 0x5e4c10u
#define CLASS_SIZE (1100u*48u+64u)
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Editor validation failure at line %d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t p[P_FIELDS],events[4096][32],writes[2048][3],used,stored,seen,plain_mode,cache_seed,cache_ready;
static uint32_t terrain_pattern[CELLS];static uint8_t class_pattern[CLASS_SIZE];static HCRYPTPROV provider;
static const uint32_t regions[][2]={{0x784b40,0x200},{0xbca2c8,0x40},{0xaf597c,CELLS*4+32},{CLASS_BASE,CLASS_SIZE},{0x77f738,200*98+32},{0x6057b0,0x2b0},{0xe20750,0x30},{0xe20980,0x30},{0xe38dc0,0x100},{0xd3b1a0,0x30},{0xf53f20,0x30},{0xe36ee0,0x30},{ALT,0x10000}};
static const char *const filenames[]={"Alpha.map","Beta.extra.map",".hidden.map","no_extension","Gamma.MAP","zeta.map","  spaced.name.map","Last."};
static const char *const profiles[]={"Restore","Alpha.Beta","","City"};
#define DECLARE(v) \
void editor_##v##_00409830(void); \
uint8_t editor_##v##_004ec360(void),editor_##v##_004095c0(void),editor_##v##_00409650(void),editor_##v##_004096e0(void),editor_##v##_00409760(void),editor_##v##_004097c0(void),editor_##v##_00409620(uint32_t); \
uint32_t editor_##v##_004ec410(uint32_t,char);bool editor_##v##_00409820(void);int editor_##v##_00443710(void); \
void editor_##v##_004cdff0(uint32_t,char),editor_##v##_004ce3a0(char *)
DECLARE(trace);DECLARE(plain);
static int32_t coordinate(uint32_t address,uint32_t width){return width==2?(int32_t)(int16_t)W(address):(int32_t)U(address);}
static int active(uint32_t group,int32_t x,int32_t y){return group==3||group==4?x>0||y>0:x>=0&&y>=0;}
static uint32_t cell(uint32_t base,int32_t x,int32_t y){return base+(uint32_t)x+(uint32_t)y*228u;}
static uint32_t allowed_address(uint32_t scene,uint32_t variant,int mode)
{uint32_t a=mode?0x5e4c24u+(scene&255u)*48u:0x5e4cb4u+((uint32_t)(int32_t)(int16_t)variant+(scene&255u)*2u)*48u;CHECK(a>=CLASS_BASE&&a+4<=CLASS_BASE+CLASS_SIZE);return a;}
static void set_coordinate(uint32_t index,uint32_t x,uint32_t y){const EditorPoint *q;CHECK(index<36);q=points+index;if(q->width==2){W(q->x)=(uint16_t)x;W(q->y)=(uint16_t)y;}else{U(q->x)=x;U(q->y)=y;}}
static void record(uint32_t index,uint32_t type,uint32_t enabled,uint32_t shape,uint32_t extra,uint32_t codes8,uint32_t codes6)
{uint32_t a,j,nibble;CHECK(index<200);a=0x77f748u+index*98u;memset((void *)(uintptr_t)a,0xa6,98);B(a)=(uint8_t)type;B(a+1)=(uint8_t)enabled;B(a+0x18)=(uint8_t)shape;B(a+0x1a)=(uint8_t)extra;for(j=0;j<14;++j){nibble=j<8?(codes8>>(j*4))&15u:(codes6>>((j-8)*4))&15u;B(a+0x1e+j)=(uint8_t)(nibble==15?p[P_BADCODE]:nibble);}}
static uint32_t string_hash(const char *s){uint32_t h=2166136261u,j;for(j=0;j<256&&s[j];++j)h=(h^(uint8_t)s[j])*16777619u;CHECK(j<256);return h;}
static void observe(uint32_t fn,uint32_t phase,uint32_t a,uint32_t b,uint32_t c,uint32_t d,uint32_t result)
{
 uint32_t *e,n,index=p[P_FOCUS],tile=UINT32_MAX;const EditorPoint *q;int32_t x,y;CHECK(index<36);q=points+index;x=coordinate(q->x,q->width);y=coordinate(q->y,q->width);if(active(q->group,x,y))tile=cell(U(0xbca2d8),x,y);
 CHECK(used<4096);e=events[used++];e[0]=fn;e[1]=phase;e[2]=a;e[3]=b;e[4]=c;e[5]=d;e[6]=result;e[7]=U(0xbca2d8);e[8]=U(0x784cb0);e[9]=W(0x784b94);e[10]=U(0x605a3c);e[11]=B(0xe38dda);e[12]=B(0xe38dca);e[13]=U(0xe20998);e[14]=B(0xd3b1c0);e[15]=U(0xf53f2c);e[16]=U(0xe20768);e[17]=U(0xe20764);e[18]=U(0x6057d0);e[19]=U(0x6059f8);e[20]=U(0x6059f4);e[21]=U(0x6059f0);e[22]=(uint32_t)x;e[23]=(uint32_t)y;e[24]=tile<CELLS?U(0xaf598cu+tile*4u):0;e[25]=U(allowed_address(U(0x784cb0),W(0x784b94),0));e[26]=U(allowed_address(U(0x784cb0),W(0x784b94),1));e[27]=B(0xd3b1b3);e[28]=U(0xe36efc);e[29]=U(0x6057cc);e[30]=U(0x6057d8);e[31]=tile;
 if(fn==p[P_MFN]&&phase==p[P_MPHASE]&&++seen==p[P_MOCC]){
  n=p[P_MBITS];if(n&1u)U(0xbca2d8)=p[P_NEWBASE];if(n&2u)set_coordinate(p[P_NEWPOINT],p[P_NEWX],p[P_NEWY]);
  if(n&4u){q=points+p[P_NEWPOINT];x=coordinate(q->x,q->width);y=coordinate(q->y,q->width);tile=cell(U(0xbca2d8),x,y);CHECK(tile<CELLS);U(0xaf598cu+tile*4u)=p[P_NEWTERRAIN];}
  if(n&8u){U(0x784cb0)=p[P_NEWSCENE];W(0x784b94)=(uint16_t)p[P_NEWVARIANT];U(0x605a3c)=p[P_NEWEDITOR];U(allowed_address(p[P_NEWSCENE],p[P_NEWVARIANT],0))=p[P_NEWALLOW0];U(allowed_address(p[P_NEWSCENE],p[P_NEWVARIANT],1))=p[P_NEWALLOW1];}
  if(n&16u){U(0xe20768)=p[P_NEWTITLE];U(0xe20764)=p[P_NEWBODY];}
  if(n&32u){B(0xe38dda)=(uint8_t)p[P_NEWMODE];B(0xe38dca)=(uint8_t)p[P_NEWDIRTY];B(0xd3b1c0)=(uint8_t)p[P_NEWFLAG];U(0xe20998)=p[P_NEWVALUE];U(0xf53f2c)=p[P_NEWCOUNTER];}
  if(n&64u)record(p[P_NEWRECORD],p[P_NEWREC0],p[P_NEWREC1],p[P_NEWSHAPE],p[P_NEWEXTRAPRESENT],p[P_NEWCODES8],p[P_NEWCODES6]);
  if(n&128u){CHECK((int32_t)p[P_NEWFILECOUNT]<=8);U(0x6057d0)=p[P_NEWFILECOUNT];}
 }
}
#define OBS(f,ph,a,b,c,d,r) observe(f,ph,(uint32_t)(a),(uint32_t)(b),(uint32_t)(c),(uint32_t)(d),(uint32_t)(r))
void restore_editor_validation_write(uint32_t a,uint32_t width,uint32_t value)
{uint32_t j;for(j=0;j<sizeof(regions)/sizeof(regions[0]);++j)if(a>=regions[j][0]&&a+width<=regions[j][0]+regions[j][1])break;CHECK(j<sizeof(regions)/sizeof(regions[0]));CHECK(stored<2048);writes[stored][0]=a;writes[stored][1]=width;writes[stored++][2]=width==1?value&255u:value;if(width==1)B(a)=(uint8_t)value;else{CHECK(width==4);U(a)=value;}}
void FUN_00409830(void){OBS(0x409830,0,0,0,0,0,0);if(plain_mode)editor_plain_00409830();else editor_trace_00409830();OBS(0x409830,1,0,0,0,0,0);}
#define BYTE0(a) uint8_t FUN_##a(void){uint8_t r;OBS(0x##a,0,0,0,0,0,0);r=plain_mode?editor_plain_##a():editor_trace_##a();OBS(0x##a,1,0,0,0,0,r);return r;}
BYTE0(004ec360) BYTE0(004095c0) BYTE0(00409650) BYTE0(004096e0) BYTE0(00409760) BYTE0(004097c0)
bool FUN_00409820(void){bool r;OBS(0x409820,0,0,0,0,0,0);r=plain_mode?editor_plain_00409820():editor_trace_00409820();OBS(0x409820,1,0,0,0,0,r);return r;}
uint8_t FUN_00409620(uint32_t terrain){uint8_t r;OBS(0x409620,0,terrain,0,0,0,0);r=plain_mode?editor_plain_00409620(terrain):editor_trace_00409620(terrain);OBS(0x409620,1,terrain,0,0,0,r);return r;}
uint32_t FUN_004ec410(uint32_t terrain,char mode){uint32_t r;OBS(0x4ec410,0,terrain,(uint8_t)mode,0,0,0);r=plain_mode?editor_plain_004ec410(terrain,mode):editor_trace_004ec410(terrain,mode);OBS(0x4ec410,1,terrain,(uint8_t)mode,0,0,r);return r;}
int FUN_00443710(void){int r;OBS(0x443710,0,0,0,0,0,0);r=plain_mode?editor_plain_00443710():editor_trace_00443710();OBS(0x443710,1,0,0,0,0,r);CHECK(r>=0&&r<=8);return r;}
void FUN_004cdff0(uint32_t unused,char maps){OBS(0x4cdff0,0,unused,(uint8_t)maps,0,0,0);if(plain_mode)editor_plain_004cdff0(unused,maps);else editor_trace_004cdff0(unused,maps);OBS(0x4cdff0,1,unused,(uint8_t)maps,0,0,0);}
void FUN_004ce3a0(char *name){OBS(0x4ce3a0,0,(uintptr_t)name,0,0,0,0);if(plain_mode)editor_plain_004ce3a0(name);else editor_trace_004ce3a0(name);OBS(0x4ce3a0,1,(uintptr_t)name,0,0,0,0);}
void FUN_004cc110(int group,int entry){OBS(0x4cc110,0,group,entry,0,0,0);CHECK(group==5&&entry>=0&&entry<256);U(0xe20768)=ALT+0x6000u+(uint32_t)entry*64u;OBS(0x4cc110,1,group,entry,0,0,0);}
void FUN_004cc160(int group,int entry){OBS(0x4cc160,0,group,entry,0,0,0);CHECK(group==5&&entry>=0&&entry<256);U(0xe20764)=ALT+0x6000u+(uint32_t)entry*64u;OBS(0x4cc160,1,group,entry,0,0,0);}
int FUN_00538cd0(uint32_t first,uint32_t second,uint32_t third){CHECK(first>=ALT&&first<ALT+0x10000&&second>=ALT&&second<ALT+0x10000);OBS(0x538cd0,0,first,second,third,0,0);(void)string_hash((const char *)(uintptr_t)first);(void)string_hash((const char *)(uintptr_t)second);OBS(0x538cd0,1,first,second,third,0,p[P_RESULT]);return(int)p[P_RESULT];}
/* Sort is an explicit endpoint. This identity must never execute in this oracle. */
int FUN_00562370(const char *first,const char *second){(void)first;(void)second;CHECK(0);return 0;}
uint8_t FUN_0041c740(RestoreFileList *self,const char *pattern,char flags)
{uint32_t j;CHECK(self==RESTORE_SAVE_FILES&&(int32_t)p[P_FILECOUNT]<=8);OBS(0x41c740,0,(uintptr_t)self,string_hash(pattern),(uint8_t)flags,(uintptr_t)pattern==0x576260u?0x576260u:0,0);self->offsets=(uint32_t *)(ALT+0x1000);self->count=(int32_t)p[P_FILECOUNT];self->capacity=8;self->strings=(char *)(ALT+0x2000);self->string_capacity=512;self->used=self->count>0?self->count*64:0;for(j=0;j<8;++j){self->offsets[j]=j*64;strcpy(self->strings+j*64,filenames[j]);}OBS(0x41c740,1,(uintptr_t)self,string_hash(pattern),(uint8_t)flags,(uintptr_t)pattern==0x576260u?0x576260u:0,p[P_ENUMRET]&255u);return(uint8_t)p[P_ENUMRET];}
void FUN_0041ca10(RestoreFileList *self,RestoreFileCompare compare){CHECK(self==RESTORE_SAVE_FILES&&compare==FUN_00562370);OBS(0x41ca10,0,(uintptr_t)self,0x562370,0,0,0);OBS(0x41ca10,1,(uintptr_t)self,0x562370,0,0,0);}
undefined4 *FUN_005169c0(void){OBS(0x5169c0,0,0,0,0,0,0);OBS(0x5169c0,1,0,0,0,0,ALT+0x3000);return(undefined4 *)(ALT+0x3000);}
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
  cmp eax,8
  jge parameter_call
  cmp eax,0
  je controller_call
  cmp eax,1
  je entry_call
  cmp eax,2
  je water1_call
  cmp eax,3
  je portal1_call
  cmp eax,4
  je portal2_call
  cmp eax,5
  je land_call
  cmp eax,6
  je water2_call
  call FUN_00409820
  jmp byte_done
entry_call:call FUN_004ec360
  jmp byte_done
water1_call:call FUN_004095c0
  jmp byte_done
portal1_call:call FUN_00409650
  jmp byte_done
portal2_call:call FUN_004096e0
  jmp byte_done
land_call:call FUN_00409760
  jmp byte_done
water2_call:call FUN_004097c0
byte_done:movzx eax,al
  jmp complete
controller_call:call FUN_00409830
  jmp void_done
parameter_call:cmp eax,10
  je counter_call
  cmp eax,11
  je selector_call
  cmp eax,12
  je trim_call
  cmp eax,9
  je mask_call
  push dword ptr[ecx+P_TERRAIN*4]
  call FUN_00409620
  add esp,4
  jmp byte_done
mask_call:push dword ptr[ecx+P_KIND*4]
  push dword ptr[ecx+P_TERRAIN*4]
  call FUN_004ec410
  add esp,8
  jmp complete
counter_call:call FUN_00443710
  jmp complete
selector_call:push dword ptr[ecx+P_MAPS*4]
  push dword ptr[ecx+P_ARGUMENT*4]
  call FUN_004cdff0
  add esp,8
  jmp void_done
trim_call:mov eax,[ecx+P_FILENAME*4]
  shl eax,6
  add eax,031002000h
  push eax
  call FUN_004ce3a0
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
static void prepare(const uint32_t *input)
{
 uint32_t j,a,g,bit,t,terrain,base,sp;int32_t x,y;DWORD old;const EditorPoint *q;memcpy(p,input,sizeof(p));used=stored=seen=0;
 for(j=0;j<sizeof(regions)/sizeof(regions[0]);++j){CHECK(VirtualProtect((void *)(uintptr_t)regions[j][0],regions[j][1],PAGE_READWRITE,&old));memset((void *)(uintptr_t)regions[j][0],(uint8_t)(p[P_SEED]+j*29u),regions[j][1]);}
 if(!cache_ready||cache_seed!=p[P_SEED]){cache_ready=1;cache_seed=p[P_SEED];for(j=0;j<CELLS;++j)terrain_pattern[j]=(j*0x9e3779b9u)^((j>>3)*0x85ebca6bu)^p[P_SEED]*0x1234567u;memset(class_pattern,(uint8_t)(p[P_SEED]+3u*29u),sizeof(class_pattern));for(j=0;j<1100;++j)*(uint32_t *)(class_pattern+20+j*48)=(j*0x9e3779b9u)^p[P_SEED]*0x13579bdu;}
 memcpy((void *)0xaf598cu,terrain_pattern,sizeof(terrain_pattern));memcpy((void *)(uintptr_t)CLASS_BASE,class_pattern,sizeof(class_pattern));memset((void *)0x77f748u,0,200*98);
 for(j=0;j<36;++j){bit=j<32?(p[P_ACTIVE0]>>j)&1u:(p[P_ACTIVE1]>>(j-32))&1u;set_coordinate(j,bit?3+j*3u:UINT32_MAX,bit?10u:UINT32_MAX);}
 if(p[P_OVERRIDE]!=UINT32_MAX)set_coordinate(p[P_OVERRIDE],p[P_X],p[P_Y]);
 U(0xbca2d8)=p[P_BASE];U(0x784cb0)=p[P_SCENE];W(0x784b94)=(uint16_t)p[P_VARIANT];U(0x605a3c)=p[P_EDITOR];U(allowed_address(p[P_SCENE],p[P_VARIANT],0))=p[P_ALLOW0];U(allowed_address(p[P_SCENE],p[P_VARIANT],1))=p[P_ALLOW1];
 for(j=0;j<36;++j){q=points+j;x=coordinate(q->x,q->width);y=coordinate(q->y,q->width);if(!active(q->group,x,y))continue;g=q->group;terrain=g==0?p[P_T0]:g==1?p[P_T1]:g==2||g==4||g==6?p[P_TWATER]:p[P_TPASS];bit=j<32?(p[P_FAIL0]>>j)&1u:(p[P_FAIL1]>>(j-32))&1u;if(bit)terrain=g==2||g==4||g==6?p[P_BADWATER]:p[P_BADCLASS];base=p[P_BASE];t=cell(base,x,y);CHECK(t<CELLS);U(0xaf598cu+t*4u)=terrain;sp=cell(p[P_NEWBASE],x,y);if(sp<CELLS)U(0xaf598cu+sp*4u)=p[P_NEWTERRAIN];}
 if(p[P_RECORD]!=UINT32_MAX)record(p[P_RECORD],p[P_REC0],p[P_REC1],p[P_SHAPE],p[P_EXTRAPRESENT],p[P_CODES8],p[P_CODES6]);for(j=0;j<8;++j)if(p[P_SPREAD]&(1u<<j))record((23u*j)%200u,1,1,0,0,j+1u,0);
 B(0xe38dda)=(uint8_t)p[P_MODE];B(0xe38dca)=(uint8_t)p[P_DIRTY];B(0xd3b1c0)=(uint8_t)p[P_FLAG];U(0xe20998)=p[P_VALUE];U(0xf53f2c)=p[P_COUNTER];U(0xe20768)=p[P_TITLE];U(0xe20764)=p[P_BODY];
 U(0x6057cc)=ALT+0x1000;U(0x6057d0)=p[P_FILECOUNT];U(0x6057d4)=8;U(0x6057d8)=ALT+0x2000;U(0x6057dc)=512;U(0x6057e0)=(int32_t)p[P_FILECOUNT]>0?p[P_FILECOUNT]*64u:0;CHECK((int32_t)p[P_FILECOUNT]<=8&&p[P_PROFILE]<4&&p[P_FILENAME]<8);
 for(j=0;j<8;++j){U(ALT+0x1000+j*4)=j*64;strcpy((char *)(ALT+0x2000+j*64),filenames[j]);}strcpy((char *)(ALT+0x3000),profiles[p[P_PROFILE]]);
 for(j=0;j<256;++j){a=ALT+0x6000+j*64;sprintf((char *)(uintptr_t)a,"line-5-%u",j);}strcpy((char *)(ALT+0xe000),"mutated-title");strcpy((char *)(ALT+0xe100),"mutated-body");
}
static uint32_t run(const uint32_t *input){uint32_t j,out[7];prepare(input);for(j=0;j<p[P_REPEATS];++j){checked_call(p[P_OP],p,out);CHECK(out[0]==0x11111111&&out[1]==0x22222222&&out[2]==0x33333333&&out[3]==0x44444444&&out[4]==out[5]);}return out[6];}
static void digest(char hex[65]){HCRYPTHASH h;DWORD n=32;uint8_t b[32];uint32_t j;static const char d[]="0123456789abcdef";CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));for(j=0;j<sizeof(regions)/sizeof(regions[0]);++j){CHECK(CryptHashData(h,(const BYTE *)regions[j],8,0));CHECK(CryptHashData(h,(const BYTE *)(uintptr_t)regions[j][0],regions[j][1],0));}CHECK(CryptGetHashParam(h,HP_HASHVAL,b,&n,0));CHECK(CryptDestroyHash(h));for(j=0;j<32;++j){hex[2*j]=d[b[j]>>4];hex[2*j+1]=d[b[j]&15];}hex[64]=0;}
int main(int argc,char **argv)
{
 uint32_t v[P_FIELDS]={0},j,k,result;char hex[65];SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(VirtualAlloc((void *)ALT,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE));CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
 if(argc==2&&(!strcmp(argv[1],"--oracle-batch")||!strcmp(argv[1],"--oracle-plain-batch"))){plain_mode=!strcmp(argv[1],"--oracle-plain-batch");printf("{\"fields\":%u}\n",(uint32_t)P_FIELDS);fflush(stdout);for(;;){for(j=0;j<P_FIELDS;++j)if(scanf("%u",v+j)!=1)goto done;result=run(v);digest(hex);printf("{\"return\":%u,\"memory\":\"%s\",\"events\":[",result,hex);for(j=0;j<used;++j){if(j)putchar(',');putchar('[');for(k=0;k<32;++k){if(k)putchar(',');printf("%u",events[j][k]);}putchar(']');}printf("],\"writes\":[");for(j=0;j<stored;++j){if(j)putchar(',');printf("[%u,%u,%u]",writes[j][0],writes[j][1],writes[j][2]);}puts("]}");fflush(stdout);}}
 else{v[P_SEED]=1;v[P_REPEATS]=1;v[P_OVERRIDE]=v[P_RECORD]=UINT32_MAX;v[P_SCENE]=2;v[P_NEWSCENE]=3;v[P_NEWBASE]=2280;v[P_NEWTERRAIN]=1;v[P_TWATER]=4;v[P_BADCLASS]=4;v[P_REC0]=v[P_REC1]=1;v[P_BADCODE]=255;v[P_TITLE]=ALT+0xe000;v[P_BODY]=ALT+0xe100;v[P_MAPS]=1;v[P_FOCUS]=0;run(v);CHECK(B(0xe38dda)==16&&B(0xd3b1c0)==0&&U(0xe20998)==0&&B(0xe38dca)==1);v[P_ACTIVE0]=v[P_FAIL0]=1;run(v);CHECK(U(0xe20768)==ALT+0x6000+0x6c*64&&U(0xe20764)==ALT+0x6000+0x6d*64);v[P_OP]=9;v[P_EDITOR]=1;v[P_TERRAIN]=0x10000000;result=run(v);CHECK(result==0x10000000);plain_mode=1;result=run(v);CHECK(result==0x10000000&&stored==0);puts("Editor validation: actual map predicates, failure labels, selector success and full raw-EAX terrain contract, traced/plain.");}
done:CHECK(CryptReleaseContext(provider,0));restore_data_close();return 0;
}
