#ifndef RESTORE_MIGRATION_FLOW_H
#define RESTORE_MIGRATION_FLOW_H
#include <stdint.h>
void restore_migration_update(void);
void restore_migration_allocate_immigrants(int32_t requested);
void restore_migration_allocate_emigrants(int32_t requested);
void restore_migration_create_immigrants(int32_t building,uint8_t people);
void restore_migration_create_emigrants(int32_t building,int32_t people);
void FUN_00458950(int32_t requested);
void FUN_00458b60(int32_t requested);
void FUN_00458d00(int32_t building,uint8_t people);
void FUN_00458d80(int32_t building,int32_t people);
#endif
