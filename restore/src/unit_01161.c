#include <stdint.h>
#include <stdbool.h>

/* Native-address local views. Indices below are in the declared C element unit. */
#define clearance_offsets (*(int32_t *)0x005f80fcu)
#define north_flags (*(uint32_t *)0x00af55fcu)
#define west_flags (*(uint32_t *)0x00af5988u)
#define tile_flags (*(uint32_t *)0x00af598cu)
#define east_flags (*(uint32_t *)0x00af5990u)
#define south_flags (*(uint32_t *)0x00af5d1cu)
#define walk_direction (*(uint32_t *)0x00bc96f4u)
#define walk_count (*(uint32_t *)0x00bc96f8u)
#define walk_end (*(uint32_t *)0x00bc96fcu)
#define walk_step (*(int32_t *)0x00bc9700u)
#define city_origin (*(uint32_t *)0x00bca2d8u)
#define tile_clearance (*(uint8_t *)0x00ee0c50u)
#define walk_start (*(uint32_t *)0x00bc9704u)

extern char FUN_0047ec80(uint32_t tile, uint32_t mask);
extern char FUN_0047ecc0(uint32_t tile, uint32_t mask);

void FUN_0047f200(int32_t tile_or_x,int32_t y)

{
  char clearance;
  uint32_t lookahead_flags;
  int32_t continued_tiles;
  int32_t *offset;
  int32_t neighbor_count;
  
  walk_end = 0;
  walk_count = 0;
  walk_step = 0;
  walk_direction = 0;
  tile_or_x = city_origin + y * 0xe4 + tile_or_x;
  walk_start = tile_or_x;
  if ((((((&tile_flags)[tile_or_x] & 4) != 0) && (((&tile_flags)[tile_or_x] & 0x48) == 0)) &&
      (continued_tiles = FUN_0047ec80(tile_or_x,4), continued_tiles == 3)) &&
     (continued_tiles = FUN_0047ec80(tile_or_x,0x10000), continued_tiles == 0)) {
    if (((&north_flags)[tile_or_x] & 4) == 0) {
      walk_direction = 4;
      walk_step = 0xe4;
      walk_count = 1;
      continued_tiles = 0;
    }
    else if (((&east_flags)[tile_or_x] & 4) == 0) {
      walk_direction = 6;
      walk_step = -1;
      walk_count = 1;
      continued_tiles = 0;
    }
    else if (((&south_flags)[tile_or_x] & 4) == 0) {
      walk_direction = 0;
      walk_step = -0xe4;
      walk_count = 1;
      continued_tiles = 0;
    }
    else {
      if (((&west_flags)[tile_or_x] & 4) != 0) {
        return;
      }
      walk_direction = 2;
      walk_step = 1;
      walk_count = 1;
      continued_tiles = 0;
    }
    while( true ) {
      tile_or_x = tile_or_x + walk_step;
      walk_count = walk_count + 1;
      lookahead_flags = (&tile_flags)[walk_step + tile_or_x];
      if ((lookahead_flags & 1) != 0) {
        return;
      }
      if ((((&tile_flags)[tile_or_x] & 0x4000000) != 0) && (((&tile_flags)[tile_or_x] & 0x40) != 0))
      {
        return;
      }
      offset = &clearance_offsets;
      do {
        clearance = *(char *)((int32_t)&tile_clearance + *offset + tile_or_x);
        if (clearance == '\x01') {
          return;
        }
        if (clearance == '\x03') {
          return;
        }
        offset = offset + 1;
      } while ((int32_t)offset < 0x5f811c);
      if ((lookahead_flags & 4) == 0) break;
      if ((lookahead_flags & 0x48) != 0) {
        return;
      }
      neighbor_count = FUN_0047ecc0(tile_or_x,4);
      if (neighbor_count != 4) {
        return;
      }
      continued_tiles = continued_tiles + 1;
      if (3 < continued_tiles) {
        return;
      }
    }
    walk_end = tile_or_x;
    continued_tiles = FUN_0047ec80(tile_or_x,4);
    if ((continued_tiles != 3) || (continued_tiles = FUN_0047ec80(tile_or_x,0x10000), continued_tiles != 0)) {
      walk_end = 0;
    }
  }
  return;
}
