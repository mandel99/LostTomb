#ifndef RESTORE_QUICKTIME_DISPATCH_H
#define RESTORE_QUICKTIME_DISPATCH_H
#include <windows.h>
#include <stdint.h>
/* The native callback reserves16 bytes. The game forwards this opaque record. */
typedef struct RestoreQuickTimeEvent {uint8_t bytes[16];} RestoreQuickTimeEvent;
typedef struct RestoreQuickTimeComponentParameters {
    uint8_t flags,parameter_bytes;
    int16_t request;
    uint32_t event;
} RestoreQuickTimeComponentParameters;
typedef char RestoreQuickTimeComponentBlockMustBeEightBytes[(sizeof(RestoreQuickTimeComponentParameters)==8)?1:-1];
void restore_quicktime_relocate_slots(void);
void FUN_00560360(void);
HMODULE FUN_005603f0(void);
uint32_t FUN_00560330(const MSG *message,RestoreQuickTimeEvent *event);
uint32_t FUN_005602f0(uint32_t controller,RestoreQuickTimeEvent *event);
uint32_t FUN_005605e0(uint32_t controller,const RestoreQuickTimeComponentParameters *parameters);
#endif
