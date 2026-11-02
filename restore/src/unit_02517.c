#include "sprite_libraries.h"
uint8_t FUN_0053dd10(RestoreSpriteLibraries *self,const char *const names[30])
{
    int32_t index=(int32_t)self->loaded;
    if(index<30) {
        const char *name=names[index];
        if(name&&*name)return FUN_0053dd80(self,name);
    }
    return 0;
}
