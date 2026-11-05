#include "city_disaster_fields.h"
#define DISASTER_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))

/* Whole4696B0. The immutable 4696EC remap and 4696E4 targets select these
   seventeen exclusions. MOVSX/type-minus183 defines the full EAX input to
   XOR AL,AL or MOV AL,1, including out-of-range signed WORD types. */
uint32_t restore_building_disaster_spread_result(int32_t building)
{
    uint32_t result;
    int32_t kind=(int32_t)(int16_t)DISASTER_W(0x7999e8u+(uint32_t)building*264u);
    result=(uint32_t)kind-183u;
    switch(kind){
    case 183:case 207:case 210:case 213:case 215:case 218:
    case 219:case 220:case 222:case 225:case 227:case 228:
    case 229:case 230:case 234:case 235:case 236:
        return result&0xffffff00u;
    default:
        return (result&0xffffff00u)|1u;
    }
}

uint8_t restore_building_allows_disaster_spread(int32_t building)
{return (uint8_t)restore_building_disaster_spread_result(building);}

uint32_t FUN_004696b0(int32_t building)
{return restore_building_disaster_spread_result(building);}
