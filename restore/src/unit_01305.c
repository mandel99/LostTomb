#include "restore_types.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
/* Whole 489A80. Statistics use 32-byte records, not DWORD-scaled byte
 * displacements. Native timing divides the signed low-DWORD counter delta
 * by the zero-extended low-DWORD frequency and keeps the x87 quotient. */
void FUN_00489a80(void)
{
    uint32_t now,elapsed,row;
    if(B(0xd05710u))QueryPerformanceCounter((LARGE_INTEGER *)(uintptr_t)0xbcb628u);
    now=GetTickCount();U(0xbcb618u)=now;
    elapsed=now-U(0xbcb614u);
    row=0xbca850u+U(0xbcb610u)*32u;
    U(row)=U(row)+1u;U(row+4u)=U(row+4u)+elapsed;
    if((int32_t)elapsed>(int32_t)U(row+8u))U(row+8u)=elapsed;
    if(B(0xd05710u)){
        int32_t delta=(int32_t)(U(0xbcb628u)-U(0xbcb620u));
        uint64_t frequency=U(0xbca848u);
        __asm {
            mov edx,row
            fild dword ptr delta
            fild qword ptr frequency
            fdivp st(1),st(0)
            fld st(0)
            fadd qword ptr [edx+16]
            fstp qword ptr [edx+16]
            fcom qword ptr [edx+24]
            fnstsw ax
            test ah,41h
            jnz keep_maximum
            fstp qword ptr [edx+24]
            jmp timing_done
keep_maximum:
            fstp st(0)
timing_done:
        }
    }
}
