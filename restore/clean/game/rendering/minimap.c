#include "pharaoh/rendering/minimap.h"

static int32_t add(int32_t a,int32_t b) {return (int32_t)((uint32_t)a+(uint32_t)b);}
static int32_t sub(int32_t a,int32_t b) {return (int32_t)((uint32_t)a-(uint32_t)b);}
static int32_t terrain_image(const PharaohMinimap *c,uint32_t f,uint8_t r,uint8_t extra)
{
    if((f&0x40000044u)!=0 && extra) return *c->building;
    if(f&0x10000000u) return add(*c->special,55);
    if(f&0x20u) return add(*c->special,50);
    if(f&4u) return add(*c->rock,r&3);
    if(f&0x11u) return add(*c->trees,r&3);
    if(f&2u) return add(*c->water,r&3);
    if(f&0x2000000u) return add(*c->terrain_02000000_image,r&3);
    if(f&0x200u) return add(*c->water,r&3);
    if(f&0x40000040u) return *c->building;
    if(f&0x100u) return *c->garden;
    if(f&0x4000u) return add(*c->special,60);
    if(f&0x10800u) return add(*c->road,r&3);
    if(f&0x40000u) return add(*c->trees,r&4);
    return add(*c->land,r&7);
}

/* Native0043E810; prior mixed Unit00579 reused with native correction:
 * highlighted walkers use the color in BOTH WORD halves (color*10001h).
 * Projection initializer is the established actual0043E670 C owner. */
void pharaoh_minimap_draw(PharaohMinimap *c,int32_t x,int32_t y,int32_t width,int32_t height)
{
    uint32_t background,highlight;
    const uint8_t climate=*c->color_variant;
    if(*c->pixel_mode==2) {
        background=0xf000f000u;highlight=climate==1?0x7800u:climate==0?0x181fu:0x80fu;
    } else {
        background=0x78007800u;highlight=climate==0?0x3c00u:climate==1?0xc1fu:0x41fu;
    }
    c->clip->clip_x_min=x;c->clip->clip_x_max=add(x,(int32_t)((uint32_t)width*2u));
    c->clip->clip_y_min=y;c->clip->clip_y_max=add(y,height);
    pharaoh_minimap_calculate_projection_origin(c->view,c->projection,width,height,&c->origin_x,&c->origin_y);
    c->left=x;c->right=c->clip->clip_x_max;c->top=y;c->bottom=add(y,height);
    c->scan_y=sub(c->origin_y,4);
    uint32_t phase=0;
    for(int32_t row=-4;row<add(height,4);row=add(row,1),c->scan_y=add(c->scan_y,1)) {
        int32_t sx=sub(x,phase?9:8),sy=add(y,row);phase^=1u;
        c->scan_x=sub(c->origin_x,4);
        for(int32_t col=-4;col<width;col=add(col,1),sx=add(sx,2),c->scan_x=add(c->scan_x,1)) {
            const int outside=row<0 || row>=height || col<0 || col>=width;
            if(c->scan_x<0 || c->scan_x>=231 || c->scan_y<0 || c->scan_y>=457) continue;
            c->tile=(uint32_t)c->projection->cells[c->scan_y*231+c->scan_x];
            if((int32_t)c->tile>=0x0fff0000) goto black;
            const uint32_t flags=c->terrain[c->tile];
            if(flags&0x80000u) goto black;
            if(!*c->show_all && (c->scan_x<c->view->visible_minimum_x || c->scan_y<c->view->visible_minimum_y ||
                c->scan_x>c->view->visible_maximum_x || c->scan_y>(int32_t)(((uint32_t)c->view->visible_maximum_y-1u)&0xfffeu))) goto black;
            const int32_t building=c->buildings[c->tile],walker=c->walkers[c->tile];
            const int32_t first=c->walker_land_kind(c->user_data,walker);
            const int32_t second=c->walker_ship_kind(c->user_data,walker);
            uint32_t pair=background;
            if(first==2 || (first==0 && second==2)) pair=highlight*0x10001u;
            else if(first==3) pair=0;
            const int32_t third=c->walker_other_kind(c->user_data,walker);
            if(first || second || third) {
                c->pixel_x=sx;c->pixel_y=sy;
                if(!outside) c->draw_pair(c->user_data,pair,sx,sy);
                continue;
            }
            int32_t image;
            if((flags&8u)!=0 && building!=0) {
                if((c->edges[c->tile]&0x40u)==0 || outside) continue;
                if(c->draw_building(c->user_data,building,sx,sy)) continue;
                image=add(*c->special,(flags&0x10000000u)?55:60);
            } else if((flags&0xc04000u)!=0 && *c->overlay==49) image=add(*c->overlay_image,5);
            else {
                if(outside) continue;
                image=terrain_image(c,flags,c->random[c->tile],c->extra[c->tile]);
            }
            c->draw_image(c->user_data,image,sx,sy);
            continue;
black:
            if(!outside) c->draw_image(c->user_data,*c->black,sx,sy);
        }
    }
    int32_t rx=add(x,(int32_t)((uint32_t)sub(c->view->camera_x,c->origin_x)*2u));
    int32_t rw=(int32_t)((uint32_t)c->view->viewport.width_tiles*2u),rh=c->view->viewport.height_tiles;
    if(*c->editor_view!=0 && *c->editor_active==1) {rw=sub(rw,4);rh=sub(rh,6);}
    if(rx<x) rx=x;
    c->draw_view_rect(c->user_data,rx,add(sub(c->view->camera_y,c->origin_y),y),rw,rh,0xe70b);
    c->clip->clip_x_min=0;c->clip->clip_x_max=*c->screen_width;
    c->clip->clip_y_min=0;c->clip->clip_y_max=*c->screen_height;
}
