#include "camera_pan.h"
#include <windows.h>
#include <mmsystem.h>
DWORD WINAPI camera_pan_verify_time(void);
#define FUN_004cf660 camera_pan_actual_sample
#ifdef CAMERA_PAN_STAGED
#include "../artifacts/camera_pan_phase/unit_01771.c"
#else
#include "../src/unit_01771.c"
#endif
#undef FUN_004cf660
#define FUN_004ddd60 camera_pan_actual_cadence
#define timeGetTime camera_pan_verify_time
#ifdef CAMERA_PAN_STAGED
#include "../artifacts/camera_pan_phase/unit_01877.c"
#else
#include "../src/unit_01877.c"
#endif
#undef timeGetTime
#undef FUN_004ddd60
#define FUN_004ddde0 camera_pan_actual_move
#ifdef CAMERA_PAN_STAGED
#include "../artifacts/camera_pan_phase/unit_01879.c"
#else
#include "../src/unit_01879.c"
#endif
#undef FUN_004ddde0
#define FUN_0041d6f0 camera_pan_actual_decay
#pragma warning(push)
#pragma warning(disable:4057)
#include "../src/unit_00281.c"
#pragma warning(pop)
#undef FUN_0041d6f0
#define FUN_004de080 camera_pan_actual_clamp
#include "../src/unit_01886.c"
#undef FUN_004de080
#undef U
#undef B
#define FUN_004ddea0 camera_pan_actual_width_facade
#include "../src/unit_01880.c"
#undef FUN_004ddea0
#define FUN_004ddeb0 camera_pan_actual_height_facade
#include "../src/unit_01881.c"
#undef FUN_004ddeb0
#define FUN_00430ca0 camera_pan_actual_width
#include "../src/unit_00464.c"
#undef FUN_00430ca0
#define FUN_00430cb0 camera_pan_actual_height
#include "../src/unit_00465.c"
#undef FUN_00430cb0
