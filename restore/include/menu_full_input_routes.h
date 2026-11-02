#ifndef RESTORE_MENU_FULL_INPUT_ROUTES_H
#define RESTORE_MENU_FULL_INPUT_ROUTES_H
#include <stdint.h>
#ifndef MIR_PUBLIC
#define MIR_PUBLIC(name) name
#endif
/* Native ECX receivers are explicit Win32 addresses; old generated exports
   and their other callers remain outside this new facade graph. */
int32_t restore_menu_route_last(uint32_t self);
uint32_t restore_menu_route_clear(uint32_t self);
void restore_menu_route_mark(uint32_t self,uint32_t value);
int32_t restore_menu_route_start_city(uint32_t self);
int32_t restore_menu_route_end_city(uint32_t self);
uint8_t restore_menu_route_endpoints(uint32_t self);
uint8_t restore_menu_route_delete(uint32_t self,int32_t index);
int32_t restore_menu_route_hit(uint32_t self,int32_t x,int32_t y,uint8_t *vertex);
#endif
