#include "pharaoh/runtime/graphics_frame_restore.h"

static const char front_restore_error[] =
    "ERR:Can't restore front buffer.  ";
static const char back_restore_error[] =
    "ERR:Can't restore back buffer.  ";

int32_t pharaoh_graphics_frame_restore_surfaces(
    const PharaohGraphicsFrameRestoreContext *context)
{
    const PharaohGraphicsFrameRestoreOperations *const operations =
        context->operations;

    if (context->front_surface != NULL &&
        operations->restore_surface(operations->user_data,
            context->front_surface) == 0) {
        if (context->back_surface != NULL &&
            operations->restore_surface(operations->user_data,
                context->back_surface) == 0) {
            return 1;
        }
        pharaoh_status_log_format(back_restore_error, NULL, 0,
            context->status_state, context->status_operations);
        return 0;
    }

    pharaoh_status_log_format(front_restore_error, NULL, 0,
        context->status_state, context->status_operations);
    return 0;
}
