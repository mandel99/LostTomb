#include "menu_list_scroll.h"
int32_t FUN_004cde40(int32_t value,int32_t percentage)
{
 int32_t product=(int32_t)((uint32_t)value*(uint32_t)percentage);
 return product/100;
}
