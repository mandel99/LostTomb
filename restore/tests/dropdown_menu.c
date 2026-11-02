#include "dropdown_menu.h"
#include "standalone_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#define U DM_U
#define I DM_I
#define W DM_W
#define B DM_B
#define CHECK(q) do{if(!(q)){fprintf(stderr,"Dropdown failure line%d\n",__LINE__);ExitProcess(1);}}while(0)
extern uint32_t FUN_004d5ef0(uint32_t);
static uint32_t p[48],events[4096][16],used,result;
static uint8_t initial[0x3b0];
static const uint32_t regions[][2]={{0x5c8f20,0x3b0},{0x705680,192},{0xe38dc0,128},{0xd3b188,32},{0xd3b508,16},{0xd3b5b0,32},{0xe945c8,48},{0xf56f80,64},{0x6efff8,208},{0x6f0ff8,224},{0x6f1ff8,144},{0xe38ea0,96},{0xe39080,16},{0x605a30,32}};
static uint32_t table(void){return p[16]==0?0x5c91f0u:p[16]==1?0x5c9230u:0x6f2000u;}
static void event(uint32_t a,uint32_t n,const uint32_t *args){uint32_t *e;CHECK(used<4096);e=events[used++];memset(e,0,64);e[0]=a;e[1]=n;if(n)memcpy(e+2,args,n*4);e[8]=U(0xd3b198);e[9]=U(0xd3b194);e[10]=U(0xd3b190);e[11]=B(0xe38dda);e[12]=U(0x705730);e[13]=U(0x70572c);e[14]=U(0x705724);e[15]=U(0x705720);}
uint32_t restore_dropdown_clock_ms(void){event(0x56f22c,0,NULL);return p[9];}
static void mutation(void){uint32_t m=p[26];if(m&1)U(0x70571c)=p[44];if(m&2)B(0xe38dda)=p[45];if(m&4)U(0x705724)=table()+p[46]*16u;if(m&8)U(0x705720)=0x6f1000u+p[47]*16u;if(m&16)W(0x6f1002)=0x1234;if(m&32){U(0xf56f8c)=0xabcdef01;U(0xf56fa4)=0x87654321;}if(m&64)B(0x6f000f)=3;}
extern void restore_dropdown_route_body(uint32_t token);
void restore_menu_callback(uint32_t token){(void)token;mutation();}
void restore_dropdown_callback(uint32_t token){event(token,0,NULL);restore_dropdown_route_body(token);}
void FUN_00536990(int32_t x,int32_t y,int32_t width,int32_t height){uint32_t a[]={x,y,width,height};event(0x536990,4,a);mutation();}
void FUN_004ccbb0(int32_t group,int32_t entry,int32_t x,int32_t y,uint32_t font,uint32_t color){uint32_t a[]={group,entry,x,y,font,color};event(0x4ccbb0,6,a);mutation();}
static void prepare(void){uint32_t i;used=result=0;for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)memset((void *)regions[i][0],(uint8_t)(0xa5u^p[29]),regions[i][1]);memcpy((void *)0x5c8f20u,initial,sizeof(initial));
 for(i=8;i<48;++i)B(0x5c9268u+i)=(uint8_t)(p[40]?i%2:0);
 for(i=0;i<8;++i){uint32_t a=0x6f0000u+i*24u;W(a)=(uint16_t)(p[23]+i*80u);W(a+2)=p[24];W(a+4)=0x9876;W(a+6)=p[22];U(a+8)=p[25];B(a+12)=p[21];B(a+13)=p[19];B(a+14)=p[20];B(a+15)=i==1&&p[43]?p[43]:p[18];U(a+16)=p[27]+i;U(a+20)=p[28]+i;}
 for(i=0;i<13;++i){uint32_t a=0x6f1000u+i*16u;W(a)=i*20u;W(a+2)=i+1;U(a+4)=p[25];U(a+8)=0xa1b2c3d4;U(a+12)=p[27]+i;}
 for(i=0;i<8;++i){uint32_t a=0x6f2000u+i*16u;W(a)=10+i*100;W(a+2)=100+i*100;W(a+4)=6;W(a+6)=i+1;U(a+8)=0x6f1000u;W(a+12)=p[42];W(a+14)=0xb6c7;}
 if(p[41])for(i=0;i<8;++i){uint32_t a=0x5c91f0u+i*16u;W(a)=10+i%4*100;W(a+2)=100+i%4*100;W(a+4)=6;}
 for(i=0;i<6;++i)U(0xe945d0u+i*4u)=p[31]+i;
 U(0x705690)=p[10];U(0x705730)=p[11];U(0x7056a8)=p[12];U(0x70572c)=p[13];U(0x705728)=p[15];U(0x705724)=table()+p[17]*16u;U(0x705720)=0x6f1000;
 U(0xe38e34)=p[4];U(0xe38e30)=p[5];B(0xe38e03)=p[6];B(0xe38e02)=p[7];B(0xe38e00)=p[8];B(0xe38dda)=p[14];
 U(0x605a38)=p[30];U(0x605a3c)=p[39];U(0xf56f8c)=p[33];U(0xf56fa4)=p[34];
 U(0xe38eac)=p[35];B(0xe38ec1)=p[36];U(0xe38eec)=p[37];B(0xe38ec3)=p[38];B(0xe39088)=p[39];
}
static void run(void){prepare();switch(p[0]){case 0:result=FUN_00425c40((int32_t)p[2],(int32_t)p[3],(RestoreSquareButton *)0x6f0000u,(int32_t)p[1]);break;case 1:FUN_00425e80((int32_t)p[2],(int32_t)p[3],(RestoreSquareButton *)0x6f0000u,(int32_t)p[1],(int32_t)p[32]);break;case 2:result=FUN_00425f20((RestoreDropdownBar *)table(),(int32_t)p[1]);break;case 3:result=FUN_00426030((RestoreDropdownBar *)table());break;case 4:FUN_00426090();break;case 5:FUN_004260d0();break;case 6:result=FUN_00426100((RestoreDropdownBar *)table(),(int32_t)p[1]);break;case 7:result=FUN_00426150((RestoreDropdownItem *)0x6f1000u,(int32_t)p[1],(int32_t)p[2],(int32_t)p[3]);break;case 8:result=FUN_004d5f60((int32_t)p[32]);break;case 9:FUN_00425430();break;case 10:FUN_00403410();break;case 11:FUN_00403490();break;case 12:FUN_00403500();break;case 13:FUN_00403540();break;case 14:FUN_0040c7d0();break;case 16:result=FUN_004d5ef0(p[32]);break;case 15:result=FUN_00426030((RestoreDropdownBar *)table());B(0xe38e03)=0;result=FUN_00425f20((RestoreDropdownBar *)table(),(int32_t)p[1]);FUN_00425430();break;default:CHECK(0);}}
static void hex(uint32_t a,uint32_t n){static const char d[]="0123456789abcdef";for(uint32_t i=0;i<n;++i){uint8_t v=B(a+i);putchar(d[v>>4]);putchar(d[v&15]);}}
static void output(void){printf("{\"result\":%u,\"state\":\"",result);for(uint32_t i=0;i<sizeof(regions)/sizeof(regions[0]);++i)hex(regions[i][0],regions[i][1]);printf("\",\"events\":[");for(uint32_t i=0;i<used;++i){printf("%s[",i?",":"");for(uint32_t j=0;j<16;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);}
int main(int argc,char **argv){CHECK(restore_data_open());memcpy(initial,(void *)0x5c8f20u,sizeof(initial));if(argc==2&&!strcmp(argv[1],"--oracle-batch")){puts("{\"fields\":48}");fflush(stdout);for(;;){for(uint32_t i=0;i<48;++i)if(scanf("%u",p+i)!=1)goto done;run();output();}}else{p[0]=2;p[1]=4;p[4]=111;p[5]=7;p[11]=1;p[14]=3;p[15]=1;p[17]=0;p[31]=10;p[41]=1;run();CHECK(U(0x705724)==0x5c9200u&&U(0x705730)==2);puts("Dropdown: full selected pointer reaches secondbar without truncation; adjacent state preserved.");}done:restore_data_close();return 0;}
