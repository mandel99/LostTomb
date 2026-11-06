#include "record_score_views.h"
#include "menu_building_kind.h"
int32_t restore_record_score_00433c90(int32_t index)
{
 uint32_t offset=(uint32_t)index*264u,selection,mode;
 uint16_t type=SCORE_W(0x7999e8u+offset);
 uint8_t marker;
 if (type==0xa7u || type==99u) return 0;
 if ((uint8_t)FUN_00488030((uint32_t)index)!=0) {
  selection=SCORE_U(0x70e488u);
  marker=SCORE_B(0xb67d1cu+selection);
  mode=SCORE_U(0xbca2e0u);
  if ((mode==0 && marker!=0x50) || (mode==2 && marker!=0x40) ||
      (mode==4 && marker!=0x42) || (mode==6 && marker!=0x52)) return 0;
 }
 return (int32_t)SCORE_S(0x799a20u+offset)/100;
}
SCORE_CANONICAL(00433c90)
