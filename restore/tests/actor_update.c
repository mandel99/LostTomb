#include "actor_update.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define H(a) (*(int16_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Actor update failure: %d\n",__LINE__);ExitProcess(1);}}while(0)
static const uint32_t regions[][2]={{0xbcd5f8,776016},{0xea5c90,0x320},{0xeaa668,0x320},{0xd056f0,16},{0xeaac28,8},{0x5dc28c,448}};
static uint32_t params[10],events[30000][10],event_count;static HCRYPTPROV provider;
static void event(uint32_t label,uint32_t value)
{
 uint32_t id=U(0xd056f8),player=U(0xeaac28)*0x49d8u,*q;CHECK(event_count<30000&&id<2000);q=events[event_count++];
 q[0]=label;q[1]=value;q[2]=id;q[3]=U(0xd056f4);q[4]=B(0xbcd606+id*388u);q[5]=B(0xbcd602+id*388u);
 q[6]=(uint32_t)(int32_t)H(0xbcd67c+id*388u);q[7]=U(0xea5ee8+player);q[8]=U(0xea5db4+player);q[9]=U(0xea5f98+player);
}
static void advance(uint32_t flag,uint32_t divisor)
{uint32_t id=U(0xd056f8);if((params[7]&flag)&&id%divisor==1&&id<1999)U(0xd056f8)=id+1;}
void FUN_00489a50(uint32_t type)
{event(0x489a50,type);advance(1,17);if(params[7]&1)B(0xbcd602+U(0xd056f8)*388u)=(uint8_t)(params[8]%110);}
void restore_actor_callback(uint32_t address)
{event(address,0);advance(2,13);U(0xea5ee8+U(0xeaac28)*0x49d8u)+=params[0]+1;if(params[7]&16)B(0xbcd606+U(0xd056f8)*388u)=2;}
void FUN_00489a80(void) {event(0x489a80,0);advance(4,11);}
void FUN_004b4980(uint32_t id) {event(0x4b4980,id);B(0xbcd606+id*388u)=0;advance(8,7);}
static void run(const uint32_t p[10])
{
 uint32_t i,j;static const uint8_t states[]={0,1,2,128};CHECK(p[1]<2&&p[2]<=1999&&p[4]<4&&p[5]<4&&p[9]>0&&p[9]<=3);memcpy(params,p,sizeof(params));event_count=0;
 for(i=0;i<5;++i)memset((void *)regions[i][0],(int)((p[0]+i)&255),regions[i][1]);
 U(0xeaac28)=p[1];U(0xd056f4)=0x12345678;U(0xd056f8)=0x23456789;
 for(j=0;j<2;++j){uint32_t base=0xea5c90+j*0x49d8u;U(base+0x124)=p[6];}
 for(i=0;i<2000;++i){uint32_t actor=0xbcd600+i*388u;B(actor+2)=(uint8_t)((p[3]+i*7u+p[0])%110u);B(actor+6)=i>0&&i<=p[2]?(p[4]==0?1:p[4]==1?states[(i+p[0])%4]:p[4]==2?2:255):0;H(actor+0x7a)=0;H(actor+0x7c)=0;}
 for(i=1;i<=p[2]&&p[5];++i){uint32_t partner=p[5]==2?1999:i%p[2]+1;H(0xbcd67c+i*388u)=(int16_t)partner;H(0xbcd67a+partner*388u)=p[5]==3?-1:(int16_t)i;}
 for(i=0;i<p[9];++i)FUN_00489b70();
}
static void digest(uint32_t a,uint32_t n,char hex[65])
{HCRYPTHASH h;DWORD length=32;uint8_t b[32];uint32_t i;static const char d[]="0123456789abcdef";CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));CHECK(CryptHashData(h,(const BYTE *)a,n,0));CHECK(CryptGetHashParam(h,HP_HASHVAL,b,&length,0));CHECK(CryptDestroyHash(h));for(i=0;i<32;++i){hex[i*2]=d[b[i]>>4];hex[i*2+1]=d[b[i]&15];}hex[64]=0;}
static void output(void)
{uint32_t i,j;char hex[65];printf("{\"hashes\":[");for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i){digest(regions[i][0],regions[i][1],hex);printf("%s\"%s\"",i?",":"",hex);}printf("],\"events\":[");for(i=0;i<event_count;++i){printf("%s[",i?",":"");for(j=0;j<10;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);}
int main(int argc,char **argv)
{
 uint32_t p[10]={1,1,1999,0,0,0,300,0,69,1},i;SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
 if(argc==2&&!strcmp(argv[1],"--oracle-batch")){puts("{\"fields\":10}");fflush(stdout);for(;;){for(i=0;i<10;++i)if(scanf("%u",p+i)!=1)goto done;run(p);output();}}
 else{run(p);CHECK(U(0xd056f4)==1999&&U(0xd056f8)==2000&&event_count==1999*3&&U(0xea5db4+0x49d8u)==299);p[2]=1;p[4]=2;p[9]=2;run(p);CHECK(!U(0xd056f4)&&event_count==4&&!B(0xbcd606+388));puts("Actor update: full counters, native actor/profile strides, paired links and callback ordering.");}
done:CryptReleaseContext(provider,0);restore_data_close();return 0;
}
