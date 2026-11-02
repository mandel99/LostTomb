#include "menu_list_getter.h"

/* Native ECX receiver is explicit in C; the result occupies AL. */
uint8_t FUN_0041b8f0(uint32_t self)
{
    return restore_menu_list_can_open(self);
}
