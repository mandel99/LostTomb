#ifndef PHARAOH_MAP_BRIDGE_REMOVAL_H
#define PHARAOH_MAP_BRIDGE_REMOVAL_H

#include <stdint.h>

typedef struct PharaohBridgeRemovalContext {
    uint32_t *terrain; /* AF598C, DWORD grid. */
    uint8_t *pieces; /* ADC36C, BYTE grid. */
    uint8_t *property_flags; /* B285CC, BYTE grid. */
    const int16_t *actor_ids; /* B350DC, WORD grid; nonzero test only. */
} PharaohBridgeRemovalContext;

/* Complete native0047F810: every nonzero mark previews, zero removes the
 * connected segment. Both routines follow the actual bordered grids until
 * a non-water or missing-piece cell; no replacement traversal cap. */
void pharaoh_map_bridge_mark_or_remove(
    const PharaohBridgeRemovalContext *,uint32_t cell,uint32_t mark);
/* Complete native0047F9B0, full EAX count. Also clears property bit40 on
 * every visited piece, including when no actor occupies the segment. */
uint32_t pharaoh_map_bridge_count_occupants(
    const PharaohBridgeRemovalContext *,uint32_t cell);

#endif
