#ifndef PHARAOH_UI_SIDEBAR_CONTENT_H
#define PHARAOH_UI_SIDEBAR_CONTENT_H
#include "pharaoh/ui/generic_button_focus.h"

typedef struct PharaohSidebarContent {
    const int32_t *collapsed,*sidebar_x,*collapsed_x,*category,*category_image_base;
    const int32_t *caption,*body_font,*origin_x,*origin_y;
    int32_t *previous_caption;
    const uint8_t *ui_redraw;
    const int32_t *command_a,*command_b;
    PharaohImageButtonRecord *command_button; /*enabled native5C7538*/
    PharaohImageButtonRecord *expanded_controls,*expanded_buildings,*expanded_extra;
    PharaohImageButtonRecord *collapsed_control,*collapsed_buildings;
    const int32_t *editor_active,*editor_dirty;
    const uint8_t *map_dirty;
    int32_t *minimap_dirty;
    void *user_data;
    void (*draw_buttons)(void *,int32_t x,int32_t y,PharaohImageButtonRecord *,int32_t count);
    void (*draw_sprite)(void *,int32_t image,int32_t x,int32_t y);
    void (*draw_centered_language)(void *,int32_t group,int32_t entry,int32_t x,int32_t y,
        int32_t width,int32_t font,int32_t color,int32_t origin_x,int32_t origin_y);
    void (*draw_minimap)(void *,int32_t x,int32_t y,int32_t width,int32_t height);
    void (*draw_line)(void *,int32_t x0,int32_t y0,int32_t x1,int32_t y1,int32_t color);
} PharaohSidebarContent;

void pharaoh_sidebar_draw_buttons(PharaohSidebarContent *);
void pharaoh_sidebar_draw_caption(PharaohSidebarContent *,int32_t force,int32_t x);
void pharaoh_sidebar_draw_category(PharaohSidebarContent *,int32_t force,int32_t x);
void pharaoh_sidebar_draw_minimap(PharaohSidebarContent *,int32_t force);
#endif
