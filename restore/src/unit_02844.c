#include "crt_eh3_frames.h"

/* 562FEA uses EBP-68h..EBP-1. The dispatch-owned exception cell at EBP-14h
   is deliberately unwritten until the EH3 handler supplies it. */
typedef struct RestoreCrtEh3StartupFrame {
 uint32_t exception_code,command_line_tail,exit_code;
 STARTUPINFOA startup_info;
 uint32_t saved_sp,exception_pointers;
 RestoreCrtEh3Registration registration;
} RestoreCrtEh3StartupFrame;
typedef char RestoreCrtEh3StartupFrameLayout[(sizeof(RestoreCrtEh3StartupFrame)==104 &&
 offsetof(RestoreCrtEh3StartupFrame,startup_info)==12 &&
 offsetof(RestoreCrtEh3StartupFrame,saved_sp)==80 &&
 offsetof(RestoreCrtEh3StartupFrame,exception_pointers)==84 &&
 offsetof(RestoreCrtEh3StartupFrame,registration)==88)?1:-1];

#define CRT_VERSION_BUILD (*(volatile uint32_t *)0x0126643cu)
#define CRT_VERSION_PACKED (*(volatile uint32_t *)0x01266440u)
#define CRT_VERSION_MAJOR (*(volatile uint32_t *)0x01266444u)
#define CRT_VERSION_MINOR (*(volatile uint32_t *)0x01266448u)
#define CRT_COMMAND_LINE (*(LPSTR volatile *)0x01267aa4u)
#define CRT_ENVIRONMENT (*(LPSTR volatile *)0x01266400u)

/* Native 413ED0 is stdcall with RET16, but only [EBP+8] is read. Its
   existing C owner exposes that one consumed instance word as cdecl. */
uint32_t __cdecl FUN_00413ed0(uint32_t instance);
uint32_t FUN_005653eb(int32_t multithreaded);
__declspec(noreturn) void FUN_00563105(uint32_t error);
void FUN_00566a03(void);
LPSTR FUN_00567c9a(void);
void FUN_00567a4d(void);
void FUN_00567994(void);
void FUN_005646c0(void);
uint8_t *FUN_0056793c(void);
/* Both wrappers pass return_to_caller=0 to 56470F, ending in ExitProcess. */
__declspec(noreturn) void FUN_005646ed(uint32_t code);
__declspec(noreturn) void __cdecl FUN_005646fe(int32_t code);
LONG FUN_005677b8(uint32_t code,EXCEPTION_POINTERS *pointers);

int32_t restore_crt_startup_filter(void);
uint32_t restore_crt_startup_recover(void);
const RestoreCrtEh3Scope restore_crt_startup_scopes[1]={
 {-1,restore_crt_startup_filter,restore_crt_startup_recover}
};

static void prepare_eh3_startup(volatile RestoreCrtEh3StartupFrame *f,
 const volatile uint32_t *arguments,uint32_t saved_sp)
{
 (void)arguments;
 restore_crt_eh3_install(&f->registration,restore_crt_startup_scopes);
 f->saved_sp=saved_sp;
}
static __declspec(noreturn) void run_eh3_startup(volatile RestoreCrtEh3StartupFrame *f,
 const volatile uint32_t *arguments,uint32_t parent_frame,uint32_t recovered)
{
 uint32_t version,show_window;
 (void)arguments;(void)parent_frame;
 if(recovered)FUN_005646fe((int32_t)f->exception_code);
 #pragma warning(suppress:4996) /* Exact original GetVersion import. */
 version=GetVersion();
 CRT_VERSION_MINOR=(version>>8)&0xffu;
 CRT_VERSION_MAJOR=version&0xffu;
 CRT_VERSION_PACKED=(version&0xffu)*0x100u+((version>>8)&0xffu);
 CRT_VERSION_BUILD=version>>16;
 if(FUN_005653eb(0)==0)FUN_00563105(0x1cu);
 f->registration.state=0;
 FUN_00566a03();
 CRT_COMMAND_LINE=GetCommandLineA();
 CRT_ENVIRONMENT=FUN_00567c9a();
 FUN_00567a4d();
 FUN_00567994();
 FUN_005646c0();
 f->startup_info.dwFlags=0;
 GetStartupInfoA((LPSTARTUPINFOA)&f->startup_info);
 f->command_line_tail=(uint32_t)(uintptr_t)FUN_0056793c();
 if((f->startup_info.dwFlags&1u)==0)show_window=10;
 else show_window=(uint32_t)f->startup_info.wShowWindow;
 /* The parsed command line, previous-instance zero and show state were the
    other three native arguments. 413ED0 never consumes those stack words. */
 (void)show_window;
 f->exit_code=FUN_00413ed0((uint32_t)(uintptr_t)GetModuleHandleA(0));
 FUN_005646ed(f->exit_code);
}
RESTORE_EH3_PARENT(restore_crt_startup,void,(void),104,prepare_eh3_startup,run_eh3_startup)
RESTORE_EH3_RECOVERY(restore_crt_startup_recover,104,run_eh3_startup)

/* 5630C1: called with the actual parent's EBP by the EH3 invocation bridge.
   The full DWORD exception code is saved before the signal/unhandled filter. */
static int32_t run_startup_filter(uint32_t parent_frame)
{
 volatile RestoreCrtEh3StartupFrame *f=(volatile RestoreCrtEh3StartupFrame *)(uintptr_t)(parent_frame-104);
 EXCEPTION_POINTERS *pointers=(EXCEPTION_POINTERS *)(uintptr_t)f->exception_pointers;
 uint32_t code=pointers->ExceptionRecord->ExceptionCode;
 f->exception_code=code;
 return (int32_t)FUN_005677b8(code,pointers);
}
__declspec(naked) int32_t restore_crt_startup_filter(void)
{
 __asm push ebp
 __asm call run_startup_filter
 __asm add esp,4
 __asm ret
}
__declspec(naked) void FUN_00562fea(void)
{__asm jmp restore_crt_startup}
