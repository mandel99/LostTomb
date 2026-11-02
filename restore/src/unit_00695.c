#include "restore_types.h"
#include "town_menu.h"
uint8_t FUN_004493c0(const RestoreTownRecord *record)
{
    return ((const uint8_t *)0x5d9c78u)[record->bytes[2]];
}
