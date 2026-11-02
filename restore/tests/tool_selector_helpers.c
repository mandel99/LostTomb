#include "tool_selector.h"
#define FUN_00404120 restore_tool_row_count_body
#define FUN_00486060 restore_tool_release_body
#ifdef RESTORE_TOOL_SELECTOR_PRODUCTION
#include "../src/unit_00037.c"
#include "../src/unit_01248.c"
#else
#include "../artifacts/tool_selector_phase/src/unit_00037.c"
#include "../artifacts/tool_selector_phase/src/unit_01248.c"
#endif
#undef FUN_00404120
#undef FUN_00486060
#define FUN_0053d850 restore_tool_media_body
#include "../src/unit_02508.c"
#undef FUN_0053d850
