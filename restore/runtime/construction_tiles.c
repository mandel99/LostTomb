#include "construction_tiles.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
void FUN_00440060(uint32_t cell,int32_t kind,uint16_t building)
{
 uint32_t size=1,row,col,at,image,base,flags,access,orientation,table,index,sprite;
 if(kind==31)size=2;else if(kind==30)size=3;else if(kind==33)size=4;else if(kind==209)size=5;
 for(row=0;row<size;++row)for(col=0;col<size;++col){
  at=cell+row*228u+col;
  if(kind!=209){
   image=U(0xb7482cu+at*4u);
   if(image==U(0xf567acu))continue;
   base=U(0xf566fcu);if(image==base)continue;if(image==base+1u)continue;
   if(image==U(0xf566fcu)+2u)continue;
   if(image==U(0xf566fcu)+3u)continue;
   if(image==U(0xf5659cu))continue;
  }
  flags=U(0xaf598cu+at*4u)&0x03052ec0u;W(0xb4e6fcu+at*2u)=building;
  access=B(0xb285ccu+at);flags|=8u;access&=0x60u;
  U(0xaf598cu+at*4u)=flags;B(0xb285ccu+at)=(uint8_t)access;
  if(!(flags&0x40u)&&kind!=209){
   base=U(0xf565f4u);flags|=0x20u;U(0xaf598cu+at*4u)=flags;
   image=B(0xacf85cu+at)&3u;U(0xb7482cu+at*4u)=base+image;
  }else{
   switch(kind){
   case 31:
    orientation=U(0xbca2e0u);index=row*2u+col;
    if(orientation==0u){base=U(0xf5679cu);image=U(0x5d2078u+index*4u);}
    else if(orientation==2u){base=U(0xf5679cu);image=U(0x5d2088u+index*4u);}
    else if(orientation==4u){image=U(0x5d2098u+index*4u);base=U(0xf5679cu);}
    else{image=U(0x5d20a8u+index*4u);base=U(0xf5679cu);}
    U(0xb7482cu+at*4u)=base+image;break;
   case 30:
    orientation=U(0xbca2e0u);index=row*3u+col;
    table=orientation==0u?0x5d20b8u:orientation==2u?0x5d20dcu:orientation==4u?0x5d2100u:0x5d2124u;
    base=U(0xf565ecu);image=U(table+index*4u);U(0xb7482cu+at*4u)=base+image;break;
   case 33:
    orientation=U(0xbca2e0u);index=row*4u+col;
    if(orientation==0u){base=U(0xf565acu);image=U(0x5d2148u+index*4u);}
    else if(orientation==2u){base=U(0xf565acu);image=U(0x5d2188u+index*4u);}
    else if(orientation==4u){image=U(0x5d21c8u+index*4u);base=U(0xf565acu);}
    else{image=U(0x5d2208u+index*4u);base=U(0xf565acu);}
    U(0xb7482cu+at*4u)=base+image;break;
   case 209:
    orientation=U(0xbca2e0u);index=row*5u+col;
    table=orientation==0u?0x5d2248u:orientation==2u?0x5d22acu:orientation==4u?0x5d2310u:0x5d2374u;
    base=U(0xf565a4u);image=U(table+index*4u);U(0xb7482cu+at*4u)=base+image;break;
   default:image=U(0xf5695cu);U(0xb7482cu+at*4u)=image;break;
   }
   B(0xee0c50u+at)=0;
  }
  sprite=B(0xb67d1cu+at);B(0xb67d1cu+at)=(uint8_t)(sprite|0x40u);
 }
}
