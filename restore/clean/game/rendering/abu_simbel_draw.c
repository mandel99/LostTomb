#include "pharaoh/rendering/abu_simbel_draw.h"

typedef struct PharaohAbuSimbelPartOffset {
    int32_t view0_x, view0_y, view3_x, view3_y;
} PharaohAbuSimbelPartOffset;

static int32_t add_i32(int32_t a, int32_t b)
{ return (int32_t)((uint32_t)a + (uint32_t)b); }

/* Complete two-endpoint C translation of Pharaoh.exe 0x00401F70. */
void pharaoh_abu_simbel_draw_built_parts(
    const PharaohAbuSimbelDrawContext *c,
    int32_t building_id, int32_t x, int32_t y)
{
    static const PharaohAbuSimbelPartOffset offsets[12] = {
        {243,-99,-411,-247},{401,-178,-253,-171},{540,-244,-114,-101},
        {243,-159,-411,-307},{401,-238,-253,-231},{540,-304,-114,-161},
        {228,-269,-396,-417},{386,-348,-238,-341},{525,-414,-99,-271},
        {208,-384,-376,-532},{366,-463,-218,-456},{505,-529,-79,-386}
    };
    int32_t main_id = building_id - c->buildings[building_id].subtype;
    const uint8_t *flags = c->built_parts(c->user_data, main_id);
    int32_t view = c->map_orientation / 2;
    for (int32_t i = 0; i < 12; ++i) {
        int32_t image;
        if (flags[i] == 0) continue;
        image = (i / 3 < 2 ? c->part_a_image_first :
            c->part_b_image_first) + (view & 1);
        if (view == 0)
            c->draw_image(c->user_data, image,
                add_i32(x, offsets[i].view0_x),
                add_i32(y, offsets[i].view0_y));
        else if (view == 3)
            c->draw_image(c->user_data, image,
                add_i32(x, offsets[i].view3_x),
                add_i32(y, offsets[i].view3_y));
    }
}
