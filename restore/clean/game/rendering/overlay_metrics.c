#include "pharaoh/rendering/overlay_metrics.h"

int32_t pharaoh_overlay_fertility_height(
    const PharaohOverlayMetricContext *c, int32_t offset)
{
    uint8_t value = c->soil_fertility[offset];
    if (value == 0u) return 0;
    if (value < 10u) return 1;
    if (value < 20u) return 2;
    if (value < 30u) return 3;
    if (value < 40u) return 4;
    if (value < 50u) return 5;
    if (value < 60u) return 6;
    if (value < 70u) return 7;
    if (value < 80u) return 8;
    return 9;
}

int32_t pharaoh_overlay_desirability_height(
    const PharaohOverlayMetricContext *c, int32_t offset)
{
    int8_t value = c->desirability[offset];
    if ((c->terrain[offset] & c->terrain_building_flag) == 0u && value == 0)
        return 0;
    if (value < -10) return 0;
    if (value < -5) return 1;
    if (value < 0) return 2;
    if (value == 0) return 3;
    if (value < 5) return 4;
    if (value < 10) return 5;
    if (value < 15) return 6;
    if (value < 20) return 7;
    return value > 24 ? 9 : 8;
}

int32_t pharaoh_overlay_malaria_height(const PharaohOverlayBuilding *b)
{ return b->house_size == 0u ? 0 : b->malaria_risk / 100; }

int32_t pharaoh_overlay_disease_height(const PharaohOverlayBuilding *b)
{ return b->house_size == 0u ? 0 : b->plague_risk / 100; }

int32_t pharaoh_overlay_crime_height(const PharaohOverlayMetricContext *c,
    const PharaohOverlayBuilding *b)
{
    if (b->house_size == 0u || c->crime_height_excluded(c->user_data, b)) return 0;
    return b->crime_risk / 100;
}

int32_t pharaoh_overlay_labor_height(const PharaohOverlayBuilding *b)
{
    return b->house_size != 0u ? 0 : ((b->houses_covered * 100) / 300) / 10;
}

int32_t pharaoh_overlay_religion_height(const PharaohOverlayMetricContext *c,
    const PharaohOverlayBuilding *b)
{
    int32_t god, kind, count = 0;
    if (b->house_size == 0u || c->religion_height_excluded(c->user_data, b)) return 0;
    for (god = 0; god < c->god_count; ++god) for (kind = 0; kind < 3; ++kind)
        count += c->active_building_count(c->user_data,
            c->building_type_for_god(c->user_data, god, kind));
    if (c->oracle_total + count == 0) return 0;
    return (b->num_gods * 17) / 10;
}
