#include <stdio.h>
#include "restore_intrinsics.h"

#define CHECK(expression,expected) do { \
    unsigned __int64 actual=(expression); \
    if (actual!=(unsigned __int64)(expected)) { \
        fprintf(stderr,"Width/overflow failure at line %d\n",__LINE__); \
        return 1; \
    } \
} while (0)

int main(void)
{
    CHECK(CONCAT11(0xffff1234ui64,0x5678ui64),0x3478ui64);
    CHECK(CONCAT31(0xabcdefui64,0x12ui64),0xabcdef12ui64);
    CHECK(CONCAT13(0x12ui64,0xabcdefui64),0x12abcdefui64);
    CHECK(CONCAT44(0x01234567ui64,0x89abcdefui64),0x0123456789abcdefui64);
    CHECK(CONCAT17(0x11ui64,0x22334455667788ui64),0x1122334455667788ui64);
    CHECK(SUB87(0xff0123456789abcdui64,1),0xff0123456789abui64);
    CHECK(SBORROW4(0x80000000ui64,1),1);
    CHECK(SBORROW4(0x7fffffffi64,0xffffffffui64),1);
    CHECK(SBORROW4(0x80000000ui64,0xffffffffui64),0);
    CHECK(SBORROW2(0x8000ui64,1),1);
    CHECK(CARRY4(0xffffffffui64,1),1);
    CHECK(CARRY4(0xffffffffui64,0),0);
    if (restore_x87_sqrt(0)!=0 || restore_x87_sqrt(25)!=5 ||
        fabs((double)restore_x87_sqrt(2)-1.4142135623730951)>1e-14) return 2;
    puts("PASS: integer helper widths, sign boundaries, and overflow flags.");
    return 0;
}
