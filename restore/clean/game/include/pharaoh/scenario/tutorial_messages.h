#ifndef PHARAOH_SCENARIO_TUTORIAL_MESSAGES_H
#define PHARAOH_SCENARIO_TUTORIAL_MESSAGES_H
#include "pharaoh/scenario/tutorial_complete.h"

typedef struct PharaohTutorialMissionMessagesContext {
    const PharaohTutorialCompletionContext *completion;
    const uint16_t *allowed_legions; /*784D2A,three WORDs*/
    /* Actual pre-existing15-word parameter block consumed by4E0CF0.
       Native417B40 writes only slots1,0,8; retain every other word.
       Their copied values have real record/text/choice-mask consumers.
       Do not supply invented zeros or unobserved extra stack bytes. */
    int16_t *retained_parameters;
} PharaohTutorialMissionMessagesContext;

/* Complete00417B40 with its live message-packet input made explicit.
   Completion flags/rules/mission/menu/messages are the actual shared owners.
   Native menu bytes44/60/72/84 alias notice/legion availability fields.
   Runtime attachment requires the actual retained packet; it is not defaulted. */
void pharaoh_tutorial_update_mission_messages(
    const PharaohTutorialMissionMessagesContext *);
#endif
