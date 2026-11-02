#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "menu_text_glyph.h"
#include "text_case.h"
#include "standalone_data.h"
#include <stdio.h>
#include <string.h>
#include <wincrypt.h>
#define U(a) (*(uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(uint32_t)(a))
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Text glyph fixture failure %d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t p[P_FIELDS],events[8192][TG_EVENT_FIELDS],used,result;
static uint8_t original_map[288];static HCRYPTPROV provider;
static void put(uint32_t a,uint32_t v,uint32_t n){if(n==1)B(a)=(uint8_t)v;else if(n==2)W(a)=(uint16_t)v;else U(a)=v;}
static uint32_t get(uint32_t a,uint32_t n){return n==1?B(a):n==2?W(a):U(a);}
static void mutation(uint32_t phase)
{
 if(p[P_MUTPHASE]==phase&&(!p[P_MUTORD]||p[P_MUTORD]==used+1)){
  uint32_t j=p[P_MUTFIELD];CHECK(j<TG_FIELDS);put(tg_fields[j][0],p[P_MUTVALUE],tg_fields[j][1]);
 }
}
int FUN_0053e590(int image,int x,int y)
{
 uint32_t j,*e;CHECK(used<8192);mutation(1);e=events[used];e[0]=0x53e590;e[1]=0xf55bb0;e[2]=(uint32_t)image;e[3]=(uint32_t)x;e[4]=(uint32_t)y;
 for(j=0;j<TG_FIELDS;++j)e[5+j]=get(tg_fields[j][0],tg_fields[j][1]);
 mutation(2);++used;return(int32_t)p[P_RESULT];
}
static void text(void)
{
 uint8_t *s=(uint8_t *)TG_TEXT;uint32_t i,n=p[P_LONGCOUNT];CHECK(n<=12000);memset(s,0,12001);
 switch(p[P_TEXT]){
 case 0:break;case 1:s[0]=(uint8_t)p[P_CHARACTER];break;case 2:memcpy(s,"ABC",3);break;
 case 3:memcpy(s,"A B",3);break;case 4:memcpy(s,"A__  B",6);break;case 5:s[0]='#';break;
 case 6:memcpy(s,"A#B",3);break;case 7:memcpy(s,"##",2);break;
 case 8:for(i=1;i<=31;++i)s[i-1]=(uint8_t)i;break;
 case 9:for(i=128;i<=255;++i)s[i-128]=(uint8_t)i;break;
 case 10:memcpy(s,"__  A  __",9);break;case 11:memcpy(s,"#_#",3);break;
 case 12:memcpy(s,"AA",2);break;case 13:for(i=1;i<=255;++i)s[i-1]=(uint8_t)i;break;
 case 14:memset(s,32,n);break;case 15:memset(s,'A',n);break;case 16:memcpy(s,"X#Y  Z",6);break;
 default:CHECK(0);
 }
}
static void substitution(void)
{
 static const uint8_t sample[][8]={{'x','y',0},{0},{'A',' ','B',0},{'#','_','_',0},{199,231,255,0},{32,32,0},{'X','Y','Z',0}};
 static const uint32_t offsets[]={0,256,512,65536};uint32_t j;CHECK(p[P_SUBPROFILE]<7);
 memset((void *)0xe34338,0,1024);memset((void *)0xe44338,0,512);
 for(j=0;j<4;++j)memcpy((void *)(uintptr_t)(0xe34338+offsets[j]),sample[p[P_SUBPROFILE]],8);
 /* Make low-byte alias at index0 distinct from all nonzero prepared offsets. */
 if(p[P_SUBSTART])B(0xe34338)='q';
}
static void prepare(void)
{
 uint32_t i,j;int index;used=0;result=0;
 for(j=0;j<sizeof(tg_regions)/sizeof(tg_regions[0]);++j){uint8_t *d=(uint8_t *)(uintptr_t)tg_regions[j][0];for(i=0;i<tg_regions[j][1];++i)d[i]=(uint8_t)(p[P_SEED]+j*29u+i*37u);}
 memcpy((void *)0x5e047c,original_map,288);
 if(p[P_GLYPHS]==0)memset((void *)0x5e047c,0,288);
 else if(p[P_GLYPHS]==1)for(i=0;i<288;++i)B(0x5e047c+i)=(uint8_t)(i%35+1);
 else CHECK(p[P_GLYPHS]==2);
 for(index=-16;index<2048;++index){uint32_t a=0x10d69f8u+(uint32_t)index*64u;int w,h;
  if(p[P_METRICS]==0){w=7+((uint32_t)index%17);h=4+((uint32_t)index%31);}
  else if(p[P_METRICS]==1){w=(int)((uint32_t)index%63)-31;h=(int)((uint32_t)index%47)-23;}
  else if(p[P_METRICS]==2){w=(index&1)?32767:-32768;h=(index&2)?32767:-32768;}
  else{CHECK(p[P_METRICS]==3);w=h=0;}
  W(a+20)=(uint16_t)w;W(a+22)=(uint16_t)h;
 }
 text();substitution();
 U(0xd3b500)=11;U(0xd3b510)=p[P_DIRECT];U(0xd3b514)=p[P_SHADOW];U(0xd3b518)=p[P_OUTLINE];
 U(0xe20770)=0xdead5678;U(0xe20774)=p[P_BASELINE];U(0xe20778)=p[P_CURSOR];U(0xe2077c)=p[P_SUBSTART];U(0xe20780)=p[P_ENABLE];U(0xe20784)=p[P_POSITION];U(0xe20788)=p[P_READY];
 B(0xe20798)=(uint8_t)p[P_COLLAPSE];B(0xe20799)=(uint8_t)p[P_BLOCK];U(0xe20990)=p[P_SELECTOR];
 U(0xe207ecu+p[P_SELECTOR]*4u)=p[P_TARGET];U(0x126517c)=p[P_COUNT];
 U(0xf56fb4)=871;U(0xf56fbc)=1005;U(0xf56f84)=201;
 if(p[P_MARKERS]==1)U(0xf56fb4)=U(0xf56fbc)=U(0xf56f84)=p[P_FONT];
 else if(p[P_MARKERS]==2)U(0xf56fb4)=U(0xf56fbc)=p[P_FONT];
 else if(p[P_MARKERS]==3){U(0xf56fb4)=1005;U(0xf56fbc)=201;U(0xf56f84)=871;}else CHECK(p[P_MARKERS]==0);
 CHECK(p[P_REPEATS]>0&&p[P_REPEATS]<=3);
}
__declspec(noinline) static uint32_t invoke(void)
{
 switch(p[P_OP]){
 case 0:FUN_004cc770((const uint8_t *)TG_TEXT,(int32_t)p[P_X],(int32_t)p[P_Y],(int32_t)p[P_FONT],(uint16_t)p[P_COLOR]);return 0;
 case 1:return FUN_004ccb00();
 case 2:return(uint32_t)FUN_004cbf30((const uint8_t *)TG_TEXT,(int32_t)p[P_FONT]);
 case 3:return(uint32_t)FUN_004cca60((int32_t)p[P_FONT],(uint8_t)p[P_CHARACTER],(int32_t)p[P_X],(int32_t)p[P_Y]);
 case 4:FUN_004cc720((const uint8_t *)TG_TEXT,(int32_t)p[P_X],(int32_t)p[P_Y],(int32_t)p[P_WIDTH],(int32_t)p[P_FONT],(uint16_t)p[P_COLOR]);return 0;
 default:CHECK(0);return 0;
 }
}
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) static void checked_call(uint32_t out[7])
{__asm{
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
 call invoke
 mov ecx,[esp+20]
 mov [ecx],eax
 mov [ecx+4],ebx
 mov [ecx+8],ebp
 mov [ecx+12],esi
 mov [ecx+16],edi
 mov [ecx+24],esp
 pop edi
 pop esi
 pop ebx
 pop ebp
 ret
}}
#pragma warning(pop)
static void run(void){uint32_t out[7],i;prepare();for(i=0;i<p[P_REPEATS];++i){checked_call(out);CHECK(out[1]==0x11111111&&out[2]==0x22222222&&out[3]==0x33333333&&out[4]==0x44444444&&out[5]==out[6]);result=out[0];}}
static void output(void)
{
 HCRYPTHASH hash;DWORD n=32;uint8_t bytes[32];uint32_t i,j;static const char hex[]="0123456789abcdef";
 CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&hash));for(i=0;i<sizeof(tg_regions)/sizeof(tg_regions[0]);++i){CHECK(CryptHashData(hash,(const BYTE *)tg_regions[i],8,0));CHECK(CryptHashData(hash,(const BYTE *)(uintptr_t)tg_regions[i][0],tg_regions[i][1],0));}CHECK(CryptGetHashParam(hash,HP_HASHVAL,bytes,&n,0));CHECK(CryptDestroyHash(hash));
 printf("{\"result\":%u,\"memory\":\"",result);for(i=0;i<32;++i){putchar(hex[bytes[i]>>4]);putchar(hex[bytes[i]&15]);}printf("\",\"events\":[");
 for(i=0;i<used;++i){printf("%s[",i?",":"");for(j=0;j<TG_EVENT_FIELDS;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);
}
int main(int argc,char **argv)
{
 uint32_t i;SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(VirtualAlloc((void *)0x4ac30000u,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE));CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));memcpy(original_map,(const void *)0x5e047c,288);
 if(argc==2&&!strcmp(argv[1],"--oracle-batch")){printf("{\"fields\":%u}\n",(uint32_t)P_FIELDS);fflush(stdout);for(;;){for(i=0;i<P_FIELDS;++i)if(scanf("%u",p+i)!=1)goto done;run();output();}}
 else{memset(p,0,sizeof(p));p[P_FONT]=335;p[P_COUNT]=2048;p[P_TEXT]=2;p[P_GLYPHS]=1;p[P_REPEATS]=1;run();CHECK(used==3);puts("Text glyph whole candidate ABI smoke.");}
done:CHECK(CryptReleaseContext(provider,0));restore_data_close();return 0;
}
