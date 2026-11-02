#ifndef RESTORE_SPRITE_WARNING_H
#define RESTORE_SPRITE_WARNING_H
#include "sprite_libraries.h"
/* Native data\<name>.sg3 has a 260-byte path, hence valid basenames <=250.
   Two signed DWORD numbers and all native message fragments fit in 512 bytes. */
#define RESTORE_SPRITE_WARNING_CAPACITY 512
#endif
