#include "city_prepare.h"
#include "terrain_redraw.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BASE 0x30000000u
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define H(a) (*(int16_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Terrain redraw failure: %d\n",__LINE__);ExitProcess(1);}}while(0)
static const uint32_t regions[][2]={{0xa43eac,0x1857e4},{0xbc9690,0xc90},{0xd05720,0x33000},{0x7999d8,0x101d00},{0xf55bb4,0x1200},{BASE+0x1000,0x100}};
static uint32_t params[16],events[8192][12],event_count;
static HCRYPTPROV provider;
static void event(uint32_t a,uint32_t n,const uint32_t *v)
{uint32_t *r;CHECK(event_count<8192&&n<=7);r=events[event_count++];memset(r,0,48);r[0]=a;r[1]=n;if(n)memcpy(r+2,v,n*4);r[10]=U(0xbc9768);r[11]=U(0xbc9748);}
#define E(a,...) do{uint32_t event_values_[]={__VA_ARGS__};event(a,sizeof(event_values_)/4,event_values_);}while(0)
int32_t FUN_004f3d70(int32_t i){E(0x4f3d70,i);return (int32_t)params[12];}
uint32_t FUN_004f3f70(int32_t a,int32_t b,int32_t c,int32_t d){E(0x4f3f70,a,b,c,d);return 0xabcdef00u|((params[15]>>8)&1);}
void FUN_004f60d0(int32_t id){E(0x4f60d0,id);U(0x799a2cu+(uint32_t)id*264u)=0;}
void FUN_004f6130(void){event(0x4f6130,0,NULL);}
uint32_t FUN_0051c5f0(int32_t i,uint32_t v){E(0x51c5f0,i,v);return (v+params[9])&127u;}
int32_t FUN_00484be0(void *p,int32_t a,int32_t b,int32_t c){E(0x484be0,(uint32_t)p,a,b,c);U(0xbc9748)=params[9]&15;return 0;}
extern int32_t FUN_00484f40(void);
/* op,w,h,x0,y0,x1,y1,preserve,exact,seed,terrain,type,state,fertility,orientation,options */
static uint32_t run(const uint32_t p[16])
{
 static const uint32_t mix[]={0,0x80,0x10000000,0x10000080,1,0x40,0x10000,0x10080,8,0x200,0x80|0x40};
 static const int offsets[]={-228,-227,1,229,228,227,-1,-229};
 uint32_t i,j,origin,center,id,parent,options=p[15],result=0;
 CHECK(p[0]<=4&&p[1]<=228&&p[2]<=228);memcpy(params,p,sizeof(params));event_count=0;
 for(i=0;i<6;++i)memset((void *)regions[i][0],(uint8_t)(p[9]+i),regions[i][1]);
 memset((void *)0xd05720u,0,0x33000);memset((void *)0x7999d8u,0,0x101d00);
 memset((void *)0xb7482cu,0,51984*4);memset((void *)0xb350dcu,0,51984*2);memset((void *)0xb4e6fcu,0,51984*2);
 origin=((228-p[2])/2)*228+(228-p[1])/2;center=origin+(p[2]/2)*228+p[1]/2;
 U(0xbca2d0)=p[1];U(0xbca2d4)=p[2];U(0xbca2d8)=origin;U(0xbca2dc)=228-p[1];U(0xbca2e0)=p[14];
 U(0xbc9768)=0x11223344;U(0xbc9748)=p[9]+0x12340000;FUN_0046f7b0();
 for(i=0;i<0x240;++i)U(0xf55bb4+i*8)=10000+i*100;
 U(0xf56b74)=17000+p[9];U(0xf56b8c)=18000+p[9];
 id=10+(p[9]&3)*500;parent=id-((options&4)?3:0);
 for(i=0;i<25;++i)U(0x799a30+(parent+i)*264)=p[12];
 H(0x7999e8+id*264)=(int16_t)p[11];H(0x7999ea+id*264)=(options&4)?3:0;U(0x799a2c+parent*264)=p[12];
 B(BASE+0x1018)=(options&2)?1:0;
 for(i=0;i<51984;++i){
  uint32_t flags=(options&64)?mix[(i+p[9])%11]:p[10];
  if(options&16)flags=i==center?p[10]:0;
  U(0xaf598c+i*4)=flags;B(0xa509bc+i)=(uint8_t)p[13];
  B(0xacf85c+i)=(uint8_t)((options&0x1000)?(0xf0|(p[9]&7)):(i+p[9]*13));
  if(flags&0x10000000)H(0xb4e6fc+i*2)=(int16_t)id;
  if(options&1)U(0xd05720+i*4)=BASE+0x1000;
 }
 if(options&8){uint32_t previous=center-20;H(0xb4e6fc+previous*2)=15;H(0x7999e8+15*264)=0xb7;U(0xaf598c+previous*4)=0x10000000;}
 if(options&32)for(i=0;i<8;i+=2)U(0xaf598c+(center+offsets[i])*4)|=0x10000;
 if(options&0x2000)U(0xaf598c+(center+229)*4)|=1;
 if(options&0x4000){for(i=0;i<8;++i)B(0xa509bc+center+offsets[i])=88;B(0xa509bc+center-229)=0;}
 j=(options>>16)&7;if(j)H(0xb350dc+(center+j*228+j)*2)=1;
 if(p[0]==0)FUN_004767d0((int32_t)p[3],(int32_t)p[4],(int32_t)p[5],(int32_t)p[6],p[7],p[8]);
 else if(p[0]==1)FUN_0047c140((int32_t)p[12],(int32_t)p[3],(int32_t)p[4],(int32_t)p[5],(int32_t)p[6],p[9],p[10]);
 else if(p[0]==2)result=(uint32_t)FUN_0047c880((int32_t)p[3],(int32_t)p[4],(int32_t)p[5],(int32_t)p[6],(int32_t)p[11],(int32_t)p[7],(int32_t)p[8]);
 else if(p[0]==3)result=FUN_0046f9a0((int32_t)p[3],(int32_t)p[4],(int32_t)p[5]);
 else {U(0xbc9768)=center;FUN_0047db00(p[10]);result=(uint32_t)FUN_00484f40();}
 return result;
}
static void digest(uint32_t a,uint32_t n,char hex[65])
{HCRYPTHASH h;DWORD length=32;uint8_t b[32];uint32_t i;static const char d[]="0123456789abcdef";CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));CHECK(CryptHashData(h,(const BYTE *)a,n,0));CHECK(CryptGetHashParam(h,HP_HASHVAL,b,&length,0));CHECK(CryptDestroyHash(h));for(i=0;i<32;++i){hex[i*2]=d[b[i]>>4];hex[i*2+1]=d[b[i]&15];}hex[64]=0;}
static void output(uint32_t result)
{uint32_t i,j;char hex[65];printf("{\"result\":%u,\"hashes\":[",result);for(i=0;i<6;++i){digest(regions[i][0],regions[i][1],hex);printf("%s\"%s\"",i?",":"",hex);}printf("],\"events\":[");for(i=0;i<event_count;++i){printf("%s[",i?",":"");for(j=0;j<12;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);}
int main(int argc,char **argv)
{uint32_t p[16]={0,40,40,20,20,20,20,0,1,1,0,0,0,0,0,0x1000},i;SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(VirtualAlloc((void *)BASE,0x100000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
 if(argc==2&&!strcmp(argv[1],"--oracle-batch")){puts("{\"fields\":16}");fflush(stdout);for(;;){for(i=0;i<16;++i)if(scanf("%u",p+i)!=1)goto done;output(run(p));}}
 else{run(p);CHECK(event_count==1);CHECK(U(0xb7482c+(U(0xbca2d8)+20*228+20)*4)!=0);puts("Terrain redraw: all passes, native footprint placement/predicate and neighborhood data.");}
done:CryptReleaseContext(provider,0);VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;}
