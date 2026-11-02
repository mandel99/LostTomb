#ifndef RESTORE_CITY_PREPARE_H
#define RESTORE_CITY_PREPARE_H
#include <stdint.h>
typedef struct RestoreWorldRegion {uint32_t words[7];} RestoreWorldRegion;
typedef char restore_world_region_size[sizeof(RestoreWorldRegion)==28?1:-1];
#define RESTORE_CITY_WORLD ((void *)0x00d05720u)
void *FUN_004bd610(void *,int32_t,int32_t,int32_t,int32_t,RestoreWorldRegion *,uint8_t);
void FUN_004bd660(RestoreWorldRegion *);
void FUN_004bd6a0(RestoreWorldRegion *);
void *FUN_004bd740(RestoreWorldRegion *,int32_t,int32_t,int32_t,int32_t,uint8_t);
void FUN_004be0f0(void *);
void FUN_004be160(void *,int32_t,int32_t,int32_t,int32_t);
void FUN_0046f7b0(void);
uint32_t FUN_0046f930(int32_t,int32_t);
void FUN_0046fa40(void);
void FUN_0046fc30(void);
void FUN_0046fc80(void);
void FUN_00475b80(void);
uint32_t FUN_004d5100(const char *);
void FUN_004d52a0(void);
void FUN_00468170(void);
int32_t FUN_004cde60(int32_t,int32_t);
#endif
