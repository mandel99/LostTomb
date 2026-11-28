#include "pharaoh/ui/building_info_industry_live.h"
#include "pharaoh/population/food_supply_query.h"
#include "pharaoh/core/percentage_scale.h"
#include "pharaoh/core/static_objects.h"
#include <stddef.h>

_Static_assert(sizeof(PharaohIndustryMothballBank)==0x49d8,"native mothball bank stride");
_Static_assert(offsetof(PharaohBuildingTypeRecord,storage_link_amount)==0x3c&&
    offsetof(PharaohBuildingTypeRecord,num_workers)==0x40&&
    offsetof(PharaohBuildingTypeRecord,farm_progress)==0x54&&
    offsetof(PharaohBuildingTypeRecord,runtime_value_a6)==0xa6&&
    offsetof(PharaohBuildingTypeRecord,runtime_value_a7)==0xa7&&
    offsetof(PharaohBuildingTypeRecord,runtime_value_a8)==0xa8&&
    offsetof(PharaohBuildingTypeRecord,industry_native_ab)==0xab&&
    offsetof(PharaohBuildingTypeRecord,secondary_input_stock)==0xcc,
    "native industry WORD stocks/progress and BYTE work-camp/curse overlays");

static const PharaohStorageInfoDrawContext *draw(const PharaohInfoIndustryLiveContext *c)
{ return c->content->drawing; }
static PharaohBuildingInfoLiveContext *ui(const PharaohInfoIndustryLiveContext *c)
{ return draw(c)->ui; }
static PharaohBuildingTypeRecord *building(const PharaohInfoIndustryLiveContext *c)
{ return &ui(c)->buildings[ui(c)->window->building_id]; }
static int32_t add32(int32_t a,int32_t b)
{
    const uint32_t v=(uint32_t)a+(uint32_t)b;
    return v<=INT32_MAX?(int32_t)v:(int32_t)(v&INT32_MAX)-INT32_MAX-1;
}
static void image(const PharaohInfoIndustryLiveContext *c,int32_t id,int32_t x,int32_t y)
{ (void)pharaoh_image_draw(draw(c)->layout->inner_panel->drawing,id,x,y); }
static void language(const PharaohInfoIndustryLiveContext *c,int32_t group,int32_t entry,int32_t x,int32_t y)
{
    pharaoh_ui_text_draw_language(draw(c)->text,draw(c)->font_black,group,entry,x,y,
        *ui(c)->font_black,0,ui(c)->window->x,ui(c)->window->y);
}
static void line(const PharaohInfoIndustryLiveContext *c,int32_t group,int32_t entry,int32_t dy)
{ language(c,group,entry,ui(c)->window->x+32+*ui(c)->text_width,ui(c)->window->y+dy); }
static void multiline(const PharaohInfoIndustryLiveContext *c,int32_t group,int32_t entry,int32_t y)
{
    const PharaohBuildingInfoLiveState *w=ui(c)->window;
    pharaoh_info_ui_draw_multiline(draw(c)->layout,draw(c)->wrapped,draw(c)->font_black,
        group,entry,w->x+32,y,(w->width_blocks-4)*16,*ui(c)->font_black,0,w->x,w->y);
}
static void number(const PharaohInfoIndustryLiveContext *c,int32_t value,const char *suffix,int32_t dy)
{
    const PharaohBuildingInfoLiveState *w=ui(c)->window;
    pharaoh_ui_text_draw_number(draw(c)->text,draw(c)->font_black,value,'@',suffix,
        w->x+32+*ui(c)->text_width,w->y+dy,*ui(c)->font_black,0,w->x,w->y);
}
static void title(const PharaohInfoIndustryLiveContext *c,int32_t help,int32_t group,int32_t resource)
{
    PharaohBuildingInfoLiveState *w=ui(c)->window;w->help_id=help;
    if(w->first_draw){(void)pharaoh_native_return_zero_0047d350();w->first_draw=0;}
    pharaoh_panel_frame_draw_native(draw(c)->layout->outer_panel,w->x,w->y,w->width_blocks,w->height_blocks);
    if(resource!=0)image(c,add32(*ui(c)->resource_images,resource),w->x+10,w->y+10);
    pharaoh_ui_text_draw_centered_language(draw(c)->text,draw(c)->font_title,group,0,w->x,w->y+10,
        w->width_blocks*16,*ui(c)->font_title,0,w->x,w->y);
}
static void inner(const PharaohInfoIndustryLiveContext *c,int32_t y,int32_t height)
{
    const PharaohBuildingInfoLiveState *w=ui(c)->window;
    pharaoh_info_layout_draw_inner_panel(draw(c)->layout,w->x+16,w->y+y,w->width_blocks-2,height,w->x,w->y);
}
static void workers(const PharaohInfoIndustryLiveContext *c)
{ inner(c,136,4);pharaoh_info_draw_employment(draw(c),ui(c)->window->y+142); }
static void description(const PharaohInfoIndustryLiveContext *c,int32_t group)
{ multiline(c,group,1,ui(c)->window->y+ui(c)->window->height_blocks*16-113); }
static void no_road(const PharaohInfoIndustryLiveContext *c,int32_t y)
{ multiline(c,69,25,y); }
static int32_t staffing(const PharaohInfoIndustryLiveContext *c,int32_t base)
{
    const int32_t percent=ui(c)->window->details.worker_percentage;
    return base+(percent>=100?0:percent>=75?1:percent>=50?2:percent>=25?3:4);
}
static int halted(const PharaohInfoIndustryLiveContext *c,int32_t resource)
{ return c->mothball_banks[*c->current_player].disabled[resource]!=0; }
static int32_t progress(const PharaohInfoIndustryLiveContext *c,int32_t maximum)
{
    /* SignedWORD*100 lies in[-3276800,3276700]. Even denominator-1 cannot
     * reach the Native IDIV overflow boundary; the status is always VALUE. */
    return pharaoh_overlay_farm_progress_percent(building(c)->farm_progress,maximum);
}
static void production(const PharaohInfoIndustryLiveContext *c,int32_t group,int32_t value,int32_t dy)
{
    *ui(c)->text_width=0;line(c,group,2,dy);number(c,value,c->percent_suffix,dy);line(c,group,3,dy);
}
static void producer_state(const PharaohInfoIndustryLiveContext *c,int32_t group,int32_t resource,
    int can_be_blighted,int32_t y)
{
    if(ui(c)->window->details.road_access<1){no_road(c,y);return;}
    int32_t entry;
    if(halted(c,resource))entry=4;
    else if(can_be_blighted&&building(c)->industry_native_ab[2]>=5)entry=11; /* BYTE+AD */
    else if(building(c)->num_workers<1)entry=5;
    else entry=staffing(c,6);
    multiline(c,group,entry,y);
}
static void workshop_state(const PharaohInfoIndustryLiveContext *c,int32_t group,int32_t halt_resource,
    int32_t no_workers,int32_t no_raw,int32_t no_raw2,int32_t full_staff)
{
    const int32_t y=ui(c)->window->y+86;
    if(ui(c)->window->details.road_access<1){no_road(c,y);return;}
    int32_t entry;
    if(halt_resource!=0&&halted(c,halt_resource))entry=4;
    else if(building(c)->num_workers<1)entry=no_workers;
    else if(no_raw>=0&&building(c)->storage_link_amount<1)entry=no_raw;
    else if(no_raw2>=0&&building(c)->secondary_input_stock<1)entry=no_raw2;
    else entry=staffing(c,full_staff);
    multiline(c,group,entry,y);
}
static void stock(const PharaohInfoIndustryLiveContext *c,int32_t resource,int32_t group,int32_t entry,
    int32_t icon_x,int32_t text_x,int32_t dy,int32_t amount)
{
    const PharaohBuildingInfoLiveState *w=ui(c)->window;
    image(c,add32(*ui(c)->resource_images,resource),w->x+icon_x,w->y+dy-4);
    *ui(c)->text_width=0;language(c,group,entry,w->x+text_x,w->y+dy);
    pharaoh_info_text_draw_amount(draw(c)->text,draw(c)->font_black,draw(c)->number_suffix,8,10,
        amount<1?0:amount,w->x+text_x+*ui(c)->text_width,w->y+dy,*ui(c)->font_black,0);
}

/* Genuine adapters for the existing whole489240/508E00 query algorithms. */
static bool query_is_farm(void *data,int32_t id)
{ return pharaoh_building_is_farm(ui(data)->buildings,id)!=0; }
static bool query_active_occupant(void *data,int32_t id)
{
    const PharaohInfoIndustryLiveContext *c=data;
    return (uint8_t)pharaoh_building_has_active_occupant(c->content->occupancy,id)!=0;
}
static int32_t query_building_type(void *data,int32_t id)
{ return ui(data)->buildings[id].building_type; }
static int32_t query_crop(void *data,int32_t type)
{
    (void)data;
    /* Exact489314/48933C table, not the semantic farm-name ordering. */
    switch(type){case 100:return 0;case 101:return 1;case 102:return 2;case 103:return 3;
    case 104:return 4;case 105:return 5;case 195:return 8;case 196:return 7;case 224:return 9;
    default:return -1;}
}
static void query_raw(void *data,int32_t help,int32_t resource,int32_t group,int32_t shows_stock)
{ pharaoh_info_industry_draw_raw_producer(data,help,resource,group,shows_stock); }
PharaohIndustryQueryContext pharaoh_info_industry_query_context(const PharaohInfoIndustryLiveContext *c)
{
    const PharaohIndustryQueryContext q={*c->month,*c->flood_season_month,95,c->harvest_months,
        query_is_farm,query_active_occupant,query_building_type,query_crop,query_raw,(void *)c};return q;
}
void pharaoh_info_industry_draw_quarry(const PharaohInfoIndustryLiveContext *c,int32_t resource)
{
    const PharaohIndustryQueryContext q=pharaoh_info_industry_query_context(c);
    pharaoh_draw_quarry(&q,resource);
}
/* Whole505C40. The historical floodplain label uses actual4894A0 occupancy;
 * it is not replaced by a terrain-bit test. */
void pharaoh_info_industry_draw_farm_crop(const PharaohInfoIndustryLiveContext *c,int32_t group)
{
    PharaohBuildingInfoLiveContext *u=ui(c);
    const PharaohIndustryQueryContext q=pharaoh_info_industry_query_context(c);
    const int32_t month=pharaoh_farm_next_harvest_month(&q,u->window->building_id);
    if(month>=0) {
        *u->text_width=0;int32_t entry=14;
        if((uint8_t)pharaoh_building_has_active_occupant(c->content->occupancy,u->window->building_id))
            {entry=2;group=177;}
        line(c,group,entry,u->window->height_blocks*16-158);
        line(c,160,month,u->window->height_blocks*16-158);
    }
    if((uint8_t)pharaoh_building_type_is_00488080_group(u->buildings,u->window->building_id)) {
        *u->text_width=0;
        const int32_t entry=pharaoh_farm_has_water_access(u->buildings,u->window->building_id)?0:1;
        line(c,177,entry,u->window->height_blocks*16-143);
    }
}
void pharaoh_info_industry_draw_farm(const PharaohInfoIndustryLiveContext *c,int32_t help,int32_t resource,int32_t group)
{
    title(c,help,group,resource);production(c,group,progress(c,*c->farm_progress_max),44);
    line(c,112,12,44);
    const int32_t fertility=pharaoh_food_supply_farm_quality(c->food,ui(c)->window->building_id);
    number(c,fertility,c->percent_suffix,44);line(c,112,13,44);
    producer_state(c,group,resource,1,ui(c)->window->y+70);
    inner(c,136,4);pharaoh_info_draw_farm_workers(c->content,ui(c)->window->y+142);
    pharaoh_info_industry_draw_farm_crop(c,group);description(c,group);
}
void pharaoh_info_industry_draw_raw_producer(const PharaohInfoIndustryLiveContext *c,
    int32_t help,int32_t resource,int32_t group,int32_t shows_stock)
{
    title(c,help,group,resource);
    if(shows_stock){*ui(c)->text_width=0;line(c,group,2,44);
        number(c,building(c)->storage_link_amount,draw(c)->employee_suffix,44);line(c,group,3,44);}
    else production(c,group,progress(c,200),44);
    producer_state(c,group,resource,0,ui(c)->window->y+70);workers(c);description(c,group);
}
void pharaoh_info_industry_draw_workshop(const PharaohInfoIndustryLiveContext *c,int32_t help,int32_t product,
    int32_t group,int32_t raw,int32_t raw2,int32_t halt_resource)
{
    title(c,help,group,product);production(c,group,progress(c,400),40);
    stock(c,raw,group,raw2!=0?13:12,32,60,60,building(c)->storage_link_amount);
    if(raw2!=0)stock(c,raw2,group,14,230,260,60,building(c)->secondary_input_stock);
    workshop_state(c,group,halt_resource,5,11,raw2!=0?12:-1,6);workers(c);
}
static void guild(const PharaohInfoIndustryLiveContext *c,int32_t group,int32_t resource,int32_t no_raw)
{ title(c,363,group,resource);workshop_state(c,group,0,5,no_raw,-1,6);workers(c); }

void pharaoh_info_industry_draw_work_camp(const PharaohInfoIndustryLiveContext *c)
{
    title(c,8,179,0);const int32_t y=ui(c)->window->y+56;
    if(ui(c)->window->details.road_access<1)no_road(c,y);
    else if(building(c)->num_workers<1)multiline(c,179,2,y);
    else {
        const int32_t entry=building(c)->runtime_value_a6==0?3:
            4+(building(c)->runtime_value_a7!=0)+2*(building(c)->runtime_value_a8!=0);
        multiline(c,179,entry,y);
    }
    inner(c,136,6);pharaoh_info_draw_employment(draw(c),ui(c)->window->y+138);
    multiline(c,179,1,ui(c)->window->y+ui(c)->window->height_blocks*16-105);
}
void pharaoh_info_industry_draw_barley_farm(const PharaohInfoIndustryLiveContext *c) { pharaoh_info_industry_draw_farm(c,89,14,181); }
void pharaoh_info_industry_draw_grain_farm(const PharaohInfoIndustryLiveContext *c) { pharaoh_info_industry_draw_farm(c,89,1,112); }
void pharaoh_info_industry_draw_lettuce_farm(const PharaohInfoIndustryLiveContext *c) { pharaoh_info_industry_draw_farm(c,90,3,113); }
void pharaoh_info_industry_draw_flax_farm(const PharaohInfoIndustryLiveContext *c) { pharaoh_info_industry_draw_farm(c,91,16,115); }
void pharaoh_info_industry_draw_henna_farm(const PharaohInfoIndustryLiveContext *c) { pharaoh_info_industry_draw_farm(c,91,32,306); }
void pharaoh_info_industry_draw_reed_gatherer(const PharaohInfoIndustryLiveContext *c)
{
    title(c,94,116,22);stock(c,22,116,2,32,60,54,building(c)->storage_link_amount);
    producer_state(c,116,22,1,ui(c)->window->y+70);workers(c);description(c,116);
}
void pharaoh_info_industry_draw_cattle_ranch(const PharaohInfoIndustryLiveContext *c)
{
    title(c,360,117,2);production(c,112,progress(c,400),44);
    stock(c,9,23,9,32,60,100,building(c)->storage_link_amount);
    producer_state(c,117,2,1,ui(c)->window->y+70);workers(c);description(c,117);
}
void pharaoh_info_industry_draw_stone_quarry(const PharaohInfoIndustryLiveContext *c) { pharaoh_info_industry_draw_raw_producer(c,95,24,118,0); }
void pharaoh_info_industry_draw_gold_mine(const PharaohInfoIndustryLiveContext *c) { pharaoh_info_industry_draw_raw_producer(c,93,21,162,0); }
void pharaoh_info_industry_draw_copper_mine(const PharaohInfoIndustryLiveContext *c) { pharaoh_info_industry_draw_raw_producer(c,93,29,193,0); }
void pharaoh_info_industry_draw_gemstone_mine(const PharaohInfoIndustryLiveContext *c) { pharaoh_info_industry_draw_raw_producer(c,361,18,163,0); }
void pharaoh_info_industry_draw_clay_pit(const PharaohInfoIndustryLiveContext *c) { pharaoh_info_industry_draw_raw_producer(c,92,11,121,0); }
void pharaoh_info_industry_draw_brewery(const PharaohInfoIndustryLiveContext *c) { pharaoh_info_industry_draw_workshop(c,96,15,122,14,0,15); }
void pharaoh_info_industry_draw_weaver(const PharaohInfoIndustryLiveContext *c) { pharaoh_info_industry_draw_workshop(c,60,17,123,16,0,17); }
void pharaoh_info_industry_draw_weaponsmith(const PharaohInfoIndustryLiveContext *c) { pharaoh_info_industry_draw_workshop(c,98,10,124,29,0,10); }
void pharaoh_info_industry_draw_carpenters_guild(const PharaohInfoIndustryLiveContext *c) { pharaoh_info_industry_draw_workshop(c,363,20,171,20,0,0); }
void pharaoh_info_industry_draw_bricklayers_guild(const PharaohInfoIndustryLiveContext *c) { guild(c,172,12,-1); }
void pharaoh_info_industry_draw_stonemasons_guild(const PharaohInfoIndustryLiveContext *c) { guild(c,173,24,11); }
void pharaoh_info_industry_draw_artisans_guild(const PharaohInfoIndustryLiveContext *c)
{
    title(c,472,312,0);*ui(c)->text_width=0;
    stock(c,33,312,10,32,60,60,building(c)->storage_link_amount);
    stock(c,11,312,11,230,260,60,building(c)->secondary_input_stock);
    workshop_state(c,312,0,2,8,9,3);workers(c);
}
void pharaoh_info_industry_draw_hunting_lodge(const PharaohInfoIndustryLiveContext *c)
{
    title(c,359,154,0);stock(c,8,154,13,32,60,60,building(c)->storage_link_amount);
    workshop_state(c,154,8,5,11,-1,6);workers(c);
}
void pharaoh_info_industry_draw_water_lift(const PharaohInfoIndustryLiveContext *c)
{
    title(c,59,107,0);const PharaohBuildingInfoLiveState *w=ui(c)->window;
    pharaoh_ui_text_draw_centered_language(draw(c)->text,draw(c)->font_black,13,1,w->x,
        w->y+w->height_blocks*16-24,w->width_blocks*16,*ui(c)->font_black,0,w->x,w->y);
    const int32_t y=w->y+w->height_blocks*16-193;
    if(w->details.road_access<1)no_road(c,y);
    else multiline(c,107,building(c)->farm_water_access==0?3:building(c)->num_workers<1?2:1,y);
    workers(c);
}
void pharaoh_info_industry_draw_water_supply(const PharaohInfoIndustryLiveContext *c)
{
    title(c,61,108,0);const PharaohBuildingInfoLiveState *w=ui(c)->window;
    const int32_t y=w->y+w->height_blocks*16-193;
    if(w->details.road_access<1)no_road(c,y);else multiline(c,108,7-building(c)->num_workers,y);
    workers(c);
}
void pharaoh_info_industry_draw_well(const PharaohInfoIndustryLiveContext *c)
{
    title(c,62,109,0);
    const int32_t houses=pharaoh_map_houses_water_in_radius(c->house_water,ui(c)->window->building_id,2);
    multiline(c,109,houses==1?2:houses==2?3:1,ui(c)->window->y+ui(c)->window->height_blocks*16-126);
}
