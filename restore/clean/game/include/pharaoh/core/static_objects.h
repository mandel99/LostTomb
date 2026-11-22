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

#endif
