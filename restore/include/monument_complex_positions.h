#ifndef RESTORE_MONUMENT_COMPLEX_POSITIONS_H
#define RESTORE_MONUMENT_COMPLEX_POSITIONS_H
#include <stdint.h>
/* Native three-DWORD/RET12 members ignore ECX. C aliases use caller cleanup.
   Full EAX and ordered output stores are preserved, including no-write arms. */
uint32_t restore_complex_position_00545a30(uint32_t building,uint32_t out_x,uint32_t out_y);
uint32_t restore_complex_position_00558f00(uint32_t building,uint32_t out_x,uint32_t out_y);
uint32_t restore_complex_position_00556230(uint32_t building,uint32_t out_x,uint32_t out_y);
uint32_t restore_complex_position_0054f8a0(uint32_t building,uint32_t out_x,uint32_t out_y);
uint32_t restore_complex_position_query(uint32_t target,uint32_t building,uint32_t out_x,uint32_t out_y);
#endif
