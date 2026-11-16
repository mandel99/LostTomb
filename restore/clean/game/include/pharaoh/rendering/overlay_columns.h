#ifndef PHARAOH_RENDERING_OVERLAY_COLUMNS_H
#define PHARAOH_RENDERING_OVERLAY_COLUMNS_H

#include <stdbool.h>
#include <stdint.h>

enum PharaohOverlayColumnStyle {
    PHARAOH_OVERLAY_COLUMN_SERVICE = 0,
    PHARAOH_OVERLAY_COLUMN_RISK = 1,
    PHARAOH_OVERLAY_COLUMN_WATER = 2
};

enum PharaohHouseEducation {
    PHARAOH_HOUSE_EDUCATION_NONE = 0,
    PHARAOH_HOUSE_EDUCATION_SCHOOL_OR_LIBRARY = 1,
    PHARAOH_HOUSE_EDUCATION_SCHOOL_AND_LIBRARY = 2,
    PHARAOH_HOUSE_EDUCATION_HIGHER = 3
};

typedef struct PharaohTileOffset {
    int32_t x;
    int32_t y;
} PharaohTileOffset;

typedef struct PharaohOverlayBuilding {
    int32_t previous_part_building_id;
    int32_t type;
    int32_t tile_x;
    int32_t tile_y;
    uint8_t house_size;
    uint8_t temple_orientation;
    int16_t fire_risk;
    int16_t damage_risk;
    int32_t plague_risk;
    int32_t malaria_risk;
    int32_t crime_risk;
    int8_t entertainment;
    int8_t education;
    uint8_t juggler_access;
    uint8_t musician_access;
    uint8_t dancer_access;
    uint8_t senet_access;
    uint8_t zoo_access;
    uint8_t water_access;
    uint8_t school_access;
    uint8_t library_access;
    uint8_t dentist_access;
    uint8_t physician_access;
    uint8_t apothecary_access;
    uint8_t mortuary_access;
    uint8_t courthouse_access;
    int32_t houses_covered;
    uint8_t num_gods;
    int32_t tax_coverage;
} PharaohOverlayBuilding;

typedef bool (*PharaohFarmColumnHiddenFunction)(
    void *user_data, const PharaohOverlayBuilding *building);
typedef void (*PharaohTileToPixelFunction)(
    void *user_data, int32_t tile_x, int32_t tile_y, int32_t *pixel_x,
    int32_t *pixel_y);
typedef void (*PharaohOverlayDrawColumnFunction)(
    void *user_data, int32_t height, int32_t x, int32_t y,
    enum PharaohOverlayColumnStyle style);
typedef void (*PharaohOverlayCaptureBaseImageFunction)(void *user_data);
typedef void (*PharaohOverlayDrawImageFunction)(
    void *user_data, int32_t image_id, int32_t x, int32_t y);
typedef int32_t (*PharaohHouseTaxIncomeFunction)(
    void *user_data, const PharaohOverlayBuilding *building);

typedef struct PharaohOverlayColumnContext {
    PharaohFarmColumnHiddenFunction is_farm_column_hidden;
    PharaohTileToPixelFunction tile_to_pixel;
    PharaohOverlayDrawColumnFunction draw_column;
    PharaohOverlayCaptureBaseImageFunction capture_base_image;
    PharaohOverlayDrawImageFunction draw_image;
    PharaohHouseTaxIncomeFunction house_tax_income;
    int32_t tax_coin_image_base;
    const PharaohTileOffset *temple_part_offsets;
    void *user_data;
} PharaohOverlayColumnContext;

/* Original addresses 0x00435290 through 0x00435D00. */
void pharaoh_overlay_draw_fire_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y);
void pharaoh_overlay_draw_damage_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y);
void pharaoh_overlay_draw_malaria_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y);
void pharaoh_overlay_draw_disease_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y);
void pharaoh_overlay_draw_crime_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y);
void pharaoh_overlay_draw_entertainment_column(
    const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y);
void pharaoh_overlay_draw_education_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y);
void pharaoh_overlay_draw_booth_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y);
void pharaoh_overlay_draw_bandstand_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y);
void pharaoh_overlay_draw_pavilion_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y);
void pharaoh_overlay_draw_senet_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y);
void pharaoh_overlay_draw_zoo_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y);
void pharaoh_overlay_draw_water_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y);
void pharaoh_overlay_draw_school_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y);
void pharaoh_overlay_draw_library_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y);
void pharaoh_overlay_draw_physician_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y);
void pharaoh_overlay_draw_dentist_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y);
void pharaoh_overlay_draw_apothecary_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y);
void pharaoh_overlay_draw_mortuary_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y);
void pharaoh_overlay_draw_courthouse_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y);
void pharaoh_overlay_draw_labor_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y);
void pharaoh_overlay_draw_religion_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y);
void pharaoh_overlay_draw_tax_income_column(const PharaohOverlayColumnContext *context,
    const PharaohOverlayBuilding *building, int32_t x, int32_t y);
void pharaoh_overlay_draw_tax_coins(const PharaohOverlayColumnContext *context,
    int32_t value, int32_t x, int32_t y);

#endif
