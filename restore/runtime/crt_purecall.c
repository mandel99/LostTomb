#include <stdint.h>
void __cdecl FUN_005630e0(int32_t error);

/* Default abstract stream method: report CRT runtime error25.
   The existing fatal-error/exit dispatcher remains an explicit dependency.
   A returning custom exit handler supplies no logical integer result. */
void restore_crt_pure_virtual_call(void)
{
 FUN_005630e0(25);
}
void FUN_00562cf7(void)
{
 restore_crt_pure_virtual_call();
}
