#include "construction_controller.h"
#include "construction_tiles.h"
#include "bridge_place.h"
#include "displaced_population.h"
#include "construction_lines.h"
#include "construction_refresh.h"
#include "construction_brush.h"
#include "building_reservation.h"
extern void FUN_0040f860(void *,void *,int32_t);
extern void FUN_004264d0(uint32_t,uint32_t,uint32_t);
extern uint32_t FUN_00474380(int32_t);
extern void FUN_00475720(int32_t,int32_t,int32_t,int32_t,uint32_t);
extern int32_t FUN_00475b50(int32_t);
extern void FUN_00475cc0(int32_t,int32_t,int32_t,int32_t,uint32_t);
extern void FUN_00476530(void);
extern void FUN_004767d0(int32_t,int32_t,int32_t,int32_t,uint32_t,uint32_t);
extern void FUN_00477500(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004776f0(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004777c0(int32_t,int32_t,int32_t,int32_t);
extern void FUN_00477890(int32_t,int32_t,int32_t,int32_t);
extern void FUN_0047b590(int32_t,int32_t,int32_t,int32_t);
extern void FUN_0047bae0(int32_t,int32_t,int32_t,int32_t,uint32_t);
extern void FUN_0047bd50(int32_t,int32_t,int32_t,int32_t);
extern uint32_t FUN_00485100(int32_t);
extern void FUN_00485830(void);
extern void FUN_004e2970(int32_t);
extern void FUN_00517370(void);
extern void FUN_00517660(void);
extern void FUN_005178b0(void);
extern void FUN_005178e0(void);
extern void FUN_0053b090(int32_t);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define I(a) ((int32_t)(uint32_t)(a))
static int class_confirmation(int32_t kind)
{
 switch(kind){case 65:case 66:case 67:case 68:case 69:case 97:case 156:case 157:case 158:case 159:case 160:return 1;default:return 0;}
}
static int preserved_kind(int32_t kind)
{
 switch(kind){case 99:case 88:case 89:case 93:case 218:case 219:case 220:case 215:return 1;default:return 0;}
}
static void mark_chain(uint32_t node,uint32_t link)
{
 uint32_t steps;
 for(steps=0;steps<525u;++steps){
  node=(uint32_t)H(link+node*264u);if(I(node)<=0)break;
  (void)FUN_00485100(I(node));B(0x7999d8u+node*264u)=6;B(0x799a97u+node*264u)=1;
 }
}

void restore_demolition_apply(int32_t preview,int32_t x1,int32_t y1,int32_t x2,int32_t y2)
{
 int32_t left=x1<x2?x1:x2,right=x1<x2?x2:x1,top=y1<y2?y1:y2,bottom=y1<y2?y2:y1,kind,people,hx,hy,linked;
 uint32_t width,height,span,cell,flags,masked,id,record,value,other,owner,radius=0,next_x,next_y,index,args[3];
 uint8_t access,sprite,north;
 U(0xbc971cu)=0;FUN_00485080();
 FUN_0040f860((void *)0x9de62cu,(void *)0xaf598cu,0x32c40);
 FUN_0040f860((void *)0x9d1b1cu,(void *)0xae8e7cu,0xcb10);FUN_00485830();
 if(left<0)left=0;
 value=U(0xbca2d0u);if(right>=I(value))right=I(value-1u);
 if(top<0)top=0;
 value=U(0xbca2d4u);if(bottom>=I(value))bottom=I(value-1u);
 width=(uint32_t)right-(uint32_t)left+1u;height=(uint32_t)bottom-(uint32_t)top+1u;
 cell=U(0xbca2d8u)+(uint32_t)top*228u+(uint32_t)left;
 U(0xbc9768u)=cell;U(0xbc9760u)=(uint32_t)top;
 if(top<=bottom)do{
  U(0xbc9764u)=(uint32_t)left;
  if(left<=right)do{
   flags=U(0xaf598cu+cell*4u);masked=flags&0xeefed77fu;
   if(masked&0x02040202u)goto next_cell;
   id=(uint32_t)H(0xb4e6fcu+cell*2u);
   if((masked&8u)||(masked&0x40000000u))goto building;
   if(id&&(flags&0x10000000u)){
    kind=H(0x7999e8u+id*264u);if(kind==225||kind==213)goto building;
   }
   if(masked&0x100u){
    value=U(0xbc971cu);U(0xaf598cu+cell*4u)=flags&0x03052e80u;B(0xae8e7cu+cell)=0;
    north=B(0xae8d98u+cell);U(0xbc971cu)=value+1u;
    if(north==5u)B(0xae8d98u+cell)=1;
    if(B(0xae8e7du+cell)==6u)B(0xae8e7du+cell)=2;
    if(B(0xae8f60u+cell)==5u)B(0xae8f60u+cell)=3;
    if(B(0xae8e7bu+cell)==6u)B(0xae8e7bu+cell)=4;
   }else if(masked&4u){
    if(!preview){
     value=FUN_0047f9b0(cell);
     if(I(value)>=1){FUN_004e2970(55);goto reload_next;}
     cell=U(0xbc9768u);
    }
    if(U(0xbc96b0u)==1u){FUN_0047f810(cell,(uint32_t)preview);U(0xbc971cu)=U(0xbc971cu)+1u;cell=U(0xbc9768u);}
   }else if(masked){
    sprite=B(0xb67d1cu+cell);U(0xaf598cu+cell*4u)=flags&0x03052e80u;access=B(0xb285ccu+cell)&0xf0u;sprite|=0x40u;
    B(0xb285ccu+cell)=access;B(0xb67d1cu+cell)=sprite;
    if(!preview){B(0xb285ccu+cell)=access&0x70u;B(0xb67d1cu+cell)=sprite|0x40u;}
    U(0xbc971cu)=U(0xbc971cu)+1u;
   }
   goto processed;
building:
   if(!id)goto processed;
   record=id*264u;kind=H(0x7999e8u+record);
   if(preserved_kind(kind)||B(0x7999d8u+record)==6u)goto next_cell;
   if((kind==57||kind==54)&&!preview&&U(0xbc96b4u)!=1u)goto next_cell;
   if((uint8_t)FUN_00474380(kind)&&!preview&&U(0xbc96acu)!=1u)goto reload_next;
   kind=H(0x7999e8u+record);
   if(class_confirmation(kind)&&!preview&&U(0xbc96a8u)!=1u)goto reload_next;
   if((kind==57||kind==54)&&!preview&&U(0xbc96b4u)==1u)U(0xbc9694u)=0;
   if(kind==183){FUN_004f4930(I(id));FUN_004f5e70(I(id),1);}
   value=B(0x799a99u+record);if(value)B(0x7900c0u+value*196u)=0;
   value=B(0x799aa3u+record);if(value)B(0x9783a4u+value*28u)=0;
   value=B(0x799aa2u+record);if(value)U(0xeaaf68u+value*224u)=0;
   if(B(0x7999ddu+record)&&W(0x7999f4u+record)&&!preview){
    hx=H(0x7999deu+record);people=H(0x7999f4u+record);hy=H(0x7999e0u+record);owner=U(0xeaac28u);
    FUN_00459020(I(owner),hx,hy,people);W(0x7999f4u+record)=0;
   }
   kind=H(0x7999e8u+record);if(kind==108||kind==195||kind==115)B(0x799a7eu+record)=0;
   if(B(0x7999d8u+record)!=6u){U(0xbc971cu)=U(0xbc971cu)+1u;(void)FUN_00485100(I(id));}
   B(0x7999d8u+record)=6;FUN_0053b090(2);B(0x799a97u+record)=1;
   mark_chain(id,0x799a10u);mark_chain(id,0x799a12u);cell=U(0xbc9768u);
processed:
   if(!preview&&(masked&0x148u))radius=5;
   goto next_cell;
reload_next:
   cell=U(0xbc9768u);
next_cell:
   next_x=U(0xbc9764u)+1u;++cell;U(0xbc9764u)=next_x;U(0xbc9768u)=cell;
  }while(I(next_x)<=right);
  next_y=U(0xbc9760u)+1u;U(0xbc9760u)=next_y;cell+=228u-width;next_y=U(0xbc9760u);U(0xbc9768u)=cell;
 }while(I(next_y)<=bottom);
 span=(I(width)>I(height)?width:height)+2u;
 FUN_004767d0(left,top,right,bottom,0,1);
 if(radius)FUN_004767d0(I((uint32_t)left-radius),I((uint32_t)top-radius),I((uint32_t)right+radius),I((uint32_t)bottom+radius),1,0);
 FUN_00477500(left,top,right,bottom);FUN_004776f0(left,top,right,bottom);FUN_004777c0(left,top,right,bottom);
 FUN_0047bae0(left,top,right,bottom,0);FUN_0047b480(left,top,I(span));FUN_00477890(left,top,right,bottom);FUN_00476530();
 height=U(0xbca2d4u)-1u;width=U(0xbca2d0u)-1u;FUN_00475720(0,0,I(width),I(height),1);
 for(index=1;index<4000u;++index){
  record=index*264u;value=B(0x7999d8u+record);if(!value||value==6u)continue;
  kind=H(0x7999e8u+record);
  if(kind!=31&&kind!=30&&kind!=33&&kind!=209)continue;
  if(kind==30){
   linked=H(0x799a12u+record);other=(uint32_t)linked*33u;
   if(H(0x7999e8u+(uint32_t)linked*264u)==30){
    args[0]=index;args[2]=(uint32_t)H(0x7999e0u+record);args[1]=(uint32_t)H(0x7999deu+record);
    /* Redraw the two Bandstand cells. Native ECX is next-link ID*33,
       retained as the fallback second image for an invalid view/direction. */
    (void)restore_bridge_00440540(args,other);
   }
  }
  if(FUN_00475b50(I(index))==I(index)&&U(0x799ab0u+record))FUN_00440060(U(0x799aa8u+record),kind,(uint16_t)index);
 }
 FUN_0047afd0(left,top,I(span));FUN_00475cc0(left,top,right,bottom,0);
 if(!preview){
  left=I((uint32_t)left-1u);top=I((uint32_t)top-1u);right=I((uint32_t)right+1u);bottom=I((uint32_t)bottom+1u);
  FUN_0047bd50(left,top,right,bottom);FUN_0047b590(left,top,right,bottom);
  FUN_005178b0();FUN_00517370();FUN_00517660();FUN_005178e0();
 }
}

void restore_demolition_request(int32_t preview,int32_t x1,int32_t y1,int32_t x2,int32_t y2)
{
 int32_t left=x1<x2?x1:x2,right=x1<x2?x2:x1,top=y1<y2?y1:y2,bottom=y1<y2?y2:y1,kind;
 uint32_t value,width,cell,id,record,water,marker,next_x,next_y;int sensitive=0,classes=0,pair=0,line=0;
 U(0xbc96b4u)=0;U(0xbc96b0u)=0;U(0xbc96acu)=0;U(0xbc96a8u)=0;
 if(preview){FUN_00474700(preview,x1,y1,x2,y2);return;}
 if(left<0)left=0;value=U(0xbca2d0u);if(right>=I(value))right=I(value-1u);
 if(top<0)top=0;value=U(0xbca2d4u);if(bottom>=I(value))bottom=I(value-1u);
 width=(uint32_t)right-(uint32_t)left+1u;cell=U(0xbca2d8u)+(uint32_t)top*228u+(uint32_t)left;
 U(0xbc9760u)=(uint32_t)top;U(0xbc9768u)=cell;
 if(top<=bottom)do{
  U(0xbc9764u)=(uint32_t)left;
  if(left<=right)do{
   id=(uint32_t)H(0xb4e6fcu+cell*2u);marker=B(0xadc36cu+cell);water=U(0xaf598cu+cell*4u)&4u;
   record=id*264u;
   if(id){kind=H(0x7999e8u+record);if(kind==57||kind==54)pair=1;}
   if(marker&&water&&!id)line=1;
   if((uint8_t)FUN_00474380(H(0x7999e8u+record)))sensitive=1;
   if(class_confirmation(H(0x7999e8u+record)))classes=1;
   next_x=U(0xbc9764u)+1u;++cell;U(0xbc9764u)=next_x;U(0xbc9768u)=cell;
  }while(I(next_x)<=right);
  next_y=U(0xbc9760u)+1u;U(0xbc9760u)=next_y;cell+=228u-width;next_y=U(0xbc9760u);U(0xbc9768u)=cell;
 }while(I(next_y)<=bottom);
 /* Callbacks consume original coordinates, not the normalized scan bounds. */
 U(0xbc96a4u)=(uint32_t)x1;U(0xbc96a0u)=(uint32_t)y1;U(0xbc969cu)=(uint32_t)x2;U(0xbc9698u)=(uint32_t)y2;
 if(sensitive)FUN_004264d0(104,0x4021d0u,2);
 else if(classes)FUN_004264d0(106,0x402210u,2);
 else if(pair)FUN_004264d0(16,0x402250u,2);
 else if(line)FUN_004264d0(20,0x402290u,2);
 else FUN_00474700(0,x1,y1,x2,y2);
}
void FUN_00474700(int32_t p,int32_t x1,int32_t y1,int32_t x2,int32_t y2){restore_demolition_apply(p,x1,y1,x2,y2);}
void FUN_004743f0(int32_t p,int32_t x1,int32_t y1,int32_t x2,int32_t y2){restore_demolition_request(p,x1,y1,x2,y2);}
