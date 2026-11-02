#include "menu_window_cursor_consumer.h"
int32_t FUN_004b5520(uint32_t tile)
{
    int32_t id=*(int16_t *)(uintptr_t)(0xb350dcu+tile*2u);
    while(id){
        RestoreOrderActor *actor=restore_order_actor((uint32_t)id);
        if(actor->kind>=11&&actor->kind<=14)return actor->tag;
        id=actor->next;
    }
    return 0;
}
