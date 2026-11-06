#include <stdint.h>
#include <stdbool.h>

/* Native-address local views. Indices below are in the declared C element unit. */
#define population_scale (*(uint32_t *)0x005d19b0u)
#define population_fraction_00709a58 (*(uint32_t *)0x00709a58u)
#define population_fraction_00709a90 (*(uint32_t *)0x00709a90u)
#define population_fraction_00709ac8 (*(uint32_t *)0x00709ac8u)
#define population_fraction_00709cc0 (*(uint32_t *)0x00709cc0u)
#define population_fraction_00709cf8 (*(uint32_t *)0x00709cf8u)
#define population_fraction_00709d30 (*(uint32_t *)0x00709d30u)
#define population_fraction_0070a698 (*(uint32_t *)0x0070a698u)
#define spawn_sequence (*(int16_t *)0x00775900u)
#define entry_width (*(int32_t *)0x00784904u)
#define entry_height (*(int32_t *)0x00784908u)
#define entrance_x (*(int16_t *)0x00784bc0u)
#define entrance_x_second_half (*(uint16_t *)0x00784bd0u)
#define entrance_y (*(int16_t *)0x00784be0u)
#define optional_entry_coordinates (*(int32_t *)0x00784dc4u)
#define source_row_count (*(int32_t *)0x00785d70u)
#define source_rows (*(uint32_t *)0x00785d74u)
#define source_row_active (*(uint8_t *)0x00785da6u)
#define actor_generation_tag (*(uint8_t *)0x0078d7c0u)
#define tile_flags (*(uint32_t *)0x00af598cu)
#define city_origin (*(uint32_t *)0x00bca2d8u)
#define formation_tag_field (*(uint16_t *)0x00bcba0cu)
#define formation_population_class (*(uint8_t *)0x00bcba32u)
#define formation_direction (*(uint8_t *)0x00bcba39u)
#define formation_source_row (*(uint16_t *)0x00bcba40u)
#define formation_sequence (*(int16_t *)0x00bcba50u)
#define actor_flag5 (*(uint8_t *)0x00bcd605u)
#define actor_destination_x (*(uint16_t *)0x00bcd628u)
#define actor_destination_y (*(uint16_t *)0x00bcd62au)
#define actor_wait (*(int16_t *)0x00bcd62eu)
#define actor_state (*(uint8_t *)0x00bcd630u)
#define actor_formation (*(int16_t *)0x00bcd658u)
#define actor_initialized (*(uint8_t *)0x00bcd65du)
#define actor_migration_flag (*(uint8_t *)0x00bcd66eu)
#define actor_generation (*(uint8_t *)0x00bcd678u)
#define actor_arrival_flag (*(uint8_t *)0x00bcd728u)
#define actor_population (*(uint32_t *)0x00bcd764u)
#define actor_sequence (*(uint32_t *)0x00bcd768u)
#define actor_population_class (*(uint32_t *)0x00bcd76cu)
#define actor_formation_tag (*(uint32_t *)0x00bcd770u)
#define actor_source_row (*(uint32_t *)0x00bcd774u)
#define actor_zero_field (*(uint32_t *)0x00bcd778u)
#define rng_history_index (*(int32_t *)0x00d3b1c4u)
#define rng_history (*(uint32_t *)0x00d3b1c8u)
#define rng_value (*(uint32_t *)0x00d3b374u)
#define rng_state_b (*(uint32_t *)0x00d3b380u)
#define rng_state_a (*(uint32_t *)0x00d3b384u)
#define population_scale_index (*(uint32_t *)0x00e39080u)
#define primary_entry_x (*(int16_t *)0x00784c64u)
#define primary_entry_y (*(int16_t *)0x00784c66u)
#define fallback_entry_x (*(int16_t *)0x00784c68u)
#define fallback_entry_y (*(int16_t *)0x00784c6au)
#define rng_mask15_b (*(uint32_t *)0x00d3b364u)
#define rng_mask7_b (*(uint32_t *)0x00d3b368u)
#define rng_mask3_b (*(uint32_t *)0x00d3b36cu)
#define rng_mask15_a (*(uint32_t *)0x00d3b370u)
#define rng_mask3_a (*(uint32_t *)0x00d3b378u)

extern void FUN_00446650(int32_t actor);
extern uint32_t FUN_004467a0(int32_t *x, int32_t *y);
extern void FUN_004620c0(int32_t actor);
extern void FUN_00469140(int32_t x, int32_t y, uint32_t tile);
extern int32_t FUN_004b4800(int32_t zero, int32_t kind, int32_t x, int32_t y, int32_t direction);
extern void FUN_004b5af0(int32_t actor);
extern int32_t FUN_004b6ff0(int32_t kind, int32_t formation_type, int32_t direction, int32_t x, int32_t y);
extern int32_t FUN_004cde40(int32_t value, uint32_t percent);

int32_t FUN_00446a30(int32_t population_class,int32_t population,int32_t entrance,uint32_t formation_tag,int32_t source_row)

{
  int16_t entry_x_word;
  uint8_t direction;
  uint16_t *entrance_cursor;
  int32_t actor_or_group_count;
  uint32_t flags_or_rng;
  int32_t option_y;
  int32_t *option_cursor;
  int32_t formation_or_entry_x;
  int16_t *entry_x_cursor;
  uint32_t rng_bit_b;
  uint32_t fraction_c;
  int16_t sequence_or_ordinal;
  int32_t population_or_offset;
  int32_t actor_or_population;
  uint32_t rng_bit_a;
  uint32_t rng_shift_b;
  int32_t actor_offset;
  int32_t entry_y_or_option_count;
  uint32_t fraction_b;
  uint32_t fraction_a;
  int32_t entry_x;
  int32_t jitter_x_or_formation_type;
  int32_t entry_y;
  int32_t spawned_or_jitter_x;
  int32_t category_or_option_count;
  int32_t *row;
  int32_t *sizes;
  int32_t jitter_y;
  uint16_t selected_y;
  int32_t entry_tile;
  uint32_t remaining_or_selected_x;
  int32_t spawn_spacing;
  int32_t workspace [18]; /* Counts, optional coordinates, and three4-DWORD size groups. */
  
  entry_y_or_option_count = 0;
  spawned_or_jitter_x = 0;
  category_or_option_count = 0;
  if ((source_row < 0) || (source_row_count <= source_row)) {
    row = 0;
  }
  else {
    row = (int32_t *)((uint8_t *)&source_rows + (uint32_t)source_row * 124u);
    if (row != 0) {
      (&source_row_active)[source_row * 0x7c] = 1;
    }
  }
  if (((entrance < 9) || (0x10 < entrance)) || (population_class == 0xd)) {
    formation_or_entry_x = FUN_004cde40(population,*(uint32_t *)((uint8_t *)&population_scale + population_scale_index * 4));
    if (0x95 < formation_or_entry_x) {
      formation_or_entry_x = 0x96;
    }
    spawn_sequence = (int16_t)((uint16_t)spawn_sequence + 1u);
    if (32000 < spawn_sequence) {
      spawn_sequence = 1;
    }
    fraction_a = population_fraction_00709cc0;
    fraction_c = population_fraction_00709d30;
    fraction_b = population_fraction_00709cf8;
    if ((population_class != 0xc) &&
       (fraction_a = population_fraction_00709a90, fraction_c = population_fraction_00709ac8, fraction_b = population_fraction_00709a58, population_class == 0xd)) {
      fraction_a = population_fraction_0070a698;
      fraction_c = 0;
      fraction_b = 0;
    }
    FUN_004cde40(formation_or_entry_x,fraction_a);
    population_or_offset = FUN_004cde40(formation_or_entry_x,fraction_b);
    actor_or_group_count = FUN_004cde40(formation_or_entry_x,fraction_c);
    workspace[0] = 0;
    actor_or_population = (formation_or_entry_x - actor_or_group_count) - population_or_offset;
    workspace[1] = 0;
    for (formation_or_entry_x = 6; formation_or_entry_x < 18; ++formation_or_entry_x) workspace[formation_or_entry_x] = 0;
    workspace[2] = 0;
    if (0 < actor_or_population) {
      if (actor_or_population < 0x11) {
        workspace[0] = 1;
      }
      else if (actor_or_population < 0x21) {
        workspace[0] = 2;
        workspace[7] = actor_or_population / 2;
        actor_or_population = actor_or_population - actor_or_population / 2;
      }
      else {
        workspace[0] = 3;
        workspace[8] = actor_or_population / 3;
        workspace[7] = workspace[8];
        actor_or_population = actor_or_population + workspace[8] * -2;
      }
      workspace[6] = actor_or_population;
    }
    if (0 < population_or_offset) {
      if (population_or_offset < 0x11) {
        workspace[1] = 1;
      }
      else if (population_or_offset < 0x21) {
        workspace[1] = 2;
        workspace[11] = population_or_offset / 2;
        population_or_offset = population_or_offset - population_or_offset / 2;
      }
      else {
        workspace[1] = 3;
        workspace[12] = population_or_offset / 3;
        workspace[11] = workspace[12];
        population_or_offset = population_or_offset + workspace[12] * -2;
      }
      workspace[10] = population_or_offset;
    }
    if (0 < actor_or_group_count) {
      if (actor_or_group_count < 0x11) {
        workspace[2] = 1;
      }
      else if (actor_or_group_count < 0x21) {
        workspace[2] = 2;
        workspace[15] = actor_or_group_count / 2;
        actor_or_group_count = actor_or_group_count - actor_or_group_count / 2;
      }
      else {
        workspace[2] = 3;
        workspace[16] = actor_or_group_count / 3;
        workspace[15] = workspace[16];
        actor_or_group_count = actor_or_group_count + workspace[16] * -2;
      }
      workspace[14] = actor_or_group_count;
    }
    entry_tile = -1;
    sequence_or_ordinal = primary_entry_y;
    entry_x_word = primary_entry_x;
    if (population_class != 0xb) {
      formation_or_entry_x = 0;
      entry_x_cursor = &entrance_x;
      do {
        if (*entry_x_cursor != -1) {
          formation_or_entry_x = formation_or_entry_x + 1;
        }
        entry_x_cursor = entry_x_cursor + 1;
      } while ((int32_t)entry_x_cursor < 0x784bd0);
      if (entrance == 8) {
        if (formation_or_entry_x < 3) {
          flags_or_rng = rng_value & 1;
        }
        else if (formation_or_entry_x < 5) {
          flags_or_rng = rng_value & 3;
        }
        else {
          flags_or_rng = rng_value & 7;
        }
      }
      else {
        flags_or_rng = entrance - 1;
      }
      formation_or_entry_x = 0;
      do {
        if ((&entrance_x)[flags_or_rng] != -1) break;
        flags_or_rng = flags_or_rng + 1;
        if (7 < (int32_t)flags_or_rng) {
          flags_or_rng = 0;
        }
        formation_or_entry_x = formation_or_entry_x + 1;
      } while (formation_or_entry_x < 8);
      sequence_or_ordinal = (&entrance_y)[flags_or_rng];
      entry_x_word = (&entrance_x)[flags_or_rng];
    }
    entry_x = (int32_t)entry_x_word;
    entry_y_or_option_count = (int32_t)sequence_or_ordinal;
    if ((entry_x == -1) || (entry_y_or_option_count == -1)) {
      entry_x = (int32_t)fallback_entry_x;
      entry_y_or_option_count = (int32_t)fallback_entry_y;
    }
    if (entry_y_or_option_count == 0) {
      direction = '\x04';
    }
    else if ((int32_t)entry_y_or_option_count < entry_height + -1) {
      if (entry_x == 0) {
        direction = '\x02';
      }
      else {
        direction = (uint8_t)((((entry_x < entry_width - 1) - 1u) & 2u) + 4u);
      }
    }
    else {
      direction = '\0';
    }
    formation_or_entry_x = city_origin + (int32_t)entry_y_or_option_count * 0xe4 + entry_x;
    flags_or_rng = (&tile_flags)[formation_or_entry_x];
    if ((flags_or_rng & 0x203) == 0) {
      entry_y = entry_y_or_option_count;
      if ((flags_or_rng & 4) == 0) {
        if ((flags_or_rng & 0xc108) != 0) {
          entry_tile = formation_or_entry_x;
          FUN_00469140(entry_x,entry_y_or_option_count,formation_or_entry_x);
          formation_or_entry_x = entry_tile;
        }
      }
      else if ((flags_or_rng & 0x40) == 0) {
        return entry_tile;
      }
      entry_tile = formation_or_entry_x;
      spawned_or_jitter_x = 0;
      category_or_option_count = 0;
      do {
        remaining_or_selected_x = workspace[(int32_t)category_or_option_count];
        if (0 < (int32_t)remaining_or_selected_x) {
          if (population_class == 0xc) {
            if (category_or_option_count == 1) {
              population = 0x37;
            }
            else if (category_or_option_count == 2) {
              population = 0x38;
            }
            else {
              population = 0x36;
            }
LAB_004472cd: ;
            jitter_x_or_formation_type = 10;
          }
          else {
            if (population_class != 0xd) {
              if (category_or_option_count == 1) {
                population = 0x2b;
              }
              else if (category_or_option_count == 2) {
                population = 0x2d;
              }
              else {
                population = 0x2c;
              }
              goto LAB_004472cd;
            }
            population = 99;
            jitter_x_or_formation_type = 8;
          }
          if (0 < (int32_t)remaining_or_selected_x) {
            sizes = workspace + category_or_option_count * 4 + 6;
            do {
              formation_or_entry_x = FUN_004b6ff0(population,jitter_x_or_formation_type,direction,entry_x,entry_y_or_option_count);
              if (0 < formation_or_entry_x) {
                actor_or_group_count = 0;
                population_or_offset = formation_or_entry_x * 0x90;
                *(uint16_t *)((uint8_t *)&formation_tag_field + population_or_offset) = (uint16_t)formation_tag;
                (&formation_direction)[population_or_offset] = direction;
                (&formation_population_class)[population_or_offset] = (uint8_t)population_class;
                sequence_or_ordinal = spawn_sequence;
                *(uint16_t *)((uint8_t *)&formation_source_row + population_or_offset) = (uint16_t)source_row;
                *(int16_t *)((uint8_t *)&formation_sequence + population_or_offset) = sequence_or_ordinal;
                population_or_offset = *sizes;
                if (0 < population_or_offset) {
                  spawn_spacing = spawned_or_jitter_x * 5;
                  do {
                    actor_or_population = FUN_004b4800(0,population,entry_x,entry_y,direction);
                    actor_offset = actor_or_population * 0x184;
                    (&actor_flag5)[actor_offset] = 0;
                    (&actor_state)[actor_offset] = 9;
                    *(int16_t *)((uint8_t *)&actor_wait + actor_offset) =
                         (int16_t)(((int16_t)actor_or_group_count + 1 + (int16_t)spawn_spacing * 4) * 10);
                    (&actor_formation)[actor_or_population * 0xc2] = (int16_t)formation_or_entry_x;
                    FUN_004b5af0(actor_or_population);
                    actor_or_group_count = actor_or_group_count + 1;
                    (&actor_initialized)[actor_offset] = 1;
                  } while (actor_or_group_count < population_or_offset);
                }
                spawned_or_jitter_x = spawned_or_jitter_x + 1;
                entry_y_or_option_count = entry_y;
              }
              ++sizes;
              remaining_or_selected_x = remaining_or_selected_x + -1;
            } while (remaining_or_selected_x != 0);
          }
        }
        category_or_option_count = category_or_option_count + 1;
      } while ((int32_t)category_or_option_count < 3);
    }
  }
  else {
    if (0x95 < population) {
      population = 0x96;
    }
    spawn_sequence = (int16_t)((uint16_t)spawn_sequence + 1u);
    if (32000 < spawn_sequence) {
      spawn_sequence = 1;
    }
    entrance_cursor = &entrance_x_second_half;
    do {
      entrance_cursor = entrance_cursor + 1;
    } while ((int32_t)entrance_cursor < 0x784be0);
    option_cursor = &optional_entry_coordinates;
    do {
      if (*option_cursor != -1) {
        workspace[entry_y_or_option_count] = *option_cursor;
        workspace[entry_y_or_option_count + 3] = option_cursor[3];
        ++entry_y_or_option_count;
      }
      ++option_cursor;
    } while ((uintptr_t)option_cursor < 0x784dd0u);
    entrance = entrance + -1;
    formation_or_entry_x = 8;
    do {
      if ((&entrance_x)[entrance] != -1) break;
      entrance = entrance + 1;
      if (0xf < entrance) {
        entrance = 8;
      }
      formation_or_entry_x = formation_or_entry_x + 1;
    } while (formation_or_entry_x < 0x10);
    formation_or_entry_x = (int32_t)(int16_t)(&entrance_x)[entrance];
    option_y = (int16_t)(&entrance_y)[entrance];
    if ((formation_or_entry_x == -1) || (option_y == -1)) {
      formation_or_entry_x = (int32_t)fallback_entry_x;
      option_y = (int32_t)fallback_entry_y;
    }
    if (option_y == 0) {
      direction = '\x04';
    }
    else if ((int32_t)option_y < entry_height + -1) {
      if (formation_or_entry_x == 0) {
        direction = '\x02';
      }
      else {
        direction = (uint8_t)((((formation_or_entry_x < entry_width - 1) - 1u) & 2u) + 4u);
      }
    }
    else {
      direction = '\0';
    }
    entry_tile = city_origin + (int32_t)option_y * 0xe4 + formation_or_entry_x;
    if (source_row == 0x8c) {
      remaining_or_selected_x = (int32_t)(population + (population >> 0x1f & 3U)) >> 2;
    }
    else {
      remaining_or_selected_x = (uint32_t)*(uint8_t *)((uint8_t *)row + 0x36);
    }
    population_or_offset = 0;
    entry_y = option_y;
    category_or_option_count = entry_y_or_option_count;
    if (0 < (int32_t)remaining_or_selected_x) {
      do {
        if (population_class == 0xc) {
          actor_or_group_count = FUN_004b4800(0,100,formation_or_entry_x,option_y,direction);
        }
        else {
          jitter_x_or_formation_type = formation_or_entry_x;
          jitter_y = option_y;
          FUN_004467a0(&jitter_x_or_formation_type,&jitter_y);
          actor_or_group_count = FUN_004b4800(0,0x5d,jitter_x_or_formation_type,jitter_y,direction);
        }
        actor_or_population = actor_or_group_count * 0x184;
        (&actor_flag5)[actor_or_population] = 0;
        (&actor_state)[actor_or_population] = 8;
        *(int16_t *)((uint8_t *)&actor_wait + actor_or_population) = (int16_t)((int16_t)population_or_offset * 0x19 + 10);
        FUN_004b5af0(actor_or_group_count);
        (&actor_initialized)[actor_or_population] = 1;
        spawned_or_jitter_x = spawned_or_jitter_x + 1;
        (&actor_migration_flag)[actor_or_population] = 1;
        *(uint32_t *)((uint8_t *)&actor_zero_field + actor_or_population) = 0;
        FUN_00446650(actor_or_group_count);
        (&actor_generation)[actor_or_population] = (uint8_t)actor_generation_tag;
        population_or_offset = population_or_offset + 1;
      } while (population_or_offset < (int32_t)remaining_or_selected_x);
    }
    population_or_offset = 0;
    sequence_or_ordinal = 0;
    if (0 < (int32_t)category_or_option_count) {
      (&rng_history)[rng_history_index] = rng_value;
      rng_history_index = (int32_t)((uint32_t)rng_history_index + 1u);
      if (99 < rng_history_index) {
        rng_history_index = 0;
      }
      population_or_offset = 0x1f;
      do {
        flags_or_rng = rng_state_a >> 4;
        rng_bit_a = rng_state_a & 1;
        rng_shift_b = rng_state_b >> 4;
        rng_bit_b = rng_state_b & 1;
        rng_state_a = rng_state_a >> 1;
        rng_state_b = rng_state_b >> 1;
        if ((flags_or_rng & 1) != rng_bit_a) {
          rng_state_a = rng_state_a | 0x40000000;
        }
        if ((rng_shift_b & 1) != rng_bit_b) {
          rng_state_b = rng_state_b | 0x40000000;
        }
        population_or_offset = population_or_offset + -1;
      } while (population_or_offset != 0);
      rng_mask3_a = rng_state_a & 7;
      rng_mask15_b = rng_state_b & 0x7fff;
      rng_mask15_a = rng_state_a & 0x7fff;
      rng_mask7_b = rng_state_b & 0x7f;
      rng_value = rng_state_a & 0x7f;
      rng_mask3_b = rng_state_b & 7;
      population_or_offset = (int32_t)((int64_t)(uint64_t)rng_mask15_a % (int64_t)(int32_t)category_or_option_count);
    }
    if (population != 0) {
      remaining_or_selected_x = (uint16_t)workspace[population_or_offset];
      selected_y = (uint16_t)workspace[population_or_offset + 3];
      do {
        population_or_offset = population;
        if (population < 0x10) {
          population = 0;
        }
        else {
          population_or_offset = 0x10;
          population = population + -0x10;
        }
        category_or_option_count = entry_y;
        spawned_or_jitter_x = formation_or_entry_x;
        FUN_004467a0(&spawned_or_jitter_x,&category_or_option_count);
        if (population_class == 0xc) {
          fraction_a = 0x65;
        }
        else {
          fraction_a = 0x5c;
        }
        actor_or_group_count = FUN_004b4800(0,fraction_a,spawned_or_jitter_x,category_or_option_count,direction);
        actor_or_population = actor_or_group_count * 0x184;
        *(uint16_t *)((uint8_t *)&actor_destination_x + actor_or_population) = (uint16_t)remaining_or_selected_x;
        *(uint16_t *)((uint8_t *)&actor_destination_y + actor_or_population) = selected_y;
        (&actor_flag5)[actor_or_population] = 0;
        (&actor_state)[actor_or_population] = 8;
        *(int16_t *)((uint8_t *)&actor_wait + actor_or_population) = (int16_t)(sequence_or_ordinal * 0x28 + 10);
        FUN_004b5af0(actor_or_group_count);
        (&actor_initialized)[actor_or_population] = 1;
        (&actor_migration_flag)[actor_or_population] = 1;
        *(uint32_t *)((uint8_t *)&actor_zero_field + actor_or_population) = 0;
        FUN_004620c0(actor_or_group_count);
        actor_or_group_count = (int32_t)spawn_sequence;
        (&actor_arrival_flag)[actor_or_population] = 1;
        *(int32_t *)((uint8_t *)&actor_population + actor_or_population) = population_or_offset;
        *(int32_t *)((uint8_t *)&actor_sequence + actor_or_population) = actor_or_group_count;
        *(int32_t *)((uint8_t *)&actor_population_class + actor_or_population) = population_class;
        *(uint32_t *)((uint8_t *)&actor_formation_tag + actor_or_population) = formation_tag;
        sequence_or_ordinal = sequence_or_ordinal + 1;
        *(int32_t *)((uint8_t *)&actor_source_row + actor_or_population) = source_row;
      } while (population != 0);
      return entry_tile;
    }
  }
  return entry_tile;
}
