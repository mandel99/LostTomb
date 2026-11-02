#include "menu_training_render.h"
#include "dropdown_menu.h"
#include "training_queries.h"
#include "resource_availability.h"
#include "menu_tail_ui.h"
#include "menu_layout.h"
#include "menu_list_scroll.h"
#include "sprite_render.h"
#include <stdlib.h>
extern void FUN_004256a0(int32_t,int32_t,const RestoreSquareButton *,int32_t);
extern void FUN_004ccc30(uint32_t,int32_t,int32_t,int32_t,int32_t,uint32_t,uint32_t);
extern void FUN_004cce90(uint32_t,char,const char *,int32_t,int32_t,int32_t,uint32_t,uint32_t);
extern void FUN_004cd510(int32_t,int32_t,int32_t,int32_t,int32_t);
extern void FUN_004d5d90(void);
extern void FUN_004d6050(void);
extern void FUN_004d6250(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d62f0(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d6860(uint32_t,uint32_t,const char *,int32_t,int32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_00536820(int32_t,int32_t,int32_t,int32_t);
extern void FUN_00536c30(int32_t,int32_t,int32_t,uint32_t,uint32_t,int32_t);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define A(n) U(sp+4u*(n))
void restore_training_render_dispatch(uint32_t site,uint32_t target,struct restore_training_render_frame *f)
{
 uint32_t sp=f->r[4],cleanup=0;(void)site;
 switch(target){
 case 0x40c760u:FUN_0040c760();break;
 case 0x4cd680u:restore_training_render_C_004cd680(f);return;
 case 0x4d65b0u:restore_training_render_C_004d65b0(f);return;
 case 0x536a30u:restore_training_render_C_00536a30(f);return;
 case 0x559e10u:FUN_00559e10();break;
 case 0x559fc0u:f->r[0]=(uint32_t)FUN_00559fc0((int32_t *)(uintptr_t)A(1));break;
 case 0x55a060u:FUN_0055a060();break;
 case 0x55a0e0u:FUN_0055a0e0();break;
 case 0x4256a0u:FUN_004256a0((int32_t)A(1),(int32_t)A(2),(const RestoreSquareButton *)(uintptr_t)A(3),(int32_t)A(4));break;
 case 0x425c40u:f->r[0]=(uint32_t)FUN_00425c40((int32_t)A(1),(int32_t)A(2),(RestoreSquareButton *)(uintptr_t)A(3),(int32_t)A(4));break;
 case 0x425e80u:FUN_00425e80((int32_t)A(1),(int32_t)A(2),(const RestoreSquareButton *)(uintptr_t)A(3),(int32_t)A(4),(int32_t)A(5));break;
 case 0x443950u:f->r[0]=FUN_00443950((int32_t)A(1));break;
 case 0x4439c0u:f->r[0]=FUN_004439c0((int32_t)A(1));break;
 case 0x443a20u:f->r[0]=FUN_00443a20((int32_t)A(1));break;
 case 0x443b20u:f->r[0]=FUN_00443b20((int32_t)A(1));break;
 case 0x447790u:f->r[0]=(f->r[0]&0xffffff00u)|(uint32_t)FUN_00447790((int32_t)A(1),(int32_t)A(2));break;
 case 0x4ccc30u:FUN_004ccc30(A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(int32_t)A(5),A(6),A(7));break;
 case 0x4cce90u:FUN_004cce90(A(1),(char)A(2),(const char *)(uintptr_t)A(3),(int32_t)A(4),(int32_t)A(5),(int32_t)A(6),A(7),A(8));break;
 case 0x4cd510u:FUN_004cd510((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(int32_t)A(5));break;
 case 0x4cde40u:f->r[0]=(uint32_t)FUN_004cde40((int32_t)A(1),(int32_t)A(2));break;
 case 0x4cde60u:f->r[0]=(uint32_t)FUN_004cde60((int32_t)A(1),(int32_t)A(2));break;
 case 0x4d5d70u:f->r[0]=(uint32_t)FUN_004d5d70((int32_t)A(1));break;
 case 0x4d5d90u:FUN_004d5d90();break;
 case 0x4d5fe0u:f->r[0]=(uint32_t)(uintptr_t)FUN_004d5fe0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));break;
 case 0x4d6050u:FUN_004d6050();break;
 case 0x4d6250u:FUN_004d6250(A(1),A(2),(int32_t)A(3),(int32_t)A(4),A(5),A(6),A(7),(int32_t)A(8),(int32_t)A(9));break;
 case 0x4d62f0u:FUN_004d62f0(A(1),A(2),(int32_t)A(3),(int32_t)A(4),A(5),A(6),(int32_t)A(7),(int32_t)A(8));break;
 case 0x4d63d0u:FUN_004d63d0(A(1),A(2),A(3),A(4),A(5),A(6),A(7),A(8));break;
 case 0x4d6860u:FUN_004d6860(A(1),A(2),(const char *)(uintptr_t)A(3),(int32_t)A(4),(int32_t)A(5),A(6),A(7),(int32_t)A(8),(int32_t)A(9));break;
 case 0x536820u:FUN_00536820((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));break;
 case 0x536c30u:FUN_00536c30((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),A(4),A(5),(int32_t)A(6));break;
 case 0x53e590u:f->r[0]=(uint32_t)restore_sprite_draw((RestoreSpriteLibraries *)(uintptr_t)f->r[1],(int32_t)A(1),(int32_t)A(2),(int32_t)A(3));cleanup=12;break;
 case 0x5596d0u:FUN_005596d0();break;
 default:abort();
 }
 f->r[4]=sp+4u+cleanup;
}
#undef A
static uint32_t invoke(uint32_t entry,const uint32_t *args,uint32_t count)
{
 uint32_t stack[256],i,sp;struct restore_training_render_frame f={{0},2,restore_training_render_dispatch};
 for(i=0;i<256;++i)stack[i]=0x55a0e011u;sp=(uint32_t)(uintptr_t)(stack+192);f.r[4]=sp;
 for(i=0;i<count;++i)stack[193+i]=args[i];
 restore_training_render_dispatch(0,entry,&f);
 if(f.r[4]!=sp+4u||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
 for(i=0;i<32;++i)if(stack[i]!=0x55a0e011u)abort();
 for(i=193+count;i<256;++i)if(stack[i]!=0x55a0e011u)abort();
 return f.r[0];
}
void FUN_004cd680(int32_t x,int32_t y,int32_t width,int32_t height,int32_t color)
{const uint32_t a[]={(uint32_t)x,(uint32_t)y,(uint32_t)width,(uint32_t)height,(uint32_t)color};(void)invoke(0x4cd680u,a,5);}
void FUN_004d65b0(int32_t x,int32_t y,int32_t columns,int32_t rows,int32_t origin_x,int32_t origin_y)
{const uint32_t a[]={(uint32_t)x,(uint32_t)y,(uint32_t)columns,(uint32_t)rows,(uint32_t)origin_x,(uint32_t)origin_y};(void)invoke(0x4d65b0u,a,6);}
void FUN_00536a30(int32_t x,int32_t y,int32_t columns,int32_t rows)
{const uint32_t a[]={(uint32_t)x,(uint32_t)y,(uint32_t)columns,(uint32_t)rows};(void)invoke(0x536a30u,a,4);}
/* Register this resolver in the central callback router during integration.
 * Original immutable5C5B78/5C5B90 both contain exactly0040C760. */
int restore_training_render_action(uint32_t token)
{if(token!=0x40c760u)return 0;FUN_0040c760();return 1;}
