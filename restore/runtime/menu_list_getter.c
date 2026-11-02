#include "menu_list_getter.h"
uint8_t restore_menu_list_can_open(uint32_t self)
{
 int32_t selected=(int8_t)*(volatile uint8_t *)(uintptr_t)0xe38eaau;
 uint32_t record;
 if(selected<0)return 0;
 if(selected>=*(volatile int32_t *)(uintptr_t)(self+0x1900u))return 0;
 record=self+((uint32_t)selected<<6);
 if(!record)return 0;
 return (uint8_t)(*(volatile int32_t *)(uintptr_t)(record+0x38u)>1);
}
