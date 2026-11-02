#include "menu_modal_buttons2.h"
#include "menu_dialog.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

/* Preserve the two actual IMUL-high / wrapped ADD / SAR / sign-correction
   sequences. Their shifts are3 and4 (division by15 and30 respectively). */
static uint32_t option_quotient(uint32_t value,uint32_t shift)
{
 int64_t product=(int64_t)(int32_t)UINT32_C(0x88888889)*(int32_t)value;
 uint32_t high=(uint32_t)((uint64_t)product>>32),sum=high+value;
 uint32_t shifted=(sum>>shift)|((sum&UINT32_C(0x80000000))?
                              (UINT32_MAX<<(32u-shift)):0u);
 return shifted+(shifted>>31);
}
/* Native408C30..408C63:17; table5C5E20 row0/count3, call4D94AC.
   The native callback itself does not publish a dirty flag. */
void restore_menu_modal2_00408c30(void)
{
 uint32_t value=U(0xd38360u);
 uint32_t next=option_quotient(value,3)-9u;
 if((int32_t)next<0||(int32_t)next>7)next=0;
 U(0xd38360u)=(next*3u+30u)*5u;
}
/* Native408C70..408CA3:16; same table/call, row1. In particular, native
   negative values are not assigned an invented lower bound. */
void restore_menu_modal2_00408c70(void)
{
 uint32_t value=U(0xd38364u);
 uint32_t next=option_quotient(value,4)+1u;
 if((int32_t)next>4)next=2;
 U(0xd38364u)=next*30u;
}
/* Native408CB0..408CC4:7; same table/call, row2. Addition wraps before
   the signed comparison, including when a large positive input wraps. */
void restore_menu_modal2_00408cb0(void)
{
 uint32_t next=U(0xd38368u)+20u;
 if((int32_t)next>100)next=0;
 U(0xd38368u)=next;
}
/* Native40AA50..40AAB8:29; table5C5168 row0/count1, call4DC3EB.
   Two actual message-dialog branches use the genuine RET no-op callback. */
void restore_menu_modal2_0040aa50(void)
{
 uint32_t player=U(0xeaac28u),base=player*0x49d8u;
 uint32_t busy=U(0xea5d68u+base),funds,action;
 if(busy)return;
 if(W(0xea6084u+base)==0){FUN_004264d0(0x58u,0x51f780u,0);return;}
 funds=U(0xea187cu+base);
 if((int32_t)funds<=-5000){FUN_004264d0(0x60u,0x51f780u,0);return;}
 action=U(0xd3b198u);
 B(0xe38dcau)=1;
 B(0xe38ddau)=action?0x52:0x2b;
}
/* Native40AC50..40AC79:8; table5C5288 rows0..4/count8, call4DCA29. */
void restore_menu_modal2_0040ac50(void)
{
 uint32_t action=U(0xd3b198u);
 if(W(0x7848e2u+action*2u)!=0){
  uint32_t player=U(0xeaac28u);
  U(0xea5d60u+player*0x49d8u)=action;
 }
 B(0xe38dcau)=1;
}
/* Native40AC80..40ACDA:24; same table/call, rows5..7. All rejection
   paths retain the existing dirty flag and selected values unchanged. */
void restore_menu_modal2_0040ac80(void)
{
 uint32_t player=U(0xeaac28u),base=player*0x49d8u;
 uint32_t funds=U(0xea187cu+base),busy,action,first;
 if((int32_t)funds<=-5000)return;
 busy=U(0xea5d84u+base);
 action=U(0xd3b198u);
 if(busy&&action==3u)return;
 first=U(0xea6010u+base);
 if(first==UINT32_MAX)return;
 if(U(0xea6014u+base)==UINT32_MAX)return;
 U(0xea5d64u+base)=action;
 U(0x775678u)=action*10u;
 B(0xe38dcau)=1;
}
int restore_menu_modal_button2_action(uint32_t token)
{
 switch(token){
 case 0x408c30u:restore_menu_modal2_00408c30();return 1;
 case 0x408c70u:restore_menu_modal2_00408c70();return 1;
 case 0x408cb0u:restore_menu_modal2_00408cb0();return 1;
 case 0x40aa50u:restore_menu_modal2_0040aa50();return 1;
 case 0x40ac50u:restore_menu_modal2_0040ac50();return 1;
 case 0x40ac80u:restore_menu_modal2_0040ac80();return 1;
 default:return 0;
 }
}
