#include "menu_modal_buttons.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
/* Existing src01767 is the genuine five-instruction leaf: DWORD E38DFC=0,
   BYTE E38E02=0, BYTE E38DF6=0, RET0. No implicit self or argument. */
extern void FUN_004cf1f0(void);

/* Native409130..40913E:3; table5763D8 row2 / count10, call4D8818. */
void restore_menu_modal_00409130(void)
{
 B(0xe38ddau)=0x27;
 B(0xe38dcau)=1;
}
/* Native409140..40914E:3; same table/call, row4. */
void restore_menu_modal_00409140(void)
{
 B(0xe38ddau)=0x46;
 B(0xe38dcau)=1;
}
/* Native40AD00..40AD2F:10; table5C5368 row0 / count8, call4DC1E3. */
void restore_menu_modal_0040ad00(void)
{
 uint32_t player=U(0xeaac28u),base,limit,current;
 B(0xe38ddau)=0x2d;
 base=player*0x49d8u;
 limit=U(0xea5cc0u+base);
 current=U(0xea5cccu+base);
 if((int32_t)current>(int32_t)limit)U(0xea5cccu+base)=limit;
 B(0xe38dcau)=1;
}
/* Native40AD30..40AD88:19; same table/call, row1. Both native returns
   are retained: a zero gate leaves mode and dirty flag untouched. */
void restore_menu_modal_0040ad30(void)
{
 uint32_t player=U(0xeaac28u),base=player*0x49d8u;
 uint32_t gate=U(0xea620cu+base),action,x,y;
 if(!gate)return;
 action=U(0xd3b198u);
 if(!action){
  B(0xe38ddau)=0x2c;
  B(0xe38dcau)=1;
  return;
 }
 x=U(0xe38da8u);y=U(0xe38da4u);
 U(0xf53f0cu)=x;
 U(0xf53f08u)=y;
 FUN_004cf1f0();
 B(0xe38ddau)=0x4d;
 B(0xe38dcau)=1;
}
/* Native40AD90..40ADF0:22; same table/call, row2. The second state and
   limit loads occur after the first conditional write, as in the PE. */
void restore_menu_modal_0040ad90(void)
{
 uint32_t player=U(0xeaac28u),base=player*0x49d8u;
 uint32_t state,price,limit;
 B(0xe38ddau)=0x2e;
 state=U(0xea5dc8u+base);
 if(state==2u){
  price=U(0xea5decu+base);
  limit=U(0xea5cc0u+base);
  if((int32_t)price>(int32_t)limit)U(0xea5dc8u+base)=1;
 }
 state=U(0xea5dc8u+base);
 if(state==1u){
  price=U(0xea5de8u+base);
  limit=U(0xea5cc0u+base);
  if((int32_t)price>(int32_t)limit)U(0xea5dc8u+base)=0;
 }
 B(0xe38dcau)=1;
}
/* Native40AF00..40AF2A:11; table5C5468 row0 / count12,
   calls4DCABC and4DCB50. Other current modes only acquire the dirty flag. */
void restore_menu_modal_0040af00(void)
{
 uint32_t mode=B(0xe38ddau);
 if(mode==0x2cu){
  B(0xe38ddau)=7;
  B(0xe38dcau)=1;
  return;
 }
 if(mode==0x4du)B(0xe38ddau)=9;
 B(0xe38dcau)=1;
}
int restore_menu_modal_button_action(uint32_t token)
{
 switch(token){
 case 0x409130u:restore_menu_modal_00409130();return 1;
 case 0x409140u:restore_menu_modal_00409140();return 1;
 case 0x40ad00u:restore_menu_modal_0040ad00();return 1;
 case 0x40ad30u:restore_menu_modal_0040ad30();return 1;
 case 0x40ad90u:restore_menu_modal_0040ad90();return 1;
 case 0x40af00u:restore_menu_modal_0040af00();return 1;
 default:return 0;
 }
}
