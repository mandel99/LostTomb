#ifndef PHARAOH_MILITARY_CONSTRUCTION_ALERT_H
#define PHARAOH_MILITARY_CONSTRUCTION_ALERT_H

#include <stddef.h>
#include <stdint.h>

typedef struct PharaohConstructionAlertFormation {
    uint8_t state;
    uint8_t reserved_01[0x28 - 1];
    uint8_t native_28;
    uint8_t reserved_29[0x2e - 0x29];
    int16_t x, y;
    uint8_t reserved_32[0x42 - 0x32];
    uint8_t native_42;
    uint8_t reserved_43[0x4e - 0x43];
    uint16_t alert_ticks;
    uint8_t reserved_50[0x69 - 0x50];
    uint8_t native_69;
    uint8_t reserved_6a[0x76 - 0x6a];
    uint8_t direction;
    uint8_t reserved_77[0x90 - 0x77];
} PharaohConstructionAlertFormation;

_Static_assert(sizeof(PharaohConstructionAlertFormation) == 144 &&
    offsetof(PharaohConstructionAlertFormation, native_28) == 0x28 &&
    offsetof(PharaohConstructionAlertFormation, x) == 0x2e &&
    offsetof(PharaohConstructionAlertFormation, y) == 0x30 &&
    offsetof(PharaohConstructionAlertFormation, native_42) == 0x42 &&
    offsetof(PharaohConstructionAlertFormation, alert_ticks) == 0x4e &&
    offsetof(PharaohConstructionAlertFormation, native_69) == 0x69 &&
    offsetof(PharaohConstructionAlertFormation, direction) == 0x76,
    "native construction-alert formation fields");

typedef struct PharaohConstructionAlertContext {
    PharaohConstructionAlertFormation *formations; /* NativeBCB9C8,50 rows. */
    uint32_t *distance_x, *distance_y; /* LiveD3B4FC/D3B4F8 DWORD scratch. */
} PharaohConstructionAlertContext;

/* Whole native004B9EE0, logical void. Fixed pool indices1..49; entry0 is
 * excluded. Distance subtraction wraps32bits, while max/radius tests remain
 * signed as native. No extra bounds/count policy is introduced. */
void pharaoh_construction_alert_formations(
    const PharaohConstructionAlertContext *,int32_t x,int32_t y);

#endif
