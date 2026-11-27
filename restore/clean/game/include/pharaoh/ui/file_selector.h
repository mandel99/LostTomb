#ifndef PHARAOH_UI_FILE_SELECTOR_H
#define PHARAOH_UI_FILE_SELECTOR_H

#include "pharaoh/runtime/file_list.h"

typedef struct PharaohFileSelectorState {
    PharaohFileList *files; /* regular list at6057C8 */
    int32_t *file_count; /* 6059F8 */
    uint32_t *first_visible,*selected_file; /* 6059F4/6059F0 */
    uint8_t *selection_active; /* D3B1B3 */
    uint32_t *slot_6059ec,*slot_e36efc;
    char *family_directory; /* actual5169C0 output EADEA4 */
    const char *player_name; /* E38E74 */
} PharaohFileSelectorState;

/* Complete004CDFF0. The native first argument is unused. Filename loading,
   first-dot removal, case-insensitive sorting and final stores use real C. */
void pharaoh_file_selector_initialize(PharaohFileSelectorState *,uint8_t maps);

#endif
