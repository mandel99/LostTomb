#include "pharaoh/ui/building_info_draw_live.h"
#include "pharaoh/core/integer_clamp.h"
#include "pharaoh/core/percentage_adjust.h"
#include "pharaoh/core/percentage_scale.h"
#include "pharaoh/core/static_objects.h"
#include <stddef.h>

static int32_t signed_bits(uint32_t x)
{ return x<=INT32_MAX?(int32_t)x:(int32_t)(x&INT32_MAX)-INT32_MAX-1; }
static int32_t add32(int32_t a,int32_t b)
{ return signed_bits((uint32_t)a+(uint32_t)b); }
static int32_t sub32(int32_t a,int32_t b)
{ return signed_bits((uint32_t)a-(uint32_t)b); }
static int32_t mul32(int32_t a,uint32_t b)
{ return signed_bits((uint32_t)a*b); }

static PharaohUiLayoutNode *layout_node(const PharaohInfoLayoutDrawContext *c,
    int32_t kind,int32_t x,int32_t y,int32_t origin_x,int32_t origin_y,
    int32_t *relative_x,int32_t *relative_y)
{
    if(c->layout->state->initialized==0)pharaoh_ui_layout_initialize(c->layout);
    *relative_x=sub32(x,origin_x);*relative_y=sub32(y,origin_y);
    const uint8_t window=*c->window_id;
    const int32_t key=pharaoh_ui_layout_window_key(window,
        window==7?*c->window_seven_layout_base:0);
    return pharaoh_ui_layout_find(c->layout->state->entries,kind,key,*relative_x,*relative_y);
}

/* Whole4D63D0, using the real layout/highlight/border child algorithms. */
void pharaoh_info_layout_draw_border(const PharaohInfoLayoutDrawContext *c,
    int32_t style,int32_t x,int32_t y,int32_t width,int32_t height,
    int32_t flags,int32_t origin_x,int32_t origin_y)
{
    int32_t relative_x,relative_y;
    const PharaohUiLayoutNode *node=layout_node(c,3,x,y,origin_x,origin_y,&relative_x,&relative_y);
    if(node) {
        relative_x=node->values_08[0];relative_y=node->values_08[1];
        width=node->values_08[2];height=node->values_08[3];
    }
    pharaoh_ui_layout_highlight(c->highlight,3,x,y,width,height,
        relative_x,relative_y,origin_x,origin_y);
    pharaoh_button_border_draw(c->border,style,add32(relative_x,origin_x),
        add32(relative_y,origin_y),width,height,(uint32_t)flags);
}

/* Whole4D65B0. Marked dimensions are blocks, while the two edit borders
 * use pixels; the generic text/frame highlight helper cannot substitute. */
void pharaoh_info_layout_draw_inner_panel(const PharaohInfoLayoutDrawContext *c,
    int32_t x,int32_t y,int32_t width_blocks,int32_t height_blocks,
    int32_t origin_x,int32_t origin_y)
{
    int32_t key_x,key_y;
    const PharaohUiLayoutNode *node=layout_node(c,4,x,y,origin_x,origin_y,&key_x,&key_y);
    int32_t relative_x=key_x,relative_y=key_y;
    if(node) {
        relative_x=node->values_08[0];relative_y=node->values_08[1];
        width_blocks=node->values_08[2];height_blocks=node->values_08[3];
    }
    const int32_t screen_x=add32(relative_x,origin_x),screen_y=add32(relative_y,origin_y);
    PharaohLayoutHighlightState *highlight=c->highlight;
    if(highlight->edit_mode==4) {
        const int32_t width=mul32(width_blocks,16),height=mul32(height_blocks,16);
        if(screen_x<highlight->mouse_x&&highlight->mouse_x<add32(screen_x,width)&&
           screen_y<highlight->mouse_y&&highlight->mouse_y<add32(screen_y,height)) {
            highlight->clear_highlight(highlight->user_data);
            highlight->marked_x=relative_x;highlight->marked_y=relative_y;
            highlight->marked_width=width_blocks;highlight->marked_height=height_blocks;
            highlight->marked_key_x=key_x;highlight->marked_key_y=key_y;
            pharaoh_button_border_draw(c->border,1,screen_x,screen_y,width,height,0xffffu);
            pharaoh_button_border_draw(c->border,0,screen_x,screen_y,width,height,0xffffu);
        }
    }
    pharaoh_inner_panel_frame_draw_native(c->inner_panel,screen_x,screen_y,width_blocks,height_blocks);
}

/* Whole4256A0. Image/coordinate globals remain shared with the real draw. */
void pharaoh_info_square_buttons_draw(const PharaohInfoSquareDrawContext *c,
    int32_t x,int32_t y,const PharaohSquareButtonRecord *buttons,int32_t count)
{
    for(int32_t i=0;i<count;++i) {
        const PharaohSquareButtonRecord *button=&buttons[i];
        *c->draw_x=add32(x,button->x);*c->draw_y=add32(y,button->y);
        int32_t image=button->image;
        if(button->trigger==4||button->trigger==5) {
            if(button->countdown)image=add32(image,-1);
        } else if(button->pressed)image=add32(image,1);
        *c->current_image=(uint32_t)image;
        (void)pharaoh_image_draw(c->drawing,image,*c->draw_x,*c->draw_y);
        if(button->countdown)*c->ui_redraw=1;
    }
}

/* Whole40D1B0; successful drag requests redraw2 before storing the DWORD. */
int32_t pharaoh_info_storage_scrollbar(PharaohBuildingInfoLiveContext *c,
    uint8_t left_down,int32_t cursor_x,int32_t cursor_y)
{
    int32_t rows=pharaoh_storage_orders_visible_rows(*c->display_mode);
    if(*c->resource_count<=rows||!left_down)return 0;
    rows=pharaoh_storage_orders_visible_rows(*c->display_mode);
    const int32_t extra=sub32(*c->resource_count,rows);
    rows=pharaoh_storage_orders_visible_rows(*c->display_mode);
    const int32_t span=mul32(sub32(pharaoh_int32_divide_round_up(mul32(rows,4),3),6),16);
    const int32_t origin=c->window->x;
    if(cursor_x<add32(origin,414)||cursor_x>add32(origin,446)||
       cursor_y<106||cursor_y>add32(span,122))return 0;
    int32_t position=sub32(cursor_y,106);
    if(position>span)position=span;
    const int32_t percentage=pharaoh_scale_value_to_percent(position,span).value;
    const int32_t value=add32(pharaoh_adjust_by_percentage(extra,percentage),1);
    const int32_t building=c->window->building_id;
    *c->redraw_requested=2;c->buildings[building].native_e0=value;
    return 1;
}

/* Whole4D6160, reusing the mapped4CCFD0 wrapped bitmap-text owner. */
void pharaoh_info_ui_draw_multiline(const PharaohInfoLayoutDrawContext *c,
    const PharaohBitmapTextCatalogWrappedNativeContext *wrapped,const PharaohBitmapFont *font,
    int32_t group,int32_t entry,int32_t x,int32_t y,int32_t width,
    int32_t font_id,uint16_t color,int32_t origin_x,int32_t origin_y)
{
    int32_t relative_x,relative_y;
    const PharaohUiLayoutNode *node=layout_node(c,1,x,y,origin_x,origin_y,&relative_x,&relative_y);
    if(node) {
        relative_x=node->values_08[0];relative_y=node->values_08[1];width=node->values_08[2];
    }
    pharaoh_ui_layout_highlight(c->highlight,1,x,y,width,24,relative_x,relative_y,origin_x,origin_y);
    PharaohBitmapWrappedNativeContext drawing=*wrapped->drawing;
    drawing.font=font;drawing.font_id=(uint32_t)font_id;
    PharaohBitmapTextCatalogWrappedNativeContext catalog=*wrapped;
    catalog.drawing=&drawing;
    pharaoh_bitmap_text_draw_catalog_wrapped_native(&catalog,group,entry,
        add32(relative_x,origin_x),add32(relative_y,origin_y),width,color);
}

/* Whole4CCC30; singular is exactly+1/-1 and native NEG wraps for INT_MIN. */
void pharaoh_info_text_draw_amount(const PharaohUiTextLayoutContext *c,
    const PharaohBitmapFont *font,const char *suffix,int32_t group,int32_t entry,
    int32_t value,int32_t x,int32_t y,int32_t font_id,uint16_t color)
{
    const int32_t plural=value==1||value==-1?0:1;
    c->drawing->cursor_x=0;
    const char prefix=value<0?'-':' ';
    if(value<0)value=signed_bits(0u-(uint32_t)value);
    pharaoh_bitmap_text_draw_number_native(font,c->drawing,c->number_text,value,prefix,
        suffix,x,y,font_id,color);
    pharaoh_bitmap_text_draw_selection_native(font,c->drawing,c->selection,group,
        add32(entry,plural),add32(c->drawing->cursor_x,x),y,font_id,color);
}

_Static_assert(offsetof(PharaohBuildingTypeRecord,granary_resource_stored)==0x56,
    "market inventory/granary capacity WORD origin");
_Static_assert(offsetof(PharaohBuildingTypeRecord,loads_stored_render)==0x3c,
    "storage-part amount signed WORD");
_Static_assert(offsetof(PharaohBuildingTypeRecord,orders_view)==0xdc,
    "normal storage panel clamp DWORD, distinct from order scroll+E0");
_Static_assert(offsetof(PharaohBuildingTypeRecord,shipwright_boat_id)==0xb6,
    "granary/storage second-cart signed WORD overlay");
_Static_assert(offsetof(PharaohBuildingTypeRecord,reserved_42)==0x42,
    "native labor-category BYTE overlay");

static PharaohBuildingTypeRecord *building(const PharaohStorageInfoDrawContext *c)
{ return &c->ui->buildings[c->ui->window->building_id]; }
static void image(const PharaohStorageInfoDrawContext *c,int32_t id,int32_t x,int32_t y)
{ (void)pharaoh_image_draw(c->layout->inner_panel->drawing,id,x,y); }
static void resource_image(const PharaohStorageInfoDrawContext *c,int32_t resource,int32_t x,int32_t y)
{ image(c,add32(*c->ui->resource_images,resource),x,y); }
static void text(const PharaohStorageInfoDrawContext *c,const PharaohBitmapFont *font,
    int32_t font_id,int32_t group,int32_t entry,int32_t x,int32_t y)
{
    pharaoh_ui_text_draw_language(c->text,font,group,entry,x,y,font_id,0,
        c->ui->window->x,c->ui->window->y);
}
static void multiline(const PharaohStorageInfoDrawContext *c,const PharaohBitmapFont *font,
    int32_t font_id,int32_t group,int32_t entry,int32_t x,int32_t y,int32_t width)
{
    pharaoh_info_ui_draw_multiline(c->layout,c->wrapped,font,group,entry,x,y,width,
        font_id,0,c->ui->window->x,c->ui->window->y);
}
static void number(const PharaohStorageInfoDrawContext *c,const PharaohBitmapFont *font,
    int32_t font_id,int32_t value,char prefix,int32_t x,int32_t y)
{
    pharaoh_ui_text_draw_number(c->text,font,value,prefix,c->employee_suffix,x,y,
        font_id,0,c->ui->window->x,c->ui->window->y);
}
static void amount(const PharaohStorageInfoDrawContext *c,int32_t value,int32_t entry,
    int32_t x,int32_t y,const PharaohBitmapFont *font,int32_t font_id)
{
    pharaoh_info_text_draw_amount(c->text,font,c->number_suffix,8,entry,value,x,y,font_id,0);
}
static void description(const PharaohStorageInfoDrawContext *c,int32_t group,int32_t entry,int32_t y)
{
    multiline(c,c->font_black,*c->ui->font_black,group,entry,c->ui->window->x+32,y,
        (c->ui->window->width_blocks-4)*16);
}
static void inner(const PharaohStorageInfoDrawContext *c,int32_t y,int32_t height)
{
    pharaoh_info_layout_draw_inner_panel(c->layout,c->ui->window->x+16,
        c->ui->window->y+y,c->ui->window->width_blocks-2,height,c->ui->window->x,c->ui->window->y);
}
static void title(const PharaohStorageInfoDrawContext *c,int32_t help,int32_t group)
{
    PharaohBuildingInfoLiveState *w=c->ui->window;
    w->help_id=help;
    if(w->first_draw) {
        (void)pharaoh_native_return_zero_0047d350();w->first_draw=0;
    }
    pharaoh_panel_frame_draw_native(c->layout->outer_panel,w->x,w->y,w->width_blocks,w->height_blocks);
    pharaoh_ui_text_draw_centered_language(c->text,c->font_title,group,0,w->x,w->y+10,
        w->width_blocks*16,*c->ui->font_title,0,w->x,w->y);
}

static void employees(const PharaohStorageInfoDrawContext *c,int32_t y)
{
    amount(c,building(c)->num_workers,12,c->ui->window->x+60,y,c->font_green,*c->ui->font_green);
    const int32_t laborers=c->models[building(c)->building_type].laborers;
    number(c,c->font_green,*c->ui->font_green,laborers,'(',
        *c->ui->text_width+c->ui->window->x+70,y);
    text(c,c->font_green,*c->ui->font_green,69,0,
        *c->ui->text_width+c->ui->window->x+70,y);
}
/* Whole4FC5C0. Text IDs0/16/17/18/19/20 come from Native, not the partial
 * building_info_panels.c model. Category+42 indexes the real20-byte rows. */
void pharaoh_info_draw_employment(const PharaohStorageInfoDrawContext *c,int32_t y)
{
    const PharaohBuildingTypeRecord *b=building(c);
    const int32_t laborers=c->models[b->building_type].laborers;
    int32_t entry;
    if(b->num_workers>=laborers)entry=b->houses_covered<40?20:0;
    else if(*c->city_population<=0)entry=16;
    else if(b->houses_covered<=0)entry=17;
    else if(b->houses_covered<40)entry=20;
    else entry=c->labor[b->reserved_42].allocated>0?19:18;
    image(c,*c->context_icons+14,c->ui->window->x+40,y+6);
    *c->ui->text_width=0;
    if(entry==0){employees(c,y+16);return;}
    employees(c,y+10);*c->ui->text_width=0;
    multiline(c,c->font_green,*c->ui->font_green,69,entry,c->ui->window->x+70,y+26,400);
}

static int32_t inventory_index(int32_t resource)
{
    if(resource>=1&&resource<=8)return resource-1;
    switch(resource) {
    case 15:return 8;case 13:return 9;case 17:return 10;case 19:return 11;
    default:return -1;
    }
}
static int32_t market_inventory(const PharaohBuildingTypeRecord *b,int32_t resource)
{
    const int32_t index=inventory_index(resource);
    /* A native -1 index reads the preceding WORD(+54), not an invented zero. */
    return index<0?b->farm_progress:b->granary_resource_stored[index];
}
static int walker_alive(const PharaohStorageInfoDrawContext *c,int32_t id)
{ return id!=0&&c->ui->walkers[id].state==1; }
static void walker_text(const PharaohStorageInfoDrawContext *c,int32_t group,int32_t entry,
    int32_t x,int32_t y,int32_t width_blocks)
{ multiline(c,c->font_green,*c->ui->font_green,group,entry,x,y,width_blocks*16); }
static void bazaar_stock(const PharaohStorageInfoDrawContext *c)
{
    const PharaohBuildingTypeRecord *b=building(c);
    int32_t total=0;
    for(int32_t i=0;i<*c->ui->food_count;++i)
        if(c->ui->foods[i]!=0)total=add32(total,market_inventory(b,c->ui->foods[i]));
    if(*c->ui->food_count<1||total==0)description(c,97,4,c->ui->window->y+45);
    for(int32_t i=0;i<*c->ui->food_count;++i) {
        const int32_t resource=c->ui->foods[i];
        if(resource==0)continue;
        const int32_t x=c->ui->window->x+32+i*110,y=c->ui->window->y+84;
        resource_image(c,resource,x,y);
        number(c,c->font_black,*c->ui->font_black,market_inventory(building(c),resource),'@',x+32,y);
    }
    static const int32_t goods[4]={13,19,17,15};
    for(int32_t i=0;i<4;++i) {
        const int32_t x=c->ui->window->x+32+i*110;
        resource_image(c,goods[i],x,c->ui->window->y+104);
        number(c,c->font_black,*c->ui->font_black,market_inventory(building(c),goods[i]),'@',x+32,c->ui->window->y+110);
    }
}
static void bazaar_trader(const PharaohStorageInfoDrawContext *c)
{
    const PharaohBuildingTypeRecord *b=building(c);
    if(walker_alive(c,b->walker_id)) {
        const PharaohWalkerQueryRecord *w=&c->ui->walkers[b->walker_id];
        const int returning=w->action_state==2;
        resource_image(c,w->resource_id,c->ui->window->x+32,c->ui->window->y+(returning?190:184));
        walker_text(c,97,returning?12:11,c->ui->window->x+64,c->ui->window->y+184,c->ui->window->width_blocks-6);
    } else if(b->num_workers>0)
        walker_text(c,97,10,c->ui->window->x+32,c->ui->window->y+184,c->ui->window->width_blocks-4);
}
/* Whole500A40, including the inventory and trader paths. */
void pharaoh_info_draw_bazaar(const PharaohStorageInfoDrawContext *c)
{
    title(c,2,97);
    if(c->ui->window->details.road_access<1)description(c,69,25,c->ui->window->y+56);
    else if(building(c)->num_workers<1)description(c,97,2,c->ui->window->y+56);
    else bazaar_stock(c);
    inner(c,128,5);pharaoh_info_draw_employment(c,c->ui->window->y+142);bazaar_trader(c);
}

static void granary_stock(const PharaohStorageInfoDrawContext *c)
{
    int32_t stored=0;
    const PharaohBuildingTypeRecord *b=building(c);
    for(int32_t resource=1;resource<9;++resource)stored=add32(stored,b->granary_resource_stored[resource]);
    int32_t y=c->ui->window->y+40;
    *c->ui->text_width=0;
    text(c,c->font_black,*c->ui->font_black,98,2,c->ui->window->x+34,y);
    amount(c,stored,16,*c->ui->text_width+c->ui->window->x+34,y,c->font_black,*c->ui->font_black);
    *c->ui->text_width=0;
    text(c,c->font_black,*c->ui->font_black,98,3,c->ui->window->x+220,y);
    amount(c,building(c)->granary_resource_stored[0],16,*c->ui->text_width+c->ui->window->x+220,y,c->font_black,*c->ui->font_black);
    for(int32_t i=0;i<*c->ui->food_count;++i) {
        const int32_t resource=c->ui->foods[i];
        if(resource==0)continue;
        const int32_t x=c->ui->window->x+(i<2?34:240);
        y=c->ui->window->y+(i%2==0?68:92);
        resource_image(c,resource,x,y);*c->ui->text_width=0;
        number(c,c->font_black,*c->ui->font_black,building(c)->granary_resource_stored[resource],'@',x+34,y+7);
        text(c,c->font_black,*c->ui->font_black,23,resource,*c->ui->text_width+x+34,y+7);
    }
}
static int32_t cart_text(const PharaohWalkerQueryRecord *cart)
{
    uint8_t action=cart->action_state;
    if(action==6||action==7)action=cart->action_state_before_6_7;
    switch(action) {
    case 11:case 14:case 17:return 17;
    case 15:case 16:case 18:case 19:case 20:case 21:return 33;
    default:return 16;
    }
}
static void cart_walker(const PharaohStorageInfoDrawContext *c,int32_t icon_y,int32_t text_y)
{
    const PharaohBuildingTypeRecord *b=building(c);
    int32_t id=b->walker_id;
    if(!walker_alive(c,id)) {
        id=b->shipwright_boat_id;
        if(!walker_alive(c,id)) {
            if(b->num_workers>0)
                walker_text(c,99,15,c->ui->window->x+32,c->ui->window->y+text_y,c->ui->window->width_blocks-4);
            return;
        }
    }
    const PharaohWalkerQueryRecord *cart=&c->ui->walkers[id];
    resource_image(c,cart->resource_id,c->ui->window->x+32,c->ui->window->y+icon_y);
    walker_text(c,99,cart_text(cart),c->ui->window->x+64,c->ui->window->y+text_y,c->ui->window->width_blocks-6);
}
/* Whole5014A0. */
void pharaoh_info_draw_granary(const PharaohStorageInfoDrawContext *c)
{
    title(c,3,98);
    if(c->ui->window->details.road_access<1)description(c,69,25,c->ui->window->y+40);
    else if(*c->kingdom_supplies_grain==0)granary_stock(c);
    else description(c,98,4,c->ui->window->y+40);
    inner(c,163,5);pharaoh_info_draw_employment(c,c->ui->window->y+163);cart_walker(c,200,200);
}

static int counted_blocks(int32_t resource)
{ return resource==24||resource==25||resource==26||resource==30||resource==35; }
static int counted_units(int32_t resource)
{ return counted_blocks(resource)||resource==28||resource==10||resource==36; }
static void storage_good(const PharaohStorageInfoDrawContext *c,int32_t resource,int32_t row,int32_t parts_used)
{
    int32_t amount_stored=pharaoh_storageyard_resource_amount(c->ui->buildings,c->ui->window->building_id,resource);
    const int right=row>=4;
    const int32_t x=c->ui->window->x+(right?220:10);
    const int32_t y=c->ui->window->y+45+(row-(right?4:0))*30;
    resource_image(c,resource,x,y);
    const int red=amount_stored%400==0&&parts_used==8;
    const PharaohBitmapFont *font=red?c->font_red:c->font_black;
    const int32_t font_id=red?*c->ui->font_red:*c->ui->font_black;
    *c->ui->text_width=0;
    if(counted_units(resource))amount_stored/=100;
    int32_t text_y=y+7,name=resource;
    number(c,font,font_id,amount_stored,'@',x+24,text_y);
    const int32_t name_x=*c->ui->text_width+x+24;
    if(counted_blocks(resource)) {
        text(c,font,font_id,99,amount_stored==1?23:24,name_x,text_y);text_y=y+22;
    } else if((resource==28||resource==10)&&amount_stored!=1)name+=54;
    text(c,font,font_id,23,name,name_x,text_y);
}
/* Whole501FD0. Its normal-panel list clamp is+DC; order scroll remains+E0. */
void pharaoh_info_draw_storage_yard(const PharaohStorageInfoDrawContext *c)
{
    int32_t amounts[36]={0},parts_used=0,total=0;
    title(c,4,99);
    PharaohBuildingTypeRecord *b=building(c);
    if(b->orders_view==0)b->orders_view=1;
    if(*c->ui->resource_count<=b->orders_view)
        b->orders_view=*c->ui->resource_count<16?1:*c->ui->resource_count-15;
    int32_t part=c->ui->window->building_id;
    for(int32_t i=0;i<8;++i) {
        part=c->ui->buildings[part].next_part_building_id;
        if(part>0) {
            const PharaohBuildingTypeRecord *room=&c->ui->buildings[part];
            if(room->loads_stored_render>0) {
                amounts[room->subtype]=add32(amounts[room->subtype],room->loads_stored_render);
                total=add32(total,room->loads_stored_render);++parts_used;
            }
        }
    }
    if(c->ui->window->details.road_access<1)description(c,69,25,c->ui->window->y+56);
    else if(total==0)description(c,99,22,c->ui->window->y+56);
    else {
        int32_t row=0;
        for(int32_t resource=1;resource<36;++resource)
            if(amounts[resource]!=0)storage_good(c,resource,row++,parts_used);
    }
    inner(c,183,5);pharaoh_info_draw_employment(c,c->ui->window->y+188);cart_walker(c,228,231);
    const int32_t y=c->ui->window->height_blocks*16-85+c->ui->window->y;
    if(c->ui->window->storage_state==1)description(c,99,13,y);
    else if(c->ui->window->storage_state==2)description(c,99,14,y);
}
