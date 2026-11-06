#ifndef RESTORE_CRT_EH3_HANDLER_H
#define RESTORE_CRT_EH3_HANDLER_H

#include "crt_local_unwind.h"

/* The callbacks have no stack arguments. Their compiler funclet environment
   is the physical EBP supplied by the invocation bridge, not a C argument. */
typedef int32_t (__cdecl *RestoreCrtEh3Filter)(void);
typedef uint32_t (__cdecl *RestoreCrtEh3Handler)(void);

typedef struct RestoreCrtEh3Scope {
 int32_t enclosing;
 RestoreCrtEh3Filter filter;
 RestoreCrtEh3Handler handler;
} RestoreCrtEh3Scope;

typedef struct RestoreCrtEh3Registration {
 uint32_t previous;
 uint32_t handler;
 const RestoreCrtEh3Scope *scopes;
 int32_t state;
} RestoreCrtEh3Registration;

typedef char RestoreCrtEh3ScopeLayout[(sizeof(RestoreCrtEh3Scope)==12 &&
 offsetof(RestoreCrtEh3Scope,filter)==4 &&
 offsetof(RestoreCrtEh3Scope,handler)==8)?1:-1];
typedef char RestoreCrtEh3RegistrationLayout[(sizeof(RestoreCrtEh3Registration)==16 &&
 offsetof(RestoreCrtEh3Registration,handler)==4 &&
 offsetof(RestoreCrtEh3Registration,scopes)==8 &&
 offsetof(RestoreCrtEh3Registration,state)==12)?1:-1];

/* Native cdecl EXCEPTION_ROUTINE: zero continues execution, one searches the
   next registration. The dispatcher argument is never read by this body. */
uint32_t __cdecl restore_crt_eh3_handler(EXCEPTION_RECORD *exception,
 RestoreCrtEh3Registration *registration,void *context,void *dispatcher);
uint32_t __cdecl FUN_00561a90(EXCEPTION_RECORD *exception,
 RestoreCrtEh3Registration *registration,void *context,void *dispatcher);

#endif
