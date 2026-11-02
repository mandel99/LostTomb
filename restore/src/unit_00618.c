#include "town_menu.h"
#include "trade_updates.h"
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define I(a) (*(int32_t *)(uintptr_t)(a))
extern int32_t FUN_004f57b0(int32_t);

void FUN_00443330(void)
{
    uint32_t costs[36] = {0},resource;
    RestoreTownRecord *town = (RestoreTownRecord *)FUN_0044baf0(0);
    while (town) {
        if ((uint8_t)FUN_0044d4c0(town)) {
            int32_t kind = *(const int16_t *)(town->bytes + 4);
            if (kind > 0 && kind < 36)
                costs[kind] += (uint32_t)(int32_t)*(const int16_t *)(town->bytes + 12) * 100u;
        }
        town = (RestoreTownRecord *)FUN_0044baf0((const int16_t *)town->bytes);
    }
    for (resource = 1; resource < 36; ++resource) {
        uint32_t offset = (resource - 1u) * 4u,base = 1000u,j;
        costs[resource] += (U(0x784decu + offset) - U(0x784e7cu + offset)) * 100u;
        for (j = 0; j < 6; ++j) {
            uint32_t category = U(0x5d2e8cu + j * 4u);
            if ((category == 1 && resource <= 8) || category == resource) {
                int32_t population = I(0xea1894u + U(0xeaac28) * 0x49d8u);
                uint32_t band = population < 1000 ? 0 : population < 2500 ? 1 : population < 5000 ? 2 : 3;
                base = U(0x7758a0u + band * 24u + j * 4u);
                break;
            }
        }
        if (j == 6) {
            switch (resource) {
            case 9: case 11: case 14: case 16: case 18: case 20: case 22: case 29: case 32:
                base = ((uint32_t)FUN_00443080(resource) + 1u) * 200u;
                break;
            case 12: case 24: case 25: case 26: case 30:
                base = (uint32_t)FUN_004f57b0((int32_t)resource) + 500u;
                break;
            }
        }
        I(0x775814u + offset) = FUN_00559c60(resource,(int32_t)(base + costs[resource]));
    }
}
