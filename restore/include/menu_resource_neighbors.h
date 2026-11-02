#ifndef RESTORE_MENU_RESOURCE_NEIGHBORS_H
#define RESTORE_MENU_RESOURCE_NEIGHBORS_H
#include <stdint.h>
struct restore_resource_neighbors_frame;
typedef void (*restore_resource_neighbors_call)(uint32_t,uint32_t,struct restore_resource_neighbors_frame *);
struct restore_resource_neighbors_frame {uint32_t r[8],flags;restore_resource_neighbors_call boundary;};
void restore_resource_neighbors_dispatch(uint32_t site,uint32_t target,struct restore_resource_neighbors_frame *frame);
void restore_resource_C_00480180(struct restore_resource_neighbors_frame *frame);
void restore_resource_C_004802c0(struct restore_resource_neighbors_frame *frame);
void restore_resource_C_00480460(struct restore_resource_neighbors_frame *frame);
void restore_resource_C_004806a0(struct restore_resource_neighbors_frame *frame);
uint32_t FUN_00480180(int32_t x,int32_t y,uint32_t orientation);
uint32_t FUN_004802c0(int32_t x,int32_t y);
uint32_t FUN_00480460(int32_t x,int32_t y,uint32_t orientation);
uint32_t FUN_004806a0(int32_t x,int32_t y);
#endif
