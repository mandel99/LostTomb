#include "small_constructor_model.h"
#include "road_check_model.h"
#include "road_scan_model.h"
#include "world_offset_model.h"
#include "construction_tiles.h"
#include "building_allocator.h"
#include "road_place.h"
#include "map_commit_state.h"
#include "editor_brush_validation.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define I(a) ((int32_t)(uint32_t)(a))
/* The copy direction is preview backup -> live map, not a new snapshot.
   485830 takes no arguments; the native callers' pushed zero is unused. */
void restore_entertainment_preview_restore(void)
{
 FUN_0040f860((void *)0x9de62cu,(void *)0xaf598cu,0x32c40);
 FUN_0040f860((void *)0x9d1b1cu,(void *)0xae8e7cu,0xcb10);
 FUN_0040f860((void *)0x9b84fcu,(void *)0xb285ccu,0xcb10);
 FUN_0040f860((void *)0x9ab9ecu,(void *)0xb67d1cu,0xcb10);
 FUN_00485830();
}
uint32_t restore_build_booth(uint32_t preview,int32_t x,int32_t y)
{
 uint32_t width=U(0xbca2d0u),height,cell,id,record,image,owner;int32_t bx,by;
 if(x>=I(width))x=I(width-1u);
 height=U(0xbca2d4u);if(y>=I(height))y=I(height-1u);
 cell=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;
 if(!FUN_0043f410(31))return 0;
 FUN_004413b0(x,y,31);
 if(FUN_00441130(x,y,&bx,&by,31,31)==UINT32_MAX)return 0;
 if(preview)return 1;
 restore_entertainment_preview_restore();
 owner=U(0xeaac28u);U(0xbc971cu)=1;
 id=(uint32_t)restore_building_allocate_0046a400(owner,31,bx,by,0,0);record=id*264u;image=U(0xf567acu);
 U(0x799ab0u+record)=1;U(0x799aa8u+record)=cell;
 FUN_0047bf50((uint16_t)id,bx,by,1,1,image,8);(void)FUN_00485100(I(id));
 FUN_00440060(cell,31,(uint16_t)id);return 1;
}
uint32_t restore_build_bandstand(uint32_t preview,int32_t x,int32_t y)
{
 uint32_t cell=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x,rotation,first,second,booth,r1,r2,rb,image;
 int32_t bx,by,cx,cy,first_image,second_image,sx,sy;
 FUN_004413b0(x,y,30);if(!FUN_0043f410(30))return 0;
 rotation=FUN_00441130(x,y,&bx,&by,30,30);if(rotation==UINT32_MAX)return 0;
 if(FUN_00441130(x,y,&cx,&cy,30,31)==UINT32_MAX)return 0;
 if(preview)return 1;
 restore_entertainment_preview_restore();U(0xbc971cu)=1;
 FUN_004405c0(I(rotation),&first_image,&second_image);
 first=(uint32_t)restore_building_allocate_0046a400(U(0xeaac28u),30,bx,by,0,0);r1=first*264u;
 U(0x799ab0u+r1)=1;U(0x799aacu+r1)=rotation;U(0x799aa8u+r1)=cell;
 FUN_0047bf50((uint16_t)first,bx,by,1,1,(uint32_t)first_image,8);(void)FUN_00485100(I(first));
 sx=I((uint32_t)bx+(rotation?1u:0u));sy=I((uint32_t)by+(rotation?0u:1u));
 second=(uint32_t)restore_building_allocate_0046a400(U(0xeaac28u),30,sx,sy,0,0);r2=second*264u;
 FUN_0047bf50((uint16_t)second,sx,sy,1,1,(uint32_t)second_image,8);
 U(0x799aa8u+r2)=cell;U(0x799aacu+r1)=rotation;U(0x799ab0u+r2)=0;(void)FUN_00485100(I(second));
 booth=(uint32_t)restore_building_allocate_0046a400(U(0xeaac28u),31,cx,cy,0,0);rb=booth*264u;image=U(0xf567acu);
 U(0x799ab0u+rb)=0;U(0x799aa8u+rb)=cell;
 FUN_0047bf50((uint16_t)booth,cx,cy,1,1,image,8);(void)FUN_00485100(I(booth));
 W(0x799a12u+r1)=(uint16_t)second;W(0x799a10u+r2)=(uint16_t)first;
 W(0x799a12u+r2)=(uint16_t)booth;W(0x799a10u+rb)=(uint16_t)second;
 FUN_00440060(cell,30,(uint16_t)first);return 1;
}
uint32_t restore_build_pavilion(uint32_t preview,int32_t x,int32_t y)
{
 uint32_t width=U(0xbca2d0u),height,cell,rotation,pavilion,booth,first,second,rp,rb,r1,r2,image,owner;
 int32_t px,py,bx,by,cx,cy,first_image,second_image,sx,sy;
 if(x>=I(width))x=I(width-1u);
 height=U(0xbca2d4u);if(y>=I(height))y=I(height-1u);
 cell=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;
 FUN_004413b0(x,y,33);if(!FUN_0043f410(33))return 0;
 if(FUN_00441130(x,y,&px,&py,33,33)==UINT32_MAX)return 0;
 rotation=FUN_00441130(x,y,&bx,&by,33,30);if(rotation==UINT32_MAX)return 0;
 if(FUN_00441130(x,y,&cx,&cy,33,31)==UINT32_MAX)return 0;
 if(preview)return 1;
 restore_entertainment_preview_restore();owner=U(0xeaac28u);U(0xbc971cu)=1;
 pavilion=(uint32_t)restore_building_allocate_0046a400(owner,33,px,py,0,0);rp=pavilion*264u;image=U(0xf5659cu);
 U(0x799ab0u+rp)=1;U(0x799aa8u+rp)=cell;
 FUN_0047bf50((uint16_t)pavilion,px,py,2,2,image,8);(void)FUN_00485100(I(pavilion));
 booth=(uint32_t)restore_building_allocate_0046a400(U(0xeaac28u),31,cx,cy,0,0);rb=booth*264u;image=U(0xf567acu);
 U(0x799ab0u+rb)=0;U(0x799aa8u+rb)=cell;
 FUN_0047bf50((uint16_t)booth,cx,cy,1,1,image,8);(void)FUN_00485100(I(booth));
 FUN_004405c0(I(rotation),&first_image,&second_image);
 first=(uint32_t)restore_building_allocate_0046a400(U(0xeaac28u),30,bx,by,0,0);r1=first*264u;
 U(0x799ab0u+r1)=0;U(0x799aacu+r1)=rotation;U(0x799aa8u+r1)=cell;
 FUN_0047bf50((uint16_t)first,bx,by,1,1,(uint32_t)first_image,8);(void)FUN_00485100(I(first));
 sx=I((uint32_t)bx+(rotation?1u:0u));sy=I((uint32_t)by+(rotation?0u:1u));
 second=(uint32_t)restore_building_allocate_0046a400(U(0xeaac28u),30,sx,sy,0,0);r2=second*264u;
 FUN_0047bf50((uint16_t)second,sx,sy,1,1,(uint32_t)second_image,8);
 U(0x799ab0u+r2)=0;U(0x799aa8u+r2)=cell;
 U(0x799aacu+r1)=rotation;U(0x799aacu+rb)=rotation;U(0x799aacu+rp)=rotation;(void)FUN_00485100(I(second));
 W(0x799a12u+rp)=(uint16_t)booth;W(0x799a10u+rb)=(uint16_t)pavilion;
 W(0x799a12u+rb)=(uint16_t)first;W(0x799a10u+r1)=(uint16_t)booth;
 W(0x799a12u+r1)=(uint16_t)second;W(0x799a10u+r2)=(uint16_t)first;
 FUN_00440060(cell,33,(uint16_t)pavilion);return 1;
}
uint32_t restore_build_festival_square(uint32_t preview,int32_t x,int32_t y)
{
 uint32_t city=U(0xeaac28u)*0x49d8u,width,height,cell,id,record;
 if(U(0xea6010u+city)!=UINT32_MAX&&U(0xea6014u+city)!=UINT32_MAX)return 0;
 if(!FUN_0043f410(209))return 0;
 if(preview)return 1;
 restore_entertainment_preview_restore();
 width=U(0xbca2d0u);if(x>=I(width))x=I(width-1u);
 height=U(0xbca2d4u);if(y>=I(height))y=I(height-1u);
 cell=U(0xbca2d8u)+(uint32_t)y*228u;U(0xbc971cu)=1;city=U(0xeaac28u)*0x49d8u;
 U(0xea6010u+city)=(uint32_t)x;cell+=(uint32_t)x;U(0xea6014u+city)=(uint32_t)y;
 FUN_004413b0(x,y,209);
 id=(uint32_t)restore_building_allocate_0046a400(U(0xeaac28u),209,x,y,0,0);record=id*264u;
 (void)FUN_00485100(I(id));U(0x799aa8u+record)=cell;U(0x799ab0u+record)=1;
 FUN_00440060(cell,209,(uint16_t)id);W(0xea6084u+U(0xeaac28u)*0x49d8u)=1;return 1;
}
/* Allocation failure is deliberately not short-circuited: the native
   constructors continue with record0, stamps and links in their same order. */
#define ENTRY(name,provider) uint32_t name(uint32_t preview,uint32_t a,uint32_t b,int32_t x,int32_t y){(void)a;(void)b;return provider(preview,x,y);}
ENTRY(FUN_00440700,restore_build_booth)
ENTRY(FUN_00440850,restore_build_bandstand)
ENTRY(FUN_00440b10,restore_build_pavilion)
ENTRY(FUN_00440ed0,restore_build_festival_square)
