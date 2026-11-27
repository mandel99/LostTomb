#ifndef PHARAOH_UI_EVENT_TEXT_FORMAT_H
#define PHARAOH_UI_EVENT_TEXT_FORMAT_H
#include "pharaoh/runtime/text_loading.h"
#include "pharaoh/ui/token_expansion.h"

/* Complete00448330. Borrow actual phrase catalogue784F20 and token context
   784F34, with genuine resolvers/comparison operations. Nonpositive capacity
   leaves destination untouched; unknown phrase IDs produce an empty string. */
void pharaoh_event_text_format(const PharaohTextPointerTable *,
    const PharaohTokenExpansionEntry *,const PharaohTokenExpansionOperations *,
    int32_t text_id,char *destination,int32_t capacity);
#endif
