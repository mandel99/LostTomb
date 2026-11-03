#ifndef RESTORE_EVENT_PARAMETERS_H
#define RESTORE_EVENT_PARAMETERS_H
#include <stdint.h>
/* 4487F0 can retain incoming EAX on its fixed-choice arm. */
uint32_t restore_event_parameter_choose(uint32_t self,uint32_t incoming_eax);
uint32_t restore_event_record_reset(uint32_t self);
#endif
