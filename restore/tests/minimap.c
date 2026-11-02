#include "minimap.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
#define BASE 0x30000000u
#define SPRITES (BASE+0x1000u)
#define FB (BASE+0x100000u)
#define ALT (BASE+0x400000u)
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define H(a) (*(int16_t *)(uintptr_t)(a))
#define W(a) (*(uint16_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Minimap failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
/* op,seed,x,y,w,h,format,pixel,overlay,terrain,variation,extra,edge,footprint,
   building,type,marker,tint,actor,type,action,links,tailType,formationValue,
   bounds,cameraX,cameraY,visibleW,visibleH,showAll,metricResult,mutations,
   lookup,image,imageCount,pointerMode,repeats,categoryMode,sidebar,screen,
   reserved,pitch,predicateResult,reserved. */
static uint32_t params[44],events[16384][18],event_count;
static const uint32_t regions[][2]={
 {BASE,0x40000},{FB,0x200000},{0x10d69f8,4097*64+8},{0x12650b8,72},{0x1265178,16},
 {ALT+0x180e48,4097*64+8},{ALT+0x30f508,72},{ALT+0x30f5c8,16},
 {0xd3b500,256},{0xe38d90,40},{0xe92df8,40},{0xe94dc0,80},{0x70e3c0,0x775620-0x70e3c0},
 {0x77567c,16},{0x5ce004,237*4+8},{0x7999d0,4000*264+16},{0xacf850,0xb74830-0xacf850},
 {0xbcb9f0,0xc8ad48-0xbcb9f0},{0x70e11c,12},{0x6fdc4c,12},{0x605a38,12},{0xf56880,0x2a0},{0x784cac,12}};
static HCRYPTPROV provider;
static void event(uint32_t address,uint32_t n,const uint32_t *a)
{
 uint32_t *e;CHECK(event_count<16384);e=events[event_count++];memset(e,0,18*4);e[0]=address;e[1]=n;memcpy(e+2,a,n*4);
 e[7]=U(0x70e488);e[8]=U(0x70e480);e[9]=U(0x70e47c);e[10]=W(0x70e120);
 e[11]=U(0xd3b5e0);e[12]=U(0xd3b5dc);e[13]=U(0xd3b55c);e[14]=U(0xd3b554);e[15]=U(0xd3b558);e[16]=U(0xd3b550);e[17]=U(0xe94e0c);
}
static int32_t metric(uint32_t address,uint32_t n,const uint32_t *a)
{
 event(address,n,a);
 if(params[31]&1)U(0x70e488)=2;
 if(params[31]&4){H(0x7999e8+params[14]*264u)=31;U(0xaf598c+U(0x70e488)*4u)|=0x40;}
 return (int32_t)params[30];
}
#define METRIC(name,address) int32_t name(int32_t x){uint32_t a=(uint32_t)x;return metric(address,1,&a);}
METRIC(FUN_004339e0,0x4339e0) METRIC(FUN_00433a60,0x433a60)
METRIC(FUN_00433c80,0x433c80) METRIC(FUN_00433c90,0x433c90) METRIC(FUN_00433d30,0x433d30) METRIC(FUN_00433d50,0x433d50)
METRIC(FUN_00433e00,0x433e00) METRIC(FUN_00433e40,0x433e40) METRIC(FUN_00433e80,0x433e80) METRIC(FUN_00433ed0,0x433ed0)
METRIC(FUN_00434060,0x434060) METRIC(FUN_004340e0,0x4340e0) METRIC(FUN_00434150,0x434150) METRIC(FUN_004341d0,0x4341d0)
METRIC(FUN_00434250,0x434250) METRIC(FUN_004342c0,0x4342c0) METRIC(FUN_004343a0,0x4343a0) METRIC(FUN_00434400,0x434400)
METRIC(FUN_00434510,0x434510) METRIC(FUN_00434570,0x434570) METRIC(FUN_004345d0,0x4345d0) METRIC(FUN_00434630,0x434630)
METRIC(FUN_00434690,0x434690) METRIC(FUN_004346f0,0x4346f0) METRIC(FUN_00434750,0x434750) METRIC(FUN_004347b0,0x4347b0)
METRIC(FUN_00434a10,0x434a10) METRIC(FUN_00434a60,0x434a60) METRIC(FUN_0047d350,0x47d350)
uint8_t FUN_00434820(int32_t x){uint32_t a=(uint32_t)x;return metric(0x434820,1,&a)!=0;}
int32_t FUN_00433ae0(int32_t x,int32_t y,int32_t z){uint32_t a[3]={(uint32_t)x,(uint32_t)y,(uint32_t)z};return metric(0x433ae0,3,a);}
uint8_t FUN_004387d0(int32_t actor)
{
 uint32_t a=(uint32_t)actor;event(0x4387d0,1,&a);
 if(params[31]&1)U(0x70e488)=2;
 if(params[31]&2)U(0xe94e0c)=51;
 return params[42]==1||(params[42]==2&&actor==1000);
}
int FUN_0053e590(int image,int x,int y)
{uint32_t a[3]={(uint32_t)image,(uint32_t)x,(uint32_t)y};event(0x53e590,3,a);return 1;}
void FUN_004cd680(int32_t x,int32_t y,int32_t w,int32_t h,uint32_t color)
{uint32_t a[5]={(uint32_t)x,(uint32_t)y,(uint32_t)w,(uint32_t)h,color};event(0x4cd680,5,a);if(params[31]&8){U(0xd3b55c)=0xdeadbeef;U(0xd3b554)=0x12345678;}}
static uint32_t pixel(uint32_t i){return params[1]^(i*0x1020301u+0x4a5b6c7du);}
static uint32_t run(const uint32_t p[44])
{
 uint32_t k,j,a,n,ids[3],result=0;CHECK(p[0]<=10&&p[14]<4000&&p[18]<2000&&p[21]<=2&&p[24]<4&&p[32]<4&&p[35]<3&&p[36]>0&&p[36]<=2);
 memcpy(params,p,44*4);event_count=0;
 for(k=0;k<sizeof(regions)/sizeof(regions[0]);++k)memset((void *)regions[k][0],(int)((p[1]+k)&255),regions[k][1]);
 for(k=0;k<4098;++k){uint32_t i=k==4097?25499:k;uint32_t pointer=p[35]==1?0:p[35]==2?0xffffffffu:SPRITES+i*8u;U(SPRITES+i*8u)=pixel(i);U(SPRITES+i*8u+4)=~pixel(i);U(0x10d69f8+i*64u)=pointer;U(ALT+0x180e48+i*64u)=pointer;}
 U(0x126517c)=p[34];U(ALT+0x30f5cc)=p[34];U(0xe92e18)=FB+0x10000;U(0xe38d94)=p[41];U(0xe38db0)=1024;U(0xe38dac)=768;U(0xe92dfc)=p[6];B(0x784cb0)=(uint8_t)p[7];
 for(k=0;k<231u*457u;++k)U(0x70e494+k*4u)=p[32]==1?0x0fff0000u:p[32]==2&&k%11u==0?0x0fff0000u:p[32]==3?50000u+k%1984u:k%51984u;
 for(k=0;k<51984;++k){U(0xaf598c+k*4u)=p[9];H(0xb4e6fc+k*2u)=(int16_t)p[14];H(0xb350dc+k*2u)=(int16_t)p[18];B(0xacf85c+k)=(uint8_t)p[10];B(0xadc36c+k)=(uint8_t)p[11];B(0xb67d1c+k)=(uint8_t)p[12];B(0xb285cc+k)=(uint8_t)p[13];}
 B(0xacf85c+2)=(uint8_t)(p[10]^7);B(0xadc36c+2)=(uint8_t)(p[11]^1);U(0x70e488)=1;
 for(k=0;k<237;++k)U(0x5ce008+k*4u)=p[37]==1?0:p[37]==2?0xffffffffu:p[37]==3?0x7fffffffu:k%15u;
 memset((void *)0x7999d8u,0,4000*264);a=0x7999d8+p[14]*264u;B(a+5)=(uint8_t)p[16];H(a+16)=(int16_t)p[15];H(a+74)=(int16_t)p[17];
 for(k=0;k<2000;++k){H(0xbcd600+k*388u)=0;B(0xbcd602+k*388u)=0;}
 ids[0]=p[18];n=1;for(k=0;k<3&&n<3;++k){static const uint32_t candidates[]={1,257,1000};if(candidates[k]!=p[18])ids[n++]=candidates[k];}
 for(k=0;k<=p[21];++k){a=0xbcd600+ids[k]*388u;H(a)=k<p[21]?(int16_t)ids[k+1]:0;B(a+2)=(uint8_t)(k==p[21]?p[22]:p[19]);B(a+48)=(uint8_t)p[20];H(a+88)=37;}
 H(0xbcba4a+37*144)=(int16_t)p[23];
 {static const uint32_t palette[]={0xf56884,0xf5688c,0xf56894,0xf568a4,0xf568ac,0xf568b4,0xf568bc,0xf568c4,0xf568d4,0xf568dc,0xf56ab4,0xf56b14};for(k=0;k<12;++k)U(palette[k])=1000u+k*31u+(p[1]&255);}
 U(0xe94e0c)=p[8];B(0x775680)=(uint8_t)p[29];U(0xe94dd0)=0;U(0xe94dc8)=0;U(0xe94dcc)=230;U(0xe94dc4)=457;
 if(p[40]==1)U(0xe94dd0)=231;
 if(p[40]==2){U(0xe94dd0)=100;U(0xe94dcc)=110;U(0xe94dc8)=220;U(0xe94dc4)=224;}
 {static const int32_t bounds[][4]={{456,230,0,0},{60,50,10,20},{3,3,0,0},{456,230,449,224}};for(k=0;k<4;++k)U(0x70e420+k*4u)=(uint32_t)bounds[p[24]][k];}
 U(0x70e478)=p[25];U(0x70e474)=p[26];U(0x70e464)=p[27];U(0x70e460)=p[28];U(0x6fdc50)=p[38];U(0x605a3c)=p[39];
 for(j=0;j<p[36];++j)switch(p[0]){
 case 0:FUN_0043e810(p[2],p[3],p[4],p[5]);break;
 case 1:result=FUN_00439980(p[14],p[2],p[3]);break;
 case 2:FUN_0053f4a0(p[33],p[2],p[3]);break;
 case 3:FUN_0043e670(p[4],p[5]);break;
 case 4:result=FUN_0043ee10(p[18]);break;case 5:result=FUN_0043eeb0(p[18]);break;case 6:result=FUN_0043e790(p[18]);break;
 case 7:result=FUN_0043e770((int32_t)p[15]);break;
 case 8:U(0xd3b5e0)=p[2];U(0xd3b5dc)=p[3];FUN_0040fbe0((void *)(SPRITES+8));break;
 case 9:U(0xd3b5e0)=p[2];U(0xd3b5dc)=p[3];FUN_0040fc10(p[33]);break;
 default:restore_minimap_cell((RestoreSpriteLibraries *)ALT,p[33],p[2],p[3]);break;
 }
 return result;
}
static void digest(uint32_t a,uint32_t n,char hex[65])
{HCRYPTHASH h;DWORD length=32;uint8_t b[32];uint32_t i;static const char d[]="0123456789abcdef";CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));CHECK(CryptHashData(h,(const BYTE *)a,n,0));CHECK(CryptGetHashParam(h,HP_HASHVAL,b,&length,0));CHECK(CryptDestroyHash(h));for(i=0;i<32;++i){hex[i*2]=d[b[i]>>4];hex[i*2+1]=d[b[i]&15];}hex[64]=0;}
int main(int argc,char **argv)
{
 uint32_t p[44]={0,1,320,144,3,2,2,1,0,4,3,0,64,0,257,90,0,1,0,0,0,0,0,0xffff,0,110,220,30,20,1,1,0,0,1,25500,0,1,0,0,0,0,4096,0,0},i,j,result;char hex[65];
 SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(VirtualAlloc((void *)BASE,0x800000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
 if(argc==2&&!strcmp(argv[1],"--oracle-batch")){
  puts("{\"fields\":44}");fflush(stdout);
  for(;;){for(i=0;i<44;++i)if(scanf("%u",p+i)!=1)goto done;result=run(p);printf("{\"result\":%u,\"hashes\":[",result);
   for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i){digest(regions[i][0],regions[i][1],hex);printf("%s\"%s\"",i?",":"",hex);}printf("],\"events\":[");
   for(i=0;i<event_count;++i){printf("%s[",i?",":"");for(j=0;j<18;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);
  }
 }else{
  run(p);CHECK(event_count==1&&events[0][0]==0x4cd680&&U(0xd3b558)==1024);
  p[0]=2;run(p);CHECK(U(FB+0x10000+p[2]*2+p[3]*4096)==pixel(1));
  p[0]=4;p[18]=257;p[22]=43;CHECK(run(p)==2);p[22]=82;CHECK(run(p)==3);
  p[0]=1;p[18]=0;p[9]=8;CHECK(run(p)==1&&W(0x70e120)==0);
  puts("Minimap: native scan/geometry/overlay contracts, linked actors and actual two-pixel writes.");
 }
done:CryptReleaseContext(provider,0);VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;
}
