#include "menu_window_cursor_consumer.h"
#define FUN_004b3250 restore_consumer_route_actual
#include CONSUMER_ROUTE_SOURCE
#undef FUN_004b3250
#define FUN_0051cd80 restore_consumer_direction_actual
#include CONSUMER_DIRECTION_SOURCE
#undef FUN_0051cd80
#define FUN_004cde60 restore_consumer_scale_actual
#include CONSUMER_SCALE_SOURCE
#undef FUN_004cde60
#define FUN_004b5520 restore_consumer_lookup_actual
#include CONSUMER_LOOKUP_SOURCE
#undef FUN_004b5520
void restore_consumer_trace(uint32_t address,uint32_t count,const uint32_t *args);
void FUN_004b3250(int32_t id)
{uint32_t args[]={(uint32_t)id};restore_consumer_trace(0x4b3250,1,args);restore_consumer_route_actual(id);}
uint8_t FUN_0051cd80(int32_t x,int32_t y,int32_t tx,int32_t ty)
{uint32_t args[]={(uint32_t)x,(uint32_t)y,(uint32_t)tx,(uint32_t)ty};restore_consumer_trace(0x51cd80,4,args);return restore_consumer_direction_actual(x,y,tx,ty);}
int32_t FUN_004cde60(int32_t value,int32_t divisor)
{uint32_t args[]={(uint32_t)value,(uint32_t)divisor};restore_consumer_trace(0x4cde60,2,args);return restore_consumer_scale_actual(value,divisor);}
int32_t FUN_004b5520(uint32_t tile)
{uint32_t args[]={tile};restore_consumer_trace(0x4b5520,1,args);return restore_consumer_lookup_actual(tile);}
