#ifndef RESTORE_WORLD_ROUTE_GEOMETRY_H
#define RESTORE_WORLD_ROUTE_GEOMETRY_H
#include <stdint.h>
#include <stddef.h>
typedef struct RestoreRoutePoint {int16_t x,y;uint8_t segment,padding;} RestoreRoutePoint;
typedef struct RestoreWorldRoute {
    uint8_t spacing,padding0[3];
    RestoreRoutePoint *samples;
    RestoreRoutePoint points[50];
    int32_t sample_count,capacity;
    int16_t first_city,last_city;
    uint8_t kind,point_count,enabled,padding1;
} RestoreWorldRoute;
typedef char restore_route_point_size[(sizeof(RestoreRoutePoint)==6)?1:-1];
typedef char restore_route_size[(sizeof(RestoreWorldRoute)==324)?1:-1];
typedef char restore_route_count_offset[(offsetof(RestoreWorldRoute,point_count)==0x141)?1:-1];
static __inline RestoreWorldRoute *restore_world_route_at(uint32_t index)
{return (RestoreWorldRoute *)(uintptr_t)(0x776fb0u+index*324u);}
uint8_t restore_world_route_bind_cities(volatile RestoreWorldRoute *self);
uint8_t restore_world_route_rebuild(volatile RestoreWorldRoute *self);
uint8_t restore_world_route_erase_point(volatile RestoreWorldRoute *self,int32_t index);
int32_t restore_world_route_hit(const volatile RestoreWorldRoute *self,int32_t x,int32_t y,uint8_t *vertex);
uint8_t FUN_00407f90(volatile RestoreWorldRoute *self);
int32_t FUN_004dd8d0(const volatile RestoreWorldRoute *self);
uint32_t FUN_004dd8e0(volatile RestoreWorldRoute *self);
void FUN_004dd900(volatile RestoreWorldRoute *self,uint8_t enabled);
int32_t FUN_004dd910(const volatile RestoreWorldRoute *self);
int32_t FUN_004dd920(const volatile RestoreWorldRoute *self);
uint8_t FUN_004dd930(volatile RestoreWorldRoute *self,int32_t index);
int32_t FUN_004ddbd0(const volatile RestoreWorldRoute *self,int32_t x,int32_t y,uint8_t *vertex);
#endif
