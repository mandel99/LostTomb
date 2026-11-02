#include "menu_route_capacity.h"
#include "menu_full_input_numeric.h"
#include <string.h>
static int32_t coordinate(const uint8_t *point,uint32_t offset)
{int16_t value;memcpy(&value,point+offset,2);return value;}
int restore_menu_route_required_samples(const uint8_t *points,uint32_t count,
                                       uint8_t step,uint32_t stop_after,uint32_t *required)
{
    uint32_t j,total=0;
    if(count>50||!required||stop_after>INT32_MAX)return 0;
    for(j=1;j<count;++j){
        uint32_t bits,exponent,integer_part,significand;
        int32_t dx,dy;
        if(total>stop_after)break;
        if(!step)continue;
        dx=coordinate(points+j*6u,0)-coordinate(points+(j-1u)*6u,0);
        dy=coordinate(points+j*6u,2)-coordinate(points+(j-1u)*6u,2);
        bits=restore_menu_input_length32(dx,dy);
        if(!(bits&0x7fffffffu))continue;
        exponent=(bits>>23)&255u;
        /* Signed-WORD endpoint deltas give a finite nonnegative length below
           2^17. Reject an impossible numeric result rather than truncate it. */
        if((bits&0x80000000u)||exponent>=144u)return 0;
        significand=(bits&0x7fffffu)|0x800000u;
        integer_part=exponent<127u?0:significand>>(150u-exponent);
        /* Original loop writes at distances0,step,2*step,... <=length.
           floor(length/step)=floor(floor(length)/step) for integer step. */
        total+=integer_part/(uint32_t)step+1u;
    }
    *required=total;return 1;
}
