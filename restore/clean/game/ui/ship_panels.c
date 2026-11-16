#include "pharaoh/ui/ship_panels.h"
enum {
    PHARAOH_BOAT_ORDER_TYPE_MASK = 0x0fff,
    PHARAOH_BOAT_ORDER_FOR_CITY = 0x1000,
    PHARAOH_BOAT_ORDER_REPAIR = 0x2000,
    PHARAOH_BOAT_ORDER_KIND_MASK = 0xf000,
    PHARAOH_FISHING_ACTION_OUT = 9,
    PHARAOH_FISHING_ACTION_FISHING = 10,
    PHARAOH_FISHING_ACTION_BACK = 11,
    PHARAOH_FISHING_ACTION_DOCKED = 12,
    PHARAOH_FISHING_ACTION_RETURNING = 13,
    PHARAOH_WALKER_FISHING_BOAT = 25,
    PHARAOH_DOCK_TEXT_SERVICING = 2,
    PHARAOH_DOCK_TEXT_WAITING = 6
};
int32_t pharaoh_ship_ferry_status(const PharaohShipPanelState *s,const PharaohShipPanelRules *r)
{
    if(s->linked_building_id<1)return r->ferry_no_landing;
    if(!s->linked_building_has_road)return r->ferry_no_road;
    return s->linked_workers!=0 ? -1 : r->ferry_no_workers;
}
int32_t pharaoh_ship_fishing_status(const PharaohShipPanelState *s,const PharaohShipPanelRules *r)
{
    if(s->fish_mothballed)return r->fishing_halted;
    if(s->fishing_action<PHARAOH_FISHING_ACTION_OUT)return r->fishing_waiting_for_boat;
    if(s->fishing_action==r->fishing_action_out)return r->fishing_out;
    if(s->fishing_action==r->fishing_action_fishing)return r->fishing_active;
    if(s->fishing_action==r->fishing_action_back)return r->fishing_back;
    if(s->fishing_action==r->fishing_action_returning)return r->fishing_catch;
    if(s->fishing_action==r->fishing_action_docked)return s->sweep_countdown<1?r->fishing_resting:r->fishing_polluted;
    return r->fishing_no_ground;
}
int32_t pharaoh_shipwright_status(const PharaohShipPanelState *s,const PharaohShipPanelRules *r)
{
    if((s->boat_order&r->order_kind_mask)==r->order_none){
        if(!s->boat_needed)return r->shipwright_no_demand;
        return s->timber_stored<r->wood_per_boat&&s->needed_boat_type!=r->fishing_boat_type?r->shipwright_needs_wood:-1;
    }
    if((s->boat_order&PHARAOH_BOAT_ORDER_KIND_MASK)==PHARAOH_BOAT_ORDER_FOR_CITY){
        if(s->timber_stored<r->wood_per_boat&&(s->boat_order&PHARAOH_BOAT_ORDER_TYPE_MASK)!=PHARAOH_WALKER_FISHING_BOAT)return r->shipwright_needs_wood;
        return r->shipwright_building;
    }
    if((s->boat_order&PHARAOH_BOAT_ORDER_KIND_MASK)==PHARAOH_BOAT_ORDER_REPAIR)
        return s->timber_stored<1?r->shipwright_repair_needs_wood:r->shipwright_repairing;
    return -1;
}
int32_t pharaoh_ship_dock_status(const PharaohShipPanelState *s,const PharaohShipPanelRules *r)
{ return (s->dock_ship_id==0?PHARAOH_DOCK_TEXT_WAITING:PHARAOH_DOCK_TEXT_SERVICING)+pharaoh_dock_staff_step(s,r); }
int32_t pharaoh_dock_staff_step(const PharaohShipPanelState *s,const PharaohShipPanelRules *r)
{
    if(s->worker_percentage<1)return 0;
    if(s->worker_percentage<r->dock_staff_few)return 1;
    return s->worker_percentage<r->dock_staff_most?2:3;
}
