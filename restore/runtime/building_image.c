#include "building_image.h"
#include "city_footprint_validation.h"
#include "sprite_libraries.h"
#include <stdlib.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define SENTINEL UINT32_C(0x8e47b123)

static void boundary(uint32_t site,uint32_t target,struct restore_building_image_frame *f)
{
 uint32_t sp=f->r[4];(void)site;
 switch(target){
 case 0x4ded50u:
  f->r[0]=(uint32_t)(uintptr_t)restore_city_geometry_004ded50((int32_t *)(uintptr_t)U(sp+4u),(int32_t)U(sp+8u),(int32_t)U(sp+12u),(int32_t)U(sp+16u));
  f->r[4]=sp+4u;return;
 case 0x53dd90u:
  /* All four native callers overwrite EAX/ECX/EDX before consuming them. */
  (void)FUN_0053dd90((RestoreSpriteLibraries *)(uintptr_t)f->r[1],(const char *)(uintptr_t)U(sp+4u),(int32_t)U(sp+8u));
  f->r[4]=sp+12u;return;
 default:abort();
 }
}
int32_t restore_building_image_004df360(int32_t building,uint32_t *mode_out)
{
 uint32_t stack[256];struct restore_building_image_frame f;unsigned i;
 for(i=0;i<256;++i)stack[i]=SENTINEL;for(i=0;i<8;++i)f.r[i]=0;
 stack[129]=(uint32_t)building;stack[130]=(uint32_t)(uintptr_t)mode_out;
 f.flags=2;f.boundary=boundary;f.r[4]=(uint32_t)(uintptr_t)(stack+128);restore_building_image_C_004df360(&f);
 if(f.r[4]!=(uint32_t)(uintptr_t)(stack+129)||stack[128]!=SENTINEL||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
 for(i=0;i<32;++i)if(stack[i]!=SENTINEL)abort();for(i=131;i<256;++i)if(stack[i]!=SENTINEL)abort();
 return (int32_t)f.r[0];
}
int32_t restore_building_slot_004f4aa0(int32_t building)
{
 uint32_t player=U(0xeaac28u),slot;
 for(slot=0;slot<3;++slot)if(U(0xea5facu+player*0x49d8u+slot*4u)==(uint32_t)building)return (int32_t)slot;
 return -1;
}
int32_t FUN_004df360(int32_t building,uint32_t *mode_out)
{return restore_building_image_004df360(building,mode_out);}
int32_t FUN_004f4aa0(int32_t building)
{return restore_building_slot_004f4aa0(building);}
