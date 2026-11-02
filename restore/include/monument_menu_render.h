#ifndef RESTORE_MONUMENT_MENU_RENDER_H
#define RESTORE_MONUMENT_MENU_RENDER_H
#include <stdint.h>
typedef struct RestoreMonumentMenuOption {uint32_t words[4];} RestoreMonumentMenuOption;
typedef char restore_monument_menu_option_size[sizeof(RestoreMonumentMenuOption)==16?1:-1];
/* Native cdecl: phase DWORD plus a sixteen-byte value, RET0, AL result. */
uint8_t restore_monument_option_allowed_0040cb40(uint32_t phase,RestoreMonumentMenuOption option);
void FUN_005374a0(void);
#endif
