#include "pharaoh/ui/rich_text.h"

#include "pharaoh/core/percentage_adjust.h"
#include "pharaoh/core/percentage_scale.h"

#include <string.h>

/* Complete pinned EXE byte table at 0x005E047C, indexed by character. */
static const uint8_t glyph_ids[256] = {
    0xb1,0x01,0x59,0x01,0x70,0xb9,0x4c,0x00,0x80,0xf7,0x51,0x00,0x00,0x00,0x00,0x01,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x73,0x61,0x76,0x65,0x5c,0x00,0x00,0x00,
    0x00,0x3f,0x40,0x00,0x00,0x41,0x00,0x4a,0x43,0x44,0x42,0x46,0x4e,0x45,0x4f,0x4d,
    0x3e,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x48,0x49,0x00,0x47,0x00,0x4b,
    0x00,0x1b,0x1c,0x1d,0x1e,0x1f,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,
    0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,0x30,0x31,0x32,0x33,0x34,0x00,0x63,0x00,0x00,0x50,
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
    0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x00,0x00,0x00,0x00,0x00,
    0x65,0x61,0x56,0x54,0x51,0x53,0x01,0x67,0x81,0x55,0x57,0x59,0x6e,0x5d,0x69,0x1b,
    0x6a,0x67,0x6d,0x60,0x5d,0x5f,0x64,0x63,0x19,0x7b,0x6b,0x00,0x6f,0x00,0x00,0x00,
    0x52,0x7f,0x5e,0x62,0x66,0x6c,0x01,0x0f,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x82,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,
    0x72,0x70,0x71,0x71,0x69,0x83,0x6d,0x65,0x74,0x6a,0x73,0x73,0x77,0x75,0x76,0x76,
    0x00,0x6c,0x7a,0x78,0x79,0x79,0x7b,0x00,0x84,0x7e,0x7c,0x7d,0x6b,0x33,0x00,0x68,
    0x53,0x52,0x54,0x51,0x51,0x85,0x67,0x65,0x57,0x56,0x58,0x55,0x5b,0x5a,0x5c,0x59,
    0x00,0x66,0x5f,0x5e,0x60,0x60,0x5d,0x00,0x86,0x63,0x62,0x64,0x61,0x19,0x00,0x19
};

static int32_t add32(int32_t a,int32_t b){return (int32_t)((uint32_t)a+(uint32_t)b);}
static int32_t sub32(int32_t a,int32_t b){return (int32_t)((uint32_t)a-(uint32_t)b);}

static const PharaohSpriteImageRecord *image_record(
    const PharaohRichTextState *state, uint32_t image)
{
    if (image >= state->image_count) image = 0;
    return &state->images[image];
}

/* Native004CC070: eight real powers; longer inputs read the following IO
 * path bytes as DWORD weights. Possible original bug; preserve the live alias.
 * tail must contain four readable bytes per exponent above7. */
int32_t pharaoh_parse_decimal_with_tail(const uint8_t *text,const uint8_t *tail)
{
    static const uint32_t powers[8]={1,10,100,1000,10000,100000,1000000,10000000};
    const uint32_t negative=*text=='-';
    if (negative) ++text;
    uint32_t digits=0,value=0;
    while (text[digits]>='0' && text[digits]<='9') ++digits;
    for (uint32_t i=0;i<digits;++i) {
        const uint32_t exponent=digits-i-1u;
        uint32_t weight;
        if (exponent<8) weight=powers[exponent];
        else {
            const uint8_t *const w=tail+(exponent-8u)*4u;
            weight=(uint32_t)w[0]|((uint32_t)w[1]<<8)|
                ((uint32_t)w[2]<<16)|((uint32_t)w[3]<<24);
        }
        value+=(uint32_t)(text[i]-'0')*weight;
    }
    return (int32_t)(negative?0u-value:value);
}

/* Convenience for the native eight-digit domain; no tail is read there. */
int32_t pharaoh_parse_decimal(const uint8_t *text)
{return pharaoh_parse_decimal_with_tail(text,NULL);}

/* Complete C translation of Pharaoh.exe 0x004C8040. */
void pharaoh_rich_text_reset_links(PharaohRichTextState *state)
{
    memset(state->links, 0, sizeof(state->links));
    state->link_count = 0;
    state->total_link_count = 0;
}

void pharaoh_rich_text_add_link(PharaohRichTextState *state, int32_t id,
    int32_t left, int32_t right, int32_t y)
{
    if (state->total_link_count >= PHARAOH_RICH_TEXT_LINK_LIMIT) return;
    PharaohRichTextLink *const link = &state->links[state->link_count];
    link->left = add32(left, -2);
    link->right = add32(right, 2);
    link->top = add32(y, -1);
    link->bottom = add32(y, 13);
    link->id = id;
    state->link_count = add32(state->link_count, 1);
    state->total_link_count = add32(state->total_link_count, 1);
}

/* Complete C translation of Pharaoh.exe 0x004CC020. */
int32_t pharaoh_rich_text_character_width(PharaohRichTextState *state,
    uint8_t character, uint32_t font)
{
    if (character == 0) return 0;
    if (character == ' ') return 4;
    state->current_glyph = glyph_ids[character];
    const uint8_t glyph = (uint8_t)state->current_glyph;
    if (glyph == 0) return 0;
    return image_record(state, font + glyph - 1u)->width + 1;
}

/* Native glyph-index table005E049C, distinct from the character table. */
static const uint8_t glyph_images[256]={
    0x00,0x3f,0x40,0x00,0x00,0x41,0x00,0x4a,0x43,0x44,0x42,0x46,0x4e,0x45,0x4f,0x4d,
    0x3e,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x48,0x49,0x00,0x47,0x00,0x4b,
    0x00,0x1b,0x1c,0x1d,0x1e,0x1f,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,
    0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,0x30,0x31,0x32,0x33,0x34,0x00,0x63,0x00,0x00,0x50,
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
    0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x00,0x00,0x00,0x00,0x00,
    0x65,0x61,0x56,0x54,0x51,0x53,0x01,0x67,0x81,0x55,0x57,0x59,0x6e,0x5d,0x69,0x1b,
    0x6a,0x67,0x6d,0x60,0x5d,0x5f,0x64,0x63,0x19,0x7b,0x6b,0x00,0x6f,0x00,0x00,0x00,
    0x52,0x7f,0x5e,0x62,0x66,0x6c,0x01,0x0f,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x82,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,
    0x72,0x70,0x71,0x71,0x69,0x83,0x6d,0x65,0x74,0x6a,0x73,0x73,0x77,0x75,0x76,0x76,
    0x00,0x6c,0x7a,0x78,0x79,0x79,0x7b,0x00,0x84,0x7e,0x7c,0x7d,0x6b,0x33,0x00,0x68,
    0x53,0x52,0x54,0x51,0x51,0x85,0x67,0x65,0x57,0x56,0x58,0x55,0x5b,0x5a,0x5c,0x59,
    0x00,0x66,0x5f,0x5e,0x60,0x60,0x5d,0x00,0x86,0x63,0x62,0x64,0x61,0x19,0x00,0x19,
    0x35,0x35,0x35,0x00,0x35,0x35,0x36,0x00,0x35,0x36,0x35,0x00,0x36,0x35,0x35,0x00,
    0x01,0x00,0x00,0x00,0x0a,0x00,0x00,0x00,0x64,0x00,0x00,0x00,0xe8,0x03,0x00,0x00,
};

/* Native004C8480, retained complete Unit01633 C. */
int32_t pharaoh_rich_text_word_width(PharaohRichTextState *state,const uint8_t *text)
{
    const uint8_t *p=(const uint8_t *)text;int32_t width=0,iterations=1;uint8_t nonspace=0,c;
    state->consumed_bytes=0;
    do {
        c=*p++;if(!c)return width;
        if(c=='@') {
            c=*p++;
            if(!nonspace) {
                if(c=='P'||c=='L'){state->consumed_bytes=add32(state->consumed_bytes,2);return 0;}
                if(c=='G') {
                    state->consumed_bytes=add32(state->consumed_bytes,2);c=*p++;
                    while(c>='0'&&c<='9'){state->consumed_bytes=add32(state->consumed_bytes,1);c=*p++;}
                    return 0;
                }
                do {state->consumed_bytes=add32(state->consumed_bytes,1);if(c<'0'||c>'9')break;c=*p++;}while(1);
            }
        }
        if(c==' '){if(nonspace)return width;width=add32(width,4);}
        else if(c>' '){width=add32(width,pharaoh_rich_text_character_width(state,c,state->normal_font));nonspace=1;}
        state->consumed_bytes=add32(state->consumed_bytes,1);iterations=add32(iterations,1);
    }while(iterations<2000);
    return width;
}

/* Native004C8680; the current glyph is reloaded after the sprite endpoint. */
int32_t pharaoh_rich_text_draw_glyph(PharaohRichTextState *state,
    const PharaohRichTextOperations *operations,uint32_t font,uint8_t glyph,
    int32_t x,int32_t y,int32_t calculate,int32_t flags)
{
    state->current_glyph=glyph_images[glyph];
    if (!state->current_glyph) return 0;
    const uint32_t index=font+state->current_glyph-1u;
    int32_t offset=sub32(image_record(state,index)->height,flags?16:11);
    if (offset<0 || glyph<96 || glyph==199 || glyph==231) offset=0;
    if (!calculate) operations->draw_glyph(operations->user_data,index,x,
        sub32(y,offset),state->color);
    return image_record(state,font+state->current_glyph-1u)->width;
}

/* Native004C8590, retained complete Unit01634 C. */
void pharaoh_rich_text_draw_line(PharaohRichTextState *state,
    const PharaohRichTextOperations *operations,const uint8_t *text,
    int32_t x,int32_t y,int32_t calculate,int32_t flags)
{
    const uint8_t *p=(const uint8_t *)text;uint8_t c=*p++;int32_t highlighted=0,font,advance;
    while(c) {
        if(c=='@') {
            int32_t link=pharaoh_parse_decimal_with_tail(p,state->decimal_tail),width;
            /* Possible native bug: an isolated numeric marker consumes its NUL.
             * Preserve the scanner; native callers require following text/delimiter. */
            c=*p++;while(c>='0'&&c<='9')c=*p++;
            width=pharaoh_rich_text_word_width(state,p-1);highlighted=add32(state->consumed_bytes,1);
            pharaoh_rich_text_add_link(state,link,x,add32(x,width),y);
        }
        if(c>=' ') {
            uint8_t glyph=(uint8_t)(c-32);
            font=state->normal_font;if(highlighted>0){--highlighted;font=state->link_font;}
            /* Possible native bug: space measures4 pixels but advances6 here. Preserved. */
            if(!glyph_images[glyph])advance=6;
            else {
                advance=pharaoh_rich_text_draw_glyph(state,operations,(uint32_t)font,glyph,x,y,calculate,flags);
                if((uint32_t)font==state->large_font)advance=add32(advance,2);
            }
            x=add32(x,advance);
        }
        c=*p++;
    }
}

/* Native004C8070, retained complete Unit01632 C. */
void pharaoh_rich_text_draw(PharaohRichTextState *state,
    const PharaohRichTextOperations *operations,const uint8_t *text,
    int32_t x,int32_t y,int32_t width,int32_t calculate,int32_t flags)
{
    const uint8_t *p=(const uint8_t *)text;int32_t remaining=0,image=0,delay=0,line=0,iterations=0;
    uint8_t more=1,indent=0;
    if(calculate==1)state->line_count=0;
    for(;;) {
        int32_t used,xIndent,length=0;uint8_t trim=1,skip=0;
        if((!more&&!remaining)||(iterations=add32(iterations,1))>=1000)return;
        memset(state->line_buffer,0,sizeof(state->line_buffer));used=xIndent=indent?50:0;indent=0;
        while((more||remaining)&&used<width) {
            int32_t consumed,n;
            if(remaining){remaining=sub32(remaining,1);break;}
            used=add32(used,pharaoh_rich_text_word_width(state,p));
            if(used>=width){if(!used)more=0;continue;}
            consumed=state->consumed_bytes;
            for(n=0;n<consumed;n=add32(n,1)) {
                uint8_t c=*p++;
                if(c=='@') {
                    if(*p=='P'){indent=1;++p;used=width;break;}
                    if(*p=='L'){++p;used=width;break;}
                    if(*p=='G') {
                        if(length&&calculate==1)state->line_count=add32(state->line_count,1);
                        ++p;image=sub32(add32(state->inline_image_first,pharaoh_parse_decimal_with_tail(p,state->decimal_tail)),1);
                        /* Possible native bug: an isolated numeric marker consumes its NUL.
             * Preserve the scanner; native callers require following text/delimiter. */
            c=*p++;while(c>='0'&&c<='9')c=*p++;
                        remaining=add32(image_record(state,(uint32_t)image)->height/16,2);used=width;if(line>0)delay=1;break;
                    }
                }
                if(!trim||c!=' '){trim=0;state->line_buffer[(uint32_t)length]=c;length=add32(length,1);}
            }
            if(!*p)more=0;
        }
        state->draw_mode=1;state->current_line_width=0;state->draw_active=1;
        if(!calculate)skip=line<state->scroll_position||line>=add32(state->viewport_lines,state->scroll_position);
        if(calculate||!skip) {
            pharaoh_rich_text_draw_line(state,operations,state->line_buffer,add32(x,xIndent),y,calculate,flags);
            if(state->normal_font==state->large_font) {
                uint16_t Color=state->color;state->color=state->color_variant==2?0xff3a:0x7f9a;
                pharaoh_rich_text_draw_line(state,operations,state->line_buffer,sub32(add32(x,xIndent),1),sub32(y,1),calculate,flags);
                state->color=Color;
            }
        }
        if(!calculate&&image) {
            if(delay)delay=sub32(delay,1);
            else {
                int32_t sy,sx,center=sub32(sub32((int32_t)(state->text_width_blocks*16u),image_record(state,(uint32_t)image)->width),4);
                remaining=add32(image_record(state,(uint32_t)image)->height/16,2);
                if(line<add32(state->viewport_lines,state->scroll_position)) {
                    sy=add32(y,8);if(line<state->scroll_position)sy=add32(sy,(int32_t)((uint32_t)sub32(line,state->scroll_position)*16u));
                    sx=sub32(add32(x,center/2),4);operations->draw_sprite(operations->user_data,(uint32_t)image,sx,sy);
                }
                image=0;
            }
        }
        if(state->current_line_width>state->maximum_line_width)state->maximum_line_width=state->current_line_width;
        line=add32(line,1);if(calculate==1)state->line_count=add32(state->line_count,1);
        if(!skip){y=add32(y,16);state->current_y_advance=add32(state->current_y_advance,16);}
    }
}

int32_t pharaoh_rich_text_handle_scrollbar(
    PharaohRichTextScrollbarState *scrollbar,
    PharaohRichTextState *rich_text)
{
    if (scrollbar->scroll_maximum < 1 ||
        scrollbar->mouse_left_down == 0) return 0;

    const int32_t right = scrollbar->panel_x +
        scrollbar->panel_width_blocks * 16;
    const int32_t range = scrollbar->panel_height_blocks * 16 - 52;
    if (scrollbar->mouse_x < right + 1 ||
        scrollbar->mouse_x > right + 41 ||
        scrollbar->mouse_y < scrollbar->panel_y + 26 ||
        scrollbar->mouse_y > scrollbar->panel_y + 26 + range) return 0;

    int32_t position = scrollbar->mouse_y - scrollbar->panel_y - 11;
    if (position > range) position = range;
    const PharaohPercentageScaleResult percentage =
        pharaoh_scale_value_to_percent(position, range);
    scrollbar->scroll_position = pharaoh_adjust_by_percentage(
        scrollbar->scroll_maximum, percentage.value);
    scrollbar->dragging = 1;
    scrollbar->drag_position = position - 25;
    pharaoh_rich_text_reset_links(rich_text);
    if (scrollbar->drag_position < 0) scrollbar->drag_position = 0;
    scrollbar->redraw_requested = 2;
    return 1;
}
