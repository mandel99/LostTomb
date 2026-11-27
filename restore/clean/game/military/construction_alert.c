#include "pharaoh/military/construction_alert.h"
#include "pharaoh/map/terrain_queries.h"

static uint32_t alert_delta(int32_t a,int32_t b)
{
    return a>b ? (uint32_t)a-(uint32_t)b : (uint32_t)b-(uint32_t)a;
}

void pharaoh_construction_alert_formations(
    const PharaohConstructionAlertContext *c,int32_t x,int32_t y)
{
    for(uint32_t index=1;index<50;++index){
        PharaohConstructionAlertFormation *formation=&c->formations[index];
        if(formation->state!=1 || formation->native_42!=0 ||
            formation->native_69==0 || formation->native_28==0)continue;
        int32_t gx=formation->x,gy=formation->y;
        const uint32_t dx=alert_delta(x,gx);
        *c->distance_x=dx;
        const uint32_t dy=alert_delta(y,gy);
        *c->distance_y=dy;
        const uint32_t distance=(int32_t)dx>=(int32_t)dy ? dx : dy;
        if((int32_t)distance>6)continue;
        formation->alert_ticks=50;
        gy=formation->y;gx=formation->x;
        formation->direction=(uint8_t)pharaoh_map_general_direction(x,y,gx,gy);
    }
}
