#ifndef PHARAOH_UI_BUILDING_INFO_PROGRAM_DATA_H
#define PHARAOH_UI_BUILDING_INFO_PROGRAM_DATA_H

#include "pharaoh/ui/building_info_live.h"

enum {
    PHARAOH_INFO_PROGRAM_GENERIC_COUNT=68,
    PHARAOH_INFO_PROGRAM_ACTION_COUNT=23,
    PHARAOH_INFO_PROGRAM_TUTORIAL_COUNT=15,
    PHARAOH_INFO_PROGRAM_LIMIT_COUNT=60,
    PHARAOH_INFO_PROGRAM_IMAGE_SEED_COUNT=17
};

/* Exact Native28-byte evidence view, not the normalized C input record.
 * Function addresses remain tokens and are never cast to function pointers. */
typedef struct PharaohInfoNativeGenericButton {
    PharaohMenuRect bounds;
    uint32_t left_token,right_token;
    uint8_t reserved_10,activated,reserved_12,trigger;
    int32_t primary_parameter,secondary_parameter;
} PharaohInfoNativeGenericButton;

typedef struct PharaohInfoProgramGenericRow {
    uint32_t native_address;
    int32_t count,first_record;
    const PharaohInfoNativeGenericButton *native_records;
    const PharaohMenuRect *rectangles;
} PharaohInfoProgramGenericRow;

typedef struct PharaohInfoProgramImageSeed {
    uint32_t native_address;
    int32_t count;
    int16_t trailing_trigger; /* real WORD at base+count*36+8 */
    const PharaohImageButtonRecord *records;
} PharaohInfoProgramImageSeed;

typedef struct PharaohInfoProgramSquareSeed {
    uint32_t native_address;
    PharaohSquareButtonRecord record;
} PharaohInfoProgramSquareSeed;

/* Immutable sidecars preserve the pinned records, dense geometry and tokens
 * needed by host action wiring. Image seed0=help_close,1=limits,2..16=tutorial.
 * Square seed0=scroll_down,1=scroll_up. Native tutorial addresses are labels;
 * the actual UI pointers are relocated to owned records by bind(). */
extern const PharaohInfoNativeGenericButton
    pharaoh_info_program_native_generic[PHARAOH_INFO_PROGRAM_GENERIC_COUNT];
extern const PharaohMenuRect
    pharaoh_info_program_rectangles[PHARAOH_INFO_PROGRAM_GENERIC_COUNT];
extern const PharaohInfoProgramGenericRow
    pharaoh_info_program_generic_rows[PHARAOH_INFO_ROW_COUNT];
extern const PharaohInfoProgramImageSeed
    pharaoh_info_program_image_seeds[PHARAOH_INFO_PROGRAM_IMAGE_SEED_COUNT];
extern const PharaohInfoProgramSquareSeed pharaoh_info_program_square_seeds[2];
extern const uint32_t
    pharaoh_info_program_tutorial_addresses[PHARAOH_INFO_PROGRAM_TUTORIAL_COUNT];
extern const uint32_t pharaoh_info_program_action_tokens[PHARAOH_INFO_PROGRAM_ACTION_COUNT];

/* Required resolver for all23 nonzero tokens, including image/square actions.
 * Return a genuine C adapter with the original effects. NULL rejects init.
 * The callback is an action resolver, not native-address execution/emulation.
 * Host image/square dispatch may use action() after publishing actual Native
 * primary/secondary parameters, and pass the genuine shared action context. */
typedef PharaohGenericButtonAction (*PharaohInfoProgramActionResolver)(
    void *user_data,uint32_t native_token);

/* Borrow these real host/program-owned objects. No PE-zero snapshot replaces
 * an SG3 load, sound-bank initializer, input clock, scenario tutorial state or
 * phrase producer. sound_channels points at the first roam DWORD (F5467C),
 * within the same initialized237x20-byte F54670 bank used by sound providers.
 * text_width aliases the real bitmap cursor E20778. The common bridge must
 * publish both input parameters to D3B198/D3B194 before invoking actions. */
typedef struct PharaohInfoProgramBorrowed {
    const int32_t *display_mode;
    int32_t *focus_primary,*focus_secondary,*focus_building;
    int32_t *button_parameter,*button_secondary_parameter,*text_width;
    const int32_t *resource_images,*panel_button_images;
    const int32_t *font_black,*font_green,*font_white,*font_red,*font_title;
    const uint8_t *tutorial_flags,*phrase_counts;
    const int32_t *sound_channels,*sound_volume;
    const uint8_t *sound_enabled;
    PharaohSquareButtonInputState *square_input;
    PharaohImageButtonInputState *image_input;
    PharaohButtonHitState *image_focus;
    PharaohGenericButtonInputState *generic_input;
    PharaohGenericButtonFocusState *generic_focus;
    PharaohButtonLayoutState *button_layout;
    const PharaohButtonLayoutOperations *button_layout_operations;
} PharaohInfoProgramBorrowed;

enum PharaohInfoProgramSupport {
    PHARAOH_INFO_PROGRAM_READY,
    PHARAOH_INFO_PROGRAM_MISSING_CONTEXT,
    PHARAOH_INFO_PROGRAM_NOT_INITIALIZED,
    PHARAOH_INFO_PROGRAM_MISSING_RESOLVER,
    PHARAOH_INFO_PROGRAM_MISSING_ACTION,
    PHARAOH_INFO_PROGRAM_MISSING_BORROWED_DATA,
    PHARAOH_INFO_PROGRAM_STATE_MISMATCH
};

typedef struct PharaohInfoProgramResult {
    enum PharaohInfoProgramSupport support;
    uint32_t native_token;
    const char *field;
} PharaohInfoProgramResult;

/* Zero-initialize once before the first initialize_once(). Keep this object
 * at a stable address while bound. Subsequent initialize_once()/bind() calls
 * preserve every mutable button byte, activation/countdown and input state.
 * Native NULL callbacks exist only in six zero/trigger0 policy padding slots;
 * they stay NULL. They never receive invented actions. */
typedef struct PharaohBuildingInfoProgramData {
    uint32_t initialized;
    PharaohGenericButtonAction actions[PHARAOH_INFO_PROGRAM_ACTION_COUNT];
    PharaohGenericButton generic[PHARAOH_INFO_PROGRAM_GENERIC_COUNT];
    PharaohImageButtonRecord help_close[2];
    PharaohImageButtonRecord limits[PHARAOH_INFO_PROGRAM_LIMIT_COUNT];
    PharaohImageButtonRecord tutorials[PHARAOH_INFO_PROGRAM_TUTORIAL_COUNT];
    PharaohBuildingInfoLiveImageRow tutorial_rows[PHARAOH_INFO_PROGRAM_TUTORIAL_COUNT];
    PharaohSquareButtonRecord scroll_down,scroll_up;
    PharaohInfoProgramBorrowed borrowed;
    PharaohBuildingInfoLiveContext *ui;
} PharaohBuildingInfoProgramData;

PharaohInfoProgramResult pharaoh_building_info_program_data_initialize_once(
    PharaohBuildingInfoProgramData *,PharaohInfoProgramActionResolver,void *user_data);
PharaohInfoProgramResult pharaoh_building_info_program_data_support(
    const PharaohBuildingInfoProgramData *);
PharaohInfoProgramResult pharaoh_building_info_program_data_bind(
    PharaohBuildingInfoProgramData *,PharaohBuildingInfoLiveContext *,
    const PharaohInfoProgramBorrowed *);
PharaohGenericButtonAction pharaoh_building_info_program_data_action(
    const PharaohBuildingInfoProgramData *,uint32_t native_token);

#endif
