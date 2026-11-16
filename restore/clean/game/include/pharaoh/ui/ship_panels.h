#ifndef PHARAOH_UI_SHIP_PANELS_H
#define PHARAOH_UI_SHIP_PANELS_H
#include <stdbool.h>
#include <stdint.h>
typedef struct PharaohShipPanelRules {
    int32_t ferry_no_landing, ferry_no_road, ferry_no_workers;
    int32_t fishing_out, fishing_active, fishing_back, fishing_catch;
    int32_t fishing_resting, fishing_polluted, fishing_no_ground;
    int32_t shipwright_no_demand, shipwright_needs_wood, shipwright_building;
    int32_t shipwright_repair_needs_wood, shipwright_repairing;
    int32_t order_kind_mask, order_for_city, order_repair, order_type_mask;
    int32_t fishing_boat_type, wood_per_boat, progress_max;
    int32_t dock_staff_few, dock_staff_most;
    int32_t order_none, order_kind_for_city, order_kind_repair;
    int32_t fishing_waiting_for_boat, fishing_halted;
    int32_t fishing_action_out, fishing_action_fishing, fishing_action_back;
    int32_t fishing_action_returning, fishing_action_docked;
} PharaohShipPanelRules;
typedef struct PharaohShipPanelState {
    int32_t linked_building_id, linked_workers;
    bool linked_building_has_road;
    int32_t fishing_action, sweep_countdown;
    bool fish_mothballed;
    int32_t boat_order, boat_order_type, needed_boat_type;
    bool boat_needed;
    int32_t timber_stored, shipyard_progress;
    int32_t dock_ship_id, worker_percentage;
} PharaohShipPanelState;
int32_t pharaoh_ship_ferry_status(const PharaohShipPanelState *,const PharaohShipPanelRules *);
int32_t pharaoh_ship_fishing_status(const PharaohShipPanelState *,const PharaohShipPanelRules *);
int32_t pharaoh_shipwright_status(const PharaohShipPanelState *,const PharaohShipPanelRules *);
int32_t pharaoh_dock_staff_step(const PharaohShipPanelState *,const PharaohShipPanelRules *);
int32_t pharaoh_ship_dock_status(const PharaohShipPanelState *,const PharaohShipPanelRules *);
#endif
