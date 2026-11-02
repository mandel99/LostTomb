#ifndef RESTORE_SPRITE_READER_EXACT_H
#define RESTORE_SPRITE_READER_EXACT_H
#include "sprite_libraries.h"
/* 53DFF0: metadata starts at saved-register ESP+130h and ends before SEH
   at ESP+D8A58h. The native read API has no capacity argument. */
#define RESTORE_SPRITE_METADATA_CAPACITY 0xD8928u
/* The existing seven-C-argument FUN_0053dff0 declaration is unchanged.
   Defined SG3 inputs fit this buffer and the supplied image/tag destinations.
   Failed/short SG3 reads leave native stack bytes indeterminate; no successful
   fallback or malformed-file compatibility is supplied here. */
#endif
