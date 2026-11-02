#ifndef RESTORE_WIDGET_POSITION_H
#define RESTORE_WIDGET_POSITION_H
#include <stdint.h>
struct restore_widget_position_frame;
typedef void (*restore_widget_position_frame_call)(uint32_t,uint32_t,struct restore_widget_position_frame *);
struct restore_widget_position_frame {uint32_t r[8],flags;restore_widget_position_frame_call boundary;};
void restore_widget_C_0041f910(struct restore_widget_position_frame *frame);
void restore_widget_C_0041fad0(struct restore_widget_position_frame *frame);
/* Raw adapter consumes the genuine RET12/RET16 in the supplied VM stack. */
void restore_widget_position_raw(uint32_t target,uint32_t registers[8],uint32_t *flags);
void restore_widget_position_0041f910(uint32_t self,int32_t *out_x,int32_t *out_y,uint32_t *out_image);
void restore_widget_position_0041fad0(uint32_t self,int32_t *out_x,int32_t *out_y,uint32_t *out_image,int32_t direction);
#endif
