#include "placement_context.h"
#include "general_placement.h"

/* Each check is native thiscall/RET4; each existing query is thiscall/RET0.
 * The complete result from the shared routine is returned, including EAX's
 * upper bytes. Object offsets and counts come from the original instructions.
 * These explicit-self entries must not replace legacy cdecl symbols in isolation.
 */
#define CONTEXT(check,existing,provider,offset,count,kind) \
 uint32_t restore_placement_check_##check(uint32_t self,uint32_t cell) \
 {return provider(self,cell,self+(offset),(count));} \
 uint32_t restore_placement_existing_##existing(uint32_t self,uint32_t incoming_eax) \
 {return restore_placement_existing_004e5790(self,(kind),(count),incoming_eax);}

CONTEXT(00401400,00401430,restore_placement_check_004e5330,0x4cu,0x10,0xe6)
CONTEXT(0040d850,0040d880,restore_placement_check_004e5330,0x4cu,0x19,0xe1)
CONTEXT(00416f70,00416fa0,restore_placement_check_004e5330,0x4cu,0x09,0xe3)
CONTEXT(004e65f0,004e6650,restore_placement_check_004e5330,0x4cu,0x02,0xe4)
CONTEXT(005456c0,00545700,restore_placement_check_005463e0,0x1e5cu,0xa7,0xe5)
CONTEXT(0054f3f0,0054f430,restore_placement_check_005463e0,0x5c88u,0x201,0xec)
CONTEXT(00555e00,00555e40,restore_placement_check_005463e0,0x4ee0u,0x1b5,0xeb)
CONTEXT(00558b60,00558ba0,restore_placement_check_005463e0,0x1cbcu,0x9e,0xea)
#undef CONTEXT
