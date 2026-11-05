#ifndef RESTORE_CITY_WORKER_UPDATES_H
#define RESTORE_CITY_WORKER_UPDATES_H
#include <stdint.h>

/* Native 4612C0/461490/4615D0/461720 are cdecl logical-void updates.
   The physical models retain their incidental GPR/flags results. */
void restore_city_update_delayed_stock_worker(int32_t building);
void restore_city_update_service_worker(int32_t building);
void restore_city_update_stock_reset_worker(int32_t building);
void restore_city_update_dual_stock_worker(int32_t building);
#endif
