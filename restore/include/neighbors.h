#ifndef RESTORE_DRAG_NEIGHBORS_H
#define RESTORE_DRAG_NEIGHBORS_H
#include <stdint.h>
uint32_t FUN_00477690(uint32_t mask,uint32_t cell);
/* Native predicate resides in AL; this facade also preserves native EAX high bits. */
uint32_t FUN_00477eb0(uint32_t cell,uint32_t mask,uint8_t diagonal);
int32_t FUN_004781a0(uint32_t cell,uint32_t mask);
uint32_t FUN_0047cae0(int32_t x,int32_t y,int32_t width,int32_t height,uint32_t flags,uint32_t image_plus_one);
#endif
