#ifndef RESTORE_GAME_LAUNCHER_H
#define RESTORE_GAME_LAUNCHER_H
/* Select real game assets and apply process-start color compatibility on
   desktops requiring it. Return1 when the caller should exit with *code;
   return0 only when this process is ready to enter the restored game. */
int restore_game_prepare_launch(int *code);
#endif
