#include "city_prepare.h"
#include "mission_loaders.h"
#include "mission_schema.h"
/* Synthetic native-post-load fixture: C-only recovery is checked separately. */
void restore_mission_schema_recover_terrain(void){}
#include "sound_streams.h"
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
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"City preparation failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
static const uint32_t regions[][2]={{0xa43eac,0x1857e4},{0xbc9690,0xc90},{0xd05720,0x33000},{0xd3b180,0x220},
 {0xe38e50,0x90},{0xf55b90,16},{0xe945bc,8},{0xe94e00,32},{0xe95118,16},{0xea184c,0xbd00},
 {0x7848d8,0x800},{0x78d828,8},{0x5ca650,0x2040},{BASE+0x10000,0x33000},{BASE+0x50000,0x10000},{BASE+0x600,0x100},
 {0x70573c,0x390},{0x78d5f0,0x44},{0x799ae0,0x101c00}};
static uint32_t params[12],events[4096][12],event_count,bump,allocation_count;
static struct {uint32_t address,size,live;} allocations[2048];
static HCRYPTPROV provider;
static void event(uint32_t a,uint32_t n,const uint32_t *v)
{
 uint32_t *q;REQUIRE(event_count<4096&&n<=6);q=events[event_count++];memset(q,0,12*4);q[0]=a;q[1]=n;if(n)memcpy(q+2,v,n*4);
 q[8]=U(0xe945c0);q[9]=U(0xeaac28);q[10]=B(0xd3b1b8);q[11]=U(0xf55b94);
}
#define E(a,...) do{uint32_t v[]={__VA_ARGS__};event(a,sizeof(v)/4,v);}while(0)
void *FUN_00561a1d(uint32_t n)
{uint32_t a=bump;REQUIRE(n<=28&&allocation_count<2048&&bump+n+32<BASE+0x800000);E(0x561a1d,n);allocations[allocation_count].address=a;allocations[allocation_count].size=n;allocations[allocation_count++].live=1;memset((void *)a,0xcc,n+32);bump+=(n+32+63)&~63u;return (void *)a;}
void FUN_00561a2b(void *p)
{uint32_t i;E(0x561a2b,(uint32_t)p);for(i=0;i<allocation_count;++i)if(allocations[i].address==(uint32_t)p)allocations[i].live=0;}
void FUN_0051f780(void){}
void FUN_0040f8f0(void *p,int n,int c){uint32_t a=(uint32_t)p;if(n==0xcb10&&(a<restore_data_arena_address||a>=restore_data_arena_address+restore_data_arena_size))a=0x34000000;E(0x40f8f0,a,n,c);memset(p,c,n);}
void FUN_004767d0(int a,int b,int c,int d,int e,int f){E(0x4767d0,a,b,c,d,e,f);}
void FUN_004ce2d0(void){event(0x4ce2d0,0,NULL);}
int FUN_00568eab(const char *path,int flags){E(0x568eab,(uint32_t)path,flags);return params[10]&4?-1:37;}
int FUN_005647eb(int fd){E(0x5647eb,fd);return 0;}
void FUN_0055f561(RestoreSound *self,int channel){E(0x55f561,(uint32_t)self,channel);}
static void terrain(void)
{
 uint32_t x,y,w=params[1],h=params[2],base=U(0xbca2d8);
 memset((void *)0xaf598cu,0,0x32c40);
 if(params[10]&1)for(y=0;y<h;++y)for(x=0;x<w;++x){uint32_t i=base+y*228+x;U(0xaf598c+i*4)=((x+y+params[3])%3?0x10000u:4u);}
}
void FUN_004d41e0(int fd){E(0x4d41e0,fd);U(0xe945c0)=params[6];terrain();}
#define VOID_ENDPOINT(id) void FUN_##id(void){event(0x##id,0,NULL);}
VOID_ENDPOINT(0046b320) VOID_ENDPOINT(0046f030) VOID_ENDPOINT(0046a320)
VOID_ENDPOINT(004f4520) VOID_ENDPOINT(0053d970) VOID_ENDPOINT(004bc720) VOID_ENDPOINT(004771c0)
VOID_ENDPOINT(004e7e20) VOID_ENDPOINT(004677c0) VOID_ENDPOINT(004e59a0)
VOID_ENDPOINT(00485f70) VOID_ENDPOINT(00486e70) VOID_ENDPOINT(00489840) VOID_ENDPOINT(004dea50)
VOID_ENDPOINT(004ddf90) VOID_ENDPOINT(004de080) VOID_ENDPOINT(00517a20) VOID_ENDPOINT(005178b0)
VOID_ENDPOINT(00517370) VOID_ENDPOINT(00517660) VOID_ENDPOINT(005178e0) VOID_ENDPOINT(004b32c0)
VOID_ENDPOINT(00468860) VOID_ENDPOINT(00457af0) VOID_ENDPOINT(0045e040) VOID_ENDPOINT(00404350)
VOID_ENDPOINT(004051a0) VOID_ENDPOINT(004e1830) VOID_ENDPOINT(0041d730) VOID_ENDPOINT(0041df60)
VOID_ENDPOINT(00417e70) VOID_ENDPOINT(00476750) VOID_ENDPOINT(0046d840) VOID_ENDPOINT(004bc7a0)
VOID_ENDPOINT(004bca90) VOID_ENDPOINT(00442660) VOID_ENDPOINT(004426c0) VOID_ENDPOINT(004d5ad0)
VOID_ENDPOINT(004d5bc0) VOID_ENDPOINT(00443330) VOID_ENDPOINT(005596d0) VOID_ENDPOINT(0045aa30)
VOID_ENDPOINT(004f61e0) VOID_ENDPOINT(004a3560)
uint32_t FUN_00540c90(void){event(0x540c90,0,NULL);return 0;}
uint32_t FUN_00540ed0(void){event(0x540ed0,0,NULL);return 0;}
uint32_t FUN_00541160(void){event(0x541160,0,NULL);return 0;}
int FUN_0046f190(void){event(0x46f190,0,NULL);return (int)params[3];}
void FUN_00430530(int a,int b){E(0x430530,a,b);}
static void list(uint32_t region,uint32_t base,uint32_t n)
{
 uint32_t i;U(region)=n;U(region+16)=n?base:0;U(region+20)=n?base+(n-1)*128:0;
 for(i=0;i<n;++i){uint32_t a=base+i*128,p=a+32;U(a)=i+1<n?a+128:0;U(a+4)=i?a-128:0;U(a+8)=10+i*30;U(a+12)=p;
  U(p)=region;U(p+4)=a;U(p+8)=100+i;}
}
/* op,width,height,seed,x,y,version,view,player,rectangle_mode,flags,nodes */
static uint32_t run(const uint32_t p[12])
{
 uint32_t i,result=0,offset,world=(p[10]&2)?BASE+0x10000:0xd05720u;
 REQUIRE(p[0]<=14&&p[1]<=228&&p[2]<=228&&p[8]<2&&p[11]<=6);memcpy(params,p,sizeof(params));event_count=allocation_count=0;bump=BASE+0x400000;
 for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)memset((void *)(uintptr_t)regions[i][0],(uint8_t)(p[3]+i),regions[i][1]);
 offset=((228-p[2])/2)*228+(228-p[1])/2;
 U(0xbca2d0)=U(0x784904)=p[1];U(0xbca2d4)=U(0x784908)=p[2];U(0xbca2d8)=U(0x78490c)=offset;U(0x784910)=p[3]+257;
 U(0xd3b1c4)=p[3]%100;U(0xd3b374)=p[3]&127;U(0xd3b380)=p[3]+0x12345678u;U(0xd3b384)=p[3]+0x76543210u;
 U(0xf56b54)=49000;U(0xe945c0)=p[6];U(0xe94e04)=p[7];U(0xeaac28)=p[8];U(0xe38ebc)=p[10]&8;
 {uint32_t mode=p[9];for(i=0;i<5;++i){H(0x7848e2+i*2)=(int16_t)(mode%5)-1;mode/=5;}}
 for(i=0;i<2;++i){uint32_t j,city=0xea0000+i*0x49d8;U(city+0x1894)=p[4]+i*1000;
  for(j=0;j<21;++j)H(city+0x3e34+j*2)=(int16_t)(p[3]+j*129+i*17);}
 for(i=0;i<161;++i)U(0x70573c+i*4)=p[3]+i*101;
 for(i=0;i<7;++i){uint32_t a=0x799afc+(i==6?3998:i)*264;B(a-28)=i%3==2?2:1;B(a-23)=i%3!=1;
  H(a)=(int16_t)(p[3]+i*129);B(a+0x5a)=(uint8_t)(i&1);B(a+0x5c)=(uint8_t)(i&2);B(a+0x59)=(uint8_t)(i&4);B(a+0x52)=(uint8_t)(i%3);}
 H(0x7848dc)=(int16_t)p[3];for(i=0;i<16;++i){H(0x784bc0+i*2)=(int16_t)(i%5==0?-1:(int32_t)(p[4]+i));H(0x784be0+i*2)=(int16_t)(i%4==0?-1:(int32_t)(p[5]+i));}
 strcpy((char *)0xe38e74u,"Restore");strcpy((char *)(BASE+0x600),"Save/Restore/restore-native.sav");
 for(i=0;i<30;++i){uint32_t g=0xd38384+i*28,a=BASE+0x10000+0x32c64+i*28;U(g)=U(g+16)=U(g+20)=U(g+24)=0;U(a)=U(a+16)=U(a+20)=U(a+24)=0;}
 U(BASE+0x50000)=0;FUN_0046f7b0();terrain();
 if(p[0]==6){list(world+0x32c64,BASE+0x51000,p[11]);list(world+0x32c64+29*28,BASE+0x52000,p[11]);}
 if(p[0]==7||p[0]==8)list(BASE+0x50000,BASE+0x51000,p[11]);
 B(0xacf85c+1000)=(uint8_t)p[3];
 if(p[0]==5)for(i=0;i<51984;++i)B(0xacf85c+i)=(uint8_t)(p[3]+i*29);
 switch(p[0]){case 0:break;case 1:result=FUN_0046f930((int32_t)p[4],(int32_t)p[5]);break;case 2:FUN_0046fa40();break;
 case 3:FUN_0046fc30();break;case 4:FUN_0046fc80();break;case 5:FUN_00475b80();break;
 case 6:FUN_004be0f0((void *)world);break;
 case 7:result=(uint32_t)(uintptr_t)FUN_004bd740((RestoreWorldRegion *)(BASE+0x50000),1000,(int32_t)p[4],(int32_t)p[5],31,(uint8_t)p[10]);break;
 case 8:FUN_004bd6a0((RestoreWorldRegion *)(BASE+0x50000));break;
 case 9:FUN_004be160((void *)world,p[9]?p[1]-1:0,p[9]?p[2]-1:0,p[9]?0:p[1]-1,p[9]?0:p[2]-1);break;
 case 10:FUN_004d52a0();break;case 11:result=FUN_004d5100((const char *)(BASE+0x600));break;
 case 12:FUN_00468170();break;case 13:result=(uint32_t)FUN_004cde60((int32_t)p[4],(int32_t)p[5]);break;
 case 14:{extern uint32_t FUN_004c06e0(uint32_t,int32_t);result=FUN_004c06e0(p[4],(int32_t)p[5]);break;}}
 return result;
}
static void digest(uint32_t a,uint32_t n,char hex[65])
{HCRYPTHASH h;DWORD length=32;uint8_t bytes[32];uint32_t i;static const char d[]="0123456789abcdef";REQUIRE(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));REQUIRE(CryptHashData(h,(const BYTE *)a,n,0));REQUIRE(CryptGetHashParam(h,HP_HASHVAL,bytes,&length,0));REQUIRE(CryptDestroyHash(h));for(i=0;i<32;++i){hex[2*i]=d[bytes[i]>>4];hex[2*i+1]=d[bytes[i]&15];}hex[64]=0;}
static void output(uint32_t result)
{
 uint32_t i,j;char hex[65];printf("{\"result\":%u,\"hashes\":[",result);
 for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i){digest(regions[i][0],regions[i][1],hex);printf("%s\"%s\"",i?",":"",hex);}
 printf("],\"events\":[");for(i=0;i<event_count;++i){printf("%s[",i?",":"");for(j=0;j<12;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}
 printf("],\"allocations\":[");for(i=0;i<allocation_count;++i){digest(allocations[i].address,allocations[i].size+32,hex);printf("%s[%u,%u,%u,\"%s\"]",i?",":"",allocations[i].address,allocations[i].size,allocations[i].live,hex);}puts("]}");fflush(stdout);
}
int main(int argc,char **argv)
{
 uint32_t p[12]={9,12,12,7,0,0,147,6,1,0,1,3},i,result;
 SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX|SEM_NOOPENFILEERRORBOX);_set_abort_behavior(0,_WRITE_ABORT_MSG|_CALL_REPORTFAULT);
 REQUIRE(restore_data_open());REQUIRE(VirtualAlloc((void *)BASE,0x800000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);
 REQUIRE(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
 if(argc==2&&!strcmp(argv[1],"--oracle-batch")){puts("{\"fields\":12}");fflush(stdout);for(;;){for(i=0;i<12;++i)if(scanf("%u",p+i)!=1)goto done;result=run(p);output(result);}}
 else{run(p);REQUIRE(allocation_count>0);p[0]=11;p[10]=0;REQUIRE(run(p)==0&&B(0xd3b1b8)==1);puts("City preparation: geometry, native RNG grid, explicit world/region lifetime and complete ordered entry controllers.");}
done:REQUIRE(CryptReleaseContext(provider,0));VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;
}
