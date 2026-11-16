#include "pharaoh/simulation/industry_production.h"

int32_t pharaoh_industry_has_produced_resource(
    const PharaohIndustryProduction *building)
{
    int32_t required_progress = PHARAOH_INDUSTRY_PROGRESS_PER_INPUT;
    if (building->subtype != PHARAOH_INDUSTRY_INPUT_NONE)
        required_progress += PHARAOH_INDUSTRY_PROGRESS_PER_INPUT;
    return building->progress >= required_progress;
}

void pharaoh_industry_start_new_production(PharaohIndustryProduction *building)
{
    building->progress = 0;
    if (building->subtype != PHARAOH_INDUSTRY_INPUT_NONE &&
        building->loads_stored != 0) {
        building->loads_stored = (int16_t)(building->loads_stored -
            PHARAOH_INDUSTRY_INPUT_LOAD);
    }
    if (building->second_input_resource != PHARAOH_INDUSTRY_INPUT_NONE &&
        building->second_input_stock != 0) {
        int32_t load = building->type == PHARAOH_INDUSTRY_TYPE_BRICKWORKS ?
            PHARAOH_INDUSTRY_STRAW_LOAD : PHARAOH_INDUSTRY_INPUT_LOAD;
        building->second_input_stock = (int16_t)(building->second_input_stock - load);
    }
}
