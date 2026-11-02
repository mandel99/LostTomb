#include "message_panel_input.h"
#include "standalone_data.h"
#include <stdlib.h>
#define BASE 0x30000000u
#define I MI_I
#define U MI_U
#define W MI_W
#define B MI_B
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"Message input failure%d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t p[32],events[256][20],count,result,record,mouse_x,mouse_y;
static uint8_t button_bytes[736],choice_bytes[84];
static const uint32_t regions[][2]={{0xd3a6e0,0xb00},{0xe38dc0,256},{0x7056a8,36},{0x5c7da8,736},{0x5e02c0,84},{BASE+0x4f8,128}};
static const uint32_t tables[]={0x5c7da8,0x5c7dd0,0x5c7df8,0x5c7e20,0x5c7e48,0x5c7e70,0x5c7ec0,0x5c7ee8,0x5c7f38,0x5c7f88,0x5c7fb0,0x5c7fd8,0x5c8050};
static void event(uint32_t address,uint32_t n,const uint32_t *args){uint32_t *r;REQUIRE(count<256&&n<=5);r=events[count++];memset(r,0,80);r[0]=address;r[1]=n;if(n)memcpy(r+2,args,n*4);r[8]=U(0xd3b198);r[9]=U(0xd3b194);r[10]=U(0xd3b124);r[11]=U(0xd3b130);r[12]=U(0xd3aae0);r[13]=U(0xd3a6ec);r[14]=B(0xe38dda);r[15]=B(0xe38dca);r[16]=U(0xd3b180);r[17]=U(0xd3a6e8);r[18]=W(0xd3b186);}
#define EV(a,...) do{uint32_t args[]={__VA_ARGS__};event(a,sizeof(args)/4,args);}while(0)
extern int restore_message_input_small_engine(int,int,short *,int),restore_message_input_choice_engine(int,int,RestoreMenuButton *,int),restore_message_input_choice_hover(int,int,const RestoreMenuButton *,int);
extern int restore_message_input_scale(int,int),restore_message_input_percent(int,int);extern uint32_t restore_message_input_native_zero(void);extern void restore_message_input_clear_links(void);
int FUN_00425990(int x,int y,const void *b,int n){EV(0x425990,x,y,(uint32_t)b,n);return restore_message_input_small_engine(x,y,(short *)b,n);}
int FUN_004261d0(int x,int y,RestoreMenuButton *b,int n){EV(0x4261d0,x,y,(uint32_t)b,n);return restore_message_input_choice_engine(x,y,b,n);}
int FUN_00426450(int x,int y,const RestoreMenuButton *b,int n){EV(0x426450,x,y,(uint32_t)b,n);return restore_message_input_choice_hover(x,y,b,n);}
int FUN_004cde40(int value,int percent){EV(0x4cde40,value,percent);return restore_message_input_scale(value,percent);}
int FUN_004cde60(int value,int maximum){EV(0x4cde60,value,maximum);return restore_message_input_percent(value,maximum);}
void FUN_004c8040(void){event(0x4c8040,0,NULL);restore_message_input_clear_links();}
uint32_t FUN_0047d350(int value){EV(0x47d350,value);return restore_message_input_native_zero();}
void FUN_00539170(void){event(0x539170,0,NULL);if(p[29]&1)B(0xd3b17c)=p[30];if(p[29]&2)U(0xd3a6e8)=p[30];}
void restore_menu_callback(uint32_t address){event(address,0,NULL);switch(address){case 0x4c7f30:FUN_004c7f30();break;case 0x4c7e90:FUN_004c7e90();break;case 0x4c7ee0:FUN_004c7ee0();break;case 0x51f780:break;case 0x40a0e0:case 0x4e2360:case 0x4e2380:case 0x44eea0:case 0x44eef0:case 0x44ee30:break;default:REQUIRE(0);}}
static uint32_t action_table(uint32_t action){static const uint32_t a[]={0x5c7e48,0x5c7ec0,0x5c7f88,0x5c7fb0,0x5c7f38,0x5c7ee8,0x5c7fd8,0x5c8050};return a[action<=7?action:0];}
static void run(void){uint32_t i,flags=p[19];int32_t x=p[7],y=p[8],bottom=mi_sub(mi_add(y,mi_cells((int16_t)p[10])),36),sx=mi_sub(mi_add(mi_add(x,16),mi_cells(p[12])),1);RestoreMessageRecord *r;count=0;for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)memset((void *)regions[i][0],0xa5,regions[i][1]);memcpy((void *)0x5c7da8u,button_bytes,sizeof(button_bytes));memcpy((void *)0x5e02c0u,choice_bytes,sizeof(choice_bytes));
 record=0xe209b8u+p[1]*80u;memset((void *)(record-8),0xa5,96);r=(RestoreMessageRecord *)record;r->type=(int16_t)p[2];r->subtype=(int16_t)p[3];r->columns=(int16_t)p[9];r->rows=(int16_t)p[10];
 I(0xd3b124)=p[1];I(0xd3a6e8)=p[4];W(0xd3b186)=p[5];I(0xd3b180)=p[6];I(0xd3b160)=x;I(0xd3b15c)=y;I(0xd3b154)=mi_add(x,16);I(0xd3b150)=p[11];I(0xd3b14c)=p[12];I(0xd3b148)=p[13];I(0xd3b130)=p[14];I(0xd3b134)=p[15];I(0xd3aae0)=p[16];I(0xd3a6ec)=p[17];U(0xd3a6f0)=p[17];I(0xd3b198)=p[18];I(0xd3b194)=0x12345678;B(0xd3b17c)=p[25];B(0xe38dda)=5;
 for(i=0;i<200;++i){U(0xd3aae4+i*4)=256+i;U(0xd3ae04+i*4)=300+i;}
 for(i=0;i<50;++i){uint32_t row=0xd3a6f8+i*20;I(row)=mi_add(x,50+(int32_t)i*40);I(row+4)=mi_add(x,80+(int32_t)i*40);I(row+8)=mi_add(y,100);I(row+12)=mi_add(y,120);I(row+16)=p[24]+i;}
 for(i=0;i<13;++i){uint32_t a=tables[i];B(a+24)=(p[27]&1)?0:1;B(a+25)=p[28];B(a+26)=p[23];B(a+27)=(p[27]&2)?1:0;}
 mouse_x=mouse_y=(uint32_t)-10000;
 switch(p[20]){case 1:mouse_x=p[6]?mi_add(x,448):mi_add(mi_sub(mi_add(x,mi_cells((int16_t)p[9])),38),12);mouse_y=p[6]?mi_add(y,422):mi_add(bottom,12);break;
 case 2:{const int16_t *b=(int16_t *)action_table(p[4]);mouse_x=mi_add(x,16+b[2]/2);mouse_y=mi_add(p[6]?mi_add(y,408):bottom,b[3]/2);break;}
 case 3:mouse_x=mi_add(sx,19);mouse_y=mi_add(p[11],13);break;case 4:mouse_x=mi_add(sx,19);mouse_y=mi_add(mi_add(p[11],mi_cells(p[13])), -13);break;
 case 5:mouse_x=mi_add(sx,20);mouse_y=mi_add(p[11],mi_cells(p[13])/2);break;
 case 6:mouse_x=mi_add(x,50+(int32_t)p[26]*40);mouse_y=mi_add(y,100);break;
 case 7:mouse_x=mi_add(x,31);mouse_y=mi_add(bottom,10);break;case 8:mouse_x=mi_add(x,76);mouse_y=mi_add(p[11],48);break;
 case 9:case 10:case 11:mouse_x=mi_add(x,103+(int32_t)(p[20]-9)*134);mouse_y=mi_add(y,292);break;}
 if(p[0]==6){mouse_x=mi_add(x,(int32_t)(p[20]*50u));mouse_y=y;}
 mouse_x+=p[21];mouse_y+=p[22];U(0xe38e34)=mouse_x;U(0xe38e30)=mouse_y;
 B(0xe38df8)=(flags&1)?1:0;B(0xe38e03)=(flags&2)?1:0;B(0xe38e00)=(flags&4)?1:0;B(0xe38e04)=(flags&8)?1:0;B(0xe38df7)=(flags&16)?1:0;B(0xe38e02)=(flags&32)?1:0;B(0xe38df4)=(flags&64)?1:0;B(0xe38df6)=(flags&128)?1:0;
 for(i=0;i<3;++i){RestoreSmallButton *b=(RestoreSmallButton *)(BASE+0x500)+i;memset(b,0,sizeof(*b));b->left=(int16_t)(i*50);b->width=24;b->height=24;}
 result=0;switch(p[0]){case 0:FUN_004c7770();break;case 1:result=FUN_004c7db0();break;case 2:FUN_004c7f30();break;case 3:result=FUN_004c7f70();break;case 4:FUN_004c7e90();break;case 5:FUN_004c7ee0();break;case 6:FUN_00425bc0(x,y,(void *)(BASE+0x500),p[26],p[18]);break;default:REQUIRE(0);}
 REQUIRE(U(0xd3a6f4)==0xa5a5a5a5);REQUIRE(U(0xd3aae0)==p[16]||p[0]==0||p[0]==5);
}
static void printbytes(uint32_t a,uint32_t n){static const char hex[]="0123456789abcdef";char b[8192];uint32_t i,k;while(n){k=n>4096?4096:n;for(i=0;i<k;++i){uint8_t v=B(a+i);b[2*i]=hex[v>>4];b[2*i+1]=hex[v&15];}REQUIRE(fwrite(b,2,k,stdout)==k);a+=k;n-=k;}}
static void output(void){uint32_t i,j;printf("{\"result\":%u,\"state\":\"",result);for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)printbytes(regions[i][0],regions[i][1]);printbytes(record-8,96);printf("\",\"events\":[");for(i=0;i<count;++i){printf("%s[",i?",":"");for(j=0;j<20;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);}
int main(int argc,char **argv){uint32_t i;REQUIRE(restore_data_open());memcpy(button_bytes,(void *)0x5c7da8u,sizeof(button_bytes));memcpy(choice_bytes,(void *)0x5e02c0u,sizeof(choice_bytes));REQUIRE(VirtualAlloc((void *)BASE,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);if(argc==2&&!strcmp(argv[1],"--oracle-batch")){puts("{\"fields\":32}");fflush(stdout);for(;;){for(i=0;i<32;++i)if(scanf("%u",p+i)!=1)goto done;run();output();}}else{uint32_t initial[32]={0,249,2,4,0,0,0,192,144,40,30,220,36,23,0,20,0,0,0,6,1,0,0,0,310,1,0,0,0,0,0,0};memcpy(p,initial,sizeof(p));run();REQUIRE(B(0xe38dda)==1&&U(0xd3a6e8)==0&&U(0xd3b180)==0);puts("Message panel input: real small-button OK routed close to prior mode; typed controller passed.");}done:VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;}
