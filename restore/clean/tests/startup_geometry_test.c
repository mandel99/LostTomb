#include "pharaoh/monuments/startup_geometry.h"
#include "pharaoh/monuments/mausoleum_projection.h"

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <stdint.h>
#include <string.h>

typedef void (*PartGeometryInitializer)(PharaohProjectionRecord *,
    const PharaohProjectionSettings *);

typedef struct PartGeometryCase {
    PartGeometryInitializer initialize;
    int32_t count;
} PartGeometryCase;

static void expected_update(PharaohProjectionRecord *record,
    const PharaohProjectionSettings *settings)
{
    const uint32_t x = record->map_x;
    const uint32_t y = record->map_y;
    const uint32_t sum = x + y;
    const uint32_t difference = x - y;
    const uint32_t step = settings->step + 1u;
    const uint32_t scaled_sum = sum * settings->scale;
    const uint32_t stepped_sum = sum * step;

    record->map_tile_offset = x + y * 228u;
    record->difference_step = difference * step;
    record->sum_step = stepped_sum;
    record->reverse_difference_step = (y - x) * step;
    record->reverse_sum_step = 0u - stepped_sum;
    record->sum_scale = scaled_sum;
    record->reverse_difference_scale = (y - x) * settings->scale;
    record->reverse_sum_scale = 0u - scaled_sum;
    record->difference_scale = difference * settings->scale;
}

static void verify_initializer(const PartGeometryCase *test_case,
    const PharaohProjectionSettings *settings)
{
    PharaohProjectionRecord actual[PHARAOH_GRAND_TOMB_PART_COUNT];
    PharaohProjectionRecord expected[PHARAOH_GRAND_TOMB_PART_COUNT];
    int32_t index;

    memset(actual, 0xDE, sizeof(actual));
    memset(expected, 0xDE, sizeof(expected));
    for (index = 0; index < PHARAOH_GRAND_TOMB_PART_COUNT; ++index) {
        actual[index].map_x = (uint32_t)(index * 3 - 7);
        actual[index].map_y = (uint32_t)(11 - index * 2);
        actual[index].preserved_word_2 = UINT32_C(0x12345678);
        actual[index].preserved_word_3 = UINT32_C(0x87654321);
        expected[index] = actual[index];
        if (index < test_case->count) {
            expected_update(&expected[index], settings);
        }
    }

    test_case->initialize(actual, settings);
    assert(memcmp(actual, expected, sizeof(actual)) == 0);
}

static void verify_mausoleum_part_description(void)
{
    PharaohMausoleumPartDescriptor source[PHARAOH_MAUSOLEUM_PART_COUNT] = {{0}};
    PharaohMausoleumPartDescriptor destination;
    PharaohMausoleumProjectionContext context = {source, NULL, 31u, 15u};

    source[0].dx = 2;
    source[0].dy = 1;
    source[0].size = 1;
    source[0].kind = 7;
    source[0].grid_delta = 19;
    source[0].view_x[0] = 23;
    source[0].view_y[0] = 29;

    pharaoh_mausoleum_part_describe(&context, &destination, 0,
        PHARAOH_MAUSOLEUM_VIEW_TOP, PHARAOH_MAUSOLEUM_VIEW_TOP);
    assert(memcmp(&destination, &source[0], sizeof(destination)) == 0);

    pharaoh_mausoleum_part_describe(&context, &destination, 0,
        PHARAOH_MAUSOLEUM_VIEW_RIGHT, PHARAOH_MAUSOLEUM_VIEW_RIGHT);
    assert(destination.dx == 1 && destination.dy == 2);
    assert(destination.grid_delta == 457);
    assert(destination.kind == source[0].kind);
    assert(destination.view_x[0] == source[0].view_x[0]);

    pharaoh_mausoleum_part_describe(&context, &destination, 0,
        PHARAOH_MAUSOLEUM_VIEW_BOTTOM,
        PHARAOH_MAUSOLEUM_VIEW_BOTTOM);
    assert(destination.dx == 19 && destination.dy == 6);
    assert(destination.grid_delta == 1387);

    pharaoh_mausoleum_part_describe(&context, &destination, 0,
        PHARAOH_MAUSOLEUM_VIEW_LEFT, PHARAOH_MAUSOLEUM_VIEW_LEFT);
    assert(destination.dx == 6 && destination.dy == 19);
    assert(destination.grid_delta == 4338);
}

static void verify_mausoleum_projection_initializer(void)
{
    PharaohMausoleumPartDescriptor initial[PHARAOH_MAUSOLEUM_PART_COUNT] = {{0}};
    PharaohMausoleumPartDescriptor projected[
        PHARAOH_MAUSOLEUM_PART_COUNT + 1] = {{0}};
    PharaohMausoleumProjectionContext context = {
        initial, projected, 30u, 15u
    };
    int32_t part;

    for (part = 0; part < PHARAOH_MAUSOLEUM_PART_COUNT; ++part) {
        initial[part].dx = 2;
        initial[part].dy = 1;
        initial[part].size = 1;
        initial[part].kind = part;
        projected[part].dx = 4;
        projected[part].dy = 5;
        projected[part].size = 6;
        projected[part].kind = part + 7;
        projected[part].grid_delta = -1;
    }
    projected[PHARAOH_MAUSOLEUM_PART_COUNT].grid_delta = 0x1234;

    pharaoh_mausoleum_projection_initialize(&context);

    for (part = 0; part < PHARAOH_MAUSOLEUM_PART_COUNT; ++part) {
        const PharaohMausoleumPartDescriptor *record = &projected[part];
        assert(record->dx == 4 && record->dy == 5 && record->size == 6);
        assert(record->kind == part + 7);
        assert(record->grid_delta == 230);
        assert(record->view_x[0] == 31 && record->view_y[0] == 45);
        assert(record->view_x[1] == 93 && record->view_y[1] == 15);
        assert(record->view_x[2] == -403 && record->view_y[2] == -375);
        assert(record->view_x[3] == -775 && record->view_y[3] == -195);
    }
    assert(projected[PHARAOH_MAUSOLEUM_PART_COUNT].grid_delta == 0x1234);
}

int main(void)
{
    static const PartGeometryCase cases[] = {
        {pharaoh_abu_simbel_part_geometry_initialize,
            PHARAOH_ABU_SIMBEL_PART_COUNT},
        {pharaoh_library_part_geometry_initialize,
            PHARAOH_LIBRARY_PART_COUNT},
        {pharaoh_caesareum_part_geometry_initialize,
            PHARAOH_CAESAREUM_PART_COUNT},
        {pharaoh_lighthouse_part_geometry_initialize,
            PHARAOH_LIGHTHOUSE_PART_COUNT},
        {pharaoh_small_tomb_part_geometry_initialize,
            PHARAOH_SMALL_TOMB_PART_COUNT},
        {pharaoh_medium_tomb_part_geometry_initialize,
            PHARAOH_MEDIUM_TOMB_PART_COUNT},
        {pharaoh_large_tomb_part_geometry_initialize,
            PHARAOH_LARGE_TOMB_PART_COUNT},
        {pharaoh_grand_tomb_part_geometry_initialize,
            PHARAOH_GRAND_TOMB_PART_COUNT},
        {pharaoh_sun_temple_part_geometry_initialize,
            PHARAOH_SUN_TEMPLE_PART_COUNT}
    };
    const PharaohProjectionSettings settings = {30u, 15u};
    uint32_t index;

    for (index = 0; index < sizeof(cases) / sizeof(cases[0]); ++index) {
        verify_initializer(&cases[index], &settings);
    }
    verify_mausoleum_part_description();
    verify_mausoleum_projection_initializer();
    return 0;
}
