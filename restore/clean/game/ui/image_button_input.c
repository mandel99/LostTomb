#include "pharaoh/ui/image_button_input.h"
#include <string.h>

static int32_t add(int32_t a,int32_t b)
{return (int32_t)((uint32_t)a+(uint32_t)b);}
static uint32_t word_at(const PharaohImageButtonRecord *b,uint32_t offset)
{uint32_t value;memcpy(&value,(const uint8_t *)b+offset,sizeof(value));return value;}

/* Reused whole typed SmallButtonInput.c algorithm, native00425990 (159
 * instructions). Volatile absolute storage is replaced with explicit state;
 * the original post-table trigger read is retained as a caller-owned input. */
int32_t pharaoh_image_buttons_handle_mouse(int32_t x,int32_t y,
    PharaohImageButtonRecord *buttons,int32_t count,int16_t trailing_trigger,
    PharaohImageButtonInputState *s)
{
    int32_t changed=0;
    pharaoh_image_buttons_decay(buttons,count,&s->redraw_requested);
    for(int32_t i=0;i<count;++i) {
        PharaohImageButtonRecord *b=&buttons[i];
        if(b->pressed!=0 && --b->pressed==0) changed=1;
        if(b->enabled!=0) {
            const int32_t left=add(x,b->x),top=add(y,b->y);
            if(left<=s->cursor_x && s->cursor_x<add(left,b->width) &&
               top<=s->cursor_y && s->cursor_y<add(top,b->height)) {
                s->hit_x=s->cursor_x;s->hit_y=s->cursor_y;
                if(b->pressed==0) changed=1;
                b->pressed=2;
            }
        }
    }
    if(trailing_trigger==2 || trailing_trigger==4) {
        if(s->left_pressed==0 && s->right_pressed==0) return changed;
    } else if(trailing_trigger==6 && s->left_pressed==0 && s->left_down==0) return changed;
    for(int32_t i=0;i<count;++i) {
        PharaohImageButtonRecord *b=&buttons[i];
        if(b->enabled==0 || b->pressed==0) continue;
        if(b->trigger==2) pharaoh_image_buttons_clear_radio(buttons,count,&s->redraw_requested);
        s->primary_parameter=word_at(b,28);s->secondary_parameter=word_at(b,32);
        if(s->left_pressed!=0) {
            b->activation_ticks=20;b->animation=0;
            s->dispatch(s->user_data,word_at(b,16));
        } else if(s->right_pressed!=0) s->dispatch(s->user_data,word_at(b,20));
        else if(b->trigger==6 && s->left_down!=0) {
            b->activation_ticks=20;s->dispatch(s->user_data,word_at(b,16));
        }
        return i+1;
    }
    return changed;
}
