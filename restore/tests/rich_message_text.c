#include "rich_message_text.h"
#include "standalone_data.h"
#include <stdlib.h>
#include <fcntl.h>
#include <io.h>
#define BASE 0x30000000u
#define SRC 0x31000000u
#define DST 0x31100000u
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"Rich text failure%d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t p[24],events[8192][10],count,result,frame_bytes;
static const uint32_t regions[][2]={{0xd3a6e0,4096},{0xe20758,80},{0xe34394,216},{0x1265174,16},{0xf55bfc,12},{0xf56f7c,12}};
static const char *texts[]={"","ABC DEF"," ABC  DEF ","One @Ptwo @Lthree","@G3 ABC","ABC @G3 DEF","@17linked word then rest","@123ABC @2DEF","@0 X","A@17B C","ABC\nDEF","@L@P@L","@G0@G12Z end","@-7ABC DEF","@999999999ABC","@17A @P@3B","\xc7\xe7\xff\x80 ab","ABCDEFGHIJKLMNOPQRSTUVWXYZ","A","@17 ","2147483647","-2147483648","-123456789","000003","0"};
static void event(int image,int x,int y){uint32_t *r;REQUIRE(count<8192);r=events[count++];r[0]=image;r[1]=x;r[2]=y;r[3]=RT_W(0xd3b51c);r[4]=RT_U(0xd3b5c0);r[5]=RT_U(0xe20778);r[6]=RT_U(0xe20760);r[7]=RT_U(0xd3a6ec);r[8]=RT_U(0xd3a6f0);r[9]=RT_U(0xd3b138);}
extern int restore_rich_message_sprite(int,int,int);
int FUN_0053e590(int image,int x,int y){event(image,x,y);if(p[21])return restore_rich_message_sprite(image,x,y);if(p[19]&1)RT_U(0xd3b5c0)=p[20];if(p[19]&2)RT_U(0xe20778)+=13;if(p[19]&4)RT_W(0xd3b51c)=0x4567;return 0x12345678;}
#define UNREACHED(name) int name(void){REQUIRE(0);return 0;}
UNREACHED(FUN_004ce310) UNREACHED(FUN_004ce480) UNREACHED(FUN_0053f7d0) UNREACHED(FUN_0053f790)
UNREACHED(FUN_004cdf20) UNREACHED(FUN_004cdeb0) UNREACHED(FUN_0040fc40) UNREACHED(FUN_0040fcf0)
UNREACHED(FUN_0040fdd0) UNREACHED(FUN_0040fed0) UNREACHED(FUN_0040ff50) UNREACHED(FUN_004105c0)
UNREACHED(FUN_00410660) UNREACHED(FUN_00410de0) UNREACHED(FUN_00410e80) UNREACHED(FUN_00410f70)
UNREACHED(FUN_00410fb0) UNREACHED(FUN_004114a0) UNREACHED(FUN_00411fd0) UNREACHED(FUN_004100f0)
UNREACHED(FUN_004101c0) UNREACHED(FUN_00410700) UNREACHED(FUN_004109d0) UNREACHED(FUN_00410a70)
UNREACHED(FUN_00427a20) UNREACHED(FUN_0042a160) UNREACHED(FUN_0042b440) UNREACHED(FUN_0051f780)
UNREACHED(FUN_0042c740) UNREACHED(FUN_0042c9a0) UNREACHED(FUN_0042cee0) UNREACHED(FUN_0042d810) UNREACHED(FUN_0042e680)
static void fixture(void){uint32_t i;RestoreSpriteLibraries *s=RESTORE_SPRITE_LIBRARIES;memset(s,0,sizeof(*s));s->next_image=25500;for(i=0;i<25500;++i){uint8_t *r=s->images[i].bytes;*(uint32_t *)r=SRC;*(uint32_t *)(r+4)=128;*(uint32_t *)(r+16)=1;*(int16_t *)(r+20)=p[7];*(int16_t *)(r+22)=p[8];r[0x32]=(p[21]&2)?0:1;}for(i=0;i<128;++i)((uint16_t *)SRC)[i]=(i%7==0)?0xf81f:(uint16_t)(i*37+123);}
static void run(void){uint32_t i;int32_t x=p[9],y=p[10];char *s=(char *)(BASE+0x1000);count=0;fixture();for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)memset((void *)regions[i][0],0xa5,regions[i][1]);memset((void *)BASE,0x5a,0x10000);if(p[1]<25)strcpy(s,texts[p[1]]);else if(p[1]==25){memset(s,'A',2001);s[2001]=0;}else if(p[1]==26){for(i=0;i<250;++i)memcpy(s+i*6,"A @L  ",6);s[1500]=0;}else if(p[1]==27)strcpy(s,"@17");else if(p[1]==28)strcpy(s,"ABC @LDEF @G3 GH");else REQUIRE(0);
 RT_I(0xd3b168)=p[2];RT_I(0xd3b164)=p[3];RT_I(0xf56f84)=p[4];RT_I(0xf55c04)=100;RT_U(0x126517c)=p[5];RT_I(0xd3b130)=p[14];RT_I(0xd3b13c)=p[15];RT_I(0xd3b14c)=p[16];RT_U(0xd3a6ec)=p[17];RT_U(0xd3a6f0)=p[18];RT_W(0xd3b51c)=0x1234;RT_U(0xe20774)=0;RT_U(0xe92dfc)=p[22];RT_B(0xe38dda)=5;
 RT_U(0xe38d9c)=640;RT_U(0xe38d98)=480;RT_U(0xe38d94)=1280+p[23];RT_U(0xe38db0)=640;RT_U(0xe38dac)=480;RT_U(0xe92e18)=DST+256;RT_U(0xe92e1c)=BASE+0x8000;RT_U(0xd3b55c)=0;RT_U(0xd3b554)=0;RT_U(0xd3b558)=640;RT_U(0xd3b550)=480;RT_W(0x70e120)=0;
 frame_bytes=RT_U(0xe38d94)*480+320;memset((void *)DST,0xa5,0x100000);result=0;
 switch(p[0]){case 0:FUN_004c7fd0(p[20],x,(int32_t)(p[9]+p[11]),y);break;case 1:FUN_004c8040();break;case 2:FUN_004c8070(s,x,y,p[11],p[12],p[13]);break;case 3:result=FUN_004c8480(s);break;case 4:FUN_004c8590(s,x,y,p[12],p[13]);break;case 5:result=FUN_004c8680(p[2],p[6],x,y,p[12],p[13]);break;case 6:result=FUN_004cc070(s);break;default:REQUIRE(0);}
 for(i=0;i<8;++i){REQUIRE(RT_B(0xe34394u+i)==0xa5);REQUIRE(RT_B(0xe34464u+i)==0xa5);}REQUIRE(RT_U(0xd3a6f4)==0xa5a5a5a5&&RT_U(0xd3aae0)==0xa5a5a5a5);
 if(p[21]){for(i=0;i<256;++i)REQUIRE(RT_B(DST+i)==0xa5);for(i=frame_bytes-64;i<frame_bytes;++i)REQUIRE(RT_B(DST+i)==0xa5);for(i=0;i<128;++i)REQUIRE(((uint16_t *)SRC)[i]==(i%7==0?0xf81f:(uint16_t)(i*37+123)));}
}
static void printbytes(uint32_t a,uint32_t n){static const char hex[]="0123456789abcdef";char b[8192];uint32_t i,k;while(n){k=n>4096?4096:n;for(i=0;i<k;++i){uint8_t v=RT_B(a+i);b[2*i]=hex[v>>4];b[2*i+1]=hex[v&15];}REQUIRE(fwrite(b,2,k,stdout)==k);a+=k;n-=k;}}
static void output(void){uint32_t i,j;printf("{\"result\":%u,\"state\":\"",result);for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)printbytes(regions[i][0],regions[i][1]);printbytes(BASE+0xff8,2020);printf("\",\"events\":[");for(i=0;i<count;++i){printf("%s[",i?",":"");for(j=0;j<10;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}printf("],\"pixels_bytes\":%u}\n",p[21]?frame_bytes:0);if(p[21])REQUIRE(fwrite((void *)DST,1,frame_bytes,stdout)==frame_bytes);fflush(stdout);}
int main(int argc,char **argv){uint32_t i;REQUIRE(restore_data_open());REQUIRE(VirtualAlloc((void *)BASE,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);REQUIRE(VirtualAlloc((void *)SRC,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)SRC);REQUIRE(VirtualAlloc((void *)DST,0x100000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)DST);if(argc==2&&!strcmp(argv[1],"--oracle-batch")){REQUIRE(_setmode(_fileno(stdout),_O_BINARY)!=-1);puts("{\"fields\":24}");fflush(stdout);for(;;){for(i=0;i<24;++i)if(scanf("%u",p+i)!=1)goto done;run();output();}}else{uint32_t initial[24]={2,6,335,1024,18765,25500,33,5,7,10,20,150,1,0,0,20,40,0,0,0,0,0,2,0};memcpy(p,initial,sizeof(p));run();REQUIRE(RT_U(0xd3b138)==1&&RT_U(0xd3a6ec)==1&&RT_U(0xd3a6f0)==1);puts("Rich message text: measurement, hyperlink, typed counters and glyph metrics passed.");}done:VirtualFree((void *)DST,0,MEM_RELEASE);VirtualFree((void *)SRC,0,MEM_RELEASE);VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;}
