#include "record_score_views.h"
#include "menu_building_kind.h"
int32_t restore_record_score_00433d50(int32_t index)
{
 uint32_t selection=SCORE_U(0x70e488u),offset,mode;
 uint8_t marker;
 SCORE_U(0xd3b5c0u)=SCORE_U(0xb7482cu+selection*4u);
 offset=(uint32_t)index*264u;
 if (SCORE_W(0x7999e8u+offset)==0x51u) return 0;
 if ((uint8_t)FUN_00488030((uint32_t)index)!=0) {
  selection=SCORE_U(0x70e488u);
  mode=SCORE_U(0xbca2e0u);
  marker=SCORE_B(0xb67d1cu+selection);
  if ((mode==0 && marker!=0x50) || (mode==2 && marker!=0x40) ||
      (mode==4 && marker!=0x42) || (mode==6 && marker!=0x52)) return 0;
 }
 return (int32_t)SCORE_S(0x799a1eu+offset)/100;
}
SCORE_CANONICAL(00433d50)
