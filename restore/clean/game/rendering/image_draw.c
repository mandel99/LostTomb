#include "pharaoh/rendering/image_draw.h"
#include "pharaoh/ui/bitmap_font.h"
#include "pharaoh/assets/sprite_pixel_conversion.h"
#include "pharaoh/rendering/sprite_raw_blit.h"
#include "pharaoh/rendering/sprite_rle_blit.h"
#include "pharaoh/rendering/terrain_diamond.h"
#include "pharaoh/rendering/city_ground.h"
#include "pharaoh/rendering/upper_sprite_draw.h"
#include "pharaoh/runtime/file_extension_operations.h"
#include "pharaoh/runtime/platform_measure_speed.h"
#include <string.h>

_Static_assert(offsetof(PharaohSpriteImageRecord, linked_image_offset)==0x10 &&
    offsetof(PharaohSpriteImageRecord, has_linked_variant)==0x3e,
    "native linked image metadata");

static int32_t add(int32_t a,int32_t b) {return (int32_t)((uint32_t)a+(uint32_t)b);}
static void draw_bitmap_glyph(void *data,uint32_t image,int32_t x,int32_t y,uint16_t color)
{
    PharaohImageGlyphDrawBinding *const binding=data;
    *binding->color=color;
    (void)pharaoh_image_draw(binding->drawing,(int32_t)image,x,y);
}
void pharaoh_image_bind_bitmap_font(PharaohBitmapFont *font,PharaohImageGlyphDrawBinding *binding)
{
    binding->drawing->solid_color=binding->color;
    font->user_data=binding;
    font->draw_glyph=draw_bitmap_glyph;
}
static int32_t lookup(const PharaohImageDrawContext *c,int32_t image)
{return image>=*c->image_count?0:image;}
static uint32_t skip_bytes(const PharaohSpriteClipState *s)
{return ((uint32_t)s->image_width*(uint32_t)s->clipped_top+(uint32_t)s->clipped_left)*2u;}
static PharaohSpriteRawBlitTarget raw_target(const PharaohImageDrawContext *c,int clipped)
{
    const PharaohSpriteClipState *s=c->clip;
    return (PharaohSpriteRawBlitTarget){c->surface_pixels,c->pitch_bytes,
        s->draw_x,s->draw_y,clipped?s->visible_width:s->image_width,
        s->visible_height,s->source_row_skip,s->destination_row_skip};
}
static PharaohSpriteRleBlitTarget rle_target(const PharaohImageDrawContext *c)
{
    const PharaohSpriteClipState *s=c->clip;
    return (PharaohSpriteRleBlitTarget){c->surface_pixels,c->pitch_bytes,
        s->draw_x,s->draw_y,s->image_width,s->visible_height,s->clipped_top,
        s->destination_row_skip,s->clipped_left*2,s->clipped_right,*c->solid_color};
}
static void raw(const PharaohImageDrawContext *c,const uint8_t *pixels,int mirror)
{
    const int side=c->clip->horizontal_clip;
    const PharaohSpriteRawBlitTarget t=raw_target(c,side==1 || side==2);
    if(mirror) {
        if(side==1 || side==2)pharaoh_sprite_raw_blit_mirrored_clipped(&t,pixels);
        else pharaoh_sprite_raw_blit_mirrored(&t,pixels);
    } else {
        if(side==1 || side==2)pharaoh_sprite_raw_blit_keyed_clipped(&t,pixels);
        else pharaoh_sprite_raw_blit_keyed(&t,pixels);
    }
}
static void rle(const PharaohImageDrawContext *c,const uint8_t *pixels,int mirror)
{
    const int side=c->clip->horizontal_clip;
    const PharaohSpriteRleBlitTarget t=rle_target(c);
    if(mirror) {
        if(side==1)pharaoh_image_blit_compressed_mirrored_left(&t,pixels);
        else if(side==2)pharaoh_image_blit_compressed_mirrored_right(&t,pixels);
        else pharaoh_image_blit_compressed_mirrored(&t,pixels);
    } else {
        if(side==1)(void)pharaoh_image_blit_compressed_left(&t,pixels);
        else if(side==2)(void)pharaoh_image_blit_compressed_right(&t,pixels);
        else pharaoh_image_blit_compressed(&t,pixels);
    }
}
static void mask(const PharaohImageDrawContext *c,const uint8_t *pixels,int compressed)
{
    if(*c->tint==0)return;
    const int side=c->clip->horizontal_clip;
    if(compressed) {
        const PharaohSpriteRleBlitTarget t=rle_target(c);
        if(side==1)pharaoh_image_blit_compressed_mask_left(&t,pixels,*c->tint);
        else if(side==2)pharaoh_image_blit_compressed_mask_right(&t,pixels,*c->tint);
        else pharaoh_image_blit_compressed_mask(&t,pixels,*c->tint);
    } else {
        const PharaohSpriteRawBlitTarget t=raw_target(c,side==1 || side==2);
        if(side==1 || side==2)pharaoh_sprite_raw_mask_clipped(&t,pixels,*c->tint);
        else pharaoh_sprite_raw_mask(&t,pixels,*c->tint);
    }
}
static const char *bitmap(const PharaohImageDrawContext *c,int32_t original)
{
    const PharaohSpriteImageRecord *s=&c->images[lookup(c,original)];
    return c->libraries[s->library_index].bitmap_names[s->bitmap_id];
}
static void opaque(const PharaohImageDrawContext *c,const uint8_t *pixels)
{
    const PharaohSpriteRawBlitTarget t=raw_target(c,1);
    pharaoh_sprite_raw_blit_clipped(&t,pixels);
}
static void ground_image(void *data,int32_t image,int32_t x,int32_t y)
{
    (void)pharaoh_image_draw(data,image,x,y);
}
void pharaoh_city_ground_bind_image_draw(PharaohCityGroundOperations *ops,
    PharaohImageDrawContext *context)
{
    ops->draw_image=ground_image;
    ops->image_user_data=context;
}
static void ground_top(void *data,int32_t image,int32_t x,int32_t y)
{
    PharaohImageDrawContext *c=data;
    const PharaohUpperSpriteDrawContext top={c->images,c->image_pixels,
        *c->image_count,c->clip,c->surface_pixels,c->pitch_bytes,*c->tint,
        &c->state->source.pixels};
    (void)pharaoh_image_draw_isometric_top(&top,image,x,y);
}
void pharaoh_city_ground_bind_sprite_draw(PharaohCityGroundOperations *ops,
    PharaohImageDrawContext *context)
{
    pharaoh_city_ground_bind_image_draw(ops,context);
    ops->draw_top=ground_top;ops->top_user_data=context;
}

/* Reused Unit02524 algorithm. Native source pointer/temporary offset overlay
 * is a typed union; loaded image pointers are supplied separately from SG3
 * file offsets. The native metadata-linked variant is a horizontal mirror. */
int32_t pharaoh_image_draw(PharaohImageDrawContext *c,int32_t image_id,int32_t x,int32_t y)
{
    PharaohImageDrawState *const d=c->state;
    PharaohSpriteClipState *const s=c->clip;
    d->image=image_id;d->x=x;d->y=y;
    if(image_id<1 || image_id>0x639b)return 0;
    int32_t index=lookup(c,image_id);
    const PharaohSpriteImageRecord *image=&c->images[index];
    int mirror=0;
    if(image->has_linked_variant!=0) {
        index=lookup(c,add(image_id,image->linked_image_offset));
        image=&c->images[index];mirror=1;
    }
    d->source.offset=image->pixel_offset;
    if((int32_t)image->pixel_offset<1)return 0;
    if(image->is_external==0) {
        d->source.pixels=c->image_pixels[index];
        if(d->source.pixels==NULL)return 0;
    }
    s->image_width=image->width;s->image_height=image->height;
    d->draw_type=image->draw_type;s->draw_x=x;s->draw_y=y;
    s->visible_width=image->width;
    if(image->is_external!=0 && image->width<=s->window_width) {
        char path[260];memcpy(path,"data\\",5);
        const char *const name=bitmap(c,image_id);
        memcpy(path+5,name,strlen(name)+1);
        pharaoh_file_change_extension(c->files->media_root->path_state,path,"555");
        pharaoh_sprite_release_pixels(c->pixel_memory,c->external_pixels,0);
        *c->external_pixels=c->pixel_memory->shared_pixel_buffer;
        if(pharaoh_file_read_part(c->files,path,*c->external_pixels,
            (int32_t)image->pixel_bytes,(int32_t)(image->pixel_offset-1u))==0)return 0;
        if(image->is_compressed==0)
            pharaoh_sprite_convert_raw_555_to_565(*c->pixel_mode,*c->external_pixels,(int32_t)image->pixel_bytes);
        else pharaoh_sprite_convert_compressed_555_to_565(*c->pixel_mode,*c->external_pixels,(int32_t)image->pixel_bytes);
        pharaoh_graphics_clip_sprite_horizontal(s);pharaoh_graphics_clip_sprite_vertical(s);
        if(s->vertical_clip==5)return 0;
        if(d->draw_type==0 && image->is_compressed!=0)
            rle(c,*c->external_pixels,mirror);
        else {
            d->source.offset=skip_bytes(s);
            raw(c,*c->external_pixels+d->source.offset,mirror);
        }
        return s->image_width;
    }
    if(image->is_external!=0) {
        d->crop_width=*c->screen_width;s->visible_width=*c->screen_width;
        d->crop_y=(int32_t)(768u-(uint32_t)*c->screen_height)/2;
        d->crop_x=(int32_t)(1024u-(uint32_t)*c->screen_width)/2;
        d->crop_height=*c->screen_height;s->visible_height=*c->screen_height;
        const uint32_t offset=((uint32_t)s->image_width*(uint32_t)d->crop_y+(uint32_t)d->crop_x)*2u;
        const char *const name=bitmap(c,image_id);memcpy(c->bitmap_name_scratch,name,strlen(name)+1);
        pharaoh_sprite_release_pixels(c->pixel_memory,c->external_pixels,0);
        *c->external_pixels=c->pixel_memory->shared_pixel_buffer;s->draw_x=0;s->draw_y=0;
        const PharaohSpriteFetchRowsContext fetch={c->files,(int16_t)s->image_width,c->pixel_mode,c->external_pixels};
        (void)pharaoh_sprite_fetch_rows(&fetch,c->bitmap_name_scratch,*c->external_pixels,
            (int32_t)((uint32_t)s->visible_width*2u),s->visible_height,(int32_t)offset);
        s->source_row_skip=0;s->destination_row_skip=0;opaque(c,*c->external_pixels);
        return s->visible_width;
    }
    if((*c->window_id==20 || *c->window_id==21) && image_id==*c->special_image) {
        s->visible_height=d->crop_height;
        s->destination_row_skip=(int32_t)(((uint32_t)s->window_width-(uint32_t)d->crop_width)*2u);
        s->source_row_skip=(int32_t)(((uint32_t)s->image_width-(uint32_t)d->crop_width)*2u);
        s->visible_width=d->crop_width;
        const uint32_t offset=((uint32_t)s->image_width*(uint32_t)d->crop_y+(uint32_t)d->crop_x)*2u;
        opaque(c,d->source.pixels+offset);return s->visible_width;
    }
    if(d->draw_type==30) {
        if(s->image_width==58)s->image_height=30;
        else if(s->image_width==26)s->image_height=14;
        else if(s->image_width==10)s->image_height=6;
        else if(s->image_width==118 || s->image_width==178 || s->image_width==238 || s->image_width==298 || s->image_width==358) {
            PharaohTerrainFootprintContext t={s,d->source.pixels+c->terrain_source_bias,c->surface_pixels,c->pitch_bytes,*c->tint};
            switch(s->image_width) {
            case 118:return pharaoh_terrain_footprint_draw2(&t);
            case 178:return pharaoh_terrain_footprint_draw3(&t);
            case 238:return pharaoh_terrain_footprint_draw4(&t);
            case 298:return pharaoh_terrain_footprint_draw5(&t);
            default:return pharaoh_terrain_footprint_draw6(&t);
            }
        }
    }
    pharaoh_graphics_clip_sprite_horizontal(s);pharaoh_graphics_clip_sprite_vertical(s);
    if(s->vertical_clip==5)return 0;
    if(d->draw_type==0) {
        if(image->is_compressed==0) {d->source.pixels+=skip_bytes(s);raw(c,d->source.pixels,mirror);}
        else rle(c,d->source.pixels,mirror);
        mask(c,d->source.pixels,image->is_compressed!=0);return s->visible_width;
    }
    if(d->draw_type==30) {
        if(s->image_width==26 || s->image_width==10) {
            pharaoh_actor_walking_no_operation();return s->visible_width;
        }
        if(s->image_width==58) {
            const PharaohTerrainRasterTarget t={c->surface_pixels,c->pitch_bytes,s->draw_x,s->draw_y,*c->tint};
            if(s->horizontal_clip==1)pharaoh_terrain_diamond_draw_left(&t,d->source.pixels,s->vertical_clip);
            else if(s->horizontal_clip==2)pharaoh_terrain_diamond_draw_right(&t,d->source.pixels,s->vertical_clip);
            else pharaoh_terrain_diamond_draw(&t,d->source.pixels,s->vertical_clip);
        } else raw(c,d->source.pixels,s->horizontal_clip==1?0:1);
        return s->visible_width;
    }
    const PharaohSpriteRawBlitTarget target=raw_target(c,1);
    if(s->horizontal_clip!=0 && (d->draw_type==10 || d->draw_type==12 || d->draw_type==13 || d->draw_type==2)) {
        if(d->draw_type==10)pharaoh_sprite_raw_blit_square16(&target,d->source.pixels);
        else if(d->draw_type==12)pharaoh_sprite_raw_blit_square24(&target,d->source.pixels);
        else if(d->draw_type==13)pharaoh_sprite_raw_blit_square32(&target,d->source.pixels);
        else {
            PharaohGraphicsPrimitives p={0};p.rgb565_pixels=(uint16_t *)(void *)c->surface_pixels;p.rgb565_pitch_bytes=c->pitch_bytes;
            pharaoh_graphics_blit_l_pixel_raw(&p,s->draw_x,s->draw_y,(const uint16_t *)(const void *)d->source.pixels);
        }
        return s->visible_width;
    }
    if(d->draw_type==20) {
        const PharaohSpriteRleBlitTarget t=rle_target(c);
        if(s->horizontal_clip==1)(void)pharaoh_image_blit_compressed_color_left(&t,d->source.pixels);
        else if(s->horizontal_clip==2)(void)pharaoh_image_blit_compressed_color_right(&t,d->source.pixels);
        else pharaoh_image_blit_compressed_color(&t,d->source.pixels);
        return s->visible_width;
    }
    d->source.pixels+=skip_bytes(s);
    if(!mirror) {
        if(s->horizontal_clip==1 || s->horizontal_clip==2 || s->vertical_clip!=0)raw(c,d->source.pixels,0);
        else opaque(c,d->source.pixels);
    } else raw(c,d->source.pixels,1);
    return s->visible_width;
}

/* Complete native0053F4F0 over resident image pointers and existing mask kernels.
   The raw source remains unadjusted after clipping, exactly as native. */
int32_t pharaoh_image_draw_masked(PharaohImageDrawContext *c,
    int32_t image_id, int32_t x, int32_t y, uint32_t mask_value)
{
    if (image_id < 1 || image_id > 0x639b) return 0;
    const int32_t index = lookup(c, image_id);
    const PharaohSpriteImageRecord *const image = &c->images[index];
    PharaohSpriteClipState *const clip = c->clip;
    c->state->source.pixels = c->image_pixels[index];
    if ((int32_t)(uint32_t)(uintptr_t)c->state->source.pixels < 1) return 0;
    clip->image_width = image->width;
    clip->image_height = image->height;
    c->state->draw_type = image->draw_type;
    clip->draw_x = x;
    clip->visible_width = clip->image_width;
    clip->draw_y = y;
    pharaoh_graphics_clip_sprite_horizontal(clip);
    pharaoh_graphics_clip_sprite_vertical(clip);
    if (clip->vertical_clip == 5) return 0;
    const uint16_t value = (uint16_t)mask_value;
    const uint8_t *const pixels = c->state->source.pixels;
    if (image->is_compressed == 0) {
        const int clipped = clip->horizontal_clip == 1 || clip->horizontal_clip == 2;
        const PharaohSpriteRawBlitTarget target = raw_target(c, clipped);
        if (clipped) pharaoh_sprite_raw_mask_clipped(&target, pixels, value);
        else pharaoh_sprite_raw_mask(&target, pixels, value);
    } else {
        const PharaohSpriteRleBlitTarget target = rle_target(c);
        if (clip->horizontal_clip == 1)
            pharaoh_image_blit_compressed_mask_left(&target, pixels, value);
        else if (clip->horizontal_clip == 2)
            pharaoh_image_blit_compressed_mask_right(&target, pixels, value);
        else pharaoh_image_blit_compressed_mask(&target, pixels, value);
    }
    return clip->visible_width;
}
