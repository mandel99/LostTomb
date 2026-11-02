#include "camera_pan.h"
#include <windows.h>
#include <mmsystem.h>
uint32_t FUN_004ddd60(void)
{
    uint32_t now=timeGetTime();
    int32_t elapsed=(int32_t)(now-CP_U(0xe94db4));
    int32_t setting=(int32_t)(100u-CP_U(0xe38e70))/10;
    if(elapsed<0)elapsed=10000;
    if(setting<10 && elapsed>=(int32_t)((uint32_t)setting*12u+2u)){
        CP_U(0xe94db4)=now;
        return 1;
    }
    return 0;
}
