#include "view_state_actions.h"
uint32_t restore_view_snapshot_bounds_raw(void)
{
 uint32_t height=VIEW_U(0xe38db0u),width=VIEW_U(0xe38dacu);
 VIEW_U(0xd3b558u)=height;VIEW_U(0xd3b55cu)=0;
 VIEW_U(0xd3b554u)=0;VIEW_U(0xd3b550u)=width;
 return 0;
}
void FUN_005370d0(void){(void)restore_view_snapshot_bounds_raw();}
