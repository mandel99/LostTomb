#include "animation_state.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BASE 0x30000000u
#define RECORDS (BASE+0x1000u)
#define DIRECT (BASE+0x8000u)
#define FRAMES (BASE+0x10000u)
#define TRANSITIONS (BASE+0x60000u)
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define H(a) (*(int16_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Animation failure: %d\n",__LINE__);ExitProcess(1);}}while(0)
static const uint32_t ranges[][3]={{0x586b08,10,8},{0x58fe48,18,8},{0x596088,13,0},{0x59edf0,27,0},{0x5a8148,56,0},{0x5ad218,27,0},{0x5b0060,15,0},{0x5be510,75,0},{0x5c2750,22,0}};
static const uint32_t regions[][2]={{BASE,0x80000},{0x5be9b8,253*40+8},{0x586b08,168},{0x58fe48,296},{0x596088,216},{0x59edf0,440},{0x5a8148,904},{0x5ad218,440},{0x5b0060,248},{0x5be510,1208},{0x5c2750,360},{0xbcd5f8,776016},{0x7999d8,0x101d08},{0xea5fac,20},{0xeaa984,20},{0xeaac28,8},{0x604950,8}};
static uint32_t params[24],random_states[8192],random_count;static HCRYPTPROV provider;
uint32_t restore_animation_real_rng(void);
uint32_t FUN_00562348(void)
{CHECK(random_count<8192);random_states[random_count++]=U(0x604950);return restore_animation_real_rng();}
#define FUN_00562348 restore_animation_real_rng
#include "../src/unit_02825.c"
#undef FUN_00562348
static void record_setup(uint32_t address,uint32_t kind,uint32_t n)
{
 U(address)=kind;U(address+4)=params[3];U(address+8)=params[4]+n%3u;H(address+16)=(int16_t)params[5];H(address+18)=(int16_t)params[15];B(address+20)=(uint8_t)params[17];B(address+21)=(uint8_t)params[6];
}
static uint32_t run(const uint32_t p[24])
{
 uint32_t k,j,r,c,n=0,result=0;CHECK(p[0]<=12&&p[8]<=16&&p[9]<=8&&p[12]<=4&&p[15]<2000&&p[18]<2&&p[23]>0&&p[23]<=16);memcpy(params,p,sizeof(params));random_count=0;
 for(k=0;k<sizeof(regions)/sizeof(regions[0]);++k)memset((void *)regions[k][0],(int)((p[1]+k)&255),regions[k][1]);
 for(k=0;k<sizeof(ranges)/sizeof(ranges[0]);++k)for(j=0;j<ranges[k][1];++j){
  uint32_t a=RECORDS+n*32u;U(ranges[k][0]+j*16u+ranges[k][2])=a;record_setup(a,n%253u,n);++n;
 }
 for(k=0;k<253;++k){
  uint32_t t=0x5be9b8+k*40u,f=FRAMES+k*1024u,e=TRANSITIONS+k*512u;U(t+16)=f;U(t+20)=p[8];U(t+24)=e;U(t+28)=p[9];
  for(j=0;j<16;++j)U(f+j*48u+12u)=p[7]+(k+j)%3u;
  for(r=0;r<8;++r){
   uint32_t row=e+r*40u;U(row)=0x10000u+r+k*10u;B(row+4)=(uint8_t)(r%3u==0?0:r%3u==1?1:255);
   for(c=0;c<4;++c){uint32_t index=(k+r+c+p[1])%16u;if(p[13]==1)index=16;if(p[13]==2||c>=p[12])index=0xffffffffu;if(p[13]==3&&(c&1u))index=16;
    U(row+8u+c*8u)=index;B(row+12u+c*8u)=(uint8_t)(c%3u==0?0:c%3u==1?1:255);}
  }
  if(p[10]<8){U(e+p[10]*40u)=p[3];B(e+p[10]*40u+4u)=(uint8_t)p[11];}
  if(p[14]<8)U(e+p[14]*40u)=0xffffffffu;
 }
 record_setup(DIRECT,p[2],0);U(DIRECT+8)=p[4];
 B(0xbcd630+p[15]*388u)=(uint8_t)p[16];U(0x604950)=p[1];U(0xeaac28)=p[18];
 memset((void *)0x7999d8u,0,0x101d08);
 for(k=0;k<3;++k){
  static const uint32_t ids[]={1,257,3999};uint32_t a=0x7999d8+ids[k]*264u;
  U(0xea5fac+p[18]*0x49d8u+k*4u)=p[22]&(1u<<k)?ids[k]:0;
  U(0xea5fac+(1u-p[18])*0x49d8u+k*4u)=ids[k]-1;
  B(a)=p[20]&(1u<<k)?1:3;H(a+16)=(int16_t)(p[19]^(k==1?0x1fu:k==2?0x100u:0));B(a+0x55)=p[21]&(1u<<k)?100:128;
 }
 switch(p[0]){
 case 0:FUN_0041fdb0((void *)DIRECT);break;
 case 1:FUN_0041fca0((void *)DIRECT,(uint16_t)p[5]);break;
 case 2:FUN_0041ff20();break;case 3:FUN_0041ff40();break;case 4:FUN_004206d0();break;case 5:FUN_00420a10();break;
 case 6:FUN_00420d40();break;case 7:FUN_00421040();break;case 8:FUN_004216d0();break;case 9:FUN_004e5a80();break;
 case 10:FUN_0041ff60();break;case 11:result=(uint32_t)FUN_004e59c0();break;
 default:for(k=0;k<p[23];++k)FUN_0041fdb0((void *)DIRECT);break;
 }
 return result;
}
static void digest(uint32_t a,uint32_t n,char hex[65])
{HCRYPTHASH h;DWORD length=32;uint8_t b[32];uint32_t i;static const char d[]="0123456789abcdef";CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));CHECK(CryptHashData(h,(const BYTE *)a,n,0));CHECK(CryptGetHashParam(h,HP_HASHVAL,b,&length,0));CHECK(CryptDestroyHash(h));for(i=0;i<32;++i){hex[i*2]=d[b[i]>>4];hex[i*2+1]=d[b[i]&15];}hex[64]=0;}
static void output(uint32_t result)
{uint32_t i;char hex[65];printf("{\"result\":%u,\"hashes\":[",result);for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i){digest(regions[i][0],regions[i][1],hex);printf("%s\"%s\"",i?",":"",hex);}printf("],\"random_states\":[");for(i=0;i<random_count;++i)printf("%s%u",i?",":"",random_states[i]);puts("]}");fflush(stdout);}
int main(int argc,char **argv)
{
 uint32_t p[24]={0,1,0,0,0,0,0,0,16,8,2,0,4,0,7,0,0,0,1,0xcf,7,0,7,16},i;SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(VirtualAlloc((void *)BASE,0x100000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
 if(argc==2&&!strcmp(argv[1],"--oracle-batch")){puts("{\"fields\":24}");fflush(stdout);for(;;){for(i=0;i<24;++i)if(scanf("%u",p+i)!=1)goto done;output(run(p));}}
 else{
  p[5]=1;p[4]=100;run(p);CHECK(U(DIRECT+8)==0&&!random_count);
  p[5]=0;p[4]=100;run(p);CHECK(random_count==1&&U(DIRECT+4)<16);
  p[15]=257;p[16]=8;p[17]=9;run(p);CHECK(H(DIRECT+16)==3&&!H(DIRECT+18)&&!B(DIRECT+20)&&!random_count);
  p[15]=0;p[0]=11;CHECK(run(p)==1);p[21]=1;CHECK(run(p)==257);
  puts("Animation: explicit records, native frame/transition strides, real RNG/setter and all ordered registries.");
 }
done:CryptReleaseContext(provider,0);VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;
}
