#include "sprite_shutdown.h"
RestoreSpriteLibraries *FUN_0053da90(RestoreSpriteLibraries *self)
{
 unsigned i;for(i=0;i<30;++i)FUN_005404e0(&self->libraries[i]);self->alternate=NULL;self->scratch=NULL;(void)FUN_0053db60(self);return self;
}
