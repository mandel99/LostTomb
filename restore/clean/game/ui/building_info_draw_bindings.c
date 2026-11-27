#include "pharaoh/ui/building_info_draw_bindings.h"
#include <stddef.h>

/* Binder metadata from the already reconstructed4F8E60 catalogue. This does
 * not introduce another Native owner. A zero is the actual default branch;
 * a one requires a renderer and must never be treated as a default/no-op. */
static const uint8_t native_catalog_present[237]={
    [7]=1,[90]=1,[10]=1,[11]=1,[12]=1,[13]=1,[14]=1,[15]=1,[16]=1,[17]=1,
    [18]=1,[19]=1,[20]=1,[21]=1,[22]=1,[23]=1,[24]=1,[25]=1,[26]=1,[27]=1,
    [28]=1,[29]=1,[30]=1,[31]=1,[32]=1,[33]=1,[34]=1,[35]=1,[36]=1,[37]=1,
    [41]=1,[42]=1,[43]=1,[46]=1,[47]=1,[49]=1,[51]=1,[53]=1,[55]=1,[56]=1,
    [57]=1,[58]=1,[200]=1,[59]=1,[74]=1,[75]=1,[192]=1,[76]=1,[77]=1,[78]=1,
    [79]=1,[80]=1,[81]=1,[84]=1,[85]=1,[187]=1,[188]=1,[189]=1,[86]=1,[87]=1,
    [88]=1,[89]=1,[92]=1,[93]=1,[94]=1,[185]=1,[186]=1,[95]=1,[98]=1,[99]=1,
    [100]=1,[101]=1,[102]=1,[103]=1,[104]=1,[105]=1,[106]=1,[108]=1,[109]=1,
    [110]=1,[111]=1,[112]=1,[113]=1,[114]=1,[115]=1,[136]=1,[138]=1,[161]=1,
    [162]=1,[167]=1,[170]=1,[201]=1,[171]=1,[202]=1,[172]=1,[173]=1,[177]=1,
    [178]=1,[179]=1,[180]=1,[181]=1,[182]=1,[183]=1,[184]=1,[194]=1,[195]=1,
    [196]=1,[199]=1,[203]=1,[204]=1,[205]=1,[206]=1,[207]=1,[208]=1,[209]=1,
    [210]=1,[213]=1,[217]=1,[222]=1,[224]=1,[225]=1,[226]=1,[227]=1,[228]=1,
    [229]=1,[234]=1,[235]=1,[236]=1,[230]=1,[231]=1,[232]=1,[233]=1
};
static int storage_type(int32_t type)
{ return type==70||type==71||type==72||type==190||type==191||type==193; }
static int temple_type(int32_t type)
{ return (type>=60&&type<=69)||(type>=140&&type<=144); }
static int quarry_type(int32_t type)
{ return type==107||type==216||type==221; }

enum PharaohBuildingInfoDrawTypeSupport pharaoh_building_info_draw_type_support(int32_t type)
{
    if((uint32_t)type>=237)return PHARAOH_INFO_DRAW_OUTSIDE_TYPE_CATALOG;
    if((type>=10&&type<=29)||(type>=41&&type<=43)||type==56||type==99||storage_type(type))
        return PHARAOH_INFO_DRAW_CONNECTED_TYPE;
    if(native_catalog_present[type]||temple_type(type)||quarry_type(type))
        return PHARAOH_INFO_DRAW_MISSING_TYPE_PROVIDER;
    return PHARAOH_INFO_DRAW_NATIVE_DEFAULT;
}

static const PharaohStorageInfoDrawContext *drawing(void *data)
{ return ((PharaohBuildingInfoDrawBindings *)data)->providers.storage; }
/* LiveOperations' text consumers use exactly these five actual font IDs. */
static const PharaohBitmapFont *font(const PharaohStorageInfoDrawContext *d,int32_t id)
{
    if(id==*d->ui->font_black)return d->font_black;
    if(id==*d->ui->font_green)return d->font_green;
    if(id==*d->ui->font_white)return d->font_white;
    if(id==*d->ui->font_red)return d->font_red;
    if(id==*d->ui->font_title)return d->font_title;
    return NULL; /* outside the supported parent font domain */
}
static void border(void *p,int32_t style,int32_t x,int32_t y,int32_t w,int32_t h,
    int32_t flags,int32_t ox,int32_t oy)
{ pharaoh_info_layout_draw_border(drawing(p)->layout,style,x,y,w,h,flags,ox,oy); }
static void text(void *p,int32_t group,int32_t entry,int32_t x,int32_t y,
    int32_t id,int32_t color,int32_t ox,int32_t oy)
{
    const PharaohStorageInfoDrawContext *d=drawing(p);
    pharaoh_ui_text_draw_language(d->text,font(d,id),group,entry,x,y,id,(uint16_t)color,ox,oy);
}
static void centered(void *p,int32_t group,int32_t entry,int32_t x,int32_t y,
    int32_t w,int32_t id,int32_t color,int32_t ox,int32_t oy)
{
    const PharaohStorageInfoDrawContext *d=drawing(p);
    pharaoh_ui_text_draw_centered_language(d->text,font(d,id),group,entry,x,y,w,id,(uint16_t)color,ox,oy);
}
static void outer(void *p,int32_t x,int32_t y,int32_t w,int32_t h)
{ pharaoh_panel_frame_draw_native(drawing(p)->layout->outer_panel,x,y,w,h); }
static void inner(void *p,int32_t x,int32_t y,int32_t w,int32_t h,int32_t ox,int32_t oy)
{ pharaoh_info_layout_draw_inner_panel(drawing(p)->layout,x,y,w,h,ox,oy); }
static void image(void *p,int32_t id,int32_t x,int32_t y)
{ (void)pharaoh_image_draw(drawing(p)->layout->inner_panel->drawing,id,x,y); }
static void arrows(void *p,int32_t x,int32_t y,PharaohSquareButtonRecord *rows,int32_t n)
{
    PharaohBuildingInfoDrawBindings *b=p;
    pharaoh_info_square_buttons_draw(b->providers.square_buttons,x,y,rows,n);
}
static void buttons(void *p,int32_t x,int32_t y,PharaohImageButtonRecord *rows,int32_t n)
{
    PharaohBuildingInfoDrawBindings *b=p;
    PharaohImageButtonDrawState state=*b->providers.image_buttons;
    state.draw_sprite=image;state.user_data=b;
    pharaoh_image_buttons_draw(x,y,rows,n,&state);
}
static void city(void *p)
{ pharaoh_city_render_current_view(((PharaohBuildingInfoDrawBindings *)p)->providers.city); }
static void sample(void *p,int32_t file,int32_t volume,int32_t pan,int32_t owner,int32_t loops)
{
    const PharaohDigitalSoundDeviceContext *s=((PharaohBuildingInfoDrawBindings *)p)->providers.sample_sound;
    const PharaohDigitalSoundRequest request={owner,file,volume,pan,loops};
    (void)pharaoh_digital_sound_play_sample(s->state,s->operations,&request);
}
static void no_info(void *p)
{ pharaoh_info_draw_no_info(((PharaohBuildingInfoDrawBindings *)p)->providers.content); }
static void terrain(void *p)
{ pharaoh_info_draw_terrain(((PharaohBuildingInfoDrawBindings *)p)->providers.terrain); }
static void house(void *p)
{ pharaoh_info_draw_house(((PharaohBuildingInfoDrawBindings *)p)->providers.house); }
static void statue(void *p)
{ pharaoh_info_draw_statue(((PharaohBuildingInfoDrawBindings *)p)->providers.content); }
static void arch(void *p)
{ pharaoh_info_draw_triumphal_arch(((PharaohBuildingInfoDrawBindings *)p)->providers.content); }
static void burning(void *p)
{ pharaoh_info_draw_burning_ruin(((PharaohBuildingInfoDrawBindings *)p)->providers.content); }
static void bazaar(void *p) { pharaoh_info_draw_bazaar(drawing(p)); }
static void granary(void *p) { pharaoh_info_draw_granary(drawing(p)); }
static void storage(void *p) { pharaoh_info_draw_storage_yard(drawing(p)); }
static void legion(void *p,int32_t formation)
{
    PharaohBuildingInfoDrawBindings *b=p;
    b->arithmetic=pharaoh_unit_panel_draw_legion(b->providers.units,formation);
}
static void transport(void *p,int32_t formation)
{
    PharaohBuildingInfoDrawBindings *b=p;(void)formation;
    b->arithmetic=pharaoh_unit_panel_draw_transport(b->providers.units);
}
static void warship(void *p,int32_t formation)
{
    PharaohBuildingInfoDrawBindings *b=p;(void)formation;
    b->arithmetic=pharaoh_unit_panel_draw_warship(b->providers.units);
}
static void legion_foreground(void *p,int32_t formation)
{ pharaoh_unit_panel_draw_legion_foreground(((PharaohBuildingInfoDrawBindings *)p)->providers.units,formation); }

static void (*const connected_catalog[237])(void *)={
    [10]=house,[11]=house,[12]=house,[13]=house,[14]=house,[15]=house,[16]=house,
    [17]=house,[18]=house,[19]=house,[20]=house,[21]=house,[22]=house,[23]=house,
    [24]=house,[25]=house,[26]=house,[27]=house,[28]=house,[29]=house,
    [41]=statue,[42]=statue,[43]=statue,[56]=arch,[99]=burning
};

void pharaoh_building_info_draw_bindings_initialize(PharaohBuildingInfoDrawBindings *b,
    const PharaohBuildingInfoDrawProviders *providers)
{
    b->providers=*providers;b->arithmetic=PHARAOH_PERCENTAGE_SCALE_VALUE;
    b->operations=(PharaohBuildingInfoLiveOperations){0};
    PharaohBuildingInfoLiveOperations *o=&b->operations;o->user_data=b;
    if(providers->storage) {
        o->border=border;o->text=text;o->centered=centered;o->outer_panel=outer;
        o->inner_panel=inner;o->image=image;o->bazaar=bazaar;o->granary=granary;o->storage_yard=storage;
    }
    if(providers->square_buttons)o->arrow_draw=arrows;
    if(providers->image_buttons)o->image_draw=buttons;
    if(providers->city)o->city_background=city;
    if(providers->sample_sound)o->play_sample=sample;
    if(providers->content)o->no_info=no_info;
    if(providers->terrain)o->terrain=terrain;
    if(providers->units) {
        o->legion=legion;o->transport=transport;o->warship=warship;o->legion_foreground=legion_foreground;
    }
    o->building_background=connected_catalog;
    if(providers->ui)providers->ui->operations=o;
}

static enum PharaohBuildingInfoDrawSupport common(const PharaohBuildingInfoDrawBindings *b)
{
    if(!b||!b->providers.ui||!b->providers.ui->window||!b->providers.storage)
        return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    if(b->arithmetic!=PHARAOH_PERCENTAGE_SCALE_VALUE)return PHARAOH_INFO_DRAW_DIVIDE_FAULT;
    const PharaohBuildingInfoDrawProviders *p=&b->providers;
    const PharaohBuildingInfoLiveContext *u=p->ui;
    const PharaohStorageInfoDrawContext *d=p->storage;
    if(u->operations!=&b->operations||d->ui!=u)return PHARAOH_INFO_DRAW_STATE_MISMATCH;
    const PharaohBuildingInfoLiveOperations *o=&b->operations;
    if(!o->border||!o->text||!o->centered||!o->outer_panel||!o->inner_panel||
       !o->image||!o->arrow_draw||!o->image_draw)return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    if(!u->buildings||!u->walkers||!d->layout||!d->text||!d->text->drawing||!d->wrapped||
       !d->wrapped->drawing||!d->layout->inner_panel||!d->layout->outer_panel||
       !d->layout->border||!d->layout->layout||!d->layout->highlight||!d->layout->window_id||
       !d->layout->inner_panel->drawing||!p->square_buttons||!p->image_buttons||
       !u->font_black||!u->font_green||!u->font_white||!u->font_red||!u->font_title||
       !d->font_black||!d->font_green||!d->font_white||!d->font_red||!d->font_title)
        return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    if(!d->font_black->draw_glyph||!d->font_green->draw_glyph||!d->font_white->draw_glyph||
       !d->font_red->draw_glyph||!d->font_title->draw_glyph)return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    const PharaohImageDrawContext *images=d->layout->inner_panel->drawing;
    if(!images->clip||!images->state||!images->surface_pixels||!d->text->selection||
       !d->text->glyph_map||!d->text->number_text||!d->text->current_text||!d->text->center_offset||
       !d->text->reset_flag||!d->wrapped->selection||!d->wrapped->current_text_00e20768||
       !d->wrapped->drawing->word_metrics||!d->wrapped->drawing->line_buffer_00e3439c||
       !d->wrapped->drawing->maximum_width_00e2076c||!p->square_buttons->current_image||
       !p->square_buttons->draw_x||!p->square_buttons->draw_y||!p->square_buttons->ui_redraw||
       !p->image_buttons->sprite_group_bases||
       !p->image_buttons->draw_x||!p->image_buttons->draw_y||!p->image_buttons->active_sprite_image||
       !p->image_buttons->ui_redraw||!p->image_buttons->window_redraw_requested||!p->image_buttons->force_refresh)
        return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    if(d->layout->outer_panel->drawing!=images||d->layout->border->drawing!=images||
       p->square_buttons->drawing!=images||
       d->text->layout!=d->layout->layout||d->text->highlight!=d->layout->highlight||
       d->text->window_id!=d->layout->window_id||d->text->window_seven_layout_base!=d->layout->window_seven_layout_base||
       u->text_width!=&d->text->drawing->cursor_x||d->wrapped->drawing->draw_state!=d->text->drawing||
       p->image_buttons->draw_x!=&images->clip->draw_x||p->image_buttons->draw_y!=&images->clip->draw_y)
        return PHARAOH_INFO_DRAW_STATE_MISMATCH;
    return PHARAOH_INFO_DRAW_READY;
}
static int city_ready(const PharaohCityRenderContext *c)
{
    return pharaoh_city_render_bound_for_current_mode(c)!=0;
}
static enum PharaohBuildingInfoDrawSupport content_ready(const PharaohBuildingInfoDrawBindings *b)
{
    const PharaohInfoContentDrawContext *c=b->providers.content;
    if(!c)return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    return c->drawing==b->providers.storage?PHARAOH_INFO_DRAW_READY:PHARAOH_INFO_DRAW_STATE_MISMATCH;
}
static enum PharaohBuildingInfoDrawSupport walkers_ready(const PharaohBuildingInfoDrawBindings *b)
{
    enum PharaohBuildingInfoDrawSupport s=content_ready(b);if(s!=PHARAOH_INFO_DRAW_READY)return s;
    const PharaohBuildingInfoDrawProviders *p=&b->providers;
    const PharaohInfoWalkerContentContext *w=p->walkers;
    const PharaohInfoPreviewContext *v=p->content->previews;
    if(!w||!v||!w->portrait_codes||!w->portrait_scripts||!w->portrait_images||!w->font_name||
       !w->font_name_id||!w->supply||!w->traders||!w->classical_names||!w->image_buttons||
       !v->view||!v->view->state||!v->view->projection||!v->render||!v->surface_pitch||
       !v->preview_base||!v->preview_offsets)return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    if(!w->portrait_codes->portrait_codes||!w->portrait_codes->selected_scenario||
       !w->supply->cities||!w->supply->route_limits||!w->supply->players||!w->supply->current_player||
       !v->grid_origin||!v->panel_position_mode||!v->projected_x||!v->projected_y||!v->images||
       !v->image_count||!v->menu_bar_images||!v->capture_x||!v->capture_y||!v->draw_x||
       !v->draw_y||!v->draw_width||!v->draw_height)return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    const PharaohImageDrawContext *images=p->storage->layout->inner_panel->drawing;
    if(w->content!=p->content||v->ui!=p->ui||v->render!=p->city||v->view->state!=p->city->view||
       v->surface_pixels!=images->surface_pixels||*v->surface_pitch!=images->pitch_bytes||
       w->portrait_codes->walkers!=p->ui->walkers||w->portrait_codes->buildings!=p->ui->buildings||
       w->portrait_codes->scripts!=w->portrait_scripts||w->image_buttons!=p->image_buttons)
        return PHARAOH_INFO_DRAW_STATE_MISMATCH;
    return PHARAOH_INFO_DRAW_READY;
}
static enum PharaohBuildingInfoDrawSupport unit_ready(const PharaohBuildingInfoDrawBindings *b)
{
    const PharaohBuildingInfoDrawProviders *p=&b->providers;
    const PharaohUnitPanelLiveContext *u=p->units;
    if(!u||!u->pixels||!u->images||!u->pixel_format||!u->pixel_read_scratch||!u->formations)
        return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    if(!u->normal.id||!u->title.id||!u->heading.id||!u->description.id||
       !u->focus_primary||!u->focus_secondary||!u->transport_button_base||!u->warship_button_base||
       !u->experience_icon_base||!u->legion_badge_base||!u->legion_type_base||!u->legion_morale_base||
       !u->legion_experience_base||!u->charioteer_button_base||!u->archer_button_base||
       !u->infantry_button_base||!u->map_orientation||!u->native_7058b8)
        return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    const PharaohStorageInfoDrawContext *d=p->storage;
    if(u->window!=p->ui->window||u->buildings!=p->ui->buildings||u->walkers!=p->ui->walkers||
       u->formations!=p->ui->formations||u->layout!=d->layout||u->text!=d->text||u->wrapped!=d->wrapped||
       u->text_width!=p->ui->text_width||u->images!=d->layout->inner_panel->drawing||
       (const void *)u->pixels->rgb565_pixels!=u->images->surface_pixels||
       u->pixels->rgb565_pitch_bytes!=u->images->pitch_bytes||u->normal.font!=d->font_black||
       u->title.font!=d->font_title||u->heading.font!=d->font_white||u->description.font!=d->font_green||
       u->normal.id!=p->ui->font_black||u->title.id!=p->ui->font_title||
       u->heading.id!=p->ui->font_white||u->description.id!=p->ui->font_green)
        return PHARAOH_INFO_DRAW_STATE_MISMATCH;
    return PHARAOH_INFO_DRAW_READY;
}
static enum PharaohBuildingInfoDrawSupport building_ready(const PharaohBuildingInfoDrawBindings *b,int32_t type)
{
    const enum PharaohBuildingInfoDrawTypeSupport type_support=pharaoh_building_info_draw_type_support(type);
    if(type_support==PHARAOH_INFO_DRAW_MISSING_TYPE_PROVIDER||type_support==PHARAOH_INFO_DRAW_OUTSIDE_TYPE_CATALOG)
        return PHARAOH_INFO_DRAW_UNSUPPORTED_TYPE;
    if(type_support==PHARAOH_INFO_DRAW_NATIVE_DEFAULT)return PHARAOH_INFO_DRAW_READY;
    if(storage_type(type)) {
        const PharaohBuildingInfoLiveContext *u=b->providers.ui;
        const PharaohStorageInfoDrawContext *d=b->providers.storage;
        if(!u->storage||!u->focus_primary||!u->focus_secondary||!u->display_mode||!u->food_count||
           !u->foods||!u->resource_count||!u->resources||!d->models||!d->labor||!d->city_population||
           !d->kingdom_supplies_grain||!d->context_icons||!d->number_suffix||!d->employee_suffix)
            return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
        if(!u->window->orders_shown&&
           (((type==70||type==190)&&!b->operations.bazaar)||
            ((type==71||type==191)&&!b->operations.granary)||
            ((type==72||type==193)&&!b->operations.storage_yard)))return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
        return PHARAOH_INFO_DRAW_READY;
    }
    if(!b->operations.building_background||!b->operations.building_background[type])
        return PHARAOH_INFO_DRAW_UNSUPPORTED_TYPE;
    if(type==99)return content_ready(b);
    enum PharaohBuildingInfoDrawSupport s=walkers_ready(b);if(s!=PHARAOH_INFO_DRAW_READY)return s;
    if(type<10||type>29)return PHARAOH_INFO_DRAW_READY;
    const PharaohInfoHouseContentContext *h=b->providers.house;
    if(!h||!h->houses||!h->models||!h->roads||!h->trade||!h->list_or||!h->comma||!h->close_parenthesis)
        return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    if(h->walkers!=b->providers.walkers||(const void *)h->houses!=b->providers.ui->buildings)
        return PHARAOH_INFO_DRAW_STATE_MISMATCH;
    return PHARAOH_INFO_DRAW_READY;
}

enum PharaohBuildingInfoDrawSupport pharaoh_building_info_draw_background_support(
    const PharaohBuildingInfoDrawBindings *b)
{
    enum PharaohBuildingInfoDrawSupport s=common(b);if(s!=PHARAOH_INFO_DRAW_READY)return s;
    const PharaohBuildingInfoDrawProviders *p=&b->providers;
    if(!b->operations.city_background||!city_ready(p->city))return PHARAOH_INFO_DRAW_MISSING_CITY_PASS;
    const PharaohBuildingInfoLiveContext *u=p->ui;
    if(!u->sound_channels||!u->sound_enabled||!u->sound_volume)return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    const PharaohBuildingTypeRecord *selected=&u->buildings[u->window->building_id];
    int32_t sound_type=selected->building_type;
    if(sound_type==57) {
        if(selected->subtype==13)sound_type=40;
        if(selected->subtype==11)sound_type=44;
        if(selected->subtype==12)sound_type=45;
    }
    if((uint32_t)sound_type>=237)return PHARAOH_INFO_DRAW_UNSUPPORTED_TYPE;
    if(u->window->mode==2&&u->window->first_draw&&*u->sound_enabled&&u->sound_channels[sound_type*5]>-1&&
       (!b->operations.play_sample||!p->sample_sound||!p->sample_sound->state||!p->sample_sound->operations))
        return PHARAOH_INFO_DRAW_MISSING_SAMPLE_SOUND;
    switch(u->window->mode) {
    case 0:return b->operations.no_info?content_ready(b):PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    case 1: {
        s=walkers_ready(b);if(s!=PHARAOH_INFO_DRAW_READY)return s;
        if(!b->operations.terrain||!p->terrain||!p->terrain->selection_voice)
            return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
        const PharaohInfoWalkerSelectionVoiceContext *voice=p->terrain->selection_voice;
        if(!voice->sound||!voice->sound->operations||!voice->sound->device||
           !voice->voice||!voice->voice->stream||!voice->voice->voice_names||!voice->voice->special_voice_names||
           !voice->voice->stream->operations||!voice->voice->stream->device)
            return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
        if(p->terrain->selection_voice->ui!=p->ui||p->terrain->selection_voice->portrait!=p->walkers->portrait_codes)
            return PHARAOH_INFO_DRAW_STATE_MISMATCH;
        return p->terrain->walkers==p->walkers?PHARAOH_INFO_DRAW_READY:PHARAOH_INFO_DRAW_STATE_MISMATCH;
    }
    case 2:return building_ready(b,selected->building_type);
    case 4:return b->operations.legion?unit_ready(b):PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    case 5:return b->operations.warship?unit_ready(b):PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    case 6:return b->operations.transport?unit_ready(b):PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    default:return PHARAOH_INFO_DRAW_READY;
    }
}
enum PharaohBuildingInfoDrawSupport pharaoh_building_info_draw_foreground_support(
    const PharaohBuildingInfoDrawBindings *b)
{
    enum PharaohBuildingInfoDrawSupport s=common(b);if(s!=PHARAOH_INFO_DRAW_READY)return s;
    const PharaohBuildingInfoLiveContext *u=b->providers.ui;
    if(u->window->mode==4)return b->operations.legion_foreground?unit_ready(b):PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    if(u->window->mode==1&&(!u->phrase_counts||!u->focus_secondary))return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    if(u->window->mode!=2)return PHARAOH_INFO_DRAW_READY;
    const int32_t type=u->buildings[u->window->building_id].building_type;
    if(type==187||type==188||type==189||type==86||type==87||type==77||type==78)
        return PHARAOH_INFO_DRAW_UNSUPPORTED_TYPE;
    if(storage_type(type)&&(!u->focus_primary||!u->display_mode||!u->resource_count||!u->food_count))
        return PHARAOH_INFO_DRAW_MISSING_CONTEXT;
    return PHARAOH_INFO_DRAW_READY;
}
PharaohBuildingInfoDrawResult pharaoh_building_info_draw_bindings_background(PharaohBuildingInfoDrawBindings *b)
{
    const enum PharaohBuildingInfoDrawSupport s=pharaoh_building_info_draw_background_support(b);
    if(s==PHARAOH_INFO_DRAW_READY)pharaoh_building_info_live_draw_background(b->providers.ui);
    const PharaohPercentageScaleStatus a=b?b->arithmetic:PHARAOH_PERCENTAGE_SCALE_VALUE;
    const PharaohBuildingInfoDrawResult r={a==PHARAOH_PERCENTAGE_SCALE_VALUE?s:PHARAOH_INFO_DRAW_DIVIDE_FAULT,a};
    return r;
}
PharaohBuildingInfoDrawResult pharaoh_building_info_draw_bindings_foreground(PharaohBuildingInfoDrawBindings *b)
{
    const enum PharaohBuildingInfoDrawSupport s=pharaoh_building_info_draw_foreground_support(b);
    if(s==PHARAOH_INFO_DRAW_READY)pharaoh_building_info_live_draw_foreground(b->providers.ui);
    const PharaohPercentageScaleStatus a=b?b->arithmetic:PHARAOH_PERCENTAGE_SCALE_VALUE;
    const PharaohBuildingInfoDrawResult r={s,a};return r;
}
