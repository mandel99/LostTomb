#include "pharaoh/rendering/building_animation_offset.h"

static int32_t image_index(const PharaohBuildingAnimationOffsetContext *c,
    int32_t image)
{return image<c->image_count?image:0;}

static int32_t advance_sprite(PharaohBuildingAnimationOffsetContext *c,
    int32_t image,int32_t tile,int32_t sprite)
{
    const int32_t indexed=image_index(c,image);
    const uint8_t *const raw=(const uint8_t *)(const void *)&c->images[indexed];
    if(c->animation_should_advance[raw[0x3a]]==0)return sprite&0x7f;
    int32_t direction=0,frame;
    if(raw[0x30]!=0) {
        direction=(sprite&0x80)!=0;frame=sprite&0x7f;
        if(direction==0) {
            ++frame;
            if(frame>c->images[indexed].animation_frames) {
                frame=c->images[indexed].animation_frames;direction=1;
            }
        } else {
            --frame;
            if(frame<1) {frame=1;direction=0;}
        }
    } else {
        frame=sprite+1;
        if(frame>c->images[indexed].animation_frames)frame=1;
    }
    c->sprites[tile]=(uint8_t)(frame|(direction?0x80:0));
    return frame;
}

static int32_t workers_then_advance(PharaohBuildingAnimationOffsetContext *c,
    int32_t workers,int32_t image,int32_t tile,int32_t sprite)
{return workers>0?advance_sprite(c,image,tile,sprite):0;}

static int32_t special_type(int32_t type)
{
    if(type>=110&&type<=114)return 1;
    switch(type) {
    case 32:case 47:case 51:case 53:case 74:case 113:
    case 177:case 178:case 187:case 188:case 189:case 194:
    case 203:case 204:case 205:case 226:case 231:case 232:case 233:return 1;
    default:return 0;
    }
}

static int32_t listed_worker_type(int32_t type)
{
    switch(type) {
    case 55:case 81:case 70:case 190:case 72:case 193:case 75:case 192:
    case 161:case 217:case 162:case 106:case 221:case 107:case 216:
    case 108:case 109:case 34:case 35:case 36:case 37:case 71:case 191:return 1;
    default:return 0;
    }
}

/* Complete typed C translation of Pharaoh.exe 0x0040F2C0. */
int32_t pharaoh_building_animation_offset(
    PharaohBuildingAnimationOffsetContext *c,int32_t image,int32_t tile)
{
    int32_t building_id=c->building_ids[tile];
    int32_t sprite=c->sprites[tile];
    if(building_id==0)return advance_sprite(c,image,tile,sprite);
    const PharaohBuildingTypeRecord *b=&c->buildings[building_id];
    int32_t workers=b->num_workers;
    if(c->is_floodplain_farm(c->user_data,building_id)!=0)
        return workers>0?advance_sprite(c,image,tile,sprite):0;
    int32_t type=b->building_type;
    if(type==232) {
        return workers>0&&b->loads_stored>0&&b->secondary_input_stock>0?
            advance_sprite(c,image,tile,sprite):0;
    }
    if(type==92) {
        const int32_t water=c->houses_water_in_radius(c->user_data,
            building_id,2);
        return water==1||water==2?0:1;
    }
    if(type==180||type==90)
        return workers>0&&b->farm_water_access!=0?
            advance_sprite(c,image,tile,sprite):0;
    const int32_t palace=c->is_palace(c->user_data,building_id);
    if(palace==0&&special_type(type)) {
        if(c->cheat_allow_all!=0)
            return workers_then_advance(c,workers,image,tile,sprite);
        int32_t allow_empty=0;
        if(type==74) {
            const uint16_t order=(uint16_t)b->runtime_value_b4;
            if((order&0xf000u)==0x1000u&&(order&0x0fffu)==25u)
                allow_empty=1;
        }
        if(b->loads_stored<=0&&allow_empty==0)return 0;
        if((type==204||type==226)&&b->secondary_input_stock<=0)return 0;
        return workers_then_advance(c,workers,image,tile,sprite);
    }
    if(listed_worker_type(type))
        return workers_then_advance(c,workers,image,tile,sprite);
    if(type==32||(type==226&&palace!=0))
        return workers_then_advance(c,workers,image,tile,sprite);
    if(type==31||type==30||type==33) {
        building_id=c->main_building(c->user_data,building_id);
        b=&c->buildings[building_id];
        if(b->num_workers<=0)return 0;
        const uint8_t show=type==31?b->runtime_value_a7:
            type==30?b->runtime_value_a8:b->temple_upgrade;
        return show!=0?advance_sprite(c,image,tile,sprite):0;
    }
    if(type==210)return advance_sprite(c,image,tile,sprite);
    if(type==228)return b->monument_stage==9?
        advance_sprite(c,image,tile,sprite):0;
    if(workers>0||c->models[type].laborers==0)
        return advance_sprite(c,image,tile,sprite);
    return 0;
}
