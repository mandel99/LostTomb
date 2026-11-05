#include "restore_types.h"
#define AV_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define AV_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define AV_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

/* Original-owner access repair, zero new algorithm credit. Native snapshots
   the building phase before scanning, then reads each descriptor and conditionally
   the actor BYTE before the descriptor kind DWORD. Keep those read phases. */
int FUN_00421230(int param_1,int param_2)
{
    uint32_t phase = AV_U(0x799a30u + (uint32_t)param_1 * 264u);
    uint32_t row = 0x5b0060u, descriptor, actor_kind, required_kind;
    do {
        descriptor = AV_U(row);
        if (AV_W(descriptor + 0x12u) == 0u && AV_U(row - 8u) == phase) {
            actor_kind = AV_B(0xbcd602u + (uint32_t)param_2 * 388u);
            required_kind = AV_U(row - 4u);
            if (required_kind == actor_kind && (int32_t)AV_U(descriptor + 12u) > 0)
                return param_1;
        }
        row += 16u;
        if (row >= 0x5b0150u) return 0;
    } while (1);
}
