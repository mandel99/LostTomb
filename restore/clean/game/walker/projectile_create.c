#include "pharaoh/walker/projectile_create.h"
#include "pharaoh/walker/cross_country_setup.h"
#include "pharaoh/core/projectile_angle.h"

_Static_assert(offsetof(PharaohWalkerQueryRecord,missile_damage)==0x1c,"projectile damage");
_Static_assert(offsetof(PharaohWalkerQueryRecord,owner_walker_id)==0x5a,"projectile owner alias");

/* Whole parent reused from old game/Walkers/Combat/Targeting.c. */
int32_t pharaoh_walker_projectile_create(PharaohProjectileCreateContext *c,
    int32_t owner,int32_t x,int32_t y,int32_t target_x,int32_t target_y,int32_t kind)
{
    const int32_t id=pharaoh_walker_create(c->walker_create,
        (uint8_t)*c->current_player,kind,x,y,0);
    PharaohWalkerQueryRecord *projectile;
    if(id==0)return 0;
    pharaoh_walker_combat_sound(c->sound,owner);
    projectile=&c->walker_create->walkers[id];
    projectile->missile_damage=kind==0x3d?60:10;
    projectile->owner_walker_id=(int16_t)owner;
    projectile->destination_x=(int16_t)target_x;
    projectile->destination_y=(int16_t)target_y;
    pharaoh_walker_cross_country_setup(projectile,
        projectile->cross_country_x,projectile->cross_country_y,
        projectile->destination_x*15,projectile->destination_y*15,1);
    projectile->direction=pharaoh_projectile_angle(
        projectile->x,projectile->y,projectile->destination_x,projectile->destination_y,c->math);
    return id;
}
