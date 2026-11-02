/* Test-only aliases; include exact staged or applied C algorithms. */
#define EDITOR_VALIDATION_TRACE
#define EV_PUBLIC(name) EV_BODY_##name
#define EV_BODY_FUN_00409830 editor_trace_00409830
#define EV_BODY_FUN_004ec360 editor_trace_004ec360
#define EV_BODY_FUN_004095c0 editor_trace_004095c0
#define EV_BODY_FUN_00409650 editor_trace_00409650
#define EV_BODY_FUN_004096e0 editor_trace_004096e0
#define EV_BODY_FUN_00409760 editor_trace_00409760
#define EV_BODY_FUN_004097c0 editor_trace_004097c0
#define EV_BODY_FUN_00409620 editor_trace_00409620
#include "editor_validation.h"
#ifdef EDITOR_VALIDATION_STAGED
#include "../runtime/editor_validation.c"
#else
#include "../../../runtime/editor_validation.c"
#endif
#undef EV_PUBLIC
#undef EV_BODY_FUN_00409830
#undef EV_BODY_FUN_004ec360
#undef EV_BODY_FUN_004095c0
#undef EV_BODY_FUN_00409650
#undef EV_BODY_FUN_004096e0
#undef EV_BODY_FUN_00409760
#undef EV_BODY_FUN_004097c0
#undef EV_BODY_FUN_00409620
#define FUN_004ec410 editor_trace_004ec410
#ifdef EDITOR_VALIDATION_STAGED
#include "../src/unit_02079.c"
#else
#include "../../../src/unit_02079.c"
#endif
#undef FUN_004ec410
#define FUN_00409820 editor_trace_00409820
#include "../../../src/unit_00071.c"
#undef FUN_00409820
#define FUN_00443710 editor_trace_00443710
#include "../../../src/unit_00621.c"
#undef FUN_00443710
#define FUN_004cdff0 editor_trace_004cdff0
#include "../../../src/unit_01739.c"
#undef FUN_004cdff0
#define FUN_004ce3a0 editor_trace_004ce3a0
#include "../../../src/unit_01745.c"
#undef FUN_004ce3a0
