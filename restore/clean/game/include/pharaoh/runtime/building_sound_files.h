#ifndef PHARAOH_RUNTIME_BUILDING_SOUND_FILES_H
#define PHARAOH_RUNTIME_BUILDING_SOUND_FILES_H

#include "pharaoh/runtime/sound_file_id_load.h"

enum { PHARAOH_BUILDING_SOUND_BANK_COUNT=237 };
typedef struct PharaohBuildingSoundBank {
    uint32_t effects[3],roam,effect_count;
} PharaohBuildingSoundBank;

typedef struct PharaohBuildingSoundFileContext {
    PharaohBuildingSoundBank *banks; /* Actual F54670,20-byte rows. */
    const PharaohSoundFileIdLoadContext *loader;
} PharaohBuildingSoundFileContext;

/* Whole Native539CF0/539D60. Returns are discarded by the observed loader
 * chain. The real539C60 provider owns file IDs and exact filename fallback. */
void pharaoh_building_sound_bind_file(const PharaohBuildingSoundFileContext *,
    int32_t bank,const char *filename,int32_t mode);
void pharaoh_building_sound_files_initialize(const PharaohBuildingSoundFileContext *);

#endif
