#include "crt_runtime_tail.h"
#include "crt_asin_math.h"
#include "crt_exception_dispatch.h"
#include "crt_local_unwind.h"
#include <malloc.h>
#include <stddef.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

/* Reuse the existing complete byte-string/zero-fill C algorithms. These
   pointer parameters carry bytes; DAT pointer units do not scale offsets. */
extern uint32_t *FUN_00566750(void *destination,const void *source);
extern uint32_t *FUN_00566760(void *destination,const void *source);
extern char *FUN_00562240(char *destination,const char *source,size_t count);
extern size_t FUN_00563380(const char *text);
extern void *FUN_00563240(void *destination,int32_t value,size_t count);
struct RestoreCrtErrorFrame {char program[260];char message[160];};
typedef char RestoreCrtErrorFrameLayout[(sizeof(struct RestoreCrtErrorFrame)==420&&
    offsetof(struct RestoreCrtErrorFrame,message)==260)?1:-1];

uint32_t restore_crt_classify_character(uint32_t character,const volatile uint32_t *native_mask,
                                       uint16_t *output)
{
    uint32_t table=U(0x604c58u),count;uint8_t bytes[4];
    if(character+1u<=0x100u)return W(table+character*2u)&*native_mask;
    if(B(table+((character>>8)&0xffu)*2u+1u)&0x80u){
        bytes[0]=(uint8_t)(character>>8);bytes[1]=(uint8_t)character;
        bytes[2]=0;count=2;
    }else{
        bytes[0]=(uint8_t)character;bytes[1]=0;count=1;
    }
    /* The remaining bytes are owned unspecified padding, as native PUSH ECX.
       Only the actual count and significant bytes reach the NLS operation. */
    if(!FUN_00569335(1,(LPCSTR)bytes,(int32_t)count,output,0,0,1))return 0;
    /* The native output begins at argument1's upper WORD. Preserve a Win32
       two-WORD write overlapping argument2, then reload that ACTUAL mask slot. */
    return (uint32_t)*output&*native_mask;
}

BOOL restore_crt_get_string_type(DWORD type,LPCSTR text,int32_t count,LPWORD output,
                                UINT *native_codepage_slot,LCID locale,int32_t strict)
{
    uint32_t backend=U(0x12666dcu);
    if(backend==0){
        WORD probe[2];
        if(GetStringTypeW(1,(LPCWSTR)(uintptr_t)0x570b98u,1,probe))backend=1;
        else if(GetStringTypeA(0,1,(LPCSTR)(uintptr_t)0x570b94u,1,probe))backend=2;
        else return 0;
        U(0x12666dcu)=backend;
    }
    if(backend==2){
        if(locale==0)locale=U(0x126648cu);
        return GetStringTypeA(locale,type,text,count,output);
    }
    if(backend==1){
        int32_t required,converted;WCHAR *wide;uint32_t bytes,aligned;
        UINT codepage=*native_codepage_slot;
        if(codepage==0){codepage=U(0x126649cu);*native_codepage_slot=codepage;}
        required=MultiByteToWideChar(codepage,strict!=0?9u:1u,text,count,0,0);
        if(required==0)return 0;
        bytes=(uint32_t)required*2u;aligned=(bytes+3u)&0xfffffffcu;
        /* Native56942E filter always executes its handler. Only the allocation
           and zero-fill belong to this SEH scope; subsequent Win32 calls do
           not. Native recovery neither resets the stack guard nor retries. */
        __try {
            wide=(WCHAR *)_alloca(aligned);
            (void)FUN_00563240(wide,0,bytes);
        }__except(EXCEPTION_EXECUTE_HANDLER){wide=0;}
        if(wide==0)return 0;
        converted=MultiByteToWideChar(codepage,1,text,count,wide,required);
        if(converted==0)return 0;
        return GetStringTypeW(type,wide,converted,output);
    }
    return 0;
}

void restore_crt_two_arg_error_live(uint32_t type,uint32_t name,uint32_t operation,
                                   uint64_t first,uint64_t second,uint16_t *saved_control)
{
    struct restore_crt_math_exception record;uint64_t value;
    record.type=(int32_t)type;record.name=name;record.arg1=first;record.arg2=second;
    __asm { fstp qword ptr value }
    record.result=value;
    restore_crt_math_dispatch(operation,&record,saved_control);
    value=record.result;
    __asm { fld qword ptr value }
    if(*saved_control!=0x27fu){
        uint16_t control=*saved_control;
        __asm { fldcw control }
    }
}

uint32_t restore_crt_probe_stack(uint32_t caller_sp,uint32_t bytes)
{
    uint32_t location=caller_sp;
    while(bytes>=0x1000u){
        location-=0x1000u;bytes-=0x1000u;
        (void)U(location);
    }
    location-=bytes;
    (void)U(location);
    return location;
}

void restore_crt_catch_cleanup(uint32_t parent_ebp,uint32_t registration_esi,
                               uint32_t exception_edi,uint32_t comparison_ebx)
{
    U(registration_esi-4u)=U(parent_ebp-0x28u);
    U(0x1266418u)=U(parent_ebp-0x1cu);
    U(0x126641cu)=U(parent_ebp-0x20u);
    if(U(exception_edi)==0xe06d7363u&&U(exception_edi+0x10u)==3u&&
       U(exception_edi+0x14u)==0x19930520u&&
       U(parent_ebp-0x24u)==comparison_ebx&&U(parent_ebp-0x2cu)!=comparison_ebx){
        uint32_t abnormal=(uint32_t)restore_crt_abnormal_termination();
        FUN_0056400a((int32_t)exception_edi,abnormal);
    }
}

uint32_t restore_crt_fatal_error(int32_t error)
{
    typedef uint32_t (__cdecl *ExitCallback)(int32_t);
    uint32_t written=(uint32_t)error;
    if(U(0x1266408u)==1u)restore_crt_error_banner();
    restore_crt_report_error((uint32_t)error,&written);
    /* Native563102..563104 cleans both pushed arguments and RETs when a
       custom exit callback returns. Its ACTUAL full EAX reaches purecall and
       virtual consumers; no zero/void return substitution is valid here. */
    return ((ExitCallback)(uintptr_t)U(0x604968u))(0xff);
}
void restore_crt_error_banner(void)
{
    typedef void (__cdecl *BannerCallback)(void);
    uint32_t mode=U(0x1266408u),callback,written;
    if(mode==1u||(mode==0&&U(0x60496cu)==1u)){
        written=0xfcu;
        restore_crt_report_error(0xfcu,&written);
        callback=U(0x12666a0u);
        if(callback)((BannerCallback)(uintptr_t)callback)();
        written=0xffu;
        restore_crt_report_error(0xffu,&written);
    }
}

void restore_crt_report_error(uint32_t error,uint32_t *written_count)
{
    uint32_t index=0,mode,message;
    while(index<18u){if(U(0x605158u+index*8u)==error)break;++index;}
    /* Preserve the native final comparison at index18. In the pinned image
       it accepts error20 despite being beyond the nominal table-loop end. */
    if(U(0x605158u+index*8u)!=error)return;
    message=U(0x60515cu+index*8u);mode=U(0x1266408u);
    if(mode==1u||(mode==0&&U(0x60496cu)==1u)){
        DWORD length=(DWORD)FUN_00563380((const char *)(uintptr_t)message);
        HANDLE error_handle=GetStdHandle((DWORD)-12);
        (void)WriteFile(error_handle,(LPCVOID)(uintptr_t)message,length,(LPDWORD)written_count,0);
    }else if(error!=0xfcu){
        struct RestoreCrtErrorFrame frame;
        char *display=frame.program;uint32_t length;
        if(GetModuleFileNameA(0,frame.program,260)==0)
            (void)FUN_00566750(frame.program,(const void *)(uintptr_t)0x570ac8u);
        length=(uint32_t)FUN_00563380(frame.program);
        if(length+1u>60u){
            length=(uint32_t)FUN_00563380(frame.program);
            display=(char *)((uintptr_t)frame.program+length-59u);
            (void)FUN_00562240(display,(const char *)(uintptr_t)0x570ac4u,3);
        }
        (void)FUN_00566750(frame.message,(const void *)(uintptr_t)0x570aa8u);
        (void)FUN_00566760(frame.message,display);
        (void)FUN_00566760(frame.message,(const void *)(uintptr_t)0x570aa4u);
        (void)FUN_00566760(frame.message,(const void *)(uintptr_t)message);
        (void)restore_crt_message_box(frame.message,(const char *)(uintptr_t)0x570a7cu,0x12010u);
    }
}

int32_t restore_crt_message_box(const char *text,const char *caption,uint32_t flags)
{
    typedef int (WINAPI *MessageBoxEntry)(HWND,LPCSTR,LPCSTR,UINT);
    typedef HWND (WINAPI *ActiveWindowEntry)(void);
    typedef HWND (WINAPI *PopupEntry)(HWND);
    HWND parent=0;uint32_t active,popup;
    if(U(0x12666e8u)==0){
        HMODULE user32=LoadLibraryA((LPCSTR)(uintptr_t)0x570bf8u);
        if(user32==0)return 0;
        U(0x12666e8u)=(uint32_t)(uintptr_t)GetProcAddress(user32,(LPCSTR)(uintptr_t)0x570becu);
        if(U(0x12666e8u)==0)return 0;
        U(0x12666ecu)=(uint32_t)(uintptr_t)GetProcAddress(user32,(LPCSTR)(uintptr_t)0x570bdcu);
        U(0x12666f0u)=(uint32_t)(uintptr_t)GetProcAddress(user32,(LPCSTR)(uintptr_t)0x570bc8u);
    }
    active=U(0x12666ecu);
    if(active){
        parent=((ActiveWindowEntry)(uintptr_t)active)();
        if(parent){
            popup=U(0x12666f0u);
            if(popup)parent=((PopupEntry)(uintptr_t)popup)(parent);
        }
    }
    return ((MessageBoxEntry)(uintptr_t)U(0x12666e8u))(parent,text,caption,flags);
}
