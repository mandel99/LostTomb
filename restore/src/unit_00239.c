#include "menu_timers.h"
#include "display_confirmation.h" /* Existing typed WinMM clock boundary. */
#define TIMER_U(a) (*(volatile uint32_t *)(uintptr_t)(a))
#define TIMER_B(a) (*(volatile uint8_t *)(uintptr_t)(a))
void FUN_0041aa60(void)
{
 uint32_t now=restore_display_confirmation_clock();
 uint8_t click=TIMER_B(0xe38df8u);
 int32_t deadline=(int32_t)TIMER_U(0x6fdca8u);
 if((int32_t)now>deadline||click!=0){
  uint8_t mode=TIMER_B(0x6fdca4u);
  TIMER_B(0xe38dcau)=1;
  TIMER_B(0xe38ddau)=mode;
 }
}
