#include "pharaoh/rendering/city_top.h"
#include "pharaoh/rendering/upper_sprite_draw.h"
#include "pharaoh/core/static_objects.h"
#include <stddef.h>
_Static_assert(offsetof(PharaohBuildingTypeRecord,render_warning_087)==0x87,"native warning byte");

static int32_t S(uint32_t bits){return (int32_t)bits;}
static void draw_top(PharaohCityTopContext *c,int32_t image,int32_t x,int32_t y){const PharaohUpperSpriteDrawContext top={c->draw->images,c->draw->image_pixels,*c->draw->image_count,c->draw->clip,c->draw->surface_pixels,c->draw->pitch_bytes,*c->draw->tint,&c->draw->state->source.pixels};(void)pharaoh_image_draw_isometric_top(&top,image,x,y);}

static int32_t building_id(PharaohCityTopContext *c,uint32_t tile)
{
    return (int16_t)((uint16_t)c->ground->building_ids[tile]);
}
static int neighbor_occludes(PharaohCityTopContext *c,uint32_t tile)
{

    int32_t building;
    int32_t id;
    if(S(tile)>=0x0fff0000)return 0;
    building=building_id(c,tile);
    if(((uint16_t)c->ground->buildings[building].building_type)==183 && (c->ground->buildings[building].pyramid_work_column)!=0 &&
       ((uint32_t)c->ground->buildings[building].origin_x)!=0 && (c->ground->buildings[building].pyramid_ramp1_height)<=3)return 1;
    id=(int16_t)((uint16_t)c->ground->walker_ids[tile]);
    while(id>0){

        if(c->ground->walkers[id].is_ghost==0){
            switch(c->ground->walkers[id].type){
            case 13:case 45:case 50:case 56:case 57:return 1;
            default:break;
            }
        }
        id=(int16_t)(uint16_t)c->ground->walkers[id].next_walker_id_on_same_tile;
    }
    return 0;
}
static int upper_is_occluded(PharaohCityTopContext *c,uint32_t grid_index,uint32_t alternate)
{
    uint32_t first,second;
    if(alternate){first=(c->ground->neighbor_cells[grid_index]);second=(c->ground->neighbor_cells[grid_index+1u]);}
    else{first=(c->ground->neighbor_cells[grid_index+1u]);second=(c->ground->neighbor_cells[grid_index+2u]);}
    if(neighbor_occludes(c,first))return 1;
    return neighbor_occludes(c,second);
}
static int footprint_position(PharaohCityTopContext *c,uint32_t footprint,uint32_t x,uint32_t y,uint32_t *out_x,uint32_t *out_y)
{
    switch(footprint){
    case 0:*out_x=x;*out_y=y;return 1;
    case 1:*out_y=y-(c->ground->scan->tile_y[3]);*out_x=x+(c->ground->scan->tile_x[3])+1u;return 1;
    case 2:*out_y=y-(c->ground->scan->tile_y[2]);*out_x=x+(c->ground->scan->tile_x[2])+2u;return 1;
    case 3:*out_y=y-(c->ground->scan->tile_y[1]);*out_x=x+(c->ground->scan->tile_x[1])+3u;return 1;
    case 4:*out_y=y-(c->ground->scan->tile_y[0]);*out_x=x+(c->ground->scan->tile_x[0])+4u;return 1;
    case 5:*out_y=y-(c->ground->scan->tile_y[0])-(c->ground->scan->tile_y[3]);*out_x=x+(c->ground->scan->tile_x[0])+(c->ground->scan->tile_x[3])+5u;return 1;
    default:return 0;
    }
}
static void base_extras(PharaohCityTopContext *c,int32_t building,uint32_t x,uint32_t y)
{
    uint32_t direction;
    uint16_t state,type;
    if(((uint16_t)c->ground->buildings[building].building_type)==210){
        direction=(*c->orientation);
        if(direction==4||direction==2){state=((uint16_t)c->ground->buildings[building].subtype);if(state==0||state==3)c->operations->draw_0041fff0(c->operations->user_data,x,y);}
        else if(direction==0||direction==6){state=((uint16_t)c->ground->buildings[building].subtype);if(state==2||state==5)c->operations->draw_0041fff0(c->operations->user_data,x,y);}
    }
    type=((uint16_t)c->ground->buildings[building].building_type);
    if(type==207||type==208)c->operations->draw_004e5e90(c->operations->user_data,building,x,y);
    if(((uint16_t)c->ground->buildings[building].building_type)==213&&((uint16_t)c->ground->buildings[building].subtype)==0){c->operations->draw_00542e10(c->operations->user_data,building,x,y);c->operations->draw_00420050(c->operations->user_data,x,y);}
    if(((uint16_t)c->ground->buildings[building].building_type)==225){
        direction=(*c->orientation);
        if((direction==0&&((uint16_t)c->ground->buildings[building].subtype)==6)||(direction==2&&((uint16_t)c->ground->buildings[building].subtype)==5)||
           (direction==4&&((uint16_t)c->ground->buildings[building].subtype)==7)||(direction==6&&((uint16_t)c->ground->buildings[building].subtype)==8))c->operations->draw_004206f0(c->operations->user_data,x,y);
    }
    if(((uint16_t)c->ground->buildings[building].building_type)==227&&((uint16_t)c->ground->buildings[building].subtype)==0)c->operations->draw_00420d60(c->operations->user_data,x,y);
    type=((uint16_t)c->ground->buildings[building].building_type);
    if((type==229||type==234||type==236||type==235)&&(c->ground->buildings[building].complex_progress_flags)!=0&&
       c->ground->walkers[c->ground->buildings[building].monument_worker_id].direction==8)c->operations->draw_00421920(c->operations->user_data,x,y,building);
    if(((uint16_t)c->ground->buildings[building].building_type)==228&&((uint16_t)c->ground->buildings[building].subtype)==0)c->operations->draw_00421060(c->operations->user_data,x,y);
}
static void draw_base(PharaohCityTopContext *c,uint32_t tile,uint32_t index,uint32_t alternate,uint32_t x,uint32_t y,uint32_t *extra)
{
    int32_t building=building_id(c,tile);
    uint32_t footprint,image,px,py,direction;
    uint16_t type;
    if(building!=0&&(c->ground->buildings[building].render_tint_bf)!=0)(*c->ground->tint)=0xf863;
    footprint=(c->ground->footprints[tile])&15u;image=(c->ground->images[tile]);(*c->ground->current_image)=image;
    type=((uint16_t)c->ground->buildings[building].building_type);
    if(type==183){*extra=32;c->operations->draw_pyramid(c->operations->user_data,image,x,y,building);}
    else if(type==230){
        *extra=80;
        if((footprint!=0||((c->ground->terrain[tile])&0xeefed77fu)!=0||!upper_is_occluded(c,index,alternate))&&
           footprint_position(c,footprint,x,y,&px,&py))draw_top(c,image,px,py);
        direction=(*c->orientation);
        if(((direction==0||direction==4)&&((uint16_t)c->ground->buildings[building].subtype)==0)||
           ((direction==2||direction==6)&&((uint16_t)c->ground->buildings[building].subtype)==1)){
            c->operations->draw_abu_simbel(c->operations->user_data,building,x,y);c->operations->draw_00420a30(c->operations->user_data,x,y);
        }
    }else{
        if(S(image)>=S(pharaoh_sprite_group_first_image(c->animations->groups,743)+48u)&&S(image)<S(pharaoh_sprite_group_first_image(c->animations->groups,743)+72u)){
            pharaoh_image_draw(c->draw,image,x,y-150u);draw_top(c,(*c->ground->current_image),x,y-150u);
        }else{
            if((footprint!=0||((c->ground->terrain[tile])&0xeefed77fu)!=0||!upper_is_occluded(c,index,alternate))&&
               footprint_position(c,footprint,x,y,&px,&py)){
                draw_top(c,image,px,py);
                if(footprint==2&&(uint8_t)c->operations->is_farm(c->operations->user_data,building)!=0){
                    footprint_position(c,2,x,y,&px,&py);c->operations->draw_farm(c->operations->user_data,building,px,py,1);
                }
            }
            if((c->ground->buildings[building].house_size)!=0){
                if((c->ground->buildings[building].render_warning_087)==1)pharaoh_image_draw(c->draw,pharaoh_sprite_group_first_image(c->animations->groups,366),x+18u,y-32u);
                if((c->ground->buildings[building].house_size)!=0&&(c->ground->buildings[building].render_warning_087)==2)pharaoh_image_draw(c->draw,pharaoh_sprite_group_first_image(c->animations->groups,742),x+18u,y-32u);
            }
        }
    }
    base_extras(c,building,x,y);
    (*c->ground->tint)=0;
}

static void detail_animation(PharaohCityTopContext *c,uint32_t image,uint32_t frames,int32_t building,uint32_t footprint,uint32_t x,uint32_t y)
{
    uint32_t base,variant,group,px,py,top,height,left,chosen;
    const PharaohSpriteImageRecord *record;
    int32_t actor;
    static const int32_t offsets[16]={132,-7,115,25,60,20,49,-4,152,-13,123,16,65,14,38,-12};
    switch((int16_t)((uint16_t)c->ground->buildings[building].building_type)){
    case 71:case 191:
        base=pharaoh_sprite_group_first_image(c->animations->groups,624);pharaoh_building_draw_sprite_at_offset(c->animations,base-1u+frames,x,y,113,3);break;
    case 72:case 193:
        base=pharaoh_sprite_group_first_image(c->animations->groups,628);pharaoh_building_draw_sprite_at_offset(c->animations,base-1u+frames,x,y,22,24);
        base=pharaoh_sprite_group_first_image(c->animations->groups,351);pharaoh_image_draw(c->draw,base+17u,x-4u,y-42u);break;
    case 90:
        base=pharaoh_sprite_group_first_image(c->animations->groups,547);variant=(image-base)&3u;
        switch(variant){
        case 0:group=578;px=4;py=0;break;
        case 1:group=579;px=3;py=0xfffffffeu;break;
        case 2:group=580;px=12;py=0;break;
        default:group=581;px=15;py=6;break;
        }
        base=pharaoh_sprite_group_first_image(c->animations->groups,group);pharaoh_building_draw_sprite_at_offset(c->animations,base-1u+frames,x,y,px-8u+(c->ground->scan->tile_x[3])*2u,py+(c->ground->scan->tile_y[3]));break;
    case 75:case 192:
        actor=(int16_t)((uint16_t)c->ground->buildings[building].runtime_value_b4);base=pharaoh_sprite_group_first_image(c->animations->groups,546);variant=(image-base)&3u;chosen=(variant-1u)&3u;
        group=632;
        if(actor!=0&&c->ground->walkers[actor].action_state==10){variant+=4;group=633;if(S(frames)>20)frames=20;}
        base=pharaoh_sprite_group_first_image(c->animations->groups,group);
        pharaoh_building_draw_sprite_at_offset(c->animations,base+chosen-4u+frames*4u,x,y,offsets[variant*2u],offsets[variant*2u+1u]);break;
    case 74:pharaoh_building_draw_shipwright_animation(c->animations,image,frames,x,y,((uint16_t)c->ground->buildings[building].runtime_value_b4));break;
    case 76:pharaoh_building_draw_fishing_wharf_animation(c->animations,image,frames,x,y,(int16_t)((uint16_t)c->ground->buildings[building].runtime_value_b4));break;
    case 161:case 162:case 217:pharaoh_building_draw_mine_animation(c->animations,image,frames,x,y);break;
    case 106:case 107:case 216:case 221:pharaoh_building_draw_quarry_animation(c->animations,image,frames,x,y);break;
    case 30:pharaoh_building_draw_bandstand_animation(c->animations,image,frames,x,y);break;
    case 34:pharaoh_building_draw_conservatory_animation(c->animations,image,frames,x,y);break;
    case 31:pharaoh_building_draw_booth_animation(c->animations,image,frames,x,y);break;
    case 33:pharaoh_building_draw_pavilion_animation(c->animations,image,frames,x,y);break;
    case 35:pharaoh_building_draw_dance_school_animation(c->animations,image,frames,x,y);break;
    default:
        if(footprint>5)break;
        record=pharaoh_sprite_image_record(c->draw->images,*c->draw->image_count,(int32_t)image);top=(uint32_t)(int32_t)(int16_t)(uint16_t)record->sprite_offset_y;
        record=pharaoh_sprite_image_record(c->draw->images,*c->draw->image_count,(int32_t)image);height=(uint32_t)(int32_t)(int16_t)(uint16_t)record->height;
        py=top-height;
        switch(footprint){
        case 0:py+=y+30u;break;
        case 1:py=py-(c->ground->scan->tile_y[3])+y+60u;break;
        case 2:py=py-(c->ground->scan->tile_y[2])+y+90u;break;
        case 3:py=py-(c->ground->scan->tile_y[1])+y+120u;break;
        case 4:py=py-(c->ground->scan->tile_y[0])+y+150u;break;
        default:py=py-(c->ground->scan->tile_y[0])-(c->ground->scan->tile_y[3])+y+180u;break;
        }
        record=pharaoh_sprite_image_record(c->draw->images,*c->draw->image_count,(int32_t)image);left=(uint32_t)(int32_t)(int16_t)(uint16_t)record->sprite_offset_x;
        pharaoh_image_draw(c->draw,image+frames,x+left,py);break;
    }
    (*c->ground->tint)=0;
    (void)building;
}
static void draw_detail(PharaohCityTopContext *c,uint32_t tile,uint32_t x,uint32_t y)
{
    uint32_t image,index,frames,footprint;
    int32_t building;
    uint16_t type,state;
    uint8_t edge;
    if(((c->ground->terrain[tile])&0x44u)==0x44u&&(c->sprites[tile])!=0){
        pharaoh_image_draw(c->draw,(uint32_t)(c->sprites[tile])-1u+pharaoh_sprite_group_first_image(c->animations->groups,559),x,y-(c->ground->scan->tile_y[2]));return;
    }
    image=(c->ground->images[tile]);index=S(image)<S((*c->draw->image_count))?image:0;
    if((uint16_t)c->draw->images[index].animation_frames==0){
        building=building_id(c,tile);edge=(c->ground->property_flags[tile]);type=((uint16_t)c->ground->buildings[building].building_type);
        if(type==57){
            if(edge&0x40){
                state=((uint16_t)c->ground->buildings[building].subtype);
                if(state==13){pharaoh_image_draw(c->draw,pharaoh_sprite_group_first_image(c->animations->groups,335)+3u,x+93u,y-20u);tile=(c->ground->scan->tile);}
                else if(state==12){pharaoh_image_draw(c->draw,pharaoh_sprite_group_first_image(c->animations->groups,335)+4u,x+93u,y-20u);tile=(c->ground->scan->tile);}
                else if(state==11){pharaoh_image_draw(c->draw,pharaoh_sprite_group_first_image(c->animations->groups,335)+2u,x+93u,y-20u);tile=(c->ground->scan->tile);}
            }
        }else if(type==202){if(edge&0x40){c->operations->draw_004c0160(c->operations->user_data,building,x,y);tile=(c->ground->scan->tile);}}
        else if(type==71||type==191){c->operations->draw_granary_stores(c->operations->user_data,building,tile,x,y);tile=(c->ground->scan->tile);}
    }else{
        if(((c->ground->property_flags[tile])&0x40)==0)return;
        building=building_id(c,tile);
        if(building!=0&&(c->ground->buildings[building].render_tint_bf)!=0)(*c->ground->tint)=0xf863;
        type=((uint16_t)c->ground->buildings[building].building_type);
        if(type==71||type==191){c->operations->draw_granary_stores(c->operations->user_data,building,tile,x,y);tile=(c->ground->scan->tile);}
        type=((uint16_t)c->ground->buildings[building].building_type);
        if(type==75)image=(c->ground->images[tile]);
        if(type==72||type==193){pharaoh_image_draw(c->draw,pharaoh_sprite_group_first_image(c->animations->groups,351)+17u,x-4u,y-42u);tile=(c->ground->scan->tile);}
        frames=c->operations->animation_offset(c->operations->user_data,image,tile);
        if(S(frames)>0){
            index=S(image)<S((*c->draw->image_count))?image:0;
            if(S(frames)>(int16_t)(uint16_t)c->draw->images[index].animation_frames){
                index=S(image)<S((*c->draw->image_count))?image:0;frames=(uint32_t)(int32_t)(int16_t)(uint16_t)c->draw->images[index].animation_frames;
            }
            tile=(c->ground->scan->tile);footprint=(c->ground->footprints[tile])&15u;
            if((uint8_t)c->operations->is_floodplain_farm(c->operations->user_data,building)!=0){pharaoh_native_noop_00489500();(*c->ground->tint)=0;}
            else detail_animation(c,image,frames,building,footprint,x,y);
        }else (*c->ground->tint)=0;
        tile=(c->ground->scan->tile);
    }
    if((c->ground->property_flags[tile])&0x40)c->operations->draw_building_details(c->operations->user_data,building_id(c,tile),x,y);
}

void pharaoh_city_draw_top(PharaohCityTopContext *c,uint32_t actor_parameter)
{
    uint32_t y=(c->ground->scan->start_y)-(c->ground->scan->tile_y[3])*9u,alternate=0,rows=0,extra=4;
    uint32_t x,row,column,step,index,tile,iteration;
    int32_t id;
    (*c->pass_sequence)=(uint16_t)((*c->pass_sequence)+1u);
    (c->ground->scan->row)=(c->ground->scan->camera_row)-8u;
    if(S((c->ground->scan->visible_rows)+18u)<=0)return;
    do{
        if(S((c->ground->scan->row))>=0&&S((c->ground->scan->row))<457){
            x=(c->ground->scan->start_x);if(alternate)x=x-(c->ground->scan->tile_x[3])-1u;
            step=(c->ground->scan->tile_x[2]);(c->ground->scan->column)=(c->ground->scan->camera_column)-4u;x+=(0xfffffffeu-step)*4u;
            row=(c->ground->scan->row);iteration=0;
            if(S((c->ground->scan->visible_columns)+6u)>=0)do{
                column=(c->ground->scan->column);
                if(S(column)>=0&&S(column)<231){
                    index=row*231u+column;tile=(c->ground->cells[index]);(c->ground->scan->tile)=tile;
                    if(S(tile)<0x0fff0000&&(((*c->ground->overlay)!=51&&(*c->ground->alternate_overlay)==0)||
                       ((c->ground->terrain[tile])&0x20000002u)!=0x20000002u)&&((c->ground->property_flags[tile])&0x40)){
                        draw_base(c,tile,index,alternate,x,y,&extra);row=(c->ground->scan->row);step=(c->ground->scan->tile_x[2]);
                    }
                }
                ++iteration;(c->ground->scan->column)=(c->ground->scan->column)+1u;x+=step+2u;
            }while(S(iteration)<=S((c->ground->scan->visible_columns)+6u));

            /* Actors whose terrain has any bit in the native priority mask. */
            x=(c->ground->scan->start_x);if(alternate)x=x-(c->ground->scan->tile_x[3])-1u;
            x+=(0xfffffffeu-step)*4u;column=(c->ground->scan->camera_column)-4u;(c->ground->scan->column)=column;iteration=0;
            if(S((c->ground->scan->visible_columns)+6u)>=0)do{
                if(S(column)>=0&&S(column)<231){
                    tile=(c->ground->cells[row*231u+column]);(c->ground->scan->tile)=tile;
                    if(S(tile)<0x0fff0000&&((c->ground->terrain[tile])&0xeefed77fu)!=0){
                        id=(int16_t)((uint16_t)c->ground->walker_ids[tile]);
                        while(id>0){

                            if(c->ground->walkers[id].is_ghost==0&&(uint16_t)c->ground->walkers[id].monument_action_120==0){
                                c->operations->draw_walker(c->operations->user_data,id,x,y,actor_parameter);column=(c->ground->scan->column);row=(c->ground->scan->row);
                            }
                            id=(int16_t)(uint16_t)c->ground->walkers[id].next_walker_id_on_same_tile;
                        }
                    }
                }
                ++iteration;++column;(c->ground->scan->column)=column;x+=(c->ground->scan->tile_x[2])+2u;
            }while(S(iteration)<=S((c->ground->scan->visible_columns)+6u));

            /* Building upper animation and state overlays. */
            x=(c->ground->scan->start_x);if(alternate)x=x-(c->ground->scan->tile_x[3])-1u;
            x+=(0xfffffffeu-(c->ground->scan->tile_x[2]))*4u;(c->ground->scan->column)=(c->ground->scan->camera_column)-4u;iteration=0;
            if(S((c->ground->scan->visible_columns)+6u)>=0)do{
                column=(c->ground->scan->column);
                if(S(column)>=0&&S(column)<231){
                    tile=(c->ground->cells[c->ground->scan->row*231u+column]);(c->ground->scan->tile)=tile;
                    if(S(tile)<0x0fff0000)draw_detail(c,tile,x,y);
                }
                ++iteration;(c->ground->scan->column)=(c->ground->scan->column)+1u;x+=(c->ground->scan->tile_x[2])+2u;
            }while(S(iteration)<=S((c->ground->scan->visible_columns)+6u));

            /* Complementary actors follow the upper building pass. */
            x=(c->ground->scan->start_x);if(alternate)x=x-(c->ground->scan->tile_x[3])-1u;
            x+=(0xfffffffeu-(c->ground->scan->tile_x[2]))*4u;column=(c->ground->scan->camera_column)-4u;row=(c->ground->scan->row);(c->ground->scan->column)=column;iteration=0;
            if(S((c->ground->scan->visible_columns)+6u)>=0)do{
                if(S(column)>=0&&S(column)<231){
                    tile=(c->ground->cells[row*231u+column]);(c->ground->scan->tile)=tile;
                    if(S(tile)<0x0fff0000&&((c->ground->terrain[tile])&0xeefed77fu)==0){
                        id=(int16_t)((uint16_t)c->ground->walker_ids[tile]);
                        while(id>0){

                            if(c->ground->walkers[id].is_ghost==0&&(uint16_t)c->ground->walkers[id].monument_action_120==0){
                                c->operations->draw_walker(c->operations->user_data,id,x,y,actor_parameter);column=(c->ground->scan->column);row=(c->ground->scan->row);
                            }
                            id=(int16_t)(uint16_t)c->ground->walkers[id].next_walker_id_on_same_tile;
                        }
                    }
                }
                x+=(c->ground->scan->tile_x[2])+2u;++iteration;++column;(c->ground->scan->column)=column;
            }while(S(iteration)<=S((c->ground->scan->visible_columns)+6u));
        }
        alternate=!alternate;y+=(c->ground->scan->tile_y[3]);++rows;(c->ground->scan->row)=(c->ground->scan->row)+1u;
    }while(S(rows)<S((c->ground->scan->visible_rows)+extra+14u));
}

static void top_step(void *data,int32_t selection)
{
    pharaoh_city_draw_top(data,(uint32_t)selection);
}
void pharaoh_city_render_bind_top(PharaohCityRenderOperations *ops,
    PharaohCityTopContext *context)
{
    ops->draw_top=top_step;
    ops->pass_user_data[PHARAOH_CITY_PASS_TOP]=context;
}
