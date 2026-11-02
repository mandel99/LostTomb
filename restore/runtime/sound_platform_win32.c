#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "sound_initialization.h"
int restore_sound_version(uint32_t *platform,uint32_t *major)
{
    OSVERSIONINFOA version={0};version.dwOSVersionInfoSize=sizeof(version);
    if(!GetVersionExA(&version))return 0;
    *platform=version.dwPlatformId;*major=version.dwMajorVersion;return 1;
}
void restore_sound_debug(const char *message) {OutputDebugStringA(message);}
