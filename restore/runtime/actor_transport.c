#include "city_supply.h"
#include "../artifacts/actor_civic_followup/actor_civic_followup.h"
#include "industry_logistics.h"
#include "granary.h"
#include "storage_transactions.h"
#include "storage_trade.h"
#include "storage_delivery.h"
#include "actor_transport.h"
#include <stdlib.h>
#include <string.h>
void restore_actor_transport_raw(uint32_t target,uint32_t registers[8],uint32_t *flags)
{
 struct restore_transport_frame f;
 memcpy(f.r,registers,sizeof(f.r));f.flags=*flags;f.boundary=restore_actor_transport_dispatch;
 restore_actor_transport_dispatch(0,target,&f);
 memcpy(registers,f.r,sizeof(f.r));*flags=f.flags;
}
static uint32_t invoke(uint32_t entry,const uint32_t *args,uint32_t count)
{
 uint32_t stack[4096],sp=(uint32_t)(uintptr_t)(stack+4000),i;
 struct restore_transport_frame f={{0},2,restore_actor_transport_dispatch};
 stack[4000]=0;f.r[4]=sp;for(i=0;i<count;++i)stack[4001+i]=args[i];
 restore_actor_transport_dispatch(0,entry,&f);if(f.r[4]!=sp+4u)abort();return f.r[0];
}
int32_t FUN_00496640(int32_t actor)
{const uint32_t a[]={(uint32_t)actor};return (int32_t)invoke(0x00496640u,a,1);}

uint32_t FUN_0045acf0(int32_t param_1,uint32_t param_2,int32_t param_3,uint32_t param_4)
{return restore_storage_delivery_destination(param_1,param_2,param_3,param_4);}

void FUN_0045b880(int32_t param_1,int32_t *param_2,int32_t *param_3,int32_t *param_4,int32_t *param_5,int32_t *param_6,
                 int32_t *param_7)
{restore_storage_rank_gathering(param_1,param_2,param_3,param_4,param_5,param_6,param_7);}

int32_t FUN_0045d510(int32_t param_1,int32_t param_2)
{return restore_storage_resource_amount(param_1,param_2);}

int32_t FUN_0045d9e0(int32_t param_1,int32_t param_2,int32_t param_3,uint32_t param_4)
{return restore_food_delivery_destination(param_1,param_2,param_3,param_4);}

int32_t FUN_0045e360(int32_t param_1,int32_t param_2,int32_t param_3,int32_t param_4)
{return restore_granary_withdraw(param_1,param_2,param_3,param_4);}

int32_t FUN_0045eab0(uint32_t param_1,int32_t param_2,uint32_t param_3)
{return restore_industry_delivery_destination(param_1,param_2,param_3);}

int32_t FUN_0045f7b0(uint32_t param_1,uint32_t param_2,int32_t param_3,uint32_t param_4)
{return restore_industry_city_destination(param_1,param_2,param_3,param_4);}

int32_t FUN_00460890(int32_t param_1,int32_t param_2,int32_t param_3)
{return restore_industry_deliver(param_1,param_2,param_3);}

__declspec(noinline) void FUN_0048ae50(int32_t param_1,uint32_t param_2)
{(void)restore_actor_find_remaining_delivery_destination(param_1,param_2);}

__declspec(noinline) void FUN_0048aef0(int32_t delivered,int32_t actor,uint32_t network)
{restore_actor_apply_cart_delivery(delivered,actor,network);}

__declspec(noinline) uint32_t FUN_0048b320(uint32_t resource)
{return restore_actor_resource_cargo_image_group(resource);}

__declspec(noinline) void FUN_0048b4a0(int32_t actor)
{restore_actor_select_cargo_image(actor);}

__declspec(noinline) void FUN_0048b620(void)
{(void)restore_actor_update_cartpusher();}

__declspec(noinline) void FUN_0048ce30(void)
{(void)restore_actor_update_warehouseman();}


int32_t FUN_00518dd0(int32_t param_1,int32_t param_2,int32_t param_3,int32_t param_4)
{const uint32_t a[]={(uint32_t)(uintptr_t)param_1,(uint32_t)(uintptr_t)param_2,(uint32_t)(uintptr_t)param_3,(uint32_t)(uintptr_t)param_4};return (int32_t)invoke(0x518dd0u,a,4);}

int32_t FUN_005190a0(int32_t param_1,int32_t param_2,int32_t *param_3,int32_t param_4)
{const uint32_t a[]={(uint32_t)(uintptr_t)param_1,(uint32_t)(uintptr_t)param_2,(uint32_t)(uintptr_t)param_3,(uint32_t)(uintptr_t)param_4};return (int32_t)invoke(0x5190a0u,a,4);}

int32_t FUN_00519e30(int32_t param_1,int32_t param_2,int32_t param_3,int32_t param_4)
{const uint32_t a[]={(uint32_t)(uintptr_t)param_1,(uint32_t)(uintptr_t)param_2,(uint32_t)(uintptr_t)param_3,(uint32_t)(uintptr_t)param_4};return (int32_t)invoke(0x519e30u,a,4);}

int32_t FUN_0051a220(int32_t param_1,int32_t param_2,int32_t param_3,int32_t param_4)
{const uint32_t a[]={(uint32_t)(uintptr_t)param_1,(uint32_t)(uintptr_t)param_2,(uint32_t)(uintptr_t)param_3,(uint32_t)(uintptr_t)param_4};return (int32_t)invoke(0x51a220u,a,4);}


int32_t FUN_0051b220(int32_t param_1,int32_t param_2,int32_t param_3,int32_t param_4)
{const uint32_t a[]={(uint32_t)(uintptr_t)param_1,(uint32_t)(uintptr_t)param_2,(uint32_t)(uintptr_t)param_3,(uint32_t)(uintptr_t)param_4};return (int32_t)invoke(0x51b220u,a,4);}

int32_t FUN_0051d6d0(int32_t param_1,int32_t param_2,int32_t param_3,int32_t param_4)
{const uint32_t a[]={(uint32_t)(uintptr_t)param_1,(uint32_t)(uintptr_t)param_2,(uint32_t)(uintptr_t)param_3,(uint32_t)(uintptr_t)param_4};return (int32_t)invoke(0x51d6d0u,a,4);}
