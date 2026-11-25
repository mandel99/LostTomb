#include "pharaoh/rendering/granary_draw.h"

static int32_t add_i32(int32_t a, int32_t b)
{ return (int32_t)((uint32_t)a + (uint32_t)b); }

static int32_t pile_count(int32_t stored)
{
    if (stored == 0) return 0;
    for (int32_t piles = 1; piles <= 8; ++piles) {
        int32_t limit = 600 + (piles - 1) * 400;
        if (stored <= (limit > 3200 ? 3200 : limit)) return piles;
    }
    return 0;
}

/* Complete two-endpoint C translation of Pharaoh.exe 0x00442280. */
void pharaoh_granary_draw_stores(const PharaohGranaryDrawContext *c,
    int32_t building_id, uint32_t tile, int32_t x, int32_t y)
{
    static const int32_t positions[8][2] = {
        {110,-73},{126,-64},{145,-55},{161,-47},
        {94,-66},{111,-57},{130,-47},{147,-38}
    };
    int32_t spot = 0;
    c->determine_available_foods(c->user_data);
    if ((c->edge_flags[tile] & 0x40) == 0) return;
    for (int32_t i = 0; i < *c->food_count; ++i) {
        int32_t resource = c->food_list[i];
        int32_t piles = pile_count(
            c->buildings[building_id].granary_resource_stored[resource]);
        while (piles != 0 && spot < 8) {
            c->draw_image(c->user_data,
                c->food_pile_image_first + resource - 1,
                add_i32(x, positions[spot][0]),
                add_i32(y, positions[spot][1]));
            --piles;
            ++spot;
        }
    }
}
