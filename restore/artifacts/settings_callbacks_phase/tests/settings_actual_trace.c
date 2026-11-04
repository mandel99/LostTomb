/* Test-only source aliases; includes exact candidate or applied C. */
#define SETTINGS_CALLBACKS_TRACE
#define restore_sound_open_stream settings_trace_restore_sound_open_stream
#define restore_sound_set_stream_volume settings_trace_restore_sound_set_stream_volume
#define restore_sound_stream_is_playing settings_trace_restore_sound_stream_is_playing
#define restore_sound_get_stream_volume settings_trace_restore_sound_get_stream_volume
#define restore_sound_set_music_volume settings_trace_restore_sound_set_music_volume
#define restore_sound_set_voice_volume settings_trace_restore_sound_set_voice_volume
#define restore_sound_set_ambient_volume settings_trace_restore_sound_set_ambient_volume
#define restore_sound_set_sample_volume settings_trace_restore_sound_set_sample_volume
#define restore_sound_music_volume_0055f32f settings_trace_restore_sound_music_volume_0055f32f
#include "settings_callbacks.h"
#define FUN_00402c10 settings_trace_00402c10
#define FUN_00402c40 settings_trace_00402c40
#define FUN_00402cc0 settings_trace_00402cc0
#define FUN_00402f70 settings_trace_00402f70
#define FUN_00403060 settings_trace_00403060
#define FUN_00403130 settings_trace_00403130
#define FUN_004031a0 settings_trace_004031a0
#define FUN_00403210 settings_trace_00403210
#define FUN_00403270 settings_trace_00403270
#define FUN_004032f0 settings_trace_004032f0
#define FUN_00403390 settings_trace_00403390
#define FUN_004ea760 settings_trace_004ea760
#ifdef SETTINGS_CALLBACKS_STAGED
#include "../runtime/settings_callbacks.c"
#else
#include "../../../runtime/settings_callbacks.c"
#endif
#undef FUN_00402c10
#undef FUN_00402c40
#undef FUN_00402cc0
#undef FUN_00402f70
#undef FUN_00403060
#undef FUN_00403130
#undef FUN_004031a0
#undef FUN_00403210
#undef FUN_00403270
#undef FUN_004032f0
#undef FUN_00403390
#undef FUN_004ea760
#define FUN_00403320 settings_trace_00403320
#ifdef SETTINGS_CALLBACKS_STAGED
#include "../src/unit_00027.c"
#else
#include "../../../src/unit_00027.c"
#endif
#undef FUN_00403320
#define FUN_00416ba0 settings_trace_00416ba0
#ifdef SETTINGS_CALLBACKS_STAGED
#include "../src/unit_00191.c"
#else
#include "../../../src/unit_00191.c"
#endif
#undef FUN_00416ba0
#define FUN_00402e50 settings_trace_00402e50
#define SETTINGS_CALLBACKS_DIFFICULTY_ONLY
#ifdef SETTINGS_CALLBACKS_STAGED
#include "../runtime/mission_menu.c"
#else
#include "../../../runtime/mission_menu.c"
#endif
#undef SETTINGS_CALLBACKS_DIFFICULTY_ONLY
#undef FUN_00402e50
#define restore_settings_sound_55f32f settings_trace_0055f32f
#define restore_settings_sound_55f3eb settings_trace_0055f3eb
#ifdef SETTINGS_CALLBACKS_STAGED
#include "../runtime/settings_callbacks_audio.c"
#else
#include "../../../runtime/settings_callbacks_audio.c"
#endif
#undef restore_settings_sound_55f32f
#undef restore_settings_sound_55f3eb
#define FUN_0055f38d settings_trace_0055f38d
#include "../../../src/unit_02705.c"
#undef FUN_0055f38d
#define restore_sound_close_stream settings_trace_close_stream
static void settings_trace_stream_publish(uint32_t base,uint32_t field,uint32_t value)
{if(field==0x154u||field==0x158u)restore_settings_write(base+field,4,value);
 else *(volatile uint32_t *)(uintptr_t)(base+field)=value;}
#define RESTORE_SOUND_STREAM_PUBLISH(base,field,value) settings_trace_stream_publish(base,field,value)
#define FUN_0055f561 settings_trace_0055f561
#undef U
#include "../../../runtime/sound_streams.c"
#include "../../../src/unit_02708.c"
#undef FUN_0055f561
#undef restore_sound_close_stream
#define FUN_0055f639 settings_trace_0055f639
#include "../../../src/unit_02711.c"
#undef FUN_0055f639
#define FUN_0051f780 settings_trace_0051f780
#include "../../../src/unit_02351.c"
#undef FUN_0051f780
