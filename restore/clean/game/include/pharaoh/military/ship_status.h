#ifndef PHARAOH_MILITARY_SHIP_STATUS_H
#define PHARAOH_MILITARY_SHIP_STATUS_H
#include <stddef.h>
#include <stdint.h>
typedef struct PharaohShipStatus {
    int32_t hit_points;
    int32_t maximum_hit_points;
    int32_t movement_mode;
} PharaohShipStatus;
typedef struct PharaohShipStatusRules {
    int32_t hull_thresholds[4];
    int32_t weak_text;
    int32_t very_strong_text;
    int32_t rested_text;
    int32_t tired_text;
    int32_t exhausted_text;
} PharaohShipStatusRules;
int32_t pharaoh_ship_damage_text(const PharaohShipStatus *, const PharaohShipStatusRules *);
int32_t pharaoh_ship_crew_text(const PharaohShipStatus *, const PharaohShipStatusRules *);
#endif
