#include "pharaoh/walker/projectile_path.h"
#include "pharaoh/walker/cross_country_setup.h"

_Static_assert(offsetof(PharaohWalkerQueryRecord,target_walker_id)==0x82,"linked target id");
_Static_assert(offsetof(PharaohWalkerQueryRecord,created_sequence)==0x86,"target sequence");
_Static_assert(offsetof(PharaohWalkerQueryRecord,target_walker_created_sequence)==0x88,"expected sequence");

/* Whole mixed/artifacts/actor_civic_followup/ActorRouteLeaves.c algorithm. */
void pharaoh_walker_cross_country_update_error(volatile PharaohWalkerQueryRecord *r)
{
    const uint8_t axis=r->cross_country_setup.axis;
    uint16_t error=(uint16_t)r->cross_country_setup.error,component,other;
    if(axis==1){
        component=(uint16_t)r->cross_country_setup.delta_y;
        if((int16_t)error<0)error=(uint16_t)((uint32_t)error+(uint32_t)component*2u);
        else{
            other=(uint16_t)r->cross_country_setup.delta_x;
            component=(uint16_t)(component-other);
            error=(uint16_t)((uint32_t)error+(uint32_t)component*2u);
        }
        r->cross_country_setup.error=(int16_t)error;
        r->cross_country_setup.delta_x=(int16_t)((uint16_t)r->cross_country_setup.delta_x-1u);
    }else{
        component=(uint16_t)r->cross_country_setup.delta_x;
        if((int16_t)error<0)error=(uint16_t)((uint32_t)error+(uint32_t)component*2u);
        else{
            other=(uint16_t)r->cross_country_setup.delta_y;
            component=(uint16_t)(component-other);
            error=(uint16_t)((uint32_t)error+(uint32_t)component*2u);
        }
        r->cross_country_setup.error=(int16_t)error;
        r->cross_country_setup.delta_y=(int16_t)((uint16_t)r->cross_country_setup.delta_y-1u);
    }
}

/* Native captures each current/target WORD once, before the signed compare. */
static void step_pixel(volatile int16_t *current,const volatile int16_t *target)
{
    const int16_t from=*current,to=*target;
    if(from<to)*current=(int16_t)((uint16_t)from+1u);
    else if(from>to)*current=(int16_t)((uint16_t)from-1u);
}

void pharaoh_walker_cross_country_step_y(volatile PharaohWalkerQueryRecord *r)
{
    step_pixel(&r->cross_country_y,&r->cross_country_setup.target_y);
}

void pharaoh_walker_cross_country_step_x(volatile PharaohWalkerQueryRecord *r)
{
    step_pixel(&r->cross_country_x,&r->cross_country_setup.target_x);
}

/* Reuse whole old Targeting.c path algorithm. Native LEA multiplication and
 * map-address additions wrap32 before their WORD or pointer consumers. */
uint32_t pharaoh_walker_projectile_path_clear(const PharaohProjectilePathContext *c,
    int32_t x,int32_t y,int32_t target_x,int32_t target_y)
{
    PharaohWalkerQueryRecord *scratch=&c->walkers[0];
    const int32_t base=*c->tile_grid_base_offset;
    const int32_t from_x=(int32_t)((uint32_t)x*15u);
    const int32_t from_y=(int32_t)((uint32_t)y*15u);
    int32_t grace=0;
    uint32_t grid=(uint32_t)base+(uint32_t)y*228u+(uint32_t)x;
    scratch->cross_country_x=(int16_t)from_x;
    scratch->cross_country_y=(int16_t)from_y;
    if((c->terrain[grid]&0xc000u)!=0)grace=6;
    pharaoh_walker_cross_country_setup(scratch,from_x,from_y,
        (int32_t)((uint32_t)target_x*15u),(int32_t)((uint32_t)target_y*15u),0);
    for(int32_t segment=0;segment<1000;++segment){
        for(int32_t step=0;step<8;++step){
            if((int32_t)scratch->cross_country_setup.delta_x+
                (int32_t)scratch->cross_country_setup.delta_y<=0)return 1;
            pharaoh_walker_cross_country_update_error(scratch);
            if(scratch->cross_country_setup.axis==2){
                pharaoh_walker_cross_country_step_y(scratch);
                if(scratch->cross_country_setup.error>=0){
                    scratch->cross_country_setup.delta_x=(int16_t)(
                        (uint16_t)scratch->cross_country_setup.delta_x-1u);
                    pharaoh_walker_cross_country_step_x(scratch);
                }
            }else{
                pharaoh_walker_cross_country_step_x(scratch);
                if(scratch->cross_country_setup.error>=0){
                    scratch->cross_country_setup.delta_y=(int16_t)(
                        (uint16_t)scratch->cross_country_setup.delta_y-1u);
                    pharaoh_walker_cross_country_step_y(scratch);
                }
            }
        }
        scratch->x=(int16_t)(scratch->cross_country_x/15);
        scratch->y=(int16_t)(scratch->cross_country_y/15);
        if(grace>0){--grace;continue;}
        grid=(uint32_t)*c->tile_grid_base_offset+(uint32_t)(int32_t)scratch->y*228u+
            (uint32_t)(int32_t)scratch->x;
        const uint32_t terrain=c->terrain[grid];
        if((terrain&0xc001u)!=0)return 0;
        if((terrain&8u)!=0&&(c->bitfields[grid]&0x0fu)!=0)return 0;
    }
    return 0;
}

uint32_t pharaoh_walker_linked_target_valid(const PharaohWalkerQueryRecord *walkers,
    int32_t walker_id)
{
    const PharaohWalkerQueryRecord *walker=&walkers[walker_id];
    const int32_t target_id=walker->target_walker_id;
    if(target_id<=0)return 0;
    const PharaohWalkerQueryRecord *target=&walkers[target_id];
    if(target->state!=1||target->action_state==4)return 0;
    return target->created_sequence==walker->target_walker_created_sequence;
}
