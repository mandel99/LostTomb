#include "training_panel.h"
#include "training_case.h"
#include "standalone_data.h"
#include <stdio.h>
#include <string.h>
#include <wincrypt.h>
#define U(a) (*(uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(uint16_t *)(uintptr_t)(uint32_t)(a))
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Training panel failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
#define AUX 0x4ac20000u
static uint32_t p[P_FIELDS],events[4096][48],writes[512][3],used,stored,seen,plain_mode;static HCRYPTPROV provider;
static const uint32_t regions[][2]={{0xea4080,0x15000},{0x4ac24080,0x3000},{0x78d63c,0x1f0},{0x60597c,0x30},{0x7056b0,0x30},{0xf53ef8,0x30},{0xe20768,0x30},{0xe38d94,0x30},{0xf56f7c,0x60},{0xf571ec,0x30},{0xf55ba0,0x20}};
void training_trace_0055ad30(void),training_plain_0055ad30(void);
static int valid_player(uint32_t player){return player<4||player==65536;}
static uint32_t wa(uint32_t base,uint32_t player,uint32_t selected){CHECK(valid_player(player)&&selected<40);return base+player*0x49d8u+selected*2u;}
static uint32_t da(uint32_t base,uint32_t player,uint32_t selected){CHECK(valid_player(player)&&selected<40);return base+player*0x49d8u+selected*4u;}
static void set_record(uint32_t player,uint32_t selected,uint32_t mode,uint32_t target,uint32_t flag,uint32_t state,uint32_t value)
{W(wa(0xea4096,player,selected))=(uint16_t)mode;W(wa(0xea40de,player,selected))=(uint16_t)target;W(wa(0xea4126,player,selected))=(uint16_t)flag;U(da(0xea42f8,player,selected))=state;U(da(0xea6160,player,selected))=value;}
static uint32_t original_kind(void){return p[P_SELECTED]==9?1:p[P_SELECTED];}
static void observe(uint32_t fn,uint32_t phase,uint32_t count,const uint32_t *args,uint32_t result)
{
 uint32_t *e,j,n,player=U(0xeaac28),selected=U(0x60598c),kind=selected==9?1:selected,original=original_kind();CHECK(used<4096&&count<=9);e=events[used++];memset(e,0,48*4);e[0]=fn;e[1]=phase;e[2]=count;if(count)memcpy(e+3,args,count*4);e[12]=result;e[13]=selected;e[14]=player;e[15]=W(wa(0xea4096,player,selected));e[16]=W(wa(0xea40de,player,selected));e[17]=W(wa(0xea4126,player,selected));e[18]=U(da(0xea42f8,player,selected));e[19]=U(da(0xea6160,player,selected));e[20]=U(0x78d6dc+kind*4);e[21]=U(0x78d64c+kind*4);e[22]=kind;e[23]=U(0x7056c0);e[24]=U(0x7056c4);e[25]=U(0x7056c8);e[26]=U(0xf53f08);e[27]=U(0xf53f0c);e[28]=U(0xe38da8);e[29]=U(0xe38da4);e[30]=U(0xf56f8c);e[31]=U(0xf56fb4);e[32]=U(0xf571fc);e[33]=U(0xe20778);e[34]=U(0xea60a8+player*0x49d8u);e[35]=U(0x78d6e0);e[36]=U(0x78d700);e[37]=U(0x78d650);e[38]=original;e[39]=U(0x78d6dc+original*4);e[40]=U(0x78d64c+original*4);e[41]=W(wa(0xea4096,p[P_PLAYER],p[P_SELECTED]));e[42]=W(wa(0xea40de,p[P_PLAYER],p[P_SELECTED]));e[43]=W(wa(0xea4126,p[P_PLAYER],p[P_SELECTED]));e[44]=U(da(0xea42f8,p[P_PLAYER],p[P_SELECTED]));e[45]=U(da(0xea6160,p[P_PLAYER],p[P_SELECTED]));e[46]=U(0xea60a8+p[P_PLAYER]*0x49d8u);e[47]=p[P_CONTEXT];
 if(fn==p[P_MFN]&&phase==p[P_MPHASE]&&++seen==p[P_MOCC]){n=p[P_MBITS];if(n&1){U(0xf53f08)=p[P_NEWY];U(0xf53f0c)=p[P_NEWX];U(0xe38da8)=p[P_NEWVX];U(0xe38da4)=p[P_NEWVY];}if(n&2){CHECK(valid_player(p[P_NEWPLAYER])&&p[P_NEWSELECTED]<40);U(0xeaac28)=p[P_NEWPLAYER];U(0x60598c)=p[P_NEWSELECTED];}if(n&4)set_record(U(0xeaac28),U(0x60598c),p[P_NEWMODE],p[P_NEWTARGET],p[P_NEWFLAG],p[P_NEWSTATE],p[P_NEWVALUE]);if(n&8){j=p[P_NEWKIND];CHECK(j<40);U(0x78d6dc+j*4)=p[P_NEWCOUNT];U(0x78d64c+j*4)=p[P_NEWAVAILABLE];}if(n&16){U(0x7056c0)=p[P_NEWH0];U(0x7056c4)=p[P_NEWH1];U(0x7056c8)=p[P_NEWH2];}if(n&32)U(0xe20778)=p[P_NEWMEASURE];if(n&64){U(0xf56f8c)=p[P_NEWFONT];U(0xf56fb4)=p[P_NEWTITLEFONT];U(0xf571fc)=p[P_NEWICON];}}
}
static void noarg_observe(uint32_t fn,uint32_t phase){observe(fn,phase,0,NULL,0);}
void FUN_0055ad30(void){noarg_observe(0x55ad30,0);if(plain_mode)training_plain_0055ad30();else training_trace_0055ad30();noarg_observe(0x55ad30,1);}
void restore_training_panel_write(uint32_t a,uint32_t value){uint32_t j;for(j=0;j<sizeof(regions)/sizeof(regions[0]);++j)if(a>=regions[j][0]&&a+4<=regions[j][0]+regions[j][1])break;CHECK(j<sizeof(regions)/sizeof(regions[0])&&stored<512);writes[stored][0]=a;writes[stored][1]=4;writes[stored++][2]=value;U(a)=value;}
#define QUERY(name,index) uint32_t FUN_##name(int selected){uint32_t a=(uint32_t)selected,r=p[P_Q0+index];observe(0x##name,0,1,&a,0);observe(0x##name,1,1,&a,r);return r;}
QUERY(00443950,0) QUERY(004439c0,1) QUERY(00443a20,2) QUERY(00443b20,3) QUERY(00443b90,4)
static void draw(uint32_t fn,uint32_t count,const uint32_t *args,int text)
{observe(fn,0,count,args,0);if(text)U(0xe20778)+=p[P_DELTA];observe(fn,1,count,args,0);}
void FUN_00536820(int a,int b,int c,int d){uint32_t args[]={a,b,c,d};draw(0x536820,4,args,0);}
int FUN_0053e590(int image,int x,int y){uint32_t args[]={0xf55bb0,(uint32_t)image,(uint32_t)x,(uint32_t)y};observe(0x53e590,0,4,args,0);observe(0x53e590,1,4,args,p[P_RESULT]);return(int)p[P_RESULT];}
void FUN_004d62f0(uint32_t a,uint32_t b,int c,int d,uint32_t e,uint32_t f,int g,int h){uint32_t args[]={a,b,c,d,e,f,g,h};draw(0x4d62f0,8,args,1);}
void FUN_004d6250(uint32_t a,uint32_t b,int c,int d,uint32_t e,uint32_t f,uint32_t g,int h,int i){uint32_t args[]={a,b,c,d,e,f,g,h,i};draw(0x4d6250,9,args,1);}
void FUN_004d63d0(uint32_t a,int b,int c,uint32_t d,uint32_t e,uint32_t f,int g,int h){uint32_t args[]={a,b,c,d,e,f,g,h};draw(0x4d63d0,8,args,0);}
void FUN_004d6860(uint32_t a,uint32_t b,uint32_t c,int d,int e,uint32_t f,uint32_t g,int h,int i){uint32_t args[]={a,b,c,d,e,f,g,h,i};CHECK(b==64&&c==0x57c048);draw(0x4d6860,9,args,1);}
void FUN_004ccc30(uint32_t a,int b,int c,int d,uint32_t e,uint32_t f,uint32_t g){uint32_t args[]={a,b,c,d,e,f,g};CHECK(a==8&&b==56);draw(0x4ccc30,7,args,1);}
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) static void checked_call(uint32_t context,uint32_t out[7])
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
 push dword ptr[esp+20]
 call FUN_0055ad30
 mov edx,[esp]
 add esp,4
 mov ecx,[esp+24]
 mov [ecx],ebx
 mov [ecx+4],ebp
 mov [ecx+8],esi
 mov [ecx+12],edi
 mov [ecx+16],esp
 mov [ecx+24],edx
 pop edi
 pop esi
 pop ebx
 pop ebp
 ret
}}
#pragma warning(pop)
static void prepare(const uint32_t *q)
{static const uint32_t players[]={0,1,2,3,65536};uint32_t j,k,kind,player;DWORD old;memcpy(p,q,sizeof(p));used=stored=seen=0;CHECK(valid_player(p[P_PLAYER])&&p[P_SELECTED]<40);for(j=0;j<sizeof(regions)/sizeof(regions[0]);++j){CHECK(VirtualProtect((void *)(uintptr_t)regions[j][0],regions[j][1],PAGE_READWRITE,&old));memset((void *)(uintptr_t)regions[j][0],(uint8_t)(p[P_SEED]+j*29u),regions[j][1]);}for(j=0;j<5;++j){player=players[j];for(k=0;k<40;++k)set_record(player,k,p[P_MODE],p[P_TARGET],p[P_FLAG],p[P_STATE],p[P_VALUE]);set_record(player,p[P_SELECTED],p[P_MODE],p[P_TARGET],p[P_FLAG],p[P_STATE],p[P_VALUE]);}for(j=0;j<36;++j){U(0x78d6dc+j*4)=p[P_COUNT];U(0x78d64c+j*4)=p[P_AVAILABLE];}kind=original_kind();U(0x78d6dc+kind*4)=p[P_COUNT];U(0x78d64c+kind*4)=p[P_AVAILABLE];U(0xeaac28)=p[P_PLAYER];U(0x60598c)=p[P_SELECTED];U(0x7056c0)=p[P_H0];U(0x7056c4)=p[P_H1];U(0x7056c8)=p[P_H2];U(0xf53f08)=p[P_Y];U(0xf53f0c)=p[P_X];U(0xe38da8)=p[P_VX];U(0xe38da4)=p[P_VY];U(0xf56f8c)=p[P_FONT];U(0xf56fb4)=p[P_TITLEFONT];U(0xf571fc)=p[P_ICON];U(0xe20778)=p[P_MEASURE];}
static void run(const uint32_t *q){uint32_t j,out[7];prepare(q);for(j=0;j<p[P_REPEATS];++j){checked_call(p[P_CONTEXT],out);CHECK(out[0]==0x11111111&&out[1]==0x22222222&&out[2]==0x33333333&&out[3]==0x44444444&&out[4]==out[5]&&out[6]==p[P_CONTEXT]);}}
static void digest(char hex[65]){HCRYPTHASH h;DWORD n=32;uint8_t bytes[32];uint32_t j;static const char d[]="0123456789abcdef";CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));for(j=0;j<sizeof(regions)/sizeof(regions[0]);++j){CHECK(CryptHashData(h,(const BYTE *)regions[j],8,0));CHECK(CryptHashData(h,(const BYTE *)(uintptr_t)regions[j][0],regions[j][1],0));}CHECK(CryptGetHashParam(h,HP_HASHVAL,bytes,&n,0));CHECK(CryptDestroyHash(h));for(j=0;j<32;++j){hex[2*j]=d[bytes[j]>>4];hex[2*j+1]=d[bytes[j]&15];}hex[64]=0;}
int main(int argc,char **argv){uint32_t q[P_FIELDS]={0},j,k;char hex[65];SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(VirtualAlloc((void *)AUX,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE));CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));if(argc==2&&(!strcmp(argv[1],"--oracle-batch")||!strcmp(argv[1],"--oracle-plain-batch"))){plain_mode=!strcmp(argv[1],"--oracle-plain-batch");printf("{\"fields\":%u}\n",(uint32_t)P_FIELDS);fflush(stdout);for(;;){for(j=0;j<P_FIELDS;++j)if(scanf("%u",q+j)!=1)goto done;run(q);digest(hex);printf("{\"memory\":\"%s\",\"events\":[",hex);for(j=0;j<used;++j){if(j)putchar(',');putchar('[');for(k=0;k<48;++k){if(k)putchar(',');printf("%u",events[j][k]);}putchar(']');}printf("],\"writes\":[");for(j=0;j<stored;++j){if(j)putchar(',');printf("[%u,%u,%u]",writes[j][0],writes[j][1],writes[j][2]);}puts("]}");fflush(stdout);}}else{q[P_SEED]=q[P_CONTEXT]=q[P_REPEATS]=1;q[P_SELECTED]=9;q[P_COUNT]=2;q[P_AVAILABLE]=1;q[P_Q4]=1;q[P_FONT]=0x123456;q[P_TITLEFONT]=0x876543;q[P_ICON]=0x10000;q[P_DELTA]=257;run(q);CHECK(U(0xea60a8)==1&&used>20);plain_mode=1;q[P_PLAYER]=65536;q[P_CONTEXT]=0x80000100;run(q);CHECK(U(0x4ac260a8)==1&&stored==0);puts("Training panel: full globals, native ignored context, all typed text/query/sprite boundaries, traced/plain.");}done:CHECK(CryptReleaseContext(provider,0));restore_data_close();return 0;}
