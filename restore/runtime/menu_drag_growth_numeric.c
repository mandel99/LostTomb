#include "growth.h"
#include "map_refresh_arithmetic.h"
#include "map_x87_conversion.h"
int32_t FUN_00486ec0(int32_t distance,int32_t limit)
{
    uint32_t difference=(uint32_t)limit-(uint32_t)distance,positive,numerator,denominator;
    uint64_t constant,result;uint8_t raw[10];uint16_t cw=mr_fp_control_word();MRFloat v;
    positive=(int32_t)difference<0?0:difference;
    numerator=limit<(int32_t)positive?(uint32_t)limit:positive;
    denominator=limit<12?12u:(uint32_t)limit;
    v=mr_fp_div(mr_fp_i32((int32_t)numerator),mr_fp_i32((int32_t)denominator),cw);
    constant=*(volatile uint64_t *)(uintptr_t)0x56f570u;
    v=mr_fp_mul(v,mr_fp_binary64(constant),cw);mr_fp_raw80(v,raw);result=restore_map_trunc80(raw);
    return (int32_t)((uint32_t)result-1u);
}
