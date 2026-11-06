#include "record_score_views.h"
int32_t restore_record_rounded_amount_0046f1c0(int32_t index)
{
 uint32_t offset=(uint32_t)index*264u,value;
 if (SCORE_B(0x7999ddu+offset)==0) return 0;
 value=SCORE_U(0x799a64u+offset)+5000u;
 return (int32_t)value/10000;
}
int32_t FUN_0046f1c0(int32_t index)
{ return restore_record_rounded_amount_0046f1c0(index); }
