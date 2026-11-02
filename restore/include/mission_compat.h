#ifndef RESTORE_MISSION_COMPAT_H
#define RESTORE_MISSION_COMPAT_H
#include <stdint.h>
typedef struct RestoreDynamicHistory {
    uint8_t flag,padding[3];
    int32_t step,head,position,capacity;
    int32_t *values;
    uint32_t reserved;
} RestoreDynamicHistory;
typedef struct RestoreHistorySnapshot {
    int32_t position,capacity,step,head;
    uint8_t flag,padding[3];
    int32_t values[200];
} RestoreHistorySnapshot;
typedef char restore_dynamic_history_size[sizeof(RestoreDynamicHistory)==28?1:-1];
typedef char restore_history_snapshot_size[sizeof(RestoreHistorySnapshot)==820?1:-1];
RestoreDynamicHistory *FUN_004f6670(RestoreDynamicHistory *);
RestoreDynamicHistory *FUN_004f6690(RestoreDynamicHistory *,int32_t);
RestoreDynamicHistory *FUN_004f66c0(RestoreDynamicHistory *,const RestoreDynamicHistory *);
void FUN_004f66f0(RestoreDynamicHistory *);
uint8_t FUN_004f6700(RestoreDynamicHistory *,int32_t);
void FUN_004f6760(RestoreDynamicHistory *);
void FUN_004f6790(RestoreDynamicHistory *,const RestoreDynamicHistory *);
uint32_t FUN_004f6a50(RestoreDynamicHistory *,int32_t);
void FUN_00451a70(void);
void FUN_004d0a40(uint8_t);
void FUN_004d19e0(void);
void FUN_004d40c0(void);
void FUN_004d32f0(void);
void FUN_004f67e0(RestoreDynamicHistory *,const RestoreHistorySnapshot *);
void FUN_004f6810(RestoreDynamicHistory *,RestoreHistorySnapshot *);
#endif
