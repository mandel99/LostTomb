#ifndef PHARAOH_MILITARY_REQUEST_DISPATCH_H
#define PHARAOH_MILITARY_REQUEST_DISPATCH_H
#include "pharaoh/military/legion_requests.h"
#include "pharaoh/military/transport_boats.h"
#include "pharaoh/commerce/empire_queries.h"
#include "pharaoh/events/event_queries.h"

typedef struct PharaohMilitaryRequestDispatchContext {
    PharaohLegionRequestContext *legions; /* actual50x144 and2000x388 pools */
    PharaohScenarioEvent *events;
    const int32_t *event_count;
    const PharaohEmpireRouteRecord *routes;
    const uint32_t *current_player; /* full DWORD EAAC28 */
    const int16_t *city_x,*city_y; /* EA3FF0/EA3FF2, actual selected bank */
    int16_t *slot_walkers; /* D04E58 */
} PharaohMilitaryRequestDispatchContext;

/* Complete004B84B0. The child contexts borrow the same native pools and
   current-walker scratch. Their actual C children do not change the player. */
void pharaoh_formation_legions_dispatch_to_request(
    const PharaohMilitaryRequestDispatchContext *,int32_t event_id);
#endif
