#ifndef RESTORE_MENU_INLINE_CAPACITY_H
#define RESTORE_MENU_INLINE_CAPACITY_H
#include "menu_input_controller.h"
#include <stdint.h>
/* Call immediately before these original capacity loads:
   4DA05A,4DA424,4DA8D3,4DACB8: self=ESI+776FB0, need=EBP.
   4DB446,4DB6E6,4DBC55: self=ESI, need=EBP.
   4DB98B: self=ESI, need=EBX. All have EDI=new capacity on resize.
   No frame register, native need, flags or healthy-domain data is changed. */
void restore_menu_inline_capacity_before(uint32_t instruction,struct restore_menu_input_frame *frame);
/* Boundary entry ESP points at native return slot. Handles paired alloc/free,
   seven nonallocating route methods and repaired 4DD930. Returns zero only to
   request dispatch of an unrelated target by the caller's normal adapter. */
int restore_menu_inline_capacity_boundary(uint32_t site,uint32_t target,struct restore_menu_input_frame *frame);
#endif
