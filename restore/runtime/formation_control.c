#include "formation_control.h"
#include "building_controllers.h"
#include "actor_create.h"
#include "actor_order_path.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define I(a) ((int32_t)(uint32_t)(a))
extern void FUN_004b77d0(void);
extern uint32_t FUN_004ea9a0(int32_t kind);
extern int32_t FUN_004ea9c0(int32_t kind);
extern uint32_t FUN_0047d350(void);
extern uint32_t FUN_0040b5e0(int32_t formation);

uint32_t restore_formation_create(uint32_t building)
{
 uint8_t used[8]={0};uint32_t defaults[3],i,colour,formation,g,r=building*264u,actor,maximum;
 uint16_t x,y;
 FUN_004b77d0();defaults[0]=U(0x6fdce4u)&255u;defaults[1]=U(0x6fdcf0u)&255u;defaults[2]=U(0x6fdcfcu)&255u;
 for(i=0;i<8u;++i)if(defaults[0]==i||defaults[1]==i||defaults[2]==i)used[i]=1;
 /* The native local palette has eight bytes. Active military formations
    carry colours0..7; keep that original record precondition. */
 for(i=0;i<50u;++i){g=0xbcb9c8u+i*144u;if(B(g)&&B(g+66u)==1u)used[B(g+2u)]=1;}
 for(colour=0;colour<8u&&used[colour];++colour){}
 if(colour==8u)colour=0;
 for(formation=1;formation<50u;++formation)if(!B(0xbcb9c8u+formation*144u))break;
 if(formation==50u)return 0;
 g=0xbcb9c8u+formation*144u;
 B(g)=1;B(g+66u)=1;B(g+1u)=B(0xeaac28u);
 W(g+4u)=W(0x7999eau+r);W(g+6u)=(uint16_t)building;y=W(0x7999e0u+r);
 W(g+42u)=1;W(g+44u)=50;B(g+3u)=1;W(g+130u)=0xffff;B(g+119u)=0;B(g+2u)=(uint8_t)colour;
 x=(uint16_t)(W(0x7999deu+r)+3u);--y;
 W(g+46u)=x;W(g+48u)=y;W(g+50u)=x;W(g+52u)=y;W(g+54u)=x;W(g+56u)=y;
 W(g+128u)=0;W(g+126u)=0;
 actor=(uint32_t)FUN_004b4800(B(0x7999d9u+r),14,0,0,0);
 W(g+64u)=(uint16_t)actor;maximum=U(0xbcb9bcu);
 W(0xbcd652u+actor*388u)=(uint16_t)building;W(0xbcd658u+actor*388u)=(uint16_t)formation;
 U(0xbcb9b4u)=U(0xbcb9b4u)+1u;if(I(formation)>I(maximum))U(0xbcb9bcu)=formation;
 return formation;
}

void restore_formation_recruit(int32_t count,uint32_t formation,int32_t x,int32_t y)
{
 uint32_t g=formation*144u,owner_address=0x7999d9u+(uint32_t)H(0xbcb9ceu+g)*264u;
 if(count>0)do {
  uint32_t player=U(0xeaac28u)*0x49d8u,actor;int32_t px=H(0xea3fe8u+player),py=H(0xea3feau+player);
  if(!FUN_00518400(px,py,H(0xbcb9f6u+g),H(0xbcb9f8u+g))){
   player=U(0xeaac28u)*0x49d8u;px=H(0xea3ff0u+player);py=H(0xea3ff2u+player);
   if(!FUN_00518400(px,py,H(0xbcb9f6u+g),H(0xbcb9f8u+g))){px=x;py=y;}
  }
  actor=(uint32_t)FUN_004b4800(B(owner_address),H(0xbcb9ccu+g),px,py,0);
  W(0xbcd658u+actor*388u)=(uint16_t)formation;B(0xbcd65bu+actor*388u)=1;B(0xbcd630u+actor*388u)=16;
 }while(--count);
 restore_formation_refresh();
}
static int eligible(uint32_t kind,uint32_t kind_address)
{
 if((kind>=11u&&kind<=13u)||(kind>=43u&&kind<=53u)||kind==57u)return 1;
 if((uint8_t)FUN_004ea9a0(I(kind)))return 1;
 if((uint8_t)FUN_004ea9c0((int32_t)B(kind_address)))return 1;
 kind=B(kind_address);return kind==99u||kind==54u||kind==55u||kind==56u;
}
void restore_formation_refresh(void)
{
 uint32_t id,g,a,slot,n=0,i,kind,removed,event,player,value,total,ready;
 for(id=1;id<50u;++id){
  g=0xbcb9c8u+id*144u;for(i=0;i<8u;++i)U(g+8u+i*4u)=0;
  B(g+40u)=0;B(g+3u)=1;W(g+74u)=0;W(g+76u)=0;
 }
 for(id=1;id<2000u;++id){
  a=0xbcd5f8u+id*388u;
  if(B(a+14u)!=1u||B(a+56u)==4u)continue;
  if(!eligible(B(a+10u),a+10u))continue;
  g=0xbcb9c8u+(uint32_t)H(a+96u)*144u;B(g+40u)=(uint8_t)(B(g+40u)+1u);
  kind=B(a+10u);W(g+76u)=(uint16_t)(W(g+76u)+W(0x7090d0u+kind*56u));
  W(g+74u)=(uint16_t)(W(g+74u)+B(a+29u));if(B(a+99u)!=1u)B(g+3u)=0;
  for(slot=0;slot<16u;++slot)if(!W(g+8u+slot*2u)){B(a+98u)=(uint8_t)slot;W(g+8u+slot*2u)=(uint16_t)id;break;}
 }
 U(0xd0563cu)=0;U(0xd05638u)=0;U(0xd05634u)=0;U(0xd05630u)=0;
 for(id=1;id<50u;++id){
  g=0xbcb9c8u+id*144u;if(B(g)!=1u||B(g+105u))continue;
  if(!B(g+66u)){
   if(B(g+40u)){
    value=U(0xd0563cu);total=U(0xd05638u);U(0xd0563cu)=value+1u;U(0xd05638u)=total+B(g+40u);
   }else{
    event=(uint32_t)H(g+120u);
    for(i=0;i<n&&U(0xbca45cu+i*4u)!=event;++i){}
    if(i==n){U(0xbca45cu+n*4u)=event;B(0xbca428u+n)=1;++n;}
    FUN_0040f8f0((uint32_t *)(uintptr_t)g,144,0);
   }
  }else if(B(g+40u)){
   ready=(uint32_t)H(g+90u);W(g+90u)=1;
   for(i=0;i<B(g+40u);++i){
    a=(uint32_t)H(g+8u+i*2u);
    if(a&&B(0xbcd609u+a*388u)!=8u)W(g+90u)=0;
   }
   value=U(0xd05634u);total=U(0xd05630u);U(0xd05634u)=value+1u;U(0xd05630u)=total+B(g+40u);
   if(W(g+4u)==13u){
    if(!ready&&W(g+90u)==1u)(void)FUN_0047d350();
    U(0xd05630u)=U(0xd05630u)+(B(g+40u)>>1);
   }
  }
 }
 player=U(0xeaac28u)*0x49d8u;
 B(0xea5eddu+player)=0;B(0xea5edfu+player)=0;B(0xea5edcu+player)=0;
 B(0xea5edau+player)=0;B(0xea5edbu+player)=0;B(0xea5ed5u+player)=0;B(0xea5ed6u+player)=0;
 for(id=1;id<50u;++id){
  g=0xbcb9c8u+id*144u;if(B(g)!=1u)continue;
  if(!B(g+66u)){
   event=(uint32_t)H(g+120u);for(i=0;i<n;++i)if(event==U(0xbca45cu+i*4u))B(0xbca428u+i)=0;
  }else{
   value=FUN_0040b5e0((int32_t)id)!=0;player=U(0xeaac28u)*0x49d8u;
   B(0xea5edcu+player)=(uint8_t)(B(0xea5edcu+player)+1u);
   a=(value?0xea5edau:0xea5edbu)+player;B(a)=(uint8_t)(B(a)+1u);
   if(B(g+103u)&&B(g+40u)){
    B(0xea5eddu+player)=(uint8_t)(B(0xea5eddu+player)+1u);
    a=(value?0xea5ed5u:0xea5ed6u)+player;B(a)=(uint8_t)(B(a)+1u);
   }
   B(0xea5edfu+player)=(uint8_t)(B(0xea5edfu+player)+B(g+40u));
  }
 }
 for(i=0;i<n;++i)if(B(0xbca428u+i)){
  event=U(0xbca45cu+i*4u);
  if(I(event)>=0&&I(event)<I(U(0x785d70u))){removed=0x785d74u+event*124u;if(removed)B(removed+50u)=0;}
 }
}
void restore_building_control_C_004b6e10(struct restore_building_control_frame *f)
{uint32_t sp=f->r[4];f->r[0]=restore_formation_create(U(sp+4u));f->r[4]=sp+4u;}
void restore_building_control_C_00472790(struct restore_building_control_frame *f)
{uint32_t sp=f->r[4];restore_formation_recruit(I(U(sp+4u)),U(sp+8u),I(U(sp+12u)),I(U(sp+16u)));f->r[4]=sp+4u;}
void restore_building_control_C_004b7c80(struct restore_building_control_frame *f)
{restore_formation_refresh();f->r[4]+=4u;}
