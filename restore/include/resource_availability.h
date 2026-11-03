#ifndef RESTORE_RESOURCE_AVAILABILITY_H
#define RESTORE_RESOURCE_AVAILABILITY_H
#include <stdint.h>
struct restore_resource_availability_frame;
typedef void (*restore_resource_availability_call)(uint32_t,uint32_t,struct restore_resource_availability_frame *);
struct restore_resource_availability_frame {uint32_t r[8],flags;restore_resource_availability_call boundary;};
/* Boolean result is AL; the DWORD API also preserves native high EAX bits. */
uint32_t restore_resource_event_supply(int32_t resource);
void restore_resource_lists_rebuild(void);
uint32_t FUN_00559480(int32_t resource);
void FUN_005596d0(void);
void restore_avail_C_00559480(struct restore_resource_availability_frame *);
void restore_avail_C_005596d0(struct restore_resource_availability_frame *);
void restore_resource_availability_dispatch(uint32_t,uint32_t,struct restore_resource_availability_frame *);
#endif
