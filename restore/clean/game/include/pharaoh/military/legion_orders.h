#ifndef PHARAOH_MILITARY_LEGION_ORDERS_H
#define PHARAOH_MILITARY_LEGION_ORDERS_H

#include <stdint.h>

enum { PHARAOH_LEGION_WALKER_COUNT = 16 };

typedef struct PharaohLegionOrderFormation {
    int16_t kind;
    int16_t walkers[PHARAOH_LEGION_WALKER_COUNT];
    int16_t order;
    int16_t morale;
    int16_t leader_id;
    int16_t previous_order;
    uint8_t order_mode;
    uint8_t subtype;
    uint8_t disabled;
    uint8_t alternate_order;
    int16_t resume_order;
    int16_t field_126;
    int16_t availability;
    uint32_t task;
} PharaohLegionOrderFormation;

typedef struct PharaohLegionOrderWalker {
    uint8_t state;
    uint8_t animation_frame;
    int16_t x;
    int16_t y;
    uint8_t action_state;
    uint32_t route;
    int32_t target_x;
    int32_t target_y;
} PharaohLegionOrderWalker;

typedef struct PharaohLegionOrderState {
    PharaohLegionOrderFormation *formations;
    PharaohLegionOrderWalker *walkers;
    uint32_t selected_formation_id;
    uint32_t button_parameter;
    uint32_t current_walker_id;
    uint32_t active_formation_id;
    uint8_t command_locked;
    uint8_t sidebar_update_code;
    uint8_t redraw_requested;
} PharaohLegionOrderState;

/* Complete C translations of Pharaoh.exe 0x004B7B50 and 0x00406720. */
void pharaoh_formation_change_morale(PharaohLegionOrderState *state,
    int32_t formation_id, int16_t delta);
void pharaoh_legion_order_button(PharaohLegionOrderState *state);

#endif
