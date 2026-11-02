#include "menu_resource_render.h"
#include "town_menu.h"
#include "town_trade.h"
#include "menu_actor_message.h"
#include "menu_record_queries.h"
#include "sprite_render.h"
#include "tool_selector_input.h"
#include "menu_text_glyph.h"
#include "menu_training_render.h"
#include "viewport_controller.h"
#include <stdlib.h>
extern void FUN_0040fa40(int32_t,int32_t,uint16_t);
extern uint32_t FUN_0047d350(void);
extern int32_t FUN_004cc6c0(int32_t,int32_t,uint32_t);
extern void FUN_004cce90(uint32_t,char,const char *,int32_t,int32_t,int32_t,uint32_t,uint32_t);
extern void FUN_004d6160(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d6250(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d62f0(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d6860(uint32_t,uint32_t,const char *,int32_t,int32_t,uint32_t,uint32_t,int32_t,int32_t);
extern uint32_t FUN_004d5ee0(void);
extern void FUN_00425730(int32_t,int32_t,const RestoreMenuButton *,int32_t);
extern void FUN_004d6480(uint32_t,int32_t,int32_t,int32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_00448330(int32_t,char *,int32_t);
extern void FUN_004ccb30(int32_t,int32_t,int32_t,int32_t,int32_t,int32_t,int32_t);
extern void FUN_004ccbb0(int32_t,int32_t,uint32_t,uint32_t,uint32_t,uint32_t);
extern void FUN_004cccb0(int32_t,int32_t,int32_t,int32_t,int32_t,int32_t);
extern void FUN_004ccfd0(int32_t,int32_t,int32_t,int32_t,int32_t,int32_t,int32_t);
extern void FUN_00536820(int32_t,int32_t,int32_t,int32_t);
extern void FUN_00536c30(int32_t,int32_t,int32_t,uint32_t,uint32_t,int32_t);
extern int32_t FUN_004cde40(int32_t,int32_t),FUN_004cde60(int32_t,int32_t);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define A(n) U(sp+4u*(n))
static void town_eligible(struct restore_resource_render_frame *f)
{
 struct restore_actor_message_frame a;uint32_t i;
 for(i=0;i<8;++i)a.r[i]=f->r[i];a.flags=f->flags;a.boundary=restore_actor_message_dispatch;
 restore_actor_message_C_0044d700(&a);
 for(i=0;i<8;++i)f->r[i]=a.r[i];f->flags=a.flags;
}
void restore_resource_render_dispatch(uint32_t site,uint32_t target,struct restore_resource_render_frame *f)
{
 uint32_t sp=f->r[4],cleanup=0;(void)site;
 switch(target){
 case 0x4e1870u:restore_resource_render_C_004e1870(f);return;
 case 0x4e1a50u:restore_resource_render_C_004e1a50(f);return;
 case 0x4dddc0u:FUN_004dddc0();break;
 case 0x425730u:FUN_00425730((int32_t)A(1),(int32_t)A(2),(const RestoreMenuButton *)(uintptr_t)A(3),(int32_t)A(4));break;
 case 0x448330u:FUN_00448330((int32_t)A(1),(char *)(uintptr_t)A(2),(int32_t)A(3));break;
 case 0x4ccb30u:FUN_004ccb30((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(int32_t)A(5),(int32_t)A(6),(int32_t)A(7));break;
 case 0x4ccbb0u:FUN_004ccbb0((int32_t)A(1),(int32_t)A(2),A(3),A(4),A(5),A(6));break;
 case 0x4cccb0u:FUN_004cccb0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(int32_t)A(5),(int32_t)A(6));break;
 case 0x4cc770u:FUN_004cc770((const uint8_t *)(uintptr_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(uint16_t)A(5));break;
 case 0x4ccfd0u:FUN_004ccfd0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(int32_t)A(5),(int32_t)A(6),(int32_t)A(7));break;
 case 0x536820u:FUN_00536820((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));break;
 case 0x536c30u:FUN_00536c30((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),A(4),A(5),(int32_t)A(6));break;
 case 0x536a30u:{
  struct restore_training_render_frame child;uint32_t i;
  for(i=0;i<8;++i)child.r[i]=f->r[i];child.flags=f->flags;child.boundary=restore_training_render_dispatch;
  restore_training_render_dispatch(site,target,&child);
  for(i=0;i<8;++i)f->r[i]=child.r[i];f->flags=child.flags;return;
 }
 case 0x4cde40u:f->r[0]=(uint32_t)FUN_004cde40((int32_t)A(1),(int32_t)A(2));break;
 case 0x4cde60u:f->r[0]=(uint32_t)FUN_004cde60((int32_t)A(1),(int32_t)A(2));break;
 case 0x520000u:restore_resource_render_C_00520000(f);return;
 case 0x403f10u:f->r[0]=(uint32_t)FUN_00403f10((int32_t)A(1),(int32_t)A(2));break;
 case 0x4d5ed0u:f->r[0]=(uint32_t)FUN_004d5ed0();break;
 case 0x4d5ee0u:f->r[0]=FUN_004d5ee0();break;
 case 0x4d6480u:FUN_004d6480(A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),A(5),A(6),(int32_t)A(7),(int32_t)A(8));break;
 case 0x40fb10u:restore_resource_render_C_0040fb10(f);return;
 case 0x444980u:restore_resource_render_C_00444980(f);return;
 case 0x4449d0u:restore_resource_render_C_004449d0(f);return;
 case 0x447430u:restore_resource_render_C_00447430(f);return;
 case 0x4477d0u:restore_resource_render_C_004477d0(f);return;
 case 0x449370u:restore_resource_render_C_00449370(f);return;
 case 0x449380u:restore_resource_render_C_00449380(f);return;
 case 0x449390u:restore_resource_render_C_00449390(f);return;
 case 0x449750u:restore_resource_render_C_00449750(f);return;
 case 0x44c110u:restore_resource_render_C_0044c110(f);return;
 case 0x44d750u:restore_resource_render_C_0044d750(f);return;
 case 0x4cd3e0u:restore_resource_render_C_004cd3e0(f);return;
 case 0x4cd490u:restore_resource_render_C_004cd490(f);return;
 case 0x4cd4e0u:restore_resource_render_C_004cd4e0(f);return;
 case 0x4cd510u:restore_resource_render_C_004cd510(f);return;
 case 0x4cd710u:restore_resource_render_C_004cd710(f);return;
 case 0x4cd810u:restore_resource_render_C_004cd810(f);return;
 case 0x4cd850u:restore_resource_render_C_004cd850(f);return;
 case 0x4cd890u:restore_resource_render_C_004cd890(f);return;
 case 0x4cd930u:restore_resource_render_C_004cd930(f);return;
 case 0x4cd9d0u:restore_resource_render_C_004cd9d0(f);return;
 case 0x4cdc30u:restore_resource_render_C_004cdc30(f);return;
 case 0x52ec50u:restore_resource_render_C_0052ec50(f);return;
 case 0x52f790u:restore_resource_render_C_0052f790(f);return;
 case 0x52f850u:restore_resource_render_C_0052f850(f);return;
 case 0x52f8b0u:restore_resource_render_C_0052f8b0(f);return;
 case 0x44d700u:town_eligible(f);return;
 case 0x40fa40u:FUN_0040fa40((int32_t)A(1),(int32_t)A(2),(uint16_t)A(3));break;
 case 0x444920u:f->r[0]=(uint32_t)FUN_00444920((int32_t)A(1));break;
 case 0x44baf0u:f->r[0]=(uint32_t)(uintptr_t)restore_town_trade_next((RestoreTownList *)(uintptr_t)f->r[1],(const int16_t *)(uintptr_t)A(1));cleanup=4;break;
 case 0x44bb90u:f->r[0]=(uint32_t)(uintptr_t)restore_town_next((RestoreTownList *)(uintptr_t)f->r[1],(const int16_t *)(uintptr_t)A(1),(uint8_t)A(2));cleanup=8;break;
 case 0x47d350u:f->r[0]=FUN_0047d350();break;
 case 0x4cc6c0u:f->r[0]=(uint32_t)FUN_004cc6c0((int32_t)A(1),(int32_t)A(2),A(3));break;
 case 0x4cce90u:FUN_004cce90(A(1),(char)A(2),(const char *)(uintptr_t)A(3),(int32_t)A(4),(int32_t)A(5),(int32_t)A(6),A(7),A(8));break;
 case 0x4d6160u:FUN_004d6160(A(1),A(2),(int32_t)A(3),(int32_t)A(4),A(5),A(6),A(7),(int32_t)A(8),(int32_t)A(9));break;
 case 0x4d6250u:FUN_004d6250(A(1),A(2),(int32_t)A(3),(int32_t)A(4),A(5),A(6),A(7),(int32_t)A(8),(int32_t)A(9));break;
 case 0x4d62f0u:FUN_004d62f0(A(1),A(2),(int32_t)A(3),(int32_t)A(4),A(5),A(6),(int32_t)A(7),(int32_t)A(8));break;
 case 0x4d6860u:FUN_004d6860(A(1),A(2),(const char *)(uintptr_t)A(3),(int32_t)A(4),(int32_t)A(5),A(6),A(7),(int32_t)A(8),(int32_t)A(9));break;
 case 0x53e590u:f->r[0]=(uint32_t)restore_sprite_draw((RestoreSpriteLibraries *)(uintptr_t)f->r[1],(int32_t)A(1),(int32_t)A(2),(int32_t)A(3));cleanup=12;break;
 default:abort();
 }
 f->r[4]=sp+4u+cleanup;
}
#undef A
void restore_resource_render_raw(uint32_t target,uint32_t registers[8],uint32_t *flags)
{
 struct restore_resource_render_frame f;uint32_t i;
 for(i=0;i<8;++i)f.r[i]=registers[i];f.flags=*flags;f.boundary=restore_resource_render_dispatch;
 restore_resource_render_dispatch(0,target,&f);
 for(i=0;i<8;++i)registers[i]=f.r[i];*flags=f.flags;
}
static uint32_t invoke(uint32_t entry,const uint32_t *args,uint32_t count)
{
 uint32_t stack[512],sp,i;struct restore_resource_render_frame f={{0},2,restore_resource_render_dispatch};
 for(i=0;i<512;++i)stack[i]=0x52ecf811u;
 sp=(uint32_t)(uintptr_t)(stack+384);f.r[4]=sp;
 for(i=0;i<count;++i)stack[385+i]=args[i];
 restore_resource_render_dispatch(0,entry,&f);
 if(f.r[4]!=sp+4u||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
 for(i=0;i<32;++i)if(stack[i]!=0x52ecf811u)abort();
 for(i=385+count;i<512;++i)if(stack[i]!=0x52ecf811u)abort();
 return f.r[0];
}
void FUN_0052ec50(void){(void)invoke(0x52ec50u,0,0);}
/* Whole520000 keeps the native delayed argument cleanup across geometry
 * getters, then supplies complete eight/nine-argument panel/text calls. */
void FUN_00520000(void){(void)invoke(0x520000u,0,0);}
void FUN_004e1870(void){(void)invoke(0x4e1870u,0,0);}
void FUN_004e1a50(void){(void)invoke(0x4e1a50u,0,0);}
void FUN_0052f8b0(void){(void)invoke(0x52f8b0u,0,0);}
void FUN_0052f850(void){(void)invoke(0x52f850u,0,0);}
uint32_t FUN_0052f790(int32_t image,uint32_t state){const uint32_t a[]={(uint32_t)image,state};return invoke(0x52f790u,a,2);}
uint32_t FUN_00444980(uint32_t value){const uint32_t a[]={value};return invoke(0x444980u,a,1);}
void FUN_0040fb10(int32_t p0,int32_t p1){const uint32_t a[]={(uint32_t)p0,(uint32_t)p1};(void)invoke(0x40fb10u,a,2);}
void FUN_004cd3e0(int32_t p0,int32_t p1,int32_t p2,int32_t p3){const uint32_t a[]={(uint32_t)p0,(uint32_t)p1,(uint32_t)p2,(uint32_t)p3};(void)invoke(0x4cd3e0u,a,4);}
void FUN_004cd490(int32_t p0){const uint32_t a[]={(uint32_t)p0};(void)invoke(0x4cd490u,a,1);}
void FUN_004cd4e0(int32_t p0,int32_t p1,int32_t p2){const uint32_t a[]={(uint32_t)p0,(uint32_t)p1,(uint32_t)p2};(void)invoke(0x4cd4e0u,a,3);}
void FUN_004cd510(int32_t p0,int32_t p1,int32_t p2,int32_t p3,int32_t p4){const uint32_t a[]={(uint32_t)p0,(uint32_t)p1,(uint32_t)p2,(uint32_t)p3,(uint32_t)p4};(void)invoke(0x4cd510u,a,5);}
void FUN_004cd710(int32_t p0,int32_t p1,int32_t p2,int32_t p3){const uint32_t a[]={(uint32_t)p0,(uint32_t)p1,(uint32_t)p2,(uint32_t)p3};(void)invoke(0x4cd710u,a,4);}
void FUN_004cd810(void){(void)invoke(0x4cd810u,0,0);}
void FUN_004cd850(void){(void)invoke(0x4cd850u,0,0);}
void FUN_004cd890(int32_t p0,int32_t p1,int32_t p2,int32_t p3){const uint32_t a[]={(uint32_t)p0,(uint32_t)p1,(uint32_t)p2,(uint32_t)p3};(void)invoke(0x4cd890u,a,4);}
void FUN_004cd930(int32_t p0,int32_t p1,int32_t p2,int32_t p3){const uint32_t a[]={(uint32_t)p0,(uint32_t)p1,(uint32_t)p2,(uint32_t)p3};(void)invoke(0x4cd930u,a,4);}
void FUN_004cd9d0(int32_t p0,int32_t p1,int32_t p2,int32_t p3,int32_t p4){const uint32_t a[]={(uint32_t)p0,(uint32_t)p1,(uint32_t)p2,(uint32_t)p3,(uint32_t)p4};(void)invoke(0x4cd9d0u,a,5);}
uint8_t FUN_004cdc30(int32_t p0,int32_t p1){const uint32_t a[]={(uint32_t)p0,(uint32_t)p1};return (uint8_t)invoke(0x4cdc30u,a,2);}
