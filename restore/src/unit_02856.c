#include "crt_exception_runtime.h"

/* Existing abort C retains the diagnostic, SIGABRT and quick-exit sequence. */
__declspec(noreturn) void FUN_005686f4(void);

__declspec(noreturn) void restore_crt_terminate(void)
{
 void (__cdecl *handler)(void);
 /* Native scope570638 has an inner execute-handler filter and an outer
    finally which aborts on normal completion and on structured unwind. */
 __try {
  handler=*(void (__cdecl * volatile *)(void))(uintptr_t)0x01266414u;
  if(handler!=0){
   __try { handler(); }
   __except(EXCEPTION_EXECUTE_HANDLER) { }
  }
 }
 __finally { FUN_005686f4(); }
}
