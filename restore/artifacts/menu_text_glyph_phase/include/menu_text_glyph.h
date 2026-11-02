#ifndef RESTORE_MENU_TEXT_GLYPH_H
#define RESTORE_MENU_TEXT_GLYPH_H
#include <stdint.h>
int32_t FUN_004cbf30(const uint8_t *text,int32_t font);
int32_t FUN_004cca60(int32_t font,uint8_t character,int32_t x,int32_t y);
uint32_t FUN_004ccb00(void);
void FUN_004cc770(const uint8_t *text,int32_t x,int32_t y,int32_t font,uint16_t color);
void FUN_004cc720(const uint8_t *text,int32_t x,int32_t y,int32_t width,int32_t font,uint16_t color);
#endif
