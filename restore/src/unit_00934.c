#include "city_prepare.h"

#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define I(a) (*(int32_t *)(uintptr_t)(a))
#define H(a) (*(int16_t *)(uintptr_t)(a))
extern uint32_t FUN_004c06e0(uint32_t, int32_t);
extern int32_t FUN_004e94a0(int32_t);
extern int32_t FUN_004e94d0(int32_t);
extern void FUN_004e7e60(uint32_t *,uint32_t *,uint32_t *,uint32_t *);

static int32_t coverage(uint32_t count, uint32_t weight, int32_t population)
{
    return FUN_004cde60((int32_t)(count * weight), population);
}

void FUN_00468170(void)
{
    static const uint32_t inputs[5] = {0x7057b8,0x7057b4,0x7057c0,0x7057bc,0x705ac4};
    static const uint32_t weights[5] = {400,700,1200,5000,7500};
    uint32_t i, active = 0, sum = 0, player = U(0xeaac28), offset = player * 0x49d8u;
    int32_t population = I(0xea1894u + offset);
    for (i = 0; i < 5; ++i)
        I(0x78d5f0u + i * 4) = coverage(U(inputs[i]), weights[i], population);
    for (i = 0; i < 5; ++i)
        if (I(0x78d5f0u + i * 4) > 100) I(0x78d5f0u + i * 4) = 100;

    for (i = 0; i < 5; ++i) {
        int32_t mode = H(0x7848e2u + i * 2), value = 0;
        if (mode == 1 || mode == 2) {
            uint32_t a = U(0x70573cu + FUN_004c06e0(i,1) * 4);
            uint32_t b = U(0x70573cu + FUN_004c06e0(i,0) * 4);
            uint32_t c = U(0x70573cu + FUN_004c06e0(i,2) * 4);
            uint32_t numerator = a * 8000u + b * (mode == 2 ? 375u : 750u)
                               + c * (mode == 2 ? 150u : 300u);
            value = FUN_004cde60((int32_t)numerator, population);
            ++active;
        }
        if (value > 100) value = 100;
        I(0x78d620u + i * 4) = value;
        sum += (uint32_t)value;
    }
    /* Native FILD/FIDIV/ftol truncates the wrapped signed sum. With no
       active category, masked x87 invalid produces INT64_MIN: its low word is 0. */
    I(0x78d60c) = active ? (int32_t)sum / (int32_t)active : 0;
    I(0xea189cu + offset) = FUN_004e94a0((int32_t)player);
    I(0xea18a0u + offset) = FUN_004e94d0((int32_t)player);
    I(0x78d604) = coverage(U(0x705808),300,I(0xea189cu + offset));
    I(0x78d608) = coverage(U(0x705810),800,population);
    if (I(0x78d604) > 100) I(0x78d604) = 100;
    if (I(0x78d608) > 100) I(0x78d608) = 100;
    FUN_004e7e60(&U(0x78d614),&U(0x78d61c),&U(0x78d618),&U(0x78d610));
}
