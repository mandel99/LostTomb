#include "restore_types.h"
#define DAT_005707a4 (*(undefined4 *)0x005707a4u)
#define DAT_00604e78 (*(undefined *)0x00604e78u)
#define DAT_00604e7c (*(undefined *)0x00604e7cu)
#define DAT_00604e80 (*(undefined *)0x00604e80u)
#define DAT_00604f10 (*(undefined *)0x00604f10u)
#define DAT_00604f20 (*(undefined *)0x00604f20u)
#define DAT_0126649c (*(undefined *)0x0126649cu)
#define DAT_012664b0 (*(undefined *)0x012664b0u)
#define DAT_012664b8 (*(undefined *)0x012664b8u)
#define DAT_012664bc (*(undefined4 *)0x012664bcu)
#define DAT_012664fe (*(undefined *)0x012664feu)
#define DAT_0126650c (*(undefined *)0x0126650cu)
#define DAT_01266510 (*(undefined4 *)0x01266510u)
#define DAT_01266552 (*(undefined *)0x01266552u)
#define DAT_01266560 (*(undefined *)0x01266560u)
#define DAT_01266564 (*(undefined *)0x01266564u)
#define PTR_DAT_00604f04 (*(unsigned char * *)0x00604f04u)
#define PTR_DAT_00604f08 (*(unsigned char * *)0x00604f08u)
extern void * FUN_00561b68();
extern void FUN_00561bdc();
extern int FUN_00561c0b();
extern char * FUN_00562240();
extern int FUN_005631b0();
extern size_t FUN_00563380();
extern uint * FUN_00566750();
extern int FUN_005696a2();

void FUN_00566bc3(void)

{
  char cVar1;
  char cVar2;
  char *_Str1;
  DWORD DVar3;
  int iVar4;
  size_t sVar5;
  char *_Source;
  int local_4;
  
  DAT_012664b0 = 0;
  DAT_00604f20 = 0xffffffff;
  DAT_00604f10 = 0xffffffff;
  _Str1 = (char *)FUN_005696a2(&DAT_005707a4);
  if (_Str1 == (char *)0x0) {
    DVar3 = GetTimeZoneInformation((LPTIME_ZONE_INFORMATION)&DAT_012664b8);
    if (DVar3 == 0xffffffff) {
      return;
    }
    DAT_00604e78 = DAT_012664b8 * 0x3c;
    DAT_012664b0 = 1;
    if (DAT_012664fe != 0) {
      DAT_00604e78 = DAT_00604e78 + DAT_0126650c * 0x3c;
    }
    if ((DAT_01266552 == 0) || (DAT_01266560 == 0)) {
      DAT_00604e7c = 0;
      DAT_00604e80 = 0;
    }
    else {
      DAT_00604e7c = 1;
      DAT_00604e80 = (DAT_01266560 - DAT_0126650c) * 0x3c;
    }
    iVar4 = WideCharToMultiByte(DAT_0126649c,0x220,(LPCWSTR)&DAT_012664bc,-1,PTR_DAT_00604f04,0x3f,
                                (LPCSTR)0x0,&local_4);
    if ((iVar4 == 0) || (local_4 != 0)) {
      *PTR_DAT_00604f04 = 0;
    }
    else {
      ((undefined4 *)&PTR_DAT_00604f04)[0x3f] = 0;
    }
    iVar4 = WideCharToMultiByte(DAT_0126649c,0x220,(LPCWSTR)&DAT_01266510,-1,PTR_DAT_00604f08,0x3f,
                                (LPCSTR)0x0,&local_4);
    if ((iVar4 != 0) && (local_4 == 0)) {
      ((undefined4 *)&PTR_DAT_00604f08)[0x3f] = 0;
      return;
    }
  }
  else {
    if (*_Str1 == '\0') {
      return;
    }
    if ((DAT_01266564 != (char *)0x0) && (iVar4 = FUN_005631b0(_Str1,DAT_01266564), iVar4 == 0)) {
      return;
    }
    FUN_00561bdc(DAT_01266564);
    sVar5 = FUN_00563380(_Str1);
    DAT_01266564 = FUN_00561b68(sVar5 + 1);
    if (DAT_01266564 == (char *)0x0) {
      return;
    }
    FUN_00566750(DAT_01266564,_Str1);
    FUN_00562240(PTR_DAT_00604f04,_Str1,3);
    _Source = _Str1 + 3;
    ((undefined4 *)&PTR_DAT_00604f04)[3] = 0;
    cVar1 = *_Source;
    if (cVar1 == '-') {
      _Source = _Str1 + 4;
    }
    iVar4 = FUN_00561c0b(_Source);
    DAT_00604e78 = iVar4 * 0xe10;
    for (; (cVar2 = *_Source, cVar2 == '+' || (('/' < cVar2 && (cVar2 < ':'))));
        _Source = _Source + 1) {
    }
    if (*_Source == ':') {
      _Source = _Source + 1;
      iVar4 = FUN_00561c0b(_Source);
      DAT_00604e78 = DAT_00604e78 + iVar4 * 0x3c;
      for (; ('/' < *_Source && (*_Source < ':')); _Source = _Source + 1) {
      }
      if (*_Source == ':') {
        _Source = _Source + 1;
        iVar4 = FUN_00561c0b(_Source);
        DAT_00604e78 = DAT_00604e78 + iVar4;
        for (; ('/' < *_Source && (*_Source < ':')); _Source = _Source + 1) {
        }
      }
    }
    if (cVar1 == '-') {
      DAT_00604e78 = -DAT_00604e78;
    }
    DAT_00604e7c = (int)*_Source;
    if (DAT_00604e7c != 0) {
      FUN_00562240(PTR_DAT_00604f08,_Source,3);
      ((undefined4 *)&PTR_DAT_00604f08)[3] = 0;
      return;
    }
  }
  *PTR_DAT_00604f08 = 0;
  return;
}
