#include "pharaoh/ui/window_layout.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <share.h>

void pharaoh_ui_layout_read_list(const PharaohUiLayoutIo *io,
    PharaohUiLayoutFile *file, int32_t count, PharaohUiLayoutNode **head)
{
    if ((io->flags(io->user_data, file) & 0x20u) != 0) return;
    *head = NULL;
    while (count > 0) {
        PharaohUiLayoutNode *const node =
            io->allocate(io->user_data, sizeof(*node));
        (void)io->read_elements(io->user_data, &node->relative_x, 4, 1, file);
        (void)io->read_elements(io->user_data, &node->relative_y, 4, 1, file);
        for (int32_t field = 0; field < 4; ++field) {
            (void)io->read_elements(io->user_data,
                &node->values_08[field], 4, 1, file);
        }
        node->next = *head;
        if ((io->flags(io->user_data, file) & 0x30u) == 0) {
            *head = node;
        } else {
            io->release(io->user_data, node);
        }
        --count;
    }
}

void pharaoh_ui_layout_load(PharaohUiLayoutState *state,
    const PharaohUiLayoutIo *io)
{
    PharaohUiLayoutFile *const file =
        io->open(io->user_data, "dialog.loc", "rb");
    if (file == NULL) return;
    int32_t count;
    for (int32_t index = 0; index < PHARAOH_UI_LAYOUT_ENTRY_COUNT; ++index) {
        PharaohUiLayoutEntry *const entry = &state->entries[index];
        (void)io->read_elements(io->user_data, &count, 4, 1, file);
        pharaoh_ui_layout_read_list(io, file, count, &entry->lists[1]);
        if (entry->lists[1] != NULL) entry->buttons_initialized = 0;
        (void)io->read_elements(io->user_data, &count, 4, 1, file);
        pharaoh_ui_layout_read_list(io, file, count, &entry->lists[0]);
        (void)io->read_elements(io->user_data, &count, 4, 1, file);
        pharaoh_ui_layout_read_list(io, file, count, &entry->lists[2]);
        if ((io->flags(io->user_data, file) & 0x30u) != 0) {
            /* Native discards these heads without freeing prior linked nodes. */
            entry->lists[1] = NULL;
            entry->lists[0] = NULL;
            entry->lists[2] = NULL;
            entry->buttons_initialized = 1;
            break;
        }
    }
    io->close(io->user_data, file);
}

void pharaoh_ui_layout_initialize(const PharaohUiLayoutContext *context)
{
    PharaohUiLayoutState *const state = context->state;
    for (int32_t index = 0; index < PHARAOH_UI_LAYOUT_ENTRY_COUNT; ++index) {
        PharaohUiLayoutEntry *const entry = &state->entries[index];
        entry->lists[1] = NULL;
        entry->lists[2] = NULL;
        entry->lists[0] = NULL;
        entry->buttons_initialized = 1;
    }
    state->initialized = 1;
    pharaoh_ui_layout_init_column_widths(context->text, state->column_widths);
    pharaoh_ui_layout_init_label_width(context->text, context->label_buttons,
        &state->label_columns, &state->label_width);
    pharaoh_ui_layout_load(state, context->io);
}

static PharaohUiLayoutFile *stdio_open(void *data,
    const char *path, const char *mode)
{
    (void)data;
    /* Native 00561A0A forwards sharing flag 0x40 (_SH_DENYNO). */
    return (PharaohUiLayoutFile *)_fsopen(path, mode, _SH_DENYNO);
}

static uint32_t stdio_read(void *data, void *out, uint32_t size,
    uint32_t count, PharaohUiLayoutFile *file)
{
    (void)data;
    return (uint32_t)fread(out, size, count, (FILE *)file);
}

static uint32_t stdio_flags(void *data, const PharaohUiLayoutFile *file)
{
    uint32_t flags = 0;
    (void)data;
    if (feof((FILE *)file) != 0) flags |= 0x10u;
    if (ferror((FILE *)file) != 0) flags |= 0x20u;
    return flags;
}

static void stdio_close(void *data, PharaohUiLayoutFile *file)
{ (void)data; (void)fclose((FILE *)file); }
static void *stdio_allocate(void *data, uint32_t bytes)
{ (void)data; return malloc(bytes); }
static void stdio_release(void *data, void *allocation)
{ (void)data; free(allocation); }

void pharaoh_ui_layout_bind_stdio(PharaohUiLayoutIo *io)
{
    *io = (PharaohUiLayoutIo){NULL, stdio_open, stdio_read, stdio_flags,
        stdio_close, stdio_allocate, stdio_release};
}
