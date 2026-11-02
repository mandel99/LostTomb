#ifndef RESTORE_NUMERIC_INPUT_H
#define RESTORE_NUMERIC_INPUT_H
#include <stdint.h>
struct restore_numeric_frame;
typedef void (*restore_numeric_call)(uint32_t,uint32_t,struct restore_numeric_frame *);
struct restore_numeric_frame {uint32_t r[8],flags;restore_numeric_call boundary;};
void restore_numeric_C_004035a0(struct restore_numeric_frame *);
void restore_numeric_C_004035f0(struct restore_numeric_frame *);
void restore_numeric_C_00403770(struct restore_numeric_frame *);
void restore_numeric_C_00403790(struct restore_numeric_frame *);
void restore_numeric_C_00403800(struct restore_numeric_frame *);
void restore_numeric_C_00403870(struct restore_numeric_frame *);
void restore_numeric_C_004038e0(struct restore_numeric_frame *);
void restore_numeric_C_004cede0(struct restore_numeric_frame *);
void restore_numeric_C_004cee00(struct restore_numeric_frame *);
void restore_numeric_C_004de160(struct restore_numeric_frame *);
void restore_numeric_C_004de200(struct restore_numeric_frame *);
/* Nullary numeric pad actions, native cdecl/RET0, no incoming self. */
void FUN_004035a0(void);
void FUN_004035f0(void);
void FUN_00403770(void);
void FUN_00403790(uint32_t x,uint32_t y,uint32_t width,uint32_t byte_target,
                  uint32_t word_target,uint32_t dword_target,uint32_t mode);
void FUN_00403870(uint32_t x,uint32_t y,uint32_t word_target,uint32_t completion);
void restore_numeric_open_dword_00403800(uint32_t x,uint32_t y,uint32_t target,uint32_t completion);
void restore_numeric_open_byte_004038e0(uint32_t x,uint32_t y,uint32_t target,uint32_t completion);
void restore_numeric_source_004de160(int32_t index);
int restore_numeric_input_action(uint32_t token);
#endif
