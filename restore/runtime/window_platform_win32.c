#include "window_messages.h"
#include "campaign_unlock.h"
#include "campaign_profile.h"
#include "sound_streams.h"
#include "graphics_session.h"
#include "unit_button_actions.h"
#include "unit_keyboard_actions.h"
#include "remaining_selection_actions.h"
#include <stdio.h>
#include <stdlib.h>
#include "final_state_actions.h"
extern uint32_t FUN_004cffe0(void);
extern uint32_t FUN_00502780(void);
extern int FUN_00538cd0(uint32_t,uint32_t,uint32_t);
extern char *FUN_00516a20(void);

extern void FUN_004ce6e0(char *,int,int);
extern void FUN_004cc110(int,int);
extern void FUN_004cc160(int,int);
extern void FUN_00407c10(int,short);
extern void FUN_004c47a0(int,uint32_t,int);
extern void FUN_00416b10(int);
extern void FUN_00416ac0(int);
extern void FUN_004cfdc0(uint32_t);
extern void FUN_004cc250(uint32_t);
extern void FUN_00416ba0(uint32_t);
extern void FUN_004168e0(int);
extern void FUN_00416990(int);
extern void FUN_005391e0(void);
extern void FUN_004cffc0(void);
extern void FUN_004cff90(void);
extern void FUN_004cc330(void);
extern void FUN_004035f0(void);
extern void FUN_004cc320(void);
extern void FUN_004cc350(void);
extern void FUN_00539170(void);
extern void FUN_00409eb0(void);
extern void FUN_0040c760(void);
extern void FUN_0040c6b0(void);
extern void FUN_00402980(void);
extern void FUN_00516890(void);
extern void FUN_0041f460(void);
extern void FUN_004cc3e0(void);
extern void FUN_004cc3c0(void);
extern void FUN_004cc370(void);
extern void FUN_0040c630(void);
extern void FUN_004cc390(void);
extern void FUN_004cc360(void);
extern void FUN_00403770(void);
extern void FUN_00404350(void);
extern void FUN_004051a0(void);

extern void FUN_004035a0(void);
extern void FUN_00414bd0(void);
extern void FUN_004e2ab0(void);
extern void FUN_00402da0(void);
extern void FUN_00406720(void);
extern void FUN_00416a20(void);
extern void FUN_00406090(void);
/* Switch keys identify recovered entries. Only linked C symbols are called;
   there is no function-pointer cast of a legacy game address. */
static uintptr_t game_call(uint32_t entry,uintptr_t a,uintptr_t b,uintptr_t c)
{
#define VOID0(address,name) case address: name(); return 0
    switch(entry) {
    case 0x41d110: return FUN_0041d110((uint32_t)a,(uint32_t)b);
    case 0x4cffe0: return FUN_004cffe0();
    case 0x4d0220: return FUN_004d0220();
    case 0x502780: return FUN_00502780();
    case 0x538cd0: return FUN_00538cd0((uint32_t)a,(uint32_t)b,(uint32_t)c);
    case 0x516a20: return (uintptr_t)FUN_00516a20();
    case 0x41b8a0: return FUN_0041b8a0(RESTORE_CAMPAIGN_PROFILE,(const char *)a);
    case 0x55f561: restore_sound_close_stream((void *)0x00e92bf8u,(uint32_t)a); return 0;
    case 0x4d01e0: FUN_004d01e0((uint32_t)a); return 0;
    case 0x4d0280: FUN_004d0280((int)a); return 0;
    case 0x4ce6e0: FUN_004ce6e0((char *)a,(int)b,(int)c); return 0;
    case 0x4cc110: FUN_004cc110((int)a,(int)b); return 0;
    case 0x4cc160: FUN_004cc160((int)a,(int)b); return 0;
    case 0x407c10: FUN_00407c10((int)a,(short)b); return 0;
    case 0x4c47a0: FUN_004c47a0((int)a,(uint32_t)b,(int)c); return 0;
    case 0x416b10: FUN_00416b10((int)a); return 0;
    case 0x416ac0: FUN_00416ac0((int)a); return 0;
    case 0x4cfdc0: FUN_004cfdc0((uint32_t)a); return 0;
    case 0x4cc250: FUN_004cc250((uint32_t)a); return 0;
    case 0x416ba0: FUN_00416ba0((uint32_t)a); return 0;
    case 0x4168e0: FUN_004168e0((int)a); return 0;
    case 0x416990: FUN_00416990((int)a); return 0;
    VOID0(0x5391e0,FUN_005391e0);
    VOID0(0x4cffc0,FUN_004cffc0);
    VOID0(0x4cff90,FUN_004cff90);
    VOID0(0x4cc330,FUN_004cc330);
    VOID0(0x4035f0,FUN_004035f0);
    VOID0(0x4cc320,FUN_004cc320);
    VOID0(0x4cc350,FUN_004cc350);
    VOID0(0x539170,FUN_00539170);
    VOID0(0x409eb0,FUN_00409eb0);
    VOID0(0x40c760,FUN_0040c760);
    VOID0(0x40c6b0,FUN_0040c6b0);
    VOID0(0x402980,FUN_00402980);
    VOID0(0x516890,FUN_00516890);
    VOID0(0x41f460,FUN_0041f460);
    VOID0(0x4cc3e0,FUN_004cc3e0);
    VOID0(0x4cc3c0,FUN_004cc3c0);
    VOID0(0x4cc370,FUN_004cc370);
    VOID0(0x40c630,FUN_0040c630);
    VOID0(0x4cc390,FUN_004cc390);
    VOID0(0x4cc360,FUN_004cc360);
    VOID0(0x403770,FUN_00403770);
    VOID0(0x404350,FUN_00404350);
    VOID0(0x4051a0,FUN_004051a0);
    case 0x41bae0: restore_campaign_unlock_all(RESTORE_CAMPAIGN_INDEX); return 0;
    VOID0(0x4035a0,FUN_004035a0);
    VOID0(0x414bd0,FUN_00414bd0);
    VOID0(0x4e2ab0,FUN_004e2ab0);
    VOID0(0x402da0,FUN_00402da0);
    VOID0(0x406fb0,restore_unit_button_00406fb0);
    VOID0(0x407110,restore_unit_button_00407110);
    VOID0(0x406b40,restore_unit_button_00406b40);
    VOID0(0x406e80,restore_unit_button_00406e80);
    VOID0(0x406c90,restore_unit_keyboard_00406c90);
    VOID0(0x406de0,restore_unit_keyboard_00406de0);
    VOID0(0x406720,FUN_00406720);
    VOID0(0x547a60,restore_remaining_selection_00547a60);
    VOID0(0x416a20,FUN_00416a20);
    VOID0(0x406090,FUN_00406090);
    case 0x560330:
        /* Optional legacy diagnostic dispatcher uses an EAX selector and
           dynamically installed callbacks. Its ABI is still unresolved. */
        fputs("Optional window trace dispatcher 0x00560330 is not reconstructed.\n",stderr);
        abort();
    default: fprintf(stderr,"Unknown recovered window action 0x%08x.\n",entry); abort();
    }
#undef VOID0
}
/* Win32 import thunks are callable here; this table never compares their identity. */
#pragma warning(push)
#pragma warning(disable:4232)
static const RestoreWindowApi api={
    DefWindowProcA,IsIconic,GetForegroundWindow,SetActiveWindow,SetForegroundWindow,ShowWindow,
    GetSystemMetrics,GetClientRect,ClientToScreen,GetWindowLongA,GetMenu,AdjustWindowRectEx,SetWindowPos,
    BeginPaint,EndPaint,PostQuitMessage,PostMessageA,GetMessageTime,GetMessagePos,game_call,InvalidateRect
};
#pragma warning(pop)
const RestoreWindowApi *restore_window_platform(void)
{
    return &api;
}
