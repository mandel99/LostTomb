#ifndef RESTORE_CRT_EXCEPTION_RUNTIME_H
#define RESTORE_CRT_EXCEPTION_RUNTIME_H
#include <stdint.h>
#include <windows.h>
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error Native exception funclets require MSVC i386.
#endif
uint32_t restore_crt_notify_jump(uint32_t destination,uint32_t code,uint32_t frame);
uint32_t restore_crt_invoke_frame(uint32_t handler,uint32_t frame,uint32_t code);
__declspec(noreturn) void restore_crt_terminate(void);
int32_t restore_crt_unhandled_filter(EXCEPTION_POINTERS *exception);
#endif
