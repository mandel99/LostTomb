#ifndef RESTORE_TERRAIN_DIAMOND_H
#define RESTORE_TERRAIN_DIAMOND_H
#include <stdint.h>
/* The six native raster leaves preserve registers with PUSHAD/POPAD.
   Their callers discard EAX; they expose no meaningful integer return. */
void FUN_00427a20(const void *pixels,int vertical_clip);
void FUN_00428c00(const void *pixels,int vertical_clip);
void FUN_0042a160(const void *pixels,int vertical_clip);
void FUN_0042aa00(const void *pixels,int vertical_clip);
void FUN_0042b440(const void *pixels,int vertical_clip);
void FUN_0042bcf0(const void *pixels,int vertical_clip);
int FUN_0042c740(void);
int FUN_0042c9a0(void);
int FUN_0042cee0(void);
int FUN_0042d810(void);
int FUN_0042e680(void);
void restore_terrain_diamond(const void *pixels,int vertical_clip,int side,int masked);
int restore_terrain_footprint(unsigned size);
#ifdef TERRAIN_DIAMOND_TEST_TRACE
void terrain_diamond_test_trace(uint32_t entry,const void *pixels,int vertical_clip);
#define TERRAIN_TRACE(a,p,c) terrain_diamond_test_trace(a,p,c)
#else
#define TERRAIN_TRACE(a,p,c) ((void)0)
#endif
#endif
