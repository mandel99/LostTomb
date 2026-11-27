#include "pharaoh/map/bridge_removal.h"

static int bridge_cell(const PharaohBridgeRemovalContext *c,uint32_t cell)
{
    return (c->terrain[cell]&4u)!=0 && c->pieces[cell]!=0;
}

static uint32_t bridge_direction(
    const PharaohBridgeRemovalContext *c,uint32_t cell)
{
    static const uint32_t offsets[8]={
        UINT32_C(0xffffff1c),UINT32_C(0xfffffe38),228,456,
        1,2,UINT32_MAX,UINT32_C(0xfffffffe)};
    uint32_t vertical=0,horizontal=0;
    for (uint32_t i=0;i<8;++i) {
        if (bridge_cell(c,cell+offsets[i])) {
            if (i<4) ++vertical;
            else ++horizontal;
        }
    }
    return vertical>=horizontal?228u:1u;
}

static uint32_t bridge_beginning(
    const PharaohBridgeRemovalContext *c,uint32_t cell,uint32_t step)
{
    while (bridge_cell(c,cell-step)) cell-=step;
    return cell;
}

/* Complete native0047F810, reused from the whole ConstructionLines C. */
void pharaoh_map_bridge_mark_or_remove(
    const PharaohBridgeRemovalContext *c,uint32_t cell,uint32_t mark)
{
    if (!bridge_cell(c,cell)) return;
    const uint32_t step=bridge_direction(c,cell);
    cell=bridge_beginning(c,cell,step);
    for (;;) {
        if (mark!=0) c->property_flags[cell]|=0x40u;
        else {
            const uint32_t flags=c->terrain[cell];
            c->pieces[cell]=0;
            c->terrain[cell]=flags&0xffffffbfu;
        }
        const uint32_t next=cell+step;
        if (!bridge_cell(c,next)) break;
        cell=next;
    }
}

/* Complete native0047F9B0, including its preview-clearing side effect. */
uint32_t pharaoh_map_bridge_count_occupants(
    const PharaohBridgeRemovalContext *c,uint32_t cell)
{
    if (!bridge_cell(c,cell)) return 0;
    const uint32_t step=bridge_direction(c,cell);
    cell=bridge_beginning(c,cell,step);
    uint32_t count=0;
    for (;;) {
        if (c->actor_ids[cell]!=0) ++count;
        c->property_flags[cell]&=0xbfu;
        const uint32_t next=cell+step;
        if (!bridge_cell(c,next)) break;
        cell=next;
    }
    return count;
}
