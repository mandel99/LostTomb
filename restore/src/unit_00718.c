#include "town_menu.h"
RestoreTownRecord *restore_town_trade_next(RestoreTownList *self,const int16_t *previous)
{
    int32_t index = previous ? (int32_t)*previous + 1 : 0;
    for (; index < self->count; ++index) {
        RestoreTownRecord *record = &self->records[index];
        if (record->bytes[2] == 1 && record->bytes[0x32]) {
            *(int16_t *)record->bytes = (int16_t)index;
            return record;
        }
    }
    return 0;
}
/* All 26 native call sites select this global receiver in ECX. */
int *FUN_0044baf0(const int16_t *previous)
{
    return (int *)restore_town_trade_next(RESTORE_TOWNS,previous);
}
