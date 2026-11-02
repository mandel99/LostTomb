/* Verification-only names let the real engines run while recording ordered calls. */
#include "message_panel_input.h"
#define FUN_00425990 restore_message_input_small_engine
#include "../src/unit_00404.c"
#undef FUN_00425990
#define add restore_message_input_choice_add
#define FUN_004261d0 restore_message_input_choice_engine
#include "../src/unit_00416.c"
#undef FUN_004261d0
#undef add
#define add restore_message_input_hover_add
#define FUN_00426450 restore_message_input_choice_hover
#include "../src/unit_00417.c"
#undef FUN_00426450
#undef add
#define FUN_004c8040 restore_message_input_clear_links
#include "../src/unit_01631.c"
#undef FUN_004c8040
#define FUN_004cde40 restore_message_input_scale
#include "../src/unit_01730.c"
#undef FUN_004cde40
#define FUN_004cde60 restore_message_input_percent
#include "../src/unit_01731.c"
#undef FUN_004cde60
#define FUN_0047d350 restore_message_input_native_zero
#include "../src/unit_01130.c"
#undef FUN_0047d350
