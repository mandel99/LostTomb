#include "record_score_views.h"
int32_t restore_record_score_00434250(int32_t index)
{
 uint32_t offset=(uint32_t)index*264u,sum;
 uint16_t type=SCORE_W(0x007999e8u+offset);
 if (type==0x20u || type==0x25u) return 0;
 if (SCORE_B(0x007999ddu+offset)==0) return 0;
 sum=SCORE_U(0x007057d0u);
 sum+=SCORE_U(0x007057bcu);
 if (sum==0) return 0;
 return 10-(int32_t)(SCORE_B(0x00799a49u+offset)/10u);
}
SCORE_CANONICAL(00434250)
