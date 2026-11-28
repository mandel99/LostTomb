#ifndef PHARAOH_MILITARY_LEGION_ORDERS_H
#define PHARAOH_MILITARY_LEGION_ORDERS_H

#include <stdint.h>

enum { PHARAOH_LEGION_WALKER_COUNT = 16 };

enum { PHARAOH_NATIVE_LEGION_COUNT=49 };

typedef struct PharaohNativeLegionRecord {
    uint8_t state;
    uint8_t reserved_001[2];
    uint8_t deployment_blocked;
    int16_t strength_class;
    uint8_t reserved_006[2];
    int16_t member_ids[PHARAOH_LEGION_WALKER_COUNT];
    uint8_t member_count;
    uint8_t maximum_walkers;
    int16_t movement_mode;
    union { int16_t months_deployed,morale; };
    int16_t x;
    int16_t y;
    int16_t order_x;
    int16_t order_y;
    int16_t home_x;
    int16_t home_y;
    int16_t destination_x;
    int16_t destination_y;
    int16_t destination_building_id;
    int16_t standard_walker_id;
    uint8_t kind;
    uint8_t reserved_043[3];
    int16_t recruit_type;
    union { uint8_t reserved_048[8];
        struct { int16_t morale_leader_id; uint8_t reserved_04a[6]; }; };
    int16_t recent_fight;
    uint8_t reserved_052[0x0a];
    int16_t missile_fired;
    int16_t missile_attack_timeout;
    int16_t missile_attack_formation_id;
    int16_t previous_movement_mode;
    int16_t cursed_by_seth;
    uint8_t months_low_morale;
    uint8_t empire_service;
    uint8_t ticks_without_target;
    uint8_t herd_kind;
    uint8_t enemy_type;
    uint8_t direction;
    int16_t previous_x;
    int16_t previous_y;
    uint8_t direction_hold_ticks;
    uint8_t orientation;
    uint8_t months_from_home;
    uint8_t months_very_low_morale;
    uint8_t reserved_074[6];
    int16_t strength;
    uint8_t reserved_07c[6];
    int16_t deployment_status;
    uint8_t reserved_084[0x0c];
} PharaohNativeLegionRecord;

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
    uint32_t secondary_button_parameter;
    uint8_t command_locked;
    uint8_t sidebar_update_code;
    uint8_t redraw_requested;
} PharaohLegionOrderState;

typedef struct PharaohLegionOrderFormationFields {
    const volatile int16_t *kind,*walkers,*leader_id;
    volatile int16_t *order,*morale,*previous_order,*resume_order;
    const volatile int16_t *field_126,*availability;
    const volatile uint8_t *order_mode,*subtype,*disabled,*alternate_order;
    volatile uint32_t *task;
} PharaohLegionOrderFormationFields;

typedef struct PharaohLegionOrderWalkerFields {
    uint8_t *base;
    size_t stride,state,animation_frame,x,y,action_state,route,target_x,target_y;
} PharaohLegionOrderWalkerFields;

typedef struct PharaohLegionOrderBorrowedState {
    uint32_t formation_id;
    PharaohLegionOrderFormationFields formation;
    PharaohLegionOrderWalkerFields walkers;
    const volatile uint32_t *button_parameter,*current_walker_id;
    volatile uint32_t *active_formation_id;
    const volatile uint8_t *command_locked;
    volatile uint8_t *sidebar_update_code,*redraw_requested;
} PharaohLegionOrderBorrowedState;

void pharaoh_legion_order_button_borrowed(const PharaohLegionOrderBorrowedState *state);

/* Concrete field references for the one407110 algorithm. chosen_order and
 * alternate_order belong to the selected formation; previous_button borrows
 * the formation identified by the actual entry BCB9C0 DWORD. It is required
 * only when secondary_button_parameter is1. No formation/walker pool copies. */
typedef struct PharaohLegionToggleBorrowedState {
    uint32_t previous_formation_id;
    const volatile uint8_t *command_locked;
    volatile uint8_t *alternate_order;
    const volatile int16_t *chosen_order;
    volatile uint32_t *button_parameter,*selected_formation_id;
    const volatile uint32_t *secondary_button_parameter;
    const PharaohLegionOrderBorrowedState *previous_button;
} PharaohLegionToggleBorrowedState;

void pharaoh_legion_order_toggle_borrowed(const PharaohLegionToggleBorrowedState *state);

void pharaoh_legion_order_toggle(PharaohLegionOrderState *state);


/* Shared real Native layout provider004B7B50; logical model API retained. */
void pharaoh_formation_change_morale_native(PharaohNativeLegionRecord *formations,
    int32_t formation_id,int16_t delta);

/* Complete C translations of Pharaoh.exe 0x004B7B50 and 0x00406720. */
void pharaoh_formation_change_morale(PharaohLegionOrderState *state,
    int32_t formation_id, int16_t delta);
void pharaoh_legion_order_button(PharaohLegionOrderState *state);

#endif
