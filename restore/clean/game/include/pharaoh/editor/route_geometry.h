#ifndef PHARAOH_EDITOR_ROUTE_GEOMETRY_H
#define PHARAOH_EDITOR_ROUTE_GEOMETRY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

enum {
    PHARAOH_EDITOR_ROUTE_POINT_LIMIT = 50
};

typedef struct PharaohEditorRoutePoint {
    int16_t x;
    int16_t y;
    uint8_t segment;
    uint8_t padding;
} PharaohEditorRoutePoint;

typedef struct PharaohEditorRoute {
    uint8_t spacing;
    uint8_t padding0[3];
    PharaohEditorRoutePoint *samples;
    PharaohEditorRoutePoint points[PHARAOH_EDITOR_ROUTE_POINT_LIMIT];
    int32_t sample_count;
    int32_t capacity;
    int16_t first_city;
    int16_t last_city;
    uint8_t kind;
    uint8_t point_count;
    uint8_t enabled;
    uint8_t padding1;
} PharaohEditorRoute;

typedef struct PharaohEditorRouteCity {
    uint8_t kind;
    uint8_t in_use;
    int16_t x;
    int16_t y;
    uint16_t image_id;
} PharaohEditorRouteCity;

typedef struct PharaohEditorRouteImage {
    int16_t width;
    int16_t height;
} PharaohEditorRouteImage;

_Static_assert(sizeof(PharaohEditorRoutePoint) == 6,
    "route points use the native six-byte stride");
_Static_assert(sizeof(PharaohEditorRoute) == 0x144 &&
    offsetof(PharaohEditorRoute, sample_count) == 0x134 &&
    offsetof(PharaohEditorRoute, capacity) == 0x138 &&
    offsetof(PharaohEditorRoute, point_count) == 0x141,
    "editor route must keep the native 324-byte layout");

/* Complete C translations of Pharaoh.exe 0x004DD8D0 through 0x004DD920. */
int32_t pharaoh_editor_route_last_point(const PharaohEditorRoute *route);
bool pharaoh_editor_route_clear(PharaohEditorRoute *route);
void pharaoh_editor_route_set_in_use(PharaohEditorRoute *route, uint8_t value);
int32_t pharaoh_editor_route_start_object(const PharaohEditorRoute *route);
int32_t pharaoh_editor_route_end_object(const PharaohEditorRoute *route);
bool pharaoh_editor_route_bind_cities(PharaohEditorRoute *route,
    const PharaohEditorRouteCity *cities, size_t city_count,
    const PharaohEditorRouteImage *images, size_t image_count);
int32_t pharaoh_editor_route_point_at(const PharaohEditorRoute *route,
    int32_t x, int32_t y, uint8_t *on_point);

#endif
