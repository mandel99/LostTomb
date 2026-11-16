#include "pharaoh/ui/menu_button_layout_init.h"

#include <limits.h>
#include <stdint.h>

static int16_t signed_word_from_bits(uint16_t bits)
{
    if (bits > INT16_MAX) {
        return (int16_t)((int32_t)bits - INT32_C(0x10000));
    }
    return (int16_t)bits;
}

static int16_t low_signed_word(int32_t value)
{
    return signed_word_from_bits((uint16_t)(uint32_t)value);
}

static int16_t wrap_word_add(int16_t left, int16_t right)
{
    const uint16_t sum = (uint16_t)left + (uint16_t)right;
    return signed_word_from_bits(sum);
}

void pharaoh_menu_buttons_initialize(PharaohGenericButton *buttons,
    const PharaohMenuLayoutNode *layout_nodes)
{
    const PharaohMenuLayoutNode *node;

    for (node = layout_nodes; node != 0; node = node->next) {
        PharaohMenuRect *const bounds =
            &buttons[node->button_index - 1].bounds;
        bounds->left = low_signed_word(node->left);
        bounds->top = low_signed_word(node->top);
        bounds->right = wrap_word_add(
            low_signed_word(node->left), low_signed_word(node->width));
        bounds->bottom = wrap_word_add(
            low_signed_word(node->top), low_signed_word(node->height));
    }
}
