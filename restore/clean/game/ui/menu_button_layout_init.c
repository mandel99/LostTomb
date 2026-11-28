#include "pharaoh/ui/menu_button_layout_init.h"

#include <limits.h>
#include <stddef.h>
#include <stdint.h>

_Static_assert(offsetof(PharaohUiLayoutNode, relative_x) == 0 &&
    offsetof(PharaohUiLayoutNode, values_08) == 0x08 &&
    offsetof(PharaohUiLayoutNode, next) == 0x18,
    "native 004D66F0 layout-node fields");

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

static void initialize_bounds(PharaohMenuRect *bounds,
    int32_t left, int32_t top, int32_t width, int32_t height)
{
    bounds->left = low_signed_word(left);
    bounds->top = low_signed_word(top);
    bounds->right = wrap_word_add(
        low_signed_word(left), low_signed_word(width));
    bounds->bottom = wrap_word_add(
        low_signed_word(top), low_signed_word(height));
}

void pharaoh_menu_buttons_initialize(PharaohGenericButton *buttons,
    const PharaohMenuLayoutNode *layout_nodes)
{
    const PharaohMenuLayoutNode *node;

    for (node = layout_nodes; node != 0; node = node->next) {
        initialize_bounds(&buttons[node->button_index - 1].bounds,
            node->left, node->top, node->width, node->height);
    }
}

void pharaoh_menu_buttons_initialize_actual(PharaohGenericButton *buttons,
    const PharaohUiLayoutNode *layout_nodes)
{
    const PharaohUiLayoutNode *node;

    for (node = layout_nodes; node != 0; node = node->next) {
        initialize_bounds(&buttons[node->relative_x - 1].bounds,
            node->values_08[0], node->values_08[1],
            node->values_08[2], node->values_08[3]);
    }
}
