#ifndef PHARAOH_UI_SIDEBAR_BACKGROUND_H
#define PHARAOH_UI_SIDEBAR_BACKGROUND_H
#include <stdint.h>

typedef struct PharaohSidebarBackgroundState {
    const int32_t *screen_width,*screen_height,*hit_height;
    const int32_t *sidebar_x,*collapsed_x,*collapsed,*editor_active;
    const int32_t *game_image_base,*editor_image_base,*body_font;
    const int32_t *origin_x,*origin_y;
    uint8_t *ui_redraw;
} PharaohSidebarBackgroundState;

typedef struct PharaohSidebarBackgroundOperations {
    void *user_data;
    void (*draw_sprite)(void *,int32_t image,int32_t x,int32_t y);
    void (*fill_bottom)(void *,int32_t x,int32_t y,int32_t width,int32_t height,int32_t color);
    void (*fill_editor)(void *,int32_t x,int32_t y,int32_t width,int32_t height,int32_t color);
    void (*draw_buttons)(void *); /*0051EC90*/
    void (*draw_0051ed30)(void *,int32_t force,int32_t x);
    void (*draw_0051edd0)(void *,int32_t force,int32_t x);
    void (*draw_0051efb0)(void *,int32_t force);
    void (*draw_0051f0a0)(void *,int32_t force);
    void (*draw_0051f680)(void *,int32_t force);
    void (*draw_city)(void *); /*004DDDC0*/
    void (*draw_editor)(void *); /*00533E70*/
    void (*draw_editor_buttons)(void *,int32_t x,int32_t y,int32_t count); /*5C7760*/
    void (*draw_language)(void *,int32_t group,int32_t entry,int32_t x,int32_t y,
        int32_t font,int32_t color,int32_t origin_x,int32_t origin_y);
    void *content_user_data; /* NULL retains user_data for the five bound content children. */
} PharaohSidebarBackgroundOperations;

/* Complete controllers00537100/00537120/0051E990. All drawing endpoints
 * must be real providers; this API does not create an operational sidebar. */
void pharaoh_sidebar_draw_right_edge800(const PharaohSidebarBackgroundState *,const PharaohSidebarBackgroundOperations *);
void pharaoh_sidebar_draw_right_edge1024(const PharaohSidebarBackgroundState *,const PharaohSidebarBackgroundOperations *);
void pharaoh_sidebar_draw_background(const PharaohSidebarBackgroundState *,const PharaohSidebarBackgroundOperations *);
#endif
