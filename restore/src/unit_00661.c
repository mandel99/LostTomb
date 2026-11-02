#include "menu_event_actions.h"
#include "town_menu.h"

/* Whole4464F0: enumerate the actual simulation manager and invoke451540
   with each event record as its explicit receiver. The legacy C body dropped
   this receiver and reached44E6F0 with an uninitialized pointer. */
void FUN_004464f0(void)
{
    RestoreTownRecord *event;
    if(*(volatile uint32_t *)(uintptr_t)0x605a3cu==1u)return;
    event=(RestoreTownRecord *)FUN_0044bb90(0,0);
    while(event){
        restore_event_update_00451540(event);
        event=(RestoreTownRecord *)FUN_0044bb90((const int16_t *)event,0);
    }
}
