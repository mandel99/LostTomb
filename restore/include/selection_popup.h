#ifndef RESTORE_SELECTION_POPUP_H
#define RESTORE_SELECTION_POPUP_H
#include "menu_input.h"
#include <stdint.h>
#define SP_B(a) (*(uint8_t *)(uintptr_t)(a))
#define SP_W(a) (*(uint16_t *)(uintptr_t)(a))
#define SP_U(a) (*(uint32_t *)(uintptr_t)(a))
#define SP_I(a) (*(int32_t *)(uintptr_t)(a))
void FUN_00403c00(uint32_t x,uint32_t y,int32_t count,uint32_t group,
    uint32_t first,uint32_t byte_destination,uint32_t word_destination,
    uint32_t dword_destination,uint32_t post_action,uint32_t storage,
    uint32_t preserve_mapping,uint32_t completion,uint32_t alternate_mode,
    int32_t rows,uint32_t disable_edges);
void FUN_00403950(void);
void FUN_00409540(void);
void FUN_004092b0(void);
void FUN_00409300(void);
void FUN_00530d70(int x,int y,int rows);
int FUN_004d6740(int x,int y,int buttons,uint32_t count,int hit);
void restore_selection_popup_input(void);
/* Identity tokens are data; this dispatcher invokes only owned C code. */
int restore_selection_popup_action(uint32_t token);
#endif
