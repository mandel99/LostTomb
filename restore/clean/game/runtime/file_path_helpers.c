#include "pharaoh/runtime/file_path_helpers.h"

void pharaoh_file_path_capture_extension(const char *path,
                                        char extension[4])
{
    while (*path != '\0') {
        if (*path == '.') {
            extension[0] = path[1];
            extension[1] = path[2];
            extension[2] = path[3];
            extension[3] = '\0';
            return;
        }
        ++path;
    }
}

void pharaoh_ascii_uppercase_in_place(uint8_t *text)
{
    uint8_t character = *text;
    while (character != 0) {
        if (character > 0x60 && character < 0x7b) {
            character = (uint8_t)(character - 0x20);
        }
        *text = character;
        ++text;
        character = *text;
    }
}
