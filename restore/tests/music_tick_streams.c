/* Compile the actual shared stream provider and canonical volume entry in
   this fixture's configuration, independently of production /O2 properties. */
#include "../runtime/sound_streams.c"
#include "../src/unit_02705.c"
#include <stdlib.h>
/* Unreached Miles imports from the shared translation unit fail closed. */
int __cdecl _AIL_close_stream_4(uintptr_t p0){(void)p0;abort();}
int __cdecl _AIL_open_stream_12(uintptr_t p0,uintptr_t p1,uintptr_t p2){(void)p0;(void)p1;(void)p2;abort();}
int __cdecl _AIL_set_stream_loop_count_8(uintptr_t p0,uintptr_t p1){(void)p0;(void)p1;abort();}
int __cdecl _AIL_start_stream_4(uintptr_t p0){(void)p0;abort();}
int __cdecl _AIL_stream_volume_4(uintptr_t p0){(void)p0;abort();}
