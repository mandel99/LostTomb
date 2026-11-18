#ifndef PHARAOH_RUNTIME_FILE_PATH_STATE_H
#define PHARAOH_RUNTIME_FILE_PATH_STATE_H

typedef struct PharaohFilePathState {
    char extension[4];
    char media_fallback_directory[40];
} PharaohFilePathState;

#endif
