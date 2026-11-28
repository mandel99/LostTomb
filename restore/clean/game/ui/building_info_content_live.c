#include "pharaoh/ui/building_info_content_live.h"
#include "pharaoh/core/static_objects.h"
#include "pharaoh/ui/terrain_panels.h"
#include <stddef.h>

static int32_t signed_bits(uint32_t x)
{ return x<=INT32_MAX?(int32_t)x:(int32_t)(x&INT32_MAX)-INT32_MAX-1; }
static int32_t add32(int32_t a,int32_t b)
{ return signed_bits((uint32_t)a+(uint32_t)b); }
static int32_t sub32(int32_t a,int32_t b)
{ return signed_bits((uint32_t)a-(uint32_t)b); }
static int32_t mul32(int32_t a,uint32_t b)
{ return signed_bits((uint32_t)a*b); }
static void *pass_data(const PharaohCityRenderOperations *ops,enum PharaohCityRenderPass pass)
{ return ops->pass_user_data[pass]?ops->pass_user_data[pass]:ops->user_data; }

/* Whole430E60. Unlike430D30 this selected-actor pass does not advance the
 * animation tick, invoke overlays/footprints or decay the highlight count. */
void pharaoh_info_render_selected_scene(PharaohCityRenderContext *c,
    int32_t x,int32_t y,int32_t grid_origin,int32_t actor)
{
    const PharaohCityRenderLiveBindings *const l=c->live;
    uint32_t *const camera_x=l?l->camera_x:(uint32_t *)(void *)&c->state->camera_x;
    uint32_t *const camera_y=l?l->camera_y:(uint32_t *)(void *)&c->state->camera_y;
    uint32_t *const origin=l?l->grid_origin:(uint32_t *)(void *)&c->state->grid_origin;
    int32_t *const left=l?l->clip_left:&c->clip->left;
    int32_t *const right=l?l->clip_right:&c->clip->right;
    int32_t *const top=l?l->clip_top:&c->clip->top;
    int32_t *const bottom=l?l->clip_bottom:&c->clip->bottom;
    pharaoh_city_render_sync_viewport(c);
    *camera_x=(uint32_t)x;*camera_y=(uint32_t)y;*origin=(uint32_t)grid_origin;
    *left=c->viewport->screen_x;*right=c->viewport->screen_right;
    *top=c->viewport->screen_y;*bottom=c->viewport->screen_bottom;
    const PharaohCityRenderOperations *ops=c->operations;
    ops->draw_base(pass_data(ops,PHARAOH_CITY_PASS_BASE));
    ops->draw_top(pass_data(ops,PHARAOH_CITY_PASS_TOP),actor);
    ops->draw_high_walkers(pass_data(ops,PHARAOH_CITY_PASS_HIGH_WALKERS),0);
    *left=0;*right=*c->screen_width;*top=0;*bottom=*c->screen_height;
}
/* Whole4FA330. The three renderer endpoints above are their genuine full
 * city C providers, drawing into the same framebuffer as the normal view. */
void pharaoh_info_prepare_walker_preview(const PharaohInfoPreviewContext *c,int32_t grid,int32_t actor)
{
    const int32_t saved_x=c->view->state->camera_x,saved_y=c->view->state->camera_y;
    pharaoh_city_view_grid_offset_to_xy_view(c->view->projection,grid,c->projected_x,c->projected_y);
    c->view->state->camera_x=sub32(*c->projected_x,2);
    c->view->state->camera_y=sub32(*c->projected_y,6);
    pharaoh_city_view_check_camera_boundaries(c->view);
    pharaoh_city_view_set_editor_viewport(c->view);
    pharaoh_info_render_selected_scene(c->render,c->view->state->camera_x,
        c->view->state->camera_y,*c->grid_origin,actor);
    c->view->state->camera_y=saved_y;c->view->state->camera_x=saved_x;
    if(*c->panel_position_mode==1)pharaoh_city_view_set_viewport_without_sidebar(c->view);
    else pharaoh_city_view_set_viewport_with_sidebar(c->view);
    pharaoh_city_render_sync_viewport(c->render);
}
_Static_assert(sizeof(PharaohSpriteImageRecord)==64&&
    offsetof(PharaohSpriteImageRecord,height)==0x16,"native preview image metadata");
/* Whole4FA3C0: width12 counts96 raw bytes, not12 pixels. */
void pharaoh_info_prepare_walker_pictures(const PharaohInfoPreviewContext *c)
{
    if(c->ui->window->walker_count<=0)return;
    int32_t frame=add32(*c->menu_bar_images,8);
    if(frame>=*c->image_count)frame=0;
    const int32_t minimum_y=c->images[frame].height;
    for(int32_t i=0;i<c->ui->window->walker_count;++i) {
        const int32_t actor=c->ui->window->walker_ids[i];
        pharaoh_info_prepare_walker_preview(c,c->ui->walkers[actor].grid_offset,actor);
        *c->draw_x=*c->capture_x;*c->draw_y=*c->capture_y;
        *c->draw_width=12;*c->draw_height=48;
        if(*c->capture_x<0)*c->draw_x=0;
        if(*c->capture_y<minimum_y)*c->draw_y=minimum_y;
        const PharaohSpriteRawBlitTarget target={c->surface_pixels,*c->surface_pitch,
            *c->draw_x,*c->draw_y,*c->draw_width,*c->draw_height,0,*c->surface_pitch-96};
        pharaoh_graphics_save_to_buffer(&target,(uint32_t *)(c->preview_base+c->preview_offsets[i]));
    }
    pharaoh_city_render_current_view(c->render);
}

static PharaohBuildingInfoLiveContext *ui(const PharaohInfoContentDrawContext *c)
{ return c->drawing->ui; }
static PharaohBuildingTypeRecord *building(const PharaohInfoContentDrawContext *c)
{ return &ui(c)->buildings[ui(c)->window->building_id]; }
static void outer(const PharaohInfoContentDrawContext *c)
{
    const PharaohBuildingInfoLiveState *w=ui(c)->window;
    pharaoh_panel_frame_draw_native(c->drawing->layout->outer_panel,w->x,w->y,w->width_blocks,w->height_blocks);
}
static void label(const PharaohInfoContentDrawContext *c,int32_t group,int32_t entry,int32_t x,int32_t y)
{
    const PharaohStorageInfoDrawContext *d=c->drawing;
    pharaoh_ui_text_draw_language(d->text,d->font_green,group,entry,x,y,*ui(c)->font_green,
        0,ui(c)->window->x,ui(c)->window->y);
}
static void number(const PharaohInfoContentDrawContext *c,int32_t value,int32_t x,int32_t y)
{
    const PharaohStorageInfoDrawContext *d=c->drawing;
    pharaoh_ui_text_draw_number(d->text,d->font_green,value,'@',d->employee_suffix,x,y,
        *ui(c)->font_green,0,ui(c)->window->x,ui(c)->window->y);
}
static void centered(const PharaohInfoContentDrawContext *c,int32_t group,int32_t entry,
    int32_t y,const PharaohBitmapFont *font,int32_t font_id)
{
    const PharaohBuildingInfoLiveState *w=ui(c)->window;
    pharaoh_ui_text_draw_centered_language(c->drawing->text,font,group,entry,w->x,y,
        w->width_blocks*16,font_id,0,w->x,w->y);
}
static void description(const PharaohInfoContentDrawContext *c,int32_t group,int32_t entry,int32_t y)
{
    const PharaohStorageInfoDrawContext *d=c->drawing;
    pharaoh_info_ui_draw_multiline(d->layout,d->wrapped,d->font_black,group,entry,
        ui(c)->window->x+32,y,(ui(c)->window->width_blocks-4)*16,*ui(c)->font_black,
        0,ui(c)->window->x,ui(c)->window->y);
}
static void reset_first_draw(const PharaohInfoContentDrawContext *c)
{
    if(ui(c)->window->first_draw) {
        (void)pharaoh_native_return_zero_0047d350();ui(c)->window->first_draw=0;
    }
}
/* Whole4FA4A0. Native terrain group is70 and title offset is8. */
void pharaoh_info_draw_no_info(const PharaohInfoContentDrawContext *c)
{
    outer(c);
    centered(c,70,0,ui(c)->window->y+8,c->drawing->font_title,*ui(c)->font_title);
    centered(c,13,1,ui(c)->window->height_blocks*16-22+ui(c)->window->y,
        c->drawing->font_green,*ui(c)->font_green);
}
/* Whole4FC150. */
void pharaoh_info_draw_house_population(const PharaohInfoContentDrawContext *c,int32_t y)
{
    *ui(c)->text_width=0;
    (void)pharaoh_image_draw(c->drawing->layout->inner_panel->drawing,*c->drawing->context_icons+13,
        ui(c)->window->x+34,y+4);
    y+=14;
    number(c,building(c)->house_population,ui(c)->window->x+50+*ui(c)->text_width,y);
    label(c,127,20,ui(c)->window->x+50+*ui(c)->text_width,y);
    const int32_t room=building(c)->house_population_room;
    if(room<0) {
        number(c,-room,ui(c)->window->x+50+*ui(c)->text_width,y);
        label(c,127,21,ui(c)->window->x+50+*ui(c)->text_width,y);
    } else if(room>0) {
        label(c,127,22,ui(c)->window->x+50+*ui(c)->text_width,y);
        number(c,building(c)->house_population_room,ui(c)->window->x+50+*ui(c)->text_width,y);
    }
}
/* Whole4FC2D0. Amount resets its own width; Native restores the saved label
 * advance by addition before drawing the final tax phrase. */
void pharaoh_info_draw_house_taxes(const PharaohInfoContentDrawContext *c,int32_t y)
{
    *ui(c)->text_width=0;
    if(building(c)->tax_collector_service==0) {
        label(c,127,23,ui(c)->window->x+36,y);return;
    }
    const int32_t income=pharaoh_building_house_tax_income(ui(c)->buildings,ui(c)->window->building_id);
    label(c,127,24,ui(c)->window->x+36+*ui(c)->text_width,y);
    const int32_t width=*ui(c)->text_width;
    pharaoh_info_text_draw_amount(c->drawing->text,c->drawing->font_green,c->drawing->number_suffix,
        8,0,income,ui(c)->window->x+36+*ui(c)->text_width,y,*ui(c)->font_green,0);
    *ui(c)->text_width=add32(*ui(c)->text_width,width);
    label(c,127,25,ui(c)->window->x+36+*ui(c)->text_width,y);
}
/* Whole4FC3B0: Native sentiment is a signed BYTE at building+C2. */
void pharaoh_info_draw_house_crime(const PharaohInfoContentDrawContext *c,int32_t y)
{
    const uint8_t bits=building(c)->native_c2;
    const int32_t sentiment=bits<128?bits:(int32_t)bits-256;
    const int32_t entry=sentiment>=50?26:sentiment>=40?27:sentiment>=30?28:
        sentiment>=20?29:sentiment>=10?30:sentiment>=1?31:32;
    label(c,127,entry,ui(c)->window->x+36,y);
}
/* Whole4FC4F0, keeping the genuine48894A0 child and its low-BYTE result. */
void pharaoh_info_draw_farm_workers(const PharaohInfoContentDrawContext *c,int32_t y)
{
    if((uint8_t)pharaoh_building_has_active_occupant(c->occupancy,ui(c)->window->building_id)) {
        pharaoh_info_ui_draw_multiline(c->drawing->layout,c->drawing->wrapped,c->drawing->font_green,
            177,building(c)->num_workers==0?5:6,ui(c)->window->x+65,y+10,
            (ui(c)->window->width_blocks-4)*16,*ui(c)->font_green,0,ui(c)->window->x,ui(c)->window->y);
        (void)pharaoh_image_draw(c->drawing->layout->inner_panel->drawing,*c->drawing->context_icons+14,
            ui(c)->window->x+40,y+6);
    } else pharaoh_info_draw_employment(c->drawing,y);
}
static void simple_terrain(const PharaohInfoContentDrawContext *c,int32_t help,int32_t group,
    int32_t title,int32_t entry,int32_t prepare)
{
    ui(c)->window->help_id=help;reset_first_draw(c);
    if(prepare)pharaoh_info_prepare_walker_pictures(c->previews);
    outer(c);centered(c,group,title,ui(c)->window->y+10,c->drawing->font_title,*ui(c)->font_title);
    description(c,group,entry,ui(c)->window->height_blocks*16-158+ui(c)->window->y);
}
void pharaoh_info_draw_mud_wall(const PharaohInfoContentDrawContext *c)
{ simple_terrain(c,85,139,0,1,0); }
void pharaoh_info_draw_brick_wall(const PharaohInfoContentDrawContext *c)
{ simple_terrain(c,85,165,0,1,0); }
void pharaoh_info_draw_strong_wall(const PharaohInfoContentDrawContext *c)
{ simple_terrain(c,85,166,0,1,0); }
void pharaoh_info_draw_statue(const PharaohInfoContentDrawContext *c)
{ simple_terrain(c,79,80,0,1,1); }
void pharaoh_info_draw_triumphal_arch(const PharaohInfoContentDrawContext *c)
{ simple_terrain(c,79,80,2,3,1); }
/* Whole5057F0. */
void pharaoh_info_draw_burning_ruin(const PharaohInfoContentDrawContext *c)
{
    ui(c)->window->help_id=0;reset_first_draw(c);outer(c);
    centered(c,111,0,ui(c)->window->y+10,c->drawing->font_title,*ui(c)->font_title);
    pharaoh_ui_text_draw_language(c->drawing->text,c->drawing->font_black,41,
        ui(c)->window->details.rubble_type,ui(c)->window->x+32,
        ui(c)->window->height_blocks*16-173+ui(c)->window->y,*ui(c)->font_black,
        0,ui(c)->window->x,ui(c)->window->y);
    description(c,111,1,ui(c)->window->height_blocks*16-143+ui(c)->window->y);
}

/* Whole50F0F0. Both result fields alias the actual window-state overlays. */
void pharaoh_info_house_find_worst_desirability(const PharaohInfoWorstDesirabilityContext *c)
{
    int32_t selected=c->ui->window->building_id;
    const PharaohBuildingTypeRecord *house=&c->ui->buildings[selected];
    int32_t left=house->x-6,right=house->x+6,top=house->y-6,bottom=house->y+6;
    *c->worst_desirability_score=0;*c->worst_desirability_building=0;
    if(left<0)left=0;if(top<0)top=0;
    if(right>=*c->map_width)right=*c->map_width-1;
    if(bottom>=*c->map_height)bottom=*c->map_height-1;
    uint32_t cell=(uint32_t)*c->grid_origin+(uint32_t)top*228u+(uint32_t)left;
    const uint32_t step=(uint32_t)left-(uint32_t)right+227u;
    for(int32_t y=top;y<=bottom;++y) {
        if(left<=right) {
            const uint32_t after=cell+(uint32_t)right-(uint32_t)left+1u;
            for(int32_t x=left;x<=right;++x,++cell) {
                const int32_t id=c->building_ids[cell];
                if(id<=0)continue;
                const PharaohBuildingTypeRecord *b=&c->ui->buildings[id];
                if(b->state!=1||id==selected)continue;
                if(b->house_size&&b->building_type>=c->ui->buildings[selected].building_type)continue;
                const int32_t type=b->building_type;
                int32_t value=c->models[type].desirability_value;
                if(value>=0)continue;
                const int32_t change=c->models[type].desirability_step_size;
                const int32_t range=c->models[type].desirability_range;
                house=&c->ui->buildings[selected];
                const int32_t dx=x>house->x?x-house->x:house->x-x;
                const int32_t dy=y>house->y?y-house->y:house->y-y;
                *c->distance_x=dx;*c->distance_y=dy;
                const int32_t distance=dx<dy?dy:dx;
                if(distance<=range) {
                    if(distance>1)value=add32(value,mul32(distance-1,(uint32_t)change));
                    if(value<*c->worst_desirability_score) {
                        *c->worst_desirability_score=value;
                        *c->worst_desirability_building=type==54?
                            pharaoh_building_follow_link(c->ui->buildings,id):id;
                    }
                }
                selected=c->ui->window->building_id;
            }
            cell=after;
        }
        cell+=step;
    }
}
_Static_assert(sizeof(PharaohInfoEmpireCulture)==8,"native empire culture stride");
static int trade_limit(const PharaohInfoHouseTradeContext *c,int32_t city)
{
    const PharaohScenarioPlayerRuntime *p=&c->supply->players[*c->supply->current_player];
    return pharaoh_empire_city_has_trade_limit(c->supply->cities,c->supply->route_limits,
        city,19,p->ra_trade_bonus_months,p->ra_trade_penalty_months,p->ra_trade_minor_penalty_months);
}
/* Whole50E3C0. The import selector is the low BYTE; counters are not reset. */
int32_t pharaoh_info_house_trade_goods(const PharaohInfoHouseTradeContext *c,int32_t counts[16],int32_t imports)
{
    if((uint8_t)imports==0) {
        if(pharaoh_empire_can_produce_resource_potentially(c->supply,18))counts[0]=1;
    } else if(*c->luxury_industry_count<1)counts[0]=1;
    for(int32_t id=1;id<61;++id) {
        const PharaohEmpireCityRecord *city=&c->supply->cities[id];
        if(city->in_use&&(city->kind==1||city->kind==3||city->kind==5)&&city->sells[19]&&trade_limit(c,id)) {
            const int32_t slot=c->cultures[city->name_id].luxury_name-38;
            counts[slot]=add32(counts[slot],1);
        }
    }
    if((uint8_t)imports!=0) {
        for(int32_t id=1;id<61;++id) {
            const PharaohEmpireCityRecord *city=&c->supply->cities[id];
            if(city->in_use&&(*c->luxury_import_status==1||*c->luxury_import_status==3)&&
                city->sells[19]&&trade_limit(c,id)) {
                const int32_t slot=c->cultures[city->name_id].luxury_name-38;
                if(counts[slot]!=0)counts[slot]=0;
            }
        }
    }
    int32_t count=0;for(int32_t i=0;i<16;++i)if(counts[i]!=0)++count;
    return count;
}

_Static_assert(offsetof(PharaohWalkerQueryRecord,reserved_06e)==0x6e&&
    offsetof(PharaohWalkerQueryRecord,destination_payload_074)==0x74&&
    offsetof(PharaohWalkerQueryRecord,reserved_07c)==0x7c&&
    offsetof(PharaohWalkerQueryRecord,reserved_131)==0x131,
    "native walker city, signed load, phrase and tutorial overlays");
_Static_assert(sizeof(PharaohTraderRecord)==88&&
    offsetof(PharaohTraderRecord,bought_resources)==8&&
    offsetof(PharaohTraderRecord,sold_resources)==44,"native trader information rows");

static PharaohBuildingInfoLiveContext *walker_ui(const PharaohInfoWalkerContentContext *c)
{ return ui(c->content); }
static void walker_text(const PharaohInfoWalkerContentContext *c,int32_t group,
    int32_t entry,int32_t x,int32_t y,int32_t name_font)
{
    const PharaohStorageInfoDrawContext *d=c->content->drawing;
    const PharaohBuildingInfoLiveState *w=walker_ui(c)->window;
    pharaoh_ui_text_draw_language(d->text,name_font?c->font_name:d->font_green,
        group,entry,x,y,name_font?*c->font_name_id:*walker_ui(c)->font_green,0,w->x,w->y);
}
static void walker_image(void *data,int32_t image,int32_t x,int32_t y)
{
    const PharaohInfoWalkerContentContext *c=data;
    (void)pharaoh_image_draw(c->content->drawing->layout->inner_panel->drawing,image,x,y);
}
static void walker_portrait(const PharaohInfoWalkerContentContext *c,int32_t image)
{
    if(image>=0)walker_image((void *)c,image,walker_ui(c)->window->x+28,
        walker_ui(c)->window->y+112);
}
static void walker_name(const PharaohInfoWalkerContentContext *c,
    const PharaohWalkerQueryRecord *f,int32_t x)
{
    if(f->name_group!=-1)walker_text(c,f->name_group,f->name_id,x,
        walker_ui(c)->window->y+108,1);
}
static void walker_kind(const PharaohInfoWalkerContentContext *c,
    const PharaohWalkerQueryRecord *f,int32_t type)
{
    PharaohBuildingInfoLiveContext *u=walker_ui(c);
    *u->text_width=0;walker_name(c,f,u->window->x+90);*u->text_width=0;
    walker_text(c,64,type,u->window->x+92,u->window->y+139,0);
}
static void walker_phrase(const PharaohInfoWalkerContentContext *c,
    const PharaohWalkerQueryRecord *f,int32_t group,int32_t y)
{
    const int32_t phrase=f->reserved_07c[1];
    if(group!=-1&&phrase<128) {
        const PharaohStorageInfoDrawContext *d=c->content->drawing;
        const PharaohBuildingInfoLiveState *w=walker_ui(c)->window;
        pharaoh_info_ui_draw_multiline(d->layout,d->wrapped,d->font_green,group,phrase,
            w->x+90,w->y+y,(w->width_blocks-9)*16,*walker_ui(c)->font_green,0,w->x,w->y);
    }
}
static int32_t resource_is_stone(int32_t resource)
{ return resource==24||resource==25||resource==26||resource==30||resource==35; }
static int32_t resource_is_unit(int32_t resource)
{ return resource_is_stone(resource)||resource==28||resource==10||resource==36; }
static int32_t docker_idle(const PharaohWalkerQueryRecord *f)
{ return f->type==38&&f->action_state==8; }
static int32_t carrier_returning(const PharaohWalkerQueryRecord *f)
{
    const int32_t a=f->action_state;
    return (f->type==4&&a==15)||(f->type==9&&(a==11||a==14||a==17))||
        (f->type==38&&(a==10||a==13||a==14));
}
static void carrier_route(const PharaohInfoWalkerContentContext *c,
    const PharaohWalkerQueryRecord *f)
{
    const int32_t home=f->building_id,destination=f->entertainment_building_id;
    if(home==0||destination==0)return;
    PharaohBuildingInfoLiveContext *u=walker_ui(c);*u->text_width=0;
    const int32_t returning=carrier_returning(f);
    if(docker_idle(f))return;
    const int32_t target=returning?home:destination,source=returning?destination:home;
    const int32_t x=u->window->x+40,y=u->window->y+200;
    walker_text(c,129,returning?16:15,x,y,0);
    walker_text(c,41,u->buildings[target].building_type,x+*u->text_width,y,0);
    walker_text(c,129,14,x+*u->text_width,y,0);
    walker_text(c,41,u->buildings[source].building_type,x+*u->text_width,y,0);
}
static void carrier_load_name(const PharaohInfoWalkerContentContext *c,
    const PharaohWalkerQueryRecord *f,int32_t amount)
{
    if(amount==0)return;
    PharaohBuildingInfoLiveContext *u=walker_ui(c);
    const int32_t r=f->resource_id,x=u->window->x+110,y=u->window->y+152;
    if(resource_is_stone(r))walker_text(c,99,amount==1?23:24,x+*u->text_width,y,0);
    else if(r!=28&&r!=10&&r!=0)walker_text(c,129,20,x+*u->text_width,y,0);
    walker_text(c,23,((r==28||r==10)&&amount!=1)?r+54:r,x+*u->text_width,y,0);
}
static void carrier_info(const PharaohInfoWalkerContentContext *c,
    const PharaohWalkerQueryRecord *f,int32_t image,int32_t phrase)
{
    PharaohBuildingInfoLiveContext *u=walker_ui(c);
    walker_portrait(c,image);*u->text_width=0;walker_name(c,f,u->window->x+90);
    *u->text_width=0;
    walker_text(c,41,u->buildings[f->building_id].building_type,u->window->x+92,u->window->y+139,0);
    walker_text(c,64,4,u->window->x+92+*u->text_width,u->window->y+139,0);
    *u->text_width=0;
    if(!docker_idle(f)&&f->destination_payload_074!=0&&f->resource_id!=0)
        walker_image((void *)c,*u->resource_images+f->resource_id,u->window->x+92,u->window->y+150);
    const uint32_t bits=f->destination_payload_074;
    int32_t amount=bits<=INT16_MAX?(int32_t)bits:(int32_t)bits-65536;
    if(resource_is_unit(f->resource_id))amount/=100;
    if(amount!=0)number(c->content,amount,u->window->x+110+*u->text_width,u->window->y+152);
    carrier_load_name(c,f,amount);walker_phrase(c,f,phrase,170);carrier_route(c,f);
}
static int32_t walker_trade_limit(const PharaohInfoWalkerContentContext *c,int32_t city,int32_t r)
{
    const PharaohEmpireSupplyContext *s=c->supply;
    const PharaohScenarioPlayerRuntime *p=&s->players[*s->current_player];
    return pharaoh_empire_city_has_trade_limit(s->cities,s->route_limits,city,r,
        p->ra_trade_bonus_months,p->ra_trade_penalty_months,p->ra_trade_minor_penalty_months);
}
static void city_trade_row(const PharaohInfoWalkerContentContext *c,
    const uint8_t *trades,int32_t city,int32_t text,int32_t y,int32_t icon_y)
{
    PharaohBuildingInfoLiveContext *u=walker_ui(c);*u->text_width=0;
    walker_text(c,129,text,u->window->x+40,u->window->y+y,0);
    for(int32_t r=1;r<36;++r)if(trades[r]&&walker_trade_limit(c,city,r)) {
        walker_image((void *)c,*u->resource_images+r,u->window->x+40+*u->text_width,u->window->y+icon_y);
        *u->text_width=add32(*u->text_width,25);
    }
}
static void trader_row(const PharaohInfoWalkerContentContext *c,
    const uint8_t *amounts,int32_t text,int32_t y,int32_t icon_y)
{
    PharaohBuildingInfoLiveContext *u=walker_ui(c);*u->text_width=0;
    walker_text(c,129,text,u->window->x+40,u->window->y+y,0);
    for(int32_t r=1;r<36;++r)if(amounts[r]) {
        const int32_t amount=resource_is_unit(r)?amounts[r]:amounts[r]*100;
        number(c->content,amount,u->window->x+40+*u->text_width,u->window->y+y);
        walker_image((void *)c,*u->resource_images+r,u->window->x+40+*u->text_width,u->window->y+icon_y);
        *u->text_width=add32(*u->text_width,25);
    }
}
static const PharaohWalkerQueryRecord *trader_info(const PharaohInfoWalkerContentContext *c,
    const PharaohWalkerQueryRecord *f,int32_t type)
{
    PharaohBuildingInfoLiveContext *u=walker_ui(c);
    if(f->type==21)f=&u->walkers[f->leading_walker_id];
    if(f->type==21)f=&u->walkers[f->leading_walker_id];
    const int32_t city=f->reserved_06e[0];
    *u->text_width=0;
    walker_text(c,64,type,u->window->x+40,u->window->y+110,0);
    walker_text(c,*c->classical_names?21:195,c->supply->cities[city].name_id,
        u->window->x+40+*u->text_width,u->window->y+110,0);
    const PharaohTraderRecord *t=&c->traders[f->trader_record_id];
    const int32_t traded=t->sold_count!=0||t->bought_count!=0,a=f->action_state;
    int32_t text;
    if(type==20)text=a==12?6:a==10?7:a==13?8:9;
    else text=a==10?10:a==11?(traded?8:13):a==9?12:8;
    walker_text(c,129,text,u->window->x+40,u->window->y+150,0);
    if(!traded) {
        city_trade_row(c,c->supply->cities[city].buys,city,2,170,167);
        city_trade_row(c,c->supply->cities[city].sells,city,3,200,197);
    } else {
        trader_row(c,t->bought_resources,4,170,167);
        trader_row(c,t->sold_resources,5,200,197);
    }
    return f;
}
static void walker_goods(const PharaohInfoWalkerContentContext *c,
    int32_t walker,int32_t delivered,int32_t x,int32_t y)
{
    PharaohBuildingInfoLiveContext *u=walker_ui(c);
    if(delivered)pharaoh_building_info_draw_walker_goods(u->walkers,walker,x,y,
        *u->resource_images,u->text_width,walker_image,(void *)c);
    else pharaoh_building_info_draw_carried_goods(u->walkers,walker,x,y,
        *u->resource_images,u->text_width,walker_image,(void *)c);
}
/* Whole4FAB20. The helper split follows the existing complete Window.c;
 * constants, saved-pool overlays and fonts are pinned to the Native body. */
void pharaoh_info_draw_walker(const PharaohInfoWalkerContentContext *c,int32_t walker)
{
    PharaohBuildingInfoLiveContext *u=walker_ui(c);
    const PharaohBuildingInfoLiveState *w=u->window;
    const PharaohWalkerQueryRecord *f=&u->walkers[walker];
    int32_t type=f->type;
    const int32_t phrase=pharaoh_info_walker_portrait_code(c->portrait_codes,walker);
    if(type==91)type=f->resource_id;
    u->window->help_id=42;
    int32_t image=pharaoh_building_info_walker_big_image(u->walkers,u->buildings,
        c->portrait_scripts,c->portrait_images,(uint8_t)*c->portrait_codes->selected_scenario,walker);
    pharaoh_info_layout_draw_border(c->content->drawing->layout,0,w->x+24,w->y+102,
        (w->width_blocks-3)*16,122,0,w->x,w->y);
    const int32_t own_type=f->type;
    if(type==19||type==20||type==21)f=trader_info(c,f,type);
    else if((type>=43&&type<=53)||type==99) {
        *u->text_width=0;walker_name(c,f,w->x+28);*u->text_width=0;
        walker_text(c,37,u->formations[f->formation_id_60].enemy_type+14,w->x+28,w->y+149,0);
    } else if(own_type==92||own_type==93||own_type==100||own_type==101) {
        *u->text_width=0;walker_text(c,64,own_type,w->x+28,w->y+108,1);
    } else if(own_type==25||(own_type>=67&&own_type<=70)) {
        walker_portrait(c,image);*u->text_width=0;
        walker_text(c,64,type,w->x+92,w->y+139,0);
    } else if(type==4||type==9||type==38)carrier_info(c,f,image,phrase);
    else if(type==39) {
        walker_portrait(c,image);walker_kind(c,f,39);
        if(f->action_state==8||f->action_state==9) {
            walker_text(c,129,f->action_state==8?17:18,w->x+90+*u->text_width,w->y+139,0);
            walker_goods(c,walker,f->action_state==9,w->x+90+*u->text_width,w->y+135);
        }
        walker_phrase(c,f,phrase,160);
    } else if(type==66) {
        static const int32_t resources[12]={1,2,3,4,5,6,7,8,15,13,17,19};
        walker_portrait(c,image);
        const int32_t slot=f->carrier_stock_index,resource=slot<12?resources[slot]:-1;
        walker_kind(c,f,66);
        walker_text(c,129,21,w->x+90+*u->text_width,w->y+139,0);
        pharaoh_building_info_draw_resource_icon(resource,w->x+90+*u->text_width,w->y+135,
            *u->resource_images,u->text_width,walker_image,(void *)c);
        walker_phrase(c,f,phrase,160);
    } else if(type==73) {
        walker_kind(c,f,73);walker_portrait(c,image);
        if(f->action_state==12) {
            walker_text(c,129,18,w->x+94+*u->text_width,w->y+139,0);
            walker_image((void *)c,*u->resource_images+8,w->x+94+*u->text_width,w->y+135);
        }
        walker_phrase(c,f,phrase,160);
    } else {
        if(own_type==86)image=-1;
        if(own_type==85)type=f->leading_walker_id?96:85;
        walker_portrait(c,image);*u->text_width=0;
        if(f->name_group>=0)walker_text(c,f->name_group,f->name_id,w->x+90,w->y+108,1);
        *u->text_width=0;
        if(type==27&&pharaoh_building_group_index(u->buildings,f->building_id)==4)
            walker_text(c,59,38,w->x+92+*u->text_width,w->y+139,0);
        else walker_text(c,64,type,w->x+92+*u->text_width,w->y+139,0);
        walker_phrase(c,f,phrase,160);
    }
    const int32_t button=f->reserved_131;
    if(button!=0&&u->tutorial_flags[button]) {
        PharaohImageButtonDrawState buttons=*c->image_buttons;
        buttons.draw_sprite=walker_image;buttons.user_data=(void *)c;
        pharaoh_image_buttons_draw(w->x+40,w->y+w->height_blocks*16-40,
            u->tutorial_buttons[button].buttons,1,&buttons);
    }
}

/* Whole4FA780. EADCE4 is the selected list index, independent of the
 * minimum-desirability score atEADCD8. Picture offsets are raw BYTE offsets. */
void pharaoh_info_draw_walker_list(const PharaohInfoWalkerContentContext *c)
{
    PharaohBuildingInfoLiveContext *u=walker_ui(c);
    PharaohBuildingInfoLiveState *w=u->window;
    const PharaohInfoLayoutDrawContext *layout=c->content->drawing->layout;
    pharaoh_info_layout_draw_inner_panel(layout,w->x+16,w->y+40,w->width_blocks-2,12,w->x,w->y);
    if(w->walker_count<=0) {
        centered(c->content,70,0,w->y+120,c->content->drawing->font_green,*u->font_green);
        w->selection=1;return;
    }
    const PharaohInfoPreviewContext *p=c->content->previews;
    int32_t dx=0;
    for(int32_t i=0;i<w->walker_count;++i,dx+=60) {
        pharaoh_info_layout_draw_border(layout,0,w->x+dx+25,w->y+45,52,52,
            i==w->selected_walker_index,w->x,w->y);
        *p->draw_x=w->x+dx+27;*p->draw_y=w->y+47;
        *p->draw_width=12;*p->draw_height=48;
        const PharaohSpriteRawBlitTarget target={p->surface_pixels,*p->surface_pitch,
            *p->draw_x,*p->draw_y,12,48,0,*p->surface_pitch-96};
        pharaoh_graphics_draw_from_buffer(&target,
            (const uint32_t *)(p->preview_base+p->preview_offsets[i]));
    }
    pharaoh_info_draw_walker(c,w->walker_ids[w->selected_walker_index]);w->selection=1;
}

/* These adapters bind the existing complete terrain leaves to their actual
 * panel, text, capture and list children; none replaces an artwork operation. */
static void terrain_title(void *data,int32_t group,int32_t entry)
{
    const PharaohInfoWalkerContentContext *c=data;
    outer(c->content);centered(c->content,group,entry,walker_ui(c)->window->y+10,
        c->content->drawing->font_title,*walker_ui(c)->font_title);
}
static void terrain_text(void *data,int32_t group,int32_t entry,int32_t y)
{ description(((const PharaohInfoWalkerContentContext *)data)->content,group,entry,y); }
static void terrain_prepare(void *data)
{ pharaoh_info_prepare_walker_pictures(((const PharaohInfoWalkerContentContext *)data)->content->previews); }
static void terrain_walkers(void *data)
{ pharaoh_info_draw_walker_list(data); }
static void terrain_type(void *data,int32_t type,int32_t x,int32_t y)
{
    const PharaohInfoWalkerContentContext *c=data;
    const PharaohStorageInfoDrawContext *d=c->content->drawing;
    const PharaohBuildingInfoLiveState *w=walker_ui(c)->window;
    pharaoh_ui_text_draw_language(d->text,d->font_black,41,type,x,y,
        *walker_ui(c)->font_black,0,w->x,w->y);
}
static void terrain_leaf(const PharaohInfoWalkerContentContext *c,int32_t kind)
{
    PharaohBuildingInfoLiveState *w=walker_ui(c)->window;
    static const int32_t help_ids[4]={79,59,79,0};
    PharaohTerrainPanelContext p={0};
    p.panel_x=w->x;p.panel_y=w->y;p.panel_height_blocks=w->height_blocks;
    p.block_height=16;p.text_x=32;p.canal_has_water=w->details.canal;
    p.rubble_building_type=w->details.rubble_type;p.help_gardens=0;p.help_canal=1;
    p.help_plaza=2;p.help_ruins=3;p.help_messages=help_ids;
    p.draw_title=terrain_title;p.draw_text=terrain_text;p.prepare_walker_list=terrain_prepare;
    p.draw_walker_list=terrain_walkers;p.draw_building_type=terrain_type;p.user_data=(void *)c;
    w->help_id=help_ids[kind];reset_first_draw(c->content);
    if(kind==0)pharaoh_draw_gardens_panel(&p);
    else if(kind==1)pharaoh_draw_canal_panel(&p);
    else if(kind==2)pharaoh_draw_plaza_panel(&p);
    else pharaoh_draw_ruins_panel(&p);
}
/* Whole4FA530. All special terrain branches use complete canonical leaves;
 * the first selected voice follows the real speech/sample stream providers. */
void pharaoh_info_draw_terrain(const PharaohInfoTerrainContentContext *c)
{
    const PharaohInfoWalkerContentContext *a=c->walkers;
    PharaohBuildingInfoLiveContext *u=walker_ui(a);
    PharaohBuildingInfoLiveState *w=u->window;
    const int32_t kind=w->terrain_kind;
    w->help_id=kind==6?57:kind==7?59:(kind==9||kind==23||kind==24)?85:kind==11?58:0;
    if(kind==12){terrain_leaf(a,0);return;}
    if(kind==13&&w->walker_count<=0){terrain_leaf(a,2);return;}
    if(kind==9){pharaoh_info_draw_mud_wall(a->content);return;}
    if(kind==23){pharaoh_info_draw_brick_wall(a->content);return;}
    if(kind==24){pharaoh_info_draw_strong_wall(a->content);return;}
    if(kind==7){terrain_leaf(a,1);return;}
    if(kind==8){terrain_leaf(a,3);return;}
    if(w->first_draw) {
        w->first_draw=0;
        if(w->walker_count<=0)(void)pharaoh_native_return_zero_0047d350();
        else pharaoh_info_walker_selection_voice(c->selection_voice);
    }
    if(w->walker_count>0) {
        const int32_t walker=w->walker_ids[w->selected_walker_index];
        if(walker!=0)w->help_id=u->walkers[walker].type<67?42:330;
    }
    pharaoh_info_prepare_walker_pictures(a->content->previews);outer(a->content);
    if(w->walker_count==0)centered(a->content,70,w->terrain_kind+10,w->y+10,
        a->content->drawing->font_title,*u->font_title);
    if(w->terrain_kind!=6&&w->terrain_kind!=13) {
        const PharaohStorageInfoDrawContext *d=a->content->drawing;
        pharaoh_info_ui_draw_multiline(d->layout,d->wrapped,d->font_black,70,
            w->terrain_kind==3?39:w->terrain_kind+36,w->x+36,
            w->y+w->height_blocks*16-113,(w->width_blocks-4)*16,*u->font_black,0,w->x,w->y);
    }
    pharaoh_info_draw_walker_list(a);
}

/* Whole50EFB0. The road child is the genuine47FE60 radius-search owner. */
void pharaoh_info_draw_vacant_lot(const PharaohInfoHouseContentContext *c)
{
    const PharaohInfoContentDrawContext *d=c->walkers->content;
    const PharaohBuildingInfoLiveState *w=ui(d)->window;
    pharaoh_info_prepare_walker_pictures(d->previews);outer(d);
    centered(d,128,0,w->y+8,d->drawing->font_title,*ui(d)->font_title);
    centered(d,13,1,w->y+w->height_blocks*16-22,d->drawing->font_black,*ui(d)->font_black);
    pharaoh_info_draw_walker_list(c->walkers);
    const PharaohBuildingTypeRecord *b=building(d);
    const uint32_t road=pharaoh_map_road_exists_within_distance(c->roads,b->x,b->y,1,2);
    description(d,128,road?1:2,w->y+w->height_blocks*16-113);
}

_Static_assert(offsetof(PharaohHouseEconomyRecord,evolve_text)==0x85&&
    offsetof(PharaohBuildingTypeRecord,render_warning_087)==0x87&&
    offsetof(PharaohBuildingTypeRecord,native_f0)==0xf0,"native house condition overlays");
static int32_t house_inventory_index(int32_t resource)
{
    if(resource>=1&&resource<=8)return resource-1;
    switch(resource) {
    case 13:return 9;case 15:return 8;case 17:return 10;case 19:return 11;
    default:return -1;
    }
}
static void house_good(const PharaohInfoHouseContentContext *c,int32_t resource,
    int32_t dx,int32_t y)
{
    const PharaohInfoContentDrawContext *a=c->walkers->content;
    const PharaohStorageInfoDrawContext *d=a->drawing;
    PharaohBuildingInfoLiveContext *u=ui(a);const PharaohBuildingInfoLiveState *w=u->window;
    (void)pharaoh_image_draw(d->layout->inner_panel->drawing,*u->resource_images+resource,w->x+dx+32,y);
    const PharaohHouseEconomyRecord *h=&c->houses[w->building_id];
    const int32_t index=house_inventory_index(resource);
    const uint32_t bits=h->unsupported_food;
    const int32_t amount=index>=0?h->market_stock[index]:
        bits<=INT16_MAX?(int32_t)bits:(int32_t)bits-65536;
    pharaoh_ui_text_draw_number(d->text,d->font_black,amount,'@',d->employee_suffix,
        w->x+dx+64,y,*u->font_black,0,w->x,w->y);
}
static void house_label(const PharaohInfoHouseContentContext *c,int32_t group,
    int32_t entry,int32_t x,int32_t y,uint16_t color)
{
    const PharaohInfoContentDrawContext *a=c->walkers->content;
    const PharaohStorageInfoDrawContext *d=a->drawing;
    const PharaohBuildingInfoLiveState *w=ui(a)->window;
    pharaoh_ui_text_draw_language(d->text,d->font_black,group,entry,x,y,*ui(a)->font_black,
        color,w->x,w->y);
}
static void house_raw(const PharaohInfoHouseContentContext *c,const uint8_t *text,int32_t x,int32_t y)
{
    const PharaohStorageInfoDrawContext *d=c->walkers->content->drawing;
    d->text->drawing->font_id=(uint32_t)*d->ui->font_black;
    pharaoh_bitmap_text_draw_native(d->font_black,d->text->drawing,text,x,y,0);
}
static void house_needed_goods(const PharaohInfoHouseContentContext *c,
    const int32_t counts[16],int32_t count,int32_t imports)
{
    const PharaohInfoContentDrawContext *a=c->walkers->content;
    PharaohBuildingInfoLiveContext *u=ui(a);
    int32_t *const advance=&a->drawing->text->drawing->cursor_y;
    const int32_t first_y=*advance;int32_t drawn=0;
    for(int32_t i=0;i<16;++i)if(counts[i]) {
        int32_t line=imports?first_y:*advance;
        if(count>1&&drawn==count-1)
            house_raw(c,c->list_or,u->window->x+32+*u->text_width,u->window->y+line+23);
        else if(drawn>0)
            house_raw(c,c->comma,u->window->x+32+*u->text_width,u->window->y+line+23);
        line=imports?first_y:*advance;
        house_label(c,23,i+39,u->window->x+32+*u->text_width,u->window->y+line+23,0);
        if(++drawn>=count)return;
    }
}
static void house_evolve_goods(const PharaohInfoHouseContentContext *c,int32_t imports)
{
    int32_t counts[16]={0};
    int32_t count=pharaoh_info_house_trade_goods(c->trade,counts,imports);
    if(count>4)count=4;
    else if(count==0){count=1;counts[0]=1;}
    const PharaohInfoContentDrawContext *a=c->walkers->content;
    PharaohBuildingInfoLiveContext *u=ui(a);
    *u->text_width=0;a->drawing->text->drawing->cursor_y=0;
    description(a,127,c->houses[u->window->building_id].evolve_text+40,u->window->y+36);
    a->drawing->text->drawing->cursor_y=add32(a->drawing->text->drawing->cursor_y,16);
    *u->text_width=0;house_needed_goods(c,counts,count,imports);
}
static void house_worst(const PharaohInfoHouseContentContext *c)
{
    const PharaohInfoContentDrawContext *a=c->walkers->content;
    PharaohBuildingInfoLiveContext *u=ui(a);const PharaohBuildingInfoLiveState *w=u->window;
    *u->text_width=0;a->drawing->text->drawing->cursor_y=0;
    house_label(c,127,c->houses[w->building_id].evolve_text+40,w->x+32,w->y+36,0);
    const PharaohBuildingTypeRecord *worst=&u->buildings[w->worst_desirability_building];
    const int32_t vacant=worst->building_type==10&&worst->house_population==0;
    house_label(c,vacant?128:41,vacant?0:worst->building_type,w->x+32+*u->text_width,w->y+36,0xf800);
    house_raw(c,c->close_parenthesis,w->x+32+*u->text_width,w->y+36);
    description(a,127,c->houses[w->building_id].evolve_text+41,w->y+52);
}
/* Whole50E570. Inventory WORDs, evolve BYTE, sickness BYTE and elapsed
 * condition DWORD retain their Native widths and actual pool aliases. */
void pharaoh_info_draw_house(const PharaohInfoHouseContentContext *c)
{
    const PharaohInfoContentDrawContext *a=c->walkers->content;
    const PharaohStorageInfoDrawContext *d=a->drawing;
    PharaohBuildingInfoLiveContext *u=ui(a);PharaohBuildingInfoLiveState *w=u->window;
    const int32_t level_title=building(a)->building_type-10;
    w->help_id=56;reset_first_draw(a);
    if(building(a)->house_population<=0&&building(a)->render_warning_087==0) {
        pharaoh_info_draw_vacant_lot(c);return;
    }
    outer(a);centered(a,29,level_title,w->y+10,d->font_title,*u->font_title);
    w->details_shown=0;
    pharaoh_info_layout_draw_inner_panel(d->layout,w->x+16,w->y+148,w->width_blocks-2,10,w->x,w->y);
    pharaoh_info_draw_house_population(a,w->y+154);
    pharaoh_info_draw_house_taxes(a,w->y+194);pharaoh_info_draw_house_crime(a,w->y+214);
    if(c->models[c->houses[w->building_id].level].food_types==0)
        pharaoh_info_ui_draw_multiline(d->layout,d->wrapped,d->font_green,127,33,w->x+36,w->y+234,
            (w->width_blocks-6)*16,*u->font_green,0,w->x,w->y);
    for(int32_t i=0;i<*u->food_count;++i)
        if(u->foods[i]!=0)house_good(c,u->foods[i],i*110,w->y+110);
    static const int32_t goods[4]={13,19,17,15};
    for(int32_t i=0;i<4;++i)house_good(c,goods[i],i*110,w->y+130);
    const int32_t sickness=building(a)->render_warning_087;
    if(sickness==1||sickness==2) {
        *u->text_width=0;description(a,127,sickness==1?107:109,w->y+76);
        pharaoh_info_text_draw_amount(d->text,d->font_black,d->number_suffix,8,4,
            building(a)->native_f0,w->x+32+*u->text_width,w->y+76,*u->font_black,0);
        return;
    }
    const int32_t evolve=c->houses[w->building_id].evolve_text;
    if(evolve==62)house_worst(c);
    else if(evolve==68||evolve==72)house_evolve_goods(c,0);
    else if(evolve==70||evolve==74)house_evolve_goods(c,1);
    else description(a,127,evolve+40,w->y+36);
}
