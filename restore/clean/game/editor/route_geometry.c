#include "pharaoh/editor/route_geometry.h"

int32_t pharaoh_editor_route_last_point(const PharaohEditorRoute *route)
{
    return (int32_t)route->point_count - 1;
}

bool pharaoh_editor_route_clear(PharaohEditorRoute *route)
{
    route->point_count = 0;
    route->sample_count = 0;
    return true;
}

void pharaoh_editor_route_set_in_use(PharaohEditorRoute *route, uint8_t value)
{
    route->enabled = value;
}

int32_t pharaoh_editor_route_start_object(const PharaohEditorRoute *route)
{
    return route->first_city;
}

int32_t pharaoh_editor_route_end_object(const PharaohEditorRoute *route)
{
    return route->last_city;
}

bool pharaoh_editor_route_bind_cities(PharaohEditorRoute *route,
    const PharaohEditorRouteCity *cities, size_t city_count,
    const PharaohEditorRouteImage *images, size_t image_count)
{
    route->last_city = -1;
    route->first_city = -1;
    if (route->point_count == 0u) {
        return false;
    }
    for (size_t i = 0; i < city_count; ++i) {
        const PharaohEditorRouteCity *city = &cities[i];
        if (city->in_use == 0u || city->kind != 1u) {
            continue;
        }
        const size_t image_id = city->image_id < image_count
            ? city->image_id : 0u;
        const int32_t right = (int32_t)city->x + images[image_id].width;
        const int32_t bottom = (int32_t)city->y + images[image_id].height;
        const PharaohEditorRoutePoint *first = &route->points[0];
        const PharaohEditorRoutePoint *last =
            &route->points[route->point_count - 1u];
        if (first->x >= city->x && first->x <= right &&
            first->y >= city->y && first->y <= bottom) {
            route->first_city = (int16_t)i;
        }
        if (last->x >= city->x && last->x <= right &&
            last->y >= city->y && last->y <= bottom) {
            route->last_city = (int16_t)i;
        }
    }
    return true;
}

int32_t pharaoh_editor_route_point_at(const PharaohEditorRoute *route,
    int32_t x, int32_t y, uint8_t *on_point)
{
    const int32_t left = x - 8;
    const int32_t right = x + 8;
    const int32_t top = y - 8;
    const int32_t bottom = y + 8;
    for (uint32_t i = 0; i < route->point_count; ++i) {
        const PharaohEditorRoutePoint *point = &route->points[i];
        if (point->x >= left && point->x <= right &&
            point->y >= top && point->y <= bottom) {
            *on_point = 1;
            return (int32_t)i;
        }
    }
    *on_point = 0;
    if (route->sample_count < 1) {
        return -1;
    }
    for (int32_t i = 0; i < route->sample_count; ++i) {
        const PharaohEditorRoutePoint *point = &route->samples[i];
        if (point->x >= left && point->x <= right &&
            point->y >= top && point->y <= bottom) {
            return point->segment;
        }
    }
    return -1;
}
