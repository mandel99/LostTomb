#include "record_score_views.h"
extern int32_t FUN_0046f1c0(int32_t index);
int32_t restore_record_score_004347b0(int32_t index)
{
 uint32_t offset=(uint32_t)index*264u,sum;
 if (SCORE_B(0x7999ddu+offset)==0) return 0;
 sum=SCORE_U(0x705a2cu);
 sum+=SCORE_U(0x705a30u);
 sum+=SCORE_U(0x705a28u);
 sum+=SCORE_U(0x705898u);
 sum+=SCORE_U(0x705894u);
 if (sum==0) return 0;
 return FUN_0046f1c0(index)/25;
}
SCORE_CANONICAL(004347b0)
