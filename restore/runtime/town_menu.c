#include "town_menu.h"
static int16_t index_of(const RestoreTownRecord *record) {return *(const int16_t *)record->bytes;}
static RestoreTownRecord *neighbor(const RestoreTownRecord *from,unsigned offset)
{
    int index=*(const int16_t *)(from->bytes+offset);RestoreTownRecord *target;
    if(index<0||index>=RESTORE_TOWNS->count)return 0;
    target=&RESTORE_TOWNS->records[index];
    return (target->bytes[0x28]&1)&&index!=index_of(from)?target:0;
}
void FUN_0041e270(RestoreTownRecord *record,int depth)
{
    RestoreTownRecord *target;uint8_t kind;
    if(depth>=7)return;
    ++depth;
    do {
        record->bytes[0x6e]=1;
        target=neighbor(record,0x24);if(target)FUN_0041e270(target,depth);
        kind=record->bytes[2];
        if(kind==1||kind==23) {
            target=neighbor(record,0x26);if(target)FUN_0041e270(target,depth);
        }
        kind=record->bytes[2];
        if(kind==1||kind==23) {
            target=neighbor(record,0x52);if(target)FUN_0041e270(target,depth);
        }
        if(record->bytes[2]!=1||!record->bytes[0x60]||record->bytes[0x60]>2)return;
        target=neighbor(record,0x54);if(!target)return;
        record=target;++depth;
    }while(depth<8);
}
