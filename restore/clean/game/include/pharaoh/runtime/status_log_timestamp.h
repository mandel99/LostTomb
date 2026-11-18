#ifndef PHARAOH_RUNTIME_STATUS_LOG_TIMESTAMP_H
#define PHARAOH_RUNTIME_STATUS_LOG_TIMESTAMP_H

#include "pharaoh/runtime/status_log_format.h"

/* Typed replacement for native helper 0x004CE8E0; the unused context matches
   PharaohStatusLogFormatOperations.refresh_timestamp_text. */
void pharaoh_status_log_refresh_timestamp_text(void *user_data,
    char timestamp_text[PHARAOH_STATUS_LOG_TIMESTAMP_TEXT_LENGTH]);

#endif
