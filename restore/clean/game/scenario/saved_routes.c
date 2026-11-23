#include "pharaoh/scenario/saved_routes.h"
#include "pharaoh/core/x87_numeric.h"

_Static_assert(sizeof(PharaohRouteMapObject)==98 &&
    offsetof(PharaohRouteMapObject,image)==12,"native route map object");
_Static_assert(sizeof(PharaohRouteImage)==64 &&
    offsetof(PharaohRouteImage,width)==12,"native route image view");

/* Reused WorldRouteGeometry C. Possible original bug: the Chebyshev estimate
 * plus50 can be smaller than the Euclidean sample count at spacing1. Retain
 * the native allocation and guard rather than inventing another policy. */
uint8_t pharaoh_saved_route_rebuild(PharaohEditorRoute *r,
    const PharaohSavedRouteBindings *c)
{
    int32_t estimate=0;
    uint32_t used=0;
    for(uint32_t i=1;i<r->point_count;++i) {
        int32_t dx=(int32_t)r->points[i].x-r->points[i-1].x;
        int32_t dy=(int32_t)r->points[i].y-r->points[i-1].y;
        if(dx<0)dx=-dx;
        if(dy<0)dy=-dy;
        estimate=(int32_t)((uint32_t)estimate+1u+(uint32_t)(dx>dy?dx:dy));
    }
    const int32_t capacity=r->capacity<estimate ?
        (int32_t)((uint32_t)estimate+50u):r->capacity;
    if(capacity>r->capacity) {
        if(r->samples!=NULL) {
            c->release(c->user_data,r->samples);
            r->samples=NULL;
        }
        r->samples=c->allocate(c->user_data,(uint32_t)capacity*6u);
        r->capacity=capacity;
    }
    r->sample_count=estimate;
    for(uint32_t i=1;i<r->point_count;++i) {
        const int32_t x=r->points[i-1].x,y=r->points[i-1].y;
        const int32_t dx=(int32_t)r->points[i].x-x;
        const int32_t dy=(int32_t)r->points[i].y-y;
        uint32_t distance=0,emitted=0;
        if((int32_t)used>r->sample_count)return 0;
        const uint32_t length=pharaoh_x87_route_length(dx,dy);
        if((length&0x7fffffffu)!=0 && r->spacing!=0) {
            const uint32_t ratio_y=pharaoh_x87_route_ratio(dy,length);
            const uint32_t ratio_x=pharaoh_x87_route_ratio(dx,length);
            if(pharaoh_x87_route_compare(0,length)&0x4100u) {
                do {
                    PharaohFloat80 value;
                    pharaoh_x87_route_coordinate(x,ratio_x,(int32_t)distance,&value);
                    r->samples[used+emitted].x=(int16_t)pharaoh_x87_truncate80(&value);
                    pharaoh_x87_route_coordinate(y,ratio_y,(int32_t)distance,&value);
                    r->samples[used+emitted].y=(int16_t)pharaoh_x87_truncate80(&value);
                    r->samples[used+emitted].segment=(uint8_t)(i-1);
                    distance+=r->spacing;
                    ++emitted;
                } while(pharaoh_x87_route_compare((int32_t)distance,length)&0x4100u);
            }
        }
        used+=emitted;
    }
    r->sample_count=(int32_t)used;
    r->first_city=-1;
    r->last_city=-1;
    if(r->point_count!=0) {
        const PharaohEditorRoutePoint *first=&r->points[0];
        const PharaohEditorRoutePoint *last=&r->points[r->point_count-1u];
        for(int32_t i=0;i<200;++i) {
            const PharaohRouteMapObject *o=&c->objects[i];
            if(o->kind!=1 || o->in_use==0)continue;
            const int32_t image=o->image<c->image_count?o->image:0;
            const int32_t right=o->x+c->images[image].width;
            const int32_t bottom=o->y+c->images[image].height;
            if(first->x>=o->x && first->x<=right && first->y>=o->y && first->y<=bottom)
                r->first_city=(int16_t)i;
            if(last->x>=o->x && last->x<=right && last->y>=o->y && last->y<=bottom)
                r->last_city=(int16_t)i;
        }
    }
    return 1;
}

static void release_samples(void *data,void *samples)
{
    PharaohSavedRouteBindings *c=data;
    c->release(c->user_data,samples);
}
static void rebuild_samples(void *data,PharaohEditorRoute *route)
{
    (void)pharaoh_saved_route_rebuild(route,data);
}
void pharaoh_saved_game_bind_route_samples(PharaohSavedGameReadOperations *ops,
    PharaohSavedRouteBindings *bindings)
{
    ops->free_route_samples=release_samples;
    ops->rebuild_route=rebuild_samples;
    ops->route_user_data=bindings;
}
