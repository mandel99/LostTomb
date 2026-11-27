#ifndef PHARAOH_UI_BUILDING_INFO_LIVE_H
#define PHARAOH_UI_BUILDING_INFO_LIVE_H

#include "pharaoh/building/type_queries.h"
#include "pharaoh/commerce/storage_queries.h"
#include "pharaoh/military/legion_orders.h"
#include "pharaoh/ui/button_layout.h"
#include "pharaoh/ui/generic_button_focus.h"
#include "pharaoh/ui/image_button_input.h"
#include "pharaoh/ui/storage_order_foreground.h"
#include "pharaoh/walker/record_queries.h"

/* Actual EADCB8 state, including the shared advisor, walker and terrain views. */
typedef struct PharaohBuildingInfoLiveState {
    int32_t storage_state,reserved_bc,orders_shown;
    int32_t warship_id,transport_id,formation_id,has_academy;
    int32_t worst_desirability_building;
    union { int32_t worst_desirability_score,walker_index; }; /* EADCD8 */
    int32_t walker_phrase,reserved_e0;
    union { int32_t selected_walker_index,walker_list_offset; }; /* EADCE4 */
    uint32_t advisor_buttons[15];
    int32_t walker_ids[7],walker_count,details_shown,selection,terrain_kind;
    int32_t mode,help_id;
    struct {
        int32_t road_access,worker_percentage,canal,groundwater,rubble_type;
    } details;
    int32_t building_id,reserved_70[2],tile_y,tile_x,grid_offset;
    int32_t height_blocks,width_blocks,y,x,first_draw;
} PharaohBuildingInfoLiveState;

enum PharaohBuildingInfoLiveRow {
    PHARAOH_INFO_ROW_HOUSE, PHARAOH_INFO_ROW_GRANARY,
    PHARAOH_INFO_ROW_BAZAAR, PHARAOH_INFO_ROW_STORAGE,
    PHARAOH_INFO_ROW_BACK, PHARAOH_INFO_ROW_STORAGE_BACK,
    PHARAOH_INFO_ROW_POLICIES, PHARAOH_INFO_ROW_WALKERS,
    PHARAOH_INFO_ROW_PHRASE, PHARAOH_INFO_ROW_LEGION,
    PHARAOH_INFO_ROW_LEGION_RETURN, PHARAOH_INFO_ROW_TRANSPORT,
    PHARAOH_INFO_ROW_WARSHIP, PHARAOH_INFO_ROW_FESTIVAL,
    PHARAOH_INFO_ROW_SALARY, PHARAOH_INFO_ROW_COUNT
};
typedef struct PharaohBuildingInfoLiveButtonRow {
    PharaohGenericButton *buttons;
    const PharaohMenuRect *rectangles;
} PharaohBuildingInfoLiveButtonRow;

typedef struct PharaohBuildingInfoLiveImageRow {
    PharaohImageButtonRecord *buttons;
    int16_t trailing_trigger;
} PharaohBuildingInfoLiveImageRow;

/* Bind these boundaries to the complete canonical drawing providers, never
 * replacement artwork or no-op handlers. Coordinates retain native UI origins.
 * The image-input callbacks retain the actual trailing record trigger. */
typedef struct PharaohBuildingInfoLiveOperations {
    void *user_data;
    void (*border)(void *,int32_t,int32_t,int32_t,int32_t,int32_t,
        int32_t,int32_t,int32_t); /* 4D63D0 */
    void (*text)(void *,int32_t,int32_t,int32_t,int32_t,int32_t,
        int32_t,int32_t,int32_t); /* 4D62F0 */
    void (*centered)(void *,int32_t,int32_t,int32_t,int32_t,int32_t,
        int32_t,int32_t,int32_t,int32_t); /* 4D6250 */
    void (*outer_panel)(void *,int32_t,int32_t,int32_t,int32_t); /* 536820 */
    void (*inner_panel)(void *,int32_t,int32_t,int32_t,int32_t,
        int32_t,int32_t); /* 4D65B0 */
    void (*image)(void *,int32_t,int32_t,int32_t); /* 53E590 */
    void (*arrow_draw)(void *,int32_t,int32_t,PharaohSquareButtonRecord *,int32_t);
    void (*image_draw)(void *,int32_t,int32_t,PharaohImageButtonRecord *,int32_t);
    void (*city_background)(void *); /* 4DDDC0 */
    void (*play_sample)(void *,int32_t,int32_t,int32_t,int32_t,int32_t); /* 55F0E8 */
    void (*no_info)(void *); /* 4FA4A0, full provider required */
    void (*terrain)(void *); /* 4FA530 */
    void (*legion)(void *,int32_t); /* 5101B0 */
    void (*transport)(void *,int32_t); /* 50F320 */
    void (*warship)(void *,int32_t); /* 50FCA0 */
    void (*legion_foreground)(void *,int32_t); /* 510B00 */
    void (*mansion_foreground)(void *); /* 503E10 */
    void (*palace_foreground)(void *); /* 504630 */
    void (*temple)(void *,int32_t);
    void (*quarry)(void *,int32_t);
    /* Relocated native per-type draw catalogue. Required for a dispatched type;
     * NULL entries are only valid for native default/no-draw cases. */
    void (*const *building_background)(void *);
    void (*bazaar)(void *); /* 500A40 */
    void (*granary)(void *); /* 5014A0 */
    void (*storage_yard)(void *); /* 501FD0 */
    void (*advisor_input)(void *); /* 515C60 */
    void (*festival_input)(void *); /* 516020 */
    int32_t (*storage_scrollbar)(void *); /* 40D1B0 */
    void (*legion_order)(void *); /* 406720 */
} PharaohBuildingInfoLiveOperations;

typedef struct PharaohBuildingInfoLiveContext {
    PharaohBuildingInfoLiveState *window;
    PharaohBuildingTypeRecord *buildings; /* Actual7999D8, stride264 */
    PharaohWalkerQueryRecord *walkers; /* ActualBCD5F8, stride388 */
    PharaohNativeLegionRecord *formations; /* ActualBCB9C8, stride144 */
    PharaohBuildingStorageRecord *storage; /* Actual7900B8, stride196 */
    const int32_t *food_count,*foods,*resource_count,*resources; /* F53DD8/DC, E6C/E70 */
    const int32_t *display_mode;
    int32_t *focus_primary,*focus_secondary,*focus_building; /* 7056C8/C4/B8 */
    uint8_t *current_window,*redraw_requested;
    const uint8_t *right_released;
    int32_t *button_parameter,*text_width;
    const int32_t *resource_images,*panel_button_images;
    const int32_t *font_black,*font_green,*font_white,*font_red,*font_title;
    const uint8_t *tutorial_flags;
    PharaohBuildingInfoLiveImageRow *tutorial_buttons;
    PharaohBuildingInfoLiveImageRow help_close;
    PharaohImageButtonRecord *limit_buttons; /* actual60 counted records */
    int16_t limit_trailing_trigger;
    PharaohSquareButtonRecord *scroll_up,*scroll_down;
    PharaohSquareButtonInputState *square_input;
    PharaohImageButtonInputState *image_input;
    PharaohButtonHitState *image_focus;
    PharaohGenericButtonInputState *generic_input;
    PharaohGenericButtonFocusState *generic_focus;
    PharaohButtonLayoutState *button_layout;
    const PharaohButtonLayoutOperations *button_layout_operations;
    PharaohBuildingInfoLiveButtonRow rows[PHARAOH_INFO_ROW_COUNT];
    const uint8_t *phrase_counts;
    const int32_t *sound_channels; /* 20-byte native records, roam DWORD */
    const uint8_t *sound_enabled;
    const int32_t *sound_volume;
    const PharaohBuildingInfoLiveOperations *operations;
} PharaohBuildingInfoLiveContext;

typedef void (*PharaohBuildingInfoLiveProgress)(void *,int32_t);
typedef struct PharaohBuildingInfoLiveInitOperations {
    void *user_data;
    void (*reset_warning)(void *); /* 4E2AB0 */
    void (*rebuild_resources)(void *); /* 5596D0 */
    int32_t (*pyramid_from_complex)(void *,int32_t); /* 4F5D90 */
    int32_t (*building_main)(void *,int32_t); /* 475B50 */
    int32_t (*percentage)(void *,int32_t,int32_t); /* 4CDE60 */
    int32_t (*any_legion_with_soldiers)(void *);
    void (*house_worst_desirability)(void *);
    void (*house_evolve_text)(void *,int32_t); /* 454190 */
    int32_t (*road)(void *,int32_t,int32_t,int32_t);
    int32_t (*granary_road)(void *,int32_t,int32_t);
    int32_t (*nearby_road)(void *,int32_t,int32_t);
    int32_t (*temple_road)(void *,int32_t,int32_t,int32_t);
    void (*walker_message)(void *,int32_t); /* 53C100 */
    void (*advisor_buttons)(void *,uint32_t[15]); /* 515130 */
    /* pyramid,sun,library,AbuSimbel,Caesareum,lighthouse,mausoleum,4 tombs */
    PharaohBuildingInfoLiveProgress progress[11];
} PharaohBuildingInfoLiveInitOperations;

typedef struct PharaohBuildingInfoLiveInitContext {
    PharaohBuildingInfoLiveContext *ui;
    const PharaohBuildingModelRecord *models; /* Actual705EA4, stride40 */
    const int32_t *editor_active,*pending_warning_count;
    const int32_t *cursor_tile_x,*cursor_tile_y,*grid_start;
    const int16_t *building_ids,*walker_ids;
    const uint8_t *rubble_types,*canals,*bitfields;
    const uint16_t *sprites;
    const uint32_t *terrain;
    const int32_t *entry_flag,*exit_flag,*neighbours;
    const int32_t *sentry_request;
    const int32_t *display_height,*mouse_y,*panel_position_mode;
    const PharaohBuildingInfoLiveInitOperations *operations;
} PharaohBuildingInfoLiveInitContext;

/* Children must be genuine canonical providers. The general dispatchers need
 * the selected panel and command branches bound; storage order routes use the
 * complete functions below. Unbound branches are not a supported host domain. */
int32_t pharaoh_building_info_live_initialize(const PharaohBuildingInfoLiveInitContext *);
int32_t pharaoh_building_info_live_height_class(const PharaohBuildingInfoLiveInitContext *);
void pharaoh_building_info_live_draw_background(PharaohBuildingInfoLiveContext *);
void pharaoh_building_info_live_draw_foreground(PharaohBuildingInfoLiveContext *);
void pharaoh_building_info_live_handle_input(PharaohBuildingInfoLiveContext *);
void pharaoh_building_info_live_toggle_resource(PharaohBuildingInfoLiveContext *,uint32_t ordinal);
void pharaoh_building_info_live_draw_bazaar_orders(PharaohBuildingInfoLiveContext *);
void pharaoh_building_info_live_draw_granary_orders(PharaohBuildingInfoLiveContext *);
void pharaoh_building_info_live_draw_storage_orders(PharaohBuildingInfoLiveContext *);
void pharaoh_building_info_live_draw_bazaar_orders_foreground(PharaohBuildingInfoLiveContext *);
void pharaoh_building_info_live_draw_granary_orders_foreground(PharaohBuildingInfoLiveContext *);
void pharaoh_building_info_live_draw_storage_orders_foreground(PharaohBuildingInfoLiveContext *);
void pharaoh_building_info_live_draw_storage_scroll_thumb(PharaohBuildingInfoLiveContext *);

#endif
