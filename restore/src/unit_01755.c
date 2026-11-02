#include "legacy_streams.h"
int FUN_004ce9e0(void)
{
    /* User-requested local-data policy: never wait for optical media. Native
       callers consume MessageBox's int result (IDOK retries, IDCANCEL stops). */
    fputs("Missing local game data: CD retry disabled.\n",stderr);
    return IDCANCEL;
}
