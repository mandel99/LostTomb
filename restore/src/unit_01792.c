#include "restore_types.h"
#include "mission_compat.h"
void FUN_004d0a40(uint8_t load)
{
    unsigned i;
    for(i=0;i<10;++i) {
        RestoreHistorySnapshot *snapshot=(RestoreHistorySnapshot *)(0x9763b8u+i*820u);
        RestoreDynamicHistory *history=(RestoreDynamicHistory *)(0x9783c0u+i*28u);
        if(load)FUN_004f67e0(history,snapshot);
        else FUN_004f6810(history,snapshot);
    }
}
