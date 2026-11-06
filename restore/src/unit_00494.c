#include "record_score_views.h"
int32_t restore_record_score_00433ed0(int32_t index)
{
 uint32_t selection=SCORE_U(0x70e488u),offset,sum;
 uint16_t type;
 SCORE_U(0xd3b5c0u)=SCORE_U(0xb7482cu+selection*4u);
 offset=(uint32_t)index*264u;
 type=SCORE_W(0x7999e8u+offset);
 /* Exact native two-target switch table:30..37 and226 are excluded. */
 if ((type>=30u && type<=37u) || type==226u) return 0;
 if (SCORE_B(0x7999ddu+offset)==0) return 0;
 sum=SCORE_U(0x7057ccu);
 sum+=SCORE_U(0x7057d0u);
 sum+=SCORE_U(0x7057c8u);
 sum+=SCORE_U(0x7057c4u);
 sum+=SCORE_U(0x7057c0u);
 sum+=SCORE_U(0x705ac4u);
 sum+=SCORE_U(0x7057bcu);
 sum+=SCORE_U(0x7057b8u);
 sum+=SCORE_U(0x7057b4u);
 if (sum==0) return 0;
 return 10-(int32_t)SCORE_C(0x799a58u+offset)/10;
}
SCORE_CANONICAL(00433ed0)
