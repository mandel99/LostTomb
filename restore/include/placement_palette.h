#ifndef RESTORE_PLACEMENT_PALETTE_H
#define RESTORE_PLACEMENT_PALETTE_H
#include <stdint.h>
/* Native5414B0 is plain cdecl2/RET0, no ECX receiver/live-in EAX.
   out_flags must address a writable DWORD; aliasing with game data is retained.
   The result is a full DWORD sprite index, not an AL boolean. */
uint32_t restore_placement_image_005414b0(int32_t building,uint32_t out_flags);
#endif
