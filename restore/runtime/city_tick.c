#include "simulation_tick.h"
#include "calendar_updates.h"
#include "flood_simulation.h"
#include "actor_predicate.h"
#include "actor_walking.h"
#include "sound_streams.h"
#include "water_refresh.h"
#include "religion_updates.h"
#include "trade_updates.h"
#include "financial_updates.h"

#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

extern void FUN_00443330(void);
extern void FUN_004525f0(void),FUN_004528b0(void),FUN_004539e0(void);
extern void FUN_00454e20(void),FUN_00454fa0(void),FUN_004550e0(void),FUN_00455430(void);
extern void FUN_004558b0(void),FUN_00455900(void),FUN_00457a40(void),FUN_00457af0(void);
extern void FUN_00458440(void),FUN_00458540(void),FUN_00459070(void),FUN_00459100(void);
extern void FUN_00459350(void),FUN_004595a0(void),FUN_00459670(void),FUN_004599a0(void);
extern void FUN_0045aa30(void),FUN_0045aaa0(void),FUN_0045d5d0(void),FUN_0045e040(void);
extern void FUN_0045e6c0(void),FUN_00462180(void),FUN_004677c0(void),FUN_00468170(void);
extern void FUN_004684e0(void),FUN_00468860(void),FUN_00468b70(void),FUN_00469730(void);
extern void FUN_00469b90(void),FUN_0046a060(void),FUN_0046a0c0(void),FUN_0046a140(void);
extern void FUN_0046d840(void),FUN_0046f200(void),FUN_0047e290(void),FUN_004860d0(void);
extern void FUN_004f3890(void),FUN_0051f780(void);
extern void FUN_004b7790(int32_t mode);
extern void FUN_00475cc0(int32_t,int32_t,int32_t,int32_t,uint32_t);

void FUN_00452150(void){U(0x78d784u)=0;}

/* Native454D70 resets27 DWORD counters for the full DWORD player index. */
void FUN_00454d70(void)
{
 static const uint32_t counters[]={
  0xea5b14u,0xea5b1cu,0xea5b24u,0xea5b2cu,0xea5b74u,0xea5b78u,0xea5b7cu,
  0xea5bf8u,0xea5b80u,0xea5b84u,0xea5bc0u,0xea5bc4u,0xea5bc8u,0xea5b90u,
  0xea5b94u,0xea5b98u,0xea5bacu,0xea5bb0u,0xea5b9cu,0xea5b88u,0xea5b8cu,
  0xea5bb4u,0xea5bb8u,0xea5bbcu,0xea5bccu,0xea5fbcu,0xea5fb8u};
 uint32_t offset=U(0xeaac28u)*0x49d8u,i;
 for(i=0;i<sizeof(counters)/sizeof(counters[0]);++i)U(counters[i]+offset)=0;
}

static void expire_sound(uintptr_t timer)
{
 uint32_t value=U(timer);
 if((int32_t)value<=0)return;
 U(timer)=--value;
 if(!value){FUN_0055f561(RESTORE_SOUND,2);U(0xf55b94u)=0;}
}

/* Complete451C50. The native dispatcher has51 phases per day, with deliberate
 * idle slots. Counters and player indices are DWORDs, not byte predicates. */
void FUN_00451c50(void)
{
 uint32_t phase=U(0x78d7ccu),offset,total,index,value,record,actor;
 int32_t elapsed;
 switch(phase){
 case 0:FUN_0051f780();break;
 case 1:FUN_004e9890(1);break;
 case 2:FUN_00443330();break;
 case 3:U(0xf53fd0u)=1;break;
 case 4:FUN_004460b0();break;
 case 5:FUN_004b7790(0);break;
 case 6:FUN_0051f780();break;
 case 7:FUN_00468860();break;
 case 8:FUN_0045e040();break;
 case 9:FUN_00452150();break;
 case 10:FUN_0045aa30();break;
 case 11:
  offset=U(0xeaac28u)*0x49d8u;U(0xea1894u+offset)=0;
  for(index=0;index<100u;++index){
   total=U(0xea1894u+offset)+(uint32_t)(int32_t)H(0xea3e34u+offset+index*2u);
   U(0xea1894u+offset)=total;
  }
  if((int32_t)U(0xea5f0cu+offset)<(int32_t)U(0xea1894u+offset))
   U(0xea5f0cu+offset)=U(0xea1894u+offset);
  break;
 case 12:FUN_004558b0();break;
 case 16:FUN_0045aaa0();break;
 case 17:FUN_0045d5d0();break;
 case 18:FUN_0045e6c0();break;
 case 19:FUN_00457a40();break;
 case 20:FUN_00455900();FUN_004860d0();break;
 case 21:FUN_00457af0();break;
 case 22:FUN_00458440();break;
 case 23:FUN_00458540();break;
 case 24:FUN_00459070();break;
 case 25:
  FUN_00459100();FUN_00459350();FUN_004595a0();
  FUN_0051f780();FUN_0051f780();FUN_00459670();break;
 case 27:FUN_004555d0();break;
 case 28:FUN_00455430();break;
 case 29:FUN_004b7790(1);break;
 case 30:U(0xf53fd0u)=1;break;
 case 31:FUN_00462180();break;
 case 32:FUN_00444c20();FUN_0046f200();break;
 case 33:FUN_004677c0();FUN_00468170();break;
 case 34:FUN_004684e0();break;
 case 35:
  offset=U(0xeaac28u)*0x49d8u;
  if(!W(0xea5ce8u+offset)||FUN_004c5120((int32_t)U(0xea5f9cu+offset))!=4u)
   FUN_00454e20();
  else{
   offset=U(0xeaac28u)*0x49d8u;
   if(U(0xea6200u+offset))U(0xea6200u+offset)=0;
   else{
    FUN_00454e20();
    offset=U(0xeaac28u)*0x49d8u;U(0xea6200u+offset)=10;
   }
  }
  break;
 case 36:FUN_00454fa0();break;
 case 37:FUN_0046a0c0();FUN_0046a140();break;
 case 38:FUN_004539e0();break;
 case 39:FUN_004528b0();break;
 case 40:FUN_004525f0();break;
 case 43:FUN_00469730();break;
 case 44:FUN_00468b70();break;
 case 45:FUN_00469b90();FUN_0047e290();break;
 case 46:FUN_0051f780();break;
 case 47:FUN_0046a060();break;
 case 48:FUN_004599a0();break;
 case 49:FUN_004550e0();break;
 case 50:FUN_0051f780();break;
 default:break;
 }
 expire_sound(0x78d79cu);expire_sound(0x78d7a8u);
 if(B(0xe38ddau)==1u)expire_sound(0x78d7a0u);
 value=U(0x78d7b0u);
 if((int32_t)value>0){
  U(0x78d7b0u)=--value;
  if(!value){
   /* Retain the original full-map argument order, including swapped bounds. */
   FUN_00475cc0(0,0,(int32_t)(U(0xbca2d4u)-1u),(int32_t)(U(0xbca2d0u)-1u),1);
   FUN_004776f0(0,0,(int32_t)(U(0xbca2d4u)-1u),(int32_t)(U(0xbca2d0u)-1u));
   FUN_0046d840();
   for(actor=1,record=0xbcd786u;record<0xc8ad42u;record+=388u,++actor)
    if(B(record+4u)&&B(record)==0x5fu)FUN_004b52f0((int32_t)actor);
  }else{
   elapsed=(int32_t)(U(0x78d7acu)-value)/2;
   if(elapsed<228){
    FUN_00475cc0((int32_t)(229u-(uint32_t)elapsed),0,
                 (int32_t)(230u-(uint32_t)elapsed),(int32_t)(U(0xbca2d4u)-1u),1);
    FUN_004776f0((int32_t)(229u-(uint32_t)elapsed),0,
                 (int32_t)(230u-(uint32_t)elapsed),(int32_t)(U(0xbca2d4u)-1u));
    FUN_0046d840();
   }
  }
  B(0xe38dcau)=1;
 }
 value=U(0x78d7a4u);
 if((int32_t)value>0){U(0x78d7a4u)=--value;if(!value)B(0xeaaf5cu)=0;}
 value=U(0x78d7ccu)+1u;U(0x78d7ccu)=value;
 if((int32_t)value>=51){U(0x78d7ccu)=0;FUN_00452370();}
 if(B(0xeaaf5cu))FUN_004f3890();
}
