#ifndef PHARAOH_BUILDING_CONSTRUCTION_WARNINGS_H
#define PHARAOH_BUILDING_CONSTRUCTION_WARNINGS_H

#include <stdint.h>

enum { PHARAOH_WARNING_RESOURCE_CAPACITY = 64 };

typedef struct PharaohConstructionWarningState {
    uint8_t editor_active;
    uint8_t mission_id;
    int32_t scenario_mode;
    int32_t population;
    int32_t unemployment_percentage;
    int32_t labor_available;
    int32_t labor_needed;
    int32_t industry_active[PHARAOH_WARNING_RESOURCE_CAPACITY];
    int32_t stored[PHARAOH_WARNING_RESOURCE_CAPACITY];
    int16_t trade_status[PHARAOH_WARNING_RESOURCE_CAPACITY];
    int32_t (*can_produce)(void *user_data, int32_t resource);
    int32_t (*can_import)(void *user_data, int32_t resource);
    void (*show_warning)(void *user_data, int32_t message,
        int32_t value, int32_t extra);
    void *user_data;
} PharaohConstructionWarningState;

void pharaoh_warning_brewery(PharaohConstructionWarningState *, int32_t);
void pharaoh_warning_weaver(PharaohConstructionWarningState *, int32_t);
void pharaoh_warning_jeweler(PharaohConstructionWarningState *, int32_t);
void pharaoh_warning_potter(PharaohConstructionWarningState *, int32_t);
void pharaoh_warning_papyrus(PharaohConstructionWarningState *, int32_t);
void pharaoh_warning_paint(PharaohConstructionWarningState *, int32_t);
void pharaoh_warning_cattle(PharaohConstructionWarningState *, int32_t);
void pharaoh_warning_carpenters(PharaohConstructionWarningState *, int32_t);

void pharaoh_warning_unemployment(PharaohConstructionWarningState *);
void pharaoh_warning_labor_need(PharaohConstructionWarningState *);

#endif
