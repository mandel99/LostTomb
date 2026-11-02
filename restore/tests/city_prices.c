#include "town_menu.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BASE 0x30000000u
#define ALT ((RestoreTownList *)(BASE+0x1000))
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define H(a) (*(int16_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define CHECK(x) do{if(!(x)){fprintf(stderr,"City price failure: %d\n",__LINE__);ExitProcess(1);}}while(0)
static const uint32_t regions[][2]={{0x785d70,18628},{BASE+0x1000,18628},{0x775814,236},{0x784dec,284},
 {0xea1894,4},{0xea626c,4},{0xeaac28,4},{0x7999d8,0x101d00},{0x78d790,4},{BASE,4}};
static uint32_t params[10],events[64][4],event_count;static HCRYPTPROV provider;
extern int32_t FUN_00443080(uint32_t);
extern int32_t FUN_00559c60(uint32_t,int32_t);
int32_t FUN_004f57b0(int32_t resource)
{uint32_t *r;CHECK(event_count<64);r=events[event_count++];r[0]=0x4f57b0;r[1]=(uint32_t)resource;r[2]=U(0xeaac28);r[3]=U(0xea1894+r[2]*0x49d8);return (int32_t)(params[2]+(uint32_t)resource*37u);}
static void setup(RestoreTownList *list,uint32_t count,uint32_t seed,uint32_t quantity)
{uint32_t i;static const uint8_t kinds[]={1,2,23,0},enabled[]={0,1,128};memset(list,0xa5,sizeof(*list));list->count=(int32_t)count;
 for(i=0;i<150;++i){uint8_t *r=list->records[i].bytes;*(int16_t *)r=(int16_t)(i+256);r[2]=kinds[(i+seed)%4];r[0x32]=enabled[(i+seed)%3];
  *(int16_t *)(r+0x30)=(int16_t)((i/3+seed)%12)-2;*(int16_t *)(r+4)=(int16_t)((i*7+seed)%40)-2;
  *(int16_t *)(r+12)=(int16_t)((uint32_t)((int32_t)(seed%31)-15)+(i%7)+quantity);}}
static uint32_t run(const uint32_t p[10])
{
 uint32_t i,j;int16_t *previous=(int16_t *)BASE;RestoreTownRecord *selected;
 static const int16_t types[]={0x6e,0x72,0x20,0x2f,0x33,0xb1,0xcd,0x80};
 CHECK(p[0]<=5&&p[1]<=150&&p[3]<2&&((int32_t)p[9]<0||p[9]<=3999));memcpy(params,p,sizeof(params));event_count=0;
 setup(RESTORE_TOWNS,p[1],p[2],p[8]);setup(ALT,p[1],p[2]+1,p[8]);*previous=(int16_t)p[5];
 memset((void *)0x775814u,0xcc,236);memset((void *)0x784decu,0x5a,284);
 for(i=0;i<4;++i)for(j=0;j<6;++j)U(0x7758a0+i*24+j*4)=1000+i*900+j*170+(p[2]%10)*3;
 for(i=0;i<35;++i){U(0x784dec+i*4)=p[6]+i*(p[2]&3);U(0x784e7c+i*4)=p[7]+i*((p[2]>>2)&3);}
 U(0xeaac28)=p[3];U(0xea1894)=p[3]?p[4]^0x13579bdf:p[4];U(0xea626c)=p[3]?p[4]:p[4]^0x13579bdf;
 memset((void *)0x7999d8u,0,0x101d00);U(0x78d790)=p[9];
 for(i=0;i<3999;++i){uint32_t a=0x7999d8+(i+1)*264;B(a)=(i+p[2])%3==0?1:3;H(a+16)=types[(i+p[2])%8];H(a+0x40)=(int16_t)((i+p[2])%2);H(a+18)=(int16_t)((i+p[2])%29)-1;H(a+0xce)=(int16_t)((i*3+p[2])%29)-1;}
 selected=&RESTORE_TOWNS->records[p[2]%150];
 if(p[0]==3){selected->bytes[2]=(uint8_t)p[6];selected->bytes[0x32]=(uint8_t)p[8];*(int16_t *)(selected->bytes+0x30)=(int16_t)p[7];}
 switch(p[0]) {
 case 0:FUN_00443330();return 0;
 case 1:return (uint32_t)(uintptr_t)FUN_0044baf0(p[5]==0xffffffffu?NULL:previous);
 case 2:return (uint32_t)(uintptr_t)restore_town_trade_next(ALT,p[5]==0xffffffffu?NULL:previous);
 case 3:return FUN_0044d4c0(selected);
 case 4:return (uint32_t)FUN_00443080(p[6]);
 default:return (uint32_t)FUN_00559c60(p[6],(int32_t)p[8]);}
}
static void digest(uint32_t a,uint32_t n,char hex[65])
{HCRYPTHASH h;DWORD length=32;uint8_t b[32];uint32_t i;static const char d[]="0123456789abcdef";CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));CHECK(CryptHashData(h,(const BYTE *)a,n,0));CHECK(CryptGetHashParam(h,HP_HASHVAL,b,&length,0));CHECK(CryptDestroyHash(h));for(i=0;i<32;++i){hex[i*2]=d[b[i]>>4];hex[i*2+1]=d[b[i]&15];}hex[64]=0;}
static void output(uint32_t result)
{uint32_t i,j;char hex[65];printf("{\"result\":%u,\"hashes\":[",result);for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i){digest(regions[i][0],regions[i][1],hex);printf("%s\"%s\"",i?",":"",hex);}printf("],\"events\":[");for(i=0;i<event_count;++i){printf("%s[",i?",":"");for(j=0;j<4;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);}
int main(int argc,char **argv)
{uint32_t p[10]={0,6,1,1,2500,0xffffffff,0,0,0,256},i;SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(VirtualAlloc((void *)BASE,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
 if(argc==2&&!strcmp(argv[1],"--oracle-batch")){puts("{\"fields\":10}");fflush(stdout);for(;;){for(i=0;i<10;++i)if(scanf("%u",p+i)!=1)goto done;output(run(p));}}
 else{run(p);CHECK(event_count==5);for(i=0;i<35;++i)CHECK((int32_t)U(0x775814+i*4)>=0);puts("City prices: explicit town receivers, native accumulation/population bands and real building count/clamp.");}
done:CryptReleaseContext(provider,0);VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;}
