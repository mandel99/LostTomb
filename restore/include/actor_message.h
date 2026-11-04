#ifndef RESTORE_ACTOR_MESSAGE_H
#define RESTORE_ACTOR_MESSAGE_H
#include <stdint.h>
/* Updates actor message/category bytes and EADCDC. The original caller ignores
   EAX; the result interface preserves it for register-frame boundaries. */
uint32_t restore_actor_update_message_result(int32_t actor,uint32_t incoming_eax);
void restore_actor_update_message(int32_t actor);
#endif
