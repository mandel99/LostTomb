#include "pharaoh/scenario/tutorial_advisors.h"

static int32_t signed_bits(uint32_t bits)
{
    return bits<=INT32_MAX?(int32_t)bits:
        (int32_t)((int64_t)bits-INT64_C(0x100000000));
}

/* Reuse of UIManagerTutorialAdvisorFindAvailable/NextAdvisor, preserving
   both the native increment store and the following wrap store. */
uint8_t pharaoh_tutorial_find_available_advisor(
    const uint8_t *flags,uint32_t *selection)
{
    const uint32_t start=*selection;
    if(flags[start]!=0) return 1;
    for(;;){
        *selection+=1u;
        if(signed_bits(*selection)>=15) *selection=1;
        if(*selection==start) return 0;
        if(flags[*selection]!=0) return 1;
    }
}
