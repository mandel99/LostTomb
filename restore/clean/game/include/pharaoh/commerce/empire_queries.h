#ifndef PHARAOH_COMMERCE_EMPIRE_QUERIES_H
#define PHARAOH_COMMERCE_EMPIRE_QUERIES_H

#include <stdint.h>

enum {
    PHARAOH_EMPIRE_OBJECT_SIZE = 98,
    PHARAOH_EMPIRE_ROUTE_SIZE = 324,
    PHARAOH_EMPIRE_CITY_RESOURCE_SLOTS = 8
};

typedef struct PharaohEmpireObjectRecord {
    uint8_t kind;
    uint8_t reserved_01[23];
    uint8_t trade_enabled;
    uint8_t reserved_19[27];
    uint8_t bought_resources[PHARAOH_EMPIRE_CITY_RESOURCE_SLOTS];
    uint8_t reserved_3c[2];
    uint8_t trade_amounts[36];
} PharaohEmpireObjectRecord;

typedef struct PharaohEmpireRouteRecord {
    uint8_t kind;
    uint8_t reserved_01[PHARAOH_EMPIRE_ROUTE_SIZE - 1];
} PharaohEmpireRouteRecord;

/* Complete typed translations of native 0x00443040, 0x00444950 and
   0x00444A00. Native callers guarantee valid record and resource indices. */
uint32_t pharaoh_empire_object_city_buys_resource(
    const PharaohEmpireObjectRecord *objects, int32_t object_index,
    uint32_t resource_kind);
uint32_t pharaoh_empire_route_is_sea(
    const PharaohEmpireRouteRecord *routes, int32_t route_kind);
uint8_t pharaoh_empire_object_trade_amount(
    const PharaohEmpireObjectRecord *objects, int32_t object_index,
    int32_t resource_kind);

#endif
