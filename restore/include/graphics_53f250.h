#ifndef RESTORE_GRAPHICS_53F250_H
#define RESTORE_GRAPHICS_53F250_H
#include "sprite_libraries.h"
/* The native method cleans three stack arguments and returns EAX=0 or 1.
   The source facade uses the ordinary C ABI, matching restored callers. */
int restore_sprite_draw_upper(RestoreSpriteLibraries *self,int image,int x,int y);
int FUN_0053f250(int image,int x,int y);
#endif
