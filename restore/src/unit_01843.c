#include "dropdown_menu.h"
int32_t FUN_004d5f60(int32_t index)
{
    if(index<0||index>4)return 10;
    return DM_I(0xe945d0u+(uint32_t)index*4u);
}
