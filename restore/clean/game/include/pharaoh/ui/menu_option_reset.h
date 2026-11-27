#ifndef PHARAOH_UI_MENU_OPTION_RESET_H
#define PHARAOH_UI_MENU_OPTION_RESET_H

#include <stdint.h>

/* Complete00406070/004060E0. Supply actual native DWORD arrays; the menu
   discards callback returns, so residual native EAX is not a C result. */
void pharaoh_dropdown_reset_four_dword_pairs(
    uint32_t first[4], uint32_t second[4]);
void pharaoh_dropdown_reset_two_dword_pairs(
    uint32_t first[2], uint32_t second[2]);

#endif
