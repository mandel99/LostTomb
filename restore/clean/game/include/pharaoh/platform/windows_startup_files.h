#ifndef PHARAOH_PLATFORM_WINDOWS_STARTUP_FILES_H
#define PHARAOH_PLATFORM_WINDOWS_STARTUP_FILES_H

#include "pharaoh/runtime/application_media_probe.h"
#include "pharaoh/runtime/file_loading.h"
#include "pharaoh/runtime/file_size.h"
#include "pharaoh/runtime/settings_ini.h"
#include "pharaoh/runtime/status_log_format.h"
#include "pharaoh/runtime/text_loading.h"

typedef struct PharaohWindowsStartupFileStatus {
    PharaohStatusLogFormatState *state;
    const PharaohStatusLogFormatOperations *operations;
} PharaohWindowsStartupFileStatus;

/* These use the same Win32 HANDLE representation as file_sizes->open_file.
   A HANDLE from that provider must never be passed to CRT _read/_lseek. */
int32_t pharaoh_windows_startup_handle_read(int32_t handle,
    void *destination, uint32_t byte_count);
int32_t pharaoh_windows_startup_handle_seek(int32_t handle,
    int32_t offset, int32_t origin);

void pharaoh_windows_startup_file_operations_bind(
    PharaohFileReadOperations *files,
    PharaohFileSizeOperations *file_sizes,
    PharaohTextFileOperations *texts,
    PharaohSettingsIniOperations *ini,
    PharaohStartupMediaProbeOperations *media);

#endif
