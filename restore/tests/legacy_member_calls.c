#include "legacy_member_calls.h"
#include <stdio.h>
static void *seen_object;
static uintptr_t seen_value;
static unsigned int calls;
static void __fastcall member0(void *object, void *unused_edx)
{
    (void)unused_edx;
    seen_object = object;
    ++calls;
}
static void __fastcall member1(void *object, void *unused_edx, uintptr_t value)
{
    (void)unused_edx;
    seen_object = object;
    seen_value = value;
    ++calls;
}
int main(void)
{
    unsigned int i;
    int object;
    /* Repetition catches an incorrect callee/caller stack-cleanup contract. */
    for (i = 0; i < 10000; ++i) {
        restore_call_member0(&object, (uintptr_t)member0);
        if (seen_object != &object) return 1;
        restore_call_member1(&object, (uintptr_t)member1, i ^ 0x89abcdefu);
        if (seen_object != &object || seen_value != (i ^ 0x89abcdefu)) return 2;
    }
    if (calls != 20000) return 3;
    puts("Member dispatch: ECX, stack arguments and cleanup verified.");
    return 0;
}
