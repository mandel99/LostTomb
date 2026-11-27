#include "pharaoh/population/displaced.h"

_Static_assert(sizeof(PharaohWalkerQueryRecord)==388 &&
    offsetof(PharaohWalkerQueryRecord,wait_ticks)==0x36 &&
    offsetof(PharaohWalkerQueryRecord,action_state)==0x38 &&
    offsetof(PharaohWalkerQueryRecord,displaced_people)==0x64,
    "native displaced actor fields");

/* Complete native00459020, reused from the whole DisplacedPopulation C. */
void pharaoh_city_spawn_displaced_population(
    const PharaohDisplacedPopulationContext *c,int32_t player,
    int32_t x,int32_t y,int32_t people)
{
    c->walker_create->tile_grid_base_offset=*c->map_origin;
    const int32_t id=pharaoh_walker_create(
        c->walker_create,(uint8_t)player,3,x,y,0);
    PharaohWalkerQueryRecord *walker=&c->walker_create->walkers[id];
    walker->action_state=8;
    walker->wait_ticks=0;
    walker->displaced_people=(uint8_t)people;
    PharaohDisplacedPopulationCity *city=&c->cities[player];
    PharaohDisplacedPopulationState view={
        &city->displaced_population,&city->population,city->people_at_age,
        &city->highest_population};
    pharaoh_city_population_remove_homeless(
        &view,people,c->random_pool,*c->random_pool_index);
}
