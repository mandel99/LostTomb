#ifndef RESTORE_MISSION_LOADERS_H
#define RESTORE_MISSION_LOADERS_H
#include <stdint.h>
typedef struct RestoreLoadSettings {int32_t values[8];} RestoreLoadSettings;
#define RESTORE_LOAD_SETTINGS ((RestoreLoadSettings *)0x00d38360u)
void FUN_004bd330(RestoreLoadSettings *);
void FUN_004bd360(RestoreLoadSettings *);
void *FUN_004bd2d0(void *);
void FUN_004bd280(void);
void FUN_004d41e0(int);
void FUN_004d4ae0(int);
uint32_t FUN_004d5100(const char *);
uint32_t FUN_004d5820(const char *,int);
uint32_t FUN_004d58b0(int,int);
#endif
