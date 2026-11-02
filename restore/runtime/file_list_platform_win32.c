#include "file_list.h"
#include <io.h>
int restore_file_unlink(const char *path) {return _unlink(path);}
BOOL restore_file_remove_directory(LPCSTR path) {return RemoveDirectoryA(path);}
BOOL restore_file_create_directory(LPCSTR path,LPSECURITY_ATTRIBUTES attributes)
{return CreateDirectoryA(path,attributes);}
HANDLE restore_file_find_first(LPCSTR pattern,WIN32_FIND_DATAA *entry) {return FindFirstFileA(pattern,entry);}
BOOL restore_file_find_next(HANDLE handle,WIN32_FIND_DATAA *entry) {return FindNextFileA(handle,entry);}
BOOL restore_file_find_close(HANDLE handle) {return FindClose(handle);}
