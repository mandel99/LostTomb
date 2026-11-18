#include "pharaoh/runtime/music_manager.h"

void pharaoh_music_manager_set_enabled(PharaohMusicManager *manager,
                                       int32_t enabled)
{
    manager->enabled = enabled;
}
