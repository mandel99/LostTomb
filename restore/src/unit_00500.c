#include "record_score_views.h"
extern int32_t FUN_004cde60(int32_t value,int32_t divisor);
int32_t restore_record_score_004342c0(int32_t index)
{
 uint32_t offset=(uint32_t)index*264u,owner_offset,i;
 uint16_t type=SCORE_W(0x7999e8u+offset);
 int32_t enabled,divisor,sum=0,percentage;
 if (type==0x46u || type==0xbeu || type==0x4cu || type==0x47u || type==0xbfu) return 0;
 if (SCORE_B(0x7999ddu+offset)==0 || SCORE_U(0x705854u)==0) return 0;
 owner_offset=(uint32_t)(int32_t)SCORE_S(0x7999eau+offset)*88u;
 enabled=(int32_t)SCORE_U(0x70cdf4u+owner_offset);
 divisor=enabled!=0 ? (int32_t)SCORE_S(0x7999f4u+offset) : 0;
 /* The eight signed reads still occur when the divisor is zero. */
 for (i=0;i<8;++i) sum+=(int32_t)SCORE_S(0x799a2cu+offset+i*2u);
 if (divisor==0) return 0;
 percentage=FUN_004cde60(sum,divisor);
 if (percentage>200) return 0;
 if (percentage>=100) return 1;
 return percentage<=0 ? 10 : 5;
}
SCORE_CANONICAL(004342c0)
