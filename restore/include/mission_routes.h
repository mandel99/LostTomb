#ifndef RESTORE_MISSION_ROUTES_H
#define RESTORE_MISSION_ROUTES_H
#include <stdint.h>
typedef struct RestoreMissionRoute {uint8_t bytes[324];} RestoreMissionRoute;
typedef char restore_mission_route_size[sizeof(RestoreMissionRoute)==324?1:-1];
uint8_t FUN_004d4720(RestoreMissionRoute *);
#endif
