#include "placement_predicate_entries.h"
/* These entries previously existed only as native vtable tokens. Known C
   predicate expressions are shared; canonical inputs never require a fake
   object/vtable for the predicates which ignore ECX. */
uint32_t restore_placement_no_existing(void){return 0;}
uint32_t restore_placement_accept_empty(uint32_t terrain,uint32_t incoming_eax)
{return (incoming_eax&0xffffff00u)|restore_placement_empty_mask(terrain);}
uint32_t restore_placement_accept_piece(uint32_t self,uint32_t terrain,int32_t piece,uint32_t incoming_eax)
{return (incoming_eax&0xffffff00u)|restore_placement_piece_mask(self,terrain,piece);}
uint32_t restore_placement_accept_selected(uint32_t self,uint32_t terrain,int32_t piece,uint32_t incoming_eax)
{return (incoming_eax&0xffffff00u)|restore_placement_selected_mask(self,terrain,piece);}
uint32_t restore_placement_accept_two_piece(uint32_t terrain,int32_t piece)
{return restore_placement_two_piece_mask(terrain,piece);}
__declspec(naked) void FUN_004e52d0(void)
{
 __asm { push ecx }
 __asm { push edx }
 __asm { call restore_placement_no_existing }
 __asm { pop edx }
 __asm { pop ecx }
 __asm { ret 8 }
}
__declspec(naked) void FUN_004e5310(void)
{
 __asm {
  push ecx
  push edx
  push eax
  push dword ptr[esp+16]
  call restore_placement_accept_empty
  add esp,8
  pop edx
  pop ecx
  ret 8
 }
}
__declspec(naked) void FUN_00401440(void)
{
 __asm {
  push ecx
  push eax
  push dword ptr[esp+16]
  push dword ptr[esp+16]
  push ecx
  call restore_placement_accept_piece
  add esp,16
  pop ecx
  mov edx,[esp+8]
  imul edx,13
  ret 8
 }
}
__declspec(naked) void FUN_00547de0(void)
{
 __asm {
  push ecx
  push eax
  push dword ptr[esp+16]
  push dword ptr[esp+16]
  push ecx
  call restore_placement_accept_selected
  add esp,16
  pop ecx
  mov edx,[esp+4]
  and edx,0feffd76fh
  ret 8
 }
}
__declspec(naked) void FUN_004e6620(void)
{
 __asm {
  push dword ptr[esp+8]
  push dword ptr[esp+8]
  call restore_placement_accept_two_piece
  add esp,8
  mov ecx,[esp+4]
  and ecx,0feffd76fh
  mov edx,[esp+8]
  ret 8
 }
}
