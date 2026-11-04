#include "sound_file_lookup.h"
#include <windows.h>
void restore_sound_destroy(RestoreSound *self)
{
    __try{(void)FUN_0055e92b(self);}
    __finally{FUN_0055dbf8(&self->files);}
}
void FUN_0055e58a(RestoreSound *self)
{restore_sound_destroy(self);}
