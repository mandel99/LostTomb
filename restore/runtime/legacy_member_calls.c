#include "legacy_member_calls.h"
void restore_call_member0(void *object, uintptr_t target)
{
    /* MSVC C does not admit __thiscall pointer types. A fastcall with an
       unused EDX parameter has the same ECX/stack/cleanup contract. */
    typedef void (__fastcall *Member)(void *, void *);
    ((Member)target)(object, 0);
}
void restore_call_member1(void *object, uintptr_t target, uintptr_t value)
{
    typedef void (__fastcall *Member)(void *, void *, uintptr_t);
    ((Member)target)(object, 0, value);
}
