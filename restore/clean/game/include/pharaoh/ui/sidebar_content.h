#ifndef PHARAOH_UI_SIDEBAR_CONTENT_H
#define PHARAOH_UI_SIDEBAR_CONTENT_H
#include "pharaoh/ui/generic_button_focus.h"
#include "pharaoh/ui/sidebar_background.h"

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
    const uint32_t *map_dirty; /* nativeE38DCC camera-scroll request DWORD */
    int32_t *minimap_dirty;
    void *user_data;
    void (*draw_buttons)(void *,int32_t x,int32_t y,PharaohImageButtonRecord *,int32_t count);
    void (*draw_sprite)(void *,int32_t image,int32_t x,int32_t y);
    void (*draw_centered_language)(void *,int32_t group,int32_t entry,int32_t x,int32_t y,
        int32_t width,int32_t font,int32_t color,int32_t origin_x,int32_t origin_y);
    void (*draw_minimap)(void *,int32_t x,int32_t y,int32_t width,int32_t height);
    void (*draw_line)(void *,int32_t x0,int32_t y0,int32_t x1,int32_t y1,int32_t color);
    const uint8_t *window_id;
    const int32_t *message_count,*problem_count,*scenario_mode,*small_font;
    int32_t *previous_message_count;
    PharaohImageButtonRecord *message_button,*problem_button,*scenario_button;
    void (*draw_number_centered)(void *,int32_t value,int32_t prefix,int32_t x,
        int32_t y,int32_t width,int32_t font,int32_t color); /*004CCE90; native empty prefix*/
    void *minimap_user_data; /*NULL retains user_data*/
} PharaohSidebarContent;

void pharaoh_sidebar_draw_buttons(PharaohSidebarContent *);
void pharaoh_sidebar_draw_caption(PharaohSidebarContent *,int32_t force,int32_t x);
void pharaoh_sidebar_draw_category(PharaohSidebarContent *,int32_t force,int32_t x);
void pharaoh_sidebar_draw_minimap(PharaohSidebarContent *,int32_t force);
void pharaoh_sidebar_draw_message_count(PharaohSidebarContent *,int32_t force);
/* Binds five actual C children and their live state; other providers retain
 * their caller-owned callbacks and user_data. Must outlive background use. */
void pharaoh_sidebar_bind_content(PharaohSidebarBackgroundOperations *,PharaohSidebarContent *);
#endif
