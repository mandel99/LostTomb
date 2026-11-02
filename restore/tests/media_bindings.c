#include "media_bindings.h"
#include "media_loader.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
static unsigned int expected,seen,resolutions;
static int __stdcall fixture0(uintptr_t p0,uintptr_t p1,uintptr_t p2)
{
 if(expected!=0u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 if(p1!=(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u))) abort();
 if(p2!=(uintptr_t)(0x89abcdefu ^ (2u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+0u);
}
static int __stdcall fixture1(uintptr_t p0)
{
 if(expected!=1u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+1u);
}
static int __stdcall fixture2(uintptr_t p0,uintptr_t p1)
{
 if(expected!=2u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 if(p1!=(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+2u);
}
static int __stdcall fixture3(uintptr_t p0)
{
 if(expected!=3u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+3u);
}
static int __stdcall fixture4(uintptr_t p0)
{
 if(expected!=4u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+4u);
}
static int __stdcall fixture5(uintptr_t p0)
{
 if(expected!=5u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+5u);
}
static int __stdcall fixture6(uintptr_t p0)
{
 if(expected!=6u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+6u);
}
static int __stdcall fixture7(uintptr_t p0)
{
 if(expected!=7u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+7u);
}
static int __stdcall fixture8(uintptr_t p0)
{
 if(expected!=8u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+8u);
}
static int __stdcall fixture9(uintptr_t p0,uintptr_t p1,uintptr_t p2)
{
 if(expected!=9u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 if(p1!=(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u))) abort();
 if(p2!=(uintptr_t)(0x89abcdefu ^ (2u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+9u);
}
static int __stdcall fixture10(uintptr_t p0,uintptr_t p1)
{
 if(expected!=10u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 if(p1!=(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+10u);
}
static int __stdcall fixture11(uintptr_t p0)
{
 if(expected!=11u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+11u);
}
static int __stdcall fixture12(uintptr_t p0)
{
 if(expected!=12u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+12u);
}
static int __stdcall fixture13(uintptr_t p0)
{
 if(expected!=13u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+13u);
}
static int __stdcall fixture14(uintptr_t p0,uintptr_t p1,uintptr_t p2)
{
 if(expected!=14u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 if(p1!=(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u))) abort();
 if(p2!=(uintptr_t)(0x89abcdefu ^ (2u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+14u);
}
static int __stdcall fixture15(uintptr_t p0)
{
 if(expected!=15u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+15u);
}
static int __stdcall fixture16(uintptr_t p0)
{
 if(expected!=16u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+16u);
}
static int __stdcall fixture17(uintptr_t p0)
{
 if(expected!=17u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+17u);
}
static int __stdcall fixture18(uintptr_t p0)
{
 if(expected!=18u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+18u);
}
static int __stdcall fixture19(uintptr_t p0,uintptr_t p1,uintptr_t p2,uintptr_t p3,uintptr_t p4,uintptr_t p5,uintptr_t p6)
{
 if(expected!=19u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 if(p1!=(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u))) abort();
 if(p2!=(uintptr_t)(0x89abcdefu ^ (2u * 0x1020304u))) abort();
 if(p3!=(uintptr_t)(0x89abcdefu ^ (3u * 0x1020304u))) abort();
 if(p4!=(uintptr_t)(0x89abcdefu ^ (4u * 0x1020304u))) abort();
 if(p5!=(uintptr_t)(0x89abcdefu ^ (5u * 0x1020304u))) abort();
 if(p6!=(uintptr_t)(0x89abcdefu ^ (6u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+19u);
}
static int __stdcall fixture20(uintptr_t p0,uintptr_t p1,uintptr_t p2,uintptr_t p3)
{
 if(expected!=20u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 if(p1!=(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u))) abort();
 if(p2!=(uintptr_t)(0x89abcdefu ^ (2u * 0x1020304u))) abort();
 if(p3!=(uintptr_t)(0x89abcdefu ^ (3u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+20u);
}
static int __stdcall fixture21(uintptr_t p0,uintptr_t p1,uintptr_t p2)
{
 if(expected!=21u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 if(p1!=(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u))) abort();
 if(p2!=(uintptr_t)(0x89abcdefu ^ (2u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+21u);
}
static int __stdcall fixture22(uintptr_t p0,uintptr_t p1)
{
 if(expected!=22u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 if(p1!=(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+22u);
}
static int __stdcall fixture23(uintptr_t p0,uintptr_t p1)
{
 if(expected!=23u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 if(p1!=(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+23u);
}
static int __stdcall fixture24(uintptr_t p0,uintptr_t p1)
{
 if(expected!=24u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 if(p1!=(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+24u);
}
static int __stdcall fixture25(uintptr_t p0,uintptr_t p1)
{
 if(expected!=25u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 if(p1!=(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+25u);
}
static int __stdcall fixture26(uintptr_t p0,uintptr_t p1,uintptr_t p2)
{
 if(expected!=26u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 if(p1!=(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u))) abort();
 if(p2!=(uintptr_t)(0x89abcdefu ^ (2u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+26u);
}
static int __stdcall fixture27(uintptr_t p0,uintptr_t p1)
{
 if(expected!=27u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 if(p1!=(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+27u);
}
static int __stdcall fixture28(uintptr_t p0,uintptr_t p1)
{
 if(expected!=28u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 if(p1!=(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+28u);
}
static int __stdcall fixture29(uintptr_t p0,uintptr_t p1,uintptr_t p2,uintptr_t p3)
{
 if(expected!=29u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 if(p1!=(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u))) abort();
 if(p2!=(uintptr_t)(0x89abcdefu ^ (2u * 0x1020304u))) abort();
 if(p3!=(uintptr_t)(0x89abcdefu ^ (3u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+29u);
}
static int __stdcall fixture30(uintptr_t p0,uintptr_t p1)
{
 if(expected!=30u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 if(p1!=(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+30u);
}
static int __stdcall fixture31(uintptr_t p0,uintptr_t p1)
{
 if(expected!=31u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 if(p1!=(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+31u);
}
static int __stdcall fixture32(uintptr_t p0,uintptr_t p1)
{
 if(expected!=32u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 if(p1!=(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+32u);
}
static int __stdcall fixture33(void)
{
 if(expected!=33u) abort();
 ++seen; return (int)(0xa9870000u+33u);
}
static int __stdcall fixture34(uintptr_t p0)
{
 if(expected!=34u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+34u);
}
static int __stdcall fixture35(uintptr_t p0)
{
 if(expected!=35u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+35u);
}
static int __stdcall fixture36(uintptr_t p0)
{
 if(expected!=36u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+36u);
}
static int __stdcall fixture37(void)
{
 if(expected!=37u) abort();
 ++seen; return (int)(0xa9870000u+37u);
}
static int __stdcall fixture38(uintptr_t p0)
{
 if(expected!=38u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+38u);
}
static int __stdcall fixture39(uintptr_t p0)
{
 if(expected!=39u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+39u);
}
static int __stdcall fixture40(uintptr_t p0,uintptr_t p1,uintptr_t p2,uintptr_t p3)
{
 if(expected!=40u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 if(p1!=(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u))) abort();
 if(p2!=(uintptr_t)(0x89abcdefu ^ (2u * 0x1020304u))) abort();
 if(p3!=(uintptr_t)(0x89abcdefu ^ (3u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+40u);
}
static int __stdcall fixture41(uintptr_t p0)
{
 if(expected!=41u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+41u);
}
static int __stdcall fixture42(uintptr_t p0)
{
 if(expected!=42u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+42u);
}
static int __stdcall fixture43(uintptr_t p0,uintptr_t p1,uintptr_t p2,uintptr_t p3,uintptr_t p4,uintptr_t p5,uintptr_t p6)
{
 if(expected!=43u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 if(p1!=(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u))) abort();
 if(p2!=(uintptr_t)(0x89abcdefu ^ (2u * 0x1020304u))) abort();
 if(p3!=(uintptr_t)(0x89abcdefu ^ (3u * 0x1020304u))) abort();
 if(p4!=(uintptr_t)(0x89abcdefu ^ (4u * 0x1020304u))) abort();
 if(p5!=(uintptr_t)(0x89abcdefu ^ (5u * 0x1020304u))) abort();
 if(p6!=(uintptr_t)(0x89abcdefu ^ (6u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+43u);
}
static int __stdcall fixture44(uintptr_t p0)
{
 if(expected!=44u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+44u);
}
static int __stdcall fixture45(uintptr_t p0)
{
 if(expected!=45u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+45u);
}
static int __stdcall fixture46(void)
{
 if(expected!=46u) abort();
 ++seen; return (int)(0xa9870000u+46u);
}
static int __stdcall fixture47(uintptr_t p0)
{
 if(expected!=47u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+47u);
}
static int __stdcall fixture48(uintptr_t p0)
{
 if(expected!=48u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+48u);
}
static int __stdcall fixture49(uintptr_t p0,uintptr_t p1)
{
 if(expected!=49u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 if(p1!=(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+49u);
}
static int __stdcall fixture50(uintptr_t p0)
{
 if(expected!=50u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+50u);
}
static int __stdcall fixture51(uintptr_t p0,uintptr_t p1)
{
 if(expected!=51u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 if(p1!=(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+51u);
}
static int __stdcall fixture52(uintptr_t p0)
{
 if(expected!=52u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+52u);
}
static int __stdcall fixture53(uintptr_t p0)
{
 if(expected!=53u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+53u);
}
static int __stdcall fixture54(uintptr_t p0)
{
 if(expected!=54u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+54u);
}
static int __stdcall fixture55(uintptr_t p0)
{
 if(expected!=55u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+55u);
}
static int __stdcall fixture56(uintptr_t p0,uintptr_t p1,uintptr_t p2)
{
 if(expected!=56u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 if(p1!=(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u))) abort();
 if(p2!=(uintptr_t)(0x89abcdefu ^ (2u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+56u);
}
static int __stdcall fixture57(uintptr_t p0)
{
 if(expected!=57u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+57u);
}
static int __stdcall fixture58(uintptr_t p0,uintptr_t p1,uintptr_t p2,uintptr_t p3,uintptr_t p4,uintptr_t p5,uintptr_t p6)
{
 if(expected!=58u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 if(p1!=(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u))) abort();
 if(p2!=(uintptr_t)(0x89abcdefu ^ (2u * 0x1020304u))) abort();
 if(p3!=(uintptr_t)(0x89abcdefu ^ (3u * 0x1020304u))) abort();
 if(p4!=(uintptr_t)(0x89abcdefu ^ (4u * 0x1020304u))) abort();
 if(p5!=(uintptr_t)(0x89abcdefu ^ (5u * 0x1020304u))) abort();
 if(p6!=(uintptr_t)(0x89abcdefu ^ (6u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+58u);
}
static int __stdcall fixture59(uintptr_t p0)
{
 if(expected!=59u) abort();
 if(p0!=(uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u))) abort();
 ++seen; return (int)(0xa9870000u+59u);
}
FARPROC restore_media_resolve(const char *dll,const char *name)
{
 ++resolutions;
 switch(expected) {
 case 0: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_3D_provider_attribute@12")) abort(); return (FARPROC)fixture0;
 case 1: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_3D_sample_status@4")) abort(); return (FARPROC)fixture1;
 case 2: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_3D_update_position@8")) abort(); return (FARPROC)fixture2;
 case 3: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_allocate_3D_sample_handle@4")) abort(); return (FARPROC)fixture3;
 case 4: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_allocate_sample_handle@4")) abort(); return (FARPROC)fixture4;
 case 5: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_close_3D_provider@4")) abort(); return (FARPROC)fixture5;
 case 6: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_close_stream@4")) abort(); return (FARPROC)fixture6;
 case 7: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_end_3D_sample@4")) abort(); return (FARPROC)fixture7;
 case 8: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_end_sample@4")) abort(); return (FARPROC)fixture8;
 case 9: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_enumerate_3D_providers@12")) abort(); return (FARPROC)fixture9;
 case 10: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_file_read@8")) abort(); return (FARPROC)fixture10;
 case 11: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_init_sample@4")) abort(); return (FARPROC)fixture11;
 case 12: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_mem_free_lock@4")) abort(); return (FARPROC)fixture12;
 case 13: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_open_3D_provider@4")) abort(); return (FARPROC)fixture13;
 case 14: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_open_stream@12")) abort(); return (FARPROC)fixture14;
 case 15: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_release_3D_sample_handle@4")) abort(); return (FARPROC)fixture15;
 case 16: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_release_sample_handle@4")) abort(); return (FARPROC)fixture16;
 case 17: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_sample_loop_count@4")) abort(); return (FARPROC)fixture17;
 case 18: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_sample_status@4")) abort(); return (FARPROC)fixture18;
 case 19: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_set_3D_orientation@28")) abort(); return (FARPROC)fixture19;
 case 20: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_set_3D_position@16")) abort(); return (FARPROC)fixture20;
 case 21: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_set_3D_provider_preference@12")) abort(); return (FARPROC)fixture21;
 case 22: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_set_3D_sample_file@8")) abort(); return (FARPROC)fixture22;
 case 23: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_set_3D_sample_loop_count@8")) abort(); return (FARPROC)fixture23;
 case 24: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_set_3D_sample_volume@8")) abort(); return (FARPROC)fixture24;
 case 25: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_set_preference@8")) abort(); return (FARPROC)fixture25;
 case 26: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_set_sample_file@12")) abort(); return (FARPROC)fixture26;
 case 27: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_set_sample_loop_count@8")) abort(); return (FARPROC)fixture27;
 case 28: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_set_sample_pan@8")) abort(); return (FARPROC)fixture28;
 case 29: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_set_sample_reverb@16")) abort(); return (FARPROC)fixture29;
 case 30: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_set_sample_volume@8")) abort(); return (FARPROC)fixture30;
 case 31: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_set_stream_loop_count@8")) abort(); return (FARPROC)fixture31;
 case 32: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_set_stream_volume@8")) abort(); return (FARPROC)fixture32;
 case 33: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_shutdown@0")) abort(); return (FARPROC)fixture33;
 case 34: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_start_3D_sample@4")) abort(); return (FARPROC)fixture34;
 case 35: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_start_sample@4")) abort(); return (FARPROC)fixture35;
 case 36: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_start_stream@4")) abort(); return (FARPROC)fixture36;
 case 37: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_startup@0")) abort(); return (FARPROC)fixture37;
 case 38: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_stream_status@4")) abort(); return (FARPROC)fixture38;
 case 39: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_stream_volume@4")) abort(); return (FARPROC)fixture39;
 case 40: if(strcmp(dll,"mss32.dll")||strcmp(name,"_AIL_waveOutOpen@16")) abort(); return (FARPROC)fixture40;
 case 41: if(strcmp(dll,"binkw32.dll")||strcmp(name,"_BinkBufferClose@4")) abort(); return (FARPROC)fixture41;
 case 42: if(strcmp(dll,"binkw32.dll")||strcmp(name,"_BinkClose@4")) abort(); return (FARPROC)fixture42;
 case 43: if(strcmp(dll,"binkw32.dll")||strcmp(name,"_BinkCopyToBuffer@28")) abort(); return (FARPROC)fixture43;
 case 44: if(strcmp(dll,"binkw32.dll")||strcmp(name,"_BinkDDSurfaceType@4")) abort(); return (FARPROC)fixture44;
 case 45: if(strcmp(dll,"binkw32.dll")||strcmp(name,"_BinkDoFrame@4")) abort(); return (FARPROC)fixture45;
 case 46: if(strcmp(dll,"binkw32.dll")||strcmp(name,"_BinkGetError@0")) abort(); return (FARPROC)fixture46;
 case 47: if(strcmp(dll,"binkw32.dll")||strcmp(name,"_BinkNextFrame@4")) abort(); return (FARPROC)fixture47;
 case 48: if(strcmp(dll,"binkw32.dll")||strcmp(name,"_BinkOpenMiles@4")) abort(); return (FARPROC)fixture48;
 case 49: if(strcmp(dll,"binkw32.dll")||strcmp(name,"_BinkOpen@8")) abort(); return (FARPROC)fixture49;
 case 50: if(strcmp(dll,"binkw32.dll")||strcmp(name,"_BinkService@4")) abort(); return (FARPROC)fixture50;
 case 51: if(strcmp(dll,"binkw32.dll")||strcmp(name,"_BinkSetSoundSystem@8")) abort(); return (FARPROC)fixture51;
 case 52: if(strcmp(dll,"binkw32.dll")||strcmp(name,"_BinkWait@4")) abort(); return (FARPROC)fixture52;
 case 53: if(strcmp(dll,"smackw32.dll")||strcmp(name,"_SmackClose@4")) abort(); return (FARPROC)fixture53;
 case 54: if(strcmp(dll,"smackw32.dll")||strcmp(name,"_SmackDoFrame@4")) abort(); return (FARPROC)fixture54;
 case 55: if(strcmp(dll,"smackw32.dll")||strcmp(name,"_SmackNextFrame@4")) abort(); return (FARPROC)fixture55;
 case 56: if(strcmp(dll,"smackw32.dll")||strcmp(name,"_SmackOpen@12")) abort(); return (FARPROC)fixture56;
 case 57: if(strcmp(dll,"smackw32.dll")||strcmp(name,"_SmackSoundUseMSS@4")) abort(); return (FARPROC)fixture57;
 case 58: if(strcmp(dll,"smackw32.dll")||strcmp(name,"_SmackToBuffer@28")) abort(); return (FARPROC)fixture58;
 case 59: if(strcmp(dll,"smackw32.dll")||strcmp(name,"_SmackWait@4")) abort(); return (FARPROC)fixture59;
 default: abort();
 }
}
int main(void)
{
 unsigned int iteration;
 for(iteration=0;iteration<1000;++iteration) {
 expected=0; if(_AIL_3D_provider_attribute_12((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (2u * 0x1020304u)))!=(int)(0xa9870000u+0u)) return 1;
 expected=1; if(_AIL_3D_sample_status_4((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)))!=(int)(0xa9870000u+1u)) return 1;
 expected=2; if(_AIL_3D_update_position_8((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u)))!=(int)(0xa9870000u+2u)) return 1;
 expected=3; if(_AIL_allocate_3D_sample_handle_4((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)))!=(int)(0xa9870000u+3u)) return 1;
 expected=4; if(_AIL_allocate_sample_handle_4((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)))!=(int)(0xa9870000u+4u)) return 1;
 expected=5; if(_AIL_close_3D_provider_4((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)))!=(int)(0xa9870000u+5u)) return 1;
 expected=6; if(_AIL_close_stream_4((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)))!=(int)(0xa9870000u+6u)) return 1;
 expected=7; if(_AIL_end_3D_sample_4((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)))!=(int)(0xa9870000u+7u)) return 1;
 expected=8; if(_AIL_end_sample_4((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)))!=(int)(0xa9870000u+8u)) return 1;
 expected=9; if(_AIL_enumerate_3D_providers_12((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (2u * 0x1020304u)))!=(int)(0xa9870000u+9u)) return 1;
 expected=10; if(_AIL_file_read_8((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u)))!=(int)(0xa9870000u+10u)) return 1;
 expected=11; if(_AIL_init_sample_4((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)))!=(int)(0xa9870000u+11u)) return 1;
 expected=12; if(_AIL_mem_free_lock_4((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)))!=(int)(0xa9870000u+12u)) return 1;
 expected=13; if(_AIL_open_3D_provider_4((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)))!=(int)(0xa9870000u+13u)) return 1;
 expected=14; if(_AIL_open_stream_12((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (2u * 0x1020304u)))!=(int)(0xa9870000u+14u)) return 1;
 expected=15; if(_AIL_release_3D_sample_handle_4((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)))!=(int)(0xa9870000u+15u)) return 1;
 expected=16; if(_AIL_release_sample_handle_4((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)))!=(int)(0xa9870000u+16u)) return 1;
 expected=17; if(_AIL_sample_loop_count_4((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)))!=(int)(0xa9870000u+17u)) return 1;
 expected=18; if(_AIL_sample_status_4((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)))!=(int)(0xa9870000u+18u)) return 1;
 expected=19; if(_AIL_set_3D_orientation_28((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (2u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (3u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (4u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (5u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (6u * 0x1020304u)))!=(int)(0xa9870000u+19u)) return 1;
 expected=20; if(_AIL_set_3D_position_16((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (2u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (3u * 0x1020304u)))!=(int)(0xa9870000u+20u)) return 1;
 expected=21; if(_AIL_set_3D_provider_preference_12((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (2u * 0x1020304u)))!=(int)(0xa9870000u+21u)) return 1;
 expected=22; if(_AIL_set_3D_sample_file_8((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u)))!=(int)(0xa9870000u+22u)) return 1;
 expected=23; if(_AIL_set_3D_sample_loop_count_8((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u)))!=(int)(0xa9870000u+23u)) return 1;
 expected=24; if(_AIL_set_3D_sample_volume_8((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u)))!=(int)(0xa9870000u+24u)) return 1;
 expected=25; if(_AIL_set_preference_8((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u)))!=(int)(0xa9870000u+25u)) return 1;
 expected=26; if(_AIL_set_sample_file_12((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (2u * 0x1020304u)))!=(int)(0xa9870000u+26u)) return 1;
 expected=27; if(_AIL_set_sample_loop_count_8((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u)))!=(int)(0xa9870000u+27u)) return 1;
 expected=28; if(_AIL_set_sample_pan_8((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u)))!=(int)(0xa9870000u+28u)) return 1;
 expected=29; if(_AIL_set_sample_reverb_16((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (2u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (3u * 0x1020304u)))!=(int)(0xa9870000u+29u)) return 1;
 expected=30; if(_AIL_set_sample_volume_8((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u)))!=(int)(0xa9870000u+30u)) return 1;
 expected=31; if(_AIL_set_stream_loop_count_8((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u)))!=(int)(0xa9870000u+31u)) return 1;
 expected=32; if(_AIL_set_stream_volume_8((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u)))!=(int)(0xa9870000u+32u)) return 1;
 expected=33; if(_AIL_shutdown_0()!=(int)(0xa9870000u+33u)) return 1;
 expected=34; if(_AIL_start_3D_sample_4((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)))!=(int)(0xa9870000u+34u)) return 1;
 expected=35; if(_AIL_start_sample_4((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)))!=(int)(0xa9870000u+35u)) return 1;
 expected=36; if(_AIL_start_stream_4((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)))!=(int)(0xa9870000u+36u)) return 1;
 expected=37; if(_AIL_startup_0()!=(int)(0xa9870000u+37u)) return 1;
 expected=38; if(_AIL_stream_status_4((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)))!=(int)(0xa9870000u+38u)) return 1;
 expected=39; if(_AIL_stream_volume_4((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)))!=(int)(0xa9870000u+39u)) return 1;
 expected=40; if(_AIL_waveOutOpen_16((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (2u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (3u * 0x1020304u)))!=(int)(0xa9870000u+40u)) return 1;
 expected=41; if(_BinkBufferClose_4((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)))!=(int)(0xa9870000u+41u)) return 1;
 expected=42; if(_BinkClose_4((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)))!=(int)(0xa9870000u+42u)) return 1;
 expected=43; if(_BinkCopyToBuffer_28((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (2u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (3u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (4u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (5u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (6u * 0x1020304u)))!=(int)(0xa9870000u+43u)) return 1;
 expected=44; if(_BinkDDSurfaceType_4((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)))!=(int)(0xa9870000u+44u)) return 1;
 expected=45; if(_BinkDoFrame_4((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)))!=(int)(0xa9870000u+45u)) return 1;
 expected=46; if(_BinkGetError_0()!=(int)(0xa9870000u+46u)) return 1;
 expected=47; if(_BinkNextFrame_4((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)))!=(int)(0xa9870000u+47u)) return 1;
 expected=48; if(_BinkOpenMiles_4((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)))!=(int)(0xa9870000u+48u)) return 1;
 expected=49; if(_BinkOpen_8((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u)))!=(int)(0xa9870000u+49u)) return 1;
 expected=50; if(_BinkService_4((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)))!=(int)(0xa9870000u+50u)) return 1;
 expected=51; if(_BinkSetSoundSystem_8((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u)))!=(int)(0xa9870000u+51u)) return 1;
 expected=52; if(_BinkWait_4((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)))!=(int)(0xa9870000u+52u)) return 1;
 expected=53; if(_SmackClose_4((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)))!=(int)(0xa9870000u+53u)) return 1;
 expected=54; if(_SmackDoFrame_4((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)))!=(int)(0xa9870000u+54u)) return 1;
 expected=55; if(_SmackNextFrame_4((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)))!=(int)(0xa9870000u+55u)) return 1;
 expected=56; if(_SmackOpen_12((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (2u * 0x1020304u)))!=(int)(0xa9870000u+56u)) return 1;
 expected=57; if(_SmackSoundUseMSS_4((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)))!=(int)(0xa9870000u+57u)) return 1;
 expected=58; if(_SmackToBuffer_28((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (1u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (2u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (3u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (4u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (5u * 0x1020304u)),(uintptr_t)(0x89abcdefu ^ (6u * 0x1020304u)))!=(int)(0xa9870000u+58u)) return 1;
 expected=59; if(_SmackWait_4((uintptr_t)(0x89abcdefu ^ (0u * 0x1020304u)))!=(int)(0xa9870000u+59u)) return 1;
 }
 if(seen!=60000u||resolutions!=seen) return 2;
 puts("60 multimedia forwarding signatures, raw argument bits, return values and stack cleanup verified.");
 return 0;
}
