#include "tool_selector_input.h"
#include "sprite_libraries.h"
#define FUN_00403e00 tool_selector_input_actual_input
#ifdef TOOL_SELECTOR_INPUT_STAGED
#include "../artifacts/tool_selector_input_phase/unit_00034.c"
#else
#include "../src/unit_00034.c"
#endif
#undef FUN_00403e00
#define FUN_00403f10 tool_selector_input_actual_row
#ifdef TOOL_SELECTOR_INPUT_STAGED
#include "../artifacts/tool_selector_input_phase/unit_00036.c"
#else
#include "../src/unit_00036.c"
#endif
#undef FUN_00403f10
#define FUN_00405200 tool_selector_input_actual_selector
#include "../src/unit_00042.c"
#undef FUN_00405200
#define FUN_00404120 tool_selector_input_actual_count
#include "../src/unit_00037.c"
#undef FUN_00404120
#define FUN_00486060 tool_selector_input_actual_release
#include "../src/unit_01248.c"
#undef FUN_00486060
#define FUN_0053d850 tool_selector_input_actual_media
#pragma warning(push)
#pragma warning(disable:4057)
#include "../src/unit_02508.c"
#pragma warning(pop)
#undef FUN_0053d850
#define FUN_00426450 tool_selector_input_actual_hover
#include "../src/unit_00417.c"
#undef FUN_00426450
#define FUN_004d5ed0 tool_selector_input_actual_offset
#include "../src/unit_01840.c"
#undef FUN_004d5ed0
