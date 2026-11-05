#include "crt_runtime_init.h"

/* 5640FE is a hardware x87 test, not a constant zero. Keep both binary64
   spills, extended intermediates, current PC/RC/exception behavior and the
   final FCOMP. C0/C3 implement the native SAHF/JBE, including unordered. */
uint32_t restore_crt_probe_fdiv_erratum(void)
{
 double divisor,numerator,residual;
 uint16_t status=0;
 __asm {
  mov eax,05706b8h
  fld qword ptr[eax]
  fstp divisor
  mov eax,05706b0h
  fld qword ptr[eax]
  fstp numerator
  fld numerator
  fdiv divisor
  fmul divisor
  fsubr numerator
  fstp residual
  fld residual
  mov eax,056f520h
  fcomp qword ptr[eax]
  fnstsw ax
  mov status,ax
 }
 return(status&0x4100u)==0?1u:0u;
}
uint32_t FUN_005640fe(void){return restore_crt_probe_fdiv_erratum();}
