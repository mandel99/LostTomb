#include "pharaoh/ui/menu_option_reset.h"

/* 00406070: interleaved DWORD stores at784CC0/784CD0. */
void pharaoh_dropdown_reset_four_dword_pairs(
    uint32_t first[4], uint32_t second[4])
{
    for (int32_t index = 0; index < 4; ++index) {
        first[index] = UINT32_MAX;
        second[index] = UINT32_MAX;
    }
}

/* 004060E0: preserve the native order784B64,784B6C,784B68,784B70. */
void pharaoh_dropdown_reset_two_dword_pairs(
    uint32_t first[2], uint32_t second[2])
{
    first[0] = UINT32_MAX;
    second[0] = UINT32_MAX;
    first[1] = UINT32_MAX;
    second[1] = UINT32_MAX;
}
