#ifndef RESTORE_EVENT_PUBLISHER_H
#define RESTORE_EVENT_PUBLISHER_H
#include "message_panel.h"
static __inline RestoreMessageEvent *restore_event_at(uint32_t index)
{return (RestoreMessageEvent *)(uintptr_t)(0xe95228u+index*48u);}
static __inline RestoreMessageRecord *restore_event_layout(uint32_t message)
{return (RestoreMessageRecord *)(uintptr_t)(0xe209b8u+message*80u);}
void FUN_004e0cf0(int32_t message,uint32_t argument0,uint32_t argument1,const int16_t *parameters,uint8_t force);
void FUN_00417ae0(int32_t index);
void FUN_004e14b0(uint32_t message);
int32_t FUN_004e1750(void);
#endif
