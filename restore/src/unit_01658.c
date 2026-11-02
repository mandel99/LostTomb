#include "text_templates.h"
extern void FUN_00561a2b(void *);
/* Whole4CBD70. Each count is cleared only when its allocation exists. */
void FUN_004cbd70(RestoreMessageCatalog *self)
{
    if(self->entries){FUN_00561a2b((void *)self->entries);self->entries=0;self->count=0;}
    if(self->unused8){FUN_00561a2b((void *)(uintptr_t)self->unused8);self->unused8=0;self->unused16=0;}
}
