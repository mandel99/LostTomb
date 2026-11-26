#ifndef PHARAOH_UI_WINDOW_LAYOUT_H
#define PHARAOH_UI_WINDOW_LAYOUT_H

#include "pharaoh/ui/window_layout_key.h"

enum { PHARAOH_UI_LAYOUT_ENTRY_COUNT = 120 };
typedef struct PharaohUiLayoutFile PharaohUiLayoutFile;

/* Native FILE/allocator boundaries. read_elements preserves unread bytes,
   and flags exposes native EOF=0x10/error=0x20. Providers must be real IO. */
typedef struct PharaohUiLayoutIo {
    void *user_data;
    PharaohUiLayoutFile *(*open)(void *, const char *, const char *);
    uint32_t (*read_elements)(void *, void *, uint32_t, uint32_t,
        PharaohUiLayoutFile *);
    uint32_t (*flags)(void *, const PharaohUiLayoutFile *);
    void (*close)(void *, PharaohUiLayoutFile *);
    void *(*allocate)(void *, uint32_t);
    void (*release)(void *, void *);
} PharaohUiLayoutIo;

typedef struct PharaohUiLayoutState {
    PharaohUiLayoutEntry entries[PHARAOH_UI_LAYOUT_ENTRY_COUNT];
    int32_t initialized;
    int32_t column_widths[6];
    int32_t label_columns, label_width;
} PharaohUiLayoutState;

typedef struct PharaohUiLayoutContext {
    PharaohUiLayoutState *state;
    const PharaohUiLayoutTextContext *text;
    PharaohGenericButton *label_buttons; /* 30 logical records */
    const PharaohUiLayoutIo *io;
} PharaohUiLayoutContext;

/* Complete native algorithms 0x004D6900, 0x004D69B0 and 0x004D5D90.
   Native-valid streams initialize each count and allocations must succeed.
   Initial truncated count bytes have native stack-dependent behavior and
   are not promoted to invented API inputs. */
void pharaoh_ui_layout_read_list(const PharaohUiLayoutIo *,
    PharaohUiLayoutFile *, int32_t count, PharaohUiLayoutNode **head);
void pharaoh_ui_layout_load(PharaohUiLayoutState *, const PharaohUiLayoutIo *);
void pharaoh_ui_layout_initialize(const PharaohUiLayoutContext *);

/* Concrete host provider; this does not relabel the original CRT entries. */
void pharaoh_ui_layout_bind_stdio(PharaohUiLayoutIo *);

#endif
