#ifndef RESTORE_SPRITE_TAG_LIST_H
#define RESTORE_SPRITE_TAG_LIST_H
/* Opaque native two-pointer list and existing 16-byte tag node. These wrappers
 * expose the original private C algorithms without changing their callers. */
void restore_sprite_tag_append(void *list,void *node);
void restore_sprite_tag_prepend(void *list,void *node);
/* Canonical members take ECX=list and one stack DWORD=node; native RET4. */
void FUN_005404b0(void);
void FUN_00540540(void);
#endif
