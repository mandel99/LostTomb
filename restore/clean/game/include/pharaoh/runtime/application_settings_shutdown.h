#ifndef PHARAOH_RUNTIME_APPLICATION_SETTINGS_SHUTDOWN_H
#define PHARAOH_RUNTIME_APPLICATION_SETTINGS_SHUTDOWN_H

#include "pharaoh/runtime/file_writing.h"
#include "pharaoh/runtime/settings_ini.h"
#include "pharaoh/runtime/status_log_format.h"

#include <stdint.h>

enum {
    PHARAOH_MAP_INFO_SETTINGS_BYTES = 0x54
};

typedef struct PharaohSettingsShutdownMemoryOperations {
    void *user_data;
    void (*release_allocation)(void *user_data, void *allocation);
} PharaohSettingsShutdownMemoryOperations;

typedef struct PharaohApplicationSettingsShutdownContext {
    const PharaohFileWriteContext *file_writer;
    const PharaohSettingsFile *settings_file;
    const uint8_t *map_info_file_buffer;
    int32_t removed_manmades_count;
    PharaohStatusLogFormatState *status_log_state;
    const PharaohStatusLogFormatOperations *status_log_operations;
    const PharaohSettingsShutdownMemoryOperations *memory_operations;
    void *owned_allocation_1;
    void *owned_allocation_2;
    void **owned_allocation_3;
    void **owned_allocation_4;
} PharaohApplicationSettingsShutdownContext;

/* Native 0x00414750. The two trailing allocation slots are cleared after
   release; the first two retain their native pointer values. */
void pharaoh_application_settings_shutdown(
    const PharaohApplicationSettingsShutdownContext *context);

#endif
