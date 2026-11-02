#include "sprite_render.h"
#include "menu_input.h"
#include "standalone_data.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#define I(a) (*(int32_t *)(uintptr_t)(a))
#define NAME ((uint8_t *)0x30000000u)
extern int FUN_004cbf30(const uint8_t *,int);
extern int FUN_004cca60(int,uint8_t,int,int);
extern void FUN_00536710(int,int,int,int,int);
extern void FUN_004180c0(void);
extern void FUN_00536820(int,int,int,int);
extern void FUN_00536a30(int,int,int,int);
extern void FUN_004256a0(int,int,short *,int);
extern void FUN_00425730(int,int,short *,int);
static uint32_t events[4096][10],count,checks;
#define REQUIRE(x) do{++checks;if(!(x)){fprintf(stderr,"Menu render failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
static void event(uint32_t address,unsigned int n,const uint32_t *values)
{REQUIRE(count<4096);memset(events[count],0,sizeof(events[count]));events[count][0]=address;events[count][1]=n;memcpy(events[count]+2,values,n*4);++count;}
int FUN_0053e590(int image,int x,int y) {uint32_t a[3]={(uint32_t)image,(uint32_t)x,(uint32_t)y};event(0x53e590,3,a);return 1;}
void FUN_004cd240(int group,int index,const int16_t *rect,int font,int color,int x,int y)
{uint32_t a[7]={(uint32_t)group,(uint32_t)index,(uint32_t)(uintptr_t)rect,(uint32_t)font,(uint32_t)color,(uint32_t)x,(uint32_t)y};event(0x4cd240,7,a);}
static int run(const uint32_t p[8])
{
    unsigned int i;static const char *strings[]={"","ABC","A B","ABC ABC"};
    for(i=0;i<2048;++i) {
        int16_t width=(int16_t)((i*3+p[6])%19+1),height=(int16_t)((i*7+p[6])%27+1);
        memcpy(RESTORE_SPRITE_LIBRARIES->images[i].bytes+20,&width,2);memcpy(RESTORE_SPRITE_LIBRARIES->images[i].bytes+22,&height,2);
    }
    memset((void *)0x5e047cu,0,256);((uint8_t *)0x5e047cu)['A']=3;((uint8_t *)0x5e047cu)['B']=4;((uint8_t *)0x5e047cu)['C']=5;
    I(0x126517c)=p[7];I(0xf56fb4)=871;I(0xf56fbc)=1005;I(0xf56f84)=201;I(0xf56f8c)=p[1];
    I(0xd3b500)=11;I(0xd3b5c0)=1234;I(0xd3b5e0)=4567;I(0xd3b5dc)=8910;I(0xf56494)=6000;
    I(0xf5683c)=p[6];I(0xf56844)=p[6];
    memset(NAME+0x100,0xa5,128);I(0xe38dc8)=0xa5000000;
    *(uint8_t *)0xe38dc9u=(p[1]&1)!=0;*(uint8_t *)0xe38dcau=(p[1]&2)!=0;
    for(i=0;i<8;++i)I(0xf55bb4+i*8)=p[6]+i*1000;
    if(p[0]==6) {
        RestoreSmallButton *buttons=(RestoreSmallButton *)(NAME+0x100);
        for(i=0;i<3;++i) {
            buttons[i].left=i*17;buttons[i].top=(int16_t)((int)i*31-10);buttons[i].image=(int16_t)i;
            buttons[i].reserved12=100+i*4;buttons[i].enabled=(p[1]&4)!=0;
            buttons[i].pressed=(uint8_t)p[5];buttons[i].hover=(uint8_t)(p[5]>>8);buttons[i].locked=(uint8_t)(p[5]>>16);
        }
    } else if(p[0]==7) {
        for(i=0;i<3;++i) {
            int16_t *button=(int16_t *)(NAME+0x100+i*24);button[0]=i*17;button[1]=(int16_t)((int)i*31-10);button[2]=31234+i;
            ((uint8_t *)button)[12]=(uint8_t)p[5];((uint8_t *)button)[13]=(uint8_t)(p[5]>>8);((uint8_t *)button)[15]=(uint8_t)(p[1]>>3);
        }
    }
    for(i=0;i<20;++i)I(0xf53f10+i*4)=100;
    I(0xf53f10+4)=4;I(0xf53f10+8)=5;I(0xf53f10+12)=14;I(0xf53f10+16)=15;
    for(i=0;i<9;++i){I(0x5f81e4+i*12)=i;I(0x5f81e8+i*12)=(int32_t)i-4;I(0x5f81ec+i*12)=(int32_t)i-3;}
    I(0xe38da8)=p[2];I(0xe38da4)=p[3];I(0x7056c8)=p[5];count=0;
    strcpy((char *)NAME,strings[p[2]%4]);
    if(p[0]==0)return FUN_004cbf30(NAME,(int)p[1]);
    if(p[0]==1)return FUN_004cca60((int)p[1],(uint8_t)p[2],(int)p[3],(int)p[4]);
    if(p[0]==2)FUN_00536710((int)p[1],(int)p[2],(int)p[3],(int)p[4],(int)p[5]);
    else if(p[0]==3)FUN_004180c0();
    else if(p[0]==4)FUN_00536820(p[2],p[3],p[4],p[5]);
    else if(p[0]==5)FUN_00536a30(p[2],p[3],p[4],p[5]);
    else if(p[0]==6)FUN_00425730(p[2],p[3],(short *)(NAME+0x100),p[4]);
    else if(p[0]==7)FUN_004256a0(p[2],p[3],(short *)(NAME+0x100),p[4]);else REQUIRE(0);
    return 0;
}
int main(int argc,char **argv)
{
    uint32_t p[8]={2,6,192,100,16,0,7,2048};int value;unsigned int i,j;
    REQUIRE(restore_data_open());REQUIRE(VirtualAlloc(NAME,4096,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==NAME);
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")) {
        puts("{\"fields\":36}");fflush(stdout);
        for(;;) {
            for(i=0;i<8;++i)if(scanf("%u",&p[i])!=1)goto done;
            value=run(p);printf("{\"result\":%d,\"state\":[%u,%u,%u,%u",value,(uint32_t)I(0xd3b5c0),(uint32_t)I(0xd3b5e0),(uint32_t)I(0xd3b5dc),(uint32_t)I(0xe38dc8));
            for(i=0;i<128;i+=4)printf(",%u",I((uintptr_t)NAME+0x100+i));printf("],\"events\":[");
            for(i=0;i<count;++i){if(i)putchar(',');putchar('[');for(j=0;j<10;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}
            puts("]}");fflush(stdout);
        }
    } else {
        run(p);REQUIRE(count==16&&events[0][2]==6000&&events[0][3]==192&&events[15][2]==6002&&events[15][3]==432);
        p[0]=3;p[1]=335;p[2]=192;p[3]=144;p[5]=3;run(p);
        REQUIRE(count==85&&events[16][0]==0x4cd240&&events[16][2]==30&&events[16][3]==0&&events[16][5]==335);
        REQUIRE(events[0][3]==384&&events[0][4]==244&&events[34][2]==6003);
        p[0]=0;p[1]=335;p[2]=1;REQUIRE(run(p)>0);
        p[0]=1;p[2]='A'-32;p[3]=500;p[4]=300;REQUIRE(run(p)>0&&count==1&&events[0][2]==337&&events[0][3]==500&&events[0][4]==297);
        p[0]=4;p[2]=320;p[3]=304;p[4]=24;p[5]=8;p[6]=35000;run(p);
        REQUIRE(count==192&&events[0][2]==35000&&events[191][2]==35143&&events[191][3]==688&&events[191][4]==416);
        printf("Menu rendering: %u checks; exact segment IDs, hover, full viewport/font values, original label rectangles and glyph placement.\n",checks);
    }
done:VirtualFree(NAME,0,MEM_RELEASE);restore_data_close();return 0;
}
