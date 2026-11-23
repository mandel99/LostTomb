#include "pharaoh/rendering/city_ground.h"
#include "pharaoh/core/static_objects.h"
#include "pharaoh/map/surface_queries.h"
#include "pharaoh/runtime/platform_measure_speed.h"
#include <stddef.h>

_Static_assert(sizeof(PharaohCityGroundScan)==0x54 &&
 offsetof(PharaohCityGroundScan,camera_row)==0x34 &&
 offsetof(PharaohCityGroundScan,tile)==0x48,"native city scan prefix");
_Static_assert(offsetof(PharaohBuildingTypeRecord,render_tint_bf)==0xbf &&
 offsetof(PharaohBuildingTypeRecord,pyramid_work_column)==0x56 &&
 offsetof(PharaohBuildingTypeRecord,origin_x)==0x68 &&
 offsetof(PharaohBuildingTypeRecord,pyramid_ramp1_height)==0x59,"native ground fields");

static void profile_terrain(PharaohCityGroundContext *c,uint32_t flags)
{
 uint32_t i;
 if((flags&4u)!=0&&(flags&0x80000u)==0)i=10;
 else if(flags&2u)i=9;
 else if(flags&0x800u)i=7;
 else if(flags&0x50000u)i=8;
 else if(flags&0x11u)i=11;
 else if(flags&0x80u)i=7;
 else i=6;
 ++c->ambient_scores[i];
}
static void draw_image(PharaohCityGroundContext *c,int32_t image,int32_t x,int32_t y)
{
 const PharaohCityGroundOperations *ops=c->operations;
 ops->draw_image(ops->image_user_data!=NULL?ops->image_user_data:ops->user_data,image,x,y);
}
static int neighbor_occludes(PharaohCityGroundContext *c,uint32_t tile)
{
 if((int32_t)tile>=0x0fff0000)return 0;
 const PharaohBuildingTypeRecord *b=&c->buildings[c->building_ids[tile]];
 if(b->building_type==183 && b->pyramid_work_column!=0 &&
    b->origin_x!=0 && b->pyramid_ramp1_height<=3)return 1;
 int32_t id=c->walker_ids[tile];
 while(id>0) {
  const PharaohWalkerQueryRecord *w=&c->walkers[id];
  if(w->is_ghost==0) {
   switch(w->type) {case 13:case 45:case 50:case 56:case 57:return 1;default:break;}
  }
  id=w->next_walker_id_on_same_tile;
 }
 return 0;
}
static int footprint_position(PharaohCityGroundContext *c,uint32_t size,
 uint32_t x,uint32_t y,uint32_t *px,uint32_t *py)
{
 const PharaohCityGroundScan *s=c->scan;
 switch(size) {
 case 0:*px=x;*py=y;return 1;
 case 1:*px=x+s->tile_x[3]+1u;*py=y-s->tile_y[3];return 1;
 case 2:*px=x+s->tile_x[2]+2u;*py=y-s->tile_y[2];return 1;
 case 3:*px=x+s->tile_x[1]+3u;*py=y-s->tile_y[1];return 1;
 case 4:*px=x+s->tile_x[0]+4u;*py=y-s->tile_y[0];return 1;
 case 5:*px=x+s->tile_x[0]+s->tile_x[3]+5u;*py=y-s->tile_y[0]-s->tile_y[3];return 1;
 default:return 0;
 }
}

/* Complete C reuse of Unit00470/Unit00472. No actor-chain repair or cycle
 * guard is added: the original traversal consumes the live chains as stored. */
static void draw_normal_tile(PharaohCityGroundContext *c,uint32_t tile,uint32_t x,uint32_t y,uint32_t alternate,uint32_t columnIndex,uint32_t rowIndex,uint32_t middleColumn,uint32_t middleRow,uint8_t boundary)
{
    int32_t Building=(int16_t)c->building_ids[tile];
    uint32_t image=c->images[tile],flags=c->terrain[tile],foot;
    uint32_t classification,base,step,span,last,index,first,second,px,py;
    int central=((int32_t)(columnIndex))>((int32_t)(middleColumn-3u))&&((int32_t)(columnIndex))<((int32_t)(middleColumn+3u))&&((int32_t)(rowIndex))>((int32_t)(middleRow-3u))&&((int32_t)(rowIndex))<((int32_t)(middleRow+3u));
    *c->current_image=image;
    foot=c->footprints[tile];
    if(Building!=0){
        if(c->buildings[Building].render_tint_bf!=0)*c->tint=0xf863;
        if(central){
            classification=pharaoh_sound_city_building_ambient_category((uint32_t)(int32_t)(int16_t)c->buildings[Building].building_type);
            if(classification==1)c->ambient_scores[1]=c->ambient_scores[1]+1u;
            else{
                classification=pharaoh_sound_city_building_ambient_category((uint32_t)(int32_t)(int16_t)c->buildings[Building].building_type);
                if(classification==2)c->ambient_scores[2]=c->ambient_scores[2]+1u;
                else if(pharaoh_sound_city_building_ambient_category((uint32_t)(int32_t)(int16_t)c->buildings[Building].building_type)==4)c->ambient_scores[4]=c->ambient_scores[4]+1u;
            }
            tile=c->scan->tile;image=*c->current_image;
        }
    }else if(central)profile_terrain(c,flags);
    *c->ambient_total=*c->ambient_total+1u;
    if(flags&0x20u){
        c->buildings[0].building_type=39;pharaoh_sound_city_mark_building_view(c->buildings,0,2,c->building_sound_ids,*c->sound_suppressed,c->sound_views);
        tile=c->scan->tile;image=*c->current_image;
    }
    if(foot&0x10u){image=*c->cleared_image;*c->current_image=image;}
    foot&=15u;
    if(foot==0){
        if((flags&4u)!=0&&*c->advance_animation!=0){
            if(flags&0x4000000u){base=boundary?*c->water_variant_b:*c->water_variant_a;step=15;span=90;}
            else{base=*c->water_image;step=1;span=6;}
            last=base-1u+step*6u;
            if(((int32_t)(image))<=((int32_t)(last))&&((int32_t)(image))>=((int32_t)(base))){
                image+=step;*c->current_image=image;
                if(((int32_t)(image))>((int32_t)(last))){image-=span;*c->current_image=image;}
                c->images[tile]=image;
            }
        }
        base=*c->royal_tomb_image;
        if(((int32_t)(image))<((int32_t)(base+48u))||((int32_t)(image))>=((int32_t)(base+72u))){
            draw_image(c,image,x,y);tile=c->scan->tile;image=*c->current_image;
        }
        if((c->terrain[tile]&0xeefed77fu)==0){
            index=c->scan->row*231u+c->scan->column;
            if(alternate){first=c->neighbor_cells[index];second=c->neighbor_cells[index+1];}
            else{first=c->neighbor_cells[index+1];second=c->neighbor_cells[index+2];}
            if(neighbor_occludes(c,first)||neighbor_occludes(c,second))c->operations->draw_top(c->operations->top_user_data!=NULL?c->operations->top_user_data:c->operations->user_data,image,x,y);
        }
    }else if(footprint_position(c,foot,x,y,&px,&py))draw_image(c,image,px,py);
    *c->tint=0;
}

void pharaoh_city_draw_ground(PharaohCityGroundContext *c)
{
    uint32_t y,ystep,widthBound,heightBound,middleColumn,middleRow,row,column,x,alternate=0,rows=0,columns,tile;
    uint8_t boundary;
    pharaoh_native_ambient_scores_clear_0053b670(c->ambient_scores,c->ambient_total);
    ystep=c->scan->tile_y[3];widthBound=c->scan->visible_columns+6u;y=c->scan->start_y-ystep*9u;
    middleColumn=(uint32_t)(((int32_t)(widthBound))/2);heightBound=c->scan->visible_rows+14u;middleRow=(uint32_t)(((int32_t)(heightBound))/2);
    row=c->scan->camera_row-8u;c->scan->row=row;
    if(((int32_t)(heightBound))<=0)return;
    do{
        x=c->scan->start_x;if(alternate)x=x-c->scan->tile_x[3]-1u;
        alternate=!alternate;x+=(0xfffffffeu-c->scan->tile_x[2])*4u;
        column=c->scan->camera_column-4u;c->scan->column=column;columns=0;
        if(((int32_t)(widthBound))>=0){
            do{
                if(((int32_t)(column))>=0&&((int32_t)(column))<231&&((int32_t)(row))>=0&&((int32_t)(row))<457){
                    tile=c->cells[row*231u+column];c->scan->tile=tile;
                    boundary=(uint8_t)pharaoh_map_sweep_has_passed(*c->sweep_countdown,*c->sweep_start,(int32_t)column);
                    tile=c->scan->tile;
                    if(tile==*c->marked_tile){*c->marked_x=x;*c->marked_y=y;}
                    if(((int32_t)(tile))>=0x0fff0000){
                        *c->current_image=*c->debug_invalid==1?0:tile-0x0fff0000u;
                        draw_image(c,*c->current_image,x,y);
                    }else if(c->property_flags[tile]&0x40u){
                        draw_normal_tile(c,tile,x,y,alternate,columns,rows,middleColumn,middleRow,boundary);
                    }
                    row=c->scan->row;column=c->scan->column;
                }
                ++columns;++column;x+=c->scan->tile_x[2]+2u;widthBound=c->scan->visible_columns+6u;c->scan->column=column;
            }while(((int32_t)(columns))<=((int32_t)(widthBound)));
            ystep=c->scan->tile_y[3];
        }
        y+=ystep;++rows;++row;c->scan->row=row;
    }while(((int32_t)(rows))<((int32_t)(c->scan->visible_rows+14u)));
}

static int SpecialBuilding(uint16_t type)
{
    return type==229||type==234||type==235||type==236||type==230;
}
static void draw_overlay_tile(PharaohCityGroundContext *c,uint32_t tile,uint32_t x,uint32_t y)
{
    uint32_t mode,flags,foot,px,py,a=0,b=0;
    int32_t Building=(int16_t)c->building_ids[tile];
    int special,prefix=1;
    special=SpecialBuilding(c->buildings[Building].building_type);mode=*c->overlay;
    switch(mode){
    case 27:if(!special){c->operations->draw_overlay_27(c->operations->user_data,x,y,0);return;}break;
    case 28:if(!special){c->operations->draw_overlay_28(c->operations->user_data,x,y);return;}break;
    case 33:a=1;b=0;break;
    case 34:a=3;b=2;break;
    case 35:a=6;b=5;break;
    case 36:a=2;b=1;break;
    case 37:a=4;b=3;break;
    case 38:a=5;b=4;break;
    case 39:a=13;b=9;break;
    case 40:a=19;b=11;break;
    case 41:a=17;b=10;break;
    case 42:a=15;b=8;break;
    default:prefix=0;break;
    }
    if(prefix&&!special){c->operations->draw_resource_overlay(c->operations->user_data,x,y,a,b);return;}
    if((c->property_flags[tile]&0x40u)==0)return;
    if(SpecialBuilding(c->buildings[Building].building_type)){
        *c->current_image=c->images[tile];foot=c->footprints[tile]&15u;
        if(footprint_position(c,foot,x,y,&px,&py))draw_image(c,*c->current_image,px,py);
        return;
    }
    mode=*c->overlay;
    if(mode==45){c->operations->draw_overlay_45(c->operations->user_data,x,y);return;}
    if(mode==30){pharaoh_actor_walking_no_operation();return;}
    if(mode==51||*c->alternate_overlay!=0){c->operations->draw_overlay_51(c->operations->user_data,x,y);return;}
    foot=c->footprints[tile]&15u;flags=c->terrain[tile];
    if(flags&0x100u){
        *c->current_image=c->images[tile];
        if(foot!=0)return;
    }else if((flags&0x40u)!=0&&(flags&8u)==0){
        *c->current_image=c->images[tile];
        if(foot>1)return;
    }else{
        if(flags&0x40000000u){c->operations->draw_building_overlay(c->operations->user_data,Building,x,y,0);return;}
        if(mode!=49&&(flags&0xc04000u)!=0){*c->current_image=*c->cleared_image;foot=0;}
        else{
            if(flags&8u){c->operations->draw_building_overlay(c->operations->user_data,Building,x,y,0);return;}
            *c->current_image=c->images[tile];
        }
    }
    if(footprint_position(c,foot,x,y,&px,&py))draw_image(c,*c->current_image,px,py);
}

void pharaoh_city_draw_overlay_ground(PharaohCityGroundContext *c)
{
    uint32_t ystep=c->scan->tile_y[3],row=c->scan->camera_row-8u,y=c->scan->start_y-ystep*9u,rows=0,alternate=0;
    uint32_t height=c->scan->visible_rows+14u,origin,x,column,tile,columns;
    c->scan->row=row;
    if(((int32_t)(height))<=0)return;
    origin=c->scan->start_x;
    do{
        x=origin;if(alternate)x=x-c->scan->tile_x[3]-1u;alternate=!alternate;
        x+=(0xfffffffeu-c->scan->tile_x[2])*4u;c->scan->column=c->scan->camera_column-4u;columns=0;
        if(((int32_t)(c->scan->visible_columns+6u))>=0){
            do{
                column=c->scan->column;
                if(((int32_t)(column))>=0&&((int32_t)(column))<231&&((int32_t)(c->scan->row))>=0&&((int32_t)(c->scan->row))<457){
                    tile=c->cells[c->scan->row*231u+column];c->scan->tile=tile;
                    if(tile==*c->marked_tile){*c->marked_x=x;*c->marked_y=y;}
                    if(((int32_t)(tile))>=0x0fff0000){*c->current_image=tile-0x0fff0000u;draw_image(c,*c->current_image,x,y);}
                    else draw_overlay_tile(c,tile,x,y);
                }
                ++columns;x+=c->scan->tile_x[2]+2u;c->scan->column=c->scan->column+1u;
            }while(((int32_t)(columns))<=((int32_t)(c->scan->visible_columns+6u)));
            ystep=c->scan->tile_y[3];origin=c->scan->start_x;
        }
        y+=ystep;++rows;c->scan->row=c->scan->row+1u;
    }while(((int32_t)(rows))<((int32_t)(c->scan->visible_rows+14u)));
}

static void ground_step(void *data) { pharaoh_city_draw_ground(data); }
static void overlay_step(void *data) { pharaoh_city_draw_overlay_ground(data); }
void pharaoh_city_render_bind_ground(PharaohCityRenderOperations *ops,
    PharaohCityGroundContext *ground)
{
    ops->draw_base=ground_step;
    ops->draw_overlay_base=overlay_step;
    ops->pass_user_data[PHARAOH_CITY_PASS_BASE]=ground;
    ops->pass_user_data[PHARAOH_CITY_PASS_OVERLAY_BASE]=ground;
}
