#include "menu_prepare_control.h"
#include "../direct_c/menu_004f7e50.h"
#include "../direct_c/menu_input_boundary.h"
#include <stdlib.h>

/* No native live-in arguments. The initial PUSH ECX scratch is overwritten
   by MOV [ESP+10],EDI at4F7EE7 before normal-path local use and calls.
   The earlier modal path only calls the no-argument reset4E2AB0, then returns
   EAX0; the editor guard also returns EAX0. Other saved registers serve ABI
   preservation only. The typed caller consumes the full EAX0/1 result. */
uint32_t FUN_004f7e50(void)
{
    uint32_t stack[2048],i,entry;
    struct restore_menu_input_frame frame={{0},2,restore_menu_input_boundary_dispatch};
    typedef char pointer_is_32[sizeof(void*)==4?1:-1];
    (void)sizeof(pointer_is_32);
    for(i=0;i<2048;++i)stack[i]=0xa5a55a5au;
    entry=(uint32_t)(uintptr_t)&stack[2000];
    stack[2000]=0;frame.r[4]=entry;
    restore_C_004f7e50(&frame);
    if(frame.r[4]!=entry+4u)abort();
    for(i=0;i<16;++i)if(stack[i]!=0xa5a55a5au)abort();
    for(i=2001;i<2048;++i)if(stack[i]!=0xa5a55a5au)abort();
    return frame.r[0];
}
