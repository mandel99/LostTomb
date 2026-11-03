#include "flood_settings.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(a))

void restore_flood_settings_initialize(uintptr_t self)
{
    U(self+12u)=180;U(self+16u)=70;U(self+4u)=70;U(self)=180;
    U(self+28u)=100;U(self+20u)=100;U(self+8u)=100;U(self+24u)=0;
}
void restore_flood_settings_reset(uintptr_t self)
{
    uint32_t duration=U(self),start=U(self+4u),quality;
    U(self+12u)=duration;quality=U(self+8u);U(self+16u)=start;
    U(self+20u)=quality;U(self+28u)=quality;U(self+24u)=0;
}
void restore_flood_settings_restore_timing(uintptr_t self)
{
    uint32_t duration=U(self),start=U(self+4u);
    U(self+12u)=duration;U(self+16u)=start;
}
void restore_flood_settings_reset_quality(uintptr_t self)
{U(self+28u)=U(self+8u);}
