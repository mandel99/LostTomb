#ifndef PHARAOH_UI_CITY_MESSAGE_POLICY_H
#define PHARAOH_UI_CITY_MESSAGE_POLICY_H
#include "pharaoh/runtime/city_warning.h"
#include "pharaoh/scenario/request_dispatch.h"

typedef struct PharaohCityMessagePolicyContext {
    const uint32_t *message_options; /* E38EF0 */
    const int32_t *current_request; /* 6057E4 */
    PharaohCityWarningContext *warnings;
    const PharaohRequestDispatchContext *requests;
} PharaohCityMessagePolicyContext;

/* Complete004E0F90. Return full0/1; retain real warning and request effects.
   Message131 dereferences the original signedWORD parameter without a guard. */
uint32_t pharaoh_city_message_should_popup(
    const PharaohCityMessagePolicyContext *,int32_t message,
    const int16_t *parameters);
#endif
