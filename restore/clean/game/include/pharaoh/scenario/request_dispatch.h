#ifndef PHARAOH_SCENARIO_REQUEST_DISPATCH_H
#define PHARAOH_SCENARIO_REQUEST_DISPATCH_H
#include "pharaoh/scenario/request_travel.h"
#include "pharaoh/military/request_dispatch.h"
#include "pharaoh/commerce/storage_withdraw.h"

/* Genuine contiguous D3B1C4..D3B387 storage. Unused bytes stay opaque. */
typedef struct PharaohRequestDispatchRandom {
    int32_t history_index;
    uint32_t history[100];
    uint8_t opaque_194[12];
    uint32_t first_15,first_7,first_3,second_15,second_7,second_3;
    uint8_t opaque_1b8[4];
    uint32_t seed_one,seed_two;
} PharaohRequestDispatchRandom;

typedef struct PharaohRequestDispatchContext {
    PharaohScenarioEvent *events;
    const int32_t *event_count;
    PharaohRequestDispatchRandom *random;
    uint32_t *treasury,*request_expenses; /* EA187C/EA5FC8, selected city */
    const PharaohRequestTravelContext *travel;
    const PharaohMilitaryRequestDispatchContext *military;
    PharaohStorageWithdrawContext *storage;
} PharaohRequestDispatchContext;

/* Complete0044C1A0. All children use real, shared event/player storage. */
void pharaoh_scenario_request_dispatch(const PharaohRequestDispatchContext *,
    int32_t event_id);
#endif
