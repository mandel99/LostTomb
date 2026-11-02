#include "house_services.h"
#include <stdint.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))

/* Whole484990/483E90/483FB0/484670 share the occupied/populated gate.
   Count every qualifying cell even when no stamp field is selected. */
static uint32_t occupied_house(int32_t building,uint32_t field)
{
    uint32_t off=(uint32_t)building*264u;
    if(building<=0||!B(0x7999ddu+off)||H(0x7999f4u+off)<=0)return 0;
    if(field)B(field+off)=96u;
    return 1;
}
int32_t FUN_00484990(int32_t x,int32_t y)
{
    return restore_house_service_scan(x,y,2u,occupied_house,0,2u);
}
int32_t FUN_00483e90(int32_t x,int32_t y)
{
    if(I(U(0x78d7b0u))>0)return 0;
    return restore_house_service_scan(x,y,2u,occupied_house,0x799a4fu,0);
}
int32_t FUN_00483fb0(int32_t x,int32_t y,int32_t deity)
{
    uint32_t field=(uint32_t)deity<=4u?0x799a51u+(uint32_t)deity:0;
    return restore_house_service_scan(x,y,2u,occupied_house,field,2u);
}
void FUN_00484670(int32_t x,int32_t y)
{
    (void)restore_house_service_scan(x,y,2u,occupied_house,0x799a48u,0);
}

/* Whole484890: radius4, positive cell IDs, building kinds88/89 only.
   It clears the target byte without the occupied-house gate and returns1. */
static uint32_t convert_building(int32_t building,uint32_t unused)
{
    uint32_t off=(uint32_t)building*264u,kind;(void)unused;
    if(building>0){
        kind=W(0x7999e8u+off);
        if(kind==88u||kind==89u)B(0x799a9au+off)=0;
    }
    return 0;
}
uint32_t FUN_00484890(int32_t x,int32_t y)
{
    (void)restore_house_service_scan(x,y,4u,convert_building,0,2u);return 1;
}

/* Whole483160: any nonzero signed cell ID, kinds10..29 and nonzero state.
   The population/occupancy gates used by ordinary coverage do not apply. */
static uint32_t clear_house_flag(int32_t building,uint32_t unused)
{
    uint32_t off=(uint32_t)building*264u;int32_t kind;(void)unused;
    if(building){
        kind=H(0x7999e8u+off);
        if(kind>=10&&kind<=29&&B(0x7999d8u+off))B(0x799a5fu+off)=0;
    }
    return 0;
}
uint32_t FUN_00483160(int32_t x,int32_t y)
{
    (void)restore_house_service_scan(x,y,2u,clear_house_flag,0,2u);return 0;
}

/* Whole483260: sample the old crime value before each subtraction, including
   repeat visits. DWORD arithmetic wraps; both limits compare signed DWORDs. */
static uint32_t reduce_crime(int32_t building,uint32_t amount)
{
    uint32_t off=(uint32_t)building*264u,value,minimum;
    if(building){
        value=U(0x799abcu+off);
        if(I(value)>I(U(0x9ab9dcu)))U(0x9ab9dcu)=value;
        value-=amount;U(0x799abcu+off)=value;
        minimum=U(0x70ce0cu+(uint32_t)H(0x7999eau+off)*88u);
        if(I(value)<I(minimum))U(0x799abcu+off)=minimum;
        if(I(U(0x799abcu+off))>1000)U(0x799abcu+off)=1000;
    }
    return 0;
}
void FUN_00483260(int32_t x,int32_t y,int32_t amount)
{
    U(0x9ab9dcu)=0;
    (void)restore_house_service_scan(x,y,2u,reduce_crime,(uint32_t)amount,3u);
}
