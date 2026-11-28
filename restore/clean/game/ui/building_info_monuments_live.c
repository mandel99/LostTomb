#include "pharaoh/ui/building_info_monuments_live.h"
#include "pharaoh/core/static_objects.h"
#include "pharaoh/core/percentage_scale.h"
#include "pharaoh/runtime/text_loading.h"
#include "pharaoh/monuments/royal_tomb_queries.h"
#include <stddef.h>
#include <string.h>

static const PharaohStorageInfoDrawContext *draw(const PharaohInfoMonumentsLiveContext *c)
{ return c->drawing; }
static PharaohBuildingInfoLiveContext *ui(const PharaohInfoMonumentsLiveContext *c)
{ return draw(c)->ui; }
static PharaohBuildingTypeRecord *building(const PharaohInfoMonumentsLiveContext *c)
{ return &ui(c)->buildings[ui(c)->window->building_id]; }
static int32_t signed32(uint32_t v)
{ return v<=INT32_MAX?(int32_t)v:(int32_t)(v&INT32_MAX)-INT32_MAX-1; }
static int32_t add32(int32_t a,int32_t b)
{ return signed32((uint32_t)a+(uint32_t)b); }
static void image(const PharaohInfoMonumentsLiveContext *c,int32_t id,int32_t x,int32_t y)
{ (void)pharaoh_image_draw(draw(c)->layout->inner_panel->drawing,id,x,y); }
static void language(const PharaohInfoMonumentsLiveContext *c,int32_t group,int32_t entry,int32_t x,int32_t y)
{
    const PharaohBuildingInfoLiveState *w=ui(c)->window;
    pharaoh_ui_text_draw_language(draw(c)->text,draw(c)->font_black,group,entry,x,y,*ui(c)->font_black,0,w->x,w->y);
}
static void multiline(const PharaohInfoMonumentsLiveContext *c,int32_t group,int32_t entry,int32_t x,int32_t y,int32_t width)
{
    const PharaohBuildingInfoLiveState *w=ui(c)->window;
    pharaoh_info_ui_draw_multiline(draw(c)->layout,draw(c)->wrapped,draw(c)->font_black,group,entry,x,y,width,*ui(c)->font_black,0,w->x,w->y);
}
static void text(const PharaohInfoMonumentsLiveContext *c,int32_t group,int32_t entry,int32_t dy)
{ multiline(c,group,entry,ui(c)->window->x+32,ui(c)->window->y+dy,(ui(c)->window->width_blocks-4)*16); }
static void number(const PharaohInfoMonumentsLiveContext *c,int32_t value,const char *suffix,int32_t x,int32_t y)
{
    const PharaohBuildingInfoLiveState *w=ui(c)->window;
    pharaoh_ui_text_draw_number(draw(c)->text,draw(c)->font_black,value,'@',suffix,x,y,*ui(c)->font_black,0,w->x,w->y);
}
static void frame(const PharaohInfoMonumentsLiveContext *c,int32_t group,int32_t entry)
{
    const PharaohBuildingInfoLiveState *w=ui(c)->window;
    pharaoh_panel_frame_draw_native(draw(c)->layout->outer_panel,w->x,w->y,w->width_blocks,w->height_blocks);
    pharaoh_ui_text_draw_centered_language(draw(c)->text,draw(c)->font_title,group,entry,w->x,w->y+10,w->width_blocks*16,*ui(c)->font_title,0,w->x,w->y);
}
static void start(const PharaohInfoMonumentsLiveContext *c,int32_t help,int32_t group)
{
    PharaohBuildingInfoLiveState *w=ui(c)->window;w->help_id=help;
    if(w->first_draw){(void)pharaoh_native_return_zero_0047d350();w->first_draw=0;}
    frame(c,group,0);
}
static void workers(const PharaohInfoMonumentsLiveContext *c)
{
    const PharaohBuildingInfoLiveState *w=ui(c)->window;
    pharaoh_info_layout_draw_inner_panel(draw(c)->layout,w->x+16,w->y+136,w->width_blocks-2,4,w->x,w->y);
    pharaoh_info_draw_employment(draw(c),ui(c)->window->y+142);
}
static void no_road(const PharaohInfoMonumentsLiveContext *c,int32_t dy)
{ text(c,69,25,dy); }
static void described(const PharaohInfoMonumentsLiveContext *c,int32_t help,int32_t group)
{ start(c,help,group);text(c,group,1,ui(c)->window->height_blocks*16-158); }

/* Existing full algorithms, shared without duplicate Native owners. */
void pharaoh_info_monuments_draw_bullfight_school(const PharaohInfoMonumentsLiveContext *c)
{ pharaoh_info_services_draw_performer_school(c->services,78); }
void pharaoh_info_monuments_draw_fig_farm(const PharaohInfoMonumentsLiveContext *c)
{ pharaoh_info_industry_draw_farm(c->industry,90,6,183); }
void pharaoh_info_monuments_draw_pomegranate_farm(const PharaohInfoMonumentsLiveContext *c)
{ pharaoh_info_industry_draw_farm(c->industry,90,5,114); }
void pharaoh_info_monuments_draw_chickpea_farm(const PharaohInfoMonumentsLiveContext *c)
{ pharaoh_info_industry_draw_farm(c->industry,90,4,182); }
void pharaoh_info_monuments_draw_wood_cutter(const PharaohInfoMonumentsLiveContext *c)
{ pharaoh_info_industry_draw_raw_producer(c->industry,94,20,120,1); }
void pharaoh_info_monuments_draw_papyrus_maker(const PharaohInfoMonumentsLiveContext *c)
{ pharaoh_info_industry_draw_workshop(c->industry,97,23,190,22,0,23); }
void pharaoh_info_monuments_draw_lamp_maker(const PharaohInfoMonumentsLiveContext *c)
{ pharaoh_info_industry_draw_workshop(c->industry,473,34,314,31,13,34); }
void pharaoh_info_monuments_draw_paint_maker(const PharaohInfoMonumentsLiveContext *c)
{ pharaoh_info_industry_draw_workshop(c->industry,470,33,313,32,0,33); }
void pharaoh_info_monuments_draw_chariot_maker(const PharaohInfoMonumentsLiveContext *c)
{ pharaoh_info_industry_draw_workshop(c->industry,98,28,185,20,0,20); }
void pharaoh_info_monuments_draw_brickworks(const PharaohInfoMonumentsLiveContext *c)
{ pharaoh_info_industry_draw_workshop(c->industry,364,12,180,11,9,12); }
void pharaoh_info_monuments_draw_jeweler(const PharaohInfoMonumentsLiveContext *c)
{ pharaoh_info_industry_draw_workshop(c->industry,99,19,125,18,0,19); }
void pharaoh_info_monuments_draw_potter(const PharaohInfoMonumentsLiveContext *c)
{ pharaoh_info_industry_draw_workshop(c->industry,1,13,126,11,0,13); }

/* Whole4FFA20..500530 military-building branches. */
void pharaoh_info_monuments_draw_fort(const PharaohInfoMonumentsLiveContext *c)
{
    const int32_t formation=ui(c)->window->formation_id;
    start(c,37,89);text(c,89,ui(c)->formations[formation].cursed_by_seth!=0?1:2,ui(c)->window->height_blocks*16-158);
}
void pharaoh_info_monuments_draw_mud_gatehouse(const PharaohInfoMonumentsLiveContext *c)
{ described(c,85,90); }
void pharaoh_info_monuments_draw_brick_gatehouse(const PharaohInfoMonumentsLiveContext *c)
{ described(c,85,167); }
void pharaoh_info_monuments_draw_clay_gatehouse(const PharaohInfoMonumentsLiveContext *c)
{ described(c,85,168); }
void pharaoh_info_monuments_draw_roadblock(const PharaohInfoMonumentsLiveContext *c)
{ described(c,358,155); }
static void tower(const PharaohInfoMonumentsLiveContext *c,int32_t group,int32_t dy)
{
    start(c,85,group);
    if(ui(c)->window->details.road_access<1)no_road(c,dy);
    else text(c,group,building(c)->num_workers<1?2:building(c)->walker_id<1?4:3,dy);
}
void pharaoh_info_monuments_draw_mud_tower(const PharaohInfoMonumentsLiveContext *c)
{ tower(c,91,56);workers(c); }
void pharaoh_info_monuments_draw_brick_tower(const PharaohInfoMonumentsLiveContext *c)
{ tower(c,169,56);workers(c); }
void pharaoh_info_monuments_draw_clay_tower(const PharaohInfoMonumentsLiveContext *c)
{ tower(c,170,50);text(c,170,1,220);workers(c); }
void pharaoh_info_monuments_draw_military_academy(const PharaohInfoMonumentsLiveContext *c)
{
    start(c,88,135);
    if(ui(c)->window->details.road_access<1)no_road(c,56);
    else text(c,135,building(c)->num_workers<1?2:building(c)->num_workers<draw(c)->models[building(c)->building_type].laborers?1:3,56);
    workers(c);
}
static void recruiter_stock(const PharaohInfoMonumentsLiveContext *c,int32_t resource,int32_t loads,
    int32_t one,int32_t more,int32_t icon_y,int32_t text_y)
{
    const PharaohBuildingInfoLiveState *w=ui(c)->window;const int32_t items=loads/100;
    image(c,add32(*ui(c)->resource_images,resource),w->x+30,w->y+icon_y);
    *ui(c)->text_width=0;number(c,items,draw(c)->number_suffix,w->x+58,w->y+text_y);
    language(c,136,items==1?one:more,w->x+58+*ui(c)->text_width,w->y+text_y);
}
void pharaoh_info_monuments_draw_recruiter(const PharaohInfoMonumentsLiveContext *c)
{
    start(c,88,136);recruiter_stock(c,10,building(c)->storage_link_amount,15,2,38,44);
    if(building(c)->secondary_input_stock!=0)recruiter_stock(c,28,building(c)->secondary_input_stock,14,13,64,70);
    if(ui(c)->window->details.road_access<1)no_road(c,90);
    else {
        const PharaohBuildingTypeRecord *b=building(c);int32_t entry;
        if(b->num_workers<1)entry=3;else if(ui(c)->window->has_academy==0)entry=4;
        else {const int32_t p=ui(c)->window->details.worker_percentage;entry=(b->storage_link_amount<1?5:9)+(p>=100?0:p>=66?1:p>=33?2:3);}
        text(c,136,entry,90);
    }
    workers(c);
}

_Static_assert(offsetof(PharaohBuildingTypeRecord,farm_progress)==0x54&&
    offsetof(PharaohBuildingTypeRecord,runtime_value_b4)==0xb4&&
    offsetof(PharaohBuildingTypeRecord,shipwright_boat_id)==0xb6&&
    offsetof(PharaohBuildingTypeRecord,linked_ferry_building_id)==0xc4,
    "native shipyard progress/order/boat and ferry link fields");
static int32_t has_road(const PharaohInfoMonumentsLiveContext *c,int32_t id)
{
    const PharaohBuildingTypeRecord *b=&ui(c)->buildings[id];
    return pharaoh_map_has_road_access(c->road_access,b->x,b->y,b->size);
}
static int32_t boat_needed(const PharaohInfoMonumentsLiveContext *c,int32_t *type)
{
    const PharaohShipwrightDemandBorrowedFields fields={(uint8_t *)ui(c)->buildings,
        sizeof(PharaohBuildingTypeRecord),offsetof(PharaohBuildingTypeRecord,state),
        offsetof(PharaohBuildingTypeRecord,building_type),offsetof(PharaohBuildingTypeRecord,num_workers),
        offsetof(PharaohBuildingTypeRecord,shipwright_boat_id),offsetof(PharaohBuildingTypeRecord,runtime_value_b4)};
    return pharaoh_shipwright_boat_needed_borrowed(&fields,PHARAOH_BUILDING_RECORD_COUNT,ui(c)->walkers,type);
}
static int32_t shipwright_state(const PharaohInfoMonumentsLiveContext *c)
{
    const int32_t order=building(c)->runtime_value_b4;
    switch(order&0xf000) {
    case 0: {
        int32_t needed;
        if(boat_needed(c,&needed)==0)return 4;
        return building(c)->storage_link_amount<100&&needed!=25?9:-1;
    }
    case 0x1000: {
        /* Signed WORD*100/160 cannot encounter an IDIV fault. */
        const int32_t progress=pharaoh_scale_value_to_percent(building(c)->farm_progress,160).value;
        const PharaohBuildingInfoLiveState *w=ui(c)->window;
        *ui(c)->text_width=0;language(c,100,2,w->x+32,w->y+62);
        number(c,progress,c->industry->percent_suffix,w->x+32+*ui(c)->text_width,w->y+62);
        language(c,100,3,w->x+32+*ui(c)->text_width,w->y+62);
        return building(c)->storage_link_amount<100&&(order&0xfff)!=25?9:5;
    }
    case 0x2000:return building(c)->storage_link_amount<1?10:6;
    default:return -1;
    }
}
/* Full replacements of the old semantic ship emitters at502F70..503C00.
 * All children draw/query the actual shared state. No emit fallback. */
void pharaoh_info_monuments_draw_shipwright(const PharaohInfoMonumentsLiveContext *c)
{
    start(c,82,100);const PharaohBuildingInfoLiveState *w=ui(c)->window;
    image(c,add32(*ui(c)->resource_images,20),w->x+30,w->y+38);
    *ui(c)->text_width=0;language(c,100,7,w->x+58,w->y+44);
    const int32_t stock=building(c)->storage_link_amount;
    pharaoh_info_text_draw_amount(draw(c)->text,draw(c)->font_black,draw(c)->number_suffix,8,10,
        stock<1?0:stock,w->x+58+*ui(c)->text_width,w->y+44,*ui(c)->font_black,0);
    if(w->details.road_access<1)no_road(c,62);
    else {const int32_t entry=shipwright_state(c);if(entry>=0)multiline(c,100,entry,w->x+32,w->y+86,(w->width_blocks-6)*16);}
    workers(c);
}
void pharaoh_info_monuments_draw_dock(const PharaohInfoMonumentsLiveContext *c)
{
    start(c,83,101);
    if(ui(c)->window->details.road_access<1)no_road(c,56);
    else {const int32_t p=ui(c)->window->details.worker_percentage;
        text(c,101,(building(c)->runtime_value_b4==0?6:2)+(p<1?0:p<50?1:p<75?2:3),56);}
    workers(c);
}
void pharaoh_info_monuments_draw_fishing_wharf(const PharaohInfoMonumentsLiveContext *c)
{
    start(c,84,102);image(c,add32(*ui(c)->resource_images,7),ui(c)->window->x+10,ui(c)->window->y+10);
    if(ui(c)->window->details.road_access<1)no_road(c,56);
    else {
        const int32_t boat=building(c)->runtime_value_b4;int32_t entry;
        if(c->industry->mothball_banks[*c->industry->current_player].disabled[7]!=0)entry=9;
        else if(boat<1)entry=2;
        else switch(ui(c)->walkers[boat].action_state) {
            case 9:entry=3;break;case 10:entry=4;break;case 11:entry=5;break;
            case 13:entry=7;break;case 12:entry=*c->map_sweep_countdown<1?6:10;break;
            default:entry=8;break;
        }
        text(c,102,entry,56);
    }
    workers(c);
}
static void wharf(const PharaohInfoMonumentsLiveContext *c,int32_t help,int32_t group)
{ start(c,help,group);if(has_road(c,ui(c)->window->building_id)==0)no_road(c,40);text(c,group,1,76);workers(c); }
void pharaoh_info_monuments_draw_transport_wharf(const PharaohInfoMonumentsLiveContext *c)
{ wharf(c,357,174); }
void pharaoh_info_monuments_draw_warship_wharf(const PharaohInfoMonumentsLiveContext *c)
{ wharf(c,356,175); }
void pharaoh_info_monuments_draw_ferry(const PharaohInfoMonumentsLiveContext *c)
{
    PharaohBuildingInfoLiveState *w=ui(c)->window;w->help_id=58;w->first_draw=0;
    const int32_t other=building(c)->linked_ferry_building_id;frame(c,159,0);
    if(has_road(c,w->building_id)==0)no_road(c,60);
    else {
        int32_t entry;
        if(other<1)entry=1;else if(has_road(c,other)==0)entry=2;
        else entry=ui(c)->buildings[other].num_workers!=0?-1:3;
        if(entry>=0)text(c,159,entry,60);
    }
    workers(c);
}

_Static_assert(sizeof(PharaohInfoMonumentPlayerRecord)==0x49d8&&
    offsetof(PharaohInfoMonumentPlayerRecord,stockpiled)==0x2a88&&
    offsetof(PharaohInfoMonumentPlayerRecord,sphinx_stage)==0x433a&&
    offsetof(PharaohInfoMonumentPlayerRecord,monument_buildings)==0x473c&&
    offsetof(PharaohInfoMonumentPlayerRecord,available)==0x48f0,"native monument player bank");
_Static_assert(sizeof(PharaohInfoMonumentTextResources)==8&&sizeof(PharaohInfoScriptedMonumentReport)==32&&
    sizeof(PharaohInfoLibraryReport)==56&&sizeof(PharaohInfoAbuSimbelReport)==16&&
    sizeof(PharaohInfoMausoleumReport)==48&&offsetof(PharaohBuildingTypeRecord,runtime_value_016)==0x16&&
    offsetof(PharaohBuildingTypeRecord,monument_stage)==0x58&&
    offsetof(PharaohBuildingTypeRecord,royal_tomb_progress)==0x60,
    "native monument report extents and signed sequence/part-stage overlays");
static const PharaohInfoMonumentPlayerRecord *monument_player(const PharaohInfoMonumentsLiveContext *c)
{ return &c->players[*c->current_player]; }
static uint8_t resource_unavailable(const PharaohInfoMonumentsLiveContext *c,int32_t resource,int32_t amount)
{ return pharaoh_monument_resource_unavailable(monument_player(c)->available,monument_player(c)->stockpiled,resource,amount); }
/* Whole4F5580. The default only clears AL; all Native UI consumers test AL. */
uint8_t pharaoh_info_monuments_guild_serves(const PharaohInfoMonumentsLiveContext *c,int32_t guild,int32_t definition)
{
    const int32_t *resources;int32_t count;
    static const int32_t carpenter[]={20},bricklayer[]={12},stonemason[]={24,25,26,30,34},artisan[]={33,11};
    switch(guild){case 177:resources=carpenter;count=1;break;case 178:resources=bricklayer;count=1;break;
        case 179:resources=stonemason;count=5;break;case 231:resources=artisan;count=2;break;default:return 0;}
    for(int32_t i=0;i<count;++i)if(pharaoh_monument_definition_uses_resource(c->definitions,definition,resources[i]))return 1;
    return 0;
}
/* Whole4F5BD0, including genuine whole progress children and local reports. */
uint8_t pharaoh_info_monuments_needs_resource(const PharaohInfoMonumentsLiveContext *c,int32_t id,int32_t resource)
{
    const int32_t type=ui(c)->buildings[id].building_type;
    if(type==183) {
        PharaohPyramidProgressReport report;pharaoh_pyramid_build_progress_report(c->pyramid,id,&report);
        switch(*c->pyramid->style) {
        case 1:case 4:return resource==25?report.casingRemaining>0:resource==24&&report.mainRemaining>0;
        case 2:return resource==25?report.casingRemaining>0:resource==12&&report.mainRemaining>0;
        case 3:return resource==24&&(report.mainRemaining>0||report.casingRemaining>0);
        case 5:return resource==12&&(report.mainRemaining>0||report.casingRemaining>0);
        default:return 0;
        }
    }
    if(type==213&&resource==30){int32_t report[14];(void)pharaoh_sun_temple_progress(c->complex,id,report);return report[11]!=0;}
    if(type==222&&resource==30){int32_t report[12];(void)pharaoh_mausoleum_progress(c->complex,id,report);return report[7]!=0;}
    return 0;
}
/* Whole4F56D0. Its historical 'can build next/available' name obscured that
 * Native returns1 for a currently needed material blocked by4F5770. */
uint8_t pharaoh_info_monuments_pyramid_material_blocked(const PharaohInfoMonumentsLiveContext *c,
    int32_t definition,const PharaohPyramidProgressReport *report,int32_t id)
{
    for(int32_t k=0;k<3;++k) {
        const int32_t resource=c->definitions[definition].resources[k];
        if(resource==0||resource==20||!pharaoh_info_monuments_needs_resource(c,id,resource))continue;
        if(resource_unavailable(c,resource,resource==25?report->materialCasing:report->materialMain))return 1;
    }
    return 0;
}
/* Whole4F5D10. The loop's logical result is AL; native unused high EAX bits
 * from counts/slot results do not become another argument or return value. */
uint8_t pharaoh_info_monuments_has_ramp_work(const PharaohInfoMonumentsLiveContext *c,int32_t id)
{
    if(*c->pyramid->style==5)return 0;
    const uint8_t initial=ui(c)->buildings[id].pyramid_size_variant;
    for(int32_t i=0;i<c->pyramid->descriptor_counts[initial];++i) {
        const uint8_t variant=ui(c)->buildings[id].pyramid_size_variant;
        const int32_t part=add32(id,c->pyramid->descriptors[(uint32_t)variant*48u+(uint32_t)i].part);
        if(pharaoh_pyramid_available_worker_slot(ui(c)->buildings,part,(uint32_t)*c->pyramid->style,
            c->pyramid->descriptor_counts,c->pyramid->descriptors)!=0)return 1;
    }
    return 0;
}
typedef struct PharaohInfoObeliskSlotState {
    uint8_t reserved_00[12];int32_t work_remaining;
    uint8_t reserved_10[2];int16_t walker_id;
} PharaohInfoObeliskSlotState;
_Static_assert(offsetof(PharaohInfoObeliskSlotState,work_remaining)==0xc&&
    offsetof(PharaohInfoObeliskSlotState,walker_id)==0x12,"native obelisk slot state fields");
/* Whole4E6150. Stage/type metadata rows are the actual16-byte rows; their
 * pointer refers to the real mutable slot record, not a copied assignment. */
uint8_t pharaoh_info_monuments_obelisk_has_free_spot(const PharaohInfoMonumentsLiveContext *c,int32_t walker_type,int32_t id)
{
    for(int32_t i=0;;++i) {
        const int small=ui(c)->buildings[id].building_type==207;
        if(i>=(small?10:18))return 0;
        const PharaohMonumentWorkAssignment *spot=&(small?c->small_obelisk_spots:c->large_obelisk_spots)[i];
        const PharaohInfoObeliskSlotState *state=(const PharaohInfoObeliskSlotState *)spot->building;
        if(state->walker_id==0&&spot->monument_id==pharaoh_building_obelisk_part_number(ui(c)->buildings,id)&&
            spot->worker_type==walker_type&&state->work_remaining>0)return 1;
    }
}

/* Genuine typed ABI adapters used by the existing whole monument UI C. */
static void mon_native_language(const PharaohInfoMonumentsLiveContext *c,int32_t group,int32_t entry,
    int32_t x,int32_t y,int32_t font,uint16_t color,int32_t ox,int32_t oy)
{ pharaoh_ui_text_draw_language(draw(c)->text,draw(c)->font_black,group,entry,x,y,font,color,ox,oy); }
static void mon_native_centered(const PharaohInfoMonumentsLiveContext *c,int32_t group,int32_t entry,
    int32_t x,int32_t y,int32_t width,int32_t font,uint16_t color,int32_t ox,int32_t oy)
{ pharaoh_ui_text_draw_centered_language(draw(c)->text,draw(c)->font_title,group,entry,x,y,width,font,color,ox,oy); }
static void mon_native_multiline(const PharaohInfoMonumentsLiveContext *c,int32_t group,int32_t entry,
    int32_t x,int32_t y,int32_t width,int32_t font,uint16_t color,int32_t ox,int32_t oy)
{ pharaoh_info_ui_draw_multiline(draw(c)->layout,draw(c)->wrapped,draw(c)->font_black,group,entry,x,y,width,font,color,ox,oy); }
static void mon_native_number(const PharaohInfoMonumentsLiveContext *c,int32_t value,char prefix,const char *suffix,
    int32_t x,int32_t y,int32_t font,uint16_t color,int32_t ox,int32_t oy)
{ pharaoh_ui_text_draw_number(draw(c)->text,draw(c)->font_black,value,prefix,suffix,x,y,font,color,ox,oy); }
static void mon_native_amount(const PharaohInfoMonumentsLiveContext *c,int32_t group,int32_t entry,int32_t value,
    int32_t x,int32_t y,int32_t font,uint16_t color)
{ pharaoh_info_text_draw_amount(draw(c)->text,draw(c)->font_black,draw(c)->number_suffix,group,entry,value,x,y,font,color); }
static void mon_native_outer(const PharaohInfoMonumentsLiveContext *c,int32_t x,int32_t y,int32_t width,int32_t height)
{ pharaoh_panel_frame_draw_native(draw(c)->layout->outer_panel,x,y,width,height); }
static void mon_native_raw_text(const PharaohInfoMonumentsLiveContext *c,const char *s,int32_t x,int32_t y,
    int32_t width,int32_t font,uint16_t color)
{
    PharaohBitmapWrappedNativeContext wrapped=*draw(c)->wrapped->drawing;
    wrapped.font=draw(c)->font_black;wrapped.font_id=(uint32_t)font;
    pharaoh_bitmap_text_draw_wrapped_native(&wrapped,(const uint8_t *)s,x,y,width,color);
}
static const char *mon_language_string(const PharaohInfoMonumentsLiveContext *c,int32_t group,int32_t entry)
{
    PharaohTextSelection *s=draw(c)->text->selection;const char *saved=s->current;
    pharaoh_text_select(s,group,entry);const char *result=s->current;s->current=saved;return result;
}
static void mon_expand(const PharaohInfoMonumentsLiveContext *c,const char *s,char *out,int32_t size)
{ pharaoh_event_text_expand(c->token_entries,c->token_operations,s,out,size); }
static int32_t mon_building_main(const PharaohInfoMonumentsLiveContext *c,int32_t id)
{ return pharaoh_building_follow_link(ui(c)->buildings,id); }
static int32_t mon_main_part(const PharaohInfoMonumentsLiveContext *c,int32_t id)
{ return add32(id,-(int32_t)ui(c)->buildings[id].subtype); }
static PharaohBuildingTypeRecord *mon_get_building(const PharaohInfoMonumentsLiveContext *c,int32_t id)
{ return &ui(c)->buildings[id]; }
static void mon_update_access(const PharaohInfoMonumentsLiveContext *c)
{ pharaoh_building_update_kingdom_access(c->kingdom_access); }
static int32_t mon_sun_progress(const PharaohInfoMonumentsLiveContext *c,int32_t id,int32_t *out)
{ return pharaoh_sun_temple_progress(c->complex,id,out); }
static int32_t mon_mausoleum_progress(const PharaohInfoMonumentsLiveContext *c,int32_t id,int32_t *out)
{ return pharaoh_mausoleum_progress(c->complex,id,out); }
static int32_t mon_library_progress(const PharaohInfoMonumentsLiveContext *c,int32_t id,int32_t *out)
{ return pharaoh_library_progress(c->library_progress,id,out); }
static int32_t mon_abu_progress(const PharaohInfoMonumentsLiveContext *c,int32_t id,int32_t *out)
{ return pharaoh_abu_simbel_progress(ui(c)->buildings,id,out); }
static void mon_sphinx_count(const PharaohInfoMonumentsLiveContext *c,int32_t type,int32_t *count)
{ pharaoh_sphinx_count_assigned(c->sphinx_assignments,type,count); }
static uint8_t mon_sphinx_working(const PharaohInfoMonumentsLiveContext *c,uint32_t type)
{ return pharaoh_sphinx_walker_is_working_on_borrowed(ui(c)->walkers,c->tile_building_ids,
    &ui(c)->buildings[0].building_type,sizeof(PharaohBuildingTypeRecord),type); }
static uint8_t mon_obelisk_working(const PharaohInfoMonumentsLiveContext *c,uint32_t type,int32_t id)
{ return pharaoh_obelisk_walker_is_working_on_borrowed(ui(c)->walkers,c->tile_building_ids,
    &ui(c)->buildings[0].building_type,sizeof(PharaohBuildingTypeRecord),type,id); }
static uint8_t mon_sphinx_free(const PharaohInfoMonumentsLiveContext *c,int32_t type)
{ return pharaoh_sphinx_has_free_spot_for_type(c->sphinx_assignments,monument_player(c)->sphinx_stage,type); }
static uint8_t mon_sun_free(const PharaohInfoMonumentsLiveContext *c,int32_t type,int32_t stage)
{ return pharaoh_sun_temple_has_free_spot_for_type(c->sun_temple_spots,stage,type); }
static uint8_t mon_library_free(const PharaohInfoMonumentsLiveContext *c,int32_t type,int32_t stage)
{ return pharaoh_library_has_free_spot_for_type(c->library_spots,stage,type); }
static uint8_t mon_abu_free(const PharaohInfoMonumentsLiveContext *c,int32_t type,int32_t stage)
{ return pharaoh_abu_simbel_has_free_spot_for_type(c->abu_simbel_spots,stage,type); }
static uint8_t mon_caesareum_free(const PharaohInfoMonumentsLiveContext *c,int32_t type,int32_t stage)
{ return pharaoh_caesareum_has_free_spot_for_type(c->caesareum_spots,stage,type); }
static uint8_t mon_lighthouse_free(const PharaohInfoMonumentsLiveContext *c,int32_t type,int32_t stage)
{ return pharaoh_lighthouse_has_free_spot_for_type(c->lighthouse_spots,stage,type); }
static void mon_sphinx_percentages(const PharaohInfoMonumentsLiveContext *c,int32_t *cp,int32_t *mp,
    int32_t *carpenters,int32_t *carpenters_done,int32_t *masons,int32_t *masons_done)
{ pharaoh_sphinx_get_work_percentages(c->sphinx_percent_spots,monument_player(c)->sphinx_stage,
    cp,mp,carpenters,carpenters_done,masons,masons_done); }
static void mon_obelisk_count(const PharaohInfoMonumentsLiveContext *c,int32_t type,int32_t *count,int32_t id)
{ pharaoh_obelisk_count_assigned_for_type(type,count,ui(c)->buildings[id].building_type,c->small_obelisk_counts,c->large_obelisk_counts); }
static int32_t mon_chambers(const PharaohInfoMonumentsLiveContext *c,int32_t type)
{
    (void)c;
    switch(type){case 229:return pharaoh_small_royal_tomb_chamber_count();case 234:return pharaoh_medium_royal_tomb_chamber_count();
        case 235:return pharaoh_large_royal_tomb_chamber_count();case 236:return pharaoh_grand_royal_tomb_chamber_count();default:return 0;}
}
typedef struct PharaohInfoTombObjectView { const PharaohInfoMonumentsLiveContext *context;int32_t type; } PharaohInfoTombObjectView;
static int32_t mon_object_chambers(void *object)
{ const PharaohInfoTombObjectView *v=object;return mon_chambers(v->context,v->type); }
static uint8_t mon_object_worker(void *object,int32_t id)
{ const PharaohInfoTombObjectView *v=object;return pharaoh_royal_tomb_worker_active(ui(v->context)->buildings,ui(v->context)->walkers,id); }
static int32_t mon_tomb_idle_query(const PharaohInfoMonumentsLiveContext *c,int32_t main)
{
    const int32_t type=building(c)->building_type;
    if(type!=229&&type!=234&&type!=235&&type!=236)return 0;
    PharaohInfoTombObjectView object={c,type};
    return pharaoh_royal_tomb_is_idle(&object,ui(c)->buildings,main,mon_object_chambers,mon_object_worker);
}
static int32_t mon_tomb_goals_query(const PharaohInfoMonumentsLiveContext *c)
{
    const int32_t type=building(c)->building_type;
    return (type==229||type==234||type==235||type==236)&&
        pharaoh_monument_resources_complete_thunk(c->victory_stored,c->victory_required)!=0;
}

/* Whole existing UIManagerBuildingInfoMonuments C, mechanically bound to the
 * actual typed contexts above. Source debug-only text counting is absent from
 * the pinned Native instructions and is not a replacement callback. */
typedef PharaohPyramidProgressReport PyramidProgress;
typedef PharaohInfoMonumentTextResources MonumentTextResources;
typedef PharaohInfoScriptedMonumentReport ScriptedMonumentInfoProgress;
typedef PharaohInfoLibraryProgress LibraryInfoProgress;
typedef PharaohInfoLibraryStock LibraryInfoStock;
typedef PharaohInfoAbuSimbelReport AbuSimbelInfoProgress;
typedef PharaohInfoMausoleumReport MausoleumInfoProgress;
typedef struct PyramidStyleRule { int32_t helpMessage,mainMaterial,casingMaterial,stonemasons,bricklayers; } PyramidStyleRule;
enum { PyramidStyleRuleOther,PyramidStyleRuleBent,PyramidStyleRuleMudbrick,PyramidStyleRuleStepped,PyramidStyleRulePyramid,PyramidStyleRuleMastaba,PyramidStyleRuleCount };
static const PyramidStyleRule pyramidStyleRules[PyramidStyleRuleCount]={
    {38,24,25,0,1},{54,24,25,1,0},{55,12,25,1,1},{375,24,-1,0,0},{38,24,25,1,0},{371,12,-1,0,1}};
static uint8_t mon_pyramid_blocked_value(const PharaohInfoMonumentsLiveContext *c,int32_t definition,
    PharaohPyramidProgressReport report,int32_t id)
{ return pharaoh_info_monuments_pyramid_material_blocked(c,definition,&report,id); }
enum monumentText {
    MonumentTextComplete = 0,
    MonumentTextFinishingOutside = 1,
    MonumentTextTaskIs = 2,
    MonumentTextClearingPyramidSite = 3,
    MonumentTextGroovesCut = 5,
    MonumentTextSitePrepared = 10,
    MonumentTextRoof = 11,
    MonumentTextForeman = 12,
    MonumentTextNoWorkCamp = 13,
    MonumentTextNoStonemasons = 14,
    MonumentTextNoBricklayers = 15,
    MonumentTextNoCarpenters = 16,
    MonumentTextNoPeasants = 17,
    MonumentTextStonemasonsNotComing = 18,
    MonumentTextBricklayersNotComing = 19,
    MonumentTextCarpentersNotComing = 20,
    MonumentTextWaitingForCarpenter = 21,
    MonumentTextNeedsStone = 22,
    MonumentTextNeedsLimestone = 23,
    MonumentTextNeedsSandstone = 24,
    MonumentTextNeedsGranite = 25,
    MonumentTextNeedsMarble = 26,
    MonumentTextNeedsBricks = 27,
    MonumentTextNeedsCopper = 29,
    MonumentTextObeliskCarpentersFinishing = 42,
    MonumentTextObeliskCarving = 43,
    MonumentTextObeliskComplete = 44,
    MonumentTextSphinxRoughShape = 45,
    MonumentTextSphinxFineCarving = 46,
    MonumentTextSphinxFinishing = 47,
    MonumentTextSphinxComplete = 48,
    MonumentTextCarpentryIs = 49,
    MonumentTextMasonWorkIs = 50,
    MonumentTextPeasantsOnFloodplain = 51,
    MonumentTextStagingAreaBlocked = 52,
    MonumentTextSunTempleBegin = 53,
    MonumentTextSunTempleScaffolding = 54,
    MonumentTextSunTempleObelisk = 55,
    MonumentTextSunTempleVestibule = 56,
    MonumentTextSunTempleComplete = 57,
    MonumentTextMausoleumBegin = 58,
    MonumentTextMausoleumWorking = 59,
    MonumentTextMausoleumComplete = 60,
    MonumentTextPercentComplete = 61,
    MonumentTextNoCourses = 62,
    MonumentTextOneCourse = 63,
    MonumentTextCourses = 64,
    MonumentTextCourseNeedsMain = 65,
    MonumentTextCourseNeedsCasing = 66,
    MonumentTextCourseNeedsBoth = 67,
    MonumentTextRemainderNeedsMain = 68,
    MonumentTextRemainderNeedsCasing = 69,
    MonumentTextRemainderNeedsBoth = 70,
    MonumentTextDoneMain = 71,
    MonumentTextDoneBoth = 72,
    MonumentTextMastaba = 73,
    MonumentTextPyramid = 74,
    MonumentTextSteppedPyramid = 75,
    MonumentTextBentPyramid = 76,
    MonumentTextBrickPyramid = 77,
    MonumentTextObelisk = 78,
    MonumentTextSphinx = 79,
    MonumentTextSunTemple = 80,
    MonumentTextLibrary = 81,
    MonumentTextAbuSimbel = 82,
    MonumentTextSmallRoyalTomb = 83,
    MonumentTextMediumRoyalTomb = 84,
    MonumentTextLargeRoyalTomb = 85,
    MonumentTextGrandRoyalTomb = 86,
    MonumentTextCaesareum = 87,
    MonumentTextLighthouse = 88,
    MonumentTextMausoleum = 89,
    MonumentTextPlainStoneBlocks = 90,
    MonumentTextLimestoneBlocks = 91,
    MonumentTextGraniteBlocks = 92,
    MonumentTextSandstoneBlocks = 93,
    MonumentTextBricks = 94,
    MonumentTextMarbleBlocks = 95,
    MonumentTextCopperIngots = 96,
    MonumentTextMausoleumNeedsCarpenter = 97,
    MonumentTextStoneStockpiled = 98,
    MonumentTextLimestoneStockpiled = 99,
    MonumentTextSandstoneStockpiled = 100,
    MonumentTextMarbleStockpiled = 101,
    MonumentTextGraniteStockpiled = 102,
    MonumentTextBricksStockpiled = 103,
    MonumentTextCopperStockpiled = 104,
    MonumentTextNoArtisans = 105,
    MonumentTextArtisansNotComing = 106,
    MonumentTextLibraryClearing = 114,
    MonumentTextCaesareumClearing = 122,
    MonumentTextLighthouseClearing = 132,
    MonumentTextLighthouseComplete = 140,
    MonumentTextAbuSimbelWorking = 141,
    MonumentTextAbuSimbelWalkers = 142,
    MonumentTextTombRefused = 143,
    MonumentTextTombWorking = 144,
    MonumentTextTombComplete = 145,
    MonumentTextTombReady = 146,
    MonumentTextLamps = 147,
    MonumentTextTombEntranceBlocked = 148,
    MonumentTextMonumentBlocked = 149,
};

enum monumentLayout {
    MonumentTextX = 32,
    MonumentTextY = 56,
    MonumentRequirementsY = 80,
    MonumentLineHeight = 24,
    MonumentTextWidthPerBlock = 15,
    TombLampsY = 60,
    TombLampsAmountX = 60,
    TombLampsWidthPerBlock = 6,
    ExpandedTextSize = 500,
    CoursesLineSize = 1000,
};

enum monumentValues {
    PyramidFirstSiteLevel = 2,
    PyramidLastEarlySiteLevel = 5,
    PyramidLastSiteLevel = 11,
    MastabaLastGrooveLevel = 10,
    SphinxStageComplete = 16,
    SphinxStageFineCarving = 7,
    SphinxStageFinishing = 13,
    MaterialLoadsPerBlock = 400,
    SandstoneBlocksPerMason = 5,
    SandstoneLoadsPerBlock = 80,
    LibraryMarbleStageEnd = 21,
    LibraryCopperStageEnd = 19,
    LibraryCopperStock = 800,
    CaesareumMarbleStageEnd = 18,
    CaesareumGraniteStageEnd = 19,
    CaesareumMarbleStock = 800,
    LighthouseStageDone = 9,
    AbuSimbelWalkersStage = 0x38,
};


static const AbuSimbelInfoProgress *monument_abu_simbel_report(const PharaohInfoMonumentsLiveContext *c);
static const ScriptedMonumentInfoProgress *monument_caesareum_report(const PharaohInfoMonumentsLiveContext *c);
static int32_t monument_caesareum_stage_text(const PharaohInfoMonumentsLiveContext *c,int32_t stage);
static void monument_draw_expanded(const PharaohInfoMonumentsLiveContext *c,int32_t index, int32_t x, int32_t y, int32_t width);
static void monument_draw_expanded_below(const PharaohInfoMonumentsLiveContext *c,int32_t index, int32_t x, int32_t y, int32_t width);
static void monument_draw_label(const PharaohInfoMonumentsLiveContext *c,int32_t index, int32_t x, int32_t y);
static void monument_draw_panel_title(const PharaohInfoMonumentsLiveContext *c);
static void monument_draw_percent_complete(const PharaohInfoMonumentsLiveContext *c,int32_t percentage, int32_t x, int32_t y);
static void monument_draw_pyramid_courses(const PharaohInfoMonumentsLiveContext *c,const PyramidProgress *report, int32_t x, int32_t y, int32_t width);
static void monument_draw_requirements_or_done(const PharaohInfoMonumentsLiveContext *c,int32_t percent, int32_t material, int32_t x, int32_t y, int32_t width);
static void monument_draw_scripted_monument_text(const PharaohInfoMonumentsLiveContext *c,int32_t text, int32_t x, int32_t y);
static void monument_draw_text(const PharaohInfoMonumentsLiveContext *c,int32_t index, int32_t x, int32_t y);
static void monument_draw_work_line(const PharaohInfoMonumentsLiveContext *c,int32_t label, int32_t percentage, int32_t x, int32_t y);
static void monument_expand_text(const PharaohInfoMonumentsLiveContext *c,int32_t index, char *buffer, int32_t size);
static int32_t monument_guild_serves(const PharaohInfoMonumentsLiveContext *c,int32_t guildType, int32_t def);
static int32_t monument_guild_text(const PharaohInfoMonumentsLiveContext *c,int32_t stonemasonsWanted, int32_t carpentersWanted, int32_t stonemasonsDelivered, int32_t carpentersDelivered);
static int32_t monument_has_ramp_work(const PharaohInfoMonumentsLiveContext *c);
static const LibraryInfoProgress *monument_library_report(const PharaohInfoMonumentsLiveContext *c);
static int32_t monument_library_stage_text(const PharaohInfoMonumentsLiveContext *c,int32_t stage);
static const LibraryInfoStock *monument_library_stock(const PharaohInfoMonumentsLiveContext *c);
static const ScriptedMonumentInfoProgress *monument_lighthouse_report(const PharaohInfoMonumentsLiveContext *c);
static int32_t monument_lighthouse_stage_text(const PharaohInfoMonumentsLiveContext *c,int32_t stage);
static int32_t monument_material_shortage_text(const PharaohInfoMonumentsLiveContext *c,int32_t resource);
static int32_t monument_mausoleum_building_text(const PharaohInfoMonumentsLiveContext *c,const MausoleumInfoProgress *report, int32_t sandstoneOk);
static const MausoleumInfoProgress *monument_mausoleum_report(const PharaohInfoMonumentsLiveContext *c);
static int32_t monument_mausoleum_stone_text(const PharaohInfoMonumentsLiveContext *c,const MausoleumInfoProgress *report, int32_t sandstoneOk);
static int32_t monument_monument_blocked(const PharaohInfoMonumentsLiveContext *c,int32_t BuildingId, int32_t recheckId);
static int32_t monument_monument_def_of_info_building(const PharaohInfoMonumentsLiveContext *c);
static int32_t monument_no_peasants_text(const PharaohInfoMonumentsLiveContext *c);
static int32_t monument_no_road_to_staging_area(const PharaohInfoMonumentsLiveContext *c,int32_t BuildingId);
static int32_t monument_obelisk_waiting_text(const PharaohInfoMonumentsLiveContext *c,int32_t carpentersAssigned, int32_t stonemasonsAssigned, uint8_t carpenterWorking, uint8_t stonemasonWorking,
    uint8_t stonemasonSpotFree, uint8_t carpenterSpotFree);
static int32_t monument_pyramid_blocks_text(const PharaohInfoMonumentsLiveContext *c,const PyramidProgress *report, const PyramidStyleRule *rule, int32_t def, int32_t x, int32_t y, int32_t *textY);
static int32_t monument_pyramid_finishing_text(const PharaohInfoMonumentsLiveContext *c,const PyramidProgress *report, const PyramidStyleRule *rule, int32_t def, int32_t x, int32_t *y);
static int32_t monument_pyramid_needs_available(const PharaohInfoMonumentsLiveContext *c,int32_t resource, int32_t amount);
static int32_t monument_pyramid_remainder_text(const PharaohInfoMonumentsLiveContext *c,const PyramidProgress *report);
static const PyramidProgress *monument_pyramid_report(const PharaohInfoMonumentsLiveContext *c);
static int32_t monument_pyramid_site_text(const PharaohInfoMonumentsLiveContext *c,const PyramidProgress *report, int32_t style);
static const PyramidStyleRule *monument_pyramid_style_rule_of(const PharaohInfoMonumentsLiveContext *c,int32_t style);
static int32_t monument_road_tile_blocked(const PharaohInfoMonumentsLiveContext *c,int32_t id);
static int32_t monument_sandstone_available(const PharaohInfoMonumentsLiveContext *c,int32_t stonemasonsWaiting);
static int32_t monument_sphinx_text(const PharaohInfoMonumentsLiveContext *c,int32_t carpentersAssigned, int32_t stonemasonsAssigned, uint8_t carpenterWorking, uint8_t stonemasonWorking, int32_t noRoad);
static void monument_start_monument_panel(const PharaohInfoMonumentsLiveContext *c,int32_t help);
static const int32_t *monument_sun_temple_report(const PharaohInfoMonumentsLiveContext *c);
static int32_t monument_sun_temple_stone_text(const PharaohInfoMonumentsLiveContext *c,uint8_t stonemasonSpotFree, uint8_t carpenterSpotFree);
static int32_t monument_sun_temple_temple_text(const PharaohInfoMonumentsLiveContext *c,uint32_t flags, int32_t sandstoneOk, int32_t y, int32_t *textY);
static MonumentTextResources *monument_text_resources(const PharaohInfoMonumentsLiveContext *c);
static int32_t monument_text_width(const PharaohInfoMonumentsLiveContext *c);
static int32_t monument_tomb_goals_reached(const PharaohInfoMonumentsLiveContext *c);
static int32_t monument_tomb_has_chamber_at(const PharaohInfoMonumentsLiveContext *c,int32_t mainId, int32_t chambers, int32_t firstStage, int32_t lastStage);
static int32_t monument_tomb_idle(const PharaohInfoMonumentsLiveContext *c,int32_t mainId);
static const int32_t *monument_tomb_report(const PharaohInfoMonumentsLiveContext *c);
static int32_t monument_tomb_work_text(const PharaohInfoMonumentsLiveContext *c,int32_t mainId, int32_t wantsStone, int32_t wantsArtisans);
void pharaoh_info_monuments_draw_abu_simbel(const PharaohInfoMonumentsLiveContext *c);
void pharaoh_info_monuments_draw_caesareum(const PharaohInfoMonumentsLiveContext *c);
void pharaoh_info_monuments_draw_great_library(const PharaohInfoMonumentsLiveContext *c);
void pharaoh_info_monuments_draw_lighthouse(const PharaohInfoMonumentsLiveContext *c);
void pharaoh_info_monuments_draw_mausoleum(const PharaohInfoMonumentsLiveContext *c);
void pharaoh_info_monuments_draw_obelisk(const PharaohInfoMonumentsLiveContext *c);
void pharaoh_info_monuments_draw_pyramid(const PharaohInfoMonumentsLiveContext *c);
void pharaoh_info_monuments_draw_royal_tomb(const PharaohInfoMonumentsLiveContext *c);
void pharaoh_info_monuments_draw_sphinx(const PharaohInfoMonumentsLiveContext *c);
void pharaoh_info_monuments_draw_sun_temple(const PharaohInfoMonumentsLiveContext *c);
static int32_t monument_work_camp_text(const PharaohInfoMonumentsLiveContext *c,int32_t laborersDelivered);
static int32_t monument_built(const PharaohInfoMonumentsLiveContext *c,int32_t type);
static int32_t monument_working(const PharaohInfoMonumentsLiveContext *c,int32_t type);

static int32_t monument_obelisk_waiting_text(const PharaohInfoMonumentsLiveContext *c,int32_t carpentersAssigned, int32_t stonemasonsAssigned, uint8_t carpenterWorking, uint8_t stonemasonWorking,
    uint8_t stonemasonSpotFree, uint8_t carpenterSpotFree)
{
    (void)c;
    if (!monument_built(c,179)) {
        return MonumentTextNoStonemasons;
    }
    if (!monument_built(c,177)) {
        return MonumentTextNoCarpenters;
    }
    if (stonemasonsAssigned == 0 && !stonemasonWorking && stonemasonSpotFree) {
        return MonumentTextStonemasonsNotComing;
    }
    if (carpentersAssigned == 0 && !carpenterWorking && carpenterSpotFree) {
        return MonumentTextCarpentersNotComing;
    }
    return -1;
}

static int32_t monument_monument_def_of_info_building(const PharaohInfoMonumentsLiveContext *c)
{
    (void)c;
    int32_t mainId = mon_building_main(c,ui(c)->window->building_id);
    for (int32_t slot = 0; slot < 3; slot++) {
        if (monument_player(c)->monument_buildings[slot] == mainId) {
            return c->goal_definition_ids[slot];
        }
    }
    return 0;
}

static int32_t monument_guild_text(const PharaohInfoMonumentsLiveContext *c,int32_t stonemasonsWanted, int32_t carpentersWanted, int32_t stonemasonsDelivered, int32_t carpentersDelivered)
{
    (void)c;
    if (stonemasonsWanted) {
        if (!monument_built(c,179)) {
            return MonumentTextNoStonemasons;
        }
        if (!monument_working(c,179) && stonemasonsDelivered == 0) {
            return MonumentTextStonemasonsNotComing;
        }
    }
    if (carpentersWanted) {
        if (!monument_built(c,177)) {
            return MonumentTextNoCarpenters;
        }
        if (!monument_working(c,177) && carpentersDelivered == 0) {
            return MonumentTextCarpentersNotComing;
        }
    }
    return 0;
}

static int32_t monument_no_road_to_staging_area(const PharaohInfoMonumentsLiveContext *c,int32_t BuildingId)
{
    (void)c;
    return monument_road_tile_blocked(c,BuildingId) || mon_get_building(c,BuildingId)->distance_from_entry == 0;
}

static void monument_draw_expanded_below(const PharaohInfoMonumentsLiveContext *c,int32_t index, int32_t x, int32_t y, int32_t width)
{
    (void)c;
    char buffer[ExpandedTextSize];
    monument_expand_text(c,index, buffer, ExpandedTextSize);
    mon_native_raw_text(c,buffer, x, (*c->draw_y_accum) + y, width, (*ui(c)->font_black), 0);
}

static int32_t monument_library_stage_text(const PharaohInfoMonumentsLiveContext *c,int32_t stage)
{
    (void)c;
    static const int32_t stageEnds[] = { 3, 6, 9, 12, 16, 20 };
    for (int32_t i = 0; i < (int32_t) (sizeof(stageEnds) / sizeof(stageEnds[0])); i++) {
        if (stage < stageEnds[i]) {
            return MonumentTextLibraryClearing + i;
        }
    }
    return (stage == 20) ? MonumentTextLibraryClearing + 6 : MonumentTextLibraryClearing + 7;
}

static void monument_draw_work_line(const PharaohInfoMonumentsLiveContext *c,int32_t label, int32_t percentage, int32_t x, int32_t y)
{
    (void)c;
    (*ui(c)->text_width) = 0;
    monument_draw_text(c,label, x, y);
    monument_draw_percent_complete(c,percentage, x, y);
}

static int32_t monument_mausoleum_stone_text(const PharaohInfoMonumentsLiveContext *c,const MausoleumInfoProgress *report, int32_t sandstoneOk)
{
    (void)c;
    if (!monument_built(c,199)) {
        return MonumentTextNoWorkCamp;
    }
    if (!monument_built(c,179)) {
        return MonumentTextNoStonemasons;
    }
    if (report->stonemasons == report->stonemasonsWaiting && !sandstoneOk) {
        return MonumentTextNeedsSandstone;
    }
    if (report->stonemasons == 0) {
        return MonumentTextStonemasonsNotComing;
    }
    if (report->stonemasonsWaiting == 0 || !sandstoneOk || report->material == 0) {
        return MonumentTextMausoleumWorking;
    }
    return (monument_player(c)->stockpiled[30] == 0) ? MonumentTextNeedsSandstone : MonumentTextSandstoneStockpiled;
}

static MonumentTextResources *monument_text_resources(const PharaohInfoMonumentsLiveContext *c)
{
    (void)c;
    return &(*c->text_resources);
}

static const ScriptedMonumentInfoProgress *monument_lighthouse_report(const PharaohInfoMonumentsLiveContext *c)
{
    (void)c;
    return &(*c->lighthouse_report);
}

static void monument_draw_pyramid_courses(const PharaohInfoMonumentsLiveContext *c,const PyramidProgress *report, int32_t x, int32_t y, int32_t width)
{
    (void)c;
    char line[CoursesLineSize];
    char second[ExpandedTextSize];
    int32_t courses = (report->height == 0) ? MonumentTextNoCourses : (report->height == 1) ? MonumentTextOneCourse : MonumentTextCourses;
    monument_expand_text(c,courses, line, CoursesLineSize);
    int32_t needs = (report->frontierMain != 0) ? ((report->frontierCasing != 0) ? MonumentTextCourseNeedsBoth : MonumentTextCourseNeedsMain)
                                                 : MonumentTextCourseNeedsCasing;
    monument_expand_text(c,needs, second, ExpandedTextSize);
    /* Exact Native append for the valid nonoverlapping, NUL-terminated
     * catalogue strings; preserve every byte including the final NUL. */
    char *tail=line;
    while(*tail!='\0')++tail;
    const char *part=second;
    do { *tail++=*part; } while(*part++!='\0');
    mon_native_raw_text(c,line, x, (*c->draw_y_accum) + y, width, (*ui(c)->font_black), 0);
}

static void monument_draw_expanded(const PharaohInfoMonumentsLiveContext *c,int32_t index, int32_t x, int32_t y, int32_t width)
{
    (void)c;
    char buffer[ExpandedTextSize];
    monument_expand_text(c,index, buffer, ExpandedTextSize);
    mon_native_raw_text(c,buffer, x, y, width, (*ui(c)->font_black), 0);
}

static int32_t monument_tomb_work_text(const PharaohInfoMonumentsLiveContext *c,int32_t mainId, int32_t wantsStone, int32_t wantsArtisans)
{
    (void)c;
    const int32_t *report = monument_tomb_report(c);
    int32_t text = monument_work_camp_text(c,report[2]);
    if (text == 0 && monument_tomb_idle(c,mainId)) {
        text = MonumentTextTombRefused;
    }
    if (text == 0 && wantsStone) {
        text = monument_guild_text(c,1, 0, report[1], 0);
    }
    if (text == 0 && wantsArtisans) {
        if (!monument_built(c,231)) {
            text = MonumentTextNoArtisans;
        } else if (!monument_working(c,231) && report[4] == 0) {
            text = MonumentTextArtisansNotComing;
        }
    }
    return (text == 0) ? MonumentTextTombWorking : text;
}

static int32_t monument_pyramid_finishing_text(const PharaohInfoMonumentsLiveContext *c,const PyramidProgress *report, const PyramidStyleRule *rule, int32_t def, int32_t x, int32_t *y)
{
    (void)c;
    if (!monument_built(c,179) && (!rule->bricklayers || rule->stonemasons) && monument_guild_serves(c,179, def)) {
        return MonumentTextNoStonemasons;
    }
    if (report->stonemasons == 0 && monument_guild_serves(c,179, def)) {
        return MonumentTextStonemasonsNotComing;
    }
    (*ui(c)->text_width) = 0;
    monument_draw_label(c,MonumentTextFinishingOutside, x, *y);
    *y += MonumentLineHeight;
    (*ui(c)->text_width) = 0;
    monument_draw_label(c,MonumentTextTaskIs, x, *y);
    monument_draw_percent_complete(c,report->phasePercent, x, *y);
    return -1;
}

static int32_t monument_work_camp_text(const PharaohInfoMonumentsLiveContext *c,int32_t laborersDelivered)
{
    (void)c;
    if (!monument_built(c,199)) {
        return MonumentTextNoWorkCamp;
    }
    if (!monument_working(c,199) && laborersDelivered == 0) {
        return MonumentTextNoPeasants;
    }
    return 0;
}

static int32_t monument_sun_temple_stone_text(const PharaohInfoMonumentsLiveContext *c,uint8_t stonemasonSpotFree, uint8_t carpenterSpotFree)
{
    (void)c;
    const int32_t *report = monument_sun_temple_report(c);
    if (stonemasonSpotFree) {
        if (!monument_built(c,179)) {
            return MonumentTextNoStonemasons;
        }
        if (!monument_working(c,179) && report[7] == 0) {
            return MonumentTextStonemasonsNotComing;
        }
        return MonumentTextSunTempleObelisk;
    }
    if (!carpenterSpotFree) {
        return MonumentTextSunTempleObelisk;
    }
    if (!monument_built(c,177)) {
        return MonumentTextNoCarpenters;
    }
    if (!monument_working(c,177) && report[10] == 0) {
        return MonumentTextCarpentersNotComing;
    }
    return MonumentTextSunTempleObelisk;
}

static int32_t monument_built(const PharaohInfoMonumentsLiveContext *c,int32_t type)
{
    (void)c;
    return c->building_count_total[type] != 0;
}

static const int32_t *monument_tomb_report(const PharaohInfoMonumentsLiveContext *c)
{
    (void)c;
    return c->tomb_report;
}

static int32_t monument_guild_serves(const PharaohInfoMonumentsLiveContext *c,int32_t guildType, int32_t def)
{
    (void)c;
    return (uint8_t) pharaoh_info_monuments_guild_serves(c,guildType, def) != 0;
}

static void monument_draw_label(const PharaohInfoMonumentsLiveContext *c,int32_t index, int32_t x, int32_t y)
{
    (void)c;
    mon_native_language(c,178, index, x, y, (*ui(c)->font_black), 0, ui(c)->window->x, ui(c)->window->y);
}

static int32_t monument_tomb_idle(const PharaohInfoMonumentsLiveContext *c,int32_t mainId)
{
    (void)c; return mon_tomb_idle_query(c,mainId); }

static void monument_draw_percent_complete(const PharaohInfoMonumentsLiveContext *c,int32_t percentage, int32_t x, int32_t y)
{
    (void)c;
    mon_native_number(c,percentage, 64, c->industry->percent_suffix, (*ui(c)->text_width) + x, y, (*ui(c)->font_black), 0, ui(c)->window->x, ui(c)->window->y);
    monument_draw_label(c,MonumentTextComplete, (*ui(c)->text_width) + x, y);
}

static const LibraryInfoStock *monument_library_stock(const PharaohInfoMonumentsLiveContext *c)
{
    (void)c;
    return &(*c->library_report).stock;
}

static const LibraryInfoProgress *monument_library_report(const PharaohInfoMonumentsLiveContext *c)
{
    (void)c;
    return &(*c->library_report).progress;
}

static const PyramidProgress *monument_pyramid_report(const PharaohInfoMonumentsLiveContext *c)
{
    (void)c;
    return &(*c->pyramid_report);
}

static int32_t monument_road_tile_blocked(const PharaohInfoMonumentsLiveContext *c,int32_t id)
{
    (void)c; const PharaohBuildingTypeRecord *b=mon_get_building(c,id); return c->land_citizen[4*(b->road_access_x+228*b->road_access_y)]<0; }

static int32_t monument_tomb_goals_reached(const PharaohInfoMonumentsLiveContext *c)
{
    (void)c; return mon_tomb_goals_query(c); }

static void monument_start_monument_panel(const PharaohInfoMonumentsLiveContext *c,int32_t help)
{
    (void)c; ui(c)->window->help_id=help; if(ui(c)->window->first_draw){ if(help!=481&&help!=492)(void)pharaoh_native_return_zero_0047d350(); ui(c)->window->first_draw=0; } monument_draw_panel_title(c); }

static int32_t monument_lighthouse_stage_text(const PharaohInfoMonumentsLiveContext *c,int32_t stage)
{
    (void)c;
    if (stage < 0) {
        return MonumentTextLighthouseClearing;
    }
    switch (stage) {
        case 0:
            return MonumentTextLighthouseClearing + 1;
        case 1:
            return MonumentTextLighthouseClearing + 2;
        case 2:
        case 3:
            return MonumentTextLighthouseClearing + 3;
        case 4:
            return MonumentTextLighthouseClearing + 4;
        case 5:
        case 6:
            return MonumentTextLighthouseClearing + 5;
        case 7:
            return MonumentTextLighthouseClearing + 6;
        case 8:
            return MonumentTextLighthouseClearing + 7;
        default:
            return -1;
    }
}

static int32_t monument_pyramid_blocks_text(const PharaohInfoMonumentsLiveContext *c,const PyramidProgress *report, const PyramidStyleRule *rule, int32_t def, int32_t x, int32_t y, int32_t *textY)
{
    (void)c;
    (*c->draw_y_accum) = 0;
    int32_t mainId = mon_building_main(c,ui(c)->window->building_id);
    PyramidProgress copy;
    memcpy(&copy, &(*c->pyramid_report), sizeof(copy));
    uint8_t canBuild = mon_pyramid_blocked_value(c,def, copy, mainId);
    int32_t stonemasonsTested = !rule->bricklayers || rule->stonemasons;
    if (!monument_built(c,199)) {
        return MonumentTextNoWorkCamp;
    }
    if (!monument_built(c,179) && stonemasonsTested && monument_guild_serves(c,179, def)) {
        return MonumentTextNoStonemasons;
    }
    if (!monument_built(c,178) && rule->bricklayers && monument_guild_serves(c,178, def)) {
        return MonumentTextNoBricklayers;
    }
    if (report->blockWorkers == report->waiting && !canBuild) {
        return MonumentTextNoPeasants;
    }
    int32_t masonsWanted = (!rule->bricklayers && report->frontierMain != 0) || (rule->stonemasons && report->frontierCasing != 0);
    if (report->stonemasons == 0 && masonsWanted && monument_guild_serves(c,179, def)) {
        return MonumentTextStonemasonsNotComing;
    }
    if (report->bricklayers == 0 && rule->bricklayers && report->frontierMain != 0 && monument_guild_serves(c,178, def)) {
        return MonumentTextBricklayersNotComing;
    }
    if (!monument_built(c,177) && monument_has_ramp_work(c) && monument_guild_serves(c,177, def)) {
        return MonumentTextNoCarpenters;
    }
    if (report->carpenters == 0 && monument_has_ramp_work(c) && monument_guild_serves(c,177, def)) {
        return MonumentTextWaitingForCarpenter;
    }
    if (report->waiting == 0) {
        return -1;
    }
    if (monument_pyramid_needs_available(c,rule->mainMaterial, report->materialMain)) {
        monument_draw_text(c,monument_material_shortage_text(c,rule->mainMaterial), x, (*c->draw_y_accum) + y);
    }
    if (rule->casingMaterial != -1 && monument_pyramid_needs_available(c,rule->casingMaterial, report->materialCasing)) {
        *textY = (*c->draw_y_accum) + y;
        return monument_material_shortage_text(c,rule->casingMaterial);
    }
    return -1;
}

static int32_t monument_mausoleum_building_text(const PharaohInfoMonumentsLiveContext *c,const MausoleumInfoProgress *report, int32_t sandstoneOk)
{
    (void)c;
    if (mon_mausoleum_progress(c,ui(c)->window->building_id, 0) >= 100) {
        return MonumentTextMausoleumComplete;
    }
    if (report->material == 0) {
        if (report->partsBelowStage2 == 0) {
            return MonumentTextMausoleumWorking;
        }
        if (!monument_built(c,199)) {
            return MonumentTextNoWorkCamp;
        }
        return (report->laborers == 0) ? MonumentTextNoPeasants : MonumentTextMausoleumWorking;
    }
    if (!monument_built(c,199)) {
        return MonumentTextNoWorkCamp;
    }
    if (!monument_built(c,179)) {
        return MonumentTextNoStonemasons;
    }
    if (report->blocksLeft != 0) {
        return MonumentTextMausoleumNeedsCarpenter;
    }
    if (report->stonemasons == report->stonemasonsWaiting && !sandstoneOk) {
        return MonumentTextNeedsSandstone;
    }
    if (report->stonemasons == 0) {
        return MonumentTextStonemasonsNotComing;
    }
    return (report->stonemasonsWaiting != 0 && sandstoneOk) ? MonumentTextNeedsSandstone : MonumentTextMausoleumWorking;
}

static int32_t monument_working(const PharaohInfoMonumentsLiveContext *c,int32_t type)
{
    (void)c;
    return c->building_count_active[type] != 0;
}

static int32_t monument_pyramid_remainder_text(const PharaohInfoMonumentsLiveContext *c,const PyramidProgress *report)
{
    (void)c;
    if (report->percent >= 100) {
        return (report->CasingTotal != 0) ? MonumentTextDoneBoth : MonumentTextDoneMain;
    }
    if (report->mainRemaining != 0) {
        return (report->casingRemaining != 0) ? MonumentTextRemainderNeedsBoth : MonumentTextRemainderNeedsMain;
    }
    return (report->casingRemaining != 0) ? MonumentTextRemainderNeedsCasing : -1;
}

static const MausoleumInfoProgress *monument_mausoleum_report(const PharaohInfoMonumentsLiveContext *c)
{
    (void)c;
    return &(*c->mausoleum_report);
}

static void monument_draw_scripted_monument_text(const PharaohInfoMonumentsLiveContext *c,int32_t text, int32_t x, int32_t y)
{
    (void)c;
    if (text >= 0) {
        monument_draw_text(c,text, x, y);
    }
}

static int32_t monument_text_width(const PharaohInfoMonumentsLiveContext *c)
{
    (void)c;
    return ui(c)->window->width_blocks * 16 - 16;
}

static int32_t monument_tomb_has_chamber_at(const PharaohInfoMonumentsLiveContext *c,int32_t mainId, int32_t chambers, int32_t firstStage, int32_t lastStage)
{
    (void)c;
    for (int32_t i = 0; i < chambers; i++) {
        int32_t stage = mon_get_building(c,mainId + i)->royal_tomb_progress;
        if (stage >= firstStage && stage <= lastStage) {
            return 1;
        }
    }
    return 0;
}

static const AbuSimbelInfoProgress *monument_abu_simbel_report(const PharaohInfoMonumentsLiveContext *c)
{
    (void)c;
    return &(*c->abu_simbel_report);
}

static int32_t monument_pyramid_site_text(const PharaohInfoMonumentsLiveContext *c,const PyramidProgress *report, int32_t style)
{
    (void)c;
    if (!monument_built(c,199)) {
        return MonumentTextNoWorkCamp;
    }
    if (!monument_working(c,199) && report->laborers == 0) {
        return monument_no_peasants_text(c);
    }
    int32_t level = report->prepMaxStage;
    if (level == report->prepMinStage) {
        level++;
    }
    if (style == 5 && level > PyramidLastEarlySiteLevel && level <= MastabaLastGrooveLevel) {
        level = PyramidLastEarlySiteLevel;
    }
    if (level >= PyramidFirstSiteLevel && level <= PyramidLastEarlySiteLevel) {
        return MonumentTextClearingPyramidSite + (style == 5);
    }
    if (level > PyramidLastEarlySiteLevel && level <= PyramidLastSiteLevel) {
        return level - 1;
    }
    return MonumentTextRoof;
}

static void monument_draw_text(const PharaohInfoMonumentsLiveContext *c,int32_t index, int32_t x, int32_t y)
{
    (void)c;
    mon_native_multiline(c,
        178, index, x, y, ui(c)->window->width_blocks * MonumentTextWidthPerBlock, (*ui(c)->font_black), 0, ui(c)->window->x, ui(c)->window->y);
}

static const ScriptedMonumentInfoProgress *monument_caesareum_report(const PharaohInfoMonumentsLiveContext *c)
{
    (void)c;
    return &(*c->caesareum_report);
}

static int32_t monument_monument_blocked(const PharaohInfoMonumentsLiveContext *c,int32_t BuildingId, int32_t recheckId)
{
    (void)c;
    if (monument_road_tile_blocked(c,BuildingId)) {
        return 1;
    }
    if (mon_get_building(c,BuildingId)->distance_from_entry == 0) {
        mon_update_access(c);
        if (mon_get_building(c,recheckId)->distance_from_entry == 0) {
            return 1;
        }
    }
    return 0;
}

static int32_t monument_material_shortage_text(const PharaohInfoMonumentsLiveContext *c,int32_t resource)
{
    (void)c;
    int32_t stockpiled = monument_player(c)->stockpiled[resource] != 0;
    switch (resource) {
        case 24:
            return stockpiled ? MonumentTextStoneStockpiled : MonumentTextNeedsStone;
        case 25:
            return stockpiled ? MonumentTextLimestoneStockpiled : MonumentTextNeedsLimestone;
        case 12:
            return stockpiled ? MonumentTextBricksStockpiled : MonumentTextNeedsBricks;
        case 30:
            return stockpiled ? MonumentTextSandstoneStockpiled : MonumentTextNeedsSandstone;
        case 35:
            return stockpiled ? MonumentTextMarbleStockpiled : MonumentTextNeedsMarble;
        case 26:
            return stockpiled ? MonumentTextGraniteStockpiled : MonumentTextNeedsGranite;
        default:
            return stockpiled ? MonumentTextCopperStockpiled : MonumentTextNeedsCopper;
    }
}

static void monument_draw_requirements_or_done(const PharaohInfoMonumentsLiveContext *c,int32_t percent, int32_t material, int32_t x, int32_t y, int32_t width)
{
    (void)c;
    if (percent < 100) {
        monument_draw_expanded(c,MonumentTextPercentComplete, x, y, width);
        y += (*c->draw_y_accum);
        if (material != 0) {
            (*c->draw_y_accum) = 0;
            monument_draw_expanded(c,MonumentTextRemainderNeedsMain, x, y, width);
        }
    } else {
        monument_draw_expanded(c,MonumentTextDoneMain, x, y, width);
    }
}

static const PyramidStyleRule *monument_pyramid_style_rule_of(const PharaohInfoMonumentsLiveContext *c,int32_t style)
{
    (void)c;
    return &pyramidStyleRules[(style >= 1 && style <= 5) ? style : 0];
}

static int32_t monument_sphinx_text(const PharaohInfoMonumentsLiveContext *c,int32_t carpentersAssigned, int32_t stonemasonsAssigned, uint8_t carpenterWorking, uint8_t stonemasonWorking, int32_t noRoad)
{
    (void)c;
    if (noRoad) {
        return MonumentTextMonumentBlocked;
    }
    if (!monument_built(c,179)) {
        return MonumentTextNoStonemasons;
    }
    if (!monument_built(c,177)) {
        return MonumentTextNoCarpenters;
    }
    if (stonemasonsAssigned == 0 && !stonemasonWorking && mon_sphinx_free(c,81) != 0) {
        return MonumentTextStonemasonsNotComing;
    }
    if (carpentersAssigned == 0 && !carpenterWorking && mon_sphinx_free(c,79) != 0) {
        return MonumentTextCarpentersNotComing;
    }
    int16_t stage = monument_player(c)->sphinx_stage;
    if (stage < SphinxStageFineCarving) {
        return MonumentTextSphinxRoughShape;
    }
    return (stage < SphinxStageFinishing) ? MonumentTextSphinxFineCarving : MonumentTextSphinxFinishing;
}

static int32_t monument_sandstone_available(const PharaohInfoMonumentsLiveContext *c,int32_t stonemasonsWaiting)
{
    (void)c;
    return resource_unavailable(c,
        30, (stonemasonsWaiting * SandstoneBlocksPerMason + SandstoneBlocksPerMason) * SandstoneLoadsPerBlock);
}

static int32_t monument_caesareum_stage_text(const PharaohInfoMonumentsLiveContext *c,int32_t stage)
{
    (void)c;
    if (stage < 0) {
        return MonumentTextCaesareumClearing;
    }
    if (stage == 0) {
        return MonumentTextCaesareumClearing + 1;
    }
    static const int32_t stageEnds[] = { 6, 9, 12, 14, 17 };
    for (int32_t i = 0; i < (int32_t) (sizeof(stageEnds) / sizeof(stageEnds[0])); i++) {
        if (stage < stageEnds[i]) {
            return MonumentTextCaesareumClearing + 2 + i;
        }
    }
    if (stage == 17) {
        return MonumentTextCaesareumClearing + 7;
    }
    return (stage == 18) ? MonumentTextCaesareumClearing + 8 : MonumentTextCaesareumClearing + 9;
}

static int32_t monument_sun_temple_temple_text(const PharaohInfoMonumentsLiveContext *c,uint32_t flags, int32_t sandstoneOk, int32_t y, int32_t *textY)
{
    (void)c;
    const int32_t *report = monument_sun_temple_report(c);
    if (mon_sun_progress(c,ui(c)->window->building_id, 0) >= 100) {
        return MonumentTextSunTempleComplete;
    }
    if (!monument_built(c,199)) {
        return MonumentTextNoWorkCamp;
    }
    if (!monument_built(c,179)) {
        return MonumentTextNoStonemasons;
    }
    if (!monument_working(c,199) && report[9] == 0 && !(flags & 16)) {
        return MonumentTextNoPeasants;
    }
    int32_t stonemasons = report[7];
    int32_t waiting = report[8];
    if (stonemasons == waiting && !sandstoneOk) {
        return MonumentTextNeedsSandstone;
    }
    if (stonemasons == 0 && (!(flags & 4) || !(flags & 8))) {
        return MonumentTextStonemasonsNotComing;
    }
    if (waiting == 0 || !sandstoneOk || report[11] == 0) {
        return MonumentTextSunTempleVestibule;
    }
    *textY = (*c->draw_y_accum) + y;
    return monument_material_shortage_text(c,30);
}

static int32_t monument_no_peasants_text(const PharaohInfoMonumentsLiveContext *c)
{
    (void)c;
    return (*c->flood_state==1||*c->flood_state==2||*c->flood_state==3) ? MonumentTextPeasantsOnFloodplain : MonumentTextNoPeasants;
}

static const int32_t *monument_sun_temple_report(const PharaohInfoMonumentsLiveContext *c)
{
    (void)c;
    return c->sun_temple_report;
}

static void monument_draw_panel_title(const PharaohInfoMonumentsLiveContext *c)
{
    (void)c; frame(c,178,12); }

static void monument_expand_text(const PharaohInfoMonumentsLiveContext *c,int32_t index, char *buffer, int32_t size)
{
    (void)c;
    mon_expand(c,mon_language_string(c,178, index), buffer, size);
}

static int32_t monument_has_ramp_work(const PharaohInfoMonumentsLiveContext *c)
{
    (void)c;
    return (uint8_t) pharaoh_info_monuments_has_ramp_work(c,mon_building_main(c,ui(c)->window->building_id)) != 0;
}

static int32_t monument_pyramid_needs_available(const PharaohInfoMonumentsLiveContext *c,int32_t resource, int32_t amount)
{
    (void)c;
    uint8_t available = resource_unavailable(c,resource, amount);
    return (uint8_t) pharaoh_info_monuments_needs_resource(c,mon_building_main(c,ui(c)->window->building_id), resource) != 0 && available;
}

void pharaoh_info_monuments_draw_pyramid(const PharaohInfoMonumentsLiveContext *c)
{
    (void)c;
    const PyramidProgress *report = monument_pyramid_report(c);
    int32_t style = (*c->pyramid->style);
    const PyramidStyleRule *rule = monument_pyramid_style_rule_of(c,style);
    ui(c)->window->help_id = rule->helpMessage;
    monument_text_resources(c)->first = rule->mainMaterial;
    monument_text_resources(c)->second = rule->casingMaterial;
    if(ui(c)->window->first_draw){(void)pharaoh_native_return_zero_0047d350();ui(c)->window->first_draw=0;}
    mon_native_outer(c,ui(c)->window->x, ui(c)->window->y, ui(c)->window->width_blocks, ui(c)->window->height_blocks);
    int32_t def = monument_monument_def_of_info_building(c);
    int32_t noRoad = monument_no_road_to_staging_area(c,ui(c)->window->building_id);
    mon_native_centered(c,178, MonumentTextForeman, ui(c)->window->x, ui(c)->window->y + 10,
        ui(c)->window->width_blocks * 16, (*ui(c)->font_title), 0, ui(c)->window->x, ui(c)->window->y);
    int32_t x = ui(c)->window->x + MonumentTextX;
    int32_t y = ui(c)->window->y + MonumentTextY;
    int32_t text = -1;
    int32_t textY = -1;
    if (noRoad && report->percent < 100) {
        y += MonumentLineHeight;
        (*ui(c)->text_width) = 0;
        (*c->draw_y_accum) = 0;
        text = MonumentTextStagingAreaBlocked;
    } else {
        (*c->draw_y_accum) = 0;
        switch (report->phase) {
            case 1:
                text = monument_pyramid_site_text(c,report, style);
                break;
            case 2:
                text = monument_pyramid_blocks_text(c,report, rule, def, x, y, &textY);
                break;
            case 3:
                text = monument_pyramid_finishing_text(c,report, rule, def, x, &y);
                break;
        }
    }
    if (text >= 0) {
        monument_draw_text(c,text, x, (textY == -1) ? y : textY);
    }
    y += (*c->draw_y_accum) + MonumentLineHeight;
    int32_t width = monument_text_width(c);
    (*c->draw_y_accum) = 0;
    if (report->percent < 100) {
        monument_draw_expanded_below(c,MonumentTextPercentComplete, x, y, width);
    }
    if (report->frontierMain != 0 || report->frontierCasing != 0) {
        monument_draw_pyramid_courses(c,report, x, y, width);
    }
    int32_t remainder = monument_pyramid_remainder_text(c,report);
    if (remainder >= 0) {
        monument_draw_expanded_below(c,remainder, x, y, width);
    }
}

void pharaoh_info_monuments_draw_sphinx(const PharaohInfoMonumentsLiveContext *c)
{
    (void)c;
    monument_start_monument_panel(c,362);
    int32_t top = ui(c)->window->y;
    int32_t x = ui(c)->window->x + MonumentTextX;
    int32_t y = ui(c)->window->y + MonumentTextY;
    int32_t carpentersAssigned;
    int32_t stonemasonsAssigned;
    mon_sphinx_count(c,79, &carpentersAssigned);
    mon_sphinx_count(c,81, &stonemasonsAssigned);
    uint8_t carpenterWorking = (uint8_t) mon_sphinx_working(c,79);
    uint8_t stonemasonWorking = (uint8_t) mon_sphinx_working(c,81);
    int32_t noRoad = monument_no_road_to_staging_area(c,ui(c)->window->building_id);
    (*c->draw_y_accum) = 0;
    int32_t complete = monument_player(c)->sphinx_stage >= SphinxStageComplete;
    if (!complete) {
        monument_draw_text(c,monument_sphinx_text(c,carpentersAssigned, stonemasonsAssigned, carpenterWorking, stonemasonWorking, noRoad), x, y);
    }
    int32_t linesY = top + MonumentRequirementsY + (*c->draw_y_accum);
    if (complete) {
        monument_draw_label(c,MonumentTextSphinxComplete, x, linesY);
        return;
    }
    int32_t carpentryPercent = 0;
    int32_t masonPercent = 0;
    int32_t carpenters;
    int32_t carpentersDone;
    int32_t masons;
    int32_t masonsDone;
    (*c->draw_y_accum) = 0;
    mon_sphinx_percentages(c,&carpentryPercent, &masonPercent, &carpenters, &carpentersDone, &masons, &masonsDone);
    monument_draw_work_line(c,MonumentTextCarpentryIs, carpentryPercent, x, (*c->draw_y_accum) + linesY);
    monument_draw_work_line(c,MonumentTextMasonWorkIs, masonPercent, x, (*c->draw_y_accum) + linesY);
}

void pharaoh_info_monuments_draw_obelisk(const PharaohInfoMonumentsLiveContext *c)
{
    (void)c;
    ui(c)->window->help_id = 372;
    int32_t x = ui(c)->window->x + MonumentTextX;
    int32_t y = ui(c)->window->y + MonumentTextY;
    if(ui(c)->window->first_draw){(void)pharaoh_native_return_zero_0047d350();ui(c)->window->first_draw=0;}
    monument_draw_panel_title(c);
    int32_t id = ui(c)->window->building_id;
    int32_t carpentersAssigned;
    int32_t stonemasonsAssigned;
    mon_obelisk_count(c,79, &carpentersAssigned, id);
    mon_obelisk_count(c,81, &stonemasonsAssigned, ui(c)->window->building_id);
    uint8_t carpenterWorking = (uint8_t) mon_obelisk_working(c,79, ui(c)->window->building_id);
    uint8_t stonemasonWorking = (uint8_t) mon_obelisk_working(c,81, ui(c)->window->building_id);
    uint8_t stonemasonSpotFree = (uint8_t) pharaoh_info_monuments_obelisk_has_free_spot(c,81, ui(c)->window->building_id);
    uint8_t carpenterSpotFree = (uint8_t) pharaoh_info_monuments_obelisk_has_free_spot(c,79, ui(c)->window->building_id);
    int32_t text = monument_obelisk_waiting_text(c,
        carpentersAssigned, stonemasonsAssigned, carpenterWorking, stonemasonWorking, stonemasonSpotFree, carpenterSpotFree);
    if (text >= 0) {
        monument_draw_text(c,text, x, y);
        return;
    }
    if (building(c)->abu_simbel_stage >= 100) {
        monument_draw_label(c,MonumentTextObeliskComplete, x, y);
        return;
    }
    int32_t carpentersOnly = !(stonemasonsAssigned != 0 || stonemasonWorking) && (carpentersAssigned != 0 || carpenterWorking);
    (*c->draw_y_accum) = 0;
    monument_draw_text(c,carpentersOnly ? MonumentTextObeliskCarpentersFinishing : MonumentTextObeliskCarving, x, y);
    int32_t width = ui(c)->window->width_blocks * 16;
    y += (*c->draw_y_accum);
    (*c->draw_y_accum) = 0;
    monument_draw_expanded(c,MonumentTextPercentComplete, x, y, width - 16);
}

void pharaoh_info_monuments_draw_sun_temple(const PharaohInfoMonumentsLiveContext *c)
{
    (void)c;
    const int32_t *report = monument_sun_temple_report(c);
    monument_start_monument_panel(c,69);
    int32_t x = ui(c)->window->x + MonumentTextX;
    int32_t y = ui(c)->window->y + MonumentTextY;
    const PharaohBuildingTypeRecord *b = building(c);
    uint8_t carpenterSpotFree = (uint8_t) mon_sun_free(c,79, b->monument_stage);
    uint8_t stonemasonSpotFree = (uint8_t) mon_sun_free(c,81, b->monument_stage);
    uint32_t flags = (uint32_t) b->complex_progress_flags;
    int32_t sandstoneOk = monument_sandstone_available(c,report[8]);
    (*c->draw_y_accum) = 0;
    int32_t textY = y;
    int32_t text;
    switch (report[0]) {
        case 1:
            if (!monument_built(c,199)) {
                text = MonumentTextNoWorkCamp;
            } else if (!monument_working(c,199) && report[9] == 0) {
                text = monument_no_peasants_text(c);
            } else {
                text = MonumentTextSunTempleBegin;
            }
            break;
        case 2:
            if (!monument_built(c,177)) {
                text = MonumentTextNoCarpenters;
            } else if (!monument_working(c,177) && report[10] == 0) {
                text = MonumentTextCarpentersNotComing;
            } else {
                text = MonumentTextSunTempleScaffolding;
            }
            break;
        case 3:
            text = monument_sun_temple_stone_text(c,stonemasonSpotFree, carpenterSpotFree);
            break;
        case 4:
            text = monument_sun_temple_temple_text(c,flags, sandstoneOk, y, &textY);
            break;
        default:
            return;
    }
    monument_draw_text(c,text, x, textY);
    y += (*c->draw_y_accum);
    monument_text_resources(c)->first = 30;
    (*c->draw_y_accum) = 0;
    monument_draw_requirements_or_done(c,report[12], report[11], x, y, monument_text_width(c));
}

void pharaoh_info_monuments_draw_great_library(const PharaohInfoMonumentsLiveContext *c)
{
    (void)c;
    const LibraryInfoProgress *report = monument_library_report(c);
    const LibraryInfoStock *stock = monument_library_stock(c);
    const ScriptedMonumentInfoProgress *caesareum = monument_caesareum_report(c);
    monument_start_monument_panel(c,481);
    int32_t top = ui(c)->window->y;
    int32_t x = ui(c)->window->x + MonumentTextX;
    int32_t y = ui(c)->window->y + MonumentTextY;
    int32_t stage = mon_get_building(c,mon_main_part(c,ui(c)->window->building_id))->monument_stage;
    uint8_t carpenterSpotFree = mon_library_free(c,79, stage);
    uint8_t stonemasonSpotFree = mon_library_free(c,81, stage);
    mon_library_progress(c, ui(c)->window->building_id, 0);
    uint8_t resourceOk = 0;
    if (stage < LibraryMarbleStageEnd) {
        resourceOk = resource_unavailable(c,35, report->stonemasonsWaiting * MaterialLoadsPerBlock);
    } else if (stage < LibraryCopperStageEnd + 1) {
        resourceOk = resource_unavailable(c,29, report->stonemasonsWaiting * MaterialLoadsPerBlock);
    }
    (*c->draw_y_accum) = 0;
    int32_t text = -1;
    if (monument_monument_blocked(c,ui(c)->window->building_id, ui(c)->window->building_id)) {
        text = MonumentTextMonumentBlocked;
    } else if (report->stonemasonsWaiting != 0 && resourceOk && stock->material != 0) {
        if (stage < LibraryMarbleStageEnd) {
            text = monument_material_shortage_text(c,35);
            y += (*c->draw_y_accum);
        } else if (stage < LibraryCopperStageEnd) {
            text = monument_material_shortage_text(c,29);
            y += (*c->draw_y_accum);
        }
    } else {
        text = 0;
        if (stonemasonSpotFree) {
            text = monument_work_camp_text(c,report->laborers);
            if (text == 0) {
                text = monument_guild_text(c,1, 0, caesareum->stonemasons, 0);
            }
        }
        if (text == 0 && carpenterSpotFree) {
            text = monument_guild_text(c,0, 1, 0, caesareum->carpenters);
        }
        if (text == 0) {
            text = monument_library_stage_text(c,report->stage);
        }
    }
    if (text >= 0) {
        monument_draw_text(c,text, x, y);
    }
    int32_t linesY = top + MonumentRequirementsY + (*c->draw_y_accum);
    int32_t width = monument_text_width(c);
    monument_text_resources(c)->first = 35;
    if (stock->percent >= 100) {
        return;
    }
    (*c->draw_y_accum) = 0;
    monument_draw_expanded(c,MonumentTextPercentComplete, x, linesY, width);
    linesY += (*c->draw_y_accum);
    if (stock->material != 0) {
        (*c->draw_y_accum) = 0;
        monument_draw_expanded(c,MonumentTextRemainderNeedsMain, x, linesY, width);
        linesY += (*c->draw_y_accum);
    }
    if (stock->material > LibraryCopperStock) {
        monument_text_resources(c)->first = 29;
        (*c->draw_y_accum) = 0;
        monument_draw_expanded(c,MonumentTextRemainderNeedsMain, x, linesY, width);
    }
}

void pharaoh_info_monuments_draw_abu_simbel(const PharaohInfoMonumentsLiveContext *c)
{
    (void)c;
    const ScriptedMonumentInfoProgress *caesareum = monument_caesareum_report(c);
    monument_start_monument_panel(c,492);
    int32_t top = ui(c)->window->y;
    int32_t x = ui(c)->window->x + MonumentTextX;
    int32_t y = ui(c)->window->y + MonumentTextY;
    int32_t stage = mon_get_building(c,mon_main_part(c,ui(c)->window->building_id))->abu_simbel_stage;
    uint8_t carpenterSpotFree = mon_abu_free(c,79, stage);
    uint8_t stonemasonSpotFree = mon_abu_free(c,81, stage);
    mon_abu_progress(c,ui(c)->window->building_id,0);
    (*c->draw_y_accum) = 0;
    int32_t text;
    if (monument_monument_blocked(c,ui(c)->window->building_id, ui(c)->window->building_id)) {
        text = MonumentTextMonumentBlocked;
    } else {
        text = monument_guild_text(c,stonemasonSpotFree, carpenterSpotFree, caesareum->stonemasons, caesareum->carpenters);
        if (text == 0) {
            text = (monument_abu_simbel_report(c)->stage < AbuSimbelWalkersStage) ? MonumentTextAbuSimbelWorking : MonumentTextAbuSimbelWalkers;
        }
    }
    monument_draw_text(c,text, x, y);
    int32_t linesY = top + MonumentRequirementsY + (*c->draw_y_accum);
    if (monument_abu_simbel_report(c)->percent < 100) {
        (*c->draw_y_accum) = 0;
        monument_draw_expanded(c,MonumentTextPercentComplete, x, linesY, ui(c)->window->width_blocks * 16 - 16);
    }
}

void pharaoh_info_monuments_draw_caesareum(const PharaohInfoMonumentsLiveContext *c)
{
    (void)c;
    const ScriptedMonumentInfoProgress *report = monument_caesareum_report(c);
    ui(c)->window->help_id = 482;
    int32_t stage = mon_get_building(c,mon_main_part(c,ui(c)->window->building_id))->monument_stage;
    if(ui(c)->window->first_draw){(void)pharaoh_native_return_zero_0047d350();ui(c)->window->first_draw=0;}
    monument_draw_panel_title(c);
    int32_t top = ui(c)->window->y;
    int32_t x = ui(c)->window->x + MonumentTextX;
    int32_t y = ui(c)->window->y + MonumentTextY;
    uint8_t carpenterSpotFree = mon_caesareum_free(c,79, stage);
    uint8_t stonemasonSpotFree = mon_caesareum_free(c,81, stage);
    (*c->draw_y_accum) = 0;
    uint8_t resourceOk = 0;
    if (stage < CaesareumMarbleStageEnd) {
        resourceOk = resource_unavailable(c,35, report->stonemasonsWaiting * MaterialLoadsPerBlock);
    } else if (stage < CaesareumGraniteStageEnd) {
        resourceOk = resource_unavailable(c,26, report->stonemasonsWaiting * MaterialLoadsPerBlock);
    }
    (*c->draw_y_accum) = 0;
    int32_t text = -1;
    if (monument_monument_blocked(c,ui(c)->window->building_id, ui(c)->window->building_id)) {
        text = MonumentTextMonumentBlocked;
    } else if (report->stonemasonsWaiting != 0 && resourceOk && report->material != 0) {
        if (stage < CaesareumMarbleStageEnd) {
            text = monument_material_shortage_text(c,35);
            y += (*c->draw_y_accum);
        } else if (stage < CaesareumGraniteStageEnd) {
            text = monument_material_shortage_text(c,26);
            y += (*c->draw_y_accum);
        }
    } else {
        text = monument_work_camp_text(c,report->laborers);
        if (text == 0) {
            text = monument_guild_text(c,stonemasonSpotFree, carpenterSpotFree, report->stonemasons, report->carpenters);
        }
        if (text == 0) {
            text = monument_caesareum_stage_text(c,stage);
        }
    }
    monument_draw_scripted_monument_text(c,text, x, y);
    monument_text_resources(c)->first = 35;
    int32_t width = monument_text_width(c);
    int32_t linesY = top + MonumentRequirementsY + (*c->draw_y_accum);
    if (report->percent >= 100) {
        return;
    }
    (*c->draw_y_accum) = 0;
    monument_draw_expanded(c,MonumentTextPercentComplete, x, linesY, width);
    linesY += (*c->draw_y_accum);
    if (report->material > CaesareumMarbleStock) {
        (*c->draw_y_accum) = 0;
        monument_draw_expanded(c,MonumentTextRemainderNeedsMain, x, linesY, width);
        linesY += (*c->draw_y_accum);
    }
    if (report->material != 0) {
        monument_text_resources(c)->first = 26;
        (*c->draw_y_accum) = 0;
        monument_draw_expanded(c,MonumentTextRemainderNeedsMain, x, linesY, width);
    }
}

void pharaoh_info_monuments_draw_lighthouse(const PharaohInfoMonumentsLiveContext *c)
{
    (void)c;
    const ScriptedMonumentInfoProgress *report = monument_lighthouse_report(c);
    ui(c)->window->help_id = 487;
    int32_t stage = mon_get_building(c,mon_main_part(c,ui(c)->window->building_id))->monument_stage;
    if(ui(c)->window->first_draw){(void)pharaoh_native_return_zero_0047d350();ui(c)->window->first_draw=0;}
    monument_draw_panel_title(c);
    int32_t top = ui(c)->window->y;
    int32_t x = ui(c)->window->x + MonumentTextX;
    int32_t y = ui(c)->window->y + MonumentTextY;
    uint8_t carpenterSpotFree = mon_lighthouse_free(c,79, stage);
    uint8_t stonemasonSpotFree = mon_lighthouse_free(c,81, stage);
    (*c->draw_y_accum) = 0;
    uint8_t resourceOk = resource_unavailable(c,35, report->stonemasonsWaiting * MaterialLoadsPerBlock);
    int32_t blocked = monument_monument_blocked(c,ui(c)->window->building_id, ui(c)->window->building_id);
    int32_t text = -1;
    if (stage >= LighthouseStageDone) {
        text = MonumentTextLighthouseComplete;
    } else if (blocked) {
        text = MonumentTextMonumentBlocked;
    } else if (report->stonemasonsWaiting != 0 && resourceOk && report->material != 0) {
        text = monument_material_shortage_text(c,35);
        y += (*c->draw_y_accum);
    } else {
        text = 0;
        if (stonemasonSpotFree) {
            text = monument_work_camp_text(c,report->laborers);
            if (text == 0) {
                text = monument_guild_text(c,1, 0, report->stonemasons, 0);
            }
        }
        if (text == 0 && carpenterSpotFree) {
            text = monument_guild_text(c,0, 1, 0, report->carpenters);
        }
        if (text == 0) {
            text = monument_lighthouse_stage_text(c,stage);
        }
    }
    monument_draw_scripted_monument_text(c,text, x, y);
    monument_text_resources(c)->first = 35;
    int32_t linesY = top + MonumentRequirementsY + (*c->draw_y_accum);
    int32_t width = monument_text_width(c);
    if (report->percent >= 100) {
        return;
    }
    (*c->draw_y_accum) = 0;
    monument_draw_expanded(c,MonumentTextPercentComplete, x, linesY, width);
    linesY += (*c->draw_y_accum);
    if (report->material != 0) {
        (*c->draw_y_accum) = 0;
        monument_draw_expanded(c,MonumentTextRemainderNeedsMain, x, linesY, width);
    }
}

void pharaoh_info_monuments_draw_royal_tomb(const PharaohInfoMonumentsLiveContext *c)
{
    (void)c;
    const int32_t *report = monument_tomb_report(c);
    int32_t mainId = mon_main_part(c,ui(c)->window->building_id);
    ui(c)->window->help_id = 478;
    const PharaohBuildingTypeRecord *main = mon_get_building(c,mainId);
    int32_t chambers = mon_chambers(c,main->building_type);
    int32_t wantsStone = monument_tomb_has_chamber_at(c,mainId, chambers, 1, 1);
    int32_t wantsArtisans = monument_tomb_has_chamber_at(c,mainId, chambers, 1 + 1, 3);
    monument_draw_panel_title(c);
    int32_t top = ui(c)->window->y;
    int32_t x = ui(c)->window->x + MonumentTextX;
    (*c->draw_y_accum) = 0;
    (*ui(c)->text_width) = 0;
    mon_native_multiline(c,178, MonumentTextLamps, x, ui(c)->window->y + TombLampsY, ui(c)->window->width_blocks * TombLampsWidthPerBlock,
        (*ui(c)->font_black), 0, ui(c)->window->x, ui(c)->window->y);
    mon_native_amount(c,8, 10, report[6], (*ui(c)->text_width) + ui(c)->window->x + TombLampsAmountX,
        ui(c)->window->y + TombLampsY, (*ui(c)->font_black), 0);
    int32_t y = top + MonumentTextY + (*c->draw_y_accum) + MonumentLineHeight;
    if (main->runtime_value_016 == -1) {
        monument_draw_text(c,MonumentTextTombReady, x, (*c->draw_y_accum) + y);
        return;
    }
    int32_t text;
    int32_t textY = (*c->draw_y_accum) + y;
    if (report[5] < 100) {
        if (monument_monument_blocked(c,mainId, ui(c)->window->building_id)) {
            text = MonumentTextTombEntranceBlocked;
            textY = y;
        } else {
            textY = (*c->draw_y_accum) + y;
            text = monument_tomb_work_text(c,mainId, wantsStone, wantsArtisans);
        }
    } else {
        text = monument_tomb_goals_reached(c) ? MonumentTextTombReady : MonumentTextTombComplete;
    }
    monument_draw_text(c,text, x, textY);
    int32_t linesY = y + MonumentLineHeight + (*c->draw_y_accum);
    if (report[5] < 100) {
        (*c->draw_y_accum) = 0;
        monument_draw_expanded(c,MonumentTextPercentComplete, x, linesY, ui(c)->window->width_blocks * 16 - 16);
    }
}

void pharaoh_info_monuments_draw_mausoleum(const PharaohInfoMonumentsLiveContext *c)
{
    (void)c;
    const MausoleumInfoProgress *report = monument_mausoleum_report(c);
    monument_start_monument_panel(c,368);
    int32_t x = ui(c)->window->x + MonumentTextX;
    int32_t y = ui(c)->window->y + MonumentTextY;
    int32_t sandstoneOk = monument_sandstone_available(c,report->stonemasonsWaiting);
    (*c->draw_y_accum) = 0;
    int32_t text;
    switch (report->status) {
        case 1:
            if (!monument_built(c,199)) {
                text = MonumentTextNoWorkCamp;
            } else if (monument_working(c,199) || report->laborers != 0) {
                text = MonumentTextMausoleumBegin;
            } else {
                text = monument_no_peasants_text(c);
            }
            break;
        case 2:
            text = monument_mausoleum_stone_text(c,report, sandstoneOk);
            break;
        case 3:
            text = monument_mausoleum_building_text(c,report, sandstoneOk);
            break;
        default:
            (*c->draw_y_accum) = 0;
            return;
    }
    monument_draw_text(c,text, x, y);
    y += (*c->draw_y_accum);
    monument_text_resources(c)->first = 30;
    (*c->draw_y_accum) = 0;
    monument_draw_requirements_or_done(c,report->percent, report->material, x, y, monument_text_width(c));
}
