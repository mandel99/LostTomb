#include "pharaoh/rendering/overlay_columns.h"

enum {
    BUILDING_TEMPLE_BAST = 64,
    BUILDING_BAZAAR = 70
};

static void draw_risk_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y, int32_t risk)
{
    int32_t pixel_x = x;
    int32_t pixel_y = y;

    if (building->previous_part_building_id != 0 ||
        (context->is_farm_column_hidden != NULL &&
            context->is_farm_column_hidden(context->user_data, building))) {
        return;
    }
    if (building->type > BUILDING_TEMPLE_BAST && building->type < BUILDING_BAZAAR &&
        context->temple_part_offsets != NULL && context->tile_to_pixel != NULL) {
        const PharaohTileOffset *offset =
            &context->temple_part_offsets[building->temple_orientation >> 1];
        context->tile_to_pixel(context->user_data, building->tile_x + offset->x,
            building->tile_y + offset->y, &pixel_x, &pixel_y);
    }
    context->draw_column(context->user_data, risk / 100, pixel_x, pixel_y,
        PHARAOH_OVERLAY_COLUMN_RISK);
}

static void capture_house_tile(const PharaohOverlayColumnContext *context)
{
    if (context->capture_base_image != NULL) {
        context->capture_base_image(context->user_data);
    }
}

static void draw_house_risk(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y, int32_t risk)
{
    capture_house_tile(context);
    if (building->house_size != 0u) {
        context->draw_column(context->user_data, risk / 100, x, y,
            PHARAOH_OVERLAY_COLUMN_RISK);
    }
}

static void draw_access_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y, uint8_t access,
    enum PharaohOverlayColumnStyle style)
{
    capture_house_tile(context);
    if (building->house_size != 0u) {
        context->draw_column(context->user_data, access / 10, x, y, style);
    }
}

void pharaoh_overlay_draw_fire_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y)
{
    draw_risk_column(context, building, x, y, building->fire_risk);
}

void pharaoh_overlay_draw_damage_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y)
{
    draw_risk_column(context, building, x, y, building->damage_risk);
}

void pharaoh_overlay_draw_malaria_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y)
{
    draw_house_risk(context, building, x, y, building->malaria_risk);
}

void pharaoh_overlay_draw_disease_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y)
{
    draw_house_risk(context, building, x, y, building->plague_risk);
}

void pharaoh_overlay_draw_crime_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y)
{
    draw_house_risk(context, building, x, y, building->crime_risk);
}

void pharaoh_overlay_draw_entertainment_column(
    const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y)
{
    capture_house_tile(context);
    if (building->house_size != 0u) {
        context->draw_column(context->user_data, building->entertainment / 10, x, y,
            PHARAOH_OVERLAY_COLUMN_SERVICE);
    }
}

void pharaoh_overlay_draw_education_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y)
{
    int32_t height;
    capture_house_tile(context);
    if (building->house_size == 0u) {
        return;
    }
    switch (building->education) {
        case PHARAOH_HOUSE_EDUCATION_SCHOOL_OR_LIBRARY:
            height = 2;
            break;
        case PHARAOH_HOUSE_EDUCATION_SCHOOL_AND_LIBRARY:
            height = 5;
            break;
        case PHARAOH_HOUSE_EDUCATION_HIGHER:
            height = 8;
            break;
        default:
            return;
    }
    context->draw_column(context->user_data, height, x, y,
        PHARAOH_OVERLAY_COLUMN_SERVICE);
}

void pharaoh_overlay_draw_booth_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y)
{
    draw_access_column(context, building, x, y, building->juggler_access,
        PHARAOH_OVERLAY_COLUMN_SERVICE);
}

void pharaoh_overlay_draw_bandstand_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y)
{
    draw_access_column(context, building, x, y, building->musician_access,
        PHARAOH_OVERLAY_COLUMN_SERVICE);
}

void pharaoh_overlay_draw_pavilion_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y)
{
    draw_access_column(context, building, x, y, building->dancer_access,
        PHARAOH_OVERLAY_COLUMN_SERVICE);
}

void pharaoh_overlay_draw_senet_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y)
{
    draw_access_column(context, building, x, y, building->senet_access,
        PHARAOH_OVERLAY_COLUMN_SERVICE);
}

void pharaoh_overlay_draw_zoo_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y)
{
    draw_access_column(context, building, x, y, building->zoo_access,
        PHARAOH_OVERLAY_COLUMN_SERVICE);
}

void pharaoh_overlay_draw_water_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y)
{
    draw_access_column(context, building, x, y, building->water_access,
        PHARAOH_OVERLAY_COLUMN_WATER);
}

void pharaoh_overlay_draw_school_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y)
{
    draw_access_column(context, building, x, y, building->school_access,
        PHARAOH_OVERLAY_COLUMN_SERVICE);
}

void pharaoh_overlay_draw_library_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y)
{
    draw_access_column(context, building, x, y, building->library_access,
        PHARAOH_OVERLAY_COLUMN_SERVICE);
}

void pharaoh_overlay_draw_physician_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y)
{
    draw_access_column(context, building, x, y, building->physician_access,
        PHARAOH_OVERLAY_COLUMN_SERVICE);
}

void pharaoh_overlay_draw_dentist_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y)
{
    draw_access_column(context, building, x, y, building->dentist_access,
        PHARAOH_OVERLAY_COLUMN_SERVICE);
}

void pharaoh_overlay_draw_apothecary_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y)
{
    draw_access_column(context, building, x, y, building->apothecary_access,
        PHARAOH_OVERLAY_COLUMN_SERVICE);
}

void pharaoh_overlay_draw_mortuary_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y)
{
    draw_access_column(context, building, x, y, building->mortuary_access,
        PHARAOH_OVERLAY_COLUMN_SERVICE);
}

void pharaoh_overlay_draw_courthouse_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y)
{
    draw_access_column(context, building, x, y, building->courthouse_access,
        PHARAOH_OVERLAY_COLUMN_SERVICE);
}

void pharaoh_overlay_draw_labor_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y)
{
    if (building->house_size == 0u)
        context->draw_column(context->user_data,
            (building->houses_covered * 100) / 300, x, y,
            PHARAOH_OVERLAY_COLUMN_SERVICE);
}

void pharaoh_overlay_draw_religion_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y)
{
    capture_house_tile(context);
    if (building->house_size != 0u)
        context->draw_column(context->user_data, building->num_gods * 17,
            x, y, PHARAOH_OVERLAY_COLUMN_SERVICE);
}

void pharaoh_overlay_draw_tax_coins(const PharaohOverlayColumnContext *context,
    int32_t value, int32_t x, int32_t y)
{
    int32_t minimum, coins;
    if (value >= 200) value = 200;
    minimum = value <= 0 ? 0 : 3;
    if (value <= minimum) value = minimum;
    coins = value / 3; y += 10;
    while (coins > 0) {
        int32_t image = coins < 4 ? coins % 4 - 1 : 3;
        context->draw_image(context->user_data,
            context->tax_coin_image_base + image, x + 15, y);
        coins -= 4; y -= 12;
    }
}

void pharaoh_overlay_draw_tax_income_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y)
{
    capture_house_tile(context);
    if (building->house_size == 0u) return;
    context->draw_column(context->user_data,
        (building->tax_coverage * 20) / 100, x, y,
        PHARAOH_OVERLAY_COLUMN_SERVICE);
    pharaoh_overlay_draw_tax_coins(context,
        context->house_tax_income(context->user_data, building), x, y);
}
