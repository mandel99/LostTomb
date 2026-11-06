#include "record_score_views.h"
int32_t restore_record_score_00434060(int32_t index)
{
 uint32_t offset=(uint32_t)index*264u,sum;
 uint16_t type=SCORE_W(0x7999e8u+offset);
 uint8_t rank;
 if (type==0x33u || type==0x35u) return 0;
 if (SCORE_B(0x7999ddu+offset)==0) return 0;
 sum=SCORE_U(0x705808u);
 sum+=SCORE_U(0x705810u);
 if (sum==0) return 0;
 rank=SCORE_B(0x799a59u+offset);
 if (rank==1) return 8;
 if (rank==2) return 5;
 if (rank==3) return 2;
 return 0;
}
SCORE_CANONICAL(00434060)
