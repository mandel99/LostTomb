#ifndef RESTORE_VIDEO_PLATFORM_H
#define RESTORE_VIDEO_PLATFORM_H
#include <windows.h>
/* Platform boundary permits replay of movie/message sequences without a
   window, real input, cursor changes or a running game. */
BOOL restore_video_peek_message(MSG *message);
void restore_video_translate_message(const MSG *message);
void restore_video_dispatch_message(const MSG *message);
void restore_video_show_cursor(BOOL show);
void restore_video_tick(void);
#endif
