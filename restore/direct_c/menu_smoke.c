#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "menu_input_controller.h"
#include "menu_input_boundary.h"
#include "menu_input_entry.h"
#include "standalone_data.h"
#include "monument_base_queries.h"
#include "campaign_score.h"
#include "resource_availability.h"
#include "menu_prepare_control.h"
#define U(a) (*(uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(uint16_t *)(uintptr_t)(uint32_t)(a))
#define CHECK(x) do{if(!(x)){fprintf(stderr,"direct menu check line%d\n",__LINE__);return 1;}}while(0)
static uint32_t map_input_calls,object_input_calls,group_input_calls;
static void observed_boundary(uint32_t site,uint32_t target,struct restore_menu_input_frame *frame)
{
 if(target==0x419090u)++map_input_calls;
 if(target==0x4f96e0u)++object_input_calls;
 if(target==0x406720u)++group_input_calls;
 restore_menu_input_boundary_dispatch(site,target,frame);
}
int main(void)
{
 static const int32_t positions[3][2]={{-1000,-1000},{8000,8000},{0,0}};
 struct restore_menu_input_frame frame;uint32_t stack[256],progress[8],options[17],receiver=0x570160u,i,j,entry;
 uint8_t expected_scratch[32];
 SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());
 restore_monument_base_initialize();
 CHECK(U(0x6fd5dcu)==2&&U(0x6fd36cu)==2&&U(0x6de54cu)==3);
 CHECK(U(0x5cca40u)==0xffffff1cu&&U(0x5cca44u)==1&&U(0x5cca48u)==228&&U(0x5cca4cu)==0xffffffffu);
 CHECK(U(0x5ccf58u)==0&&U(0x5ccf5cu)==0xffffffffu);
 for(i=0;i<22;++i){
  B(0xe38ddau)=i>=18?9:i>=14?0x34:i==3?0x18:i==5?4:i==12?0x17:i>=10?0x35:i>=6?0x33:0;B(0xe38dcau)=0;
  B(0xe38df7u)=i==3;B(0xe38df8u)=i==5||i==12||i>=18;U(0xe38e14u)=0;B(0xe38e00u)=B(0xe38e02u)=B(0xe38e04u)=0;B(0xe38e03u)=i==4||i==9||i==13;
  B(0x785d6cu)=0x53;
  U(0x6fdca4u)=0xabcdef53u;U(0x6fdca8u)=0x7fffffffu;
  B(0xe38dd9u)=0;B(0xea17fcu)=0;B(0xea1380u)=i>=7;
  U(0x784c48u)=i>=8?0x100:0;U(0xeaac28u)=0;U(0xea6018u)=i==9?4:1;
  U(0xf53db4u)=U(0xf53db8u)=256;
  U(0x6fdc9cu)=U(0x6fdca0u)=100;
  U(0x6ff608u)=1;U(0x6fdd40u)=2;B(0xe38eaau)=i==11?0:255;
  U(0x6fdc98u)=1;B(0x700998u)=0;
  memcpy((void *)0xe38e74u,"__restore_smoke_20261031__",sizeof("__restore_smoke_20261031__"));
  U(0xe38da8u)=U(0xe38da4u)=0;
  U(0xe38e34u)=(uint32_t)positions[i%3][0];U(0xe38e30u)=(uint32_t)positions[i%3][1];
  if(i==4){U(0xe38e34u)=320;U(0xe38e30u)=232;}
  if(i>=6)U(0xe38e34u)=U(0xe38e30u)=0xfffffc18u;
  if(i==9){U(0xe38e34u)=320;U(0xe38e30u)=360;}
  if(i==13){
   U(0xe38e34u)=674;U(0xe38e30u)=520;
   U(0xf55b88u)=0;U(0xf55b94u)=0xa5a5a5a5u;
   U(0xea5f8cu)=0x01020304u;U(0xea6018u)=4;U(0xe39080u)=2;B(0xe38dd6u)=255;
  }
  if(i>=14&&i<18){
   B(0xe38eabu)=0;B(0x7027a4u)=0;B(0xe38e04u)=i!=16;
   U(0x6fdd28u)=50;U(0x6fdd2cu)=70;U(0x6fdd34u)=150;U(0x6fdd38u)=170;
   U(0xe38e34u)=i==15?150:i==17?94:50;U(0xe38e30u)=i==15?170:70;
   map_input_calls=0;
  }
  if(i>=18){
   U(0xeadd50u)=i==18?0:4;U(0xeadcccu)=1;
   B(0xbcba80u)=1;W(0xbcba5cu)=i==20?13:57;W(0xbcba82u)=i==20?0:i==21?6:1;
   W(0xbcbadau)=0xffff;W(0xbcbad6u)=0;B(0xbcbaCFu)=1;B(0xbcbabeu)=0;B(0xbcbacbu)=0;
   B(0xe38e60u)=0;U(0xbcbae4u)=0x12345678u;
   object_input_calls=group_input_calls=0;
  }
  for(j=0;j<256;++j)stack[j]=0xa5a5a5a5u;entry=(uint32_t)(uintptr_t)(stack+128);stack[128]=0x12345678u;
  memset(&frame,0,sizeof(frame));frame.r[4]=entry;frame.flags=2;frame.boundary=observed_boundary;
  restore_menu_input_controller(&frame);
  CHECK(frame.r[4]==entry+4u&&stack[128]==0x12345678u&&stack[129]==0xa5a5a5a5u);
  CHECK(frame.r[3]==0&&frame.r[5]==0&&frame.r[6]==0&&frame.r[7]==0);
  if(i==0){FUN_004d8310();CHECK(B(0xe38ddau)==0);}
  if(i==3||i==5)CHECK(B(0xe38ddau)==0x53&&B(0xe38dcau)==1);
  if(i==4)CHECK(B(0xe38ddau)==0x53&&U(0xd3b198u)==3&&B(0x57bdedu)==1);
  if(i>=6&&i<=8)CHECK(B(0xe38ddau)==0x33&&U(0x7056c8u)==0);
  if(i==9)CHECK(B(0xe38ddau)==1&&B(0xe38dcau)==1&&U(0xea6018u)==1&&U(0xe39080u)==1&&B(0x5c6691u)==1);
  if(i==10||i==11)CHECK(B(0xe38ddau)==0x35);
  if(i==12)CHECK(B(0xe38ddau)==0x30&&B(0xe38dcau)==1&&B(0xd3b18au)==0&&B(0xd3b18bu)==0);
  if(i==13)CHECK(B(0xe38ddau)==1&&B(0xe38dcau)==1&&B(0xe38dd6u)==2&&U(0xea5f8cu)==0&&U(0xea6018u)==2&&U(0xf55b94u)==0);
  if(i>=14&&i<18)CHECK(map_input_calls==1&&B(0xe38ddau)==0x34&&B(0x7027a4u)==(i<16?i-13:0)&&B(0xe38dcau)==(i<16));
  if(i>=18)CHECK(object_input_calls==1&&group_input_calls==(i==19)&&B(0xe38dcau)==1&&B(0xe38ddau)==(i==20?0x49:i==19?0x1e:1));
  if(i==19)CHECK(W(0xbcba82u)==2&&U(0xbcbae4u)==0xffffffffu&&U(0xbcb9c0u)==1);
 }
 /* Exercise the typed return bridge with a high-byte-only pending tool flag. */
 U(0x6059e8u)=0x100;U(0xbca310u)=0x100;B(0xe38ddau)=1;B(0xe38dcau)=0;
 memset(&frame,0,sizeof(frame));frame.r[4]=entry;
 observed_boundary(0x4d8ad2u,0x405ba0u,&frame);
 CHECK(frame.r[4]==entry+4u&&frame.r[0]==1&&U(0x6059e8u)==0&&U(0xbca310u)==0x100&&B(0xe38dcau)==1);
 CHECK(stack[128]==0x12345678u&&stack[129]==0xa5a5a5a5u);
 /* This original callee consumes two DWORD arguments with RET8. */
 W(0x799af2u)=0;U(0x799b34u)=56;
 for(j=0;j<8;++j)progress[j]=0xcafebabeu;
 stack[129]=1;stack[130]=(uint32_t)(uintptr_t)progress;stack[131]=0x89abcdefu;
 memset(&frame,0,sizeof(frame));frame.r[4]=entry;
 observed_boundary(0,0x401540u,&frame);
 CHECK(frame.r[4]==entry+12u&&frame.r[0]==100&&progress[0]==56&&progress[3]==100);
 CHECK(progress[1]==0xcafebabeu&&progress[2]==0xcafebabeu&&stack[128]==0x12345678u&&stack[131]==0x89abcdefu);
 /* Use the owned callback resolver with an explicit receiver and nine pieces. */
 CHECK(U(0x570184u)==0x5477e0u);
 for(j=1;j<=9;++j){U(0x799a34u+j*264u)=0;U(0x799a38u+j*264u)=0;}
 U(0x799b78u)=0x76543210u;
 for(j=0;j<8;++j)progress[j]=0xcafebabeu;
 memset(&frame,0,sizeof(frame));frame.r[4]=entry;frame.r[1]=(uint32_t)(uintptr_t)&receiver;
 observed_boundary(0,0x545740u,&frame);
 CHECK(frame.r[4]==entry+12u&&frame.r[0]==0&&progress[1]==0&&progress[2]==0&&progress[4]==0&&progress[5]==0&&progress[6]==0x76543210u);
 CHECK(progress[0]==0xcafebabeu&&progress[3]==0xcafebabeu&&progress[7]==0xcafebabeu&&stack[131]==0x89abcdefu);
 /* Newly wired leaves use full-width controls and ordinary cdecl cleanup. */
 U(0xe38e34u)=0xfffffc18u;U(0x70e490u)=0;U(0xbca300u)=0x11223344u;
 U(0xbca2c4u)=U(0xbca2c8u)=U(0xbca2ccu)=0xa5a5a5a5u;
 memset(&frame,0,sizeof(frame));frame.r[4]=entry;observed_boundary(0,0x405f80u,&frame);
 CHECK(frame.r[4]==entry+4u&&U(0xbca2c4u)==0&&U(0xbca2c8u)==0&&U(0xbca2ccu)==0&&U(0xbca300u)==0x11223344u);
 B(0xe38df7u)=0;memset(&frame,0,sizeof(frame));frame.r[4]=entry;observed_boundary(0,0x407870u,&frame);
 CHECK(frame.r[4]==entry+4u&&frame.r[0]==0);
 B(0x77f749u)=0;U(0xf53fdcu)=0;progress[0]=progress[1]=0xcafebabeu;
 stack[129]=(uint32_t)(uintptr_t)progress;stack[130]=(uint32_t)(uintptr_t)(progress+1);
 memset(&frame,0,sizeof(frame));frame.r[4]=entry;observed_boundary(0,0x407a10u,&frame);
 CHECK(frame.r[4]==entry+4u&&progress[0]==0&&progress[1]==0xffffffffu&&stack[131]==0x89abcdefu);
 U(0x785d70u)=0x100;U(0x78a620u)=0;B(0xe38e00u)=1;
 U(0xe38da8u)=U(0xe38da4u)=0;U(0xe38e34u)=508;U(0xe38e30u)=178;
 memset(&frame,0,sizeof(frame));frame.r[4]=entry;observed_boundary(0,0x4483f0u,&frame);
 CHECK(frame.r[4]==entry+4u&&frame.r[0]==1&&U(0x78a620u)==122&&B(0xe38dcau)==2);
 U(0xf53e6cu)=0x100;U(0xf53f0cu)=U(0xf53f08u)=U(0xeaac28u)=0;
 U(0xe38e34u)=592;U(0xe38e30u)=206;
 memset(&frame,0,sizeof(frame));frame.r[4]=entry;observed_boundary(0,0x55bb90u,&frame);
 CHECK(frame.r[4]==entry+4u&&frame.r[0]==1&&U(0xea60a4u)==121&&B(0xe38dcau)==2);
 U(0x7056b8u)=0x89abcdefu;U(0xe38e34u)=U(0xe38e30u)=0xfffffc18u;
 B(0xe38e03u)=B(0xe38e04u)=0;B(0xe38e00u)=0;
 memset(&frame,0,sizeof(frame));frame.r[4]=entry;frame.r[0]=0x12345678u;observed_boundary(0,0x425840u,&frame);
 CHECK(frame.r[4]==entry+4u&&(frame.r[0]&255u)==0&&(frame.r[0]&0xffffff00u)==0x12345600u&&U(0x7056b8u)==0x89abcdefu&&U(0x705698u)==0);
 U(0xbca2c4u)=0;B(0xe38ddau)=1;B(0xe38dcau)=0;U(0x6059e8u)=0;
 for(j=0;j<3;++j){
  static const uint32_t targets[3]={0x40b630u,0x40b6d0u,0x40b790u};
  memset(&frame,0,sizeof(frame));frame.r[4]=entry;frame.r[0]=0xffffffffu;observed_boundary(0,targets[j],&frame);
  CHECK(frame.r[4]==entry+4u&&frame.r[0]==0&&B(0xe38ddau)==1&&B(0xe38dcau)==0);
 }
 for(j=0;j<12;++j)U(0xe950f4u+j*4u)=0;
 B(0xe38df8u)=1;B(0xe38ddau)=0x22;B(0xe38dcau)=0;
 memset(&frame,0,sizeof(frame));frame.r[4]=entry;observed_boundary(0,0x4e1e10u,&frame);
 CHECK(frame.r[4]==entry+4u&&B(0xe38ddau)==1&&B(0xe38dcau)==1&&stack[128]==0x12345678u&&stack[131]==0x89abcdefu);
 /* New message dispatcher must distinguish the full selector DWORD. */
 U(0x70569cu)=0x100;B(0xe38ddau)=0x22;B(0xe38dcau)=0;
 memset(&frame,0,sizeof(frame));frame.r[4]=entry;observed_boundary(0,0x4072c0u,&frame);
 CHECK(frame.r[4]==entry+4u&&B(0xe38ddau)==0x22&&B(0xe38dcau)==1);
 U(0x70569cu)=1;U(0xe950dcu)=1;U(0xe209f4u+15u*80u)=0x100;
 U(0xd3ae04u)=U(0xd3aae4u)=U(0xd3a6f8u)=0xa5a5a5a5u;
 memset(&frame,0,sizeof(frame));frame.r[4]=entry;observed_boundary(0,0x4072c0u,&frame);
 CHECK(frame.r[4]==entry+4u&&B(0xe38ddau)==5&&U(0xd3b17cu)==0x22&&U(0xd3b124u)==15&&U(0xd3b180u)==1);
 CHECK(U(0xd3ae04u)==0&&U(0xd3aae4u)==0&&U(0xd3a6f8u)==0);
 U(0xd05704u)=0;B(0xe38ddau)=0x27;B(0xe38dcau)=0;
 memset(&frame,0,sizeof(frame));frame.r[4]=entry;observed_boundary(0,0x40b830u,&frame);
 CHECK(frame.r[4]==entry+4u&&B(0xe38ddau)==1&&B(0xe38dcau)==1);
 U(0xf54118u)=0;U(0xf53ffcu)=0x89abcdefu;
 for(j=1;j<=5;++j)stack[128+j]=0x11110000u+j;
 memset(&frame,0,sizeof(frame));frame.r[4]=entry;frame.r[0]=0xffffffffu;observed_boundary(0,0x538dc0u,&frame);
 CHECK(frame.r[4]==entry+4u&&frame.r[0]==0&&U(0xf53ffcu)==0x89abcdefu&&stack[133]==0x11110005u);
 /* Whole score entry uses the real numeric tail and ordinary by-value ABI. */
 U(0xeaac28u)=0;U(0xea5facu)=U(0xea5fb0u)=U(0xea5fb4u)=0;
 U(0xea5b48u)=U(0xea5b40u)=U(0xea5b3cu)=U(0xea1894u)=U(0xea187cu)=U(0xea6018u)=0;
 U(0xea5d10u)=7;CHECK(FUN_0041c380(12)==-7);
 U(0x7056a0u)=0x101;
 memset(&frame,0,sizeof(frame));frame.r[4]=entry;frame.r[0]=0xffffffffu;observed_boundary(0,0x4061e0u,&frame);
 CHECK(frame.r[4]==entry+4u&&frame.r[0]==0);
 /* The raw integer bridge exports native volatile registers and RET0. */
 stack[129]=1;W(0x799a10u+264u)=0xffff;
 memset(&frame,0,sizeof(frame));frame.r[4]=entry;frame.r[3]=0x11223344u;frame.flags=2;
 observed_boundary(0,0x475b50u,&frame);
 CHECK(frame.r[4]==entry+4u&&frame.r[0]==1&&frame.r[1]==0xffff&&frame.r[2]==0&&frame.r[3]==0x11223344u&&stack[129]==1);
 W(0x799a12u+264u)=0;
 memset(&frame,0,sizeof(frame));frame.r[4]=entry;frame.r[6]=0x11223344u;frame.r[7]=0x55667788u;frame.flags=2;
 observed_boundary(0,0x45d570u,&frame);
 CHECK(frame.r[4]==entry+4u&&frame.r[0]==0&&frame.r[1]==0&&frame.r[2]==0&&frame.r[6]==0x11223344u&&frame.r[7]==0x55667788u);
 W(0x7999eau+264u)=0xfffe;
 memset(&frame,0,sizeof(frame));frame.r[4]=entry;observed_boundary(0,0x4f6000u,&frame);
 CHECK(frame.r[4]==entry+4u&&frame.r[0]==0xfffffffeu);
 B(0xe38e00u)=B(0xe38e04u)=B(0xe38e03u)=0;stack[129]=0x12345678u;stack[130]=0xabcdef01u;
 memset(&frame,0,sizeof(frame));frame.r[4]=entry;observed_boundary(0,0x407620u,&frame);
 CHECK(frame.r[4]==entry+4u&&stack[129]==0x12345678u&&stack[130]==0xabcdef01u);
 U(0xe38da8u)=U(0xe38da4u)=0;U(0xe38e34u)=U(0xe38e30u)=0xfffffc18u;
 U(0xe2079cu+7u*4u)=0x7027ecu;U(0xe208f0u+7u*4u)=99;memset((void *)0x7027ecu,0,100);
 U(0xe20998u)=0;B(0x7027e0u)=0xa7;
 memset(&frame,0,sizeof(frame));frame.r[4]=entry;observed_boundary(0,0x41d310u,&frame);
 CHECK(frame.r[4]==entry+4u&&U(0x7027e4u)==0&&B(0x7027e0u)==0xa7);
 B(0xe38e60u)=0;U(0xbca2c4u)=0;
 for(j=0;j<2;++j){
  B(0xe38ddau)=0x27;B(0xe38dcau)=0;
  memset(&frame,0,sizeof(frame));frame.r[4]=entry;observed_boundary(0,j?0x40c570u:0x40c440u,&frame);
  CHECK(frame.r[4]==entry+4u&&B(0xe38ddau)==1&&B(0xe38dcau)==1);
 }
 U(0xeadd50u)=1;U(0xeadd4cu)=7;
 memset(&frame,0,sizeof(frame));frame.r[4]=entry;frame.r[0]=0xffffffffu;observed_boundary(0,0x4f8960u,&frame);
 CHECK(frame.r[4]==entry+4u&&frame.r[0]==4);
 stack[129]=1;stack[130]=1;U(0xbca2d8u)=0;
 memset(&frame,0,sizeof(frame));frame.r[4]=entry;frame.r[3]=0x11223344u;frame.flags=2;
 observed_boundary(0,0x4806a0u,&frame);
 CHECK(frame.r[4]==entry+4u&&frame.r[0]<=1&&frame.r[3]==0x11223344u&&stack[129]==1&&stack[130]==1);
 U(0xeaac28u)=0;U(0xea5fa0u)=U(0xea5fa4u)=U(0xea5fa8u)=0;W(0x784de0u)=0;W(0x5d19ceu)=24;
 stack[129]=24;stack[130]=0x101;
 memset(&frame,0,sizeof(frame));frame.r[4]=entry;observed_boundary(0,0x4f5050u,&frame);
 CHECK(frame.r[4]==entry+4u&&frame.r[0]==0x5d1901u&&stack[130]==0x101);
 for(j=0;j<17;++j)options[j]=0xcafebabeu;
 U(0xeadd6cu)=1;W(0x7999e8u+264u)=0x26;stack[129]=(uint32_t)(uintptr_t)(options+1);
 memset(&frame,0,sizeof(frame));frame.r[4]=entry;observed_boundary(0,0x515130u,&frame);
 CHECK(frame.r[4]==entry+4u&&options[0]==0xcafebabeu&&options[16]==0xcafebabeu);
 for(j=0;j<15;++j)CHECK(options[j+1]==(j==13));
 U(0x785d70u)=0;CHECK(FUN_00559480(-1)==0xffffff00u);
 memset((void *)0x77de00u,0,0x1942u);W(0x784d32u)=W(0x784d34u)=0;
 for(j=1;j<36;++j)B(0xea4205u+j)=0;
 for(j=1;j<9;++j)B(0xea41fcu+j)=0;
 U(0xf53e6cu)=U(0xf53dd8u)=0xffffffffu;U(0xf53f00u)=0x89abcdefu;
 memset(&frame,0,sizeof(frame));frame.r[4]=entry;observed_boundary(0,0x5596d0u,&frame);
 CHECK(frame.r[4]==entry+4u&&U(0xf53e6cu)==0&&U(0xf53dd8u)==0&&U(0xf53f00u)==0x89abcdefu);
 for(j=0;j<36;++j)CHECK(U(0xf53e70u+j*4u)==0&&U(0xf53ddcu+j*4u)==0);
 B(0xea4205u+35u)=0x80;B(0xea41fcu+8u)=1;
 memset(&frame,0,sizeof(frame));frame.r[4]=entry;observed_boundary(0,0x5596d0u,&frame);
 CHECK(frame.r[4]==entry+4u&&U(0xf53e6cu)==1&&U(0xf53dd8u)==1&&U(0xf53e70u)==35&&U(0xf53ddcu)==8);
 CHECK(B(0xea4205u+35u)==1&&B(0xea41fcu+8u)==1&&U(0xf53f00u)==0x89abcdefu);
 U(0xbca300u)=0;U(0x6059d0u)=0x89abcdefu;
 memset(&frame,0,sizeof(frame));frame.r[4]=entry;observed_boundary(0,0x405c20u,&frame);
 CHECK(frame.r[4]==entry+4u&&U(0x6059d0u)==0x89abcdefu);
 U(0xbca300u)=0x100;B(0xe38e60u)=0;U(0xbca304u)=3;U(0xbca308u)=4;
 U(0xbca2d8u)=0;U(0xbca310u)=0;U(0x6059e8u)=0x88;U(0x6059e4u)=0;B(0xab623cu+915u)=7;
 memset(&frame,0,sizeof(frame));frame.r[4]=entry;frame.r[3]=0x11223344u;observed_boundary(0,0x405c20u,&frame);
 CHECK(frame.r[4]==entry+4u&&frame.r[3]==0x11223344u&&U(0x6059d0u)==915&&U(0x6059d8u)==4&&U(0x6059e0u)==3);
 CHECK(U(0xbc96ccu)==7&&U(0x6059e4u)==1);
 for(j=15;j<=25;++j)B(0x7027b8u+j)=1;
 U(0xe38ebcu)=0;B(0x6fdcb4u)=0;B(0x6fdcc4u)=1;B(0x6fdcd0u)=B(0x6fdcdcu)=0;
 W(0x784d2au)=1;W(0x784d2cu)=W(0x784d2eu)=0;B(0xe38ddau)=9;U(0xf55b88u)=0;
 for(j=0;j<1000;++j)W(0xe95232u+j*48u)=1;
 W(0xe95232u)=0;U(0xe951ecu)=7;U(0xe951f0u)=0x7fff;
 memset(stack+120,0xcc,32);memset(expected_scratch,0xcc,sizeof(expected_scratch));
 expected_scratch[0]=0x49;expected_scratch[1]=2;expected_scratch[2]=0x48;expected_scratch[3]=2;
 expected_scratch[16]=0x4a;expected_scratch[17]=2;
 memset(&frame,0,sizeof(frame));frame.r[4]=entry;frame.r[3]=0x11223344u;frame.flags=2;
 observed_boundary(0x4d86c7u,0x417b40u,&frame);
 CHECK(frame.r[4]==entry+4u&&frame.r[3]==0x11223344u&&B(0x6fdcb4u)==1);
 CHECK(memcmp(stack+120,expected_scratch,32)==0&&memcmp((void *)0xe95238u,expected_scratch,30)==0);
 CHECK(W(0xe95232u)==131&&U(0xe951ecu)==8&&U(0xe951f0u)==0x8000);
 U(0x785d70u)=0;B(0xbcd602u+388u)=1;B(0xbcd674u+388u)=0;stack[129]=1;
 memset(&frame,0,sizeof(frame));frame.r[4]=entry;frame.r[3]=0x11223344u;frame.flags=2;
 observed_boundary(0x4f8703u,0x53c100u,&frame);
 CHECK(frame.r[4]==entry+4u&&frame.r[3]==0x11223344u&&B(0xbcd674u+388u)==1&&U(0xeadcdcu)==1);
 stack[129]=0;memset(&frame,0,sizeof(frame));frame.r[4]=entry;frame.r[0]=0x89abcdefu;frame.flags=2;
 observed_boundary(0x4f8703u,0x53c100u,&frame);
 CHECK(frame.r[4]==entry+4u&&frame.r[0]==0x89abcdefu&&B(0xbcd674u+388u)==1);
 U(0x605a3cu)=1;memset(&frame,0,sizeof(frame));frame.r[4]=entry;frame.r[3]=0x11223344u;frame.flags=2;
 observed_boundary(0,0x4f7e50u,&frame);
 CHECK(frame.r[4]==entry+4u&&frame.r[0]==0&&frame.r[3]==0x11223344u);
 U(0x605a3cu)=0;U(0xe950d8u)=0;U(0xbca2c8u)=U(0xbca2ccu)=100;
 U(0xbca2d0u)=U(0xbca2d4u)=228;U(0xbca2d8u)=0;U(0xe38dacu)=768;
 U(0xe38e30u)=U(0xf53f78u)=0;W(0xb4e6fcu+22900u*2u)=0;U(0xaf598cu+22900u*4u)=0;
 CHECK(FUN_004f7e50()==1&&U(0xeadd80u)==22900u&&B(0xe38ddau)==9);
 CHECK(stack[128]==0x12345678u&&stack[134]==0xa5a5a5a5u);
 restore_data_close();puts("Direct C menu:22 input passes +1 physical entry +40 ABI/integration checks; real C callees and stack/data guards passed.");return 0;
}
