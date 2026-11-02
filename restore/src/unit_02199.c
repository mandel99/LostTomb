#include "restore_types.h"
#include "mission_compat.h"
void FUN_004f67e0(RestoreDynamicHistory *history,const RestoreHistorySnapshot *source)
{
    RestoreHistorySnapshot snapshot=*source;
    (void)FUN_004f6700(history,snapshot.capacity);
    /* Native 004f67e0 imports metadata only; snapshot.values is not copied. */
    history->position=snapshot.position;history->flag=snapshot.flag;
    history->head=snapshot.head;history->step=snapshot.step;
}
