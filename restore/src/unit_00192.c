#include "final_state_actions.h"
extern void FUN_00560560(uint32_t context);
extern void FUN_005605c0(uint32_t context);
extern void FUN_00560580(uint32_t context);
void restore_input_capture_close(void)
{
 uint32_t context=VIEW_U(0x6fdc38u),other,i;
 if (context==0) return;
 FUN_00560560(context);
 other=VIEW_U(0x6fdc3cu);
 VIEW_U(0x6fdc40u)=0;
 if (other!=0) FUN_005605c0(other);
 context=VIEW_U(0x6fdc38u);
 if (context!=0) FUN_00560580(context);
 for (i=0;i<68;++i) VIEW_U(0x6fdb38u+i*4u)=0;
}
void FUN_00416bc0(void){restore_input_capture_close();}
