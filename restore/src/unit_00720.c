#include "town_menu.h"
RestoreTownRecord *restore_town_next(RestoreTownList *self,const int16_t *previous,uint8_t create)
{
    int first=previous?*previous+1:0,index=first;
    *(uint8_t *)0x78a634u=1;
    for(;index<self->count;++index)if(self->records[index].bytes[2]==2) {
        *(int16_t *)self->records[index].bytes=(int16_t)index;
        *(int32_t *)0x5d9a40u=index;return &self->records[index];
    }
    if(create&&first<140) {
        *(int16_t *)self->records[index].bytes=(int16_t)index;
        *(int32_t *)0x5d9a40u=index;return &self->records[140];
    }
    --*(uint32_t *)0x5d9a40u;return 0;
}
/* All 24 native callers select ECX=0x785d70. Keep the existing C facade. */
int *FUN_0044bb90(const int16_t *previous,char create)
{
    return (int *)restore_town_next(RESTORE_TOWNS,previous,(uint8_t)create);
}
