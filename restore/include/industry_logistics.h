#ifndef RESTORE_INDUSTRY_LOGISTICS_H
#define RESTORE_INDUSTRY_LOGISTICS_H
#include <stdint.h>
int32_t restore_industry_delivery_destination(uint32_t actor,int32_t resource,uint32_t network);
int32_t restore_industry_producer_destination(uint32_t actor,int32_t resource,uint32_t network);
int32_t restore_industry_city_destination(uint32_t unused1,uint32_t unused2,int32_t resource,uint32_t network);
int32_t restore_industry_deliver(int32_t building,int32_t amount,int32_t resource);
int32_t FUN_0045f450(uint32_t actor,int32_t resource,uint32_t network);
#endif
