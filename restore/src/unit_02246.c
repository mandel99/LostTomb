#include "menu_list_scroll.h"
uint32_t FUN_00502780(void)
{
 uint32_t mode=*(volatile uint32_t *)0xe38e68u;
 if(mode==1)return 15;
 if(mode==2)return 21;
 if(mode==3)return 24;
 return 10;
}
