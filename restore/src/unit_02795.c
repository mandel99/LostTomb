#include "crt_local_unwind.h"

uint32_t restore_crt_notify_frame(uint32_t destination,uint32_t parent_frame)
{
 uint32_t code=*(volatile uint32_t *)(uintptr_t)(parent_frame+8u);
 return restore_crt_notify_jump(destination,code,parent_frame);
}
