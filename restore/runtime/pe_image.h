#ifndef RESTORE_PE_IMAGE_H
#define RESTORE_PE_IMAGE_H
#include <windows.h>
#include <stdio.h>

typedef struct RestoreImage {
    BYTE *base;
    DWORD size;
    DWORD entry_rva;
    DWORD imported_modules;
    DWORD imported_functions;
    IMAGE_NT_HEADERS32 *nt;
    char filename[MAX_PATH];
} RestoreImage;

typedef FARPROC (*RestoreImportHook)(const char *module, const char *name, FARPROC original);
int restore_image_adopt(RestoreImage *image, const char *filename, RestoreImportHook hook, FILE *log);
int restore_image_jump(RestoreImage *image, DWORD original_va, const void *replacement);
void restore_image_close(RestoreImage *image);
#endif
