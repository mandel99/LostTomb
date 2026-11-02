#include <stdint.h>
#include <intrin.h>
/* Test-only entries intercepted before execution by the finite protocol oracle.
   The production library uses the real button and message-panel C functions. */
void FUN_00425bc0(int32_t x,int32_t y,const void *b,int32_t n,int32_t first)
{(void)x;(void)y;(void)b;(void)n;(void)first;__debugbreak();__assume(0);}
int32_t FUN_00425990(int32_t x,int32_t y,const void *b,int32_t n)
{(void)x;(void)y;(void)b;(void)n;__debugbreak();__assume(0);}
int32_t FUN_004c7db0(void){__debugbreak();__assume(0);}
