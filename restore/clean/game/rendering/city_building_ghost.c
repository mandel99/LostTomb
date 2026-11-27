#include "pharaoh/rendering/city_building_ghost.h"
#include <stddef.h>

_Static_assert(sizeof(PharaohGhostDefinition)==20&&offsetof(PharaohGhostDefinition,image_group)==8,
    "native construction definition DWORD fields");
_Static_assert(sizeof(PharaohGhostImageGroup)==8&&sizeof(PharaohGhostDrawQueueEntry)==16&&
    sizeof(PharaohGhostScreenOffset)==8&&offsetof(PharaohGhostMapObject,state)==24,
    "native image groups, deferred draw records, offsets and object BYTE");
static int32_t signed_bits(uint32_t v)
{ return v<=INT32_MAX?(int32_t)v:(int32_t)(v&INT32_MAX)-INT32_MAX-1; }
static int32_t PvAdd(int32_t a,int32_t b) { return signed_bits((uint32_t)a+(uint32_t)b); }
static int32_t PvSub(int32_t a,int32_t b) { return signed_bits((uint32_t)a-(uint32_t)b); }
static int32_t PvMul(int32_t a,int32_t b) { return signed_bits((uint32_t)a*(uint32_t)b); }
static int32_t treasury(const PharaohCityBuildingGhostContext *c)
{ return c->cities[*c->current_player].treasury; }
static uint32_t terrain(const PharaohCityBuildingGhostContext *c,int32_t tile)
{ return c->map->terrain[tile]; }
static uint8_t LiveObject(const PharaohCityBuildingGhostContext *c,int32_t tile)
{ return c->objects[tile]&&c->objects[tile]->state==0; }
static void project(const PharaohCityBuildingGhostContext *c,int32_t x,int32_t y,int32_t *px,int32_t *py)
{ pharaoh_city_view_tile_to_pixel(c->view->state,c->view->projection,c->view->map_bounds,x,y,px,py); }
static void draw_image(const PharaohCityBuildingGhostContext *c,int32_t image,int32_t x,int32_t y)
{ (void)pharaoh_image_draw(c->images,image,x,y); }
static void draw_top(const PharaohCityBuildingGhostContext *c,int32_t image,int32_t x,int32_t y)
{
    PharaohUpperSpriteDrawContext top=*c->upper;top.tint_mask=*c->tint;
    (void)pharaoh_image_draw_isometric_top(&top,image,x,y);
}
static void draw_mask(const PharaohCityBuildingGhostContext *c,int32_t image,int32_t x,int32_t y,uint32_t mask)
{ (void)pharaoh_image_draw_masked(c->images,image,x,y,mask); }
static void DrawValid(const PharaohCityBuildingGhostContext *c,int32_t image,int32_t x,int32_t y)
{ *c->tint=0x1fe3;draw_image(c,image,x,y);draw_top(c,image,x,y);*c->tint=0; }
static int32_t region_exists(const PharaohCityBuildingGhostContext *c,
    int32_t x,int32_t y,int32_t size,int32_t radius,uint32_t mask)
{ return pharaoh_map_terrain_exists_in_area_with(c->map,x,y,size,radius,mask,c->area); }
static int32_t region_all(const PharaohCityBuildingGhostContext *c,
    int32_t x,int32_t y,int32_t size,int32_t radius,uint32_t mask)
{ return pharaoh_map_terrain_all_in_area_are(c->map,x,y,size,radius,mask,c->area); }
static uint32_t predicate(uint32_t bits,void *data)
{ return (*(const PharaohConstructionTerrainRule *)data)(bits); }
static int32_t region_predicate(const PharaohCityBuildingGhostContext *c,
    int32_t x,int32_t y,int32_t size,int32_t radius,PharaohConstructionTerrainRule rule)
{
    return pharaoh_map_region_matches_predicate(c->map,x,y,size,radius,predicate,&rule,
        c->surface->area_x,c->surface->area_y);
}
static int32_t region_count(const PharaohCityBuildingGhostContext *c,
    int32_t x,int32_t y,int32_t size,int32_t radius,uint32_t mask,uint32_t required)
{
    return (int32_t)pharaoh_map_region_terrain_count_reached(c->map,x,y,size,radius,mask,
        signed_bits(required),c->surface->area_x,c->surface->area_y);
}
static void channel_match(const PharaohCityBuildingGhostContext *c)
{
    PharaohMapImageContext match=*c->image_match;match.orientation=*c->orientation;
    (void)pharaoh_map_image_context_match(&match,c->surface->channel_context,16,-1,0);
}

/* Whole43B630, reused from the complete Unit00565 C. The split below keeps
 * road/canal/bridge and ordinary building algorithms, including native reloads
 * after children, deferred reverse drawing, tint stores and per-cell failures. */
static void road(const PharaohCityBuildingGhostContext *c)
{
    int32_t tile=*c->selected_tile,image=*c->group_f56c94,x,y,base;uint32_t bits;uint8_t invalid=0;
    project(c,*c->tile_x,*c->tile_y,&x,&y);y=PvAdd(y,*c->half_height);bits=terrain(c,tile);
    if((bits&0x10000)||LiveObject(c,tile)) {
        image=*c->group_f56c34;
        if(!pharaoh_map_can_place_road_on_canal_object(c->routes,tile))invalid=1;
        else image=PvAdd(image,pharaoh_map_road_canal_crossing_image(tile,PvSub(c->image_ids[tile],image)));
    }
    bits&=0xeefed77f;
    if(bits&0x100) {
        image=*c->group_f56b84;if(!pharaoh_map_can_place_road_under_canal(c->map->terrain,c->image_ids,*c->group_f56b84,*c->orientation,tile))invalid=1;else image=PvAdd(image,(int32_t)c->operations->road_canal_image_51c460.query(c->operations->road_canal_image_51c460.context,tile));
    } else if(bits)invalid=1;
    else {
        image=(c->property_flags[tile]&0x80)?*c->group_f56c44:*c->group_f56c94;
        if(!pharaoh_map_terrain_has_adjacent_y(c->map->terrain,*c->selected_tile,0x40)&&pharaoh_map_terrain_has_adjacent_x(c->map->terrain,*c->selected_tile,0x40))image=PvAdd(image,1);
    }
    if(treasury(c)<=-5000)invalid=1;
    if(pharaoh_map_grid_size_reaches_outside(c->geometry,PvAdd(*c->view_origin_x,*c->tile_x),PvAdd(*c->view_origin_y,*c->tile_y),3))invalid=1;
    base=PvSub(y,*c->half_height);
    if(invalid)draw_mask(c,*c->group_f56be4,x,base,0xfffff863u);else DrawValid(c,image,x,base);
}
static void well(const PharaohCityBuildingGhostContext *c)
{
    int32_t x,y,screenY,tile,image;uint8_t invalid=0;
    project(c,*c->tile_x,*c->tile_y,&x,&y);y=PvAdd(y,*c->half_height);
    if(!*c->native_6059e4) {
        tile=PvAdd(PvAdd(*c->map_origin,PvMul(*c->tile_y,228)),*c->tile_x);*c->surface->current_tile=tile;
        if(terrain(c,tile)&0x40)invalid=pharaoh_map_can_place_canal_on_road(c->routes,tile)==0;
        else invalid=(terrain(c,tile)&0xeefed77f)!=0||LiveObject(c,tile);
    } else if(!*c->native_bc9724)invalid=1;
    if(treasury(c)<=-5000)invalid=1;
    if(pharaoh_map_grid_size_reaches_outside(c->geometry,PvAdd(*c->view_origin_x,*c->tile_x),PvAdd(*c->view_origin_y,*c->tile_y),3))invalid=1;
    screenY=PvSub(y,*c->half_height);
    if(invalid){draw_mask(c,*c->group_f56be4,x,screenY,0xfffff863u);return;}
    *c->tint=0x1fe3;*c->surface->current_tile=*c->selected_tile;pharaoh_map_adjacent_channel_flags(c->surface);channel_match(c);tile=*c->surface->current_tile;
    if(terrain(c,tile)&0x40) {
        if(!c->image_match->selection->record_value)c->image_match->selection->orientation_offset=((uint8_t)~(uint8_t)c->map->terrain[PvSub(tile,228)]>>6)&1;
        if(c->property_flags[tile]&0x80)image=PvSub(c->image_ids[tile],*c->group_f56b84);
        else {pharaoh_map_last_tile_is_buildable_for_editor(c->map->terrain,c->surface->height_values,(int32_t)*c->surface->current_tile);image=PvAdd(c->image_match->selection->orientation_offset,48);}
    } else image=PvAdd(c->image_match->selection->orientation_offset,48);
    draw_image(c,PvAdd(*c->group_f56b84,image),x,screenY);*c->tint=0;
}
static void QueueDraw(const PharaohCityBuildingGhostContext *c)
{
    PharaohGhostDrawQueueEntry *q=&c->draw_queue[*c->draw_count];
    q->image=c->images->state->image;q->x=c->images->state->x;q->y=c->images->state->y;
    *c->draw_count+=1u;
}
static void step(const PharaohCityBuildingGhostContext *c,int32_t Direction,int32_t *x,int32_t *y)
{
    switch(Direction){case 0:*x=PvAdd(*x,*c->half_width);*y=PvSub(*y,*c->half_height);break;case 2:*x=PvAdd(*x,*c->half_width);*y=PvAdd(*y,*c->half_height);break;case 4:*x=PvSub(*x,*c->half_width);*y=PvAdd(*y,*c->half_height);break;case 6:*x=PvSub(*x,*c->half_width);*y=PvSub(*y,*c->half_height);break;}
}
static void bridge(const PharaohCityBuildingGhostContext *c)
{
    int32_t x,y,Direction,length,index,image,peak=5;uint8_t invalid;
    project(c,*c->tile_x,*c->tile_y,&x,&y);y=PvAdd(y,*c->half_height);c->operations->bridge_probe_47f200.draw(c->operations->bridge_probe_47f200.context,*c->tile_x,*c->tile_y,*c->tool==0x52?0:1);
    Direction=PvSub(signed_bits(c->bridge->direction),*c->orientation);if(Direction<0)Direction=PvAdd(Direction,8);
    length=c->bridge->length;invalid=(*c->tool==0x53&&length<5)||!c->bridge->available;
    if(treasury(c)<=-5000||invalid) {
        int32_t remaining;y=PvSub(y,*c->half_height);draw_mask(c,*c->group_f56be4,x,y,length<1?0xfffff863u:0x1fe3u);
        if(c->bridge->length<2)return;
        remaining=PvSub(c->bridge->length,1);do{step(c,Direction,&x,&y);remaining=PvSub(remaining,1);}while(remaining);
        draw_mask(c,*c->group_f56be4,x,y,0xfffff863u);return;
    }
    y=PvSub(PvSub(y,*c->bridge_height),*c->half_height);*c->tint=0x1fe3;*c->draw_count=0;
    if(*c->tool==0x52) {
        image=Direction==0?3:Direction==2?0:Direction==4?5:2;
        draw_mask(c,PvAdd(*c->group_f56d34,image),x,y,0x1fe3);draw_image(c,PvAdd(*c->group_f56d34,image),x,y);QueueDraw(c);
        for(index=1;index<c->bridge->length;index=PvAdd(index,1)) {
            step(c,Direction,&x,&y);
            if(index==PvSub(c->bridge->length,1))image=Direction==0?5:Direction==2?2:Direction==4?3:0;
            else image=Direction==0||Direction==4?4:1;
            draw_image(c,PvAdd(*c->group_f56d34,image),x,y);QueueDraw(c);
        }
    } else {
        int32_t lengthNow=c->bridge->length;
        if(lengthNow==9||lengthNow==10)peak=4;else if(lengthNow==13||lengthNow==14)peak=6;else if(lengthNow==15||lengthNow==16)peak=7;else if(lengthNow!=11&&lengthNow!=12)peak=8;
        switch(Direction){case 0:draw_image(c,PvAdd(*c->group_f56d34,11),PvSub(x,3),PvSub(y,50));break;case 2:draw_image(c,PvAdd(*c->group_f56d34,6),PvSub(x,1),PvSub(y,12));break;case 4:draw_image(c,PvAdd(*c->group_f56d34,9),PvSub(x,30),PvSub(y,12));break;case 6:draw_image(c,PvAdd(*c->group_f56d34,8),PvSub(x,23),PvSub(y,53));break;}
        QueueDraw(c);
        for(index=1;index<c->bridge->length;index=PvAdd(index,1)) {
            int32_t drawX=x,drawY=y;uint8_t draw=1;step(c,Direction,&x,&y);drawX=x;drawY=y;
            if(index==1||index==PvSub(c->bridge->length,1))continue;
            if(index==PvSub(c->bridge->length,2)) {
                switch(Direction){case 0:image=9;drawX=PvAdd(x,1);drawY=PvSub(y,24);break;case 2:image=8;drawX=PvAdd(x,7);drawY=PvSub(y,39);break;case 4:image=11;drawX=PvSub(x,34);drawY=PvSub(y,35);break;case 6:image=6;drawX=PvSub(x,29);drawY=PvSub(y,22);break;default:draw=0;image=0;break;}
            } else if(index==peak) {
                if(Direction==0||Direction==4){image=13;drawY=PvSub(y,38);}
                else if(Direction==2||Direction==6){image=12;drawX=PvAdd(x,7);drawY=PvSub(y,38);}else{draw=0;image=0;}
            } else {
                if(Direction==0||Direction==4){image=10;drawY=PvSub(y,37);}
                else if(Direction==2||Direction==6){image=7;drawX=PvAdd(x,7);drawY=PvSub(y,38);}else{draw=0;image=0;}
            }
            if(draw)draw_image(c,PvAdd(*c->group_f56d34,image),drawX,drawY);QueueDraw(c);
        }
    }
    if((Direction==0||Direction==6)&&PvSub(*c->draw_count,1)>=0) {
        int32_t remaining=*c->draw_count;const PharaohGhostDrawQueueEntry *row=&c->draw_queue[PvSub(remaining,1)];
        do {draw_image(c,row->image,row->x,row->y);--row;remaining=PvSub(remaining,1);}while(remaining);
    }
    *c->tint=0;
}
static void Building(const PharaohCityBuildingGhostContext *c);
static void draw(const PharaohCityBuildingGhostContext *c)
{
    int32_t mode=*c->tool;*c->special_tool=mode==0xe5||mode==0xea||mode==0xeb||mode==0xec;
    if(!*c->selected_tile||(*c->native_e38dcc&&!*c->placement_byte)||*c->native_605994||mode<=0)return;
    if(*c->editor==1) {
        switch(mode){case 0x7e:case 0x84:case 0x85:case 0x7f:case 0x80:case 0x77:case 0x82:case 0x83:case 0x8b:case 0xd6:case 0x58:case 0x59:case 0x5d:case 0x81:break;default:return;}
    }
    switch(mode) {
    case 7:case 0x5a:c->operations->preview_43dd70.draw(c->operations->preview_43dd70.context,1,1);return;
    case 5:road(c);return;case 8:well(c);return;case 0x52:case 0x53:bridge(c);return;
    case 0x28:case 0x2c:case 0x2d:c->operations->preview_43d720.draw(c->operations->preview_43d720.context);return;case 0xd2:c->operations->preview_441490.draw(c->operations->preview_441490.context);return;case 0xd5:c->operations->preview_541c00.draw(c->operations->preview_541c00.context);return;
    case 0xe1:c->operations->member_40d870.draw(c->operations->member_40d870.context);return;case 0xe6:c->operations->member_401420.draw(c->operations->member_401420.context);return;
    case 0xe3:c->operations->member_416f90.draw(c->operations->member_416f90.context);return;case 0xe5:c->operations->member_5456e0.draw(c->operations->member_5456e0.context);return;
    case 0xea:c->operations->member_558b80.draw(c->operations->member_558b80.context);return;case 0xeb:c->operations->member_555e20.draw(c->operations->member_555e20.context);return;
    case 0xec:c->operations->member_54f410.draw(c->operations->member_54f410.context);return;case 0xe4:c->operations->member_4e6610.draw(c->operations->member_4e6610.context);return;
    case 0xde:c->operations->preview_4debe0.draw(c->operations->preview_4debe0.context);return;case 0x4a:c->operations->preview_43df10.draw(c->operations->preview_43df10.context,0x20b);return;case 0xb7:c->operations->preview_4edbb0.draw(c->operations->preview_4edbb0.context);return;
    case 0x4c:case 0xb5:c->operations->preview_43dd70.draw(c->operations->preview_43dd70.context,1,0);return;case 0xb6:c->operations->preview_43df10.draw(c->operations->preview_43df10.context,0x20d);return;case 0x4b:c->operations->preview_43df10.draw(c->operations->preview_43df10.context,0x222);return;case 0xc0:c->operations->preview_43df10.draw(c->operations->preview_43df10.context,0x20c);return;
    case 0x81:c->operations->preview_43e090.draw(c->operations->preview_43e090.context);return;case 0x41:case 0x42:case 0x43:case 0x44:case 0x45:c->operations->preview_4c0f30.draw(c->operations->preview_4c0f30.context,mode);return;
    case 0xd3:case 0xd4:c->operations->preview_4c2f50.draw(c->operations->preview_4c2f50.context,mode);return;case 0xab:case 0xca:c->operations->preview_4bfcd0.draw(c->operations->preview_4bfcd0.context);return;
    case 0x29:case 0x2a:case 0x2b:c->operations->preview_441f80.draw(c->operations->preview_441f80.context);return;case 0x88:c->operations->preview_43d300.draw(c->operations->preview_43d300.context);return;
    case 0x1f:case 0x21:case 0x1e:case 0xd1:c->operations->preview_43f5b0.draw(c->operations->preview_43f5b0.context,mode);return;
    }
    Building(c);
}

static uint8_t ScenarioForbids(const PharaohCityBuildingGhostContext *c)
{
    const uint32_t player=*c->current_player;
    const PharaohConstructionCommitCity *city=&c->cities[player];
    switch(*c->tool) {
    case 0xbb:case 0xbc:case 0xbd:return city->palace_placed!=0;
    case 0x4d:case 0x4e:case 0x4f:return city->mansion_placed!=0;
    case 0x5f:return *c->native_705c6c!=0;
    case 0x32:return c->preview_cities[player].native_4600!=0;
    case 0x5e:return city->academy_placed!=0;default:return 0;
    }
}
static uint8_t IndividualTerrainMode(int32_t mode)
{
    switch(mode){case 0x7e:case 0x84:case 0x85:case 0x7f:case 0x80:case 0x77:case 0x82:case 0x83:case 0x8b:case 0xd6:return 1;default:return 0;}
}
static void Building(const PharaohCityBuildingGhostContext *c)
{
    int32_t mode,size,originX,originY,tile,image,count,orientation,x,y,baseY,n;
    uint8_t allInvalid=0,anyInvalid=0,skipScenario=0,seenRaised=0,seenGround=0,canRaise;
    const int32_t *offsetBase;const PharaohGhostDefinition *definition;
    if(*c->variant>0&&c->operations->key_state(c->operations->keyboard_context,0x52)==-32767)*c->variant=2-(*c->variant!=1);
    tile=*c->selected_tile;mode=*c->tool;definition=&c->definitions[mode];
    size=definition->size;image=PvAdd(c->image_groups[definition->image_group].first_image,definition->image_offset);if(mode==0x48)size=3;
    originX=*c->tile_x;originY=*c->tile_y;
    if(*c->orientation==2)originX=PvAdd(PvSub(originX,size),1);
    else if(*c->orientation!=0){if(*c->orientation==4)originX=PvAdd(PvSub(originX,size),1);originY=PvAdd(PvSub(originY,size),1);}
    if(!*c->editor&&(*c->tile_x<3||PvAdd(PvSub(*c->tile_x,1),size)>=PvSub(c->geometry->width,3)||*c->tile_y<3||PvAdd(PvSub(*c->tile_y,1),size)>=PvSub(c->geometry->height,3)))allInvalid=anyInvalid=1;
    if(region_exists(c,originX,originY,size,1,0x80000))allInvalid=anyInvalid=1;
    if(c->requirements->terrain_10800){if(!region_exists(c,originX,originY,3,1,0x10800))allInvalid=anyInvalid=1;}
    else if(*c->terrain_rule_token){if(!region_predicate(c,originX,originY,2,1,c->terrain_predicate))allInvalid=anyInvalid=1;}
    else if(c->requirements->forest){if(!region_exists(c,originX,originY,2,1,1))allInvalid=anyInvalid=1;}
    else if(c->requirements->water_10004){if(!region_exists(c,originX,originY,2,3,0x10004))allInvalid=anyInvalid=1;}
    else if(c->requirements->water_40000){if(!region_exists(c,originX,originY,2,1,0x40000))allInvalid=anyInvalid=1;}
    else if(c->requirements->wall){if(!region_all(c,originX,originY,2,0,0x4000))allInvalid=anyInvalid=1;}
    else if(c->requirements->road){if(!region_all(c,originX,originY,1,0,0x40))allInvalid=anyInvalid=1;}
    else if(c->requirements->floodplain_count){if(!region_count(c,originX,originY,size,0,0x80,c->requirements->floodplain_count))allInvalid=anyInvalid=1;}
    else {
        mode=*c->tool;
        if(mode==0x7e||mode==0x7f||mode==0x80||mode==0x82||mode==0x83) {
            if(!pharaoh_map_grid_size_reaches_outside(c->geometry,PvAdd(*c->view_origin_x,*c->tile_x),PvAdd(*c->view_origin_y,*c->tile_y),4))allInvalid=anyInvalid=1;
            mode=*c->tool;
            if(mode==0x7e){skipScenario=1;if(*c->native_605a28>8&&!(terrain(c,*c->selected_tile)&4))allInvalid=anyInvalid=1;}
            else if((mode==0x82||mode==0x83)&&!pharaoh_map_water_corner_at(c->map->terrain,*c->selected_tile))allInvalid=anyInvalid=1;
        } else if(mode==0x84){skipScenario=1;if(!(terrain(c,*c->selected_tile)&4))allInvalid=anyInvalid=1;}
        else if(mode==0x85||mode==0x8b){if(!(uint8_t)pharaoh_editor_terrain_kind_result(c->editor_terrain,terrain(c,*c->selected_tile),mode==0x8b))allInvalid=anyInvalid=1;}
        else if(mode==0xd6){if(!pharaoh_editor_tile_deep_water_edge(c->map->terrain,*c->map_origin,*c->tile_x,*c->tile_y))allInvalid=anyInvalid=1;}
        else if(mode>=0x29&&mode<=0x2b)image=PvAdd(image,*c->variant);
    }
    if(!skipScenario&&ScenarioForbids(c))allInvalid=anyInvalid=1;
    project(c,*c->tile_x,*c->tile_y,&x,&y);baseY=PvAdd(y,*c->half_height);
    count=c->footprint_counts[size];orientation=*c->orientation/2;canRaise=(uint8_t)pharaoh_building_type_is_0046fde0_group(*c->tool);offsetBase=c->footprint_offsets[orientation];
    for(n=0;n<count;n=PvAdd(n,1)) {
        int32_t cell=PvAdd(offsetBase[n],tile);uint32_t bits,blocked;uint8_t test=1;
        if(LiveObject(c,cell)){allInvalid=anyInvalid=1;continue;}
        bits=terrain(c,cell);
        if(bits&0x10000){if(seenGround||!canRaise)anyInvalid=1;seenRaised=1;}
        else {if(seenRaised)anyInvalid=1;seenGround=1;}
        blocked=bits&0xeefed77f;mode=*c->tool;
        if(mode==0x3b||mode==0xac||mode==0xad)blocked&=0xff3fbfff;
        if(mode==0x38||mode==0x26)blocked&=0xffffffbf;
        else if(mode==0x8a)blocked&=8;
        else {
            if(mode==0x7f||mode==0x80||mode==0x77)blocked&=0xffffffbf;
            if(mode==0x7e){if(*c->native_605a28<=8)blocked&=0xffffffbf;else test=0;}
            else {if(mode==0x82||mode==0x83)blocked=0;if(mode==0x84||mode==0x8b||mode==0x85||mode==0xd6)test=0;}
        }
        if(test&&blocked)anyInvalid=1;
        if(c->walker_ids[cell])anyInvalid=1;
    }
    mode=*c->tool;
    if(mode==0x26) {
        if(!(terrain(c,PvAdd(offsetBase[0],tile))&0x40))anyInvalid=1;*c->surface->current_tile=tile;if(!pharaoh_map_last_tile_is_buildable_for_editor(c->map->terrain,c->surface->height_values,(int32_t)*c->surface->current_tile))anyInvalid=1;
    }
    if(*c->tool==0x8a){uint32_t bits=terrain(c,PvAdd(offsetBase[0],tile));if(!(bits&0x40)||(bits&0x10000)||c->tile_direction[tile])anyInvalid=1;}
    if(treasury(c)<=-5000)allInvalid=1;
    else if(!anyInvalid) {
        int32_t drawY=PvSub(baseY,*c->half_height);mode=*c->tool;
        if(IndividualTerrainMode(mode)){draw_mask(c,*c->group_f56be4,x,drawY,0x1fe3);return;}
        if(mode==0xc3||mode==0xc2) {
            const int32_t *source=mode==0xc3?c->group_f564dc:c->group_f56764;*c->tint=0x1fe3;draw_image(c,*source,x,drawY);draw_top(c,*source,x,drawY);*c->tint=0;return;
        }
        if((uint8_t)pharaoh_building_type_is_00487f80_group(mode)) {
            *c->tint=0x1fe3;
            if((uint8_t)pharaoh_region_has_active_occupant(c->occupancy,*c->tile_x,*c->tile_y,3,3)){int32_t base=*c->group_f56544;image=PvAdd(base,pharaoh_food_supply_area_quality(c->food,*c->tile_x,*c->tile_y,3,3)/14);}
            else image=*c->group_f56b24;
            draw_image(c,image,x,drawY);draw_top(c,image,x,drawY);*c->tint=0;return;
        }
        mode=*c->tool;
        if(mode==0xcf||mode==0xd0) {
            *c->tint=0x1fe3;switch(*c->native_576144){case 1:image=*c->group_f56d54;break;case 2:image=*c->group_f56d5c;break;case 3:image=*c->group_f56d64;break;default:image=0;break;}
            draw_image(c,image,x,drawY);draw_top(c,image,x,drawY);*c->tint=0;return;
        }
        if(mode==0x48||mode==0xc1) {
            int32_t pos;*c->tint=0x1fe3;draw_image(c,image,x,drawY);draw_top(c,image,x,drawY);draw_image(c,PvAdd(*c->group_f566ac,17),PvSub(x,4),PvSub(drawY,42));
            for(pos=1;pos<9;++pos) {
                int32_t xx=PvAdd(x,c->screen_offsets[pos].x),yy=PvAdd(baseY,PvSub(c->screen_offsets[pos].y,*c->half_height));
                draw_image(c,*c->group_f566b4,xx,yy);draw_top(c,*c->group_f566b4,xx,yy);
            }
            *c->tint=0;return;
        }
        if(mode!=0x47&&mode!=0xbf) {
            if(mode==10){*c->tint=0x1fe3;draw_image(c,*c->group_f5653c,x,drawY);draw_top(c,*c->group_f5653c,x,drawY);*c->tint=0;return;}
            if(mode>10&&mode<0x1e){for(n=0;n<count;n=PvAdd(n,1))draw_mask(c,*c->group_f56be4,PvAdd(x,c->screen_offsets[n].x),PvAdd(baseY,PvSub(c->screen_offsets[n].y,*c->half_height)),0x1fe3);return;}
            if(mode==0x38)return;
        }
        DrawValid(c,image,x,drawY);return;
    }
    for(n=0;n<count;n=PvAdd(n,1)) {
        int32_t xx=PvAdd(x,c->screen_offsets[n].x),yy=PvAdd(baseY,PvSub(c->screen_offsets[n].y,*c->half_height)),cell=PvAdd(offsetBase[n],tile);
        uint32_t bits=terrain(c,cell),raised=bits&0x10000,blocked,mask=0xfffff863u;
        if(*c->tool==0x26){*c->surface->current_tile=cell;if(!pharaoh_map_last_tile_is_buildable_for_editor(c->map->terrain,c->surface->height_values,(int32_t)*c->surface->current_tile))bits=1;}
        blocked=bits&0xeefed77f;mode=*c->tool;if(mode==0x7e||mode==0x7f||mode==0x80||mode==0x77)blocked&=0xffffffbf;
        if(c->walker_ids[cell])blocked=1;
        if(!allInvalid&&!blocked&&!LiveObject(c,cell)&&(!raised||canRaise))mask=0x1fe3;
        draw_mask(c,*c->group_f56be4,xx,yy,mask);
    }
}

static PharaohCityBuildingGhostResult availability(int present,uint32_t address)
{
    const PharaohCityBuildingGhostResult r={present?PHARAOH_BUILDING_GHOST_READY:
        PHARAOH_BUILDING_GHOST_MISSING_PROVIDER,present?0:address};return r;
}
PharaohCityBuildingGhostResult pharaoh_city_building_ghost_support(const PharaohCityBuildingGhostContext *c)
{
    const int32_t mode=*c->tool;
    const PharaohCityBuildingGhostOperations *o=c->operations;
    if(!*c->selected_tile||(*c->native_e38dcc&&!*c->placement_byte)||*c->native_605994||mode<=0)
        return availability(1,0);
    if(*c->editor==1) {
        switch(mode) {
        case 0x7e:case 0x84:case 0x85:case 0x7f:case 0x80:case 0x77:case 0x82:
        case 0x83:case 0x8b:case 0xd6:case 0x58:case 0x59:case 0x5d:case 0x81:break;
        default:return availability(1,0);
        }
    }
    switch(mode) {
    case 7:case 0x5a:case 0x4c:case 0xb5:return availability(o&&o->preview_43dd70.draw,0x0043dd70);
    case 5:return availability(o&&o->road_canal_image_51c460.query,0x0051c460);
    case 8:return availability(1,0);
    case 0x52:case 0x53:return availability(o&&o->bridge_probe_47f200.draw,0x0047f200);
    case 0x28:case 0x2c:case 0x2d:return availability(o&&o->preview_43d720.draw,0x0043d720);
    case 0xd2:return availability(o&&o->preview_441490.draw,0x00441490);
    case 0xd5:return availability(o&&o->preview_541c00.draw,0x00541c00);
    case 0xe1:return availability(o&&o->member_40d870.draw,0x0040d870);
    case 0xe6:return availability(o&&o->member_401420.draw,0x00401420);
    case 0xe3:return availability(o&&o->member_416f90.draw,0x00416f90);
    case 0xe5:return availability(o&&o->member_5456e0.draw,0x005456e0);
    case 0xea:return availability(o&&o->member_558b80.draw,0x00558b80);
    case 0xeb:return availability(o&&o->member_555e20.draw,0x00555e20);
    case 0xec:return availability(o&&o->member_54f410.draw,0x0054f410);
    case 0xe4:return availability(o&&o->member_4e6610.draw,0x004e6610);
    case 0xde:return availability(o&&o->preview_4debe0.draw,0x004debe0);
    case 0x4a:case 0xb6:case 0x4b:case 0xc0:return availability(o&&o->preview_43df10.draw,0x0043df10);
    case 0xb7:return availability(o&&o->preview_4edbb0.draw,0x004edbb0);
    case 0x81:return availability(o&&o->preview_43e090.draw,0x0043e090);
    case 0x41:case 0x42:case 0x43:case 0x44:case 0x45:return availability(o&&o->preview_4c0f30.draw,0x004c0f30);
    case 0xd3:case 0xd4:return availability(o&&o->preview_4c2f50.draw,0x004c2f50);
    case 0xab:case 0xca:return availability(o&&o->preview_4bfcd0.draw,0x004bfcd0);
    case 0x29:case 0x2a:case 0x2b:return availability(o&&o->preview_441f80.draw,0x00441f80);
    case 0x88:return availability(o&&o->preview_43d300.draw,0x0043d300);
    case 0x1f:case 0x21:case 0x1e:case 0xd1:return availability(o&&o->preview_43f5b0.draw,0x0043f5b0);
    default:break;
    }
    if(*c->variant>0&&(!o||!o->key_state))return availability(0,0x0056f1e4);
    if(!c->requirements->terrain_10800&&*c->terrain_rule_token) {
        const uint32_t token=*c->terrain_rule_token;
        if(token!=0x00405b40&&token!=0x00405b60&&token!=0x00405b80) {
            const PharaohCityBuildingGhostResult r={PHARAOH_BUILDING_GHOST_UNKNOWN_TERRAIN_RULE,token};return r;
        }
        return availability(c->terrain_predicate!=NULL,token);
    }
    return availability(1,0);
}
PharaohCityBuildingGhostResult pharaoh_city_building_ghost(const PharaohCityBuildingGhostContext *c)
{
    const PharaohCityBuildingGhostResult result=pharaoh_city_building_ghost_support(c);
    if(result.status==PHARAOH_BUILDING_GHOST_READY)draw(c);
    return result;
}
