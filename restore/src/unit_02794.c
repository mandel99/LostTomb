#include "crt_exception_runtime.h"

uint32_t restore_crt_notify_jump(uint32_t destination,uint32_t code,uint32_t frame)
{
 /* The debugger record is written in native code,destination,frame order. */
 *(volatile uint32_t *)(uintptr_t)0x00604930u=code;
 *(volatile uint32_t *)(uintptr_t)0x0060492cu=destination;
 *(volatile uint32_t *)(uintptr_t)0x00604934u=frame;
 return destination;
}
