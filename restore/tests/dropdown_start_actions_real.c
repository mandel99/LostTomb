/* Test-only symbol aliases retain actual callback and stream implementations.
   The production library uses their normal, real family and media bindings. */
#define FUN_00516bb0 restore_start_family
#define FUN_00516990 restore_start_choose
#define FUN_004cb6a0 restore_start_campaign
#define FUN_0055f449 restore_start_stream_body
#define FUN_004cdff0 restore_start_selector_body
#define _AIL_close_stream_4 restore_start_close_stream
#define _AIL_open_stream_12 restore_start_open_stream
#define _AIL_set_stream_loop_count_8 restore_start_stream_loop
#define _AIL_set_stream_volume_8 restore_start_stream_volume
#define _AIL_start_stream_4 restore_start_stream_start
#include "../src/unit_02707.c"
#include "../runtime/dropdown_start_actions.c"
