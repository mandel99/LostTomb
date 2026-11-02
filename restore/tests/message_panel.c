#include "message_panel_calls.h"
#include "standalone_data.h"
#include <stdlib.h>
#include <fcntl.h>
#include <io.h>
#define BASE 0x30000000u
#define ALT ((RestoreSpriteLibraries *)(BASE+0x10000u))
#define SRC ((uint8_t *)0x31000000u)
#define DST ((uint8_t *)0x31100000u)
#define CAPACITY 0x300000u
#define I MP_I
#define U MP_U
#define W MP_W
#define B MP_B
#define R MP_R
#define E MP_E
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"Message panel failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t count,events[128][20],measured,advance,body_height,draw_height,post_redraw,variant,pixel_mode,render_eax;
static const uint32_t fixed_regions[][2]={{0xd3a6e0,2732},{0xd3b500,232},{0xe20760,64},{0xe38da0,24},{0xe38dc4,16},{0xe38dd4,16},{0x5c7da8,684}};
static uint8_t descriptors[684];
static uint32_t record_address,event_address,frame_bytes;
extern int restore_message_raster_sprite(int,int,int);
extern void restore_message_raster_buttons(int,int,short *,int);
static uint32_t hash(const char *s){uint32_t v=2166136261u;while(*s)v=(v^(uint8_t)*s++)*16777619u;return v;}
static void event(uint32_t address,uint32_t n,const uint32_t *args)
{
 uint32_t *r;REQUIRE(count<128&&n<=8);r=events[count++];memset(r,0,80);r[0]=address;r[1]=n;if(n)memcpy(r+2,args,n*4);
 r[12]=U(0xd3b124);r[13]=U(0xd3b138);r[14]=U(0xd3b150);r[15]=U(0xe20778);r[16]=B(0xe38dca);r[17]=B(0xe38dc9);
}
#define EV(a,...) do{uint32_t args[]={__VA_ARGS__};event(a,sizeof(args)/4,args);}while(0)
void FUN_004c8040(void){event(0x4c8040,0,NULL);memset((void *)0xd3a6f8u,0,1000);U(0xd3a6ec)=0;U(0xd3a6f0)=0;}
void FUN_004dddc0(void){event(0x4dddc0,0,NULL);B(0xe38dca)=(uint8_t)post_redraw;}
void FUN_0051e990(void){event(0x51e990,0,NULL);}
void FUN_00448330(int id,char *out,int capacity){EV(0x448330,id,capacity);snprintf(out,capacity,"Template%u",(uint32_t)id);}
const char *FUN_00447840(void){event(0x447840,0,NULL);U(0xe20778)+=19;return (const char *)(BASE+0x100);}
int FUN_0047d350(int input,int limit){EV(0x47d350,input,limit);I(0xd3b16c)+=2;return (int)variant;}
void FUN_004c8070(const char *text,int x,int y,int width,int calculate,int flags)
{EV(0x4c8070,hash(text),x,y,width,calculate,flags);if(calculate)I(0xd3b138)=measured;else I(0xe20774)+=body_height;}
void FUN_004cc720(const char *s,int x,int y,int w,int f,int c){EV(0x4cc720,hash(s),x,y,w,f,c);I(0xe20778)+=advance;}
void FUN_004cc770(const char *s,int x,int y,int f,int c){EV(0x4cc770,hash(s),x,y,f,c);I(0xe20778)+=advance;}
void FUN_004ccbb0(int g,int i,int x,int y,int f,int c){EV(0x4ccbb0,g,i,x,y,f,c);I(0xe20778)+=advance;}
void FUN_004ccc30(int g,int i,int n,int x,int y,int f,int c){EV(0x4ccc30,g,i,n,x,y,f,c);I(0xe20778)=advance;}
void FUN_004cccb0(int g,int n,int x,int y,int f,int c){EV(0x4cccb0,g,n,x,y,f,c);I(0xe20778)+=advance;}
void FUN_004cce00(int n,char prefix,const char *suffix,int x,int y,int f,int c){EV(0x4cce00,n,(uint8_t)prefix,hash(suffix),x,y,f,c);I(0xe20778)+=advance;}
void FUN_004cd010(const char *s,int x,int y,int w,int f,int c){EV(0x4cd010,hash(s),x,y,w,f,c);I(0xe20774)+=31;}
void FUN_004cd9d0(int x,int y,int w,int h,int c){EV(0x4cd9d0,x,y,w,h,c);}
void FUN_004cd680(int x,int y,int w,int h,int c){EV(0x4cd680,x,y,w,h,c);}
void FUN_00536820(int x,int y,int w,int h){EV(0x536820,x,y,w,h);}
void FUN_00536990(int x,int y,int w,int h){EV(0x536990,x,y,w,h);}
void FUN_00536a30(int x,int y,int w,int h){EV(0x536a30,x,y,w,h);}
int FUN_00538dc0(const char *s,int x,int y,int flag,int mode){EV(0x538dc0,hash(s),x,y,flag,mode);return 7;}
int FUN_00561c96(const char *s){EV(0x561c96,hash(s));return atoi(s);}
void FUN_00425730(int x,int y,const void *table,int n){EV(0x425730,x,y,(uint32_t)table,n);if(pixel_mode)restore_message_raster_buttons(x,y,(short *)table,n);}
void FUN_004cd240(int g,int i,const void *table,int f,int c,int x,int y){EV(0x4cd240,g,i,(uint32_t)table,f,c,x,y);}
void FUN_004cd1a0(int n,const void *table,int hover,int x,int y){EV(0x4cd1a0,n,(uint32_t)table,hover,x,y);}
int FUN_0053e590(int image,int x,int y)
{
 EV(0x53e590,0xf55bb0,image,x,y);if(pixel_mode)return restore_message_raster_sprite(image,x,y);
 W(0xd3b5bc)=(int16_t)draw_height;return 0x55667788;
}
/* These deep sprite paths are intentionally excluded by the in-memory raw
   sprite fixture. A reach is a failed guard, never a successful no-op. */
#define UNREACHED(name) int name(void){REQUIRE(0);return 0;}
UNREACHED(FUN_004ce310) UNREACHED(FUN_004ce480) UNREACHED(FUN_0053f7d0) UNREACHED(FUN_0053f790)
UNREACHED(FUN_004cdf20) UNREACHED(FUN_004cdeb0) UNREACHED(FUN_0040fc40) UNREACHED(FUN_0040fcf0)
UNREACHED(FUN_0040fdd0) UNREACHED(FUN_0040fed0) UNREACHED(FUN_0040ff50) UNREACHED(FUN_004105c0)
UNREACHED(FUN_00410660) UNREACHED(FUN_00410de0) UNREACHED(FUN_00410e80) UNREACHED(FUN_00410f70)
UNREACHED(FUN_00410fb0) UNREACHED(FUN_004114a0) UNREACHED(FUN_00411fd0) UNREACHED(FUN_004100f0)
UNREACHED(FUN_004101c0) UNREACHED(FUN_00410700) UNREACHED(FUN_004109d0) UNREACHED(FUN_00410a70)
UNREACHED(FUN_00427a20) UNREACHED(FUN_0042a160) UNREACHED(FUN_0042b440) UNREACHED(FUN_0051f780)
UNREACHED(FUN_0042c740) UNREACHED(FUN_0042c9a0) UNREACHED(FUN_0042cee0) UNREACHED(FUN_0042d810) UNREACHED(FUN_0042e680)
static void sprite_fixture(RestoreSpriteLibraries *self)
{
 unsigned i;memset(self,0,sizeof(*self));self->next_image=5000;
 for(i=0;i<1000;++i)self->tags[i].image=100+i*4;
 for(i=0;i<5000;++i){uint8_t *r=self->images[i].bytes;*(uint32_t *)r=(uint32_t)SRC;*(uint32_t *)(r+4)=64;*(uint32_t *)(r+16)=1;*(int16_t *)(r+20)=8;*(int16_t *)(r+22)=9;r[0x32]=(pixel_mode&&(draw_height&16))?0:1;}
}
static uint32_t run(const uint32_t p[32])
{
 unsigned i;uint32_t result=0;RestoreMessageRecord *record;RestoreMessageEvent *evt;
 REQUIRE(p[0]<=9&&p[1]<=300);count=0;pixel_mode=p[0]>=8;measured=p[10];advance=37;body_height=48;draw_height=p[31];post_redraw=(p[30]>>8)&255;variant=p[31]&7;render_eax=p[31];
 memset((void *)BASE,0,0x10000);for(i=0;i<sizeof(fixed_regions)/sizeof(fixed_regions[0]);++i)memset((void *)fixed_regions[i][0],0xa5,fixed_regions[i][1]);
 /* The descriptor table is immutable native data except animation bytes. */
 memcpy((void *)0x5c7da8u,descriptors,sizeof(descriptors));
 U(0xd3b124)=p[1];U(0xe951e4)=2;record_address=(uint32_t)R;event_address=(uint32_t)E;
 memset((void *)(record_address-8),0xa5,96);memset((void *)(event_address-8),0xa5,64);record=R;evt=E;
 record->type=(int16_t)p[2];record->subtype=(int16_t)p[3];record->x=(int16_t)p[17];record->y=(int16_t)p[18];record->columns=(int16_t)p[19];record->rows=(int16_t)p[20];
 record->title_x=(int16_t)p[21];record->title_y=17;record->picture1=(int16_t)p[22];record->picture1_x=12;record->picture1_y=28;
 record->picture2=(int16_t)p[23];record->picture2_x=40;record->picture2_y=20;record->caption_x=(int16_t)p[24];record->caption_y=45;record->media_x=20;record->media_y=22;
 record->media=(p[30]&64)?0:0x200;record->title=0x300;record->caption=0x400;record->body=0x500;U(0xe36c74)=BASE;
 strcpy((char *)(BASE+0x100),"Fixture Family");strcpy((char *)(BASE+0x200),(p[30]&4)?"clip.smk":"@17");strcpy((char *)(BASE+0x300),"Fixture title");strcpy((char *)(BASE+0x400),"Fixture caption");strcpy((char *)(BASE+0x500),"Fixture body");
 evt->body_template=(p[7]==1||p[7]==3)?1:-1;evt->title_template=(p[7]==1||p[7]==4)?2:-1;evt->caption_template=(p[7]==1||p[7]==5)?3:-1;
 evt->quantity=(int16_t)p[26];evt->resource=(int16_t)p[25];evt->cost=43;evt->buttons=(uint16_t)p[8];
 U(0xd3a6e8)=p[4];U(0xd3b180)=p[5];U(0xd3aae0)=p[6];B(0xd3b184)=(uint8_t)p[7];*(uint16_t *)0xd3b186u=(uint16_t)p[8];
 U(0xd3b138)=p[9];U(0xd3b130)=p[11];U(0xd3b134)=p[12];U(0xd3b12c)=p[13];U(0xd3b128)=p[14];
 U(0xe38da8)=p[15];U(0xe38da4)=p[16];U(0xe38db0)=640+p[15]*2;U(0xe38dac)=480+p[16]*2;
 U(0xe38d9c)=U(0xe38db0);U(0xe38d98)=U(0xe38dac);U(0xe38d94)=U(0xe38d9c)*2+((pixel_mode&&!(p[30]&0x80000000u))?0:16);U(0xe92e18)=(uint32_t)DST+256;U(0xe92e1c)=BASE+0x8000;
 B(0xe38dca)=p[30]&1;B(0xe38dc9)=0;B(0xe38dc8)=0x5a;B(0xe38dda)=5;
 U(0xd3b140)=(p[30]&2)?256:0;U(0xe950dc)=(p[30]&8)?1:0;
 U(0xd3b150)=120+p[16];U(0xd3b15c)=p[16];U(0xd3b160)=p[15];U(0xd3b154)=p[15]+16;
 U(0xd3b148)=(p[0]==3||p[0]==9)?p[20]:23;U(0xd3b14c)=(p[0]==3||p[0]==9)?p[19]:36;U(0xd3b13c)=22;U(0xd3b144)=48;U(0xd3b16c)=7;U(0xd3b170)=p[27];U(0xd3b174)=17;U(0xd3b178)=-1500;
 U(0xe20778)=0x12345678;U(0xe20774)=11;U(0x785d70)=p[28];B(0xe39088)=(p[30]>>4)&1;U(0x978d7c)=(p[30]&32)?2:0;
 U(0xeaac28)=1;U(0xea5db0+0x49d8)=p[31]%7;
 if((int32_t)p[27]>=0&&p[27]<300){uint32_t external=0x785d74+p[27]*124;memset((void *)external,0,124);B(external+2)=1;W(external+48)=(int16_t)p[29];B(0x77de03+p[27]*106)=3;}
 sprite_fixture(ALT);sprite_fixture(RESTORE_SPRITE_LIBRARIES);
 U(0xf56f8c)=335;U(0xf56f9c)=1005;U(0xf56fa4)=201;U(0xf56fac)=871;U(0xf56fb4)=18765;
 U(0xf55c04)=296;U(0xf55c7c)=18000;U(0xf5690c)=20000;U(0xf571fc)=5700;U(0xf5731c)=23908;U(0xf56494)=1000;U(0xf55c34)=1500;U(0x7056c8)=p[31]&3;*(uint16_t *)0x70e120u=0;
 frame_bytes=U(0xe38d94)*U(0xe38d98)+320;REQUIRE(frame_bytes<CAPACITY);memset(DST,0xa5,CAPACITY);
 if(pixel_mode) {
  static const uint32_t tables[]={0x5c7da8,0x5c7dd0,0x5c7df8,0x5c7e20,0x5c7e48,0x5c7e70,0x5c7ec0,0x5c7ee8,0x5c7f38,0x5c7f88,0x5c7fb0,0x5c7fd8,0x5c8050};
  U(0xd3b55c)=0;U(0xd3b554)=0;U(0xd3b558)=U(0xe38d9c);U(0xd3b550)=U(0xe38d98);
  if(p[0]==8){U(0xd3b160)=p[15]+p[17];U(0xd3b15c)=p[16]+p[18];U(0xd3b154)=p[15]+p[17]+16;U(0xd3b150)=120+p[16]+p[18];}
  for(i=0;i<13;++i){uint32_t a=tables[i];B(a+24)=!(p[31]&8);B(a+25)=(p[31]&1)?20:0;B(a+26)=(p[31]&2)?2:0;B(a+27)=(p[31]&4)?0x41:0;}
 }
 for(i=0;i<256;++i)((uint16_t *)SRC)[i]=(i%7==0)?0xf81f:(uint16_t)(i*37+123);
 if(p[0]==6){ALT->next_image=p[23];*(int16_t *)(ALT->images[0].bytes+20)=(int16_t)p[24];*(int16_t *)(ALT->images[0].bytes+22)=(int16_t)p[25];if(p[22]<5000){*(int16_t *)(ALT->images[p[22]].bytes+20)=(int16_t)p[24];*(int16_t *)(ALT->images[p[22]].bytes+22)=(int16_t)p[25];}U(BASE+0x800)=0xa5a5a5a5;U(BASE+0x804)=0xa5a5a5a5;result=restore_message_sprite_dimensions(ALT,(int32_t)p[22],(int32_t *)(BASE+0x800),(int32_t *)(BASE+0x804));}
 else switch(p[0]){case 0:FUN_004c5bf0(p[1],U(0xd3b140),(uint8_t)p[7]);break;case 1:FUN_004c5e60();break;case 2:FUN_004c6c70();break;case 3:case 9:FUN_004c72b0();break;case 4:case 8:FUN_004c7340();break;case 5:FUN_004c5ce0();break;case 7:FUN_004c5cc0();break;}
 if(pixel_mode){for(i=0;i<256;++i){uint16_t expected=(i%7==0)?0xf81f:(uint16_t)(i*37+123);REQUIRE(((uint16_t *)SRC)[i]==expected);REQUIRE(DST[i]==0xa5);}for(i=frame_bytes-64;i<frame_bytes;++i)REQUIRE(DST[i]==0xa5);}
 return result;
}
static void printbytes(uint32_t a,uint32_t n){uint32_t i;for(i=0;i<n;++i)printf("%02x",B(a+i));}
static void output(uint32_t result)
{
 unsigned i,j;printf("{\"result\":%u,\"state\":\"",result);
 for(i=0;i<sizeof(fixed_regions)/sizeof(fixed_regions[0]);++i)printbytes(fixed_regions[i][0],fixed_regions[i][1]);
 printbytes(record_address-8,96);printbytes(event_address-8,64);printbytes(BASE+0x800,8);
 printf("\",\"events\":[");for(i=0;i<count;++i){printf("%s[",i?",":"");for(j=0;j<20;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}
 printf("],\"pixels_bytes\":%u}\n",pixel_mode?frame_bytes:0);if(pixel_mode)REQUIRE(fwrite(DST,1,frame_bytes,stdout)==frame_bytes);fflush(stdout);
}
int main(int argc,char **argv)
{
 uint32_t p[32]={1,3,0,0,0,0,0,0,0,0,40,0,20,0,0,192,144,0,0,40,30,0,0,0,0,24,3,1,3,0,0x100,32},i;
 REQUIRE(restore_data_open());memcpy(descriptors,(void *)0x5c7da8u,sizeof(descriptors));REQUIRE(VirtualAlloc((void *)BASE,0x500000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);
 REQUIRE(VirtualAlloc(SRC,0x100000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==SRC);REQUIRE(VirtualAlloc(DST,CAPACITY,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==DST);
 if(argc==2&&!strcmp(argv[1],"--reject-invalid-external")){_set_abort_behavior(0,_WRITE_ABORT_MSG|_CALL_REPORTFAULT);p[0]=2;p[2]=2;p[3]=2;p[27]=0xffffffffu;run(p);REQUIRE(0);}
 if(argc==2&&!strcmp(argv[1],"--oracle-batch")){REQUIRE(_setmode(_fileno(stdout),_O_BINARY)!=-1);puts("{\"fields\":32}");fflush(stdout);for(;;){for(i=0;i<32;++i)if(scanf("%u",p+i)!=1)goto done;output(run(p));}}
 else{run(p);REQUIRE(I(0xd3b160)==192&&I(0xd3b15c)==144&&I(0xd3b134)>0);p[0]=0;p[1]=256;p[7]=1;p[8]=0x100;p[30]|=8;run(p);REQUIRE(I(0xd3b124)==256&&*(uint16_t *)0xd3b186u==0x100&&I(0xd3b180)==1);puts("Message panel: normal/media producers, typed records/setup, controls, thumb and dimensions.");}
 done:VirtualFree(DST,0,MEM_RELEASE);VirtualFree(SRC,0,MEM_RELEASE);VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;
}
