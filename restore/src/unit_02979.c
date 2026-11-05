#include "crt_format.h"

RestoreCrtDecimal *restore_crt_binary64_decimal(uint64_t value)
{
 RestoreCrt96 raw80;
 RestoreCrtDecimalDigits *digits=(RestoreCrtDecimalDigits *)(uintptr_t)0x12666a8u;
 RestoreCrtDecimal *decimal=(RestoreCrtDecimal *)(uintptr_t)0x12666c8u;
 /* 568DEC writes exactly ten bytes. The other two copied native bytes
    are ignored by 56A47E, which masks its third DWORD to sign/exponent. */
 raw80.word[2]=0;
 FUN_00568dec(raw80.word,(const uint32_t *)&value);
 decimal->status=FUN_0056a47e(raw80.word[0],raw80.word[1],raw80.half[4],17,0,digits);
 decimal->sign=(int32_t)digits->sign;
 decimal->exponent=(int32_t)digits->exponent;
 decimal->digits=(char *)(uintptr_t)0x12666acu;
 return decimal;
}
