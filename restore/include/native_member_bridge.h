#ifndef RESTORE_NATIVE_MEMBER_BRIDGE_H
#define RESTORE_NATIVE_MEMBER_BRIDGE_H
#include <stdint.h>
#include <stddef.h>
#if !defined(_MSC_VER)||!defined(_M_IX86)
#error The native member bridge requires the pinned MSVC i386 ABI.
#endif

/* Actual PUSHAD image after PUSHFD. saved_esp is the real ESP after PUSHFD;
   the modeled native entry ESP is saved_esp+4. No input is synthesized. */
struct RestoreNativeMemberFrame {
    uint32_t edi,esi,ebp,saved_esp,ebx,edx,ecx,eax,flags;
};
typedef char RestoreNativeMemberFrameLayout[(sizeof(struct RestoreNativeMemberFrame)==36&&
    offsetof(struct RestoreNativeMemberFrame,saved_esp)==12&&
    offsetof(struct RestoreNativeMemberFrame,eax)==28&&
    offsetof(struct RestoreNativeMemberFrame,flags)==32)?1:-1];
typedef void (*RestoreNativeRaw)(uint32_t target,uint32_t registers[8],uint32_t *flags);

/* The original argument/local stack remains at its real addresses, retaining
   aliases and parent EBP. C execution uses a probed control stack below the
   borrowed64KB native-frame workspace. The named verification interface needs
   the same separation and an out-of-workspace snapshot. Deeper frames and
   guard-fault/unwind context are not asserted by this bounded bridge contract. */
void restore_native_member_run(uint32_t target,struct RestoreNativeMemberFrame *frame,
                               uint32_t cleanup,RestoreNativeRaw body);
void FUN_00561f30(void);

#define RESTORE_NATIVE_MEMBER_ENTRY(canonical,named,target,cleanup,body) \
 void named(struct RestoreNativeMemberFrame *frame) \
 {restore_native_member_run(target,frame,cleanup,body);} \
 __declspec(naked) void canonical(void) { \
 __asm { pushfd } \
 __asm { pushad } \
 __asm { mov ebx,esp } \
 __asm { cld } \
 __asm { mov eax,10000h } \
 __asm { call FUN_00561f30 } \
 __asm { mov esi,ebx } \
 __asm { mov edi,esp } \
 __asm { mov ecx,9 } \
 __asm { rep movsd } \
 __asm { push esp } \
 __asm { call named } \
 __asm { add esp,4 } \
 __asm { popad } \
 __asm { popfd } \
 __asm { mov esp,dword ptr[esp-24] } \
 __asm { lea esp,[esp+4] } \
 __asm { ret cleanup } }
#endif
