#ifndef RESTORE_FERRY_PREVIEW_H
#define RESTORE_FERRY_PREVIEW_H
#include <stdint.h>
#define FRY_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define FRY_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define FRY_H(a) ((int32_t)(int16_t)FRY_W(a))
#define FRY_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define FRY_I(a) ((int32_t)(uint32_t)(a))
void restore_ferry_scan_crossing(int32_t x,int32_t y);
int32_t restore_ferry_image_orientation(int32_t orientation);
uint8_t restore_ferry_highlight_approaches(int32_t orientation,int32_t x,int32_t y,int32_t screen_x,int32_t screen_y,int32_t length);
void restore_ferry_preview(void);
void FUN_0047f3e0(int32_t x,int32_t y);
int32_t FUN_0043d010(int32_t orientation);
uint8_t FUN_0043d030(int32_t orientation,int32_t x,int32_t y,int32_t screen_x,int32_t screen_y,int32_t length);
void FUN_0043d300(void);
#endif
