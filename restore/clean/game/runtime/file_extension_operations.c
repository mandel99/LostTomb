#include "pharaoh/runtime/file_extension_operations.h"

void pharaoh_file_change_extension(PharaohFilePathState *state,
                                   char *path,
                                   const char extension[3])
{
    state->extension[0] = extension[0];
    state->extension[1] = extension[1];
    state->extension[2] = extension[2];
    pharaoh_file_replace_extension(state, path);
}

void pharaoh_file_replace_extension(PharaohFilePathState *state,
                                    char *path)
{
    while (*path != '\0') {
        if (*path == '.') {
            path[1] = state->extension[0];
            path[2] = state->extension[1];
            path[3] = state->extension[2];
            path[4] = '\0';
            return;
        }
        ++path;
    }
}

void pharaoh_file_remove_extension(char *path)
{
    while (*path != '\0') {
        if (*path == '.') {
            *path = '\0';
            return;
        }
        ++path;
    }
}

void pharaoh_file_append_extension(PharaohFilePathState *state,
                                   char *path)
{
    while (*path != '\0') {
        ++path;
    }
    path[0] = '.';
    path[1] = state->extension[0];
    path[2] = state->extension[1];
    path[3] = state->extension[2];
    path[4] = '\0';
}
