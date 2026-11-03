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
RestoreFileList *restore_file_list_construct(RestoreFileList *);
RestoreFileList *restore_file_list_construct_mission(RestoreFileList *);
void restore_file_list_destroy(RestoreFileList *);
RestoreFileList *restore_file_list_delete(RestoreFileList *,uint32_t);
void restore_file_list_destroy_frame(uint32_t,uint32_t);
int FUN_00402360(void),FUN_00402380(void);
RestoreFileList *FUN_00402370(void);
void FUN_00402390(void);
int FUN_0041e050(void),FUN_0041e070(void);
RestoreFileList *FUN_0041e060(void);
void FUN_0041e080(void);
int FUN_00516570(void),FUN_00516590(void);
RestoreFileList *FUN_00516580(void);
void FUN_005165a0(void);
/* Normalized legacy C entries keep cdecl cleanup and capture ECX/EBP before C. */
#define RESTORE_FILE_ENTRY0(name,body) \
 __declspec(naked) void name(void) { \
 __asm { push ecx } __asm { call body } __asm { add esp,4 } __asm { ret } }
#define RESTORE_FILE_ENTRY1(name,body) \
 __declspec(naked) void name(void) { \
 __asm { push dword ptr[esp+4] } __asm { push ecx } \
 __asm { call body } __asm { add esp,8 } __asm { ret } }
#define RESTORE_FILE_FRAME(name,distance) \
 __declspec(naked) void name(void) { \
 __asm { push distance } __asm { push ebp } __asm { call restore_file_list_destroy_frame } \
 __asm { add esp,8 } __asm { ret } }
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
