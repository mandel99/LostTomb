#include "selection_popup.h"
#include "standalone_data.h"
#include <stdio.h>
#include <string.h>
#define CHECK(v) do{if(!(v)){fprintf(stderr,"Selection popup failure line%d\n",__LINE__);ExitProcess(1);}}while(0)
static const uint32_t regions[][2]={{0x6057d0,0x2a0},{0x704da0,0x8d0},{0x7056a0,64},{0x5c5e60,64},
 {0x775900,64},{0x77f740,0x180},{0x6f2000,0x100},{0x784ee0,64},{0x5d2a20,0x840},
 {0xe38da0,0x130},{0xd3b180,0x280},{0xe945e0,0x1100},{0xf56f70,64}};
static uint32_t p[64],events[256][20],used,predicates,tiles,texts,result;
static uint8_t original_button[28];
static void event(uint32_t address,uint32_t n,const uint32_t *argv)
{
    uint32_t *e;CHECK(used<256&&n<=9);e=events[used++];memset(e,0,80);
    e[0]=address;e[1]=n;if(n)memcpy(e+2,argv,n*4);
    e[11]=SP_B(0xe38dda);e[12]=SP_U(0x605a58);e[13]=SP_U(0xd3b198);
    e[14]=SP_U(0x605930)?SP_B(SP_U(0x605930)):0;e[15]=SP_U(0x605938)?SP_U(SP_U(0x605938)):0;e[16]=SP_U(0x605a54);
    e[17]=SP_U(0x605a50);e[18]=SP_U(0x605a3c);
    e[19]=SP_U(0x6057f4u+SP_U(0xd3b198)*4u);
}
int FUN_00443710(void)
{
    event(0x443710,0,NULL);
    if(p[29]&1)SP_U(0xd3b198)=p[30];
    if(p[29]&256)SP_U(0x605930)=0x6f2040;
    if(p[29]&512)SP_U(0x6057f4u+SP_U(0xd3b198)*4u)=p[39];
    return (int)p[26];
}
uint8_t FUN_00442870(int value)
{
    uint32_t a[]={(uint32_t)value},answer=predicates++?p[28]:p[27];event(0x442870,1,a);
    if(predicates==1&&(p[29]&2))SP_U(0xd3b198)=p[31];
    if(predicates==2&&(p[29]&4))SP_U(0xd3b198)=p[32];
    return (uint8_t)answer;
}
void FUN_005344c0(int message){uint32_t a[]={(uint32_t)message};event(0x5344c0,1,a);}
void FUN_004de430(int player){uint32_t a[]={(uint32_t)player};event(0x4de430,1,a);if(p[29]&16)SP_U(0x605a3c)=p[35];}
void FUN_00444b10(void){event(0x444b10,0,NULL);}
void FUN_004bb090(void){event(0x4bb090,0,NULL);}
void FUN_00536820(int x,int y,int w,int h)
{
    uint32_t a[]={(uint32_t)x,(uint32_t)y,(uint32_t)w,(uint32_t)h};event(0x536820,4,a);
    if(tiles++==0){
        if(p[29]&32){SP_U(0x605948)=p[36];SP_U(0xe38da8)=p[50];SP_U(0xe38da4)=p[51];}
        if(p[29]&128){RestoreMenuButton *b=(RestoreMenuButton *)0x704dc8u;b->left=-300;b->right=123;b->top=-32768;b->bottom=32767;}
    }
}
void FUN_00536c30(int k,int x,int y,int w,int h,uint32_t color)
{uint32_t a[]={(uint32_t)k,(uint32_t)x,(uint32_t)y,(uint32_t)w,(uint32_t)h,color};event(0x536c30,6,a);}
void FUN_004d63d0(int k,int x,int y,int w,int h,int selected,int sx,int sy)
{uint32_t a[]={(uint32_t)k,(uint32_t)x,(uint32_t)y,(uint32_t)w,(uint32_t)h,(uint32_t)selected,(uint32_t)sx,(uint32_t)sy};event(0x4d63d0,8,a);}
void FUN_004d6250(int group,int entry,int x,int y,int w,uint32_t font,int flags,int sx,int sy)
{uint32_t a[]={(uint32_t)group,(uint32_t)entry,(uint32_t)x,(uint32_t)y,(uint32_t)w,font,(uint32_t)flags,(uint32_t)sx,(uint32_t)sy};event(0x4d6250,9,a);}
void FUN_004d6380(int group,int entry,RestoreMenuButton *button,uint32_t font,uint32_t color,int x,int y,int sx,int sy)
{
    uint32_t a[]={(uint32_t)group,(uint32_t)entry,(uint32_t)(uintptr_t)button,font,color,(uint32_t)x,(uint32_t)y,(uint32_t)sx,(uint32_t)sy};event(0x4d6380,9,a);
    if(texts++==0&&(p[29]&64)){SP_U(0x605948)=p[37];SP_U(0x605944)=p[38];SP_U(0xe38da8)=p[50];SP_U(0xe38da4)=p[51];}
}
void restore_menu_callback(uint32_t token)
{
    if(restore_selection_popup_action(token))return;
    if(token==0x51f780)return;
    CHECK(token==0x33000100);event(token,0,NULL);
    if(p[29]&8){SP_U(0x605a50)=p[33];SP_B(0xe38dda)=(uint8_t)p[34];}
}
static void setup(void)
{
    uint32_t i,j,base=p[6]*98u;RestoreMenuButton *b=(RestoreMenuButton *)0x704dc8u;
    for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)for(j=0;j<regions[i][1];++j)SP_B(regions[i][0]+j)=(uint8_t)(p[1]+j*37u+i*11u);
    memcpy((void *)0x5c5e78u,original_button,28);
    for(i=0;i<78;++i){SP_U(0x6057f4+i*4)=p[24]+i*p[25];b[i].left=(int16_t)(i/26*200+5);b[i].top=(int16_t)(i%26*20+14);b[i].right=(int16_t)(i/26*200+201);b[i].bottom=(int16_t)(i%26*20+32);b[i].on_left=0x403950;b[i].on_right=0x51f780;b[i].trigger=3;b[i].primary=(int)i;b[i].secondary=0;}
    SP_U(0x6057f4+p[7]*4)=p[23];SP_U(0x77591c)=p[6];SP_U(0xd3b198)=p[7];
    SP_B(0xe38dda)=(uint8_t)p[2];SP_B(0xe38dca)=(uint8_t)p[3];SP_U(0x605a58)=p[4];SP_U(0x605a54)=p[5];
    SP_U(0x605930)=p[40]<14?0x77f766+base+p[40]:0x6f2010;SP_U(0x605934)=0x6f2020;SP_U(0x605938)=0x6f2030;
    SP_U(0x60592c)=p[17];SP_U(0x60593c)=p[16];SP_U(0x605940)=p[12];SP_U(0x605950)=p[21];SP_B(0x6057f0)=(uint8_t)p[22];
    SP_U(0x605a50)=p[56];SP_U(0x605958)=p[57];SP_U(0x605a3c)=p[42];SP_U(0x605a44)=p[6];SP_U(0x605a18)=p[43];SP_B(0x77f760+base)=(uint8_t)p[41];SP_B(0x77f761+base)=(uint8_t)p[58];
    SP_U(0x605944)=p[11];SP_U(0x605948)=p[10];SP_U(0x605954)=p[8];SP_U(0x60594c)=p[9];SP_U(0x6057ec)=p[59];
    SP_U(0x7056c8)=p[52];SP_U(0xf56f8c)=p[53];SP_U(0xe38da8)=p[54];SP_U(0xe38da4)=p[55];SP_U(0xe94d98)=p[47];
    for(i=0;i<256;++i){SP_U(0xe94624+i*16)=p[46];SP_U(0xe9461c+i*16)=0;}
    SP_U(0x6f2080)=1;SP_U(0x6f2088)=0x12348001;SP_U(0x6f208c)=0xabcd7fff;SP_U(0x6f2090)=0x3434ffff;SP_U(0x6f2094)=0x21210003;SP_U(0x6f2098)=p[60]==2?0x6f20a0:0;
    SP_U(0x6f20a0)=2;SP_U(0x6f20a8)=0x1ffff;SP_U(0x6f20ac)=0x10001;SP_U(0x6f20b0)=0x8001;SP_U(0x6f20b4)=0xffff;SP_U(0x6f20b8)=0;
    if(p[60])SP_U(0xe9461cu+(p[2]&255)*16u)=0x6f2080;
    for(i=0xe38df4;i<=0xe38e04;++i)SP_B(i)=0;
    SP_U(0xe38e34)=p[61];SP_U(0xe38e30)=p[62];used=predicates=tiles=texts=result=0;
}
static void helper(void){FUN_00403c00(p[8],p[9],(int)p[10],p[11],p[12],p[13],p[14],p[15],p[16],p[17],p[18],p[19],p[20],(int)p[21],p[22]);}
static void mouse(void)
{
    RestoreMenuButton *b=(RestoreMenuButton *)(uintptr_t)(0x704dc8u+p[45]*28u);
    SP_I(0xe38e34)=(int)((uint32_t)SP_I(0x605954)+(uint32_t)(int32_t)b->left+1u);
    SP_I(0xe38e30)=(int)((uint32_t)SP_I(0x60594c)+(uint32_t)(int32_t)b->top+1u);
    if(p[44]==1)SP_B(0xe38e04)=1;
    if(p[44]==2)SP_B(0xe38df8)=1;
    if(p[44]==3)SP_B(0xe38df7)=1;
    if(p[44]==4){SP_I(0xe38e34)=INT32_MIN;SP_I(0xe38e30)=INT32_MIN;}
    if(p[44]==5)SP_I(0xe38e34)=(int)((uint32_t)SP_I(0x605954)+(uint32_t)(int32_t)b->right);
    if(p[44]==6)SP_I(0xe38e30)=(int)((uint32_t)SP_I(0x60594c)+(uint32_t)(int32_t)b->bottom);
    if(p[44]==7)SP_I(0xe38e30)=(int)((uint32_t)SP_I(0x60594c)+(uint32_t)(int32_t)b->top-1u);
}
static void run(void)
{
    setup();
    switch(p[0]){
    case 0:helper();break;case 1:FUN_00409540();break;
    case 2:FUN_004092b0();break;case 3:FUN_00409300();break;
    case 4:FUN_00403950();break;
    case 5:helper();mouse();restore_selection_popup_input();break;
    case 6:if(p[63]){SP_B(0xe38e04)=1;}FUN_00530d70((int)p[8],(int)p[9],(int)p[48]);break;
    case 7:result=(uint32_t)FUN_004d6740((int)p[8],(int)p[9],0x704dc8,p[10],(int)p[45]);break;
    case 8:FUN_00409540();mouse();restore_selection_popup_input();break;
    default:CHECK(0);
    }
}
static void output(void)
{
    static const char hex[]="0123456789abcdef";static char state[24000];uint32_t i,j,n=0;printf("{\"result\":%u,\"state\":\"",result);
    for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)for(j=0;j<regions[i][1];++j){uint8_t b=SP_B(regions[i][0]+j);CHECK(n+2<=sizeof(state));state[n++]=hex[b>>4];state[n++]=hex[b&15];}
    fwrite(state,1,n,stdout);
    printf("\",\"events\":[");for(i=0;i<used;++i){printf("%s[",i?",":"");for(j=0;j<20;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);
}
int main(int argc,char **argv)
{
    uint32_t i;CHECK(restore_data_open());memcpy(original_button,(void *)0x5c5e78u,28);
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){puts("{\"fields\":64}");fflush(stdout);for(;;){for(i=0;i<64;++i)if(scanf("%u",p+i)!=1)goto done;run();output();}}
    else{memset(p,0,sizeof(p));p[13]=0x6f2010;p[14]=0x6f2020;p[15]=0x6f2030;p[46]=1;p[1]=123;p[0]=1;run();CHECK(SP_U(0x605948)==7&&SP_U(0x605950)==26&&SP_U(0x605938)==0x605a18&&SP_B(0xe38dda)==15);CHECK(!restore_selection_popup_action(0xdeadbeef));puts("Selection popup setup and typed action routing passed.");}
done:restore_data_close();return 0;
}
