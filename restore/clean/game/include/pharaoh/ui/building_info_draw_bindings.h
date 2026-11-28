#ifndef PHARAOH_UI_BUILDING_INFO_DRAW_BINDINGS_H
#define PHARAOH_UI_BUILDING_INFO_DRAW_BINDINGS_H

#include "pharaoh/ui/building_info_content_live.h"
#include "pharaoh/ui/unit_panel_live.h"
#include "pharaoh/ui/building_info_industry_live.h"
#include "pharaoh/ui/building_info_services_live.h"
#include "pharaoh/ui/building_info_monuments_live.h"
#include "pharaoh/runtime/digital_sound_device.h"

/* Borrowed providers, not copies of saved pools or renderer state. storage is
 * also the common layout/text/font binding for the LiveOperations primitives.
 * All supplied content/unit contexts must share this UI, text cursor, layout,
 * image renderer and locked framebuffer. city requires every genuine pass
 * reached by its current Native overlay/alternate branch, including the
 * footprint; this binder does not manufacture a missing pass.
 * Each borrowed context must be fully initialized for its existing provider;
 * readiness checks establish binding availability and the shared-state links. */
typedef struct PharaohBuildingInfoDrawProviders {
    PharaohBuildingInfoLiveContext *ui;
    const PharaohStorageInfoDrawContext *storage;
    const PharaohInfoContentDrawContext *content;
    const PharaohInfoWalkerContentContext *walkers;
    const PharaohInfoTerrainContentContext *terrain;
    const PharaohInfoHouseContentContext *house;
    const PharaohUnitPanelLiveContext *units;
    const PharaohInfoSquareDrawContext *square_buttons;
    const PharaohImageButtonDrawState *image_buttons;
    PharaohCityRenderContext *city;
    const PharaohDigitalSoundDeviceContext *sample_sound;
    const PharaohInfoIndustryLiveContext *industry;
    const PharaohInfoServicesLiveContext *services;
    const PharaohInfoMonumentsLiveContext *monuments;
} PharaohBuildingInfoDrawProviders;

enum PharaohBuildingInfoDrawTypeSupport {
    PHARAOH_INFO_DRAW_NATIVE_DEFAULT,
    PHARAOH_INFO_DRAW_CONNECTED_TYPE,
    PHARAOH_INFO_DRAW_MISSING_TYPE_PROVIDER,
    PHARAOH_INFO_DRAW_OUTSIDE_TYPE_CATALOG
};

enum PharaohBuildingInfoDrawSupport {
    PHARAOH_INFO_DRAW_READY,
    PHARAOH_INFO_DRAW_MISSING_CONTEXT,
    PHARAOH_INFO_DRAW_STATE_MISMATCH,
    PHARAOH_INFO_DRAW_MISSING_CITY_PASS,
    PHARAOH_INFO_DRAW_MISSING_SAMPLE_SOUND,
    PHARAOH_INFO_DRAW_UNSUPPORTED_TYPE,
    PHARAOH_INFO_DRAW_DIVIDE_FAULT
};

typedef struct PharaohBuildingInfoDrawResult {
    enum PharaohBuildingInfoDrawSupport support;
    PharaohPercentageScaleStatus arithmetic;
} PharaohBuildingInfoDrawResult;

typedef struct PharaohBuildingInfoDrawBindings {
    PharaohBuildingInfoDrawProviders providers;
    PharaohBuildingInfoLiveOperations operations;
    PharaohPercentageScaleStatus arithmetic;
} PharaohBuildingInfoDrawBindings;

/* Installs only real drawing endpoints in ui->operations. Input/advisor/
 * festival/command callbacks remain unbound: do not call handle_input with
 * this draw-only table. Provider objects and bindings must outlive the UI.
 * Keep bindings at a stable address after installation (operations.user_data
 * refers to it). LiveOperations text accepts the five actual ui font IDs. */
void pharaoh_building_info_draw_bindings_initialize(
    PharaohBuildingInfoDrawBindings *,const PharaohBuildingInfoDrawProviders *);

enum PharaohBuildingInfoDrawTypeSupport pharaoh_building_info_draw_type_support(int32_t type);
enum PharaohBuildingInfoDrawSupport pharaoh_building_info_draw_background_support(
    const PharaohBuildingInfoDrawBindings *);
enum PharaohBuildingInfoDrawSupport pharaoh_building_info_draw_foreground_support(
    const PharaohBuildingInfoDrawBindings *);

/* These guards run before the Native parent dispatchers. An unsupported
 * branch performs no drawing. A unit-panel divide status is returned without
 * further drawing; the host must immediately raise the native divide fault.
 * The background parent has no stateful work after its unit callback returns.
 * A pending fault also blocks subsequent foreground/background calls. */
PharaohBuildingInfoDrawResult pharaoh_building_info_draw_bindings_background(
    PharaohBuildingInfoDrawBindings *);
PharaohBuildingInfoDrawResult pharaoh_building_info_draw_bindings_foreground(
    PharaohBuildingInfoDrawBindings *);

#endif
