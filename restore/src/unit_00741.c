#include "town_menu.h"
uint32_t FUN_0044d4c0(const RestoreTownRecord *record)
{
    int32_t mode;
    /* The native disabled paths define AL only; unused incoming high EAX
       bits have a defined zero value in this C interface. */
    if (record->bytes[2] != 1 || !record->bytes[0x32]) return 0;
    mode = *(const int16_t *)(record->bytes + 0x30);
    return ((uint32_t)mode & 0xffffff00u) | (uint32_t)(mode == 0 || mode == 1);
}
