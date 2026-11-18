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

void pharaoh_windows_startup_file_operations_bind(
    PharaohFileReadOperations *files,
    PharaohFileSizeOperations *file_sizes,
    PharaohTextFileOperations *texts,
    PharaohSettingsIniOperations *ini,
    PharaohStartupMediaProbeOperations *media);

#endif
