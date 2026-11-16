#include "pharaoh/military/enemy_ship_queries.h"
int32_t pharaoh_enemy_ship_facing_direction(const PharaohEnemyShipWalker *w,const PharaohEnemyShipRules *r)
{ return w->direction>r->direction_top_left?w->previous_tile_direction:w->direction; }
void pharaoh_enemy_ship_pick_escape_point(PharaohEnemyShipContext *c,int32_t *x,int32_t *y,const int32_t dx[3],const int32_t dy[3],const PharaohEnemyShipWalker *w)
{
    int32_t i,ox=0,oy=0,best=0;
    *x=w->x;*y=w->y;
    for(i=0;i<3;++i){int32_t d=c->water_farthest_on_line(c->user_data,w->x,w->y,w->x+dx[i],w->y+dy[i],&ox,&oy);if((i==0&&d>-1)||(i>0&&best<=d)){*x=ox;*y=oy;best=d;}}
    c->water_find_free_tile(c->user_data,x,y);
}
void pharaoh_enemy_ship_play_ram_sound(PharaohEnemyShipContext *c,int32_t divisor)
{ if(divisor>=1&&divisor<=c->rules->ram_divisor_max)c->play_sound(c->user_data,c->rules->ram_sound_first-1+divisor); }
int32_t pharaoh_enemy_ship_find_ship_at(PharaohEnemyShipContext *c,int32_t grid,int32_t excluded)
{
    int32_t id=c->walker_at_grid(c->user_data,grid);
    while(id){const PharaohEnemyShipWalker *w=c->walker(c->user_data,id);if(!w)return 0;if(id!=excluded&&(w->type==c->rules->transport_type||w->type==c->rules->warship_type||w->type==c->rules->fishing_boat_type))return id;id=w->next_walker_on_tile;}
    return 0;
}
int32_t pharaoh_enemy_ship_find_ram_target(PharaohEnemyShipContext *c,const PharaohEnemyShipWalker *w)
{
    int32_t d=w->direction,grid,id;
    if((uint8_t)d>c->rules->direction_top_left)return 0;
    grid=c->rules->grid_start+w->x+w->y*c->rules->grid_size;
    id=pharaoh_enemy_ship_find_ship_at(c,grid,w->id);
    if(id)return id;
    return pharaoh_enemy_ship_find_ship_at(c,grid+c->rules->grid_offsets_by_direction[d],w->id);
}
void pharaoh_enemy_ship_reroute_tick(int32_t *reroute_ticks,int32_t last_tick,
    void (*remove_route)(void *,int32_t),void *user_data,int32_t walker_id)
{
    if(++*reroute_ticks>last_tick){*reroute_ticks=0;remove_route(user_data,walker_id);}
}
