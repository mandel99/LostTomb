#include "restore_types.h"
undefined4 FUN_004cff20(void)
{
    MEMORYSTATUS *status=(MEMORYSTATUS *)0x00e39108u;
    memset(status,0,sizeof(*status));
    status->dwLength=sizeof(*status);
    GlobalMemoryStatus(status);
    return status->dwAvailPhys;
}
