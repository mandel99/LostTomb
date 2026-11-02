#ifndef RESTORE_GENERAL_PLACEMENT_H
#define RESTORE_GENERAL_PLACEMENT_H
#include <stdint.h>
/* Logical cdecl facades for genuine ECX+three-DWORD / RET12 members.
   Full EAX is retained: only its low byte is the boolean result. */
uint32_t restore_placement_check_004e5330(uint32_t self,uint32_t cell,uint32_t pieces,int32_t count);
uint32_t restore_placement_check_005463e0(uint32_t self,uint32_t cell,uint32_t pieces,int32_t count);
/* Native stdcall3/RET12. The fourth C parameter models genuine incoming
   EAX, retained above AL on paths which inspect no active building. */
uint32_t restore_placement_existing_004e5790(uint32_t self,int32_t building_type,int32_t count,uint32_t incoming_eax);
/* Owned native-token dispatch; never calls the original machine code.
   +10 predicates have ECX/two-DWORD/RET8, AL boolean and preserved high EAX
   except4E6620 (full0/1). +0C is the actual two-instruction4E52D0 zero body. */
uint32_t restore_placement_virtual_check(uint32_t self,uint32_t terrain,int32_t piece,uint32_t incoming_eax);
uint32_t restore_placement_virtual_existing(uint32_t self,int32_t building,uint32_t mode);
#endif
