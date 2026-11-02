#ifndef RESTORE_MISSION_SCHEMA_H
#define RESTORE_MISSION_SCHEMA_H
void FUN_004d1340(void);
void FUN_004d1350(void);
void FUN_004d4a50(void);
void FUN_004d4a60(void);
/* Compatibility with early C-only saves that omitted all late descriptors.
   These recovery helpers have no original executable counterpart. */
void restore_mission_schema_recover_loaded(void);
void restore_mission_schema_recover_terrain(void);
#endif
