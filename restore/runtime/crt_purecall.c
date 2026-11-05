#include "crt_runtime_tail.h"
/* The authenticated native tail returns when a custom exit callback returns.
   Preserve its actual full EAX through the abstract stream method. */
uint32_t restore_crt_pure_virtual_call(void)
{
 return FUN_005630e0(25);
}
uint32_t FUN_00562cf7(void)
{
 return restore_crt_pure_virtual_call();
}
