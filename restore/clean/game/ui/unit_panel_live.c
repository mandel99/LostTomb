#include "pharaoh/ui/unit_panel_live.h"
#include "pharaoh/military/ship_status.h"
#include <stddef.h>

_Static_assert(sizeof(PharaohBuildingInfoLiveState)==224 &&
    offsetof(PharaohBuildingInfoLiveState,transport_id)==16 &&
    offsetof(PharaohBuildingInfoLiveState,details_shown)==140 &&
    offsetof(PharaohBuildingInfoLiveState,help_id)==156,
    "native unit panel window fields");
_Static_assert(sizeof(PharaohWalkerQueryRecord)==388 &&
    offsetof(PharaohWalkerQueryRecord,ship_formation_id_092)==0x92 &&
    offsetof(PharaohWalkerQueryRecord,transport_state_130)==0x130 &&
    offsetof(PharaohWalkerQueryRecord,transport_state_154)==0x154,
    "native ship panel BYTE fields");
_Static_assert(sizeof(PharaohNativeLegionRecord)==144 &&
    offsetof(PharaohNativeLegionRecord,member_count)==0x28 &&
    offsetof(PharaohNativeLegionRecord,strength)==0x7a,
    "native legion panel fields");
_Static_assert(offsetof(PharaohSpriteImageRecord,width)==0x14 &&
    offsetof(PharaohSpriteImageRecord,height)==0x16,
    "native unit panel image dimensions");

static int32_t add32(int32_t a,int32_t b)
{return (int32_t)((uint32_t)a+(uint32_t)b);}
static int32_t mul32(int32_t a,uint32_t b)
{return (int32_t)((uint32_t)a*b);}
static int32_t px(const PharaohUnitPanelLiveContext *c,int32_t offset)
{return add32(c->window->x,offset);}
static int32_t py(const PharaohUnitPanelLiveContext *c,int32_t offset)
{return add32(c->window->y,offset);}
static int32_t after_text(const PharaohUnitPanelLiveContext *c,int32_t offset)
{return add32(px(c,offset),*c->text_width);}
static int16_t formation_word(const volatile PharaohNativeLegionRecord *f,uint32_t offset)
{return *(const volatile int16_t *)((const volatile uint8_t *)f+offset);}
static uint8_t formation_legion_id(const volatile PharaohNativeLegionRecord *f)
{return f->reserved_001[1];}

static void label(const PharaohUnitPanelLiveContext *c,const PharaohUnitPanelFont *font,
    int32_t group,int32_t entry,int32_t x,int32_t y)
{
    pharaoh_ui_text_draw_language(c->text,font->font,group,entry,x,y,*font->id,0,
        c->window->x,c->window->y);
}
static void centered(const PharaohUnitPanelLiveContext *c,const PharaohUnitPanelFont *font,
    int32_t group,int32_t entry,int32_t x,int32_t y,int32_t width)
{
    pharaoh_ui_text_draw_centered_language(c->text,font->font,group,entry,x,y,width,
        *font->id,0,c->window->x,c->window->y);
}
static void description(const PharaohUnitPanelLiveContext *c,int32_t group,int32_t entry)
{
    pharaoh_info_ui_draw_multiline(c->layout,c->wrapped,c->description.font,
        group,entry,px(c,24),py(c,252),mul32(add32(c->window->width_blocks,-4),16),
        *c->description.id,0,c->window->x,c->window->y);
}
static void border(const PharaohUnitPanelLiveContext *c,int32_t x,int32_t y,
    int32_t width,int32_t height,int32_t flags)
{
    pharaoh_info_layout_draw_border(c->layout,0,x,y,width,height,flags,
        c->window->x,c->window->y);
}
static void orders_inner(const PharaohUnitPanelLiveContext *c)
{
    pharaoh_info_layout_draw_inner_panel(c->layout,px(c,16),py(c,230),
        add32(c->window->width_blocks,-2),5,c->window->x,c->window->y);
}
static PharaohPercentageScaleResult ship_percentage(const PharaohUnitPanelLiveContext *c,int32_t id)
{
    const volatile PharaohWalkerQueryRecord *const w=&c->walkers[id];
    const int32_t maximum=w->maximum_hit_points,value=w->hit_points;
    return pharaoh_scale_value_to_percent(value,maximum);
}

/* Whole33-instruction50FC10; every normal branch assigns the entire EAX. */
PharaohPercentageScaleResult pharaoh_unit_panel_hull_text(const PharaohUnitPanelLiveContext *c,int32_t id)
{
    PharaohPercentageScaleResult result=ship_percentage(c,id);
    if(result.status!=PHARAOH_PERCENTAGE_SCALE_VALUE)return result;
    const int32_t percentage=result.value;
    result.value=percentage<=15?8:percentage<=30?7:percentage<=50?6:
        percentage<=70?5:percentage<=90?4:3;
    return result;
}

/* Whole50FC70 reuses the existing complete crew classifier. Only its one
 * observed BYTE is borrowed; this is a query value, not a copied game pool. */
int32_t pharaoh_unit_panel_crew_text(const PharaohUnitPanelLiveContext *c,int32_t id)
{
    const PharaohShipStatus ship={0,0,c->walkers[id].transport_state_154};
    const PharaohShipStatusRules rules={{0,0,0,0},0,0,28,29,30};
    return pharaoh_ship_crew_text(&ship,&rules);
}

/* Complete107-instruction Unit01765 C. Each real pixel read updates605B70.
 * Entire format DWORD is reloaded after the native WORD clear. */
void pharaoh_unit_panel_grey_rect(const PharaohUnitPanelLiveContext *c,
    int32_t x,int32_t y,int32_t width,int32_t height,uint32_t shift)
{
    uint32_t row=(uint32_t)y;
    const uint32_t end_y=row+(uint32_t)height,end_x=(uint32_t)x+(uint32_t)width;
    if((int32_t)row>=(int32_t)end_y)return;
    do{
        for(uint32_t column=(uint32_t)x;(int32_t)column<(int32_t)end_x;++column){
            int32_t red,green;
            const uint8_t *const pixels=(const uint8_t *)(const void *)c->pixels->rgb565_pixels;
            const int32_t pitch=c->pixels->rgb565_pitch_bytes;
            if(*c->pixel_format==0x565u){
                red=(int32_t)pharaoh_graphics_read_pixel_masked(pixels,pitch,(int32_t)column,row,0xf800u,c->pixel_read_scratch)>>11;
                green=(int32_t)pharaoh_graphics_read_pixel_masked(pixels,pitch,(int32_t)column,row,0x7e0u,c->pixel_read_scratch)>>6;
            }else{
                red=(int32_t)pharaoh_graphics_read_pixel_masked(pixels,pitch,(int32_t)column,row,0x7c00u,c->pixel_read_scratch)>>10;
                green=(int32_t)pharaoh_graphics_read_pixel_masked(pixels,pitch,(int32_t)column,row,0x3e0u,c->pixel_read_scratch)>>5;
            }
            const uint32_t blue=pharaoh_graphics_read_pixel_masked(pixels,pitch,(int32_t)column,row,0x1fu,c->pixel_read_scratch);
            uint32_t grey=(uint32_t)((int32_t)((uint32_t)red+(uint32_t)green+blue)/3);
            grey=(uint32_t)((int32_t)grey>>(shift&31u));
            pharaoh_graphics_and_pixel_raw(c->pixels,(int32_t)column,(int32_t)row,0);
            if(*c->pixel_format==0x565u){
                pharaoh_graphics_or_pixel_raw(c->pixels,(int32_t)column,(int32_t)row,(uint16_t)(grey<<11));
                pharaoh_graphics_or_pixel_raw(c->pixels,(int32_t)column,(int32_t)row,(uint16_t)(grey<<6));
            }else{
                pharaoh_graphics_or_pixel_raw(c->pixels,(int32_t)column,(int32_t)row,(uint16_t)(grey<<10));
                pharaoh_graphics_or_pixel_raw(c->pixels,(int32_t)column,(int32_t)row,(uint16_t)(grey<<5));
            }
            pharaoh_graphics_or_pixel_raw(c->pixels,(int32_t)column,(int32_t)row,(uint16_t)grey);
        }
        ++row;
    }while((int32_t)row<(int32_t)end_y);
}

static int repair_action(uint8_t action,int warship)
{return action==(warship?13:11) || action==15;}
static int fallback_order(uint8_t action,int warship)
{
    if(action==20)return 0;
    if(warship){if(action==23)return 1;if(action==10)return 2;if(action==13)return 3;}
    else{if(action==10)return 1;if(action==12 || action==13)return 2;if(action==11)return 3;}
    return 4;
}

/* The two existing native order bodies share this complete algorithm. Their
 * action maps/title maps differ, while every child is a real draw/query. */
static PharaohPercentageScaleStatus ship_orders(const PharaohUnitPanelLiveContext *c,int32_t id,int warship)
{
    orders_inner(c);
    const PharaohPercentageScaleResult percentage=ship_percentage(c,id);
    if(percentage.status!=PHARAOH_PERCENTAGE_SCALE_VALUE)return percentage.status;
    const volatile PharaohWalkerQueryRecord *const w=&c->walkers[id];
    int32_t selected=1;
    if(!repair_action(w->action_state,warship) || percentage.value>15){
        int fallback=0;
        for(int32_t i=0;i<5;++i){
            const int32_t hover=add32(*c->focus_primary,-1);
            switch(hover){
            case 0:case 1:case 2:if(i==hover)selected=i;break;
            case 3:
                if(i==3){if(repair_action(w->action_state,warship) || percentage.value==100)fallback=1;else selected=i;}
                break;
            case 4:
                if(i==4){if(pharaoh_ship_is_docked(c->walkers,c->buildings,id))fallback=1;else selected=i;}
                break;
            default:selected=1;fallback=1;break;
            }
        }
        if(fallback)selected=fallback_order(w->action_state,warship);
    }
    for(int32_t i=0;i<5;++i){
        const int32_t x=px(c,add32(mul32(i,85),19));
        border(c,x,py(c,139),84,84,selected==i);
        const uint8_t action=w->action_state;
        int disabled=repair_action(action,warship) && percentage.value<16;
        if(!disabled && i==3)disabled=repair_action(action,warship) || percentage.value==100;
        else if(!disabled && i==4)disabled=pharaoh_ship_is_docked(c->walkers,c->buildings,id);
        if(disabled)pharaoh_unit_panel_grey_rect(c,px(c,add32(mul32(i,85),19)),py(c,139),84,84,0);
    }
    int32_t title;
    const int32_t focus=*c->focus_primary;
    if(focus==0){
        const uint8_t action=w->action_state;
        if(warship)title=action==10?13:action==13?15:action==20?9:action==23?11:17;
        else title=action==10?21:action==11?15:action==12?23:action==13?25:action==20?19:17;
    }else if(warship)title=focus==1?9:focus==2?11:focus==3?13:focus==4?15:17;
    else title=focus==1?19:focus==2?21:focus==3?(w->transport_state_130!=0?25:23):focus==4?15:17;
    label(c,&c->heading,184,title,px(c,24),py(c,236));
    description(c,184,add32(title,1));
    return PHARAOH_PERCENTAGE_SCALE_VALUE;
}

/* Whole Unit02260/2264 C; the private shared core retains their distinct maps. */
PharaohPercentageScaleStatus pharaoh_unit_panel_draw_transport_orders(const PharaohUnitPanelLiveContext *c,int32_t id)
{return ship_orders(c,id,0);}
PharaohPercentageScaleStatus pharaoh_unit_panel_draw_warship_orders(const PharaohUnitPanelLiveContext *c,int32_t id)
{return ship_orders(c,id,1);}

/* Whole447-instruction50F320. Native53E590 uses the actual loaded asset view. */
PharaohPercentageScaleStatus pharaoh_unit_panel_draw_transport(const PharaohUnitPanelLiveContext *c)
{
    static const int32_t order_offset[5]={0,1,2,4,5}; /* pinned5F78DC */
    const int32_t id=c->window->transport_id;
    const volatile PharaohWalkerQueryRecord *const w=&c->walkers[id];
    c->window->help_id=0x16f;
    pharaoh_panel_frame_draw_native(c->layout->outer_panel,c->window->x,c->window->y,c->window->width_blocks,c->window->height_blocks);
    centered(c,&c->title,184,0,c->window->x,py(c,10),mul32(c->window->width_blocks,16));
    const PharaohPercentageScaleResult p=ship_percentage(c,id);
    if(p.status!=PHARAOH_PERCENTAGE_SCALE_VALUE)return p.status;
    label(c,&c->normal,184,2,px(c,100),py(c,40));
    const int32_t hull=p.value<1?8:p.value<31?7:p.value<51?6:p.value<71?5:p.value<91?4:3;
    label(c,&c->normal,184,hull,px(c,300),py(c,40));
    if(w->transport_state_130!=0){
        const uint8_t group=w->ship_formation_id_092;
        const volatile PharaohNativeLegionRecord *const f=&c->formations[group];
        const int32_t company=f->strength_class==11?31:f->strength_class==12?33:32;
        *c->text_width=0;
        label(c,&c->normal,184,company,px(c,100),py(c,60));
        label(c,&c->normal,138,formation_legion_id(f),after_text(c,100),py(c,60));
        label(c,&c->normal,184,34,after_text(c,100),py(c,60));
        label(c,&c->normal,138,23,px(c,100),py(c,76));
        pharaoh_ui_text_draw_number(c->text,c->normal.font,f->member_count,'@'," ",px(c,294),py(c,76),
            *c->normal.id,0,c->window->x,c->window->y);
        const PharaohPercentageScaleResult damage=pharaoh_scale_value_to_percent(formation_word(f,0x4a),formation_word(f,0x4c));
        if(damage.status!=PHARAOH_PERCENTAGE_SCALE_VALUE)return damage.status;
        label(c,&c->normal,138,24,px(c,100),py(c,92));
        const int32_t damaged=damage.value<1?26:damage.value<21?27:damage.value<41?28:
            damage.value<56?29:damage.value<71?30:damage.value>90?32:31;
        label(c,&c->normal,138,damaged,px(c,300),py(c,92));
        *c->text_width=0;
        label(c,&c->normal,138,73,px(c,100),py(c,108));
        label(c,&c->normal,138,25,after_text(c,100),py(c,108));
        const int32_t experience=(f->strength+10)/20;
        (void)pharaoh_image_draw(c->images,add32(*c->experience_icon_base,experience),px(c,275),py(c,108));
        label(c,&c->normal,138,(f->strength+10)/20+60,px(c,300),py(c,108));
        if(f->cursed_by_seth==0){
            *c->text_width=0;
            label(c,&c->normal,138,73,px(c,100),py(c,124));
            label(c,&c->normal,138,36,after_text(c,100),py(c,124));
            label(c,&c->normal,138,f->morale/5+37,px(c,300),py(c,124));
        }else label(c,&c->normal,138,59,px(c,100),py(c,124));
    }
    for(int32_t i=0;i<5;++i){
        int32_t image=add32(*c->transport_button_base,order_offset[i]);
        if(i==2 && w->transport_state_130!=0)image=add32(*c->transport_button_base,3);
        else if(i==4 && pharaoh_ship_is_docked(c->walkers,c->buildings,id))image=add32(*c->transport_button_base,6);
        (void)pharaoh_image_draw(c->images,image,px(c,add32(mul32(i,85),21)),py(c,141));
    }
    return pharaoh_unit_panel_draw_transport_orders(c,id);
}

/* Whole135-instruction50FCA0. Hull and crew children have fullDWORD results. */
PharaohPercentageScaleStatus pharaoh_unit_panel_draw_warship(const PharaohUnitPanelLiveContext *c)
{
    const int32_t id=c->window->warship_id;
    c->window->help_id=0x16d;
    pharaoh_panel_frame_draw_native(c->layout->outer_panel,c->window->x,c->window->y,c->window->width_blocks,c->window->height_blocks);
    centered(c,&c->title,184,1,c->window->x,py(c,10),mul32(c->window->width_blocks,16));
    label(c,&c->normal,184,2,px(c,100),py(c,60));
    const PharaohPercentageScaleResult hull=pharaoh_unit_panel_hull_text(c,id);
    if(hull.status!=PHARAOH_PERCENTAGE_SCALE_VALUE)return hull.status;
    label(c,&c->normal,184,hull.value,px(c,300),py(c,60));
    label(c,&c->normal,184,27,px(c,100),py(c,80));
    label(c,&c->normal,184,pharaoh_unit_panel_crew_text(c,id),px(c,300),py(c,80));
    for(int32_t i=0;i<5;++i){
        *c->layout->outer_panel->tile_y_00d3b5dc=py(c,141);
        *c->layout->outer_panel->tile_x_00d3b5e0=px(c,add32(mul32(i,85),21));
        const int32_t offset=i==4 && pharaoh_ship_is_docked(c->walkers,c->buildings,id)?5:i;
        (void)pharaoh_image_draw(c->images,add32(*c->warship_button_base,offset),
            *c->layout->outer_panel->tile_x_00d3b5e0,*c->layout->outer_panel->tile_y_00d3b5dc);
    }
    return pharaoh_unit_panel_draw_warship_orders(c,id);
}

/* Whole220-instruction510B00. Actual formation fields and live hover states. */
void pharaoh_unit_panel_draw_legion_foreground(const PharaohUnitPanelLiveContext *c,int32_t id)
{
    const volatile PharaohNativeLegionRecord *const f=&c->formations[id];
    if(f->member_count==0)return;
    for(int32_t i=0;i<5;++i){
        int32_t selected=0;
        if(*c->focus_primary==0){
            const int16_t order=f->movement_mode;
            selected=((order==1 || order==2) && i==0) ||
                ((order==3 || order==4) && i==1) ||
                (order==0 && i==1 && formation_word(f,0x7e)==0) ||
                (order==15 && i==2) || (order==6 && i==3);
        }else selected=i==add32(*c->focus_primary,-1);
        border(c,px(c,add32(mul32(i,85),19)),py(c,139),84,84,selected);
    }
    orders_inner(c);
    int32_t title,help;
    const int32_t focus=*c->focus_primary;
    if(focus==1){title=12;help=13;}
    else if(focus==2){if(f->strength_class==13){title=20;help=21;}else{title=14;help=15;}}
    else if(focus==3){title=16;help=17;}
    else if(focus==4){title=18;help=19;}
    else if(focus==5){title=58;help=22;}
    else{
        const int16_t order=f->movement_mode;
        if(order==1 || order==2){title=12;help=13;}
        else if(order==3 || order==4){title=14;help=15;}
        else if(order==0){title=20;help=21;}
        else if(order==15){title=16;help=17;}
        else if(order==6){title=18;help=19;}
        else{title=58;help=22;}
    }
    label(c,&c->heading,138,title,px(c,24),py(c,236));
    description(c,138,help);
    const int32_t x=px(c,mul32(add32(c->window->width_blocks,-20),16)/2);
    border(c,x,py(c,add32(mul32(c->window->height_blocks,16),-40)),320,30,*c->focus_secondary==1);
    centered(c,&c->normal,138,77,px(c,mul32(add32(c->window->width_blocks,-20),16)/2),
        py(c,mul32(add32(c->window->height_blocks,-2),16)),320);
}

/* Native parent dimension queries use the original image index, an upper
 * bound fallback to record0, and signed WORD dimensions. The renderer's own
 * linked-variant selection happens later and does not change this centering. */
static const PharaohSpriteImageRecord *panel_image_metadata(
    const PharaohUnitPanelLiveContext *c,int32_t image)
{
    const int32_t index=image>=*c->images->image_count?0:image;
    return &c->images->images[index];
}
static void legion_column_image(const PharaohUnitPanelLiveContext *c,int32_t image,int32_t y)
{
    const int32_t width=panel_image_metadata(c,image)->width;
    (void)pharaoh_image_draw(c->images,image,px(c,(40-width)/2+16),y);
}

/* Whole765-instruction Unit02265 C. Both orientation paths reach the actual
 * distinct renderers, including the standard's special move-order/return art.
 * The only exceptional result is the real percentage child's IDIV boundary. */
PharaohPercentageScaleStatus pharaoh_unit_panel_draw_legion(const PharaohUnitPanelLiveContext *c,int32_t id)
{
    const volatile PharaohNativeLegionRecord *const f=&c->formations[id];
    c->window->help_id=0x57;
    pharaoh_panel_frame_draw_native(c->layout->outer_panel,c->window->x,c->window->y,
        c->window->width_blocks,c->window->height_blocks);
    c->window->details_shown=0;
    centered(c,&c->title,138,formation_legion_id(f),c->window->x,py(c,10),
        mul32(c->window->width_blocks,16));

    int32_t image=add32(*c->legion_badge_base,formation_legion_id(f));
    const int32_t badge_height=panel_image_metadata(c,image)->height;
    legion_column_image(c,image,py(c,16));
    int32_t type_title;
    if(f->strength_class==13){type_title=76;image=*c->legion_type_base;}
    else if(f->strength_class==12){type_title=74;image=add32(*c->legion_type_base,18);}
    else{type_title=75;image=add32(*c->legion_type_base,9);}
    if(formation_word(f,0x5a)!=0)image=add32(image,8);
    const int16_t type_height=panel_image_metadata(c,image)->height;
    legion_column_image(c,image,py(c,add32(16,badge_height)));
    centered(c,&c->title,138,type_title,c->window->x,py(c,30),
        mul32(c->window->width_blocks,16));
    image=add32(add32(*c->legion_morale_base,-(f->morale/5)),20);
    legion_column_image(c,image,py(c,add32(add32(type_height,16),badge_height)));
    image=add32(add32(*c->legion_experience_base,-((f->strength+3)/5)),20);
    legion_column_image(c,image,py(c,add32(add32(type_height,16),badge_height)));

    label(c,&c->normal,138,23,px(c,100),py(c,60));
    pharaoh_ui_text_draw_number(c->text,c->normal.font,f->member_count,'@'," ",
        px(c,294),py(c,60),*c->normal.id,0,c->window->x,c->window->y);
    const PharaohPercentageScaleResult damage=pharaoh_scale_value_to_percent(
        formation_word(f,0x4a),formation_word(f,0x4c));
    if(damage.status!=PHARAOH_PERCENTAGE_SCALE_VALUE)return damage.status;
    label(c,&c->normal,138,24,px(c,100),py(c,80));
    const int32_t damage_text=damage.value<1?26:damage.value<21?27:
        damage.value<41?28:damage.value<56?29:damage.value<71?30:damage.value<91?31:32;
    label(c,&c->normal,138,damage_text,px(c,300),py(c,80));
    *c->text_width=0;
    label(c,&c->normal,138,73,px(c,100),py(c,100));
    label(c,&c->normal,138,25,after_text(c,100),py(c,100));
    (void)pharaoh_image_draw(c->images,add32(*c->experience_icon_base,(f->strength+10)/20),
        px(c,275),py(c,100));
    label(c,&c->normal,138,(f->strength+10)/20+60,px(c,300),py(c,100));
    if(f->cursed_by_seth==0){
        *c->text_width=0;
        label(c,&c->normal,138,73,px(c,100),py(c,120));
        label(c,&c->normal,138,36,after_text(c,100),py(c,120));
        label(c,&c->normal,138,f->morale/5+37,px(c,300),py(c,120));
    }else label(c,&c->normal,138,59,px(c,100),py(c,120));

    if(f->member_count==0){
        const int32_t group=f->cursed_by_seth!=0?89:138;
        const int32_t entry=group==89?1:*c->native_7058b8==0?11:10;
        pharaoh_info_ui_draw_multiline(c->layout,c->wrapped,c->normal.font,group,entry,
            px(c,32),py(c,172),mul32(add32(c->window->width_blocks,-4),16),
            *c->normal.id,0,c->window->x,c->window->y);
        return PHARAOH_PERCENTAGE_SCALE_VALUE;
    }
    for(int32_t i=0;i<5;++i){
        *c->layout->outer_panel->tile_y_00d3b5dc=py(c,141);
        *c->layout->outer_panel->tile_x_00d3b5e0=px(c,add32(mul32(i,85),21));
        const int16_t kind=f->strength_class;
        const int32_t base=kind==13?*c->charioteer_button_base:
            kind==11?*c->archer_button_base:*c->infantry_button_base;
        int reflected=0;
        if(kind==13 && i==1)image=add32(base,formation_word(f,0x7e)==0?1:6);
        else if(i==4 && f->deployment_blocked!=0)image=add32(base,5);
        else{
            image=add32(base,i);
            const int32_t orientation=*c->map_orientation;
            const int axis=orientation==0 || orientation==4;
            const int alternate=i<4 && f->reserved_074[3]!=0;
            reflected=alternate!=axis;
        }
        const int32_t x=*c->layout->outer_panel->tile_x_00d3b5e0;
        const int32_t y=*c->layout->outer_panel->tile_y_00d3b5dc;
        if(reflected)(void)pharaoh_image_draw_mirrored(c->images,image,x,y);
        else (void)pharaoh_image_draw(c->images,image,x,y);
    }
    return PHARAOH_PERCENTAGE_SCALE_VALUE;
}
