#ifndef RESTORE_COMPACT_MENU_INPUT_BOUNDARY_H
#define RESTORE_COMPACT_MENU_INPUT_BOUNDARY_H
#include "menu_input_controller.h" /* Public restore/direct_c frame. */
/* Entry ESP points at the generated controller's native return slot. */
void restore_menu_input_boundary_dispatch(uint32_t site,uint32_t target,struct restore_menu_input_frame *frame);
#endif
