#include "pharaoh/building/construction_undo.h"
#include "pharaoh/core/memory_fill.h"
#include "pharaoh/core/memory_strings.h"

uint32_t pharaoh_construction_backup_map(const PharaohConstructionUndoContext *c)
{
    for(int32_t i=0;i<36;++i) c->costs[i]=0;
    const int32_t tool=*c->current_tool;
    *c->value_bc968c=0;*c->buildings->available=1;*c->value_bc9688=0;
    *c->buildings->count=0;*c->value_bc95f0=0;*c->saved_tool=tool;
    pharaoh_memory_fill_bytes((uint32_t *)(void *)c->buildings->backup,0x21d68,0);
    pharaoh_memory_fill_bytes((uint32_t *)(void *)c->buildings->ids,0x41a,0);
    for(int32_t id=1;id<4000;++id){
        const uint8_t state=c->buildings->live[id].state;
        if(state==2){*c->buildings->available=0;return 0;}
        if(state==6) *c->buildings->available=0;
    }
    pharaoh_memory_copy_forward_words64(c->live_images,c->saved_images,0x32c40);
    pharaoh_memory_copy_forward_words64(c->live_terrain,c->saved_terrain,0x32c40);
    for(int32_t i=0;i<4;++i)
        pharaoh_memory_copy_forward_words64(c->live_bytes[i],c->saved_bytes[i],0xcb10);
    return 1;
}
