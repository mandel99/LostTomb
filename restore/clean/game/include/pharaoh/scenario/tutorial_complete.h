#ifndef PHARAOH_SCENARIO_TUTORIAL_COMPLETE_H
#define PHARAOH_SCENARIO_TUTORIAL_COMPLETE_H
#include "pharaoh/scenario/scenario_start_mission.h"
#include "pharaoh/ui/sidebar_background.h"
#include "pharaoh/ui/city_message_publish.h"

typedef struct PharaohTutorialCompletionContext {
    uint8_t *flags,*menu_flags; /*7027B8/6FDC88*/
    const int8_t *mission_id; /*E38EA8*/
    const int32_t *advisor_override; /*E38EBC*/
    const PharaohTutorialRule *rules; /*5794B0,26x28*/
    const PharaohTutorialMenuCountsContext *counts;
    const PharaohSidebarBackgroundState *sidebar;
    const PharaohSidebarBackgroundOperations *sidebar_operations;
    const PharaohCityMessagePublishContext *messages;
} PharaohTutorialCompletionContext;

/* Complete00417AE0. Real menu/sidebar providers and actual shared flags,
   rules, message state and window are required. No placeholder operations. */
void pharaoh_tutorial_set_flag(const PharaohTutorialCompletionContext *,
    int32_t tutorial);
#endif
