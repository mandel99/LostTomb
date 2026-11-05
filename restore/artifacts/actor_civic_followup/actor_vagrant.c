#include <stdint.h>
#define VU(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define VW(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define VB(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define VH(a) ((int32_t)(int16_t)VW(a))
extern void FUN_004b17d0(int32_t),FUN_004b1d50(int32_t),FUN_004b3250(int32_t);
extern void FUN_004a5da0(int32_t,uint32_t);
extern int32_t FUN_00458c30(int32_t,int32_t),FUN_004b3ff0(int32_t,int32_t);
extern uint32_t FUN_0047fe60(int32_t,int32_t,uint32_t,int32_t);
extern void FUN_004b4220(int32_t,int32_t,int32_t,int32_t,int32_t,int32_t);
extern void FUN_0046cea0(int32_t,int16_t),FUN_004e91e0(int32_t,int32_t);
/* Native47A1C0 consumes only the first of the two caller slots. The
   otherwise unused Y load is retained at each original call below. */
extern uint32_t FUN_0047a1c0(int32_t);
static uint32_t v_actor(uint32_t id){return 0xbcd5f8u+id*388u;}
static uint32_t v_building(int32_t id){return 0x7999d8u+(uint32_t)id*264u;}

static uint32_t vagrant_render(uint32_t id,uint8_t special)
{
 uint32_t a=v_actor(id),rotation=VU(0xbca2e0u),direction,base,image;int32_t half,age,x,y;uint8_t frame,water;
 direction=(uint32_t)(int32_t)(int8_t)VB(a+0x11u)-rotation;VU(0xe94e00u)=direction;
 if((int32_t)direction<0){direction+=8u;VU(0xe94e00u)=direction;}
 if(VB(a+0x38u)==4u){
  half=VH(a+0x36u)/2;base=VU(0xf55d54u);frame=VB(0x5d048cu+(uint32_t)half);VU(a+4u)=base+frame;
  if(!special)return (uint32_t)half&UINT32_C(0xffffff00);
  age=VH(a+0x36u);
  if(age>=33){age-=33;if(age>=23)age=22;base=VU(0xf56e14u);image=base+(uint32_t)age;
   VU(a+4u)=image;VU(a+0x180u)=0;return image;}
  y=VH(a+0x16u);x=VH(a+0x14u);(void)y;water=(uint8_t)FUN_0047a1c0(x);
  id=VU(0xd056f8u);a=v_actor(id);age=VH(a+0x36u)/3;direction=VU(0xe94e00u);
  base=VU(water?0xf571c4u:0xf56144u);image=base+(uint32_t)age+(uint32_t)((int32_t)direction/8);
  VU(a+4u)=image;VU(a+0x180u)=0;return image;
 }
 if(!special){base=VU(0xf55d4cu);frame=VB(a+1u);VU(a+4u)=base+(uint32_t)frame*8u+direction;return direction;}
 y=VH(a+0x16u);x=VH(a+0x14u);(void)y;water=(uint8_t)FUN_0047a1c0(x);
 id=VU(0xd056f8u);a=v_actor(id);frame=VB(a+1u);base=VU(water?0xf571bcu:0xf5613cu);
 image=base+((uint32_t)frame/3u)*8u;direction=VU(0xe94e00u);image+=direction;
 VU(a+4u)=image;VU(a+0x180u)=0;return direction;
}

/* Complete direct CFG of48A720..48AE4F, registry kind3 FIG_VAGRANT.
   Fresh bytes decode to491 reachable instructions /1840 bytes; the older
   unauthenticated provider summary reported a shorter1823-byte extent. */
__declspec(noinline) uint32_t restore_actor_update_vagrant(void)
{
 uint32_t id=VU(0xd056f8u),a=v_actor(id),kind=VB(a+0x0au),tile=VU(a+0x20u),speed,terrain,b;
 uint8_t special=1,frame,state,direction,size;int32_t target,x,y,from_x,from_y,to_x,to_y;uint32_t found;uint16_t target_x,target_y;
 speed=VU(0x7090ecu+kind*56u);terrain=VU(0xaf598cu+tile*4u)&4u;
 if(terrain){if(VB(0xadc36cu+tile))special=0;}else special=0;
 frame=(uint8_t)(VB(a+1u)+1u);VB(a+1u)=frame;if(frame>=12u)VB(a+1u)=0;VB(a+0x72u)=3;state=VB(a+0x38u);
 switch(state){
 case 5:FUN_004b17d0((int32_t)id);id=VU(0xd056f8u);break;
 case 4:FUN_004b1d50((int32_t)id);id=VU(0xd056f8u);break;
 case 8:
  VB(a+1u)=0;VW(a+0x36u)=(uint16_t)(VW(a+0x36u)+1u);
  if(VH(a+0x36u)>51){
   y=VH(a+0x16u);x=VH(a+0x14u);target=FUN_00458c30(x,y);
   if(!target){uint32_t player;id=VU(0xd056f8u);player=VU(0xeaac28u)*0x49d8u;a=v_actor(id);
    target_x=VW(0xea3ff0u+player);target_y=VW(0xea3ff2u+player);VB(a+0x38u)=11;
    VW(a+0x24u)=target_x;VW(a+0x26u)=target_y;VW(a+0x44u)=0;VW(a+0x36u)=0;
   }else{
    b=v_building(target);y=VH(b+8u);size=VB(b+3u);x=VH(b+6u);found=FUN_0047fe60(x,y,size,2);
    id=VU(0xd056f8u);a=v_actor(id);
    if(!found)VB(a+0x0eu)=2;
    else{target_x=VW(0xd3b4e4u);VW(b+0x2eu)=(uint16_t)id;VW(a+0x5cu)=(uint16_t)target;VB(a+0x38u)=9;
     VW(a+0x24u)=target_x;VW(a+0x26u)=VW(0xd3b4e0u);VW(a+0x44u)=0;}
   }
  }break;
 case 9:
  VB(a+0x65u)=0;FUN_004a5da0((int32_t)id,speed);id=VU(0xd056f8u);a=v_actor(id);
  direction=VB(a+0x11u);target=VH(a+0x5cu);
  if(direction==9u||direction==10u){VB(a+0x0eu)=2;VW(v_building(target)+0x2eu)=0;}
  else if(direction==8u){VB(a+0x38u)=10;b=v_building(target);target_x=VW(b+6u);target_y=VW(b+8u);
   VW(a+0x24u)=target_x;VW(a+0x26u)=target_y;to_y=(int32_t)((uint32_t)(int32_t)(int16_t)target_y*15u);
   to_x=(int32_t)((uint32_t)VH(a+0x24u)*15u);from_y=VH(a+0x4cu);from_x=VH(a+0x4au);
   FUN_004b4220((int32_t)id,from_x,from_y,to_x,to_y,0);id=VU(0xd056f8u);VW(v_actor(id)+0x44u)=0;
  }break;
 case 11:
  FUN_004a5da0((int32_t)id,speed);id=VU(0xd056f8u);a=v_actor(id);direction=VB(a+0x11u);
  if(direction==8u||direction==10u)VB(a+0x0eu)=2;
  else if(direction==9u){FUN_004b3250((int32_t)id);id=VU(0xd056f8u);}
  a=v_actor(id);VW(a+0x36u)=(uint16_t)(VW(a+0x36u)+1u);
  if(VH(a+0x36u)>30){
   VW(a+0x36u)=0;y=VH(a+0x16u);x=VH(a+0x14u);target=FUN_00458c30(x,y);
   if(target<=0)id=VU(0xd056f8u);
   else{b=v_building(target);y=VH(b+8u);size=VB(b+3u);x=VH(b+6u);found=FUN_0047fe60(x,y,size,2);
    if(!found)id=VU(0xd056f8u);
    else{id=VU(0xd056f8u);target_x=VW(0xd3b4e4u);VW(b+0x2eu)=(uint16_t)id;a=v_actor(id);
     VW(a+0x5cu)=(uint16_t)target;VB(a+0x38u)=9;VW(a+0x24u)=target_x;VW(a+0x26u)=VW(0xd3b4e0u);VW(a+0x44u)=0;
     FUN_004b3250((int32_t)id);id=VU(0xd056f8u);}
   }
  }break;
 case 10:
  target=VH(a+0x5cu);VB(a+0x0cu)=1;VB(a+0x65u)=1;
  if(FUN_004b3ff0((int32_t)id,1)!=1)id=VU(0xd056f8u);
  else{
   int32_t building_kind,subtype;uint32_t capacity,available;uint8_t incoming;
   id=VU(0xd056f8u);a=v_actor(id);VB(a+0x0eu)=2;
   if(!target)break;b=v_building(target);building_kind=VH(b+0x10u);
   if(building_kind<10||building_kind>29)break;if(VB(b+0x87u))break;
   subtype=VH(b+0x12u);capacity=VU(0x70ce14u+(uint32_t)subtype*88u);if(VB(b+4u))capacity*=4u;
   available=capacity-(uint32_t)VH(b+0x1cu);if((int32_t)available<0)available=0;
   incoming=VB(a+0x64u);if((int32_t)available<(int32_t)incoming)VB(a+0x64u)=(uint8_t)available;
   if(VW(b+0x1cu)==0u){FUN_0046cea0(target,10);id=VU(0xd056f8u);}
   a=v_actor(id);incoming=VB(a+0x64u);VW(b+0x1cu)=(uint16_t)(VW(b+0x1cu)+incoming);
   target_x=VW(b+0x1cu);kind=VU(0xeaac28u);VW(b+0x1eu)=(uint16_t)(capacity-target_x);
   FUN_004e91e0((int32_t)kind,incoming);id=VU(0xd056f8u);VW(b+0x2eu)=0;
  }break;
 default:VB(a+0x38u)=8;break;
 }
 return vagrant_render(id,special);
}
