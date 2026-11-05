#include "city_worker_supply.h"
#include "menu_list_scroll.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

/* Whole461F90. Reload the actor kind for each native table access. */
void restore_actor_initialize_type_parameters(int32_t actor)
{
    uint32_t off=(uint32_t)actor*388u,value;
    U(0xbcd71cu+off)=0u;
    U(0xbcd720u+off)=U(0x7090d0u+(uint32_t)B(0xbcd602u+off)*56u);
    U(0xbcd724u+off)=U(0x7090d0u+(uint32_t)B(0xbcd602u+off)*56u);
    B(0xbcd728u+off)=0u;
    U(0xbcd730u+off)=0u;
    U(0xbcd72cu+off)=0u;
    U(0xbcd734u+off)=U(0x7090ecu+(uint32_t)B(0xbcd602u+off)*56u);
    U(0xbcd738u+off)=U(0x7090f4u+(uint32_t)B(0xbcd602u+off)*56u);
    U(0xbcd73cu+off)=U(0x7090fcu+(uint32_t)B(0xbcd602u+off)*56u);
    U(0xbcd740u+off)=U(0x709100u+(uint32_t)B(0xbcd602u+off)*56u);
    U(0xbcd748u+off)=U(0x7090d4u+(uint32_t)B(0xbcd602u+off)*56u);
    B(0xbcd74cu+off)=0u;
    value=U(0x7090d4u+(uint32_t)B(0xbcd602u+off)*56u);
    value=(uint32_t)FUN_004cde40((int32_t)value,20);
    U(0xbcd750u+off)=value;
}
