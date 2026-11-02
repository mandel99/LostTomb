#ifndef RESTORE_MENU_PREPARE_INTEGER_H
#define RESTORE_MENU_PREPARE_INTEGER_H
#include <stdint.h>
struct restore_prepare_integer_frame;
typedef void (*restore_prepare_integer_call)(uint32_t,uint32_t,struct restore_prepare_integer_frame *);
struct restore_prepare_integer_frame {uint32_t r[8],flags;restore_prepare_integer_call boundary;};
void restore_prepare_integer_dispatch(uint32_t site,uint32_t target,struct restore_prepare_integer_frame *frame);
void restore_prepare_C_00454190(struct restore_prepare_integer_frame *frame);
void restore_prepare_C_0045d570(struct restore_prepare_integer_frame *frame);
void restore_prepare_C_00475b50(struct restore_prepare_integer_frame *frame);
void restore_prepare_C_004b7590(struct restore_prepare_integer_frame *frame);
void restore_prepare_C_0050f0f0(struct restore_prepare_integer_frame *frame);
uint32_t FUN_00454190(uint32_t building);
uint32_t FUN_0045d570(uint32_t building);
uint32_t FUN_00475b50(uint32_t building);
uint32_t FUN_004b7590(void);
uint32_t FUN_0050f0f0(void);
#endif
