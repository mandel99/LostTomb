#ifndef PHARAOH_UI_BUILDING_INFO_INIT_SUPPORT_H
#define PHARAOH_UI_BUILDING_INFO_INIT_SUPPORT_H

#include "pharaoh/ui/building_info_live.h"
#include "pharaoh/map/surface_tiles.h"
#include "pharaoh/monuments/build_requirements.h"
#include "pharaoh/walker/sound.h"
#include "pharaoh/runtime/digital_sound_stream.h"
#include "pharaoh/ui/walker_portrait.h"

typedef struct PharaohInfoPyramidSelectionContext {
    PharaohMapSurfaceContext *surface;
    const uint32_t *part_counts,*mastaba_part_counts; /* 56F7D8/56F7EC */
} PharaohInfoPyramidSelectionContext;

/* Whole4F5D90. Uses the real mode6 surface child and shared EAAE84/88
 * found/target fields; it does not approximate complex membership. */
int32_t pharaoh_info_pyramid_from_complex(
    const PharaohInfoPyramidSelectionContext *,int32_t building);

typedef struct PharaohInfoAdvisorButtonsContext {
    PharaohBuildingInfoLiveContext *ui;
    const PharaohBuildingModelRecord *models;
    PharaohMonumentBuildRequirements *requirements;
    PharaohScenarioEvent *events;
    const int32_t *event_count;
    const uint32_t *resource_advisors; /* actual784DE8 DWORDs */
} PharaohInfoAdvisorButtonsContext;

/* Whole515130. Fifteen actual DWORDs, real resource/progress/request children,
 * and the native definition BYTE at5D19D6 (record+0E), not a resource WORD. */
void pharaoh_info_advisor_buttons(const PharaohInfoAdvisorButtonsContext *,
    uint32_t output[15]);

/* Reuse the existing48-byte view. Its legacy walker_type member is the
 * script-ID DWORD for actual5E4C20; this input is not the climate table. */
typedef PharaohClimatePreyRecord PharaohInfoPortraitScript;
typedef struct PharaohInfoWalkerPortraitCodeContext {
    const PharaohWalkerQueryRecord *walkers;
    const PharaohBuildingTypeRecord *buildings;
    const int32_t *portrait_codes; /* actual6012EC DWORD table */
    const PharaohInfoPortraitScript *scripts; /* actual5E4C20,48-byte rows */
    const uint32_t *selected_scenario; /* low BYTE of784CB0 */
} PharaohInfoWalkerPortraitCodeContext;

/* Whole53D280. Full signed EAX portrait code, including the real building
 * group query. The walker resource BYTE is the native alternate-kind overlay. */
int32_t pharaoh_info_walker_portrait_code(
    const PharaohInfoWalkerPortraitCodeContext *,int32_t walker);

typedef struct PharaohInfoVoiceStreamContext {
    const volatile int32_t *enabled,*volume; /* F55B88/E38ECC */
    const volatile uint8_t *voice_enabled; /* E38E65 */
    const char *prefix; /* actual5F90A4 */
    char *filename_scratch; /* actual sharedF55A20 */
    PharaohDigitalSoundDeviceState *device;
    const PharaohDigitalSoundStreamCloseOperations *operations;
} PharaohInfoVoiceStreamContext;
typedef struct PharaohInfoWalkerVoiceContext {
    const PharaohInfoVoiceStreamContext *stream;
    const char (*voice_names)[24]; /* actual5FB16C */
    const char (*special_voice_names)[24]; /* actual60110C */
} PharaohInfoWalkerVoiceContext;
typedef struct PharaohInfoWalkerSelectionVoiceContext {
    PharaohBuildingInfoLiveContext *ui;
    const PharaohInfoWalkerPortraitCodeContext *portrait;
    const PharaohWalkerSoundContext *sound;
    const PharaohInfoWalkerVoiceContext *voice;
} PharaohInfoWalkerSelectionVoiceContext;

/* Whole539A70/53BFE0/53D0B0. Native names are NUL-terminated in24-byte
 * records and fit the32-byte local scratch. Actual digital device/stream
 * operations are required. The two void owners have discarded EAX results
 * throughout the recovered direct caller chain; no incoming garbage API. */
int32_t pharaoh_info_voice_stream_play(const PharaohInfoVoiceStreamContext *,
    const char *name);
void pharaoh_info_walker_voice_variant(const PharaohInfoWalkerVoiceContext *,
    int32_t kind,int32_t variant);
void pharaoh_info_walker_selection_voice(
    const PharaohInfoWalkerSelectionVoiceContext *);

#endif
