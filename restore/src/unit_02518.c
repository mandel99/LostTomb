#include "sprite_libraries.h"
void FUN_0053dd40(RestoreSpriteLibraries *self,const char *const names[30])
{
    uint8_t result=1;
    do {
        int32_t index=(int32_t)self->loaded;
        const char *name;
        if(index>=30)return;
        name=names[index];
        if(name&&*name)result=FUN_0053dd80(self,name);
    } while(result);
}
