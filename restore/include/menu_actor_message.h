#ifndef RESTORE_MENU_ACTOR_MESSAGE_H
#define RESTORE_MENU_ACTOR_MESSAGE_H
#include <stdint.h>
struct restore_actor_message_frame;
typedef void (*restore_actor_message_call)(uint32_t,uint32_t,struct restore_actor_message_frame *);
/* Internal x86 frame: consumed-register contracts at typed providers are explicit in dispatch. */
struct restore_actor_message_frame {uint32_t r[8],flags;restore_actor_message_call boundary;};
void restore_actor_message_dispatch(uint32_t site,uint32_t target,struct restore_actor_message_frame *frame);
void restore_actor_message_C_004434e0(struct restore_actor_message_frame *frame);
void restore_actor_message_C_004435f0(struct restore_actor_message_frame *frame);
void restore_actor_message_C_0044d6d0(struct restore_actor_message_frame *frame);
void restore_actor_message_C_0044d700(struct restore_actor_message_frame *frame);
void restore_actor_message_C_00459240(struct restore_actor_message_frame *frame);
void restore_actor_message_C_0045b230(struct restore_actor_message_frame *frame);
void restore_actor_message_C_0045cf50(struct restore_actor_message_frame *frame);
void restore_actor_message_C_0045d180(struct restore_actor_message_frame *frame);
void restore_actor_message_C_0045d510(struct restore_actor_message_frame *frame);
void restore_actor_message_C_004893c0(struct restore_actor_message_frame *frame);
void restore_actor_message_C_00489470(struct restore_actor_message_frame *frame);
void restore_actor_message_C_004894a0(struct restore_actor_message_frame *frame);
void restore_actor_message_C_00495b60(struct restore_actor_message_frame *frame);
void restore_actor_message_C_004bf9d0(struct restore_actor_message_frame *frame);
void restore_actor_message_C_0053c060(struct restore_actor_message_frame *frame);
void restore_actor_message_C_0053c100(struct restore_actor_message_frame *frame);
void restore_actor_message_C_0053d3a0(struct restore_actor_message_frame *frame);
void FUN_0053c100(int32_t actor);
uint32_t FUN_00495b60(uint32_t actor);
uint32_t FUN_0053c060(void);
#endif
