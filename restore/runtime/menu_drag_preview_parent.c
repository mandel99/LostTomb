#include "preview_parent_model.h"
#include "small_constructor_model.h"
#include "connection_plan_model.h"
#include "map_commit_route_actions.h"
#include "building_rect_commit.h"
#include "drag_commit_model.h"
#include "drag_leaves.h"
#include "map_commit_placement.h"
#include "paving_region_model.h"
#include "canal_region_model.h"
#include "footprint.h"
#include "region_gate.h"
#include "preview_geometry.h"
#include "route_query_model.h"
#include "map_commit_warnings.h"
#include "region_validation.h"
#include <stdlib.h>
extern void FUN_004743f0(int32_t preview,int32_t x1,int32_t y1,int32_t x2,int32_t y2);
extern code *restore_placement_rule(uint32_t token);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define SENTRY UINT32_C(0x8e47b123)
static void boundary(uint32_t site,uint32_t target,struct restore_menu_input_frame *f)
{
 uint32_t sp=f->r[4],a,b,c,d,e;(void)site;
 if(target==0x486020u){FUN_00486020();goto done;}
 a=U(sp+4);
 if(target==0x46fde0u){f->r[0]=FUN_0046fde0(a);goto done;}
 if(target==0x486cf0u){f->r[0]=FUN_00486cf0(a);goto done;}
 b=U(sp+8);
 if(target==0x478440u){FUN_00478440((int32_t)a,(int32_t)b);goto done;}
 c=U(sp+12);
 switch(target){
 case 0x40f920u:FUN_0040f920((void *)(uintptr_t)a,(int32_t)b,(uint8_t)c);goto done;
 case 0x4bfa50u:FUN_004bfa50((int32_t)a,(int32_t)b,c);goto done;
 case 0x47d060u:f->r[0]=(uint32_t)FUN_0047d060((int32_t)a,(int32_t)b,c);goto done;
 }
 d=U(sp+16);
 if(target==0x47cc70u){f->r[0]=(uint32_t)FUN_0047cc70((int32_t)a,(int32_t)b,c,(uint8_t)d);goto done;}
 e=U(sp+20);
 switch(target){
 case 0x46fef0u:FUN_0046fef0((int32_t)a,(int32_t)b,(int32_t)c,(int32_t)d,(int32_t)e);break;
 case 0x440700u:f->r[0]=FUN_00440700(a,b,c,(int32_t)d,(int32_t)e);break;
 case 0x440850u:f->r[0]=FUN_00440850(a,b,c,(int32_t)d,(int32_t)e);break;
 case 0x440b10u:f->r[0]=FUN_00440b10(a,b,c,(int32_t)d,(int32_t)e);break;
 case 0x440ed0u:f->r[0]=FUN_00440ed0(a,b,c,(int32_t)d,(int32_t)e);break;
 case 0x4743f0u:FUN_004743f0((int32_t)a,(int32_t)b,(int32_t)c,(int32_t)d,(int32_t)e);break;
 case 0x47eda0u:f->r[0]=FUN_0047eda0(a,(int32_t)b,(int32_t)c,(int32_t)d,(int32_t)e);break;
 case 0x47bae0u:FUN_0047bae0((int32_t)a,(int32_t)b,(int32_t)c,(int32_t)d,e);break;
 case 0x475220u:FUN_00475220((int32_t)a,(int32_t)b,(int32_t)c,(int32_t)d,(int32_t)e,U(sp+24));break;
 case 0x474f50u:FUN_00474f50((int32_t)a,(int32_t)b,(int32_t)c,(int32_t)d,(int32_t)e);break;
 case 0x4754a0u:FUN_004754a0(a,(int32_t)b,(int32_t)c,(int32_t)d,(int32_t)e);break;
 case 0x4759e0u:FUN_004759e0(a,(int32_t)b,(int32_t)c,(int32_t)d,(int32_t)e);break;
 case 0x475090u:FUN_00475090((int32_t)a,(int32_t)b,(int32_t)c,(int32_t)d,(int32_t)e);break;
 case 0x474130u:restore_building_rect_00474130(a,(int32_t)b,(int32_t)c,(int32_t)d,(int32_t)e);break;
 case 0x47d530u:f->r[0]=FUN_0047d530((int32_t)a,(int32_t)b,(int32_t)c,d,(uint8_t)e);break;
 case 0x486a30u:f->r[0]=FUN_00486a30((int32_t)a,(int32_t)b,(int32_t)c,(int32_t)d,e,(int32_t)U(sp+24),(int32_t)U(sp+28),(uint32_t *)(uintptr_t)U(sp+32),(int32_t *)(uintptr_t)U(sp+36));break;
 case 0x4829b0u:f->r[0]=FUN_004829b0((int32_t)a,(int32_t)b,(int32_t)c,(int32_t)d,e,(int32_t)U(sp+24));break;
 case 0x4828e0u:f->r[0]=FUN_004828e0((int32_t)a,(int32_t)b,(int32_t)c,(int32_t)d,e);break;
 case 0x482650u:f->r[0]=FUN_00482650((int32_t)a,(int32_t)b,(int32_t)c,(int32_t)d,e);break;
 case 0x482720u:f->r[0]=FUN_00482720((int32_t)a,(int32_t)b,(int32_t)c,(int32_t)d,restore_placement_rule(e));break;
 case 0x47d360u:FUN_0047d360((int32_t)a,(int32_t)b,(int32_t)c,d,(const void *)(uintptr_t)e,(uint8_t)U(sp+24));break;
 default:abort();
 }
 done:f->r[4]=sp+4u;
}
static void execute(void (*body)(struct restore_menu_input_frame *),const uint32_t *args,unsigned count)
{
 uint32_t stack[64];struct restore_menu_input_frame f;unsigned i;
 for(i=0;i<64;++i)stack[i]=SENTRY;for(i=0;i<8;++i)f.r[i]=0;
 for(i=0;i<count;++i)stack[33+i]=args[i];
 f.flags=2;f.boundary=boundary;f.r[4]=(uint32_t)(uintptr_t)(stack+32);body(&f);
 if(f.r[4]!=(uint32_t)(uintptr_t)(stack+33)||stack[32]!=SENTRY||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
 for(i=0;i<17;++i)if(stack[i]!=SENTRY)abort();for(i=33+count;i<64;++i)if(stack[i]!=SENTRY)abort();
}
void FUN_0046fef0(int32_t x1,int32_t y1,int32_t x2,int32_t y2,int32_t tool)
{uint32_t args[5]={(uint32_t)x1,(uint32_t)y1,(uint32_t)x2,(uint32_t)y2,(uint32_t)tool};execute(restore_C_0046fef0,args,5);}
void FUN_00405d90(void){execute(restore_C_00405d90,0,0);}
