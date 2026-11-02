#include "menu_input.h"
#include "menu_dialog.h"
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
extern int FUN_00425990(int,int,short *,int);
void FUN_00426530(void)
{
    int handled;
    B(0xe38dc9)=1;
    handled=FUN_00425990((int)U(0x705718),(int)U(0x705714),(short *)0x5c8168u,(int)U(0x705708));
    if(!handled&&!U(0x705708)&&B(0xe38df8)) {
        restore_menu_callback(U(0x705704));
        B(0xe38dca)=1;
        B(0xe38dda)=B(0x70570c);
    }
}
