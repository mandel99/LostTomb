#include "pharaoh/ui/city_topbar.h"

static int32_t add(int32_t a,int32_t b)
{return (int32_t)((uint32_t)a+(uint32_t)b);}
static const PharaohScenarioPlayerRuntime *player(const PharaohCityTopbar *c)
{return &c->players[*c->current_player];}

/* Native0051F0A0; reused Unit02349. Keep drawing advance and all cache
 * storage live across the native child boundaries. No previous_month store
 * appears in the original controller. */
void pharaoh_city_draw_topbar(PharaohCityTopbar *c,int32_t force)
{
    const int32_t font=*c->body_font;
    if(*c->editor_active==1 && *c->window_id==1)
        c->draw_text(c->user_data,c->scenario_name,260,4,*c->heading_font,0);
    switch(*c->window_id) {
    case 0:case 4:case 5:case 7:case 13:case 14:case 15:case 16:case 17:
    case 18:case 19:case 20:case 21:case 23:case 38:case 40:case 42:case 43:
    case 44:case 45:case 46:case 47:case 48:case 63:case 72:case 74:case 76:case 77:case 82:return;
    }
    if(*c->window_id>50) return;
    int32_t redraw=player(c)->population==*c->previous_population?force:1;
    if(player(c)->treasury!=*c->previous_treasury) redraw=1;
    if(*c->month==*c->previous_month && redraw==0) {
        if(*c->redraw_mode<1) return;
        if(*c->redraw_mode==1 || *c->redraw_mode==2)
            c->draw_background(c->user_data,0,*c->screen_width);
    } else {
        *c->redraw_mode=1;
        c->draw_background(c->user_data,0,*c->screen_width);
    }
    *c->redraw_mode=0;
    *c->previous_population=player(c)->population;
    *c->previous_treasury=player(c)->treasury;
    if(*c->editor_active==1) {
        c->draw_buttons(c->user_data,1,4);
        /* Native signed divide-by-five, final result stored as WORD. */
        *c->menu_button_x=(int16_t)((uint32_t)*c->screen_width-(uint32_t)(*c->screen_width/5));
        c->draw_menu_button(c->user_data);
        return;
    }
    c->draw_buttons(c->user_data,0,4);
    *c->text_advance=0;
    const int32_t money_font=player(c)->treasury<0?*c->negative_font:*c->body_font;
    /* cache x positions, treasury label/number, population label/number,
     * month label and year offset differ independently in the native code. */
    static const int32_t positions[3][9]={
        {242,367,485,250,245,375,367,515,504},
        {338,458,652,343,339,470,466,655,655},
        {493,637,885,495,501,645,651,883,883}
    };
    const int32_t width=*c->screen_width;
    const int32_t row=width==640?0:width==800?1:width==1024?2:-1;
    if(row>=0) {
        const int32_t *p=positions[row];
        *c->treasury_x=p[0];*c->population_x=p[1];*c->date_x=p[2];
        c->draw_language(c->user_data,6,0,p[3],5,money_font,0,*c->origin_x,*c->origin_y);
        c->draw_number(c->user_data,player(c)->treasury,'@',add(*c->text_advance,p[4]),5,
            money_font,0,*c->origin_x,*c->origin_y);
        *c->text_advance=0;
        c->draw_language(c->user_data,6,1,p[5],5,font,65535,*c->origin_x,*c->origin_y);
        c->draw_number(c->user_data,player(c)->population,'@',add(*c->text_advance,p[6]),5,
            font,65535,*c->origin_x,*c->origin_y);
        *c->text_advance=0;
        c->draw_language(c->user_data,25,*c->month,p[7],5,font,0,*c->origin_x,*c->origin_y);
        c->draw_year(c->user_data,row==0?2:0,*c->year,add(*c->text_advance,p[8]),5,font,0);
    }
    *c->menu_button_x=(int16_t)((uint32_t)*c->date_x-(uint32_t)(*c->screen_width==640?10:47));
    c->draw_menu_button(c->user_data);
}

static void topbar(void *data,int32_t force)
{pharaoh_city_draw_topbar(data,force);}
void pharaoh_sidebar_bind_topbar(PharaohSidebarBackgroundOperations *o,PharaohCityTopbar *c)
{o->draw_0051f0a0=topbar;o->topbar_user_data=c;}
