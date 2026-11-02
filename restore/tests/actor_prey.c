#include "actor_update.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define I(a) (*(int32_t *)(uintptr_t)(a))
#define H(a) (*(int16_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Prey failure: %d\n",__LINE__);ExitProcess(1);}}while(0)
static const uint32_t regions[][2]={{0xbcd5f8,776016},{0x7999d8,0x101d08},{0xbcb9c8,8*144+8},{0xea5ce8,8},{0xeaa6c0,8},{0xeaac28,8},{0xd056f4,12},{0xd3b1c4,404},{0xd3b364,36},{0xbca2e0,8},{0xe94e00,12},{0xf561c0,64},{0x7090e8,110*56+8},{0x5cdb78,136}};
static uint32_t params[24],events[128][16],event_count,other;static HCRYPTPROV provider;
static uint32_t actor(uint32_t id){return 0xbcd600u+id*388u;}
static void endpoint(uint32_t label,uint32_t n,const uint32_t *args)
{
 uint32_t id=U(0xd056f8),a=actor(id),*q;CHECK(event_count<128);q=events[event_count++];memset(q,0,64);q[0]=label;q[1]=n;if(n)memcpy(q+2,args,n*4);
 q[8]=id;q[9]=B(a+6);q[10]=B(a+0x30);q[11]=B(a+9);q[12]=(uint32_t)(int32_t)H(a+0x2e);q[13]=U(a+0x140);q[14]=U(0xd3b1c4);q[15]=(uint16_t)H(0xea5cea+U(0xeaac28)*0x49d8u);
 if(params[15]&1)U(0xd056f8)=id==params[2]?other:params[2];
 a=actor(U(0xd056f8));
 if(params[15]&2)B(a+0x30)=(uint8_t)params[16];
 if(params[15]&4){U(a+0x140)=params[1]+7u;U(a+0x13c)=params[1]%17u+1u;H(a+0x2e)=(int16_t)(params[1]*129u);}
 if(params[15]&16)U(0xbca2e0)=params[1]^0x87654321u;
 if(label==0x4a5da0&&params[14]!=0xffffffffu)B(a+9)=(uint8_t)params[14];
 if(label==0x4eab10&&(params[15]&8)){uint32_t f=0xbcb9c8+params[10]*144u;H(f+58)=(int16_t)((uint16_t)H(f+58)+1u);H(f+60)=(int16_t)((uint16_t)H(f+60)-2u);}
}
#define E(a,...) do{uint32_t v[]={__VA_ARGS__};endpoint(a,sizeof(v)/4,v);}while(0)
void FUN_004a5da0(int32_t id,uint32_t speed){E(0x4a5da0,(uint32_t)id,speed);}
void FUN_004b17d0(int32_t id){E(0x4b17d0,(uint32_t)id);}
void FUN_004b1d50(int32_t id){E(0x4b1d50,(uint32_t)id);}
void FUN_004b3250(int32_t id){E(0x4b3250,(uint32_t)id);}
void FUN_004b5050(int32_t id){E(0x4b5050,(uint32_t)id);}
uint32_t FUN_004eb000(int32_t id){E(0x4eb000,(uint32_t)id);return 0;}
int32_t FUN_004eab10(int32_t a,int32_t b,int32_t c,int32_t d,int32_t e,int32_t f){E(0x4eab10,(uint32_t)a,(uint32_t)b,(uint32_t)c,(uint32_t)d,(uint32_t)e,(uint32_t)f);return (int32_t)params[13];}
static void run(const uint32_t p[24])
{
 uint32_t i,j;CHECK(p[0]<=2&&p[2]>0&&p[2]<2000&&p[10]<8&&p[11]<4000&&p[17]<2&&p[18]<100&&p[19]<110);memcpy(params,p,sizeof(params));event_count=0;other=p[2]==1999?1:p[2]+1;
 for(i=0;i<13;++i)memset((void *)regions[i][0],(int)((p[1]+i)&255),regions[i][1]);
 U(0xd056f8)=p[2];U(0xeaac28)=p[17];U(0xbca2e0)=p[9];U(0xe94e00)=0xeeeeeeee;
 for(i=0;i<2;++i){uint32_t a=actor(i?other:p[2]);B(a+2)=(uint8_t)p[19];B(a+6)=1;B(a+0x30)=(uint8_t)p[3];H(a+0x2e)=(int16_t)p[4];U(a+0x140)=p[5];U(a+0x13c)=p[6];B(a+9)=(uint8_t)p[7];B(a+10)=(uint8_t)p[8];H(a+0x58)=(int16_t)p[10];U(a+0x130)=p[11];B(a-7)=(uint8_t)p[20];U(a-4)=p[21]^a;B(a+0x5a)=(uint8_t)(p[1]+i);H(a+12)=i?-120:(int16_t)(10+p[1]%200);H(a+14)=i?255:(int16_t)(20-p[1]%200);}
 memset((void *)0x7999d8u,0,0x101d08);{uint32_t b=0x7999d8+p[11]*264u;H(b+16)=p[12]&1?0x73:0;B(b+0xa6)=p[12]&2?128:0;B(b)=p[12]&4?1:3;H(b+0x40)=p[12]&8?256:0;}
 for(i=0;i<8;++i){H(0xbcb9c8+i*144+58)=(int16_t)(p[1]+i*37u);H(0xbcb9c8+i*144+60)=(int16_t)(0x7fff-p[1]-i*19u);}
 U(0xd3b1c4)=p[18];memset((void *)0xd3b1c8u,0x5a,400);U(0xd3b380)=p[1]+0x12345678;U(0xd3b384)=p[1]+0x76543210;U(0xd3b374)=p[1]&127;
 for(i=0;i<110;++i)U(0x7090ec+i*56u)=p[23]+i*257u;
 for(i=0;i<2;++i)H(0xea5cea+i*0x49d8u)=(int16_t)p[1];
 for(i=0;i<5;++i){static const uint32_t addresses[]={0xf561cc,0xf561d4,0xf561dc,0xf561e4,0xf561ec};U(addresses[i])=p[21]+i*10000u;}
 for(j=0;j<(p[0]==2?6u:1u);++j){if(p[0]==1)restore_actor_callback(0x49bd40);else FUN_0049bd40();}
}
static void digest(uint32_t a,uint32_t n,char hex[65])
{HCRYPTHASH h;DWORD length=32;uint8_t b[32];uint32_t i;static const char d[]="0123456789abcdef";CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));CHECK(CryptHashData(h,(const BYTE *)a,n,0));CHECK(CryptGetHashParam(h,HP_HASHVAL,b,&length,0));CHECK(CryptDestroyHash(h));for(i=0;i<32;++i){hex[i*2]=d[b[i]>>4];hex[i*2+1]=d[b[i]&15];}hex[64]=0;}
static void output(void)
{uint32_t i,j;char hex[65];printf("{\"hashes\":[");for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i){digest(regions[i][0],regions[i][1],hex);printf("%s\"%s\"",i?",":"",hex);}printf("],\"events\":[");for(i=0;i<event_count;++i){printf("%s[",i?",":"");for(j=0;j<16;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);}
int main(int argc,char **argv)
{
 uint32_t p[24]={0,1,1,24,1,0,8,0,3,0,1,1,15,0,0xffffffff,0,14,0,99,69,255,0x12340000,0,2},i;SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
 if(argc==2&&!strcmp(argv[1],"--oracle-batch")){puts("{\"fields\":24}");fflush(stdout);for(;;){for(i=0;i<24;++i)if(scanf("%u",p+i)!=1)goto done;run(p);output();}}
 else{CHECK(restore_actor_callback_resolve(0x49bd40)==FUN_0049bd40&&restore_actor_callback_resolve(0x51f780)!=NULL&&restore_actor_callback_resolve(0x489ca0)&&!restore_actor_callback_resolve(0x48a720));run(p);CHECK(B(actor(1)+0x30)==8&&!U(actor(1)-4));p[0]=1;p[3]=8;run(p);CHECK(B(actor(1)+0x30)>=18&&B(actor(1)+0x30)<=19&&H(actor(1)+0x2e)>=150&&H(actor(1)+0x2e)<300&&U(0xd3b1c4)==1);puts("Prey: complete state dispatcher, native random/history, sprite selection and explicit C registry.");}
done:CryptReleaseContext(provider,0);restore_data_close();return 0;
}
