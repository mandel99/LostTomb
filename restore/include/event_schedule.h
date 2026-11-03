#ifndef RESTORE_EVENT_SCHEDULE_H
#define RESTORE_EVENT_SCHEDULE_H
#include <stdint.h>
struct restore_event_actions_frame;
int32_t restore_event_parameter_count(uint32_t self);
uint32_t restore_event_dispatch_deferred(uint32_t self,int32_t index,uint32_t incoming_eax);
void restore_event_rearm_completed(uint32_t self);
void restore_event_schedule_apply(uint32_t self,int32_t template_id,int16_t reason,struct restore_event_actions_frame *context);
/* Actual C query/action graph, sharing the caller's virtual stack. */
uint32_t restore_event_graph_query_context(uint32_t target,uint32_t self,int32_t argument,struct restore_event_actions_frame *context);
#endif
