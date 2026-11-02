#include "video_platform.h"
BOOL restore_video_peek_message(MSG *message) {return PeekMessageA(message,0,0,0,PM_REMOVE);}
void restore_video_translate_message(const MSG *message) {TranslateMessage(message);}
void restore_video_dispatch_message(const MSG *message) {DispatchMessageA(message);}
void restore_video_show_cursor(BOOL show) {ShowCursor(show);}
void restore_video_tick(void) {(void)GetTickCount();}
