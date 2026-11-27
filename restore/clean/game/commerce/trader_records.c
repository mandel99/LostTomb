#include "pharaoh/commerce/trader_records.h"
#include "pharaoh/core/memory_fill.h"

#include <stddef.h>

_Static_assert(sizeof(PharaohTradePrice) == 8,
    "native trade-price record layout");
_Static_assert(sizeof(PharaohTraderRecord) == 88 &&
    offsetof(PharaohTraderRecord, bought_resources) == 8 &&
    offsetof(PharaohTraderRecord, sold_resources) == 44 &&
    offsetof(PharaohTraderRecord, bought_value) == 80 &&
    offsetof(PharaohTraderRecord, sold_value) == 84,
    "native trader record layout");

/* Complete one-callee C translation of Pharaoh.exe 0x00444B30. */

void pharaoh_trader_create(
    PharaohTraderRecord records[PHARAOH_TRADER_RECORD_COUNT],
    PharaohWalkerQueryRecord walkers[PHARAOH_WALKER_RECORD_COUNT],
    int32_t walker_id, int32_t *next_index)
{
    const int32_t index = *next_index;
    walkers[walker_id].trader_record_id = (uint8_t)index;
    pharaoh_memory_fill_bytes((uint32_t *)(void *)&records[index],
        sizeof(records[index]), 0);
    ++*next_index;
    if (*next_index > 99) *next_index = 0;
}
