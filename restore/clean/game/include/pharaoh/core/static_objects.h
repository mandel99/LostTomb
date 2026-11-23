#ifndef PHARAOH_CORE_STATIC_OBJECTS_H
#define PHARAOH_CORE_STATIC_OBJECTS_H

#include <stdint.h>

enum { PHARAOH_EMPIRE_ROUTE_POINTS = 50 };

typedef struct PharaohEmpireRoutePoint {
    int16_t x;
    int16_t y;
    uint8_t segment;
    uint8_t pad;
} PharaohEmpireRoutePoint;

typedef struct PharaohEmpireRoute {
    uint8_t step;
    uint8_t reserved_1[3];
    PharaohEmpireRoutePoint *path;
    PharaohEmpireRoutePoint points[PHARAOH_EMPIRE_ROUTE_POINTS];
    int32_t path_length;
    int32_t path_capacity;
    int16_t first_object;
    int16_t last_object;
    uint8_t kind;
    uint8_t point_count;
    uint8_t in_use;
    uint8_t reserved_2;
} PharaohEmpireRoute;

typedef struct PharaohScreenFade {
    int32_t steps;
    int32_t level;
    int32_t width;
    int32_t height;
    int32_t format;
    int32_t mode;
    uint8_t jitter;
    uint8_t pad[3];
    uint8_t reserved_1[8];
    uint16_t *pixels[2];
    uint8_t reserved_2[4];
} PharaohScreenFade;

/* Original addresses 0x00445460 and 0x00413E80. */
void pharaoh_empire_route_construct(PharaohEmpireRoute *route);
void pharaoh_screen_fade_initialize(PharaohScreenFade *fade);

uint32_t pharaoh_native_player_milestone_code_004c5190(
    int16_t first, int16_t second, int16_t third);

int32_t pharaoh_gods_ra_adjust_value(int32_t value,
    int16_t first, int16_t second, int16_t third);

void pharaoh_native_flag_from_byte_004e7e20(
    uint32_t *flag, uint8_t enabled);

void pharaoh_native_dwords_set_minus_one_00517a20(
    int32_t values[12996]);

void pharaoh_native_state_pair_reset_0041df60(
    uint32_t *first, uint32_t *second);

int32_t pharaoh_native_reference_count_004204a0(
    int32_t id, const int32_t record_ids[13],
    const int16_t *const targets[13]);

int32_t pharaoh_native_three_nonzero_count_004f5020(
    const int32_t values[3]);

uint8_t pharaoh_native_return_zero_byte_00477f60(void);

typedef struct PharaohNativeRecord90 {
    uint8_t bytes[0x90];
} PharaohNativeRecord90;
typedef struct PharaohNativeRecord30 {
    uint8_t bytes[0x30];
} PharaohNativeRecord30;
void pharaoh_native_record90_set_word_minus_one_004d1840(
    PharaohNativeRecord90 records[49], int32_t word_offset);
void pharaoh_native_record30_set_three_words_minus_one_004d1860(
    PharaohNativeRecord30 records[1000], int32_t word_offset);
void pharaoh_native_player_copy_seven_dwords_004d1880(
    uint8_t *players, int32_t count);
void pharaoh_native_player_expand_word_tables_004d1a70(uint8_t *players);
void pharaoh_native_record90_clear_byte_004d3f40(
    PharaohNativeRecord90 records[49], int32_t byte_offset);
void pharaoh_native_current_player_block_clear_004d4040(uint8_t *player);
void pharaoh_native_building_field_clear_004d4080(
    uint8_t *records, int32_t count, int32_t stride, int32_t byte_offset);
void pharaoh_native_two_words_clear_004d40a0(uint8_t *players);
void pharaoh_native_pairs_set_minus_one_004de140(int32_t pairs[4][2]);

void pharaoh_native_ambient_scores_clear_0053b670(
    uint32_t scores[12], uint32_t *active);

/* Native empty RET16; its caller discards EAX. No logical inputs. */
void pharaoh_native_noop_00489500(void);

int32_t pharaoh_native_undo_contains_004851b0(int32_t id,int32_t ready,int32_t mode,int32_t count,const int16_t values[525]);
#endif
