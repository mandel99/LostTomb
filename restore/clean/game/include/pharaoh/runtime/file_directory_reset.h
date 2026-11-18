#ifndef PHARAOH_RUNTIME_FILE_DIRECTORY_RESET_H
#define PHARAOH_RUNTIME_FILE_DIRECTORY_RESET_H

#include "pharaoh/runtime/crt_error_translation.h"
#include "pharaoh/runtime/status_log_format.h"

typedef struct PharaohFileDirectoryResetContext {
    const char *startup_directory;
    PharaohCrtErrorState *crt_error_state;
    char *current_directory_detail;
    PharaohStatusLogFormatState *status_log_state;
    const PharaohStatusLogFormatOperations *status_log_operations;
} PharaohFileDirectoryResetContext;

/* Native entry 0x004CE2D0. The saved directory, detail buffer, and status-log
   state must persist like their original process-global counterparts. */
void pharaoh_file_directory_reset(
    const PharaohFileDirectoryResetContext *context);

#endif
