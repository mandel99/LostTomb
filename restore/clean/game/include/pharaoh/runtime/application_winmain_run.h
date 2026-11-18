#ifndef PHARAOH_RUNTIME_APPLICATION_WINMAIN_RUN_H
#define PHARAOH_RUNTIME_APPLICATION_WINMAIN_RUN_H

#include "pharaoh/runtime/application_game_shutdown.h"
#include "pharaoh/runtime/application_message_loop.h"
#include "pharaoh/runtime/application_settings_shutdown.h"
#include "pharaoh/runtime/application_shutdown.h"
#include "pharaoh/runtime/application_startup_prepare.h"

enum {
    PHARAOH_APPLICATION_ENTRY_RESET_WORD_COUNT = 0x44
};

typedef struct PharaohApplicationWinMainRunContext {
    /* The native entry clears this contiguous block of DWORD state before
       configuration; individual globals are not yet semantically named. */
    uint32_t *entry_reset_words;
    const PharaohApplicationStartupPreparationContext *startup;
    const PharaohApplicationMessageLoopContext *message_loop;
    const PharaohApplicationGameShutdownContext *game_shutdown;
    const PharaohApplicationSettingsShutdownContext *settings_shutdown;
    const PharaohApplicationShutdownContext *shutdown;
} PharaohApplicationWinMainRunContext;

/* Composes the known normal startup, message loop, and cleanup order from
   WinMain. The original SEH cleanup path and host context construction remain
   outside this owner. */
int32_t pharaoh_application_winmain_run(
    const PharaohApplicationWinMainRunContext *context);

#endif
