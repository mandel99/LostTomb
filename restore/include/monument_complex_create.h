#ifndef RESTORE_MONUMENT_COMPLEX_CREATE_H
#define RESTORE_MONUMENT_COMPLEX_CREATE_H
#include "monument_create.h"
/* ECX receiver, four DWORD arguments, native RET16; skip uses only AL. */
void restore_complex_create_00545c30(uint32_t self,int32_t x,int32_t y,uint32_t root,uint8_t skip);
void restore_complex_create_00559130(uint32_t self,int32_t x,int32_t y,uint32_t root,uint8_t skip);
void restore_complex_create_00556480(uint32_t self,int32_t x,int32_t y,uint32_t root,uint8_t skip);
void restore_complex_create_0054fba0(uint32_t self,int32_t x,int32_t y,uint32_t root,uint8_t skip);
void restore_complex_create_frame(uint32_t target,struct restore_menu_input_frame *frame);
#endif
