#ifndef RESTORE_STARTUP_GEOMETRY_H
#define RESTORE_STARTUP_GEOMETRY_H
#include <stdint.h>
typedef struct RestoreGeometryRecord {uint32_t value[13];} RestoreGeometryRecord;
typedef char restore_geometry_size[(sizeof(RestoreGeometryRecord)==0x34)?1:-1];
void FUN_004e5190(RestoreGeometryRecord *record);
void FUN_004e52b0(RestoreGeometryRecord *records,int count);
int *FUN_004ded50(int *output,int index,int rotation,int orientation);
void FUN_00541d50(void);
void FUN_004dee10(void);
void FUN_004013f0(void *self);
void FUN_0040d7e0(void *self);
void FUN_00416f60(void *self);
void FUN_004e6590(void *self);
void FUN_005456a0(void *self);
void FUN_00558b40(void *self);
void FUN_00555de0(void *self);
void FUN_0054f3d0(void *self);
#endif
