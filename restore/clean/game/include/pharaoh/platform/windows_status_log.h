#ifndef PHARAOH_PLATFORM_WINDOWS_STATUS_LOG_H
#define PHARAOH_PLATFORM_WINDOWS_STATUS_LOG_H

#include "pharaoh/runtime/status_log_append.h"
#include "pharaoh/runtime/status_log_format.h"

void pharaoh_windows_status_log_append_operations_initialize(
    PharaohStatusLogAppendOperations *operations);

void pharaoh_windows_status_log_format_operations_initialize(
    PharaohStatusLogFormatOperations *operations,
    const PharaohStatusLogAppendOperations *append_operations);

#endif
