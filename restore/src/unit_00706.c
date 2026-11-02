#include "event_description.h"
/* Whole449620: ECX is the124-byte event; EAX is zero extended. */
uint32_t restore_event_has_quantity_range(const RestoreTownRecord *event)
{
    return *(const uint8_t *)(uintptr_t)(0x5d9e00u+event->bytes[2]);
}
uint32_t FUN_00449620(const RestoreTownRecord *event)
{return restore_event_has_quantity_range(event);}
