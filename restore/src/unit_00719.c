#include "town_record_queries.h"
#include "town_menu.h"
/* Both native callers select this global receiver. Preserve the established
   gameplay guard for the original invalid-index null-page read. */
int *FUN_0044bb40(int32_t index)
{
    uint32_t self=(uint32_t)(uintptr_t)RESTORE_TOWNS;
    if(index!=140&&(index<0||index>=RESTORE_TOWNS->count))return 0;
    return (int *)(uintptr_t)restore_event_list_record_kind2(self,index);
}
