#include <stdint.h>
#include <stdbool.h>

/* Native-address local views. Indices below are in the declared C element unit. */
#define tile_flags (*(uint32_t *)0x00af598cu)
#define tile_actor (*(int16_t *)0x00b350dcu)
#define search_tile (*(uint32_t *)0x00bc9768u)
#define city_width (*(int32_t *)0x00bca2d0u)
#define city_height (*(int32_t *)0x00bca2d4u)
#define city_origin (*(uint32_t *)0x00bca2d8u)
#define actor_next (*(int16_t *)0x00bcd600u)
#define actor_kind (*(uint8_t *)0x00bcd602u)

int32_t FUN_004eb800(int32_t ring_length_or_x,int32_t step_or_y,int32_t kind_count,uint32_t *kinds,uint32_t flag_mask,int32_t *found_x,
                int32_t *found_y,int32_t remaining_rings,int32_t excluded_actor)

{
  int32_t actor;
  int32_t ring_origin;
  int32_t kind_or_x;
  uint32_t *kind;
  int32_t ring_number;
  
  kind_or_x = ring_length_or_x;
  ring_number = 0;
  ring_origin = city_origin + step_or_y * 0xe4 + ring_length_or_x;
  ring_length_or_x = 1;
  *found_x = kind_or_x;
  *found_y = step_or_y;
  do {
    step_or_y = 0;
    search_tile = ring_origin;
    if (0 < ring_length_or_x) {
      do {
        if ((((-1 < *found_x) && (-1 < *found_y)) && (*found_x < city_width)) &&
           (*found_y < city_height)) {
          kind_or_x = 0;
          if (0 < kind_count) {
            kind = kinds;
            do {
              if (*(int16_t *)(&tile_actor + search_tile) != 0) {
                actor = (int32_t)*(int16_t *)(&tile_actor + search_tile);
                do {
                  if (((uint32_t)(uint8_t)(&actor_kind)[actor * 0x184] == *kind) && (excluded_actor != actor))
                  {
                    return actor;
                  }
                  actor = (int32_t)*(int16_t *)(&actor_next + actor * 0xc2);
                } while (actor != 0);
              }
              kind_or_x = kind_or_x + 1;
              kind = kind + 1;
            } while (kind_or_x < kind_count);
          }
          if ((((&tile_flags)[search_tile] & 0x80000) == 0) &&
             ((flag_mask & (&tile_flags)[search_tile]) != 0)) goto LAB_004ebc80;
        }
        step_or_y = step_or_y + 1;
        search_tile = search_tile + 1;
        *found_x = *found_x + 1;
      } while (step_or_y < ring_length_or_x);
    }
    step_or_y = 0;
    search_tile = search_tile + -1;
    *found_x = *found_x + -1;
    if (0 < ring_length_or_x) {
      do {
        if ((((-1 < *found_x) && (-1 < *found_y)) && (*found_x < city_width)) &&
           (*found_y < city_height)) {
          kind_or_x = 0;
          if (0 < kind_count) {
            kind = kinds;
            do {
              if (*(int16_t *)(&tile_actor + search_tile) != 0) {
                actor = (int32_t)*(int16_t *)(&tile_actor + search_tile);
                do {
                  if (((uint32_t)(uint8_t)(&actor_kind)[actor * 0x184] == *kind) && (excluded_actor != actor))
                  {
                    return actor;
                  }
                  actor = (int32_t)*(int16_t *)(&actor_next + actor * 0xc2);
                } while (actor != 0);
              }
              kind_or_x = kind_or_x + 1;
              kind = kind + 1;
            } while (kind_or_x < kind_count);
          }
          if ((((&tile_flags)[search_tile] & 0x80000) == 0) &&
             ((flag_mask & (&tile_flags)[search_tile]) != 0)) goto LAB_004ebc80;
        }
        search_tile = search_tile + 0xe4;
        step_or_y = step_or_y + 1;
        *found_y = *found_y + 1;
      } while (step_or_y < ring_length_or_x);
    }
    step_or_y = 0;
    search_tile = search_tile + -0xe4;
    *found_y = *found_y + -1;
    if (0 < ring_length_or_x) {
      do {
        if ((((-1 < *found_x) && (-1 < *found_y)) && (*found_x < city_width)) &&
           (*found_y < city_height)) {
          kind_or_x = 0;
          if (0 < kind_count) {
            kind = kinds;
            do {
              if (*(int16_t *)(&tile_actor + search_tile) != 0) {
                actor = (int32_t)*(int16_t *)(&tile_actor + search_tile);
                do {
                  if (((uint32_t)(uint8_t)(&actor_kind)[actor * 0x184] == *kind) && (excluded_actor != actor))
                  {
                    return actor;
                  }
                  actor = (int32_t)*(int16_t *)(&actor_next + actor * 0xc2);
                } while (actor != 0);
              }
              kind_or_x = kind_or_x + 1;
              kind = kind + 1;
            } while (kind_or_x < kind_count);
          }
          if ((((&tile_flags)[search_tile] & 0x80000) == 0) &&
             ((flag_mask & (&tile_flags)[search_tile]) != 0)) goto LAB_004ebc80;
        }
        step_or_y = step_or_y + 1;
        search_tile = search_tile + -1;
        *found_x = *found_x + -1;
      } while (step_or_y < ring_length_or_x);
    }
    step_or_y = 0;
    search_tile = search_tile + 1;
    *found_x = *found_x + 1;
    if (0 < ring_length_or_x) {
      do {
        if ((((-1 < *found_x) && (-1 < *found_y)) && (*found_x < city_width)) &&
           (*found_y < city_height)) {
          kind_or_x = 0;
          if (0 < kind_count) {
            kind = kinds;
            do {
              if (*(int16_t *)(&tile_actor + search_tile) != 0) {
                actor = (int32_t)*(int16_t *)(&tile_actor + search_tile);
                do {
                  if (((uint32_t)(uint8_t)(&actor_kind)[actor * 0x184] == *kind) && (excluded_actor != actor))
                  {
                    return actor;
                  }
                  actor = (int32_t)*(int16_t *)(&actor_next + actor * 0xc2);
                } while (actor != 0);
              }
              kind_or_x = kind_or_x + 1;
              kind = kind + 1;
            } while (kind_or_x < kind_count);
          }
          if ((((&tile_flags)[search_tile] & 0x80000) == 0) &&
             ((flag_mask & (&tile_flags)[search_tile]) != 0)) {
LAB_004ebc80: ;
            return kind_count + 1;
          }
        }
        search_tile = search_tile + -0xe4;
        step_or_y = step_or_y + 1;
        *found_y = *found_y + -1;
      } while (step_or_y < ring_length_or_x);
    }
    search_tile = search_tile + 0xe4;
    *found_y = *found_y + 1;
    if (remaining_rings == 0) {
      return 0;
    }
    ring_number = ring_number + 1;
    ring_length_or_x = ring_length_or_x + 2;
    ring_origin = ring_origin + -0xe5;
    *found_x = *found_x + -1;
    *found_y = *found_y + -1;
    remaining_rings = remaining_rings + -1;
    if (0xe3 < ring_number) {
      return 0;
    }
  } while( true );
}
