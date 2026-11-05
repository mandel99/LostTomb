#ifndef RESTORE_CITY_MIGRATION_ACTIONS_H
#define RESTORE_CITY_MIGRATION_ACTIONS_H
#include <stdint.h>
/* Logical cdecl/RET0 update. Native incidental EAX remains a RAW contract. */
void restore_city_update_migration_requests(void);
/* Cdecl/RET0, one signed DWORD kind; full EAX is cached record+12 or zero. */
uint32_t restore_city_remove_first_building_kind(int32_t kind);
uint32_t FUN_00469f80(int32_t kind);
#endif
