#include "pharaoh/ui/window_state_actions.h"

/* Native004CA980: adopted complete existing C leaf. */
void pharaoh_tooltip_invalidate(int32_t current_tick,
    int32_t *tooltip_id, int32_t *tooltip_tick)
{
    *tooltip_id = 0;
    *tooltip_tick = current_tick;
}
