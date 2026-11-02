#include "map_x87_conversion.h"
uint64_t restore_map_trunc80(const uint8_t raw80[10])
{
    const uint64_t indefinite=UINT64_C(0x8000000000000000);
    uint64_t significand=0,magnitude;
    uint32_t exponent=(uint32_t)raw80[8]|((uint32_t)(raw80[9]&0x7fu)<<8);
    uint32_t negative=raw80[9]>>7,i;
    int32_t power;
    for(i=0;i<8;++i)significand|=(uint64_t)raw80[i]<<(8u*i);
    if(exponent==0x7fffu)return indefinite; /* Infinity and every NaN encoding. */
    if(exponent==0)return 0; /* Zeros, tiny subnormals and pseudo-denormals. */
    if((significand&indefinite)==0)return indefinite; /* Unsupported unnormal. */
    power=(int32_t)exponent-16383;
    if(power<0)return 0;
    if(power>63)return indefinite;
    if(power==63){
        if(negative&&significand==indefinite)return indefinite; /* Valid INT64_MIN. */
        return indefinite; /* Every other value is outside signed64. */
    }
    magnitude=significand>>(63u-(uint32_t)power);
    return negative?UINT64_C(0)-magnitude:magnitude;
}
