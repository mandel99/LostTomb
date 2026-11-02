#ifndef RESTORE_EDITOR_PERMISSIONS_ACTIONS_H
#define RESTORE_EDITOR_PERMISSIONS_ACTIONS_H
#include <stdint.h>
void restore_editor_permissions_00408610(void);
void restore_editor_permissions_00408840(void);
void restore_editor_permissions_00408920(void);
void restore_editor_permissions_00408ce0(void);
void restore_editor_permissions_00409150(void);
void restore_editor_permissions_004091b0(void);
void restore_editor_permissions_00408820(void);
void restore_editor_permissions_00408830(void);
/* Native one-DWORD/RET0 helper consumes only the argument's low BYTE. */
void restore_editor_permission_update_00408710(uint32_t secondary);
/* Native three-DWORD/RET0 setter, BYTE planes and state. */
void restore_editor_permission_set_00444a40(uint32_t record,uint32_t kind,int32_t state);
int restore_editor_permissions_action(uint32_t token);
int restore_editor_permissions_completion_action(uint32_t token);
#endif
