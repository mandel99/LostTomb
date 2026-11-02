#include "camera_navigation.h"
#define FUN_00430cc0 camera_navigation_actual_area
#ifdef CAMERA_NAVIGATION_STAGED
#include "../artifacts/camera_navigation_phase/unit_00466.c"
#else
#include "../src/unit_00466.c"
#endif
#undef FUN_00430cc0
#define FUN_00430cd0 camera_navigation_actual_navigate
#ifdef CAMERA_NAVIGATION_STAGED
#include "../artifacts/camera_navigation_phase/unit_00467.c"
#else
#include "../src/unit_00467.c"
#endif
#undef FUN_00430cd0
#define FUN_004304c0 camera_navigation_actual_lookup
#include "../src/unit_00458.c"
#undef FUN_004304c0
#define FUN_004de140 camera_navigation_actual_reset
#include "../src/unit_01887.c"
#undef FUN_004de140
#define FUN_004de080 camera_navigation_actual_clamp
#ifdef CAMERA_NAVIGATION_STAGED
#include "../artifacts/camera_navigation_phase/unit_01886.c"
#else
#include "../src/unit_01886.c"
#endif
#undef FUN_004de080
#undef U
#undef B
#define FUN_004ddea0 camera_navigation_actual_width_facade
#include "../src/unit_01880.c"
#undef FUN_004ddea0
#define FUN_004ddeb0 camera_navigation_actual_height_facade
#include "../src/unit_01881.c"
#undef FUN_004ddeb0
#define FUN_00430ca0 camera_navigation_actual_width
#include "../src/unit_00464.c"
#undef FUN_00430ca0
#define FUN_00430cb0 camera_navigation_actual_height
#include "../src/unit_00465.c"
#undef FUN_00430cb0
