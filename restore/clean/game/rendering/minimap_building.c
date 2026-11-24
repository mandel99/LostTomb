#include "pharaoh/rendering/minimap_building.h"
#include "pharaoh/rendering/overlay_metrics.h"
#include <string.h>

static int32_t add(int32_t a,int32_t b)
{return (int32_t)((uint32_t)a+(uint32_t)b);}
static void cell(PharaohMinimap *m,int32_t image,int32_t x,int32_t y)
{m->draw_image(m->image_user_data!=0?m->image_user_data:m->user_data,image,x,y);}

/* Complete Unit00563 controller00439980; explicit required child metric
 * providers retain the native switch boundaries. It is not a fallback UI. */
int32_t pharaoh_minimap_draw_building(PharaohMinimapBuilding *c,int32_t id,int32_t x,int32_t y)
{
    PharaohMinimap *m=c->map;
    void *const metric_data=c->metric_user_data!=0?c->metric_user_data:c->user_data;
    if((m->edges[m->tile]&0x40u)==0 || (m->terrain[m->tile]&8u)==0) return 0;
    const int32_t mode=*m->overlay;
    int32_t image;
    switch(mode) {
    case 4:case 8:case 10:case 11:case 12:case 13:case 14:case 15:case 16:
    case 17:case 18:case 19:case 21:case 22:case 23:case 24:case 25:case 26:
    case 29:case 30:case 31:case 43:case 45:case 48:case 49:case 50:
        image=c->height[mode](metric_data,id);break;
    case 9:
        image=(m->terrain[m->tile]&0x4140u)!=0?
            c->terrain_damage(metric_data,(int32_t)m->tile):c->height[9](metric_data,id);break;
    case 20:image=0;break; /*0047D350 is native XOR EAX,EAX; RET*/
    case 27:case 28:image=c->height[mode](metric_data,(int32_t)m->tile);break;
    case 33:case 34:case 35:case 36:case 37:case 38:case 39:case 40:case 41:case 42: {
        static const int32_t resource[10]={1,3,6,2,4,5,13,19,17,15};
        static const int32_t slot[10]={0,2,5,1,3,4,9,11,10,8};
        image=c->resource_height(metric_data,id,resource[mode-33],slot[mode-33]);break;
    }
    default:
        image=*c->house_image;
        if(c->buildings[id].house_size==0) {
            if(c->buildings[id].building_type==90) image=add(*c->garden_image,-1);
            else {
                const int32_t kind=pharaoh_building_type_overlay_tile_index(c->type_image_ids,c->buildings[id].building_type);
                if(kind<0) return 0;
                image=add(*c->special_image,(int32_t)((uint32_t)kind*5u));
            }
        }
        {int16_t value;memcpy(&value,(const uint8_t *)&c->buildings[id]+0x4a,2);
            *c->tint=value!=0?0xf863u:0;}
        break;
    }
    if(*m->overlay!=0 && *m->overlay!=51) {
        if(image>10) image=10;
        image=add(*c->overlay_image,(int32_t)((uint32_t)image*5u));
    }
    const int32_t type=c->buildings[id].building_type;
    if((type==31 || type==30 || type==33 || type==209) && (m->terrain[m->tile]&0x40u)!=0)
        image=*c->building_image;
    const uint32_t footprint=c->footprints[m->tile]&15u;
    if(footprint==0) cell(m,image,x,y);
    else if(footprint<=4) c->draw_sprite(c->user_data,add(image,(int32_t)footprint),x,add(y,-(int32_t)footprint));
    else if(footprint==5) {
        image=add(image,2);
        c->draw_sprite(c->user_data,image,add(x,3),add(y,-5));
        c->draw_sprite(c->user_data,image,add(x,3),add(y,1));
        c->draw_sprite(c->user_data,image,x,add(y,-2));
        c->draw_sprite(c->user_data,image,add(x,6),add(y,-2));
    }
    *c->tint=0;return 1;
}
static int32_t building(void *data,int32_t id,int32_t x,int32_t y)
{return pharaoh_minimap_draw_building(data,id,x,y);}
void pharaoh_minimap_bind_buildings(PharaohMinimap *m,PharaohMinimapBuilding *c)
{c->map=m;m->draw_building=building;m->building_user_data=c;}
