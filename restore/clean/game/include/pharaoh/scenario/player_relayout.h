#ifndef PHARAOH_SCENARIO_PLAYER_RELAYOUT_H
#define PHARAOH_SCENARIO_PLAYER_RELAYOUT_H
#include "pharaoh/runtime/game_file_upgrade.h"
/* Native004D1AD0, full two-player transformation; current-sized storage. */
void pharaoh_game_file_relayout_players(PharaohUpgradePlayerRecord players[2]);
#endif
