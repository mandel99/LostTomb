#include "monument_complex_create.h"
#include "building_create_model.h"
#include "building_allocator.h"
#include "building_support.h"
#include "editor_brush_validation.h"
#include <stdlib.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_004dea50(void);

struct complex_layout {uint32_t target,count,kind,geometry;};
static const struct complex_layout layouts[4]={
 {0x545c30,167,229,0x1e5c},{0x559130,158,234,0x1cbc},
 {0x556480,437,235,0x4ee0},{0x54fba0,513,236,0x5c88}
};
static void copy_words(uint32_t destination,uint32_t source,uint32_t count)
{uint32_t i;for(i=0;i<count;++i)U(destination+i*4u)=U(source+i*4u);}
static void draw_part(struct restore_menu_input_frame *parent,uint32_t self,uint32_t building)
{
 uint32_t arena[256],sp,i,target=U(U(self)+0x18u);struct restore_menu_input_frame child;
 if(parent){child=*parent;sp=parent->r[4]-64u;}
 else{for(i=0;i<8u;++i)child.r[i]=0;child.flags=2;child.boundary=restore_building_create_dispatch;sp=(uint32_t)(uintptr_t)(arena+224);}
 child.r[1]=self;child.r[4]=sp;U(sp)=0;U(sp+4u)=building;
 child.boundary(0,target,&child);if(child.r[4]!=sp+8u)abort();
}
static void create(uint32_t target,uint32_t self,uint32_t x,uint32_t y,uint32_t root,uint8_t skip,struct restore_menu_input_frame *context)
{
 const struct complex_layout *layout=0;uint32_t n,geometry,one,six,three,a,b,index,i,r,part,previous=root,registration=0,dx,dy;
 for(i=0;i<4u;++i)if(layouts[i].target==target){layout=layouts+i;break;}
 if(!layout)abort();n=layout->count;geometry=self+layout->geometry;
 /* Four adjacent streams of DWORDs, then two packed BYTE streams. */
 one=self+0x58u+n*4u;six=self+0x58u+n*8u;three=self+0x58u+n*32u;
 a=self+0x58u+n*44u;b=a+n;
 if(!skip)registration=(uint32_t)FUN_004f4f80((int32_t)root);
 dx=U(geometry);dy=U(geometry+4u);B(self+0x4cu)=0;U(self+0x54u)=1;r=root*264u;
 W(0x7999deu+r)=(uint16_t)(x+dx);W(0x7999e0u+r)=(uint16_t)(y+dy);
 U(0x7999e4u+r)=U(0xbca2d8u)+(y+dy)*228u+dx+x;
 W(0x7999eau+r)=W(geometry+12u);B(0x7999dbu+r)=B(geometry+8u);
 W(0x799a10u+r)=0;U(0x799a2cu+r)=registration;
 U(0x799a30u+r)=0;U(0x799a38u+r)=1;U(0x799a34u+r)=0;
 if(target==0x545c30u||target==0x54fba0u){U(0x799a3cu+r)=1;B(0x799a74u+r)=B(b);}
 else{B(0x799a74u+r)=B(b);U(0x799a3cu+r)=1;}
 U(0x799a68u+r)=U(self+0x58u);U(0x799a40u+r)=U(one);
 B(0x799a6cu+r)=B(a);U(0x799a70u+r)=0;
 copy_words(0x799a44u+r,six,6);copy_words(0x799a5cu+r,three,3);
 (void)FUN_00485100((int32_t)root);U(self+8u)=0;
 for(index=1;index<n;++index){
  part=geometry+index*52u;dy=U(part+4u);dx=U(part);
  i=(uint32_t)restore_building_allocate_0046a400(U(0xeaac28u),layout->kind,(int32_t)(x+dx),(int32_t)(y+dy),0,(int32_t)(previous+1u));
  r=i*264u;W(0x799a10u+r)=(uint16_t)previous;W(0x799a12u+previous*264u)=(uint16_t)i;previous=i;
  W(0x799a12u+r)=0;W(0x7999eau+r)=W(part+12u);B(0x7999dbu+r)=B(part+8u);
  U(0x799a2cu+r)=registration;U(0x799a30u+r)=0;U(0x799a38u+r)=0;U(0x799a34u+r)=0;
  /* The original members place their packed-byte stores at different points. */
  if(target==0x559130u)B(0x799a74u+r)=B(b+index);
  U(0x799a68u+r)=U(self+0x58u+index*4u);U(0x799a3cu+r)=0;
  if(target==0x54fba0u)B(0x799a74u+r)=B(b+index);
  U(0x799a40u+r)=U(one+index*4u);
  if(target==0x556480u)B(0x799a74u+r)=B(b+index);
  B(0x799a6cu+r)=B(a+index);U(0x799a70u+r)=0;
  if(target==0x545c30u)B(0x799a74u+r)=B(b+index);
  copy_words(0x799a44u+r,six+index*24u,6);copy_words(0x799a5cu+r,three+index*12u,3);
  (void)FUN_00485100((int32_t)i);
 }
 /* Native code redraws root+ordinal, rather than following allocation links. */
 for(index=0;index<n;++index)draw_part(context,self,root+index);
 FUN_004dea50();
}
void restore_complex_create_frame(uint32_t target,struct restore_menu_input_frame *f)
{
 uint32_t sp=f->r[4];create(target,f->r[1],U(sp+4u),U(sp+8u),U(sp+12u),(uint8_t)U(sp+16u),f);f->r[4]=sp+20u;
}
#define ENTRY(token) \
void restore_complex_create_##token(uint32_t self,int32_t x,int32_t y,uint32_t root,uint8_t skip) \
{create(0x##token##u,self,(uint32_t)x,(uint32_t)y,root,skip,0);}
ENTRY(00545c30)
ENTRY(00559130)
ENTRY(00556480)
ENTRY(0054fba0)
