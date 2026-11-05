#ifndef RESTORE_CRT_RUNTIME_TAIL_H
#define RESTORE_CRT_RUNTIME_TAIL_H
#include <stdint.h>
#include <windows.h>
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error Native stack and live x87 entry bridges require MSVC i386.
#endif
uint32_t restore_crt_classify_character(uint32_t character,const volatile uint32_t *native_mask,
                                       uint16_t *output);
BOOL restore_crt_get_string_type(DWORD type,LPCSTR text,int32_t count,LPWORD output,
                                UINT *native_codepage_slot,LCID locale,int32_t strict);
void restore_crt_two_arg_error_live(uint32_t type,uint32_t name,uint32_t operation,
                                   uint64_t first,uint64_t second,uint16_t *saved_control);
uint32_t restore_crt_probe_stack(uint32_t caller_sp,uint32_t bytes);
void restore_crt_catch_cleanup(uint32_t parent_ebp,uint32_t registration_esi,
                               uint32_t exception_edi,uint32_t comparison_ebx);
uint32_t restore_crt_fatal_error(int32_t error);
void restore_crt_error_banner(void);
void restore_crt_report_error(uint32_t error,uint32_t *written_count);
int32_t restore_crt_message_box(const char *text,const char *caption,uint32_t flags);

uint32_t FUN_00565f66(uint32_t character,uint32_t mask);
BOOL FUN_00569335(DWORD,LPCSTR,int32_t,LPWORD,UINT,LCID,int32_t);
void FUN_00567500(void); /* Physical EAX/ECX/EDX/ST0 plus original spill slots. */
void FUN_00561f30(void); /* Physical EAX byte count; allocates caller stack. */
void FUN_00563dd6(void); /* Physical EBP/ESI/EDI/EBX cleanup context. */
uint32_t __cdecl FUN_005630e0(int32_t error);
void FUN_00567dcc(void);
void FUN_00567e05(uint32_t error);
int32_t FUN_00569c11(const char *text,const char *caption,uint32_t flags);
#endif
