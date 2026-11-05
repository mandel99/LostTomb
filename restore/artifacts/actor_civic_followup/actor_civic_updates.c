#include <stdint.h>
#define CU(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define CW(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define CB(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define CH(a) ((int32_t)(int16_t)CW(a))
extern void restore_actor_update_service_state(void);
extern int32_t FUN_004b3ff0(int32_t actor,int32_t mode);
extern void FUN_004b17d0(int32_t actor),FUN_004b1d50(int32_t actor),FUN_004b3250(int32_t actor);
extern void FUN_004a5da0(int32_t actor,uint32_t speed);
extern int32_t FUN_004450c0(int32_t actor,uint32_t x,uint32_t y,int32_t trader,uint32_t region);
extern int32_t FUN_0045cf50(int32_t actor,int32_t player,int32_t building,int32_t trader);
extern int32_t FUN_0045d180(int32_t actor,int32_t player,int32_t building,int32_t trader);
extern int32_t FUN_0045d010(int32_t player,int32_t building,uint32_t trader);
extern int32_t FUN_0045d370(int32_t player,int32_t building,uint32_t trader);
extern void FUN_00444bd0(int32_t actor,int32_t resource),FUN_00444b80(int32_t actor,int32_t resource);
extern uint32_t FUN_004a35e0(int32_t actor);
static uint32_t cv_actor(uint32_t id){return 0xbcd5f8u+id*388u;}
static uint32_t cv_building(int32_t id){return 0x7999d8u+(uint32_t)id*264u;}

/* 48BFF0..48C0DF. Registry kind5, FIG_GHOST. The native extra stack
   argument1 to493610 is not read by that void state handler. */
__declspec(noinline) uint32_t restore_actor_update_ghost(void)
{
 uint32_t id=CU(0xd056f8u),a=cv_actor(id),b,rotation,direction,base;int32_t home;
 uint8_t frame;int32_t half;
 CB(a+0x72u)=1;CB(a+0x0cu)=0;CW(a+0x42u)=0x180;
 home=CH(a+0x5au);b=cv_building(home);
 if(CB(b)!=1u || (uint32_t)CH(b+0x2cu)!=id)CB(a+0x0eu)=2;
 frame=(uint8_t)(CB(a+1u)+1u);CB(a+1u)=frame;if(frame>=12u)CB(a+1u)=0;
 restore_actor_update_service_state();id=CU(0xd056f8u);rotation=CU(0xbca2e0u);a=cv_actor(id);
 direction=(uint32_t)(int32_t)(int8_t)CB(a+0x11u)-rotation;CU(0xe94e00u)=direction;
 if((int32_t)direction<0){direction+=8u;CU(0xe94e00u)=direction;}
 if(CB(a+0x38u)==4u){half=CH(a+0x36u)/2;frame=CB(0x5d048cu+(uint32_t)half);
  base=CU(0xf56364u);CU(a+4u)=base+frame;return base;}
 base=CU(0xf5635cu);frame=CB(a+1u);CU(a+4u)=base+(uint32_t)frame*8u+direction;return direction;
}

/* 48C200..48C2A9. Registry kind6, FIG_EXPLOSION. Signed BYTE age
   wraps before its signed lifetime comparison and signed frame division. */
__declspec(noinline) uint32_t restore_actor_update_explosion(void)
{
 uint32_t id=CU(0xd056f8u),a=cv_actor(id),animation,steps,divisor,result;uint8_t age,direction;
 CB(a+0x0cu)=1;
 if(CU(a+0x15cu)){animation=0x24du;steps=20;divisor=(id&1u)+1u;}
 else{animation=0x246u;steps=15;divisor=3;}
 age=(uint8_t)(CB(a+0x39u)+1u);CB(a+0x39u)=age;
 if((int32_t)(int8_t)age>=(int32_t)(steps*divisor))CB(a+0x0eu)=2;
 direction=CB(a+0x59u);(void)FUN_004b3ff0((int32_t)id,direction);
 id=CU(0xd056f8u);a=cv_actor(id);result=(uint32_t)((int32_t)(int8_t)CB(a+0x39u)/(int32_t)divisor);
 result+=CU(0xf55bb4u+animation*8u);CU(a+4u)=result;return result;
}

static void cv_caravan_exit_destination(uint32_t a)
{
 uint32_t player=CU(0xeaac28u)*0x49d8u;uint16_t x=CW(0xea3ff0u+player),y;
 y=CW(0xea3ff2u+player);CB(a+0x38u)=11;CW(a+0x36u)=0;CW(a+0x24u)=x;CW(a+0x26u)=y;
}
static void cv_caravan_count_trade(uint32_t trader,uint32_t resource)
{
 uint32_t region=CB(0x77de04u+trader*106u),at=0x77592cu+(resource+region*36u)*4u;
 CU(at)=CU(at)+1u;
}
static uint32_t cv_caravan_render(uint32_t id)
{
 uint32_t a=cv_actor(id),direction,base;int32_t facing=(int8_t)CB(a+0x11u);uint8_t frame;
 if(facing>=8)facing=(int8_t)CB(a+0x12u);
 direction=(uint32_t)facing-CU(0xbca2e0u);CU(0xe94e00u)=direction;
 if((int32_t)direction<0){direction+=8u;CU(0xe94e00u)=direction;}
 if(CB(a+0x38u)==4u){int32_t half=CH(a+0x36u)/2;frame=CB(0x5d048cu+(uint32_t)half);
  base=CU(0xf56e64u);CU(a+4u)=base+frame;return base;}
 base=CU(0xf56e5cu);frame=CB(a+1u);CU(a+4u)=base+(uint32_t)frame*8u+direction;return direction;
}

/* 491420..491A87. Registry kind19, FIG_CARAVAN. The kind21
   FIG_VAN_FOLLOWER callback is a different, already reconstructed owner. */
__declspec(noinline) uint32_t restore_actor_update_trade_caravan(void)
{
 uint32_t id=CU(0xd056f8u),a=cv_actor(id),kind=CB(a+0x0au),speed,b;
 uint8_t frame,state,direction,trader,player;int32_t target,x,y,resource;uint16_t target_x;
 CB(a+0x65u)=0;CB(a+0x72u)=3;frame=CB(a+1u);frame=(uint8_t)(frame+1u);CB(a+1u)=frame;
 speed=CU(0x7090ecu+kind*56u);if(frame>=12u)CB(a+1u)=0;CU(a+0x180u)=0;state=CB(a+0x38u);
 switch(state){
 case 5:FUN_004b17d0((int32_t)id);id=CU(0xd056f8u);break;
 case 4:FUN_004b1d50((int32_t)id);id=CU(0xd056f8u);break;
 case 8:
  CW(a+0x36u)=(uint16_t)(CW(a+0x36u)+1u);
  if(CH(a+0x36u)>20){
   y=CH(a+0x16u);x=CH(a+0x14u);trader=CB(a+0x6eu);
   target=FUN_004450c0((int32_t)id,(uint32_t)x,(uint32_t)y,trader,0);
   id=CU(0xd056f8u);a=cv_actor(id);
   if(target){b=cv_building(target);CW(a+0x5eu)=(uint16_t)target;CB(a+0x38u)=9;
    target_x=CW(b+0x26u);CW(a+0x36u)=0;CW(a+0x24u)=target_x;CW(a+0x26u)=CW(b+0x28u);}
   else cv_caravan_exit_destination(a);
   CB(a+1u)=0;
  }else{CB(a+0x65u)=1;CB(a+1u)=0;}
  break;
 case 9:
  FUN_004a5da0((int32_t)id,speed);id=CU(0xd056f8u);a=cv_actor(id);direction=CB(a+0x11u);
  if(direction==8u)CB(a+0x38u)=10;
  else if(direction==9u){FUN_004b3250((int32_t)id);id=CU(0xd056f8u);}
  else if(direction==10u){uint8_t blocked=(uint8_t)(CB(a+0x154u)+1u);CB(a+0x154u)=blocked;
   if(blocked>=100u){CB(a+0x0eu)=2;CB(a+0x65u)=1;}}
  else CB(a+0x154u)=0;
  a=cv_actor(id);b=cv_building(CH(a+0x5eu));
  if(CB(b)!=1u || CW(b+0x40u)==0u){FUN_004b3250((int32_t)id);id=CU(0xd056f8u);a=cv_actor(id);
   CB(a+0x38u)=8;CW(a+0x36u)=20;}
  break;
 case 10:
  CW(a+0x36u)=(uint16_t)(CW(a+0x36u)+1u);
  if(CH(a+0x36u)>10){
   uint32_t failures=0;CW(a+0x36u)=0;trader=CB(a+0x6eu);target=CH(a+0x5eu);player=CB(a+0x0fu);
   if(FUN_0045cf50((int32_t)id,player,target,trader)){
    id=CU(0xd056f8u);a=cv_actor(id);trader=CB(a+0x6eu);target=CH(a+0x5eu);player=CB(a+0x0fu);
    resource=FUN_0045d010(player,target,trader);
    if(resource){id=CU(0xd056f8u);a=cv_actor(id);trader=CB(a+0x6eu);cv_caravan_count_trade(trader,(uint32_t)resource);
     FUN_00444bd0((int32_t)id,resource);id=CU(0xd056f8u);a=cv_actor(id);
     frame=(uint8_t)(CB(a+0x6fu)+1u);CB(a+0x6fu)=frame;
    }else{id=CU(0xd056f8u);failures=1;}
   }else{id=CU(0xd056f8u);failures=1;}
   a=cv_actor(id);trader=CB(a+0x6eu);target=CH(a+0x5eu);player=CB(a+0x0fu);
   if(FUN_0045d180((int32_t)id,player,target,trader)){
    id=CU(0xd056f8u);a=cv_actor(id);trader=CB(a+0x6eu);target=CH(a+0x5eu);player=CB(a+0x0fu);
    resource=FUN_0045d370(player,target,trader);
    if(resource){id=CU(0xd056f8u);a=cv_actor(id);trader=CB(a+0x6eu);cv_caravan_count_trade(trader,(uint32_t)resource);
     FUN_00444b80((int32_t)id,resource);id=CU(0xd056f8u);a=cv_actor(id);CW(a+0x74u)=(uint16_t)(CW(a+0x74u)+1u);
    }else{id=CU(0xd056f8u);++failures;}
   }else{id=CU(0xd056f8u);++failures;}
   a=cv_actor(id);
   if(failures==2u){trader=CB(a+0x6eu);y=CH(a+0x16u);x=CH(a+0x14u);
    target=FUN_004450c0((int32_t)id,(uint32_t)x,(uint32_t)y,trader,UINT32_MAX);
    id=CU(0xd056f8u);a=cv_actor(id);
    if(target){CW(a+0x5eu)=(uint16_t)target;target_x=CW(0xd3b4e4u);CB(a+0x38u)=9;CW(a+0x36u)=0;
     CW(a+0x24u)=target_x;CW(a+0x26u)=CW(0xd3b4e0u);}
    else cv_caravan_exit_destination(a);
   }
  }
  CB(cv_actor(id)+1u)=0;break;
 case 11:
  FUN_004a5da0((int32_t)id,speed);id=CU(0xd056f8u);a=cv_actor(id);direction=CB(a+0x11u);
  if(direction==8u){CB(a+0x38u)=direction;CB(a+0x0eu)=2;}
  else if(direction==9u){FUN_004b3250((int32_t)id);id=CU(0xd056f8u);}
  else if(direction==10u)CB(a+0x0eu)=2;
  break;
 case 7:
  {uint32_t facing=CU(0xe94e00u),base=CU(0xf56e5cu);CB(a+1u)=0;CU(a+4u)=base+facing+1u;
   CW(a+0x36u)=(uint16_t)(CW(a+0x36u)+1u);
   if(CH(a+0x36u)>100){CW(a+0x36u)=0;
    if((uint8_t)FUN_004a35e0((int32_t)id)){id=CU(0xd056f8u);}
    else{id=CU(0xd056f8u);CB(cv_actor(id)+0x38u)=8;}}
  }break;
 case 6:CU(a+4u)=0;break;
 default:CB(a+0x38u)=8;break;
 }
 return cv_caravan_render(id);
}
