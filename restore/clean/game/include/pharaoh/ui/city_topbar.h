#ifndef PHARAOH_UI_CITY_TOPBAR_H
#define PHARAOH_UI_CITY_TOPBAR_H
#include "pharaoh/scenario/scenario_init_custom.h"
#include "pharaoh/ui/sidebar_background.h"

typedef struct PharaohCityTopbar {
    const uint8_t *window_id;
    const int32_t *editor_active,*screen_width,*current_player,*month,*year;
    const PharaohScenarioPlayerRuntime *players;
    const int32_t *body_font,*negative_font,*heading_font,*origin_x,*origin_y;
    const char *scenario_name;
    int32_t *previous_population,*previous_treasury;
    const int32_t *previous_month;
    int32_t *redraw_mode,*treasury_x,*population_x,*date_x,*text_advance;
    int16_t *menu_button_x;
    void *user_data;
    void (*draw_text)(void *,const char *,int32_t x,int32_t y,int32_t font,int32_t color);
    void (*draw_background)(void *,int32_t x,int32_t width); /*00536670*/
    void (*draw_buttons)(void *,int32_t editor,int32_t count); /*5C91F0/5C9230*/
    void (*draw_language)(void *,int32_t group,int32_t entry,int32_t x,int32_t y,
        int32_t font,int32_t color,int32_t origin_x,int32_t origin_y);
    void (*draw_number)(void *,int32_t value,int32_t prefix,int32_t x,int32_t y,
        int32_t font,int32_t color,int32_t origin_x,int32_t origin_y);
    void (*draw_year)(void *,int32_t mode,int32_t year,int32_t x,int32_t y,int32_t font,int32_t color);
    void (*draw_menu_button)(void *); /*00425900*/
} PharaohCityTopbar;

void pharaoh_city_draw_topbar(PharaohCityTopbar *,int32_t force);
void pharaoh_sidebar_bind_topbar(PharaohSidebarBackgroundOperations *,PharaohCityTopbar *);
#endif
