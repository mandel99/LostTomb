#ifndef RESTORE_MONUMENT_IMAGES_H
#define RESTORE_MONUMENT_IMAGES_H
#include "building_methods.h"
/* Native members: ECX=self, building/output DWORDs, RET8. C aliases use
   caller cleanup. Nine-part reads outside its 36-slot table need the native
   frame context; the game dispatcher supplies it rather than inventing data. */
uint32_t restore_monument_image_00401800(uint32_t self,uint32_t building,uint32_t output);
uint32_t restore_monument_image_0040de80(uint32_t self,uint32_t building,uint32_t output);
uint32_t restore_monument_image_00417310(uint32_t self,uint32_t building,uint32_t output);
uint32_t restore_monument_image_004e6a70(uint32_t self,uint32_t building,uint32_t output);
uint32_t restore_monument_image_query(uint32_t target,uint32_t self,uint32_t building,uint32_t output,struct restore_menu_input_frame *context);
#endif
