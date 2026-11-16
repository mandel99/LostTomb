#include "pharaoh/runtime/text_loading.h"

#include <windows.h>

void pharaoh_text_select(PharaohTextSelection *selection, int32_t group,
                         int32_t entry)
{
    const PharaohTextGroupIndex *group_index =
        &selection->catalog->primary_index[group];
    const uint8_t *text = (const uint8_t *)selection->catalog->primary_text +
        group_index->text_offset;
    selection->current = (const char *)text;

    while (entry > 0) {
        if (text[0] == 0 && (text[-1] >= 32 || text[-1] == 0)) {
            --entry;
        }
        ++text;
        selection->current = (const char *)text;
    }

    while (*text < 32) {
        ++text;
        selection->current = (const char *)text;
    }
}

int32_t pharaoh_file_load_retry_prompt(PharaohTextSelection *selection)
{
    const char *previous_text = selection->current;
    pharaoh_text_select(selection, PHARAOH_TEXT_ERROR_GROUP,
        PHARAOH_TEXT_ERROR_CAPTION_ENTRY);
    const char *caption = selection->current;
    pharaoh_text_select(selection, PHARAOH_TEXT_ERROR_GROUP,
        PHARAOH_TEXT_ERROR_BODY_ENTRY);
    const char *body = selection->current;
    selection->current = previous_text;

    return MessageBoxA(NULL, body, caption,
        PHARAOH_TEXT_ERROR_MESSAGE_BOX_FLAGS);
}
