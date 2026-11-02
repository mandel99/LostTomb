#ifndef RESTORE_MEDIA_LOADER_H
#define RESTORE_MEDIA_LOADER_H
#include <windows.h>
/* Called before media imports, with the launcher's selected absolute asset
   directory. NULL clears the optional location. No DLL is copied or loaded. */
int restore_media_set_game_directory(const wchar_t *directory);
FARPROC restore_media_resolve(const char *dll,const char *export_name);
#endif
