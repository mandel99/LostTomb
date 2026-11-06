#include "final_state_actions.h"
#include "crt_lowio.h"
extern int32_t FUN_00568eab(const char *path,uint32_t flags,...);
extern uint32_t FUN_00562517(uint32_t descriptor);
void restore_status_append_log(uint32_t count)
{
 uint32_t append=VIEW_U(0xe92e48u);int32_t descriptor;
 descriptor=FUN_00568eab((const char *)0x5e0714u,append ? 0x8101u : 0x8301u,0x180);
 if (descriptor==-1) return;
 if (append) (void)FUN_005662e3((uint32_t)descriptor,0,2);
 (void)FUN_005664d5((uint32_t)descriptor,(const uint8_t *)0xe36c90u,count);
 (void)FUN_00562517((uint32_t)descriptor);(void)FUN_005647eb((uint32_t)descriptor);
 VIEW_U(0xe92e48u)=1;
}
void FUN_004ce860(uint32_t count){restore_status_append_log(count);}
