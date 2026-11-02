#include <stdint.h>
extern uint32_t FUN_00489470(int32_t x,int32_t y,int32_t width,int32_t height);

/* Whole4894A0: both signed-WORD coordinates use the same264-byte record.
   Adding a byte offset to &DAT_7999E0 as a DWORD pointer read record4*id.
   Preserve the genuine query's full EAX, including its non-boolean high bits. */
uint32_t FUN_004894a0(int32_t building)
{
    uint32_t offset=(uint32_t)building*264u;
    int32_t y=*(volatile int16_t *)(uintptr_t)(0x7999e0u+offset);
    int32_t x=*(volatile int16_t *)(uintptr_t)(0x7999deu+offset);
    return FUN_00489470(x,y,3,3);
}
