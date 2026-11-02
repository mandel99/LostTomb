#include "actor_combat.h"
void FUN_004b3f00(int32_t id)
{
 uint32_t a=cb_actor((uint32_t)id);uint8_t value;
 switch((int8_t)CB_B(a+9u)){
 case 0:CB_W(a+0x44u)--;break;
 case 1:CB_W(a+0x44u)--;CB_W(a+0x42u)++;break;
 case 2:CB_W(a+0x42u)++;break;
 case 3:CB_W(a+0x44u)++;CB_W(a+0x42u)++;break;
 case 4:CB_W(a+0x44u)++;break;
 case 5:CB_W(a+0x44u)++;CB_W(a+0x42u)--;break;
 case 6:CB_W(a+0x42u)--;break;
 case 7:CB_W(a+0x44u)--;CB_W(a+0x42u)--;break;
 }
 value=CB_B(a+0x6fu);
 if(value){value--;CB_B(a+0x6fu)=value;if(!value){CB_B(a+0x5du)=0;return;}CB_B(a+0x5du)=1;
  if(CB_B(a+0x70u)<CB_B(a+0x71u))CB_B(a+0x70u)++;if(CB_B(a+0x70u)>CB_B(a+0x71u))CB_B(a+0x70u)--;
 }else if(CB_B(a+0x70u))CB_B(a+0x70u)--;
}
