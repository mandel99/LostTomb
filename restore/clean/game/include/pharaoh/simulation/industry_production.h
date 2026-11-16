#ifndef PHARAOH_SIMULATION_INDUSTRY_PRODUCTION_H
#define PHARAOH_SIMULATION_INDUSTRY_PRODUCTION_H

#include <stdbool.h>
#include <stdint.h>

typedef struct PharaohIndustryProduction {
    int32_t type;
    int16_t subtype;
    int16_t loads_stored;
    int16_t second_input_resource;
    int16_t second_input_stock;
    int16_t progress;
} PharaohIndustryProduction;

enum {
    PHARAOH_INDUSTRY_INPUT_NONE = 0,
    PHARAOH_INDUSTRY_TYPE_BRICKWORKS = 204,
    PHARAOH_INDUSTRY_INPUT_LOAD = 100,
    PHARAOH_INDUSTRY_STRAW_LOAD = 25,
    PHARAOH_INDUSTRY_PROGRESS_PER_INPUT = 200
};

int32_t pharaoh_industry_has_produced_resource(
    const PharaohIndustryProduction *building);
void pharaoh_industry_start_new_production(
    PharaohIndustryProduction *building);

#endif
