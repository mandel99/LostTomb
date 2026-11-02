#include <windows.h>
#include <stdint.h>
extern void FUN_005672c8(DWORD error);
extern unsigned int FUN_0056b363(unsigned int value);

uint32_t FUN_0056b2dd(const char *path)
{
    char current[0x108],drive[4];
    DWORD length;
    if(!SetCurrentDirectoryA(path)) goto failed;
    length=GetCurrentDirectoryA(0x105,current);
    if(!length) goto failed;
    if(length>=0x105) {SetLastError(ERROR_FILENAME_EXCED_RANGE);goto failed;}
    if(!((current[0]=='\\'||current[0]=='/')&&current[0]==current[1])) {
        drive[0]='=';
        drive[1]=(char)FUN_0056b363((unsigned char)current[0]);
        drive[2]=':';
        drive[3]=0;
        if(!SetEnvironmentVariableA(drive,current)) goto failed;
    }
    return 0;
failed:
    FUN_005672c8(GetLastError());
    return UINT32_MAX;
}
