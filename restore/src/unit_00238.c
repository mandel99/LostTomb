#include "menu_timers.h"
#include "display_confirmation.h" /* Existing typed WinMM clock boundary. */
#define TIMER_U(a) (*(volatile uint32_t *)(uintptr_t)(a))
#define TIMER_B(a) (*(volatile uint8_t *)(uintptr_t)(a))
void FUN_0041aa20(uint32_t first,uint32_t mode,int32_t duration)
{
 uint32_t now;
 TIMER_U(0x6fdcb0u)=first;
 now=restore_display_confirmation_clock();
 TIMER_U(0x6fdcacu)=now;
 TIMER_U(0x6fdca8u)=now+(uint32_t)duration;
 TIMER_U(0x6fdca4u)=mode;
 TIMER_B(0xe38ddau)=4;
 TIMER_B(0xe38dcau)=1;
}
