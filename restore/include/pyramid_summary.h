#ifndef RESTORE_PYRAMID_SUMMARY_H
#define RESTORE_PYRAMID_SUMMARY_H
#include <stdint.h>
struct restore_pyramid_summary_frame;
typedef void (*restore_pyramid_summary_frame_call)(uint32_t,uint32_t,struct restore_pyramid_summary_frame *);
struct restore_pyramid_summary_frame {uint32_t r[8],flags;restore_pyramid_summary_frame_call boundary;};
uint32_t FUN_004f1310(uint32_t building,int32_t output[28]);
void restore_pyramid_summary_dispatch(uint32_t site,uint32_t target,struct restore_pyramid_summary_frame *frame);
void restore_pyramid_C_004f1310(struct restore_pyramid_summary_frame *frame);
void restore_pyramid_C_004eea10(struct restore_pyramid_summary_frame *frame);
void restore_pyramid_C_004ef100(struct restore_pyramid_summary_frame *frame);
void restore_pyramid_C_004ef450(struct restore_pyramid_summary_frame *frame);
void restore_pyramid_C_004f26a0(struct restore_pyramid_summary_frame *frame);
void restore_pyramid_C_004efd90(struct restore_pyramid_summary_frame *frame);
#endif
