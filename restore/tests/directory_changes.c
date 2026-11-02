#include <windows.h>
#include <stdint.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
extern uint32_t FUN_0056b2dd(const char *path);
static DWORD mapped_error;
static int checks;
void FUN_005672c8(DWORD error) {mapped_error=error;}
unsigned int FUN_0056b363(unsigned int value) {return (unsigned int)toupper((unsigned char)value);}
#define REQUIRE(x) do {++checks;if(!(x)){fprintf(stderr,"Directory change failure at %d, Windows error %lu\n",__LINE__,GetLastError());ExitProcess(1);}}while(0)
int main(void)
{
    char original[32768],temp[MAX_PATH],folder[MAX_PATH],actual[32768],drive[4],saved[32768],variable[32768];
    DWORD saved_length;size_t n,i;
    REQUIRE(GetCurrentDirectoryA(sizeof(original),original)>0);
    REQUIRE(GetTempPathA(sizeof(temp),temp)>0);
    snprintf(folder,sizeof(folder),"%srestore-dir-%lu-",temp,GetCurrentProcessId());
    n=strlen(folder);for(i=n;i<230;++i)folder[i]='a'+(char)(i%26);folder[230]=0;
    REQUIRE(CreateDirectoryA(folder,0));
    drive[0]='=';drive[1]=(char)toupper((unsigned char)folder[0]);drive[2]=':';drive[3]=0;
    saved_length=GetEnvironmentVariableA(drive,saved,sizeof(saved));
    REQUIRE(FUN_0056b2dd(folder)==0&&mapped_error==0);
    REQUIRE(GetCurrentDirectoryA(sizeof(actual),actual)>0&&!_stricmp(actual,folder));
    REQUIRE(GetEnvironmentVariableA(drive,variable,sizeof(variable))>0&&!_stricmp(variable,folder));
    REQUIRE(FUN_0056b2dd(".")==0);
    REQUIRE(FUN_0056b2dd("missing")==UINT32_MAX);
    REQUIRE(mapped_error==ERROR_FILE_NOT_FOUND||mapped_error==ERROR_PATH_NOT_FOUND);
    REQUIRE(GetCurrentDirectoryA(sizeof(actual),actual)>0&&!_stricmp(actual,folder));
    REQUIRE(FUN_0056b2dd(original)==0);
    REQUIRE(RemoveDirectoryA(folder));
    REQUIRE(SetEnvironmentVariableA(drive,saved_length?saved:0));
    printf("Directory changes: %d checks; long local path, drive environment, relative path, failure preservation and cleanup.\n",checks);
    return 0;
}
