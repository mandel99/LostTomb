#include "text_templates.h"
const char *FUN_004cbdb0(const RestoreMessageCatalog *self,int id)
{
    return id>=0&&id<self->count?self->entries[id]:0;
}
