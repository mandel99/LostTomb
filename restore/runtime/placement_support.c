#include "placement_support.h"
#include "actor_order_flood.h"
#include "terrain_redraw.h"
#include "map_commit_proximity.h"
#include "map_commit_connect.h"
#include "map_commit_costs.h"
#include "map_commit_fields.h"
#include <stdlib.h>

#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define SENTINEL UINT32_C(0x8e47b123)

static void boundary(uint32_t site,uint32_t target,struct restore_placement_support_frame *f)
{
 uint32_t sp=f->r[4];(void)site;
 /* Owned children share the native frame, including volatile registers. */
 switch(target){
 case 0x481420u:restore_placement_support_C_00481420(f);return;
 case 0x459ef0u:restore_placement_support_C_00459ef0(f);return;
 case 0x45b570u:restore_placement_support_C_0045b570(f);return;
 case 0x45b490u:restore_placement_support_C_0045b490(f);return;
 case 0x5178b0u:FUN_005178b0();break;
 case 0x517370u:FUN_00517370();break;
 case 0x517660u:FUN_00517660();break;
 case 0x51ce90u:f->r[0]=FUN_0051ce90((int32_t)U(sp+4u),(int32_t)U(sp+8u));break;
 case 0x470900u:
  f->r[0]=FUN_00470900((int32_t)U(sp+4u),(int32_t)U(sp+8u),(int32_t)U(sp+12u),(int32_t)U(sp+16u));break;
 case 0x47c880u:
  f->r[0]=(uint32_t)FUN_0047c880((int32_t)U(sp+4u),(int32_t)U(sp+8u),(int32_t)U(sp+12u),(int32_t)U(sp+16u),
                              (int32_t)U(sp+20u),(int32_t)U(sp+24u),(int32_t)U(sp+28u));break;
 default:abort();
 }
 f->r[4]=sp+4u;
}

static uint32_t execute(void (*body)(struct restore_placement_support_frame *),const uint32_t *args,unsigned count)
{
 uint32_t stack[256];struct restore_placement_support_frame f;unsigned i;
 for(i=0;i<256;++i)stack[i]=SENTINEL;
 for(i=0;i<8;++i)f.r[i]=0;
 for(i=0;i<count;++i)stack[193+i]=args[i];
 f.flags=2;f.boundary=boundary;f.r[4]=(uint32_t)(uintptr_t)(stack+192);body(&f);
 if(f.r[4]!=(uint32_t)(uintptr_t)(stack+193)||stack[192]!=SENTINEL||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
 for(i=0;i<32;++i)if(stack[i]!=SENTINEL)abort();
 for(i=193+count;i<256;++i)if(stack[i]!=SENTINEL)abort();
 return f.r[0];
}

uint32_t FUN_00457ac0(int32_t x,int32_t y)
{uint32_t a[2]={(uint32_t)x,(uint32_t)y};return execute(restore_placement_support_C_00457ac0,a,2);}
uint32_t FUN_00459fa0(void)
{return execute(restore_placement_support_C_00459fa0,0,0);}
int32_t FUN_0045b4d0(int32_t player,int32_t resource,int32_t amount)
{uint32_t a[3]={(uint32_t)player,(uint32_t)resource,(uint32_t)amount};return (int32_t)execute(restore_placement_support_C_0045b4d0,a,3);}
void FUN_0047f530(int32_t x,int32_t y,int32_t mode)
{uint32_t a[3]={(uint32_t)x,(uint32_t)y,(uint32_t)mode};(void)execute(restore_placement_support_C_0047f530,a,3);}
uint32_t FUN_004bff60(int32_t x,int32_t y,int32_t mode)
{uint32_t a[3]={(uint32_t)x,(uint32_t)y,(uint32_t)mode};return execute(restore_placement_support_C_004bff60,a,3);}
int32_t FUN_004c3730(int32_t x,int32_t y)
{uint32_t a[2]={(uint32_t)x,(uint32_t)y};return (int32_t)execute(restore_placement_support_C_004c3730,a,2);}
uint32_t FUN_00481420(int32_t x,int32_t y,int32_t mode)
{uint32_t a[3]={(uint32_t)x,(uint32_t)y,(uint32_t)mode};return execute(restore_placement_support_C_00481420,a,3);}
uint32_t FUN_00459ef0(int32_t building)
{uint32_t a[1]={(uint32_t)building};return execute(restore_placement_support_C_00459ef0,a,1);}
int32_t FUN_0045b570(int32_t player,int32_t building,int32_t resource,int32_t amount)
{uint32_t a[4]={(uint32_t)player,(uint32_t)building,(uint32_t)resource,(uint32_t)amount};return (int32_t)execute(restore_placement_support_C_0045b570,a,4);}
int32_t FUN_0045b490(int32_t amount)
{uint32_t a[1]={(uint32_t)amount};return (int32_t)execute(restore_placement_support_C_0045b490,a,1);}
