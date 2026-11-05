#ifndef RESTORE_MAP_COMMIT_H
#define RESTORE_MAP_COMMIT_H
#include <stdint.h>
/* Original 004728A0: cdecl, seven DWORD arguments, RET0, no receiver.
 * The original has no observable return-value contract. */
void restore_map_commit(uint32_t player,int32_t orientation,int32_t first_x,
                        int32_t first_y,int32_t last_x,int32_t last_y,int32_t tool);
#endif
