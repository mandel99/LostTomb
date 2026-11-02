#ifndef RESTORE_ORDER_ACTIONS_FINISH_H
#define RESTORE_ORDER_ACTIONS_FINISH_H
#include <stdint.h>
#include "actor_order_action.h"
#include "actor_order_path.h"
#include "actor_order_los.h"
#define OA_U(a) (*(volatile uint32_t *)(uintptr_t)(a))
#define OA_S(a) (*(volatile int16_t *)(uintptr_t)(a))
#define OA_B(a) (*(volatile uint8_t *)(uintptr_t)(a))
#ifdef ORDER_ACTIONS_TRACE
void order_actions_store(uint32_t address,uint32_t value,uint32_t size);
#define OA_W(a,v) order_actions_store((a),(uint32_t)(v),4)
#define OA_WS(a,v) order_actions_store((a),(uint16_t)(v),2)
#define OA_WB(a,v) order_actions_store((a),(uint8_t)(v),1)
#else
#define OA_W(a,v) (OA_U(a)=(uint32_t)(v))
#define OA_WS(a,v) (*(volatile uint16_t *)(uintptr_t)(a)=(uint16_t)(v))
#define OA_WB(a,v) (OA_B(a)=(uint8_t)(v))
#endif
static uint32_t oa_actor(int32_t id){return 0xbcd5f8u+(uint32_t)id*388u;}
static uint32_t oa_group(int32_t id){return 0xbcb9c8u+(uint32_t)id*144u;}
static int oa_eligible(uint32_t group,uint32_t actor,int32_t id,int skip_five)
{
    uint8_t state=OA_B(actor+56u);
    if(state==4||(skip_five&&state==5)||OA_B(group+115u)||OA_B(group+102u)>1)return 0;
    return !(uint8_t)FUN_004b63d0(id);
}
uint32_t FUN_004b66e0(int32_t x,int32_t y,int32_t group,uint32_t task);
uint32_t FUN_004b6a40(int32_t x,int32_t y,int32_t group);
void FUN_004b6c20(int32_t group,int32_t command);
int32_t FUN_004b55f0(uint32_t tile),FUN_004b5640(uint32_t tile);
void FUN_0040c440(void),FUN_0040c570(void);
uint32_t FUN_0047d350(void);
#endif
