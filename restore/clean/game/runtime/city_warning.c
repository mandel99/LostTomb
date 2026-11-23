#include "pharaoh/runtime/city_warning.h"
#include "pharaoh/core/memory_fill.h"
#include "pharaoh/core/memory_strings.h"
#include "pharaoh/core/text_length.h"

/* Completed prior MessageController C, native004E2970. */
void pharaoh_city_warning_post(PharaohCityWarningContext *c,int32_t message)
{
    uint32_t slot;
    if (!*c->enabled || (message==2 && *c->editor_active==1) ||
        !*c->construction_enabled) return;
    for (slot=0;slot<5;++slot) if (!c->warnings[slot].in_use) break;
    if (slot==5) return;
    PharaohCityWarning *const r=&c->warnings[slot];
    c->state[slot]=0;r->in_use=1;
    if (message==1) pharaoh_text_select(c->selection,17,(int32_t)*c->orientation);
    else pharaoh_text_select(c->selection,19,(int32_t)((uint32_t)message-2u));
    *c->redraw=1;r->warning_id=(int16_t)message;
    r->box_width=(int16_t)pharaoh_bitmap_text_width_native(c->font,
        (const uint8_t *)c->selection->current,c->glyph_map,c->current_glyph);
    r->text_length=(int16_t)pharaoh_text_length(c->selection->current);
    const int16_t width=r->box_width;
    r->box_width=width<=100?200:width<=200?300:width<=300?400:460;
    r->time=(int32_t)*c->time;
    uint8_t *const text=(uint8_t *)(void *)c->warnings+slot*sizeof(*r)+18u;
    pharaoh_memory_fill_bytes((uint32_t *)(void *)text,100,0);
    /* Possible original bug: no100-byte cap after the fixed100-byte clear.
       Keep native copy length; the resource/caller must supply valid storage. */
    pharaoh_memory_copy_forward((const uint8_t *)c->selection->current,text,r->text_length);
}
