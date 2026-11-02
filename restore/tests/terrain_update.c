#include "city_prepare.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Terrain check failed: %d\n",__LINE__);ExitProcess(1);}}while(0)
static const uint32_t regions[][2]={{0xa43eac,0x1857e4},{0xbc9690,0xc90},{0x605a3c,4},{0x78d7ac,8}};
static uint32_t events[8192][8],event_count;
static HCRYPTPROV provider;
void FUN_00479770(int,int,int);
void FUN_00475bf0(int,int,int,int);
void FUN_004dea50(void);
uint32_t FUN_0047a1c0(int32_t);
uint32_t FUN_0047a1f0(int32_t);
static void event(uint32_t address,uint32_t n,const uint32_t *args)
{uint32_t *row;CHECK(event_count<8192&&n<=6);row=events[event_count++];memset(row,0,32);row[0]=address;row[1]=n;if(n)memcpy(row+2,args,n*4);}
#define E(a,...) do{uint32_t v[]={__VA_ARGS__};event(a,sizeof(v)/4,v);}while(0)
#define E0(a) void FUN_##a(void){event(0x##a,0,NULL);}
#define E4(a) void FUN_##a(int x,int y,int z,int w){E(0x##a,x,y,z,w);}
#define E5(a) void FUN_##a(int x,int y,int z,int w,int k){E(0x##a,x,y,z,w,k);}
E0(00475dc0) E0(00476530) E0(00477210) E0(0047b570) E0(0047b1a0)
E4(0051f780) E4(004770f0) E4(004777c0) E4(00477500) E4(004776f0) E4(00477890)
E5(00475720) E5(00475cc0) E5(0047bae0)
void FUN_00486600(int i){E(0x486600,i);}
void FUN_0047d870(int i){E(0x47d870,i);}
void FUN_004767d0(int a,int b,int c,int d,int e,int f){E(0x4767d0,a,b,c,d,e,f);}
/* op,width,height,seed,x,y,x1,y1,enable */
static uint32_t run(const uint32_t p[9])
{
 static const uint32_t terrain[]={0,1,0x201,0x401,0x601,0x10001,2,3};
 uint32_t i;CHECK(p[0]<=4&&p[1]<=228&&p[2]<=228);event_count=0;
 for(i=0;i<4;++i)memset((void *)regions[i][0],(uint8_t)(p[3]+i),regions[i][1]);
 U(0x78d7ac)=p[3];U(0x78d7b0)=p[8];
 U(0xbca2d0)=p[1];U(0xbca2d4)=p[2];U(0xbca2d8)=((228-p[2])/2)*228+(228-p[1])/2;U(0xbca2dc)=228-p[1];U(0x605a3c)=p[3]&0x100;
 FUN_0046f7b0();
 for(i=0;i<51984;++i)U(0xaf598c+i*4)=terrain[(i+p[3])&7];
 if(p[0]==0)FUN_00479770((int32_t)p[4],(int32_t)p[5],(int32_t)p[8]);
 else if(p[0]==1)FUN_00475bf0((int32_t)p[4],(int32_t)p[5],(int32_t)p[6],(int32_t)p[7]);
 else if(p[0]==2)FUN_004dea50();
 else if(p[0]==3)return FUN_0047a1c0((int32_t)p[4]);
 else return FUN_0047a1f0((int32_t)p[4]);
 return 0;
}
static void digest(uint32_t a,uint32_t n,char hex[65])
{HCRYPTHASH h;DWORD length=32;uint8_t b[32];uint32_t i;static const char d[]="0123456789abcdef";CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));CHECK(CryptHashData(h,(const BYTE *)a,n,0));CHECK(CryptGetHashParam(h,HP_HASHVAL,b,&length,0));CHECK(CryptDestroyHash(h));for(i=0;i<32;++i){hex[i*2]=d[b[i]>>4];hex[i*2+1]=d[b[i]&15];}hex[64]=0;}
static void output(uint32_t result)
{uint32_t i,j;char hex[65];printf("{\"result\":%u,\"hashes\":[",result);for(i=0;i<4;++i){digest(regions[i][0],regions[i][1],hex);printf("%s\"%s\"",i?",":"",hex);}printf("],\"events\":[");for(i=0;i<event_count;++i){printf("%s[",i?",":"");for(j=0;j<8;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);}
int main(int argc,char **argv)
{uint32_t p[9]={0,12,12,0,6,6,11,11,1},i;SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
 if(argc==2&&!strcmp(argv[1],"--oracle-batch")){puts("{\"fields\":9}");fflush(stdout);for(;;){for(i=0;i<9;++i)if(scanf("%u",p+i)!=1)goto done;output(run(p));}}
 else{run(p);CHECK(event_count>0);CHECK(U(0xbc9768)>255);p[0]=2;run(p);puts("Terrain update: full-width counters, byte grids and ordered refresh controller.");}
done:CryptReleaseContext(provider,0);restore_data_close();return 0;}
