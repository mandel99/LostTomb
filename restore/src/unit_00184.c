#include "host_allocator.h"
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define BUFFER (*(void **)(uintptr_t)0xe92e1cu)
extern void FUN_0051e930(void);
extern void FUN_0053f790(int *,char);
extern void *FUN_00561b68(size_t);
extern void FUN_004ddec0(void);
extern void FUN_004ddf20(void);
extern void FUN_004ddf90(void);
extern void FUN_004de080(void);
void FUN_00414bd0(void)
{
    uint32_t bytes;
    FUN_0051e930();
    FUN_0053f790((int *)(uintptr_t)0x1265170u,1);
    if(BUFFER) FUN_00561bdc(BUFFER);
    /* Native IMUL/SHL wrap to 32 bits before the signed minimum comparison. */
    bytes=U(0xe38dac)*U(0xe38db0)*2u;
    if((int32_t)bytes<=1200000) bytes=1200000;
    BUFFER=FUN_00561b68(bytes);
    if(U(0xf53f78)==1) FUN_004ddec0(); else FUN_004ddf20();
    FUN_004ddf90();
    FUN_004de080();
}
