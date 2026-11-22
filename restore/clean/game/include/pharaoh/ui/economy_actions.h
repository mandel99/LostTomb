#ifndef PHARAOH_UI_ECONOMY_ACTIONS_H
#define PHARAOH_UI_ECONOMY_ACTIONS_H

#include <stdint.h>

enum { PHARAOH_ECONOMY_RESOURCE_CAPACITY = 64 };

typedef struct PharaohEconomyPlayer {
    int32_t tax_rate;
    uint8_t trade_filter;
    uint16_t trade_state[PHARAOH_ECONOMY_RESOURCE_CAPACITY];
    uint16_t trade_limit[PHARAOH_ECONOMY_RESOURCE_CAPACITY];
    uint32_t trade_first;
    int32_t treasury;
    int32_t donation_amount;
    int32_t donated_total;
    int32_t finance_total;
    uint32_t selected_resource;
    uint32_t selected_amount;
    int32_t stock[PHARAOH_ECONOMY_RESOURCE_CAPACITY];
} PharaohEconomyPlayer;

typedef struct PharaohEconomyState {
    PharaohEconomyPlayer *players;
    uint32_t current_player;
    uint32_t button_parameter;
    uint32_t selected_trade_resource;
    uint32_t trade_direction;
    uint32_t selection_marker;
    uint8_t current_screen;
    uint8_t redraw_requested;
    uint8_t amount_down_enabled;
    const uint32_t *trade_resources;
    const int32_t *resource_floor;
    const int32_t *resource_ceiling;
    uint32_t (*can_export)(void *user_data, int32_t resource);
    uint32_t (*can_import)(void *user_data, int32_t resource);
    int32_t (*clamp_trade_limit)(void *user_data, uint32_t resource,
        int32_t value);
    void (*enter_trade_parent)(void *user_data);
    void (*refresh_tax_projection)(void *user_data);
    void (*refresh_finance_totals)(void *user_data);
    void (*refresh_finance_allowance)(void *user_data);
    void (*show_message)(void *user_data, int32_t message, int32_t value,
        uint8_t templated);
    void *user_data;
} PharaohEconomyState;

uint32_t pharaoh_economy_validate_trade_mode(PharaohEconomyState *state,
    uint32_t resource);
void pharaoh_economy_trade_filter(PharaohEconomyState *state);
void pharaoh_economy_trade_back(PharaohEconomyState *state);
void pharaoh_economy_trade_select(PharaohEconomyState *state);
void pharaoh_economy_trade_direction(PharaohEconomyState *state);
void pharaoh_economy_tax_rate(PharaohEconomyState *state);
void pharaoh_economy_trade_help(PharaohEconomyState *state);
void pharaoh_economy_trade_limit(PharaohEconomyState *state);
void pharaoh_economy_donate(PharaohEconomyState *state);
void pharaoh_economy_donation_amount(PharaohEconomyState *state);
void pharaoh_economy_resource_amount_up(PharaohEconomyState *state);
void pharaoh_economy_resource_amount_down(PharaohEconomyState *state);

int32_t pharaoh_city_resource_cap_amount(uint32_t resource, int32_t value);

#endif
