#include "pyramid_work_selection.h"
/* Whole native4EFD90:146 bytes,48 instructions. Shape1 has an alternate
   three-column row; the root of family5 has a one-column row. */
uint8_t restore_pyramid_piece_has_pending_work(int32_t building)
{
 uint32_t record=restore_monument_worker_building(building),row=MW_B(record+87u),height=MW_B(record+88u),columns,mode;
 if(row>=height)return 0;
 mode=MW_U(0x5e4ef4u);
 if(mode==1u)columns=MW_B(0x5e68d8u+MW_B(record+84u)*6u+row)?3u:6u;
 else if(mode==5u&&MW_H(record+18u)==0)columns=1u;
 else columns=6u;
 if(MW_B(record+86u)==columns&&row+1u==height)return 0;
 return 1;
}
uint8_t FUN_004efd90(int32_t building){return restore_pyramid_piece_has_pending_work(building);}
