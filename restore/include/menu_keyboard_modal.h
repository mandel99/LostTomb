#ifndef RESTORE_MENU_KEYBOARD_MODAL_H
#define RESTORE_MENU_KEYBOARD_MODAL_H
#include "menu_input.h"
#include <stdint.h>
#define MM_B(a) (*(uint8_t *)(uintptr_t)(a))
#define MM_U(a) (*(uint32_t *)(uintptr_t)(a))
#define MM_I(a) (*(int32_t *)(uintptr_t)(a))
typedef struct RestoreModalEventApi {
    BOOL (WINAPI *peek)(LPMSG,HWND,UINT,UINT,UINT);
    BOOL (WINAPI *get)(LPMSG,HWND,UINT,UINT);
    BOOL (WINAPI *translate)(const MSG *);
    LRESULT (WINAPI *dispatch)(const MSG *);
    BOOL (WINAPI *wait)(void);
} RestoreModalEventApi;
const RestoreModalEventApi *restore_menu_modal_events(void);
void FUN_00539170(void);
void FUN_00539220(uint32_t ignored);
int FUN_00538940(uint32_t title,uint32_t body,uint32_t flags);
int FUN_00538c80(uint32_t title,uint32_t body,uint32_t flags);
int FUN_00538cd0(uint32_t title,uint32_t body,uint32_t flags);
void FUN_00538920(void);
void restore_menu_modal_input(void);
int restore_menu_modal_action(uint32_t token);
#endif
