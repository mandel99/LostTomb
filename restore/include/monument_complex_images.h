#ifndef RESTORE_MONUMENT_COMPLEX_IMAGES_H
#define RESTORE_MONUMENT_COMPLEX_IMAGES_H
#include <stdint.h>
/* Native ECX receiver + building/output DWORDs, RET8. Public C uses caller
   cleanup and explicit self; output writes precede dependent record reads. */
uint32_t restore_complex_image_00545760(uint32_t self,uint32_t building,uint32_t output);
uint32_t restore_complex_image_00558c00(uint32_t self,uint32_t building,uint32_t output);
uint32_t restore_complex_image_00555ea0(uint32_t self,uint32_t building,uint32_t output);
uint32_t restore_complex_image_0054f490(uint32_t self,uint32_t building,uint32_t output);
uint32_t restore_complex_image_query(uint32_t target,uint32_t self,uint32_t building,uint32_t output);
#endif
