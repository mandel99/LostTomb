#ifndef RESTORE_RECORD_SCORE_VIEWS_H
#define RESTORE_RECORD_SCORE_VIEWS_H
#include <stdint.h>
/* Fixed native views. All address calculations wrap at32 bits. */
#define SCORE_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define SCORE_W(a) (*(volatile const uint16_t *)(uintptr_t)(uint32_t)(a))
#define SCORE_S(a) (*(volatile const int16_t *)(uintptr_t)(uint32_t)(a))
#define SCORE_B(a) (*(volatile const uint8_t *)(uintptr_t)(uint32_t)(a))
#define SCORE_C(a) (*(volatile const int8_t *)(uintptr_t)(uint32_t)(a))
#define SCORE_CANONICAL(Address) \
 int32_t FUN_##Address(int32_t index) \
 { return restore_record_score_##Address(index); }
#endif
