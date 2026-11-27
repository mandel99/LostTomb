#include "pharaoh/ui/event_text_format.h"

/* Complete old mixed Unit00674 algorithm with real clean C children. */
void pharaoh_event_text_format(const PharaohTextPointerTable *table,
    const PharaohTokenExpansionEntry *tokens,
    const PharaohTokenExpansionOperations *operations,
    int32_t text_id,char *destination,int32_t capacity)
{
    if(capacity<1) return;
    destination[0]='\0';
    const char *const source=pharaoh_text_pointer_table_get(table,text_id);
    if(source!=0)
        (void)pharaoh_token_expand(tokens,operations,source,destination,capacity);
}
