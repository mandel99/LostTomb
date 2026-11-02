#include "menu_dialog.h"
#include "menu_input.h"
static void respond(uint8_t accepted)
{
    *(uint8_t *)0xd3b439u=accepted;
    restore_menu_callback(*(uint32_t *)0x705704u);
    /* The callback may change the stored return mode; read it afterwards. */
    *(uint8_t *)0xe38dcau=1;
    *(uint8_t *)0xe38ddau=*(uint8_t *)0x70570cu;
}
/* Local native callbacks at the two modal button addresses. */
void FUN_00426590(void) {respond(1);}
void FUN_004265b0(void) {respond(0);}
