#include "replay_mission.h"
#include "replay_case.h"
#include "family_menu.h"
#include "standalone_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <wincrypt.h>
#define U(a) (*(uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(uint32_t)(a))
#define ALT 0x31000000u
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Replay mission failure at line %d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t p[P_FIELDS],events[1024][32],writes[512][3],used,stored,seen,plain_mode,path_used;
static struct{uint32_t fn,phase;char hex[201];}paths[128];
static char *active_filename;static HCRYPTPROV provider;
static const uint32_t regions[][2]={{0xe38db0,0x130},{0x6059d8,0x80},{0x702798,0x40},{0x6fdc98,0x40},{0x6de500,0x30},{0xd3b110,0x40},{0xe37068,0x60},{0xeade90,0x80},{ALT,0x2000}};
void replay_trace_00418510(void),replay_plain_00418510(void),replay_trace_004181e0(void),replay_plain_004181e0(void);
char *replay_trace_005169c0(void),*replay_plain_005169c0(void);
static uint32_t bounded_length(const char *s){uint32_t n;for(n=0;n<100&&s[n];++n){}CHECK(n<100);return n;}
static uint32_t text_hash(const char *s){uint32_t h=2166136261u,j,n=bounded_length(s);for(j=0;j<n;++j)h=(h^(uint8_t)s[j])*16777619u;return h;}
static void make_text(char *s,uint32_t length,uint32_t style,uint32_t max)
{static const char *const alphabets[]={"ABCDEFGHIJKLMNOPQRSTUVWXYZ","abcdefghijklmnopqrstuvwxyz0123456789","Ab 9_-.","0123456789"};uint32_t j;CHECK(length<=max&&style<4);for(j=0;j<length;++j)s[j]=alphabets[style][j%(uint32_t)strlen(alphabets[style])];s[length]=0;}
static void path_observe(uint32_t fn,uint32_t phase,const char *s)
{uint32_t j,n=bounded_length(s);static const char d[]="0123456789abcdef";CHECK(path_used<128);paths[path_used].fn=fn;paths[path_used].phase=phase;for(j=0;j<n;++j){paths[path_used].hex[j*2]=d[(uint8_t)s[j]>>4];paths[path_used].hex[j*2+1]=d[(uint8_t)s[j]&15];}paths[path_used++].hex[n*2]=0;}
static void observe(uint32_t fn,uint32_t phase,uint32_t a,uint32_t b,uint32_t c,uint32_t d,uint32_t result)
{
 uint32_t *e,n;CHECK(used<1024);e=events[used++];e[0]=fn;e[1]=phase;e[2]=a;e[3]=b;e[4]=c;e[5]=d;e[6]=result;
 e[7]=U(0xe38ebc);e[8]=B(0x7027b4);e[9]=U(0x6059e8);e[10]=U(0x605a3c);e[11]=B(0x6de515);e[12]=B(0x6fdcb4);e[13]=B(0xe38dda);e[14]=B(0xe38dca);e[15]=U(0x7027a8);e[16]=U(0xd3b124);e[17]=U(0xd3b130);e[18]=U(0xd3b138);e[19]=text_hash((const char *)0xe38e74u);e[20]=text_hash((const char *)0xeadea4u);e[21]=text_hash((const char *)0xe37078u);e[22]=U(ALT+0x100c);e[23]=U(ALT+0x1010);e[24]=U(ALT+0x110c);e[25]=U(ALT+0x1110);e[26]=bounded_length((const char *)0xe38e74u);e[27]=bounded_length((const char *)0xeadea4u);e[28]=bounded_length((const char *)0xe37078u);e[29]=U(0xe38eb8);e[30]=U(0xe38ec0);e[31]=active_filename?text_hash(active_filename):0;
 if(fn==p[P_MFN]&&phase==p[P_MPHASE]&&++seen==p[P_MOCC]){
  n=p[P_MBITS];
  if(n&1u){U(0xe38ebc)=p[P_NEWKIND];B(0x7027b4)=(uint8_t)p[P_NEWCAMPAIGN];U(0x6059e8)=p[P_NEWCHOOSER];}
  if(n&2u){U(0x605a3c)=p[P_NEWEDITOR];B(0x6de515)=(uint8_t)p[P_NEWFLAG];B(0x6fdcb4)=(uint8_t)p[P_NEWFALLBACK];B(0xe38dda)=(uint8_t)p[P_NEWMODE];B(0xe38dca)=(uint8_t)p[P_NEWDIRTY];}
  if(n&4u){U(0x7027a8)=p[P_NEWSETUP];U(0xd3b124)=p[P_NEWSELECTION];U(0xd3b130)=p[P_NEWPLAYER];U(0xd3b138)=p[P_NEWPROGRESS];}
  if(n&8u)make_text((char *)0xe38e74u,p[P_NEWPROFILE_LENGTH],p[P_NEWPROFILE_STYLE],31);
  if(n&16u)make_text((char *)0xeadea4u,p[P_NEWPATH_LENGTH],p[P_NEWPATH_STYLE],36);
  if(n&32u)make_text((char *)0xe37078u,p[P_NEWMISSION_LENGTH],p[P_NEWMISSION_STYLE],32);
  if(n&64u){uint32_t stream=ALT+(p[P_STREAM]==2?0x1100u:0x1000u);U(stream+12)=p[P_NEWSTREAM_FLAG];U(stream+16)=p[P_NEWSTREAM_FILE];}
  if(n&128u){CHECK(active_filename);make_text(active_filename,p[P_NEWPATH_LENGTH],p[P_NEWPATH_STYLE],36);}
 }
}
#define OBS(f,ph,a,b,c,d,r) observe(f,ph,(uint32_t)(a),(uint32_t)(b),(uint32_t)(c),(uint32_t)(d),(uint32_t)(r))
void restore_replay_mission_write(uint32_t a,uint32_t width,uint32_t value)
{uint32_t j;for(j=0;j<sizeof(regions)/sizeof(regions[0]);++j)if(a>=regions[j][0]&&a+width<=regions[j][0]+regions[j][1])break;CHECK(j<sizeof(regions)/sizeof(regions[0])&&stored<512);writes[stored][0]=a;writes[stored][1]=width;writes[stored++][2]=width==1?value&255u:value;if(width==1)B(a)=(uint8_t)value;else{CHECK(width==4);U(a)=value;}}
void FUN_00418510(void){OBS(0x418510,0,0,0,0,0,0);if(plain_mode)replay_plain_00418510();else replay_trace_00418510();active_filename=NULL;OBS(0x418510,1,0,0,0,0,0);}
void FUN_004181e0(void){OBS(0x4181e0,0,0,0,0,0,0);if(plain_mode)replay_plain_004181e0();else replay_trace_004181e0();OBS(0x4181e0,1,0,0,0,0,0);}
char *FUN_005169c0(void){char *result;OBS(0x5169c0,0,0,0,0,0,0);result=plain_mode?replay_plain_005169c0():replay_trace_005169c0();OBS(0x5169c0,1,0,0,0,0,(uintptr_t)result);return result;}
undefined4 *FUN_00561a0a(undefined4 path,undefined4 mode)
{const char *name=(const char *)(uintptr_t)path;uint32_t result;CHECK(mode==0x5763bc&&strcmp((const char *)(uintptr_t)mode,"rb")==0);active_filename=(char *)name;path_observe(0x561a0a,0,name);OBS(0x561a0a,0,text_hash(name),bounded_length(name),mode,text_hash((const char *)(uintptr_t)mode),0);CHECK(p[P_STREAM]<=2);result=p[P_STREAM]?ALT+(p[P_STREAM]==2?0x1100u:0x1000u):0;path_observe(0x561a0a,1,name);OBS(0x561a0a,1,text_hash(name),bounded_length(name),mode,text_hash((const char *)(uintptr_t)mode),result);return(undefined4 *)(uintptr_t)result;}
uint32_t FUN_00561994(restore_legacy_FILE *stream)
{uint32_t address=(uint32_t)(uintptr_t)stream;CHECK(address==ALT+(p[P_STREAM]==2?0x1100u:0x1000u)&&p[P_STREAM]);OBS(0x561994,0,address,0,0,0,0);OBS(0x561994,1,address,0,0,0,p[P_CLOSERET]);return p[P_CLOSERET];}
uint32_t FUN_004d51b0(uint32_t path)
{const char *name=(const char *)(uintptr_t)path;CHECK(name==active_filename);path_observe(0x4d51b0,0,name);OBS(0x4d51b0,0,text_hash(name),bounded_length(name),0,0,0);path_observe(0x4d51b0,1,name);OBS(0x4d51b0,1,text_hash(name),bounded_length(name),0,0,p[P_LOADRET]);return p[P_LOADRET];}
void FUN_004cb6a0(void){OBS(0x4cb6a0,0,0,0,0,0,0);OBS(0x4cb6a0,1,0,0,0,0,0);}
void FUN_005405b0(char *name,char kind)
{CHECK(name==(char *)0xe37078u);path_observe(0x5405b0,0,name);OBS(0x5405b0,0,(uintptr_t)name,(uint8_t)kind,text_hash(name),bounded_length(name),0);path_observe(0x5405b0,1,name);OBS(0x5405b0,1,(uintptr_t)name,(uint8_t)kind,text_hash(name),bounded_length(name),0);}
void FUN_0041aa20(uint32_t first,uint32_t mode,int value){CHECK(first==0&&mode==53&&value==1000);OBS(0x41aa20,0,first,mode,value,0,0);OBS(0x41aa20,1,first,mode,value,0,0);}
int FUN_00418140(void){OBS(0x418140,0,0,0,0,0,0);OBS(0x418140,1,0,0,0,0,p[P_QUERYRET]);return(int)p[P_QUERYRET];}
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) static void checked_call(uint32_t op,uint32_t out[6])
{__asm{
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
 cmp eax,1
 je setup_call
 cmp eax,2
 je profile_call
 call FUN_00418510
 jmp complete
setup_call:call FUN_004181e0
 jmp complete
profile_call:call FUN_005169c0
complete:mov ecx,[esp+24]
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
}}
#pragma warning(pop)
static void prepare(const uint32_t *input)
{uint32_t j;DWORD old;memcpy(p,input,sizeof(p));used=stored=seen=path_used=0;active_filename=NULL;for(j=0;j<sizeof(regions)/sizeof(regions[0]);++j){CHECK(VirtualProtect((void *)(uintptr_t)regions[j][0],regions[j][1],PAGE_READWRITE,&old));memset((void *)(uintptr_t)regions[j][0],(uint8_t)(p[P_SEED]+j*29u),regions[j][1]);}U(0xe38ebc)=p[P_KIND];B(0x7027b4)=(uint8_t)p[P_CAMPAIGN];U(0x6059e8)=p[P_CHOOSER];U(0x605a3c)=p[P_EDITOR];B(0x6de515)=(uint8_t)p[P_FLAG];B(0x6fdcb4)=(uint8_t)p[P_FALLBACK];B(0xe38dda)=(uint8_t)p[P_MODE];B(0xe38dca)=(uint8_t)p[P_DIRTY];U(0x7027a8)=p[P_SETUP];U(0xd3b124)=p[P_SELECTION];U(0xd3b130)=p[P_PLAYER];U(0xd3b138)=p[P_PROGRESS];make_text((char *)0xe38e74u,p[P_PROFILE_LENGTH],p[P_PROFILE_STYLE],31);make_text((char *)0xe37078u,p[P_MISSION_LENGTH],p[P_MISSION_STYLE],32);strcpy((char *)0xeadea4u,"previous-profile-path");}
static void run(const uint32_t *input){uint32_t j,out[6];prepare(input);for(j=0;j<p[P_REPEATS];++j){checked_call(p[P_OP],out);CHECK(out[0]==0x11111111&&out[1]==0x22222222&&out[2]==0x33333333&&out[3]==0x44444444&&out[4]==out[5]);}}
static void digest(char hex[65]){HCRYPTHASH h;DWORD n=32;uint8_t bytes[32];uint32_t j;static const char d[]="0123456789abcdef";CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));for(j=0;j<sizeof(regions)/sizeof(regions[0]);++j){CHECK(CryptHashData(h,(const BYTE *)regions[j],8,0));CHECK(CryptHashData(h,(const BYTE *)(uintptr_t)regions[j][0],regions[j][1],0));}CHECK(CryptGetHashParam(h,HP_HASHVAL,bytes,&n,0));CHECK(CryptDestroyHash(h));for(j=0;j<32;++j){hex[j*2]=d[bytes[j]>>4];hex[j*2+1]=d[bytes[j]&15];}hex[64]=0;}
int main(int argc,char **argv)
{uint32_t v[P_FIELDS]={0},j,k;char hex[65];SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(sizeof(restore_legacy_FILE)==32&&offsetof(restore_legacy_FILE,_flag)==12&&offsetof(restore_legacy_FILE,_file)==16);CHECK(restore_data_open());CHECK(VirtualAlloc((void *)ALT,0x2000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE));CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
 if(argc==2&&(!strcmp(argv[1],"--oracle-batch")||!strcmp(argv[1],"--oracle-plain-batch"))){plain_mode=!strcmp(argv[1],"--oracle-plain-batch");printf("{\"fields\":%u}\n",(uint32_t)P_FIELDS);fflush(stdout);for(;;){for(j=0;j<P_FIELDS;++j)if(scanf("%u",v+j)!=1)goto done;run(v);digest(hex);printf("{\"memory\":\"%s\",\"events\":[",hex);for(j=0;j<used;++j){if(j)putchar(',');putchar('[');for(k=0;k<32;++k){if(k)putchar(',');printf("%u",events[j][k]);}putchar(']');}printf("],\"writes\":[");for(j=0;j<stored;++j){if(j)putchar(',');printf("[%u,%u,%u]",writes[j][0],writes[j][1],writes[j][2]);}printf("],\"paths\":[");for(j=0;j<path_used;++j){if(j)putchar(',');printf("[%u,%u,\"%s\"]",paths[j].fn,paths[j].phase,paths[j].hex);}puts("]}");fflush(stdout);}}
 else{v[P_SEED]=1;v[P_REPEATS]=1;v[P_PROFILE_LENGTH]=31;v[P_STREAM]=1;v[P_CAMPAIGN]=1;v[P_KIND]=0;v[P_MODE]=53;v[P_DIRTY]=2;run(v);CHECK(B(0xe38dda)==1&&B(0xe38dca)==1&&path_used==4);v[P_STREAM]=0;run(v);CHECK(B(0xe38dda)==53&&B(0xe38dca)==2);v[P_CAMPAIGN]=0;v[P_QUERYRET]=0x89abcdef;v[P_SELECTION]=0x12345678;run(v);CHECK(U(0xd3b124)==0x89abcdef&&U(0xd3b130)==0&&U(0xd3b138)==0);plain_mode=1;v[P_KIND]=256;run(v);CHECK(B(0xe38dda)==1&&B(0x7027b4)==0&&stored==0);puts("Replay mission: native filename, legacy FILE boundary, full DWORD kind and campaign fallback, traced/plain.");}
done:CHECK(CryptReleaseContext(provider,0));restore_data_close();return 0;}
