#include "pharaoh/ui/building_info_live.h"
#include "pharaoh/core/integer_clamp.h"
#include "pharaoh/core/percentage_adjust.h"
#include "pharaoh/core/percentage_scale.h"
#include "pharaoh/core/static_objects.h"
#include <stddef.h>

_Static_assert(sizeof(PharaohBuildingInfoLiveState)==224,"native info state");
_Static_assert(offsetof(PharaohBuildingInfoLiveState,advisor_buttons)==48,"advisor DWORDs");
_Static_assert(offsetof(PharaohBuildingInfoLiveState,worst_desirability_score)==32,"native score EADCD8");
_Static_assert(offsetof(PharaohBuildingInfoLiveState,selected_walker_index)==44,"native selected walker EADCE4");
_Static_assert(offsetof(PharaohBuildingInfoLiveState,walker_ids)==108,"walker DWORDs");
_Static_assert(offsetof(PharaohBuildingInfoLiveState,mode)==152,"mode DWORD");
_Static_assert(offsetof(PharaohBuildingInfoLiveState,building_id)==180,"building DWORD");
_Static_assert(offsetof(PharaohBuildingInfoLiveState,first_draw)==220,"first draw DWORD");
_Static_assert(offsetof(PharaohBuildingTypeRecord,native_e0)==0xe0,"order scroll DWORD");
_Static_assert(offsetof(PharaohWalkerQueryRecord,formation_id_60)==0x60,"formation WORD");
_Static_assert(offsetof(PharaohNativeLegionRecord,deployment_status)==0x82,"request WORD");
_Static_assert(offsetof(PharaohBuildingModelRecord,laborers)==0x14,"model labor DWORD");

static int32_t signed_bits(uint32_t x)
{ return x<=INT32_MAX?(int32_t)x:(int32_t)(x&INT32_MAX)-INT32_MAX-1; }
static int32_t add32(int32_t a,int32_t b)
{ return signed_bits((uint32_t)a+(uint32_t)b); }
static int32_t mul32(int32_t a,uint32_t b)
{ return signed_bits((uint32_t)a*b); }
static PharaohBuildingTypeRecord *info_building(const PharaohBuildingInfoLiveContext *c)
{ return &c->buildings[c->window->building_id]; }
static int32_t visible_rows(const PharaohBuildingInfoLiveContext *c)
{ return pharaoh_storage_orders_visible_rows(*c->display_mode); }
static int32_t shown_rows(const PharaohBuildingInfoLiveContext *c)
{ int32_t n=visible_rows(c);return n<*c->resource_count?visible_rows(c):*c->resource_count; }
static int32_t panel_blocks(int32_t rows)
{ return pharaoh_int32_divide_round_up(mul32(rows,4),3); }
static int32_t panel_bottom(int32_t rows)
{ return mul32(add32(panel_blocks(rows),7),16); }
static int granary(int32_t type) { return type==71||type==191; }
static int bazaar(int32_t type) { return type==70||type==190; }
static int storage_yard(int32_t type) { return type==72||type==193; }
static int32_t bazaar_rows(const PharaohBuildingInfoLiveContext *c)
{
    int32_t rows=0;
    for(int32_t i=0;i<*c->resource_count;++i)
        if(pharaoh_native_type_is_supported_005013a0((uint32_t)c->resources[i]))++rows;
    return rows;
}
static const PharaohBuildingStorageRecord *info_storage(const PharaohBuildingInfoLiveContext *c)
{ return &c->storage[info_building(c)->storage_id]; }
static void label(PharaohBuildingInfoLiveContext *c,int32_t group,int32_t entry,
    int32_t x,int32_t y,int32_t font)
{
    c->operations->text(c->operations->user_data,group,entry,x,y,font,0,
        c->window->x,c->window->y);
}
static void border(PharaohBuildingInfoLiveContext *c,int32_t x,int32_t y,
    int32_t width,int32_t height,int32_t focused)
{
    c->operations->border(c->operations->user_data,0,x,y,width,height,focused,
        c->window->x,c->window->y);
}
static void centered(PharaohBuildingInfoLiveContext *c,int32_t group,int32_t entry,
    int32_t x,int32_t y,int32_t width,int32_t font)
{
    c->operations->centered(c->operations->user_data,group,entry,x,y,width,font,0,
        c->window->x,c->window->y);
}
static void resource_image(PharaohBuildingInfoLiveContext *c,int32_t resource,
    int32_t x,int32_t y)
{
    c->operations->image(c->operations->user_data,
        add32(*c->resource_images,resource),x,y);
}
static void orders_background(PharaohBuildingInfoLiveContext *c,int32_t help,
    int32_t group,int32_t entry,int32_t rows)
{
    const int32_t blocks=panel_blocks(rows);
    c->window->help_id=help;
    c->operations->outer_panel(c->operations->user_data,c->window->x,32,29,blocks+7);
    centered(c,group,entry,c->window->x,42,c->window->width_blocks*16,*c->font_title);
    c->operations->inner_panel(c->operations->user_data,c->window->x+16,74,
        c->window->width_blocks-2,blocks,c->window->x,c->window->y);
}

/* Complete5013E0/501AD0/5028F0. */
void pharaoh_building_info_live_draw_bazaar_orders(PharaohBuildingInfoLiveContext *c)
{ orders_background(c,2,97,7,bazaar_rows(c)); }
void pharaoh_building_info_live_draw_granary_orders(PharaohBuildingInfoLiveContext *c)
{ orders_background(c,3,98,6,*c->food_count); }
void pharaoh_building_info_live_draw_storage_orders(PharaohBuildingInfoLiveContext *c)
{ orders_background(c,4,99,3,shown_rows(c)); }

/* Complete406300. Native UI publishes one-based ordinals for enabled rows.
 * The availability producer clears36 resource slots; keep that whole scan. */
void pharaoh_building_info_live_toggle_resource(PharaohBuildingInfoLiveContext *c,
    uint32_t ordinal)
{
    PharaohBuildingTypeRecord *b=info_building(c);
    const uint8_t owner=b->storage_id;
    int32_t resource;
    if(storage_yard(b->building_type)) {
        if(b->native_e0==0)b->native_e0=1;
        else if(b->native_e0>14)b->native_e0=add32(*c->resource_count,-13);
        resource=c->resources[(uint32_t)b->native_e0+ordinal-2u];
    } else if(bazaar(b->building_type)) {
        int32_t filtered[36],count=0;
        for(int32_t i=0;i<36;++i)
            if(pharaoh_native_type_is_supported_005013a0((uint32_t)c->resources[i]))
                filtered[count++]=c->resources[i];
        resource=filtered[ordinal-1u];
    } else resource=c->foods[ordinal-1u];
    uint8_t *policy=&c->storage[owner].resource_state[resource];
    switch(*policy) {
    case 0:*policy=bazaar(info_building(c)->building_type)?1:2;break;
    case 1:*policy=0;break;
    case 2:*policy=3;break;
    case 3:*policy=1;break;
    default:break;
    }
    *c->redraw_requested=1;
}

static void accept_none(PharaohBuildingInfoLiveContext *c,int32_t y)
{
    const int32_t width=(c->window->width_blocks-10)*16;
    border(c,c->window->x+80,y,width,20,*c->focus_primary==1);
    centered(c,99,7,c->window->x+80,y+4,width,*c->font_black);
}
static void row_name(PharaohBuildingInfoLiveContext *c,int32_t resource,
    int32_t icon_x,int32_t text_x,int32_t y)
{
    resource_image(c,resource,c->window->x+icon_x,y-5);
    label(c,23,resource,c->window->x+text_x,y-1,*c->font_white);
}
static void row_focus(PharaohBuildingInfoLiveContext *c,int32_t row,
    int32_t width,int32_t y)
{ if(*c->focus_secondary==row+1)border(c,c->window->x+180,y-5,width,20,1); }
static int32_t amount_text(int32_t limit,int32_t all_text)
{ return limit<=800?25:limit<=1600?26:limit<=2400?27:all_text; }
static void order_limit(PharaohBuildingInfoLiveContext *c,int32_t first,
    int32_t limit,int32_t all_text,int32_t storage_text,int32_t x,int32_t y)
{
    *c->text_width=0;
    label(c,99,first,c->window->x+x,y,*c->font_white);
    label(c,99,amount_text(limit,all_text),*c->text_width+c->window->x+x,y,*c->font_white);
    if(limit<=2400)
        label(c,99,storage_text,*c->text_width+c->window->x+x,y,*c->font_white);
}
static void order_state(PharaohBuildingInfoLiveContext *c,
    const PharaohBuildingStorageRecord *s,int32_t resource,int32_t x,int32_t y,
    int32_t storage_text)
{
    switch(s->resource_state[resource]) {
    case 0:order_limit(c,s->accept_amount[resource]<=2400?32:18,
        s->accept_amount[resource],28,storage_text,x,y);break;
    case 1:label(c,99,8,c->window->x+x,y,*c->font_red);break;
    case 2:order_limit(c,19,s->get_amount[resource],31,storage_text,x,y);break;
    case 3:label(c,99,storage_text==30?20:21,c->window->x+x,y,*c->font_red);break;
    default:break;
    }
}
static void image_tooltip(PharaohBuildingInfoLiveContext *c,int32_t x,int32_t y,
    const PharaohImageButtonRecord *buttons,int32_t count,int32_t first)
{
    pharaoh_image_buttons_set_tooltip(x,y,buttons,count,first,
        c->image_input->cursor_x,c->image_input->cursor_y,c->image_focus);
}
static int32_t image_input(PharaohBuildingInfoLiveContext *c,int32_t x,int32_t y,
    PharaohImageButtonRecord *buttons,int32_t count,int16_t trailing)
{ return pharaoh_image_buttons_handle_mouse(x,y,buttons,count,trailing,c->image_input); }
static void order_buttons(PharaohBuildingInfoLiveContext *c,
    const PharaohBuildingStorageRecord *s,int32_t resource,int32_t row,int32_t y)
{
    if(s->resource_state[resource]==0||s->resource_state[resource]==2) {
        PharaohImageButtonRecord *buttons=&c->limit_buttons[row*2];
        int16_t trailing=row==29?c->limit_trailing_trigger:buttons[2].trigger;
        image_tooltip(c,c->window->x+200,y-3,buttons,2,10);
        image_input(c,c->window->x+200,y-3,buttons,2,trailing);
        c->operations->image_draw(c->operations->user_data,c->window->x+200,y-3,buttons,2);
    }
}

/* Complete5011D0.47D350 is the proven native zero leaf; icon additions vanish. */
void pharaoh_building_info_live_draw_bazaar_orders_foreground(PharaohBuildingInfoLiveContext *c)
{
    const PharaohBuildingStorageRecord *s=info_storage(c);
    int32_t row=1,y=78;
    for(int32_t i=0;i<*c->resource_count;++i) {
        const int32_t resource=c->resources[i];
        if(resource==0||!pharaoh_native_type_is_supported_005013a0((uint32_t)resource))continue;
        resource_image(c,resource,c->window->x+32,y);
        resource_image(c,resource,c->window->x+408,y);
        label(c,23,resource,c->window->x+72,y+4,*c->font_white);
        if(*c->focus_secondary==row)border(c,c->window->x+180,y,210,20,1);
        if(s->resource_state[resource]==0)
            label(c,97,8,c->window->x+260,y+5,*c->font_white);
        else if(s->resource_state[resource]==1)
            label(c,97,9,c->window->x+240,y+5,*c->font_red);
        ++row;y+=20;
    }
    *c->redraw_requested=1;
}
/* Complete501B60. */
void pharaoh_building_info_live_draw_granary_orders_foreground(PharaohBuildingInfoLiveContext *c)
{
    const PharaohBuildingStorageRecord *s=info_storage(c);
    accept_none(c,panel_bottom(*c->food_count)-8);
    int32_t y=83;
    for(int32_t row=0;row<*c->food_count;++row,y+=20) {
        const int32_t resource=c->foods[row];
        row_name(c,resource,32,72,y);row_focus(c,row,250,y);
        order_state(c,s,resource,250,y,30);order_buttons(c,s,resource,row,y);
    }
    *c->redraw_requested=1;
}

static void scroll_arrows(PharaohBuildingInfoLiveContext *c,int32_t blocks)
{
    const int32_t bottom=blocks*16,x=c->window->x+414;
    PharaohSquareButtonInputState *input=c->square_input;
    pharaoh_square_buttons_get_focus(x,bottom-78,c->scroll_down,1,10,
        input->cursor_x,input->cursor_y,c->image_focus);
    pharaoh_square_buttons_handle_mouse(x,bottom-78,c->scroll_down,1,input);
    c->operations->arrow_draw(c->operations->user_data,x,bottom-72,c->scroll_down,1);
    pharaoh_square_buttons_get_focus(c->window->x+390,82,c->scroll_up,1,10,
        input->cursor_x,input->cursor_y,c->image_focus);
    pharaoh_square_buttons_handle_mouse(c->window->x+390,82,c->scroll_up,1,input);
    c->operations->arrow_draw(c->operations->user_data,c->window->x+390,82,c->scroll_up,1);
    c->operations->inner_panel(c->operations->user_data,c->window->x+411,106,2,
        blocks==27?16:24,c->window->x,c->window->y);
}
/* Complete502840. */
void pharaoh_building_info_live_draw_storage_scroll_thumb(PharaohBuildingInfoLiveContext *c)
{
    const int32_t hidden=*c->resource_count-visible_rows(c);
    if(hidden<=0)return;
    const int32_t track=(panel_blocks(visible_rows(c))-6)*16;
    const int32_t first=info_building(c)->native_e0;
    int32_t percentage=0;
    if(first>1) {
        if(*c->resource_count<visible_rows(c)+first)percentage=100;
        else percentage=pharaoh_scale_value_to_percent(first-1,hidden).value;
    }
    c->operations->image(c->operations->user_data,*c->panel_button_images+39,
        c->window->x+414,pharaoh_adjust_by_percentage(track,percentage)+106);
}
/* Complete502990. Native zero resource slots do not advance the list cursor. */
void pharaoh_building_info_live_draw_storage_orders_foreground(PharaohBuildingInfoLiveContext *c)
{
    const PharaohBuildingStorageRecord *s=info_storage(c);
    const int32_t blocks=panel_blocks(shown_rows(c))+7;
    accept_none(c,blocks*16);
    int32_t first=info_building(c)->native_e0;
    if(first==0)first=1;
    if(*c->resource_count<=first)
        first=visible_rows(c)+1<*c->resource_count?*c->resource_count-visible_rows(c)-1:1;
    if(visible_rows(c)<*c->resource_count)scroll_arrows(c,blocks);
    int32_t index=first-1,y=83;
    for(int32_t row=0;row<visible_rows(c);++row,y+=20) {
        const int32_t resource=c->resources[index];
        if(resource==0)continue;
        row_name(c,resource,25,60,y);row_focus(c,row,230,y);
        order_state(c,s,resource,240,y,29);order_buttons(c,s,resource,row,y);
        ++first;++index;if(*c->resource_count<first)break;
    }
    *c->redraw_requested=1;
    pharaoh_building_info_live_draw_storage_scroll_thumb(c);
}

/* Complete4F8960; the farm predicate's consumed result is its low BYTE. */
int32_t pharaoh_building_info_live_height_class(const PharaohBuildingInfoLiveInitContext *c)
{
    PharaohBuildingInfoLiveContext *ui=c->ui;
    if(ui->window->mode==1) {
        switch(ui->window->terrain_kind) {
        case 12:case 9:case 23:case 24:case 8:return 1;
        case 7:return 4;
        default:return 0;
        }
    }
    if(ui->window->mode!=2)return 0;
    int32_t type=info_building(ui)->building_type;
    if((type>=140&&type<=149)||type==60)return 1;
    if((uint8_t)pharaoh_building_is_farm(ui->buildings,ui->window->building_id))return 5;
    type=info_building(ui)->building_type;
    switch(type) {
    case 0x3d:case 0x3e:case 0x3f:case 0x40:case 0x41:case 0x42:case 0x43:
    case 0x44:case 0x45:case 0x29:case 0x2a:case 0x2b:case 0x22:case 0x23:
    case 0x24:case 0x25:case 0xd1:case 0x2e:case 0x2f:case 0xce:case 0x31:
    case 0x62:case 0x63:case 0x5a:case 0xb4:case 0x58:case 0x59:case 0x5d:
    case 0x50:case 0x37:case 0x51:case 0x33:case 0x35:case 0x3a:case 0xaa:
    case 0xab:case 0xc8:case 0xc9:case 0xca:case 0x3b:case 0xac:case 0x39:
    case 0x5e:case 0xb9:case 0xba:case 0x5f:case 0x46:case 0xbe:case 0x4a:
    case 0x4b:case 0xc0:case 0x4c:case 0xb5:case 0xb6:case 0x4d:case 0x4e:
    case 0x4f:case 0xbb:case 0xbc:case 0xbd:case 0x56:case 0x57:case 0x6e:
    case 0x6f:case 0x70:case 0x71:case 0x72:case 0xcb:case 0xcc:case 0xe8:
    case 0xe9:case 0xcd:case 0xb1:case 0xb2:case 0xb3:case 0xe7:return 1;
    case 0x1f:case 0x20:case 0xe2:case 0x21:case 0x47:case 0xbf:case 0x54:
    case 0x55:return 2;
    case 0x1e:return 3;
    case 0x5c:return 4;
    default:return 0;
    }
}

static int32_t terrain_kind(const PharaohBuildingInfoLiveInitContext *c,int32_t grid)
{
    const uint32_t t=c->terrain[grid];
    if(c->building_ids[grid]==0&&c->sprites[grid]!=0)return t&4u?11:10;
    if(c->bitfields[grid]&0x80u) {
        if(t&0x02000000u)return 22;
        if(t&2u)return 5;
        if(t&0x40u)return 13;
        return -1;
    }
    if(t&1u)return 1;
    if((t&2u)&&info_building(c->ui)->building_type!=228) {
        if(grid==*c->entry_flag)return 14;
        if(grid==*c->exit_flag)return 15;
        const uint32_t rocks=t&0x20300002u;
        if(rocks==0x00100002u)return 16;
        if(rocks==2u)return 17;
        if(rocks==0x20000002u)return 26;
        return 2;
    }
    const uint32_t walls=t&0x00c04000u;
    const int building=(t&8u)!=0;
    if((t&4u)&&!(t&0x10008u))return 3;
    if(t&0x10u)return 4;
    if(t&0x20u)return 12;
    if((t&0x40u)&&!building)return 6;
    if(t&0x100u)return 7;
    if(t&0x1000u)return 8;
    if(t&0x40000u)return 21;
    if(t&0x02000000u)return 22;
    if(walls==0x4000u)return 9;
    if(walls==0x404000u)return 23;
    if(walls==0x804000u)return 24;
    if((t&0x10004u)==0x10000u&&!building)return 19;
    if((t&0x800u)&&!building)return 25;
    if((t&0x10000u)&&!building)return 20;
    return c->ui->window->building_id==0?10:-2;
}
static void use_building_position(PharaohBuildingInfoLiveContext *c)
{
    const PharaohBuildingTypeRecord *b=info_building(c);
    c->window->grid_offset=b->grid_offset;c->window->tile_x=b->x;c->window->tile_y=b->y;
}
static void progress_report(const PharaohBuildingInfoLiveInitContext *c)
{
    const int32_t building=c->ui->window->building_id;
    int32_t index;
    switch(info_building(c->ui)->building_type) {
    case 183:index=0;break;case 213:index=1;break;case 225:index=2;break;
    case 230:index=3;break;case 227:index=4;break;case 228:index=5;break;
    case 222:index=6;break;case 229:index=7;break;case 234:index=8;break;
    case 235:index=9;break;case 236:index=10;break;default:return;
    }
    c->operations->progress[index](c->operations->user_data,building);
}
static int needs_main(int32_t type)
{
    switch(type) {
    case 73:case 32:case 226:case 210:case 31:case 30:case 33:return 1;
    default:return 0;
    }
}
static int temple_complex(int32_t type)
{ return (type>=65&&type<=69)||(type>=156&&type<=160); }
static void building_details(const PharaohBuildingInfoLiveInitContext *c)
{
    PharaohBuildingInfoLiveContext *ui=c->ui;
    const PharaohBuildingInfoLiveInitOperations *ops=c->operations;
    PharaohBuildingTypeRecord *b=info_building(ui);
    int32_t type=b->building_type;
    ui->window->mode=2;
    ui->window->details.worker_percentage=ops->percentage(ops->user_data,
        b->num_workers,c->models[type].laborers);
    b=info_building(ui);type=b->building_type;
    if(type==54) {
        ui->window->building_id=b->runtime_value_038;use_building_position(ui);
        ui->window->formation_id=info_building(ui)->formation_id;
    } else if(type==57)ui->window->formation_id=b->formation_id;
    else if(needs_main(type)) {
        ui->window->building_id=ops->building_main(ops->user_data,ui->window->building_id);
        use_building_position(ui);
    } else if(b->house_size) {
        ops->house_worst_desirability(ops->user_data);
        ops->house_evolve_text(ops->user_data,ui->window->building_id);
    } else if(type==95) {
        ui->window->has_academy=add32(ops->any_legion_with_soldiers(ops->user_data),
            *c->sentry_request);
    } else if(temple_complex(type)) {
        ui->window->building_id=ops->building_main(ops->user_data,ui->window->building_id);
        use_building_position(ui);
    }
    ui->window->details.road_access=0;b=info_building(ui);type=b->building_type;
    if(storage_yard(type)) {
        if(ops->road(ops->user_data,b->x,b->y,3))ui->window->details.road_access=1;
        ui->window->storage_state=pharaoh_storageyard_get_space_info(ui->buildings,ui->window->building_id);
        if(visible_rows(ui)>=*ui->resource_count)info_building(ui)->native_e0=1;
    } else {
        int32_t access;
        if(granary(type))access=ops->granary_road(ops->user_data,b->x,b->y);
        else if(type>=140&&type<=149)access=ops->nearby_road(ops->user_data,b->x,b->y);
        else if(type>=65&&type<=69) {
            /* Native byte at+AA is the temple-complex orientation overlay. */
            access=ops->temple_road(ops->user_data,b->x,b->y,((const uint8_t *)b)[0xaa]);
        } else access=ops->road(ops->user_data,b->x,b->y,b->size);
        if(access)ui->window->details.road_access=1;
    }
}
static int selectable_walker(const PharaohBuildingInfoLiveContext *c,
    const PharaohWalkerQueryRecord *w)
{
    if(w->state==2||w->action_state==4)return 0;
    switch(w->type) {
    case 0:case 6:case 36:case 37:case 59:case 60:case 61:case 62:case 63:
    case 65:case 71:case 72:case 74:return 0;
    case 11:case 12:case 13:case 14:
        return c->formations[w->formation_id_60].deployment_status<0;
    case 77:case 78:return w->request_event_id<0&&w->action_state!=17;
    default:return 1;
    }
}
static void collect_walkers(const PharaohBuildingInfoLiveInitContext *c,int32_t grid)
{
    PharaohBuildingInfoLiveContext *ui=c->ui;
    int32_t count=0;
    ui->window->selected_walker_index=0;ui->window->walker_count=0;
    for(int32_t i=0;i<7;++i)ui->window->walker_ids[i]=0;
    for(int32_t neighbour=0;neighbour<9;++neighbour) {
        int32_t id=c->walker_ids[add32(grid,c->neighbours[neighbour])];
        while(id>0) {
            const PharaohWalkerQueryRecord *w=&ui->walkers[id];
            if(selectable_walker(ui,w)) {
                ui->window->walker_ids[count]=id;ui->window->walker_count=++count;
                c->operations->walker_message(c->operations->user_data,id);
                count=ui->window->walker_count;
            }
            id=w->next_walker_id_on_same_tile;if(count>=7)return;
        }
    }
}
static void select_military(PharaohBuildingInfoLiveContext *c)
{
    for(int32_t i=0;i<7;++i) {
        const int32_t id=c->window->walker_ids[i];
        if(id<=0)continue;
        const uint8_t type=c->walkers[id].type;
        if(type>=11&&type<=14) {
            c->window->mode=4;c->window->formation_id=c->walkers[id].formation_id_60;return;
        }
        if(type==77){c->window->mode=6;c->window->transport_id=id;return;}
        if(type==78){c->window->mode=5;c->window->warship_id=id;return;}
    }
}
static void position_window(const PharaohBuildingInfoLiveInitContext *c)
{
    PharaohBuildingInfoLiveState *w=c->ui->window;
    w->x=8;w->y=32;w->width_blocks=29;
    switch(pharaoh_building_info_live_height_class(c)) {
    case 1:w->height_blocks=16;break;case 2:w->height_blocks=18;break;
    case 3:w->height_blocks=19;break;case 4:w->height_blocks=14;break;
    case 5:w->height_blocks=24;break;default:w->height_blocks=22;break;
    }
    if(*c->display_height==480) {
        w->y=32;w->x=*c->panel_position_mode==1?68:8;
    } else if(*c->display_height==600) {
        w->y=*c->mouse_y<=312?mul32(add32(37,-w->height_blocks),16):32;
        w->x=*c->panel_position_mode==1?128:68;
    } else if(*c->display_height==768) {
        w->y=*c->mouse_y<=396?add32(760,-mul32(w->height_blocks,16)):32;
        w->x=*c->panel_position_mode==1?248:188;
    }
}

/* Complete4F7E50. The initial native PUSH ECX is overwritten scratch.
 * Whole prior InfoInit57 algorithm, with actual saved pool views retained. */
int32_t pharaoh_building_info_live_initialize(const PharaohBuildingInfoLiveInitContext *c)
{
    PharaohBuildingInfoLiveContext *ui=c->ui;
    const PharaohBuildingInfoLiveInitOperations *ops=c->operations;
    if(*c->editor_active==1)return 0;
    if(*c->pending_warning_count>0){ops->reset_warning(ops->user_data);return 0;}
    ui->window->tile_y=*c->cursor_tile_y;ui->window->tile_x=*c->cursor_tile_x;
    *ui->focus_secondary=0;*ui->focus_primary=0;*ui->focus_building=0;
    ui->window->first_draw=1;ui->window->orders_shown=0;
    *ui->current_window=9;*ui->redraw_requested=1;
    const int32_t grid=add32(add32(*c->cursor_tile_x,mul32(*c->cursor_tile_y,228)),*c->grid_start);
    ui->window->building_id=c->building_ids[grid];
    ui->window->details.rubble_type=c->rubble_types[grid];
    const int16_t type=info_building(ui)->building_type;ui->window->grid_offset=grid;
    if(type==218||type==219||type==220)
        ui->window->building_id=ops->pyramid_from_complex(ops->user_data,ui->window->building_id);
    if(info_building(ui)->building_type==215)
        ui->window->building_id=pharaoh_building_subtype(ui->buildings,ui->window->building_id);
    ui->window->details.canal=c->canals[grid];ui->window->details.groundwater=(int32_t)(c->terrain[grid]&0x80u);
    if(ui->window->building_id&&info_building(ui)->state==0){*ui->current_window=1;return 0;}
    ops->rebuild_resources(ops->user_data);progress_report(c);
    ui->window->mode=1;ui->window->selection=0;
    const int32_t kind=terrain_kind(c,grid);
    if(kind==-2)building_details(c);
    else if(kind!=-1)ui->window->terrain_kind=kind;
    collect_walkers(c,grid);select_military(ui);position_window(c);
    if(ui->window->mode==2)ops->advisor_buttons(ops->user_data,ui->window->advisor_buttons);
    return 1;
}

static int32_t bottom_buttons(const PharaohBuildingInfoLiveContext *c)
{ return c->window->height_blocks*16-40+c->window->y; }
static int32_t orders_button_y(const PharaohBuildingInfoLiveContext *c)
{ return c->window->height_blocks*16-34+c->window->y; }
static void row_position(const PharaohBuildingInfoLiveContext *c,
    enum PharaohBuildingInfoLiveRow row,int32_t fixed_y,int32_t *x,int32_t *y,int32_t *count)
{
    *x=c->window->x;*y=c->window->y;
    switch(row) {
    case PHARAOH_INFO_ROW_POLICIES:*x+=250;*y=78;break;
    case PHARAOH_INFO_ROW_BACK:case PHARAOH_INFO_ROW_STORAGE_BACK:*x+=80;*y=fixed_y;break;
    case PHARAOH_INFO_ROW_HOUSE:*x+=112;*y+=(c->window->height_blocks-2)*16;break;
    case PHARAOH_INFO_ROW_GRANARY:case PHARAOH_INFO_ROW_BAZAAR:case PHARAOH_INFO_ROW_STORAGE:
        *x+=100;*y=orders_button_y(c);break;
    case PHARAOH_INFO_ROW_LEGION_RETURN:
        *x=add32(*x,mul32(c->window->width_blocks-20,16)/2);*y=bottom_buttons(c);break;
    case PHARAOH_INFO_ROW_PHRASE:*x+=285;*y+=195;break;
    case PHARAOH_INFO_ROW_WALKERS:*count=c->window->walker_count;break;
    default:break;
    }
}
static int32_t generic_row(PharaohBuildingInfoLiveContext *c,
    enum PharaohBuildingInfoLiveRow row,int32_t fixed_y,int32_t count,int32_t *focus)
{
    PharaohBuildingInfoLiveButtonRow *buttons=&c->rows[row];
    int32_t x,y;
    row_position(c,row,fixed_y,&x,&y,&count);
    int32_t hit=pharaoh_generic_buttons_get_focus_from_buttons(x,y,buttons->buttons,count,
        c->generic_input->cursor_x,c->generic_input->cursor_y,c->generic_focus);
    row_position(c,row,fixed_y,&x,&y,&count);
    *focus=pharaoh_ui_layout_buttons(c->button_layout,c->button_layout_operations,
        x,y,buttons->buttons,count,hit);
    row_position(c,row,fixed_y,&x,&y,&count);
    c->generic_input->origin_x=x;c->generic_input->origin_y=y;
    return pharaoh_generic_buttons_handle_mouse(c->generic_input,buttons->buttons,count);
}
static int32_t help_close(PharaohBuildingInfoLiveContext *c,int32_t y)
{
    image_tooltip(c,c->window->x,y,c->help_close.buttons,2,10);
    return image_input(c,c->window->x,y,c->help_close.buttons,2,c->help_close.trailing_trigger);
}
static int32_t special_rows(const PharaohBuildingInfoLiveContext *c)
{
    const int32_t type=info_building(c)->building_type;
    return storage_yard(type)?shown_rows(c):bazaar(type)?bazaar_rows(c):*c->food_count;
}
static int32_t window_buttons(PharaohBuildingInfoLiveContext *c)
{
    if(c->window->orders_shown==0) {
        if(c->window->mode==2)c->operations->advisor_input(c->operations->user_data);
        return help_close(c,bottom_buttons(c));
    }
    const int32_t rows=special_rows(c);
    if(generic_row(c,PHARAOH_INFO_ROW_POLICIES,78,rows,c->focus_secondary))return 1;
    return help_close(c,panel_bottom(rows)-8);
}
static void right_click(PharaohBuildingInfoLiveContext *c)
{
    if(c->window->mode==4) {
        const PharaohNativeLegionRecord *f=&c->formations[c->window->formation_id];
        if(f->member_count) {
            if(f->strength_class==13&&f->movement_mode==0) {
                *c->current_window=73;*c->redraw_requested=1;return;
            }
            if(f->movement_mode!=6) {
                switch(f->movement_mode) {
                case 1:case 2:*c->button_parameter=0;break;
                case 3:case 4:*c->button_parameter=1;break;
                case 15:*c->button_parameter=2;break;
                default:break;
                }
                c->operations->legion_order(c->operations->user_data);
                *c->current_window=30;*c->redraw_requested=1;return;
            }
        }
    }
    *c->current_window=1;*c->redraw_requested=1;
}
static void walker_list_input(PharaohBuildingInfoLiveContext *c)
{
    if(generic_row(c,PHARAOH_INFO_ROW_WALKERS,0,c->window->walker_count,c->focus_primary))return;
    if(generic_row(c,PHARAOH_INFO_ROW_PHRASE,0,1,c->focus_secondary))return;
    const int32_t id=c->window->walker_ids[c->window->selected_walker_index];
    const uint8_t group=c->walkers[id].reserved_131;
    if(c->tutorial_flags[group]) {
        image_tooltip(c,c->window->x+40,bottom_buttons(c),c->tutorial_buttons[group].buttons,1,group+40);
        const uint8_t reloaded=c->walkers[id].reserved_131;
        image_input(c,c->window->x+40,bottom_buttons(c),c->tutorial_buttons[reloaded].buttons,
            1,c->tutorial_buttons[reloaded].trailing_trigger);
    }
}
static void building_buttons(PharaohBuildingInfoLiveContext *c)
{
    int32_t type=info_building(c)->building_type;
    if(type>=10&&type<=29)
        if(generic_row(c,PHARAOH_INFO_ROW_HOUSE,0,1,c->focus_secondary))return;
    if(granary(info_building(c)->building_type)) {
        int32_t handled;
        if(c->window->orders_shown==0)
            handled=generic_row(c,PHARAOH_INFO_ROW_GRANARY,0,1,c->focus_primary);
        else handled=generic_row(c,PHARAOH_INFO_ROW_BACK,panel_bottom(*c->food_count)-16,1,c->focus_primary);
        if(handled)return;
    }
    type=info_building(c)->building_type;
    if(bazaar(type)) {
        if(c->window->orders_shown)
            generic_row(c,PHARAOH_INFO_ROW_BACK,436,1,c->focus_primary);
        else generic_row(c,PHARAOH_INFO_ROW_BAZAAR,0,1,c->focus_primary);
    } else if(storage_yard(type)) {
        if(c->window->orders_shown==0) {
            generic_row(c,PHARAOH_INFO_ROW_STORAGE,0,1,c->focus_primary);return;
        }
        const int32_t rows=shown_rows(c);
        if(!c->operations->storage_scrollbar(c->operations->user_data))
            generic_row(c,PHARAOH_INFO_ROW_STORAGE_BACK,panel_bottom(rows)-24,1,c->focus_primary);
    } else if(type==209) {
        if(!generic_row(c,PHARAOH_INFO_ROW_FESTIVAL,0,1,c->focus_primary))
            c->operations->festival_input(c->operations->user_data);
    } else if(type==77||type==78||type==79)
        generic_row(c,PHARAOH_INFO_ROW_SALARY,0,1,c->focus_primary);
}

/* Whole4F96E0 including its inline window/building/walker button algorithms. */
void pharaoh_building_info_live_handle_input(PharaohBuildingInfoLiveContext *c)
{
    *c->focus_primary=0;
    if(*c->right_released){right_click(c);return;}
    if(window_buttons(c)||c->window->mode==0)return;
    if(c->window->mode==4) {
        if(generic_row(c,PHARAOH_INFO_ROW_LEGION,0,5,c->focus_primary))return;
        generic_row(c,PHARAOH_INFO_ROW_LEGION_RETURN,0,1,c->focus_secondary);return;
    }
    if(c->window->mode==6) {
        generic_row(c,PHARAOH_INFO_ROW_TRANSPORT,0,5,c->focus_primary);return;
    }
    if(c->window->mode==5) {
        generic_row(c,PHARAOH_INFO_ROW_WARSHIP,0,5,c->focus_primary);return;
    }
    if(c->window->selection){walker_list_input(c);return;}
    if(c->window->mode==2)building_buttons(c);
}

static int walker_speaks(uint8_t type)
{
    switch(type) {
    case 5:case 7:case 10:case 15:case 16:case 17:case 18:case 27:case 29:
    case 30:case 31:case 32:case 34:case 35:case 39:case 40:case 87:case 88:
    case 89:case 105:return 1;
    default:return 0;
    }
}
static void phrase_button(PharaohBuildingInfoLiveContext *c)
{
    if(!c->window->selection)return;
    const int32_t id=c->window->walker_ids[c->window->selected_walker_index];
    if(!walker_speaks(c->walkers[id].type))return;
    for(int32_t i=0;i<8;++i) {
        if(i!=c->window->walker_phrase&&c->phrase_counts[i]>1) {
            border(c,c->window->x+285,c->window->y+195,150,25,*c->focus_secondary==1);
            centered(c,18,7,c->window->x+285,c->window->y+200,150,*c->font_green);return;
        }
    }
}
static void ordinary_order_button(PharaohBuildingInfoLiveContext *c,int32_t type)
{
    const PharaohStorageOrderForegroundState state={c->window->x,c->window->y,
        c->window->height_blocks,c->window->width_blocks,*c->focus_primary,*c->font_black};
    const PharaohStorageOrderForegroundOperations ops={c->operations->user_data,
        c->operations->border,c->operations->centered};
    if(granary(type))pharaoh_building_info_draw_granary_foreground(&state,&ops);
    else if(bazaar(type))pharaoh_building_info_draw_bazaar_foreground(&state,&ops);
    else pharaoh_building_info_draw_storage_yard_foreground(&state,&ops);
}
/* Whole4F94A0. */
void pharaoh_building_info_live_draw_foreground(PharaohBuildingInfoLiveContext *c)
{
    const PharaohBuildingInfoLiveOperations *ops=c->operations;
    if(c->window->mode==1){phrase_button(c);return;}
    if(c->window->mode==4){ops->legion_foreground(ops->user_data,c->window->formation_id);return;}
    if(c->window->mode!=2)return;
    const int32_t type=info_building(c)->building_type;
    if(type>=10&&type<=29)return;
    if(granary(type)||bazaar(type)||storage_yard(type)) {
        if(!c->window->orders_shown){ordinary_order_button(c,type);return;}
        if(granary(type))pharaoh_building_info_live_draw_granary_orders_foreground(c);
        else if(bazaar(type))pharaoh_building_info_live_draw_bazaar_orders_foreground(c);
        else pharaoh_building_info_live_draw_storage_orders_foreground(c);
    } else if(type==187||type==188||type==189||type==86||type==87)
        ops->palace_foreground(ops->user_data);
    else if(type==77||type==78)ops->mansion_foreground(ops->user_data);
}

/* This catalogue is filled from the pinned native selector/table and verified
 * against the whole existing C catalogue; its non-default children are required.
 * Keep it separate from the relocated callable C pointers. */
static const uint8_t building_background_defined[237]={
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
static void draw_building(PharaohBuildingInfoLiveContext *c)
{
    const PharaohBuildingInfoLiveOperations *ops=c->operations;
    const int32_t type=info_building(c)->building_type;
    const int orders=c->window->orders_shown!=0;
    if((type>=60&&type<=69)||(type>=140&&type<=144)) {
        const int32_t index=type>=140?type-140:(type-60)%5;
        ops->temple(ops->user_data,index);return;
    }
    if(bazaar(type)) {
        if(orders)pharaoh_building_info_live_draw_bazaar_orders(c);else ops->bazaar(ops->user_data);
    } else if(granary(type)) {
        if(orders)pharaoh_building_info_live_draw_granary_orders(c);else ops->granary(ops->user_data);
    } else if(storage_yard(type)) {
        if(orders)pharaoh_building_info_live_draw_storage_orders(c);else ops->storage_yard(ops->user_data);
    } else if(type==107)ops->quarry(ops->user_data,25);
    else if(type==216)ops->quarry(ops->user_data,26);
    else if(type==221)ops->quarry(ops->user_data,30);
    else if((uint32_t)type<237u&&building_background_defined[type])
        ops->building_background[type](ops->user_data);
}
/* Whole4F8E60 plus its previously unlisted native tail-jump case blocks.
 * The transport/warship branches really read the shared formation field here. */
void pharaoh_building_info_live_draw_background(PharaohBuildingInfoLiveContext *c)
{
    const PharaohBuildingInfoLiveOperations *ops=c->operations;
    ops->city_background(ops->user_data);
    const PharaohBuildingTypeRecord *b=info_building(c);
    int32_t type=b->building_type;
    if(type==57) {
        if(b->subtype==13)type=40;
        if(b->subtype==11)type=44;
        if(b->subtype==12)type=45;
    }
    const int32_t channel=c->sound_channels[type*5];
    if(channel>-1&&c->window->first_draw&&*c->sound_enabled&&c->window->mode==2)
        ops->play_sample(ops->user_data,channel,*c->sound_volume,64,-1,1);
    switch(c->window->mode) {
    case 0:ops->no_info(ops->user_data);break;
    case 1:ops->terrain(ops->user_data);break;
    case 4:ops->legion(ops->user_data,c->window->formation_id);break;
    case 6:ops->transport(ops->user_data,c->window->formation_id);break;
    case 5:ops->warship(ops->user_data,c->window->formation_id);break;
    case 2:draw_building(c);break;
    default:break;
    }
}
