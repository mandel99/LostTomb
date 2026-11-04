#ifndef RESTORE_MESSAGE_CATEGORY_H
#define RESTORE_MESSAGE_CATEGORY_H
#include <stdint.h>
/* Category meanings remain address based until message text is reconstructed. */
uint32_t restore_actor_building_work_state(uint32_t actor);
uint32_t restore_message_build_category_weights(volatile uint8_t *weights,volatile uint8_t *all_defaults);
uint32_t restore_message_choose_weighted_category(void);
uint32_t FUN_0053d3a0(volatile uint8_t *weights,volatile uint8_t *all_defaults);
uint32_t FUN_00495b60(uint32_t actor);
uint32_t FUN_0053c060(void);
#endif
