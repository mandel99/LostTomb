#ifndef PHARAOH_UI_CITY_INPUT_ACTION_BINDINGS_H
#define PHARAOH_UI_CITY_INPUT_ACTION_BINDINGS_H

#include "pharaoh/ui/city_input_program_data.h"
#include "pharaoh/ui/city_pending_actions.h"
#include "pharaoh/ui/city_undo_live.h"
#include "pharaoh/ui/advisor_open.h"
#include "pharaoh/ui/editor_city_route_actions_live.h"
#include "pharaoh/ui/small_button_callbacks.h"
#include "pharaoh/runtime/city_rotation.h"
#include "pharaoh/ui/editor_city_tools.h"

typedef enum PharaohCityInputActionSupport {
    PHARAOH_CITY_INPUT_ACTION_READY,
    PHARAOH_CITY_INPUT_ACTION_MISSING_CONTEXT,
    PHARAOH_CITY_INPUT_ACTION_UNKNOWN_TOKEN
} PharaohCityInputActionSupport;

typedef struct PharaohCityInputActionProviders {
    const PharaohCityBuildSelectionContext *build;
    const PharaohCitySidebarActionContext *sidebar;
    const PharaohCityUndoLiveContext *undo;
    const PharaohCityPendingMessageContext *pending_messages;
    const PharaohCityMessageQueueContext *message_help;
    const PharaohAdvisorOpenContext *advisor;
    const PharaohEditorCityRouteLiveContext *editor_route;
    const PharaohEditorCityToolsContext *editor_tools;
    PharaohCityRotationContext *rotation;
    PharaohSmallButtonState *timer;
    uint32_t *button_parameter,*button_secondary_parameter;
    PharaohImageButtonInputState *image_input;
    PharaohGenericButtonInputState *generic_input;
} PharaohCityInputActionProviders;

typedef struct PharaohCityInputActionBindings {
    PharaohCityInputActionProviders providers;
    PharaohCityInputActionSupport last_support;
    uint32_t last_token;
} PharaohCityInputActionBindings;

PharaohCityInputActionSupport pharaoh_city_input_action_bindings_bind(
    PharaohCityInputActionBindings *,const PharaohCityInputActionProviders *);
PharaohGenericButtonAction pharaoh_city_input_action_bindings_resolve_compiled(
    void *user_data,uint32_t token);
void pharaoh_city_input_action_bindings_image_dispatch(void *user_data,uint32_t token);

#endif
