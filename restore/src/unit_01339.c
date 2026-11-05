#include "route_helper_internal.h"

/* Width/ABI repair of the original complete 496640 owner. The address is
   a BYTE base plus actor*388; the old DWORD-base expression scaled twice. */
int32_t restore_transport_assert_no_predecessor(int32_t actor)
{
    if (RH_W(0xbcd660u + (uint32_t)actor * 388u) != 0) {
        /* Native 496657 jumps to itself. It does not reread the link. */
        for (;;) {
        }
    }
    return actor;
}
