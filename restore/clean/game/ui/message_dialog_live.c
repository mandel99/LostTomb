#include "pharaoh/ui/message_dialog_live.h"

void pharaoh_message_dialog_close_live(
    const PharaohMessageDialogLiveCloseContext *c)
{
    if(*c->show_video!=0)
        pharaoh_smacker_window_video_stop(c->video,c->video_stop);
    const uint8_t previous=*c->previous_window;
    *c->show_video=0;
    *c->current_window=previous;
    *c->message_category=0;
    *c->redraw=1;
}
