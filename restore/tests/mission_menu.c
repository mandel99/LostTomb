#include "mission_menu.h"
#include "file_list.h"
#include "sound_streams.h"
#include "standalone_data.h"
#include <stdio.h>
#include <string.h>
#define BASE 0x30000000u
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define W(a) (*(int16_t *)(uintptr_t)(a))
extern uint32_t FUN_0041e140(void);
extern unsigned char FUN_0041e170(void);
extern int FUN_0041f210(void);
extern int FUN_0041e470(void);
extern void FUN_0041e1b0(void);
static uint32_t count,events[160][16],checks,mask,small_calls,big_calls,metric;
#define REQUIRE(x) do{++checks;if(!(x)){fprintf(stderr,"Mission menu failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t hash(const char *s) {uint32_t v=2166136261u;while(*s)v=(v^(uint8_t)*s++)*16777619u;return v;}
static void event(uint32_t a,uint32_t n,const uint32_t *v)
{
    REQUIRE(count<160&&n<=8);memset(events[count],0,sizeof(events[count]));events[count][0]=a;events[count][1]=n;if(n)memcpy(events[count]+2,v,n*4);
    events[count][10]=U(0x704db0);events[count][11]=U(0x704dbc);events[count][12]=U(0xd3b198);events[count][13]=B(0xe38dca);events[count][14]=B(0xe38dda);events[count][15]=B(0x7027b4);++count;
}
#define E(a,...) do{uint32_t v[]={__VA_ARGS__};event(a,sizeof(v)/4,v);}while(0)
int FUN_0053e590(int image,int x,int y) {E(0x53e590,image,x,y);return 1;}
void FUN_00536a30(int x,int y,int w,int h) {E(0x536a30,x,y,w,h);}
void FUN_004cc110(int g,int i) {E(0x4cc110,g,i);U(0xe20768)=BASE+0x4000;}
void FUN_004cc160(int g,int i) {E(0x4cc160,g,i);U(0xe20764)=BASE+0x4100;}
void FUN_00448370(const char *s,char *out,int capacity) {E(0x448370,hash(s),capacity,U(0x784f0c));REQUIRE(capacity==200);strcpy(out,"Restore missions");}
void FUN_004cc720(const char *s,int x,int y,int w,int font,int c) {E(0x4cc720,hash(s),x,y,w,font,c);}
void FUN_004cc770(const char *s,int x,int y,int font,int c) {E(0x4cc770,hash(s),x,y,font,c);}
void FUN_004ccb30(int g,int i,int x,int y,int w,int font,int c) {E(0x4ccb30,g,i,x,y,w,font,c);U(0xe20778)+=17;}
void FUN_004ccbb0(int g,int i,int x,int y,int font,int c) {E(0x4ccbb0,g,i,x,y,font,c);U(0xe20778)+=17;}
void FUN_004cccb0(int kind,int value,int x,int y,int font,int c) {E(0x4cccb0,kind,value,x,y,font,c);}
void FUN_004ccfd0(int g,int i,int x,int y,int w,int font,int c) {E(0x4ccfd0,g,i,x,y,w,font,c);}
void FUN_004cce00(int value,int max,const char *suffix,int x,int y,int font,int c) {E(0x4cce00,value,max,(uint32_t)suffix,x,y,font,c);U(0xe20778)+=17;}
void FUN_004ccf20(int value,const char *first,const char *second,int x,int y,int w,int font,int c) {E(0x4ccf20,value,hash(first),hash(second),x,y,w,font,c);U(0xe20778)+=17;}
void FUN_004ccd60(const char *first,const char *second,int x,int y,int w,int font,int c) {E(0x4ccd60,hash(first),hash(second),x,y,w,font,c);U(0xe20778)+=17;}
void FUN_004cd1a0(int kind,const void *rect,int selected,int x,int y) {E(0x4cd1a0,kind,(uint32_t)rect,selected,x,y);}
void FUN_004cd240(int g,int i,const void *rect,int font,int c,int x,int y) {E(0x4cd240,g,i,(uint32_t)rect,font,c,x,y);}
void FUN_00425730(int x,int y,const void *buttons,int n) {E(0x425730,x,y,(uint32_t)buttons,n);}
int FUN_00425990(int x,int y,const void *buttons,int n) {E(0x425990,x,y,(uint32_t)buttons,n);return (mask>>small_calls++)&1;}
int FUN_00426450(int x,int y,const void *buttons,int n) {E(0x426450,x,y,(uint32_t)buttons,n);return (mask>>8)&255;}
int FUN_004d6740(int x,int y,const void *buttons,int n,int hit) {E(0x4d6740,x,y,(uint32_t)buttons,n,hit);return 256+hit;}
int FUN_004261d0(int x,int y,const void *buttons,int n) {E(0x4261d0,x,y,(uint32_t)buttons,n);return (mask>>(4+big_calls++))&1;}
void FUN_0051f780(void) {}
int FUN_004cde60(int n,int d) {return n*100/d;}
int FUN_004cde40(int value,int percent) {return value*percent/100;}
int FUN_0041e3b0(void) {return metric;}
void FUN_0055f561(RestoreSound *self,int channel) {E(0x55f561,(uint32_t)self,channel);}
void FUN_0041f650(void) {event(0x41f650,0,NULL);B(0xe38dda)=48;B(0xe38dca)=1;}
void FUN_004181e0(void) {event(0x4181e0,0,NULL);B(0xe38dda)=53;}
void FUN_005405b0(char *path,char scenario) {E(0x5405b0,(uint32_t)path,scenario);if(mask&(1<<20))B(0x7027b4)=0;B(0xe38dda)=0x55;B(0xe38dca)=0x77;}
void FUN_00540c90(void) {event(0x540c90,0,NULL);}
void FUN_00540ed0(void) {event(0x540ed0,0,NULL);}
void FUN_00541160(void) {event(0x541160,0,NULL);}
void FUN_004bc720(void) {event(0x4bc720,0,NULL);}
void FUN_0040f8f0(void *out,int n,int value) {E(0x40f8f0,(uint32_t)out,n,value);memset(out,value,n);}
void FUN_004ce3a0(char *s) {E(0x4ce3a0,hash(s));if(*s>='a'&&*s<='z')*s-=32;}
void FUN_004ce3c0(char *s) {E(0x4ce3c0,hash(s));}
void FUN_004cbea0(void *a,void *b,int n) {E(0x4cbea0,(uint32_t)a,(uint32_t)b,n);}
int FUN_004d5820(const char *path,int header) {E(0x4d5820,hash(path),header);B(0xe38dca)=0x77;return 1;}
int FUN_004d58b0(int mission,int header) {E(0x4d58b0,mission,header);B(0xe38ea8)=0xee;B(0xe38dca)=0x77;return 1;}
uint8_t FUN_0041c740(RestoreFileList *self,const char *path,char mode) {E(0x41c740,(uint32_t)self,(uint32_t)path,mode);return 1;}
int FUN_00562370(const char *a,const char *b) {return strcmp(a,b);}
void FUN_0041ca10(RestoreFileList *self,RestoreFileCompare compare) {REQUIRE(compare==FUN_00562370);E(0x41ca10,(uint32_t)self,0x562370);}
static const uint32_t regions[][2]={{0x704dac,24},{0xd3b198,4},{0xd3b1b3,1},{0xd3b1c0,1},{0xe38dc9,2},{0xe38dda,1},{0xe38dd6,1},{0xe38df8,1},{0xe38e04,1},{0xe38ea8,1},{0x7027b4,1},{0xd3b568,16},{0x784f0c,4},{0xe20764,8},{0xe20778,4},{0x7056c4,8},{0xe39080,4},{0xe37078,64},{0xea187c,4}};
/* op,kind,count,scroll,selected,hover,width,height,flags,thumbnail,map_size,
   metric,year,towns,input_mask,clicked_or_mouse_x,difficulty_or_mouse_y,screen */
static uint32_t run(const uint32_t p[18])
{
    uint32_t i,result=0;RestoreFileList *self=(RestoreFileList *)(BASE+0x2000);
    REQUIRE(p[0]<18&&p[2]<=80&&p[13]<=40);count=small_calls=big_calls=0;mask=p[14];metric=p[11];
    memset((void *)BASE,0,0x10000);for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)memset((void *)(uintptr_t)regions[i][0],0xa5,regions[i][1]);
    memset((void *)0x7848d0,0,0x2000);U(0x704db0)=p[1];U(0x704db4)=p[2];U(0x704db8)=p[3];U(0x704dbc)=p[4];U(0x7056c8)=p[5];U(0x7056c4)=p[5];
    U(0xe38db0)=p[6];U(0xe38dac)=p[7];U(0xe38da8)=(p[6]-640)/2;U(0xe38da4)=(p[7]-480)/2;
    U(0x704dac)=(p[8]&2)?3:2;U(0x605a3c)=(p[8]&1)?1:(p[8]&256);B(0xe38dca)=(p[8]&4)?1:0;
    W(0x784b5e)=p[9];W(0x784b60)=(p[8]&8)?256:0;W(0x7848d8)=p[12];U(0x784904)=p[10];U(0x784f0c)=258;
    U(0x785d70)=p[13];for(i=0;i<p[13];++i)B(0x785d74+i*124+2)=1;
    for(i=0;i<5;++i)W(0x7848e2+i*2)=(p[8]>>(9+i*2))&3;
    U(0x784c24)=p[11];U(0x784c28)=p[11]+1;U(0x784c2c)=p[11]+2;U(0x784c30)=p[11]+3;U(0x784c34)=p[11];U(0x784c38)=258;
    U(0x784c48)=U(0x784c50)=U(0x784c58)=p[8]&16;U(0x784c4c)=U(0x784c54)=U(0x784c5c)=p[11]*100+501;
    B(0x784c3c)=B(0x784c3d)=B(0x784c3f)=(p[8]&32)!=0;W(0x784de0)=258;W(0x784de2)=-1;W(0x784de4)=0;
    U(0x784cb0)=258;U(0xe39080)=p[16];U(0xd3b198)=p[15];B(0xe38dda)=p[17];B(0xe38ea8)=1;B(0xe38df8)=mask&(1<<17)?1:0;B(0xe38e00)=mask&(1<<16)?1:0;
    U(0xe38e34)=U(0xe38da8)+p[15];U(0xe38e30)=U(0xe38da4)+p[16];U(0xeaac28)=0;U(0x7848f8)=1234;U(0x700994)=p[2];
    U(0xf55c2c)=31000;U(0xf55c94)=35000;U(0xf55cb4)=36000;U(0xf55cbc)=37000;U(0xf57314)=46000;U(0xf56494)=49000;
    U(0xf56f8c)=1005;U(0xf56f94)=335;U(0xf56f9c)=871;U(0xf56fa4)=999;U(0xf56fb4)=18765;
    strcpy((char *)0xe37078u,"Map000.map");strcpy((char *)0x784914u,"Mission title");strcpy((char *)(BASE+0x4000),"[player_name] missions");strcpy((char *)(BASE+0x4100),"Secondary");
    for(i=0;i<80;++i){U(BASE+0x1000+i*4)=i*32;sprintf_s((char *)(BASE+0x5000+i*32),32,"map%03u.map",i);sprintf_s((char *)(0x6ff60c+i*50),50,"Mission%03u",i);U(0x704c1c+i*4)=U(BASE+0x201c+i*4)=79-i;}
    self->count=RESTORE_MISSION_FILES->count=p[2];self->offsets=RESTORE_MISSION_FILES->offsets=(uint32_t *)(BASE+0x1000);self->strings=RESTORE_MISSION_FILES->strings=(char *)(BASE+0x5000);
    for(i=0;i<3;++i){uint32_t a=0x7009dc+i*76;U(a)=(p[8]&64)?1200:0;U(a+40)=258;U(a+44)=259;U(a+48)=260;U(a+52)=261;U(a+56)=262;U(a+60)=1234;U(a+64)=2;}
    switch(p[0]) {
    case 0:FUN_0041e5c0();break;case 1:FUN_0041f220();break;case 2:FUN_0041e1b0();break;case 3:result=FUN_0041e170();break;
    case 4:result=FUN_0041e140()*100+FUN_0041f210();break;case 5:result=FUN_0041dfc0(self,(int)p[4]);break;case 6:result=FUN_0041dfe0(self,(int)p[4]);break;case 7:result=(uint32_t)FUN_0041e010(self,(int)p[4]);break;
    case 8:FUN_0041f460();break;case 9:result=FUN_0041f4d0();break;case 10:FUN_0041f870();break;case 11:FUN_0041f670();break;case 12:FUN_0041e0e0();break;
    case 13:FUN_0041f5c0();break;case 14:FUN_00402e50();break;case 15:restore_mission_menu_input();break;case 16:restore_mission_menu_buttons();break;case 17:result=FUN_0041e470();break;
    }return result;
}
static void output(uint32_t result)
{
    uint32_t i,j;printf("{\"result\":%u,\"state\":\"",result);for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)for(j=0;j<regions[i][1];++j)printf("%02x",B(regions[i][0]+j));
    printf("\",\"events\":[");for(i=0;i<count;++i){printf("%s[",i?",":"");for(j=0;j<16;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);
}
int main(int argc,char **argv)
{
    uint32_t p[18]={0,1,80,1,2,256,1024,768,116,19,140,3,0xfffff448,12,0,0,2,61},i,result;
    REQUIRE(restore_data_open());REQUIRE(VirtualAlloc((void *)BASE,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")) {puts("{\"fields\":18}");fflush(stdout);for(;;){for(i=0;i<18;++i)if(scanf("%u",p+i)!=1)goto done;result=run(p);output(result);}}
    else {run(p);REQUIRE(count>30&&B(0xe38dc9)==1);p[0]=6;p[4]=80;REQUIRE(run(p)==3);p[0]=10;p[2]=0;run(p);REQUIRE(B(0xe38dda)==61&&U(0x704db8)==0);puts("Mission menu: renderer, rows, explicit receivers, scrolling, callbacks, controller and native argument ordering.");}
done:VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;
}
