#include "pharaoh/military/transport_boats.h"

static PharaohTransportWalker *walker(PharaohTransportContext *c, int32_t id)
{ return id > 0 && (size_t)id < c->walker_count ? &c->walkers[id] : 0; }
static PharaohTransportFormation *formation(PharaohTransportContext *c, int32_t id)
{ return id > 0 && (size_t)id < c->formation_count ? &c->formations[id] : 0; }

int32_t pharaoh_transport_legion_ready(PharaohTransportContext *c, int32_t walker_id)
{
    PharaohTransportWalker *ship = walker(c, walker_id);
    PharaohTransportFormation *legion;
    int32_t i, ready = 0;
    if (!ship || ship->ship_formation_id <= 0) return 0;
    legion = formation(c, ship->ship_formation_id);
    if (!legion) return 0;
    for (i = 0; i < legion->num_walkers; ++i) {
        PharaohTransportWalker *soldier;
        if (legion->walkers[i] == 0) break;
        soldier = walker(c, legion->walkers[i]);
        if (soldier && soldier->action_state != c->rules->action_corpse &&
            soldier->direction == c->rules->direction_destination) ++ready;
    }
    return ready == legion->num_walkers ? ship->ship_formation_id : 0;
}

void pharaoh_transport_legion_board(PharaohTransportContext *c, int32_t formation_id)
{
    PharaohTransportFormation *legion = formation(c, formation_id);
    int32_t i;
    if (!legion) return;
    legion->is_at_fort = 0;
    for (i = 0; i < c->rules->formation_max_walkers; ++i) {
        int32_t id = legion->walkers[i];
        PharaohTransportWalker *soldier;
        if (id == 0) return;
        soldier = walker(c, id);
        if (soldier && soldier->action_state != c->rules->action_corpse) {
            soldier->alternative_location_index = 0;
            soldier->action_before_attack = soldier->action_state;
            soldier->action_state = c->rules->action_on_ship;
            c->route_remove(c->user_data, id);
            c->map_walker_delete(c->user_data, id);
        }
    }
}

void pharaoh_transport_legion_land(PharaohTransportContext *c,
    int32_t formation_id, int32_t x, int32_t y)
{
    PharaohTransportFormation *legion = formation(c, formation_id);
    int32_t i;
    if (!legion) return;
    for (i = 0; i < c->rules->formation_max_walkers; ++i) {
        int32_t id = legion->walkers[i];
        PharaohTransportWalker *soldier;
        if (id == 0) return;
        soldier = walker(c, id);
        if (soldier && soldier->action_state != c->rules->action_corpse) {
            soldier->grid_offset = c->grid_offset(c->user_data, x, y);
            soldier->previous_x = soldier->x = (int16_t)x;
            soldier->previous_y = soldier->y = (int16_t)y;
            soldier->action_state = soldier->action_before_attack;
            soldier->cross_x = soldier->x * 15;
            soldier->cross_y = soldier->y * 15;
            c->map_walker_add(c->user_data, id);
        }
    }
}

void pharaoh_transport_sink(PharaohTransportContext *c, int32_t walker_id)
{
    PharaohTransportWalker *ship;
    c->handle_corpse(c->user_data, walker_id);
    ship = walker(c, walker_id);
    if (!ship || ship->wait_ticks != c->rules->drown_tick) return;
    if (ship->troops_on_board) {
        PharaohTransportFormation *legion = formation(c, ship->ship_formation_id);
        int32_t i;
        if (legion) {
            legion->ship_walker_id = 0;
            for (i = 0; i < c->rules->formation_max_walkers; ++i) {
                PharaohTransportWalker *soldier;
                int32_t id = legion->walkers[i];
                if (id == 0) break;
                soldier = walker(c, id);
                if (soldier) soldier->state = (uint8_t)c->rules->walker_state_dead;
            }
        }
    }
    c->calculate_formations(c->user_data);
}

static int32_t move(PharaohTransportContext *c, int32_t id,
    PharaohTransportWalker **ship)
{
    *ship = walker(c, id);
    if (!*ship) return 0;
    c->move_ticks(c->user_data, id, c->walker_speed(c->user_data, (*ship)->type));
    return (*ship)->direction;
}

void pharaoh_transport_sail_to_dock(PharaohTransportContext *c, int32_t id,
    int32_t *moving)
{
    PharaohTransportWalker *ship;
    int32_t direction = move(c, id, &ship);
    if (!ship) return;
    if (direction == c->rules->direction_destination) c->arrived_at_dock(c->user_data, id, moving);
    else if (direction == c->rules->direction_reroute || direction == c->rules->direction_lost) c->route_remove(c->user_data, id);
}

void pharaoh_transport_sail_to_legion(PharaohTransportContext *c, int32_t id,
    int32_t *moving)
{
    PharaohTransportWalker *ship;
    int32_t direction = move(c, id, &ship);
    if (!ship) return;
    if (direction == c->rules->direction_destination) {
        int32_t formation_id;
        *moving = 0;
        formation_id = pharaoh_transport_legion_ready(c, id);
        if (formation_id == 0) return;
        if (formation_id == ship->ship_formation_id) {
            PharaohTransportFormation *legion = formation(c, formation_id);
            if (legion && legion->ship_walker_id == 0) {
                ship->troops_on_board = 1;
                legion->order_x = ship->x; legion->order_y = ship->y;
                pharaoh_transport_legion_board(c, formation_id);
                ship->action_state = c->rules->action_sailing;
                legion->ship_walker_id = id; ship->wait_ticks = 0; return;
            }
            ship->action_state = c->rules->action_going_to_legion;
            ship->ship_formation_id = 0;
        }
        ship->wait_ticks = 0;
    } else if (direction == c->rules->direction_reroute) c->route_remove(c->user_data, id);
    else if (direction == c->rules->direction_lost) {
        c->route_remove(c->user_data, id); ship->action_state = c->rules->action_evading;
    }
}

void pharaoh_transport_sail_to_landing(PharaohTransportContext *c, int32_t id,
    int32_t *moving)
{
    PharaohTransportWalker *ship;
    int32_t direction = move(c, id, &ship), formation_id;
    if (!ship) return;
    formation_id = ship->ship_formation_id;
    if (direction == c->rules->direction_destination) {
        PharaohTransportFormation *legion;
        int32_t x = id, y = 0;
        *moving = 0;
        if (ship->landing_x == 0 && ship->landing_y == 0) return;
        c->water_landing_point(c->user_data, ship->landing_x, ship->landing_y,
            ship->x, ship->y, &x, &y);
        c->nearest_free_land(c->user_data, &x, &y);
        legion = formation(c, formation_id);
        if (!legion) return;
        legion->order_x = legion->x = (int16_t)x;
        legion->order_y = legion->y = (int16_t)y;
        legion->ship_walker_id = 0;
        pharaoh_transport_legion_land(c, formation_id, x, y);
        ship->troops_on_board = 0; ship->ship_formation_id = 0;
        c->legion_move_to(c->user_data, ship->landing_x, ship->landing_y, formation_id, 0);
        ship->action_state = c->rules->action_sailing;
        ship->landing_x = ship->landing_y = 0;
    } else if (direction == c->rules->direction_reroute) c->route_remove(c->user_data, id);
    else if (direction == c->rules->direction_lost) {
        c->route_remove(c->user_data, id); ship->action_state = c->rules->action_evading;
    }
}

void pharaoh_transport_docked(PharaohTransportContext *c, int32_t id)
{
    PharaohTransportWalker *ship = walker(c, id);
    if (ship && ++ship->wait_ticks > c->rules->face_enemy_wait)
        c->face_enemy(c->user_data, id);
}

void pharaoh_transport_enemy_move(PharaohTransportContext *c, int32_t id,
    int32_t *moving)
{
    PharaohTransportWalker *ship = walker(c, id);
    if (!ship || !moving || !c->rules || !c->walker_speed || !c->move_ticks) return;
    c->move_ticks(c->user_data, id, c->walker_speed(c->user_data, ship->type));
    if (ship->direction == c->rules->direction_reroute) {
        if (c->route_remove) c->route_remove(c->user_data, id);
    } else if (ship->direction == c->rules->direction_lost) {
        ship->action_state = c->rules->action_corpse;
    } else if (ship->direction == c->rules->direction_destination) {
        *moving = 0;
    }
}

int32_t pharaoh_formation_legion_is_on_ship(
    const int16_t formation_walkers[16],
    const PharaohWalkerQueryRecord *walkers)
{
    int32_t index;
    for (index = 0; index < 16; ++index) {
        const int32_t walker_id = formation_walkers[index];
        if (walker_id == 0) return 0;
        if (walkers[walker_id].action_state == 0x12) return 1;
    }
    return 0;
}
