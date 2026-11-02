#include "restore_types.h"
#include <stdio.h>
#ifndef MODAL_KEYBOARD_CONTEXT_SOURCE
#error MODAL_KEYBOARD_CONTEXT_SOURCE must name the unchanged root unit00274 source.
#endif
SHORT WINAPI restore_modal_async_key(int key);
/* Rebind only the OS boundary, after the Windows declarations are included. */
#define GetAsyncKeyState restore_modal_async_key
#pragma warning(push)
/* Frozen unit274 has a pointer/integer assignment in its unexecuted Ctrl+F
   branch. Only the unchanged Escape path is composed by this adapter. */
#pragma warning(disable:4047)
#include MODAL_KEYBOARD_CONTEXT_SOURCE
#pragma warning(pop)
#undef GetAsyncKeyState
