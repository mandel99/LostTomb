#ifndef PHARAOH_UI_MESSAGE_DIALOG_LIVE_H
#define PHARAOH_UI_MESSAGE_DIALOG_LIVE_H
#include "pharaoh/runtime/smacker_window_video_start.h"

typedef struct PharaohMessageDialogLiveCloseContext {
    int32_t *show_video; /*D3B180*/
    const uint8_t *previous_window; /*D3B17C*/
    uint8_t *current_window; /*E38DDA*/
    int32_t *message_category; /*D3A6E8*/
    uint8_t *redraw; /*E38DCA*/
    PharaohSmackerWindowVideoState *video;
    const PharaohSmackerWindowVideoStopOperations *video_stop;
} PharaohMessageDialogLiveCloseContext;

/* Complete004C7F30. Borrow the actual dialog/window fields and active video
   state/operations. Reload previous_window after the actual00539170 C child. */
void pharaoh_message_dialog_close_live(
    const PharaohMessageDialogLiveCloseContext *);
#endif
