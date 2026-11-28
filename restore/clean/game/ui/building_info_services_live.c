#include "pharaoh/ui/building_info_services_live.h"
#include "pharaoh/core/percentage_scale.h"
#include "pharaoh/core/static_objects.h"
#include "pharaoh/runtime/text_loading.h"
#include <stddef.h>

/* Whole current/old BuildingInfoManager service/culture/religion/government
 * C, with the pinned Native field widths and rendering call contracts.
 * The noncontiguous515D10..516010 festival tail belongs to4FC820's body. */
_Static_assert(sizeof(PharaohInfoServicesPlayerRecord)==0x49d8,"native player stride");
_Static_assert(offsetof(PharaohInfoServicesPlayerRecord,tax_percentage)==0x8&&
    offsetof(PharaohInfoServicesPlayerRecord,palace_building_id)==0x2790&&
    offsetof(PharaohInfoServicesPlayerRecord,stolen_this_year)==0x2c84&&
    offsetof(PharaohInfoServicesPlayerRecord,salary)==0x2c98&&
    offsetof(PharaohInfoServicesPlayerRecord,salary_rank)==0x2c9c&&
    offsetof(PharaohInfoServicesPlayerRecord,months_since_festival)==0x438c&&
    offsetof(PharaohInfoServicesPlayerRecord,palace_placed)==0x4486&&
    offsetof(PharaohInfoServicesPlayerRecord,palace_robberies)==0x448c&&
    offsetof(PharaohInfoServicesPlayerRecord,festival_planned_size)==0x44f8&&
    offsetof(PharaohInfoServicesPlayerRecord,festival_planned_months)==0x44fc&&
    offsetof(PharaohInfoServicesPlayerRecord,festival_effect_first)==0x4528&&
    offsetof(PharaohInfoServicesPlayerRecord,festival_effect_second)==0x452c,
    "native service player fields");
_Static_assert(sizeof(PharaohBuildingTypeRecord)==264&&
    offsetof(PharaohBuildingTypeRecord,building_type)==0x10&&
    offsetof(PharaohBuildingTypeRecord,walker_id)==0x2a&&
    offsetof(PharaohBuildingTypeRecord,storage_link_amount)==0x3c&&
    offsetof(PharaohBuildingTypeRecord,num_workers)==0x40&&
    offsetof(PharaohBuildingTypeRecord,runtime_value_a6)==0xa6&&
    offsetof(PharaohBuildingTypeRecord,runtime_value_a7)==0xa7&&
    offsetof(PharaohBuildingTypeRecord,runtime_value_a8)==0xa8&&
    offsetof(PharaohBuildingTypeRecord,temple_upgrade)==0xa9&&
    offsetof(PharaohBuildingTypeRecord,money_value)==0xb8&&
    offsetof(PharaohBuildingTypeRecord,secondary_input_stock)==0xcc,
    "native signed WORD workers/stocks/type and BYTE show-day overlays");

static const PharaohStorageInfoDrawContext *draw(const PharaohInfoServicesLiveContext *c)
{ return c->drawing; }
static PharaohBuildingInfoLiveContext *ui(const PharaohInfoServicesLiveContext *c)
{ return draw(c)->ui; }
static const PharaohBuildingTypeRecord *building(const PharaohInfoServicesLiveContext *c)
{ return &ui(c)->buildings[ui(c)->window->building_id]; }
static const PharaohInfoServicesPlayerRecord *player(const PharaohInfoServicesLiveContext *c)
{ return &c->players[*c->current_player]; }
static int32_t signed32(uint32_t v)
{ return v<=INT32_MAX?(int32_t)v:(int32_t)(v&INT32_MAX)-INT32_MAX-1; }
static int32_t add32(int32_t a,int32_t b)
{ return signed32((uint32_t)a+(uint32_t)b); }
static void image(const PharaohInfoServicesLiveContext *c,int32_t id,int32_t x,int32_t y)
{ (void)pharaoh_image_draw(draw(c)->layout->inner_panel->drawing,id,x,y); }
static void language_font(const PharaohInfoServicesLiveContext *c,int32_t group,int32_t entry,
    int32_t x,int32_t y,const PharaohBitmapFont *font,int32_t font_id)
{
    const PharaohBuildingInfoLiveState *w=ui(c)->window;
    pharaoh_ui_text_draw_language(draw(c)->text,font,group,entry,x,y,font_id,0,w->x,w->y);
}
static void language(const PharaohInfoServicesLiveContext *c,int32_t group,int32_t entry,int32_t x,int32_t y)
{ language_font(c,group,entry,x,y,draw(c)->font_black,*ui(c)->font_black); }
static void centered(const PharaohInfoServicesLiveContext *c,int32_t group,int32_t entry,
    int32_t x,int32_t y,int32_t width,const PharaohBitmapFont *font,int32_t font_id)
{
    const PharaohBuildingInfoLiveState *w=ui(c)->window;
    pharaoh_ui_text_draw_centered_language(draw(c)->text,font,group,entry,x,y,width,font_id,0,w->x,w->y);
}
static void multiline_font(const PharaohInfoServicesLiveContext *c,int32_t group,int32_t entry,
    int32_t x,int32_t y,int32_t width,const PharaohBitmapFont *font,int32_t font_id)
{
    const PharaohBuildingInfoLiveState *w=ui(c)->window;
    pharaoh_info_ui_draw_multiline(draw(c)->layout,draw(c)->wrapped,font,group,entry,x,y,width,
        font_id,0,w->x,w->y);
}
static void multiline(const PharaohInfoServicesLiveContext *c,int32_t group,int32_t entry,int32_t y)
{
    const PharaohBuildingInfoLiveState *w=ui(c)->window;
    multiline_font(c,group,entry,w->x+32,y,(w->width_blocks-4)*16,draw(c)->font_black,*ui(c)->font_black);
}
static void number(const PharaohInfoServicesLiveContext *c,int32_t value,const char *suffix,int32_t x,int32_t y)
{
    const PharaohBuildingInfoLiveState *w=ui(c)->window;
    pharaoh_ui_text_draw_number(draw(c)->text,draw(c)->font_black,value,'@',suffix,x,y,
        *ui(c)->font_black,0,w->x,w->y);
}
static void amount_font(const PharaohInfoServicesLiveContext *c,int32_t entry,int32_t value,
    int32_t x,int32_t y,const PharaohBitmapFont *font,int32_t font_id)
{ pharaoh_info_text_draw_amount(draw(c)->text,font,draw(c)->number_suffix,8,entry,value,x,y,font_id,0); }
static void title_at(const PharaohInfoServicesLiveContext *c,int32_t group,int32_t entry,int32_t dy)
{
    const PharaohBuildingInfoLiveState *w=ui(c)->window;
    pharaoh_panel_frame_draw_native(draw(c)->layout->outer_panel,w->x,w->y,w->width_blocks,w->height_blocks);
    centered(c,group,entry,w->x,w->y+dy,w->width_blocks*16,draw(c)->font_title,*ui(c)->font_title);
}
static void start(const PharaohInfoServicesLiveContext *c,int32_t help,int32_t group,int32_t entry)
{
    PharaohBuildingInfoLiveState *w=ui(c)->window;w->help_id=help;
    if(w->first_draw){(void)pharaoh_native_return_zero_0047d350();w->first_draw=0;}
    title_at(c,group,entry,10);
}
static void inner(const PharaohInfoServicesLiveContext *c,int32_t y,int32_t height)
{
    const PharaohBuildingInfoLiveState *w=ui(c)->window;
    pharaoh_info_layout_draw_inner_panel(draw(c)->layout,w->x+16,w->y+y,w->width_blocks-2,height,w->x,w->y);
}
static void workers(const PharaohInfoServicesLiveContext *c)
{ inner(c,136,4);pharaoh_info_draw_employment(draw(c),ui(c)->window->y+142); }
static void venue_workers(const PharaohInfoServicesLiveContext *c,int32_t height)
{ inner(c,136,height);pharaoh_info_draw_employment(draw(c),ui(c)->window->y+138); }
static void no_road(const PharaohInfoServicesLiveContext *c,int32_t y)
{ multiline(c,69,25,y); }
static int32_t staffing(const PharaohInfoServicesLiveContext *c,int32_t base)
{
    const int32_t percent=ui(c)->window->details.worker_percentage;
    return base+(percent>=100?0:percent>=75?1:percent>=50?2:percent>=25?3:4);
}
static void service_state(const PharaohInfoServicesLiveContext *c,int32_t group,int32_t working)
{
    const int32_t y=ui(c)->window->y+56;
    if(ui(c)->window->details.road_access<1)no_road(c,y);
    else multiline(c,group,building(c)->num_workers<1?2:working,y);
}
static void walker_state(const PharaohInfoServicesLiveContext *c,int32_t group)
{
    const int32_t y=ui(c)->window->y+56;
    if(ui(c)->window->details.road_access<1){no_road(c,y);return;}
    if(building(c)->num_workers==0){multiline(c,group,9,y);return;}
    multiline(c,group,building(c)->walker_id==0?3:2,y);
    multiline(c,group,staffing(c,4),ui(c)->window->y+72);
}
static void stock(const PharaohInfoServicesLiveContext *c,int32_t resource)
{
    const PharaohBuildingInfoLiveState *w=ui(c)->window;
    *ui(c)->text_width=0;
    image(c,add32(*ui(c)->resource_images,resource),w->x+20,w->y+30);
    number(c,building(c)->storage_link_amount,c->space_suffix,w->x+40+*ui(c)->text_width,w->y+35);
    language(c,23,resource,w->x+40+*ui(c)->text_width,w->y+35);
}
static void description(const PharaohInfoServicesLiveContext *c,int32_t group,int32_t bottom)
{ multiline(c,group,1,ui(c)->window->y+ui(c)->window->height_blocks*16-bottom); }
static void venue_stock(const PharaohInfoServicesLiveContext *c,int32_t resource,int32_t icon_x,
    int32_t text_x,int32_t group,int32_t text,int32_t amount)
{
    const PharaohBuildingInfoLiveState *w=ui(c)->window;
    image(c,add32(*ui(c)->resource_images,resource),w->x+icon_x,w->y+36);
    *ui(c)->text_width=0;language(c,group,text,w->x+text_x,w->y+40);
    amount_font(c,10,amount<1?0:amount,w->x+text_x+*ui(c)->text_width,w->y+40,
        draw(c)->font_black,*ui(c)->font_black);
}
static void show_days(const PharaohInfoServicesLiveContext *c,int32_t group,int32_t no_show,
    int32_t days_text,uint8_t days,int32_t dy)
{
    const PharaohBuildingInfoLiveState *w=ui(c)->window;
    if(days==0){language_font(c,group,no_show,w->x+32,w->y+dy,draw(c)->font_green,*ui(c)->font_green);return;}
    *ui(c)->text_width=0;
    language_font(c,group,days_text,w->x+32,w->y+dy,draw(c)->font_green,*ui(c)->font_green);
    amount_font(c,44,(int32_t)days*2,w->x+32+*ui(c)->text_width,w->y+dy,
        draw(c)->font_green,*ui(c)->font_green);
}

/* 004FC8E0 /004FCC90 /004FCF10 /004FD120 /004FD4B0. */
void pharaoh_info_services_draw_bandstand(const PharaohInfoServicesLiveContext *c)
{
    start(c,72,71,0);
    if(ui(c)->window->details.road_access<1)no_road(c,ui(c)->window->y+56);
    else {
        const PharaohBuildingTypeRecord *b=building(c);int32_t text=-1;
        if(b->num_workers<1)text=6;else if(b->runtime_value_a6==0)text=2;
        else if(b->runtime_value_a6==2)text=3;else if(b->runtime_value_a8!=0)text=4;
        else if(b->runtime_value_a7!=0)text=5;
        if(text>=0)multiline(c,71,text,ui(c)->window->y+56);
    }
    venue_workers(c,7);
    show_days(c,71,9,10,building(c)->runtime_value_a7,182);
    show_days(c,71,7,8,building(c)->runtime_value_a8,202);
}
void pharaoh_info_services_draw_booth(const PharaohInfoServicesLiveContext *c)
{
    start(c,71,72,0);
    if(ui(c)->window->details.road_access<1)no_road(c,ui(c)->window->y+56);
    else {
        const PharaohBuildingTypeRecord *b=building(c);int32_t text=-1;
        if(b->num_workers<1)text=4;else if(b->runtime_value_a6==0)text=2;
        else if(b->runtime_value_a7!=0)text=3;
        if(text>=0)multiline(c,72,text,ui(c)->window->y+56);
    }
    venue_workers(c,6);show_days(c,72,5,6,building(c)->runtime_value_a7,182);
}
void pharaoh_info_services_draw_senet_house(const PharaohInfoServicesLiveContext *c)
{
    start(c,74,73,0);venue_stock(c,15,32,60,73,7,building(c)->storage_link_amount);
    if(ui(c)->window->details.road_access<1)no_road(c,ui(c)->window->y+56);
    else if(building(c)->num_workers<1)multiline(c,73,4,ui(c)->window->y+56);
    venue_workers(c,6);
}
void pharaoh_info_services_draw_zoo(const PharaohInfoServicesLiveContext *c)
{
    start(c,479,308,0);venue_stock(c,8,32,60,308,6,building(c)->storage_link_amount);
    venue_stock(c,9,255,280,308,7,building(c)->secondary_input_stock);
    const int32_t y=ui(c)->window->y+72;
    if(ui(c)->window->details.road_access<1)no_road(c,y);
    else {
        const PharaohBuildingTypeRecord *b=building(c);
        const int32_t text=b->num_workers<1?4:b->storage_link_amount<1?2:b->secondary_input_stock<1?3:1;
        multiline(c,308,text,y);
    }
    venue_workers(c,6);
}
void pharaoh_info_services_draw_pavilion(const PharaohInfoServicesLiveContext *c)
{
    start(c,73,74,0);
    if(ui(c)->window->details.road_access<1)no_road(c,ui(c)->window->y+56);
    else {
        const PharaohBuildingTypeRecord *b=building(c);
        const int juggling=b->runtime_value_a7!=0,music=b->runtime_value_a8!=0,dancing=b->temple_upgrade!=0;
        const int shows=juggling+music+dancing;
        const int32_t text=b->num_workers<1?10:shows==0?2:shows==3?3:
            dancing&&music?4:dancing&&juggling?5:music&&juggling?6:juggling?7:music?8:9;
        multiline(c,74,text,ui(c)->window->y+56);
    }
    venue_workers(c,6);
    show_days(c,74,11,12,building(c)->runtime_value_a7,182);
    show_days(c,74,13,14,building(c)->runtime_value_a8,197);
    show_days(c,74,15,16,building(c)->temple_upgrade,212);
}
void pharaoh_info_services_draw_performer_school(const PharaohInfoServicesLiveContext *c,int32_t group)
{
    start(c,75,group,0);
    if(ui(c)->window->details.road_access<1)no_road(c,ui(c)->window->y+56);
    else multiline(c,group,building(c)->num_workers<1?7:staffing(c,2),ui(c)->window->y+56);
    workers(c);
}
/* 004FDA00 /004FDC40 /004FDE80. */
void pharaoh_info_services_draw_conservatory(const PharaohInfoServicesLiveContext *c)
{ pharaoh_info_services_draw_performer_school(c,75); }
void pharaoh_info_services_draw_dance_school(const PharaohInfoServicesLiveContext *c)
{ pharaoh_info_services_draw_performer_school(c,76); }
void pharaoh_info_services_draw_juggler_school(const PharaohInfoServicesLiveContext *c)
{ pharaoh_info_services_draw_performer_school(c,77); }
static void school(const PharaohInfoServicesLiveContext *c,int32_t help,int32_t group)
{ start(c,help,group,0);stock(c,23);service_state(c,group,3);workers(c); }
/* 004FF000 /004FF200. */
void pharaoh_info_services_draw_scribal_school(const PharaohInfoServicesLiveContext *c)
{ school(c,68,85); }
void pharaoh_info_services_draw_library(const PharaohInfoServicesLiveContext *c)
{ school(c,70,87); }
static void service(const PharaohInfoServicesLiveContext *c,int32_t help,int32_t group)
{ start(c,help,group,0);service_state(c,group,3);workers(c); }
/* 004FE9A0 /004FEB00 /004FED40 /004FEEA0. */
void pharaoh_info_services_draw_apothecary(const PharaohInfoServicesLiveContext *c)
{ service(c,65,81); }
void pharaoh_info_services_draw_mortuary(const PharaohInfoServicesLiveContext *c)
{ start(c,66,82,0);stock(c,17);service_state(c,82,building(c)->storage_link_amount<1?4:3);workers(c); }
void pharaoh_info_services_draw_physician(const PharaohInfoServicesLiveContext *c)
{ service(c,64,83); }
void pharaoh_info_services_draw_dentist(const PharaohInfoServicesLiveContext *c)
{ service(c,63,84); }

/* 004FF400 /004FF740 /00504070. 4CC110's temporary selection is restored
 * before either number draw, just as the original police suffix lookups. */
void pharaoh_info_services_draw_police_station(const PharaohInfoServicesLiveContext *c)
{
    start(c,86,88,0);walker_state(c,88);
    PharaohTextSelection *selection=draw(c)->text->selection;const char *saved=selection->current;
    pharaoh_text_select(selection,88,10);const char *thefts=selection->current;
    pharaoh_text_select(selection,88,11);const char *stolen=selection->current;
    const int32_t theft_count=player(c)->palace_robberies;selection->current=saved;
    number(c,theft_count,thefts,ui(c)->window->x+32,ui(c)->window->y+104);
    number(c,player(c)->stolen_this_year,stolen,ui(c)->window->x+32,ui(c)->window->y+119);
    workers(c);
}
void pharaoh_info_services_draw_firehouse(const PharaohInfoServicesLiveContext *c)
{ start(c,355,164,0);walker_state(c,164);workers(c);description(c,164,113); }
void pharaoh_info_services_draw_architect_post(const PharaohInfoServicesLiveContext *c)
{ start(c,81,104,0);walker_state(c,104);workers(c); }

static void money_icon(const PharaohInfoServicesLiveContext *c)
{ image(c,add32(*ui(c)->resource_images,21),ui(c)->window->x+16,ui(c)->window->y+36); }
static void money_kept(const PharaohInfoServicesLiveContext *c,int32_t group,int32_t entry,int32_t dx,int32_t dy)
{
    const PharaohBuildingInfoLiveState *w=ui(c)->window;
    *ui(c)->text_width=0;language(c,group,entry,w->x+dx,w->y+dy);
    amount_font(c,0,signed32(building(c)->money_value),w->x+dx+*ui(c)->text_width,w->y+dy,
        draw(c)->font_black,*ui(c)->font_black);
}
static void money_and_tax(const PharaohInfoServicesLiveContext *c,int32_t group,int32_t dx)
{
    const PharaohBuildingInfoLiveState *w=ui(c)->window;
    money_kept(c,group,2,dx,43);*ui(c)->text_width=0;language(c,60,1,w->x+260,w->y+43);
    number(c,player(c)->tax_percentage,c->percent_suffix,w->x+260+*ui(c)->text_width,w->y+43);
    pharaoh_info_square_buttons_draw(c->square_buttons,w->x+226,w->y-37,c->tax_arrows,2);
}
/* 00503E10 /00503F00 /00504680 /005048D0 /00504C20. */
void pharaoh_info_services_draw_mansion_foreground(const PharaohInfoServicesLiveContext *c)
{
    const PharaohBuildingInfoLiveState *w=ui(c)->window;
    pharaoh_info_layout_draw_border(draw(c)->layout,0,w->x+40,w->y+61,400,25,*ui(c)->focus_primary==1,w->x,w->y);
    *ui(c)->text_width=0;language(c,103,add32(player(c)->salary_rank,3),w->x+44,w->y+65);
    number(c,player(c)->salary,c->space_suffix,w->x+44+*ui(c)->text_width,w->y+65);
    language(c,103,2,w->x+44+*ui(c)->text_width,w->y+65);
}
void pharaoh_info_services_draw_mansion(const PharaohInfoServicesLiveContext *c)
{ start(c,78,41,building(c)->building_type);money_icon(c);money_kept(c,106,2,48,47);description(c,103,143); }
void pharaoh_info_services_draw_palace(const PharaohInfoServicesLiveContext *c)
{
    start(c,77,41,building(c)->building_type);money_icon(c);money_and_tax(c,105,32);
    if(ui(c)->window->details.road_access<1)no_road(c,ui(c)->window->y+56);
    workers(c);
}
void pharaoh_info_services_draw_courthouse(const PharaohInfoServicesLiveContext *c)
{
    start(c,76,176,0);money_kept(c,176,9,32,43);
    const int32_t y=ui(c)->window->y+63;
    if(ui(c)->window->details.road_access<1)no_road(c,y);
    else if(building(c)->num_workers==0)multiline(c,176,2,y);
    else {
        multiline(c,176,building(c)->walker_id!=0?7:8,y);
        multiline(c,176,6-staffing(c,0),ui(c)->window->y+83);
    }
    workers(c);description(c,176,113);
}
void pharaoh_info_services_draw_tax_collector(const PharaohInfoServicesLiveContext *c)
{
    start(c,80,106,0);money_icon(c);money_and_tax(c,106,35);
    const PharaohBuildingTypeRecord *palace=&ui(c)->buildings[player(c)->palace_building_id];
    /* MOVSX WORD workers *100 is in[-3276800,3276700], so IDIV cannot
     * reach INT_MIN/-1. Canonical zero-denominator behavior is retained. */
    const PharaohPercentageScaleResult percentage=pharaoh_scale_value_to_percent(
        palace->num_workers,draw(c)->models[palace->building_type].laborers);
    if(ui(c)->window->details.road_access<1)no_road(c,ui(c)->window->y+56);
    else {
        const int32_t text=player(c)->palace_placed==0?11:percentage.value==0?12:
            building(c)->num_workers==0?10:staffing(c,5);
        multiline(c,106,text,ui(c)->window->y+72);
    }
    workers(c);
}

/* 00504300 /005043C0 /00504490 /00504560. */
void pharaoh_info_services_draw_mission_post(const PharaohInfoServicesLiveContext *c)
{ start(c,8,134,0);multiline(c,134,1,ui(c)->window->y+106); }
void pharaoh_info_services_draw_native_hut(const PharaohInfoServicesLiveContext *c)
{ start(c,0,131,0);description(c,131,143); }
void pharaoh_info_services_draw_native_meeting(const PharaohInfoServicesLiveContext *c)
{ start(c,0,132,0);description(c,132,143); }
void pharaoh_info_services_draw_native_crops(const PharaohInfoServicesLiveContext *c)
{ start(c,0,133,0);description(c,133,143); }

/* 00505460 /00505560. The unsigned switch rejects negative temple god IDs;
 * shrine titles and god images separately use the Native signed min(god,4). */
void pharaoh_info_services_draw_oracle(const PharaohInfoServicesLiveContext *c)
{
    PharaohBuildingInfoLiveState *w=ui(c)->window;w->help_id=67;
    if(w->first_draw){(void)pharaoh_native_return_zero_0047d350();w->first_draw=0;}
    title_at(c,110,0,12);
    centered(c,13,1,w->x,w->y+w->height_blocks*16-24,w->width_blocks*16,draw(c)->font_black,*ui(c)->font_black);
    description(c,110,143);
}
void pharaoh_info_services_draw_temple(const PharaohInfoServicesLiveContext *c,int32_t god)
{
    PharaohBuildingInfoLiveState *w=ui(c)->window;
    const int32_t type=building(c)->building_type;
    const int shrine=type>=140&&type<=149;
    const int32_t group=shrine?161:92+((uint32_t)god<=3?(int32_t)god:4);
    if(w->first_draw){(void)pharaoh_native_return_zero_0047d350();w->first_draw=0;}
    w->help_id=67;
    const int32_t shown_god=god<4?god:4;
    const int32_t draw_type=building(c)->building_type;
    const int draw_shrine=draw_type>=140&&draw_type<=149;
    title_at(c,group,draw_shrine?add32(shown_god,shown_god):0,12);
    const int32_t panel_blocks=shrine?0:4;
    if(panel_blocks){inner(c,56,panel_blocks);pharaoh_info_draw_employment(draw(c),w->y+62);}
    if(w->details.road_access>0)
        image(c,add32(add32(*ui(c)->panel_button_images,21),shown_god),w->x+190,(w->height_blocks+panel_blocks)*16+w->y-182);
    else no_road(c,(w->height_blocks-8)*16+w->y);
}

static int32_t unplanned_festival(const PharaohInfoServicesLiveContext *c)
{
    const PharaohBuildingInfoLiveState *w=ui(c)->window;
    if(player(c)->festival_effect_first==0||player(c)->festival_effect_second==0){
        centered(c,58,16,w->x+82,w->y+154,300,draw(c)->font_white,*ui(c)->font_white);
        const int32_t m=player(c)->months_since_festival;
        return m<2?0:m<7?1:m<13?2:m<19?3:m<25?4:m<31?5:6;
    }
    centered(c,58,52,w->x+82,w->y+154,300,draw(c)->font_white,*ui(c)->font_white);
    const int32_t m=player(c)->months_since_festival;return m<2?7:m<7?8:9;
}
static int32_t planned_festival(const PharaohInfoServicesLiveContext *c,int32_t size)
{
    const PharaohBuildingInfoLiveState *w=ui(c)->window;
    const int32_t ordered_months=size==1?2:size==2?3:4;
    const int32_t one_month_text=size==1?10:size==2?20:31;
    *ui(c)->text_width=0;
    language_font(c,58,player(c)->festival_planned_months>ordered_months?53:34,w->x+102,w->y+154,
        draw(c)->font_white,*ui(c)->font_white);
    const int32_t months=player(c)->festival_planned_months;
    if(months==1)language_font(c,58,54,w->x+102+*ui(c)->text_width,w->y+154,draw(c)->font_white,*ui(c)->font_white);
    else language_font(c,160,add32(months,*c->month)%12,w->x+102+*ui(c)->text_width,w->y+154,
        draw(c)->font_white,*ui(c)->font_white);
    return add32(one_month_text-1,months);
}
/* Whole004FC820 including its existing515D10..516010 shared tail; no
 * additional Native owner is claimed for these private C helpers. */
void pharaoh_info_services_draw_festival_square(const PharaohInfoServicesLiveContext *c)
{
    start(c,366,188,0);multiline(c,188,1,ui(c)->window->y+55);
    const PharaohBuildingInfoLiveState *w=ui(c)->window;
    pharaoh_info_layout_draw_inner_panel(draw(c)->layout,w->x+40,w->y+118,w->width_blocks-5,6,w->x,w->y);
    *ui(c)->text_width=0;
    amount_font(c,4,player(c)->months_since_festival,w->x+112,w->y+130,draw(c)->font_white,*ui(c)->font_white);
    language_font(c,58,15,w->x+112+*ui(c)->text_width,w->y+130,draw(c)->font_white,*ui(c)->font_white);
    const int32_t size=player(c)->festival_planned_size;
    const int32_t text=size==0?unplanned_festival(c):planned_festival(c,size);
    multiline_font(c,295,text,w->x+56,w->y+175,380,draw(c)->font_white,*ui(c)->font_white);
}
