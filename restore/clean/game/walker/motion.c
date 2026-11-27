#include "pharaoh/walker/contact.h"
#include "pharaoh/walker/projectile_path.h"

_Static_assert(offsetof(PharaohWalkerQueryRecord,previous_x)==0x18,"old coordinate X");
_Static_assert(offsetof(PharaohWalkerQueryRecord,previous_y)==0x1a,"old coordinate Y");
_Static_assert(offsetof(PharaohWalkerQueryRecord,cross_country_terrain_timer)==0x40,"terrain timer");
_Static_assert(offsetof(PharaohWalkerQueryRecord,combat_delay)==0x6a,"contact BYTE");
_Static_assert(offsetof(PharaohWalkerQueryRecord,high_layer_077)==0x77,"height timer");
_Static_assert(offsetof(PharaohWalkerQueryRecord,target_height)==0x79,"target height");

/* Whole3FF0 from old mixed ActorRoutePixels C; all movement children real C. */
int32_t pharaoh_walker_cross_country_advance(const PharaohWalkerMotionContext *c,
    int32_t id,int32_t steps)
{
    PharaohWalkerQueryRecord *a=&c->tile_chain->walkers[id];
    int32_t arrived=0;
    pharaoh_walker_detach_from_tile(c->tile_chain,id);
    for(int32_t step=0;step<steps;++step){
        const uint8_t phase=a->missile_damage;
        a->missile_damage=phase?(uint8_t)(phase-1u):0;
        if((int32_t)a->cross_country_setup.delta_y+(int32_t)a->cross_country_setup.delta_x<=0){arrived=1;break;}
        pharaoh_walker_cross_country_update_error(a);
        if(a->cross_country_setup.axis==2){
            pharaoh_walker_cross_country_step_y(a);
            if(a->cross_country_setup.error>=0){
                a->cross_country_setup.delta_x=(int16_t)((uint16_t)a->cross_country_setup.delta_x-1u);
                pharaoh_walker_cross_country_step_x(a);
            }
        }else{
            pharaoh_walker_cross_country_step_x(a);
            if(a->cross_country_setup.error>=0){
                a->cross_country_setup.delta_y=(int16_t)((uint16_t)a->cross_country_setup.delta_y-1u);
                pharaoh_walker_cross_country_step_y(a);
            }
        }
    }
    a->x=(int16_t)(a->cross_country_x/15);
    const int32_t y=a->cross_country_y/15;
    const uint32_t base=(uint32_t)*c->tile_grid_base_offset;
    a->y=(int16_t)y;
    const uint32_t tile=base+(uint32_t)(int32_t)a->x+(uint32_t)y*228u;
    a->grid_offset=(int32_t)tile;
    /* Native is BYTE at terrain+tile*4; the pool's pointer unit stays DWORD. */
    if((*(const uint8_t *)&c->terrain[tile]&8u)!=0)a->cross_country_terrain_timer=8;
    else{
        const uint8_t timer=a->cross_country_terrain_timer;
        if(timer!=0)a->cross_country_terrain_timer=(uint8_t)(timer-1u);
    }
    pharaoh_walker_attach_to_tile(c->tile_chain,id);
    return arrived;
}

/* Whole old bridge-height C algorithm, inlined by Native3C60. No second owner. */
static uint8_t bridge_height(uint8_t sprite)
{
    switch(sprite){
    case 0:return 0;
    case 1:case 4:return 15;
    case 2:return 27;
    case 3:case 6:case 7:case 8:case 9:case 10:return 14;
    case 5:case 13:return 30;
    default:return 36;
    }
}

void pharaoh_walker_move_to_next_tile(const PharaohWalkerMotionContext *c,int32_t id)
{
    static const int8_t dx[8]={0,1,1,1,0,-1,-1,-1};
    static const int8_t dy[8]={-1,-1,0,1,1,1,0,-1};
    static const int32_t delta[8]={-228,-227,1,229,228,227,-1,-229};
    PharaohWalkerQueryRecord *a=&c->tile_chain->walkers[id];
    const int16_t old_x=a->x,old_y=a->y;
    uint32_t direction,tile,terrain;
    uint8_t kind,sprite;
    if((int8_t)a->direction>=8)return;
    pharaoh_walker_detach_from_tile(c->tile_chain,id);
    direction=(uint32_t)(int32_t)(int8_t)a->direction;
    if(direction>7u)return;
    if(dy[direction])a->y=(int16_t)((uint16_t)a->y+dy[direction]);
    if(dx[direction])a->x=(int16_t)((uint16_t)a->x+dx[direction]);
    a->grid_offset=(int32_t)((uint32_t)a->grid_offset+(uint32_t)delta[direction]);
    pharaoh_walker_attach_to_tile(c->tile_chain,id);
    tile=(uint32_t)a->grid_offset;terrain=c->terrain[tile];
    if((terrain&0x40000040u)==0)a->is_on_road=0;
    else{
        a->is_on_road=1;
        if((terrain&4u)!=0&&(sprite=c->bridge_sprites[tile])!=0){
            /* Query records start atBCD5F8; movement views start eight bytes later. */
            a->high_layer_077=18;a->target_height=bridge_height(sprite);
        }
    }
    kind=a->type;
    if(kind!=77&&kind!=78&&kind!=92&&kind!=93&&kind!=100&&kind!=101){
        tile=(uint32_t)a->grid_offset;
        pharaoh_walker_contact_chain(c->contact,id,c->contact->grid_heads[tile]);
    }
    a->previous_x=old_x;a->previous_y=old_y;
}
