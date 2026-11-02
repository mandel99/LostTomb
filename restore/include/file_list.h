#ifndef RESTORE_FILE_LIST_H
#define RESTORE_FILE_LIST_H
#include <windows.h>
#include <stdint.h>
typedef struct RestoreFileList {
    uint32_t legacy_vtable;
    uint32_t *offsets;
    int32_t count,capacity;
    char *strings;
    int32_t string_capacity,used;
} RestoreFileList;
typedef char restore_file_list_size[sizeof(RestoreFileList)==28?1:-1];
typedef int (*RestoreFileCompare)(const char *,const char *);
uint8_t FUN_0041c740(RestoreFileList *,const char *,char);
uint8_t FUN_0041c8f0(RestoreFileList *,const char *,int *,int *);
void FUN_0041ca10(RestoreFileList *,RestoreFileCompare);
uint8_t FUN_0041df70(RestoreFileList *,const char *,char);
char *FUN_0041e010(RestoreFileList *,int);
int32_t FUN_0041dfc0(const RestoreFileList *,int32_t);
uint32_t FUN_0041dfe0(const RestoreFileList *,int32_t);
HANDLE restore_file_find_first(LPCSTR,WIN32_FIND_DATAA *);
BOOL restore_file_find_next(HANDLE,WIN32_FIND_DATAA *);
BOOL restore_file_find_close(HANDLE);
#define RESTORE_FAMILY_FILES ((RestoreFileList *)0x00eadfa8u)
#define RESTORE_MISSION_FILES ((RestoreFileList *)0x00704c00u)
#define RESTORE_SAVE_FILES ((RestoreFileList *)0x006057c8u)
BOOL restore_file_create_directory(LPCSTR path,LPSECURITY_ATTRIBUTES attributes);
int restore_file_unlink(const char *path);
BOOL restore_file_remove_directory(LPCSTR path);
#endif
