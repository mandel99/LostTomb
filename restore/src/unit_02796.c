#include "crt_runtime_init.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))

void restore_crt_initialize_floating_runtime(void)
{
 FUN_005618ae();
 U(0x12663c4u)=restore_crt_query_fdiv_erratum();
 FUN_005640ec();
 /* Native FNCLEX is nonwaiting and affects x87, not SSE state. */
 __asm fnclex
}
void FUN_00561896(void){restore_crt_initialize_floating_runtime();}
