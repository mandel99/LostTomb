#ifndef PHARAOH_BUILDING_MODEL_TYPES_H
#define PHARAOH_BUILDING_MODEL_TYPES_H

#include <stddef.h>
#include <stdint.h>

enum {
    PHARAOH_BUILDING_MODEL_RECORD_COUNT = 237,
    PHARAOH_HOUSE_MODEL_RECORD_COUNT = 20,
    PHARAOH_BUILDING_MODEL_FIELD_COUNT = 10,
    PHARAOH_HOUSE_MODEL_FIELD_COUNT = 22
};

typedef struct PharaohBuildingModelRecord {
    int32_t cost;
    int32_t desirability_value;
    int32_t desirability_step;
    int32_t desirability_step_size;
    int32_t desirability_range;
    int32_t laborers;
    int32_t fire_risk_increment;
    int32_t damage_risk_increment;
    int32_t walker_cost;
    int32_t service_reduction;
} PharaohBuildingModelRecord;

typedef struct PharaohHouseModel {
    int32_t devolve_desirability;
    int32_t evolve_desirability;
    int32_t entertainment;
    int32_t water;
    int32_t religion;
    int32_t education;
    int32_t market;
    int32_t dentist;
    int32_t courthouse;
    int32_t health;
    int32_t food_types;
    int32_t pottery;
    int32_t linen;
    int32_t jewelry;
    int32_t beer;
    int32_t crime_growth;
    int32_t crime_base;
    int32_t prosperity;
    int32_t max_people;
    int32_t tax_multiplier;
    int32_t malaria_risk;
    int32_t disease_risk;
} PharaohHouseModel;

typedef PharaohBuildingModelRecord PharaohBuildingModelTable[
    PHARAOH_BUILDING_MODEL_RECORD_COUNT];
typedef PharaohHouseModel PharaohHouseModelTable[PHARAOH_HOUSE_MODEL_RECORD_COUNT];

_Static_assert(sizeof(PharaohBuildingModelRecord) == 0x28,
    "native building model row has ten 32-bit fields");
_Static_assert(offsetof(PharaohBuildingModelRecord, service_reduction) == 0x24,
    "native building model field order");
_Static_assert(sizeof(PharaohHouseModel) == 0x58,
    "native house model row has twenty-two 32-bit fields");
_Static_assert(offsetof(PharaohHouseModel, disease_risk) == 0x54,
    "native house model field order");

#endif
