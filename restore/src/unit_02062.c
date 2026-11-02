#include "menu_table_queries.h"
#define QUERY_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define QUERY_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
static uint32_t table_kind(void)
{
 uint8_t blocked=QUERY_B(0xea17fcu),enabled=QUERY_B(0xea1380u);
 if(blocked==0&&enabled!=0&&QUERY_U(0x784c48u)!=0){
  uint32_t offset=QUERY_U(0xeaac28u)*0x49d8u;
  if((int32_t)QUERY_U(0xea6018u+offset)>0)return 3;
 }
 return enabled!=0?2u:1u;
}
RestoreMenuButton *FUN_004ea8d0(void)
{
 uint32_t kind=table_kind();
 return (RestoreMenuButton *)(uintptr_t)(kind==3?0x5c6680u:kind==2?0x5c6648u:0x5c6628u);
}
