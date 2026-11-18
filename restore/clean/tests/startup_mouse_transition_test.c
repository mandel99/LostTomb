#include "pharaoh/platform/windows_startup_dispatch.h"
#include <stdio.h>

#define CHECK(condition) do { if (!(condition)) return __LINE__; } while (0)

int main(void)
{
    PharaohWindowsStartupDispatch dispatch = { 0 };
    dispatch.refresh.current_window_id = 0x3c;
    /* Bink consumes the down event while skipping. Its orphan release must
       leave the newly selected intro screen waiting. */
    pharaoh_windows_startup_dispatch_mouse(&dispatch, WM_LBUTTONUP, 0);
    CHECK(dispatch.input.mouse_left_went_up == 0);
    CHECK(dispatch.input.video_input_flag_a == 0);

    pharaoh_windows_startup_dispatch_mouse(&dispatch, WM_LBUTTONDOWN, 0);
    pharaoh_windows_startup_dispatch_mouse(&dispatch, WM_LBUTTONUP, 0);
    CHECK(dispatch.input.mouse_left_went_up == 1);
    CHECK(dispatch.input.video_input_flag_a == 1);
    dispatch.input.mouse_left_went_up = 0;
    dispatch.input.video_input_flag_a = 0;

    dispatch.refresh.current_window_id = 0x13;
    pharaoh_windows_startup_dispatch_mouse(&dispatch, WM_LBUTTONDOWN, 0);
    dispatch.refresh.current_window_id = 0x3c;
    pharaoh_windows_startup_dispatch_mouse(&dispatch, WM_LBUTTONUP, 0);
    CHECK(dispatch.input.mouse_left_went_up == 0);
    CHECK(dispatch.input.video_input_flag_a == 0);

    pharaoh_windows_startup_dispatch_mouse(&dispatch, WM_RBUTTONUP, 0);
    CHECK(dispatch.input.mouse_right_went_up == 0);
    pharaoh_windows_startup_dispatch_mouse(&dispatch, WM_RBUTTONDOWN, 0);
    pharaoh_windows_startup_dispatch_mouse(&dispatch, WM_RBUTTONUP, 0);
    CHECK(dispatch.input.mouse_right_went_up == 1);
    CHECK(dispatch.input.video_input_flag_b == 1);
    puts("PASS: orphan release, new click, held transition, right click");
    return 0;
}
