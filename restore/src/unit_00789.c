#include "native_member_bridge.h"
#include "menu_advisor_actions.h"
/* Reuse complete advisor C; native consumers test AL and then replace EAX.
   The actual incoming upper EAX and all real caller context stay captured. */
RESTORE_NATIVE_MEMBER_ENTRY_TYPED(uint8_t,FUN_00451b10,restore_advisor_scan_building_issues_00451b10,0x00451b10,0,restore_advisor_raw)
