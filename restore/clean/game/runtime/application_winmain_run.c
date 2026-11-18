#include "pharaoh/runtime/application_winmain_run.h"

#include <string.h>

static LONG startup_exception_filter(
    volatile int32_t exception_filter_active,
    const PharaohApplicationMessageLoopContext *message_loop)
{
    if (exception_filter_active != 0 && message_loop != NULL &&
        message_loop->window != NULL) {
        const HWND window = *message_loop->window;
        if (window != NULL) {
            (void)ShowWindow(window, SW_MINIMIZE);
        }
    }
    return EXCEPTION_CONTINUE_SEARCH;
}

int32_t pharaoh_application_winmain_run(
    const PharaohApplicationWinMainRunContext *context)
{
    volatile int32_t exception_filter_active = 0;
    PharaohApplicationStartupPreparationContext startup = *context->startup;
    startup.native_exception_filter_active = &exception_filter_active;

    __try {
        memset(context->entry_reset_words, 0,
            PHARAOH_APPLICATION_ENTRY_RESET_WORD_COUNT *
                sizeof *context->entry_reset_words);
        *startup.runtime->native_message_loop_state = 0;

        const PharaohApplicationStartupPreparationResult startup_result =
            pharaoh_application_startup_prepare_load(&startup);

        if (startup_result.status ==
                PHARAOH_APPLICATION_STARTUP_SETTINGS_FAILED ||
            startup_result.status == PHARAOH_APPLICATION_STARTUP_TEXT_FAILED ||
            startup_result.status == PHARAOH_APPLICATION_STARTUP_WINDOW_FAILED) {
            return 0;
        }

        int32_t exit_code = 0;
        if (startup_result.status == PHARAOH_APPLICATION_STARTUP_PREPARED) {
            exit_code = pharaoh_application_message_loop_run(
                context->message_loop);
        }

        pharaoh_application_game_shutdown(context->game_shutdown);
        pharaoh_application_settings_shutdown(context->settings_shutdown);
        pharaoh_application_shutdown_finish(context->shutdown);
        return exit_code;
    } __except (startup_exception_filter(exception_filter_active,
                   context->message_loop)) {
        /* The native filter always continues exception search. */
        return 0;
    }
}
