#include "town_menu.h"
int FUN_0041e3b0(void)
{
    int i,result=0,count=RESTORE_TOWNS->count;RestoreTownRecord *record;
    for(i=0;i<count;++i)RESTORE_TOWNS->records[i].bytes[0x6e]=0;
    for(i=0;i<count;++i) {
        record=&RESTORE_TOWNS->records[i];
        if(record->bytes[0x28]&0x10)FUN_0041e270(record,1);
    }
    record=restore_town_next(RESTORE_TOWNS,0,0);
    while(record) {
        if(!record->bytes[0x6e])++result;
        record=restore_town_next(RESTORE_TOWNS,(const int16_t *)record->bytes,0);
    }
    return result;
}
