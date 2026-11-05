#include "city_worker_supply.h"
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))

/* Whole460EA0. Native fixed scan, signed links, and stale-link clearing. */
int32_t restore_city_find_vacant_service_building(uint32_t actor_kind)
{
    uint32_t index=1u,cursor=0x799b96u,off,kind;
    int32_t wanted=actor_kind==25u?76:actor_kind==77u?181:182;
    int32_t linked;
    do{
        if(B(cursor-0xb6u)!=0u && H(cursor-0xa6u)==wanted){
            linked=H(cursor);
            if(linked<1)return (int32_t)index;
            off=(uint32_t)linked*388u;
            if(B(0xbcd606u+off)!=0u){
                kind=B(0xbcd602u+off);
                if((kind==actor_kind || kind==0u) && H(0xbcd652u+off)==(int32_t)index){
                    cursor+=264u;++index;
                    continue;
                }
            }
            W(0x799a8eu+index*264u)=0u;
            return (int32_t)index;
        }
        cursor+=264u;++index;
    }while((int32_t)cursor<0x89b78e);
    return 0;
}
