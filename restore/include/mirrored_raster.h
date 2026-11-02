#ifndef RESTORE_MIRRORED_RASTER_H
#define RESTORE_MIRRORED_RASTER_H
#include <stdint.h>
void FUN_0040fc40(const void *pixels);
char *FUN_00410f20(char *pixels);
#ifdef RESTORE_MIRRORED_RASTER_TRACE
void restore_mirrored_raster_finish(uint32_t source);
void restore_mirrored_raster_pixel(uint32_t address,uint16_t value);
#endif
#endif
