#include "pyramid_worker_state.h"
/* Whole native4F28B0:35 bytes,14 instructions. The out flag has its own
   BYTE slot instead of aliasing a by-value argument's low byte. */
uint8_t restore_pyramid_is_second_work_piece(int32_t building)
{
 uint8_t second;int32_t pair=restore_pyramid_find_paired_work_piece(building,&second);
 return (uint8_t)(pair>0&&second!=0);
}
uint8_t FUN_004f28b0(int32_t building){return restore_pyramid_is_second_work_piece(building);}
