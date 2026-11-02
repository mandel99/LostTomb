/* Link-only integration probe. No callback is executed here. In particular,
   this file does not link the private fixture or any controlled endpoint. */
#include "remaining_menu_actions.h"
static void (*volatile roots[])(void)={FUN_00406090,FUN_0040a0e0};
int main(void){return roots[0]==0||roots[1]==0;}
