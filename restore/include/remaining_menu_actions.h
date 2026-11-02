#ifndef RESTORE_REMAINING_MENU_ACTIONS_H
#define RESTORE_REMAINING_MENU_ACTIONS_H
#include <stdint.h>
#include "city_prepare.h"
#include "message_panel_input.h"
void FUN_00406090(void);
void FUN_0040a0e0(void);
void FUN_00478420(void);
void FUN_00486e70(void);
void FUN_004dea50(void);
void FUN_00488e90(int32_t,int32_t,int32_t,int32_t,char);
void FUN_004e2970(int);
void FUN_0040a160(void);
#ifdef REMAINING_MENU_TRACE
uint32_t remaining_menu_read(uint32_t,uint32_t);
void remaining_menu_write(uint32_t,uint32_t,uint32_t);
void remaining_menu_context(uint32_t,uintptr_t);
#define RA_U(a) remaining_menu_read((uint32_t)(a),4)
#define RA_B(a) remaining_menu_read((uint32_t)(a),1)
#define RA_W(a,n,v) remaining_menu_write((uint32_t)(a),(n),(uint32_t)(v))
#define RA_CONTEXT(a,c) remaining_menu_context((a),(uintptr_t)(c))
#else
#define RA_U(a) (*(volatile const uint32_t *)(uintptr_t)(uint32_t)(a))
#define RA_B(a) (*(volatile const uint8_t *)(uintptr_t)(uint32_t)(a))
#define RA_W(a,n,v) do{if((n)==1)*(volatile uint8_t *)(uintptr_t)(uint32_t)(a)=(uint8_t)(v);else *(volatile uint32_t *)(uintptr_t)(uint32_t)(a)=(uint32_t)(v);}while(0)
#define RA_CONTEXT(a,c) ((void)(c))
#endif
/* Complete 489030 facade. The supplied native ECX context is explicit, but
   all132 instructions of488E90 overwrite it before use: no receiver fields
   are read. Its real existing C definition has five cdecl stack arguments. */
static __inline void restore_remaining_refresh_489030(void *context)
{
    uint32_t height,width;
    RA_CONTEXT(0x489030u,context);
    height=RA_U(0xbca2d4)-1u;width=RA_U(0xbca2d0)-1u;
    FUN_00488e90(0,0,(int32_t)width,(int32_t)height,1);
}
/* 4E2970's complete native body reads only incoming stack argument1. Its two
   extra pushed arguments are unused; forward to the real one-argument C ABI. */
static __inline void restore_remaining_message_4e2970(int32_t message,uint32_t unused1,uint32_t unused2)
{
    (void)unused1;(void)unused2;FUN_004e2970(message);
}
int restore_remaining_menu_action(uint32_t token);
#endif
