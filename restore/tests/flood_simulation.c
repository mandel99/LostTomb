#include "flood_simulation.h"
#include "simulation_tick.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BASE 0x30000000u
#define WORLD 0x00d05720u
#define ALT (BASE+0x1000u)
#define POOL (BASE+0x40000u)
#define POOL_SIZE (30u*300u*64u+32u)
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define H(a) (*(int16_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Flood failure: %d\n",__LINE__);ExitProcess(1);}}while(0)
static const uint32_t regions[][2]={{WORLD,0x33008},{ALT,0x33008},{POOL,POOL_SIZE},
 {0x5df75c,12},{0x7848d8,8},{0x78d7bc,24},{0xeaac28,8},{0xbc9694,8},{0xd05718,8},
 {0xd3b1c4,404},{0xd3b364,36},{0xbcd600,0xbd770},{0xea5ce8,8},{0xeaa6c0,8},{0xea5f9c,8},{0xeaa974,8}};
static uint32_t params[20],active,event_count,events[4096][10];static HCRYPTPROV provider;
static void event(uint32_t address,uint32_t n,const uint32_t *values)
{
 uint32_t *q;CHECK(event_count<4096&&n<=4);q=events[event_count++];memset(q,0,40);q[0]=address;q[1]=n;if(n)memcpy(q+2,values,n*4);
 q[6]=U(active+0x32fb4);q[7]=U(active+0x32fac);q[8]=U(active+0x32c54);q[9]=U(0x5df760);
}
#define E(a,...) do{uint32_t v[]={__VA_ARGS__};event(a,sizeof(v)/4,v);}while(0)
void FUN_00489510(void) {event(0x489510,0,NULL);}
void FUN_004be4a0(uint32_t id) {E(0x4be4a0,id);}
int32_t FUN_004c5120(int32_t id) {E(0x4c5120,(uint32_t)id);return !!(params[17]&8);}
void FUN_004bd950(uintptr_t payload,uint8_t flooding)
{
 uint32_t node=U(payload+4),next=U(node);E(0x4bd950,payload,flooding);
 U(payload+20)^=0x13579bdfu;
 if((params[17]&128)&&next)U(U(payload)+24)=U(next+12);
}
static void setup_world(uint32_t world)
{
 uint32_t r;
 memset((void *)world,(int)(params[1]&255),0x33008);
 U(world+0x32c4c)=params[7];U(world+0x32c50)=params[8];U(world+0x32c54)=params[11]+(world==active?0u:17u);
 U(world+0x32c5c)=params[12];U(world+0x32c60)=params[10];U(world+0x32fb8)=params[9];
 U(world+0x32fb4)=params[13];U(world+0x32fac)=params[18];B(world+0x32fbc)=params[17]&2?128:0;
 U(world+0x32c40)=params[7];U(world+0x32c44)=params[8];U(world+0x32c48)=params[10];
 U(world+0x32c58)=params[10];U(world+0x32c5c)=params[12];U(world+0x32c60)=params[10];
 for(r=0;r<30;++r){
  uint32_t region=world+0x32c64+r*28u,head=POOL+r*300u*64u;
  U(region)=params[14];U(region+4)=params[18];U(region+16)=params[14]&&!(params[17]&32)?head:0;
  U(region+24)=params[14]&&(params[17]&16)?head+32:0;
 }
}
static uint32_t run(const uint32_t p[20])
{
 uint32_t i,r,result=0;CHECK(p[0]<=11&&p[9]<=30&&p[10]<=100&&p[14]<=300);memcpy(params,p,sizeof(params));active=p[17]&1?ALT:WORLD;event_count=0;
 for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)memset((void *)regions[i][0],(int)((p[1]+i)&255),regions[i][1]);
 setup_world(WORLD);setup_world(ALT);memset((void *)POOL,0xa5,POOL_SIZE);
 for(r=0;r<30;++r)for(i=0;i<p[14];++i){
  uint32_t a=POOL+(r*300u+i)*64u,region=active+0x32c64+r*28u;
  U(a)=i+1<p[14]?a+64:0;U(a+4)=i?a-64:0;U(a+8)=i;U(a+12)=a+32;
  U(a+32)=region;U(a+36)=a;U(a+40)=i+1000;B(a+56)=p[17]&64?0:(i+p[1])%3?128:0;
 }
 U(0x5df75c)=p[19]^0xabcdef00;U(0x5df760)=p[19];U(0x5df764)=0xdeadbeef;
 H(0x7848d8)=(int16_t)p[3];U(0x78d7bc)=p[2];U(0x78d7c0)=p[4];U(0x78d7c4)=p[5];U(0x78d7cc)=p[6];
 U(0xeaac28)=p[1]&1;U(0xeaac2c)=0xa5a5a500;U(0xbc9694)=0x87654321;U(0xbc9698)=0x12345678;
 U(0xd3b1c4)=p[1]%100;memset((void *)0xd3b1c8u,0x5a,400);U(0xd3b374)=p[1]&127;U(0xd3b380)=p[1]+0x12345678;U(0xd3b384)=p[1]+0x76543210;
 for(i=0;i<2;++i){uint32_t offset=i*0x49d8u;H(0xea5ce8+offset)=p[17]&4?0x100:0;U(0xea5f9c+offset)=p[1]+2048+i;}
 memset((void *)0xbcd600u,0,0xbd770);
 for(i=1;i<2000;++i){static const uint8_t types[]={20,25,77,78,92,93,100,101,0,19,102,255};static const uint8_t states[]={10,12,0,128};
  B(0xbcd600+i*388u+2)=types[(i+p[1])%12];B(0xbcd600+i*388u+0x30)=states[(i/3+p[1])%4];}
 FUN_004bd310();
 switch(p[0]){
 case 0:FUN_004be770(active);break;
 case 1:FUN_004bd850((RestoreWorldRegion *)(active+0x32c64),p[13],(int32_t)p[15],(uint8_t)p[16]);break;
 case 2:FUN_004be0d0((RestoreWorldRegion *)(active+0x32c64));break;
 case 3:FUN_004be490(active);break;
 case 4:FUN_004be580(active);break;
 case 5:result=(uint32_t)restore_flood_quality(active+0x32c40);break;
 case 6:result=(uint32_t)FUN_004bd590();break;
 case 7:FUN_004bd380(active+0x32c40);break;
 case 8:FUN_004bd550(active+0x32c40);break;
 case 9:FUN_004bd580(active+0x32c40);break;
 case 10:break;
 default:for(i=0;i<12;++i){U(0x78d7cc)=p[6]+i;FUN_004be770(active);}break;
 }return result;
}
static void digest(uint32_t a,uint32_t n,char hex[65])
{HCRYPTHASH h;DWORD length=32;uint8_t b[32];uint32_t i;static const char d[]="0123456789abcdef";CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));CHECK(CryptHashData(h,(const BYTE *)a,n,0));CHECK(CryptGetHashParam(h,HP_HASHVAL,b,&length,0));CHECK(CryptDestroyHash(h));for(i=0;i<32;++i){hex[i*2]=d[b[i]>>4];hex[i*2+1]=d[b[i]&15];}hex[64]=0;}
static void output(uint32_t result)
{uint32_t i,j;char hex[65];printf("{\"result\":%u,\"hashes\":[",result);for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i){digest(regions[i][0],regions[i][1],hex);printf("%s\"%s\"",i?",":"",hex);}printf("],\"events\":[");for(i=0;i<event_count;++i){printf("%s[",i?",":"");for(j=0;j<10;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);}
int main(int argc,char **argv)
{
 uint32_t p[20]={0,1,0,0,0,0,1,0,60,10,50,50,80,6,50,0,0,2,0,0xffffffff},i;
 SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(VirtualAlloc((void *)BASE,0x200000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
 if(argc==2&&!strcmp(argv[1],"--oracle-batch")){puts("{\"fields\":20}");fflush(stdout);for(;;){for(i=0;i<20;++i)if(scanf("%u",p+i)!=1)goto done;output(run(p));}}
 else {
  run(p);CHECK(*(double *)0xd05718u==391.68);CHECK(U(active+0x32fb4)==1&&U(active+0x32c54)==55);
  p[0]=1;p[15]=0;run(p);CHECK(U(active+0x32c64+24)==POOL+3*64+32&&event_count==2);
  p[0]=0;p[17]=0;run(p);CHECK(U(active+0x32fb4)==6&&!U(active+0x32c54)&&!event_count);
  puts("Flood simulation: annual initializer, complete cycle controller and explicit region/settings contracts.");
 }
done:CryptReleaseContext(provider,0);VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;
}
