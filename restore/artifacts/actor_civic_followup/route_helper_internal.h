#ifndef RESTORE_ROUTE_HELPER_INTERNAL_H
#define RESTORE_ROUTE_HELPER_INTERNAL_H
#include "route_helper_candidates.h"
#include "actor_order_path.h"
#define RH_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define RH_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define RH_C(a) ((int32_t)*(volatile int8_t *)(uintptr_t)(uint32_t)(a))
#define RH_I(v) ((int32_t)(uint32_t)(v))
#endif
