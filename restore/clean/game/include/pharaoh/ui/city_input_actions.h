#ifndef PHARAOH_UI_CITY_INPUT_ACTIONS_H
#define PHARAOH_UI_CITY_INPUT_ACTIONS_H
#include "pharaoh/rendering/minimap.h"
#include "pharaoh/ui/city_chrome_draw.h"
#include "pharaoh/ui/image_button_input.h"
#include "pharaoh/ui/city_message_queue.h"
#include "pharaoh/ui/transport_cursor.h"
#include "pharaoh/military/formation_counts.h"

/* Native0043EF10: all minimap scratch fields alias the draw owner's fields;
   mouse coordinates are actual E38E34/E38E30 slots. */
uint32_t pharaoh_minimap_mouse_grid(PharaohMinimap *,
    const int32_t *mouse_x,const int32_t *mouse_y,
    int32_t x,int32_t y,int32_t width,int32_t height);

typedef struct PharaohMinimapClickContext {
    PharaohMinimap *minimap;
    PharaohCityViewContext *view;
    const int32_t *mouse_x,*mouse_y,*hovered,*sidebar_x;
    const uint8_t *left_down,*right_went_down;
    const uint32_t *editor_active; /*605A3C*/
    int32_t *focus_x,*focus_y; /* actual00430CD0 output scratch */
} PharaohMinimapClickContext;
uint32_t pharaoh_minimap_sidebar_click(const PharaohMinimapClickContext *);

typedef struct PharaohCityRotateInputContext {
    PharaohCityChromeDrawContext *chrome;
    PharaohImageButtonInputState *input; /* actual shared cursor/parameter slots */
    const int16_t *trailing_trigger; /* actual adjacent storage used by00425990 */
    uint32_t *tooltip,*selected_mode; /*7056B8/705698*/
    uint8_t *redraw; /*E38DC9*/
} PharaohCityRotateInputContext;
uint8_t pharaoh_city_rotate_button_input(const PharaohCityRotateInputContext *);
void pharaoh_city_top_menu_help(const int32_t *hovered,
    const PharaohCityMessageQueueContext *messages,uint8_t *redraw);

typedef struct PharaohCitySelectionContext {
    uint8_t *current_window,*redraw;
    const int32_t *construction_tool,*selected_grid,*cursor_x,*cursor_y,*grid_start;
    const int32_t *direction_deltas; /* eight actual entries */
    const int16_t *walker_heads;
    const PharaohWalkerQueryRecord *walkers;
    const PharaohTransportCursorGrid *legion_grid; /* same heads/walker pool */
    const PharaohFormationCountRecord *formations; /* BCB9C8,50x144 */
    int32_t *selected_legion; /*BCB9C0*/
    int32_t *selected_warship,*warship_active; /*D05704/0C*/
    int32_t *selected_transport,*transport_active; /*D05700/08*/
} PharaohCitySelectionContext;

/* Complete0040B630/0040B6D0/0040B790. Borrow actual pools and shared fields.
   Native IDs/list/cell domains required; no invented bounds/cycle guards. */
uint32_t pharaoh_city_select_legion(const PharaohCitySelectionContext *);
uint32_t pharaoh_city_select_warship_live(const PharaohCitySelectionContext *);
uint32_t pharaoh_city_select_transport_live(const PharaohCitySelectionContext *);
#endif
