#ifndef RESTORE_TEMPLE_COMPLEX_H
#define RESTORE_TEMPLE_COMPLEX_H
#include <stdint.h>
void restore_temple_complex_tile(int32_t x,int32_t y,uint32_t group,uint32_t image_offset);
void restore_temple_complex_decorate(uint32_t building);
void restore_temple_complex_redraw(uint32_t building,uint32_t image_offset);
void restore_temple_complex_construct(uint32_t building,int32_t x,int32_t y,uint32_t owner,uint32_t kind,uint32_t initialize);
#endif
