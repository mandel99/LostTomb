#include "population_advisor_render.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
/* Whole4E97C0: signed WORD start year, DWORD current date/player and the
   native arithmetic return, including negative distances shorter than a year. */
int32_t FUN_004e97c0(int32_t distance)
{
    uint32_t year,month,result;int32_t years=distance/12;
    if(I(U(0xea3e30u+U(0xeaac28u)*0x49d8u))<=distance){
        year=(uint32_t)(int32_t)*(volatile int16_t *)(uintptr_t)0x7848d8u;
        U(0xea180cu)=0;U(0xea1808u)=year;U(0xea1804u)=(uint32_t)(distance%12);
        U(0xea1800u)=year+(uint32_t)years;
        return I((uint32_t)years>>31u);
    }
    year=U(0x78d7bcu);U(0xea1800u)=year;
    month=U(0x78d7c0u)-1u;U(0xea1804u)=month;
    if(I(month)<0){month+=12u;--year;U(0xea1804u)=month;U(0xea1800u)=year;}
    month-=(uint32_t)(distance%12);U(0xea180cu)=month;result=(uint32_t)years;
    if(I(month)<0){result=month+12u;U(0xea180cu)=result;}
    U(0xea1808u)=year-(uint32_t)years;
    return I(result);
}
