#ifndef PHARAOH_UI_WINDOW_LAYOUT_KEY_H
#define PHARAOH_UI_WINDOW_LAYOUT_KEY_H

#include <stdint.h>

/* Original entry 0x004D5D70. The special layout base is read from native
   state only for window key 7. */
int32_t pharaoh_ui_layout_window_key(int32_t window_key,
    int32_t window_seven_layout_base);

#endif
