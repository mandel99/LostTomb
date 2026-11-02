#ifndef RESTORE_MENU_TRAINING_RENDER_H
#define RESTORE_MENU_TRAINING_RENDER_H
#include <stdint.h>
struct restore_training_render_frame;
typedef void (*restore_training_render_call)(uint32_t,uint32_t,struct restore_training_render_frame *);
struct restore_training_render_frame {uint32_t r[8],flags;restore_training_render_call boundary;};
/* Whole integer native bodies, cdecl/RET0, no implicit incoming receiver.
 * Real dropdown callbacks and font/sprite/layout providers are composed;
 * pixel, OS-clock and dialog_loc file fidelity remain qualified. */
void FUN_00559e10(void);
uint32_t FUN_005599c0(int32_t resource);
int32_t FUN_00559fc0(int32_t counts[16]);
void FUN_0055a060(void);
void FUN_0055a0e0(void);
void FUN_0040c760(void);
/* Required central callback-router integration for the two real scroll buttons. */
int restore_training_render_action(uint32_t token);
void FUN_004cd680(int32_t x,int32_t y,int32_t width,int32_t height,int32_t color);
void FUN_004d65b0(int32_t x,int32_t y,int32_t columns,int32_t rows,int32_t origin_x,int32_t origin_y);
void FUN_00536a30(int32_t x,int32_t y,int32_t columns,int32_t rows);
void restore_training_render_dispatch(uint32_t site,uint32_t target,struct restore_training_render_frame *f);
void restore_training_render_C_0040c760(struct restore_training_render_frame *f);
void restore_training_render_C_004cd680(struct restore_training_render_frame *f);
void restore_training_render_C_004d65b0(struct restore_training_render_frame *f);
void restore_training_render_C_00536a30(struct restore_training_render_frame *f);
void restore_training_render_C_00559e10(struct restore_training_render_frame *f);
void restore_training_render_C_00559fc0(struct restore_training_render_frame *f);
void restore_training_render_C_0055a060(struct restore_training_render_frame *f);
void restore_training_render_C_0055a0e0(struct restore_training_render_frame *f);
#endif
