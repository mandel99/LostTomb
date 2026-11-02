#ifndef RESTORE_WORLD_RENDER_H
#define RESTORE_WORLD_RENDER_H
#include "sprite_libraries.h"
#include <stdint.h>
void FUN_00436b60(uint32_t actor_parameter);
void FUN_00439f10(void);
int32_t restore_world_image_record(RestoreSpriteLibraries *self,int32_t image);
uint32_t restore_world_image_base(RestoreSpriteLibraries *self,int32_t group);
int32_t FUN_00438180(int32_t image);
uint32_t FUN_004381a0(int32_t group);
#endif
