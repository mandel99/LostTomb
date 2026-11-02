#include "restore_types.h"
#include <string.h>
#define DAT_00570aa4 (*(undefined4 *)0x00570aa4u)
#define DAT_0060496c (*(undefined *)0x0060496cu)
#define DAT_00605158 (*(undefined4 *)0x00605158u)
#define DAT_01266408 (*(undefined *)0x01266408u)
extern char * FUN_00562240();
extern size_t FUN_00563380();
extern uint * FUN_00566750();
extern uint * FUN_00566760();
extern int FUN_00569c11();

void FUN_00567e05(DWORD param_1)

{
  undefined4 *puVar1;
  DWORD *pDVar2;
  DWORD DVar3;
  size_t sVar4;
  HANDLE hFile;
  int iVar5;
  char acStackY_1e3 [7];
  undefined1 *puStackY_1dc;
  char *pcStackY_1d8;
  undefined4 uStackY_1d4;
  undefined1 *puStackY_1d0;
  undefined4 uStackY_1cc;
  undefined1 *puStackY_1c8;
  undefined *puStackY_1c4;
  LPCVOID lpBuffer;
  LPDWORD lpNumberOfBytesWritten;
  LPOVERLAPPED lpOverlapped;
  CHAR local_1a8 [260];
  undefined1 local_a4 [160];
  
  iVar5 = 0;
  pDVar2 = &DAT_00605158;
  do {
    if (param_1 == *pDVar2) break;
    pDVar2 = pDVar2 + 2;
    iVar5 = iVar5 + 1;
  } while ((int)pDVar2 < 0x6051e8);
  if (param_1 == (&DAT_00605158)[iVar5 * 2]) {
    if ((DAT_01266408 == 1) || ((DAT_01266408 == 0 && (DAT_0060496c == 1)))) {
      lpNumberOfBytesWritten = &param_1;
      puVar1 = (undefined4 *)(iVar5 * 8 + 0x60515c);
      lpOverlapped = (LPOVERLAPPED)0x0;
      sVar4 = FUN_00563380((char *)*puVar1);
      lpBuffer = (LPCVOID)*puVar1;
      puStackY_1c4 = (undefined *)0x567f4e;
      hFile = GetStdHandle(0xfffffff4);
      puStackY_1c4 = (undefined *)0x567f55;
      WriteFile(hFile,lpBuffer,sVar4,lpNumberOfBytesWritten,lpOverlapped);
    }
    else if (param_1 != 0xfc) {
      DVar3 = GetModuleFileNameA((HMODULE)0x0,local_1a8,0x104);
      if (DVar3 == 0) {
        FUN_00566750();
      }
      sVar4 = FUN_00563380(local_1a8);
      if (0x3c < sVar4 + 1) {
        sVar4 = FUN_00563380(local_1a8);
        puStackY_1c4 = (undefined *)0x567ecf;
        FUN_00562240(acStackY_1e3 + sVar4,"...",3);
      }
      FUN_00566750();
      puStackY_1c4 = (undefined *)0x567ef0;
      FUN_00566760();
      puStackY_1c8 = local_a4;
      puStackY_1c4 = &DAT_00570aa4;
      uStackY_1cc = 0x567f01;
      FUN_00566760();
      uStackY_1cc = *(undefined4 *)(iVar5 * 8 + 0x60515c);
      puStackY_1d0 = local_a4;
      uStackY_1d4 = 0x567f13;
      FUN_00566760();
      uStackY_1d4 = 0x12010;
      puStackY_1dc = local_a4;
      pcStackY_1d8 = "Microsoft Visual C++ Runtime Library";
      memcpy(acStackY_1e3 + 3,")\x7fV",4);
      FUN_00569c11();
    }
  }
  return;
}
