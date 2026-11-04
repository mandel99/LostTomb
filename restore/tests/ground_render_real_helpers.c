#define FUN_0053b670 ground_render_actual_clear
#define FUN_0053b830 ground_render_actual_classify
#define restore_sound_reset_ambient_weights ground_render_profile_clear
#define restore_sound_building_ambient_class ground_render_profile_classify
#include "../runtime/sound_ambient_profile.c"
#ifdef GROUND_RENDER_STAGED
#include "../artifacts/ground_render_phase/unit_02493.c"
#else
#include "../src/unit_02493.c"
#endif
#include "../src/unit_02497.c"
#undef U
#define FUN_0047a1c0 ground_render_actual_boundary
#include "../src/unit_01097.c"
#define FUN_0051f780 ground_render_actual_empty
#include "../src/unit_02351.c"
