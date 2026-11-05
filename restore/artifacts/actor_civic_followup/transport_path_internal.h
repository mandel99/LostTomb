#ifndef RESTORE_TRANSPORT_PATH_INTERNAL_H
#define RESTORE_TRANSPORT_PATH_INTERNAL_H
#include "transport_path_candidates.h"
#include "actor_order_path.h"
#define TP_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define TP_C(a) ((int32_t)*(volatile int8_t *)(uintptr_t)(uint32_t)(a))
#define TP_I(v) ((int32_t)(uint32_t)(v))
#endif
