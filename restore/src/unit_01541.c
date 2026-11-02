#include "restore_types.h"
#include "mission_loaders.h"
void *FUN_004bd2d0(void *object)
{
    uint8_t *base=(uint8_t *)object;
    unsigned i;
    FUN_004bd330((RestoreLoadSettings *)(base+0x32c40));
    /* Original omitted constructor 004bd650 clears these four DWORDs. */
    for(i=0;i<30;++i) {
        uint32_t *record=(uint32_t *)(base+0x32c64+i*28);
        record[0]=record[4]=record[5]=record[6]=0;
    }
    return object;
}
