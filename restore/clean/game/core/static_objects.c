#include "pharaoh/core/static_objects.h"

#include <stddef.h>

void pharaoh_empire_route_construct(PharaohEmpireRoute *route)
{
    route->path_length = 0;
    route->path = NULL;
    route->point_count = 0;
    route->path_capacity = 0;
    route->in_use = 0;
    route->step = 5;
    route->first_object = -1;
    route->last_object = -1;
    route->kind = 1;
}

void pharaoh_screen_fade_initialize(PharaohScreenFade *fade)
{
    fade->format = -1;
    fade->steps = 0;
    fade->pixels[1] = NULL;
    fade->pixels[0] = NULL;
}
