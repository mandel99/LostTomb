#ifndef RESTORE_UPPER_RASTER_H
#define RESTORE_UPPER_RASTER_H
#include <stdint.h>
void FUN_00410230(const void *pixels);
uint32_t FUN_004105c0(const void *pixels);
uint32_t FUN_00410660(const void *pixels);
void FUN_00410700(const void *pixels,uint16_t mask);
void FUN_004109d0(const void *pixels,uint16_t mask);
void FUN_00410a70(const void *pixels,uint16_t mask);
void FUN_00410b10(const void *pixels);
uint32_t FUN_00410de0(const void *pixels);
uint32_t FUN_00410e80(const void *pixels);
char *FUN_00410f20(char *pixels);
void restore_upper_rle(const void *pixels,int side,int operation,uint16_t mask);
#endif
