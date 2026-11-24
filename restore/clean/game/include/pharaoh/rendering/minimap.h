#ifndef PHARAOH_RENDERING_MINIMAP_H
#define PHARAOH_RENDERING_MINIMAP_H
#include "pharaoh/runtime/city_view.h"
#include "pharaoh/rendering/primitives.h"

typedef struct PharaohMinimap {
    const PharaohCityViewState *view;
    const PharaohCityProjectionLookup *projection;
    PharaohSpriteClipState *clip;
    const uint32_t *terrain;
    const int16_t *buildings,*walkers;
    const uint8_t *edges,*random,*extra;
    const int32_t *pixel_mode,*overlay,*editor_active,*editor_view;
    const uint8_t *color_variant,*show_all; /*native784CB0/775680; BYTE tests*/
    const int32_t *screen_width,*screen_height;
    /* Native group first-image DWORDs F56884,8C,94,A4,AC,B4,C4,D4,DC,F56B14. */
    const int32_t *land,*rock,*trees,*water,*road,*building,*overlay_image,*garden,*black,*special;
    const int32_t *terrain_02000000_image; /*F56AB4, distinct from roadF568AC*/
    int32_t origin_x,origin_y,scan_x,scan_y,left,right,top,bottom;
    uint32_t tile;
    int32_t pixel_x,pixel_y;
    void *user_data;
    int32_t (*walker_land_kind)(void *,int32_t walker); /*0043EE10*/
    int32_t (*walker_ship_kind)(void *,int32_t walker); /*0043EEB0*/
    int32_t (*walker_other_kind)(void *,int32_t walker); /*0043E790*/
    int32_t (*draw_building)(void *,int32_t building,int32_t x,int32_t y); /*00439980*/
    void (*draw_image)(void *,int32_t image,int32_t x,int32_t y); /*0053F4A0*/
    void (*draw_pair)(void *,uint32_t color_pair,int32_t x,int32_t y); /*0040FC10*/
    void (*draw_view_rect)(void *,int32_t x,int32_t y,int32_t width,int32_t height,uint32_t color);
    void *walker_user_data,*pair_user_data; /*NULL retains user_data*/
    void *image_user_data; /*NULL retains user_data*/
} PharaohMinimap;
void pharaoh_minimap_draw(PharaohMinimap *,int32_t x,int32_t y,int32_t width,int32_t height);
struct PharaohSidebarContent;
void pharaoh_sidebar_bind_minimap(struct PharaohSidebarContent *,PharaohMinimap *);
#endif
