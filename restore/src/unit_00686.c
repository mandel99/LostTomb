#include "menu_record_queries.h"

void FUN_00448cf0(void)

{
  uint8_t mode=*(volatile uint8_t *)(uintptr_t)0x00785d6cu;
  *(volatile uint8_t *)(uintptr_t)0x00e38dcau=1;
  *(volatile uint8_t *)(uintptr_t)0x00e38ddau=mode;
}
