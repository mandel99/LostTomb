#include "monument_clear_model.h"
#include "menu_prepare_integer.h"
#include "terrain_redraw.h"
#include "clear_helpers.h"
extern void FUN_0047a220(int32_t,int32_t,int32_t);
extern void FUN_0047b590(int32_t,int32_t,int32_t,int32_t);
extern void FUN_00477500(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004777c0(int32_t,int32_t,int32_t,int32_t);
extern void FUN_00475cc0(int32_t,int32_t,int32_t,int32_t,uint32_t);
extern void FUN_004776f0(int32_t,int32_t,int32_t,int32_t);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define I(a) ((int32_t)(uint32_t)(a))

void restore_temple_complex_clear(int32_t building)
{
 uint32_t record=(uint32_t)building*264u,view=B(0x799a82u+record),offset=(view>>1)*8u;
 uint32_t x=(uint32_t)H(0x7999deu+record)+U(0x5e00ccu+offset),y=(uint32_t)H(0x7999e0u+record)+U(0x5e00d0u+offset);
 uint32_t row,col,px,py,end_x,end_y,cell,flags,sprite;
 int32_t owner;
 if(view==0){end_x=x+13u;end_y=y+7u;}
 else if(view==2){end_x=x-7u;end_y=y-13u;}
 else if(view==4){end_x=x-13u;end_y=y-7u;}
 else{end_x=x+7u;end_y=y+13u;}
 /* The native shape is seven rows of thirteen 20-byte table records.
    Only the first DWORD is the clearing mask; orientation is cached. */
 for(row=0;row<7u;++row)for(col=0;col<13u;++col){
  if(view==0){px=x+col;py=y+row;}
  else if(view==2){px=x-row;py=y-col;}
  else if(view==4){px=x-col;py=y-row;}
  else{px=x+row;py=y+col;}
  cell=U(0xbca2d8u)+py*228u+px;
  if(!U(0x5df9b0u+(row*13u+col)*20u))continue;
  if(building){
   owner=H(0xb4e6fcu+cell*2u);
   if(owner!=building&&(int32_t)FUN_00475b50((uint32_t)owner)!=building)continue;
   if(W(0x7999e8u+record)!=99u)B(0xa9010cu+cell)=B(0x7999e8u+record);
  }
  B(0xb285ccu+cell)=B(0xb285ccu+cell)&0xe0u;sprite=B(0xb67d1cu+cell)&0xc0u;
  W(0xb4e6fcu+cell*2u)=0;W(0xa9cc1cu+cell*2u)=0;B(0xb67d1cu+cell)=(uint8_t)(sprite|0x40u);
  flags=U(0xaf598cu+cell*4u)&0xfffffff7u;B(0xadc36cu+cell)=0;U(0xaf598cu+cell*4u)=flags;B(0xae8e7cu+cell)=0;
  if((flags&4u)&&!(flags&0x10000u)){
   U(0xaf598cu+cell*4u)=flags&4u;FUN_0047a220(I(px),I(py),1);
  }else{
   sprite=B(0xacf85cu+cell)&7u;U(0xb7482cu+cell*4u)=sprite+U(0xf56b54u);U(0xaf598cu+cell*4u)=flags&0x03052e84u;
  }
 }
 FUN_0047b590(I(x),I(y),I(end_x),I(end_y));FUN_004767d0(I(x),I(y),I(end_x),I(end_y),0,0);
 FUN_00477500(I(x),I(y),I(end_x),I(end_y));FUN_004777c0(I(x),I(y),I(end_x),I(end_y));
 FUN_00475cc0(I(x),I(y),I(end_x),I(end_y),1);FUN_004776f0(I(x),I(y),I(end_x),I(end_y));
 FUN_00477890(I(x),I(y),I(end_x),I(end_y));B(0xe38dcau)=1;
}
void FUN_004c0920(int32_t building){restore_temple_complex_clear(building);}
