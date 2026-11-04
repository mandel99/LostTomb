float10 FUN_004f78a0(char param_1)

{
  short sVar1;
  bool bVar2;
  char cVar3;
  int iVar4;
  short *psVar5;
  int iVar6;
  int iVar7;
  int *piVar8;
  float10 fVar9;
  int iStack_88;
  double dStack_80;
  undefined1 auStack_78 [52];
  undefined4 uStack_44;
  
  dStack_80 = 0.0;
  bVar2 = false;
  iVar4 = FUN_004f4ff0();
  if (iVar4 == 0) {
    fVar9 = (float10)0.0;
  }
  else if (param_1 == '\0') {
    fVar9 = (float10)0.0;
    iStack_88 = 0;
    bVar2 = false;
    do {
      iVar6 = (_DAT_00eaac28 * 0x1276 + iStack_88) * 4;
      iVar4 = *(int *)(iVar6 + 0xea5fac);
      if (iVar4 == 0) goto switchD_004f79d2_caseD_b8;
      iVar7 = iVar4 * 0x108;
      iVar6 = *(int *)(iVar6 + 0xea5fa0) * 0x10;
      switch(*(undefined2 *)(&DAT_007999e8 + iVar7)) {
      case 0xb7:
        if (*(short *)(iVar7 + 0x799a10) == 0) {
          FUN_004f1310(iVar4,auStack_78);
          cVar3 = FUN_004f53d0(iVar4);
          if (cVar3 == '\0') {
            bVar2 = true;
          }
          iVar4 = FUN_004cde40((int)*(short *)(iVar6 + 0x5d19d4),uStack_44);
          fVar9 = (float10)iVar4;
          break;
        }
      default:
        goto switchD_004f79d2_caseD_b8;
      case 0xcf:
      case 0xd0:
        if (*(byte *)(iVar7 + 0x799a2d) < 100) {
          bVar2 = true;
        }
        iVar4 = FUN_004cde40((int)*(short *)(iVar6 + 0x5d19d4),*(byte *)(iVar7 + 0x799a2d));
        fVar9 = (float10)iVar4;
        break;
      case 0xd2:
        sVar1 = *(short *)(_DAT_00eaac28 * 0x49d8 + 0xea5baa);
        if (sVar1 < 0x10) {
          bVar2 = true;
        }
        iVar4 = sVar1 * 100;
        iVar4 = FUN_004cde40((int)*(short *)(iVar6 + 0x5d19d4),
                             (int)(iVar4 + (iVar4 >> 0x1f & 0xfU)) >> 4);
        fVar9 = (float10)iVar4;
        break;
      case 0xd5:
        iVar4 = FUN_00542ad0(iVar4,0);
        if (iVar4 < 100) {
          bVar2 = true;
        }
        iVar4 = FUN_004cde40((int)*(short *)(iVar6 + 0x5d19d4),iVar4);
        fVar9 = (float10)iVar4;
        break;
      case 0xde:
        iVar4 = FUN_004df040(iVar4,0);
        if (iVar4 < 100) {
          bVar2 = true;
        }
        iVar4 = FUN_004cde40((int)*(short *)(iVar6 + 0x5d19d4),iVar4);
        fVar9 = (float10)iVar4;
        break;
      case 0xe1:
        iVar4 = FUN_0040d940(iVar4,0);
        if (iVar4 < 100) {
          bVar2 = true;
        }
        iVar4 = FUN_004cde40((int)*(short *)(iVar6 + 0x5d19d4),iVar4);
        fVar9 = (float10)iVar4;
        break;
      case 0xe3:
        iVar4 = FUN_00417050(iVar4,0);
        if (iVar4 < 100) {
          bVar2 = true;
        }
        iVar4 = FUN_004cde40((int)*(short *)(iVar6 + 0x5d19d4),iVar4);
        fVar9 = (float10)iVar4;
        break;
      case 0xe4:
        iVar4 = FUN_004e6710(iVar4,0);
        if (iVar4 < 100) {
          bVar2 = true;
        }
        iVar4 = FUN_004cde40((int)*(short *)(iVar6 + 0x5d19d4),iVar4);
        fVar9 = (float10)iVar4;
        break;
      case 0xe5:
        iVar4 = FUN_00545740(iVar4,0);
        if (iVar4 < 100) {
          bVar2 = true;
        }
        iVar4 = FUN_004cde40((int)*(short *)(iVar6 + 0x5d19d4),iVar4);
        fVar9 = (float10)iVar4;
        break;
      case 0xe6:
        iVar4 = FUN_00401540(iVar4,0);
        if (iVar4 < 100) {
          bVar2 = true;
        }
        iVar4 = FUN_004cde40((int)*(short *)(iVar6 + 0x5d19d4),iVar4);
        fVar9 = (float10)iVar4;
        break;
      case 0xea:
        iVar4 = FUN_00558be0(iVar4,0);
        if (iVar4 < 100) {
          bVar2 = true;
        }
        iVar4 = FUN_004cde40((int)*(short *)(iVar6 + 0x5d19d4),iVar4);
        fVar9 = (float10)iVar4;
        break;
      case 0xeb:
        iVar4 = FUN_00555e80(iVar4,0);
        if (iVar4 < 100) {
          bVar2 = true;
        }
        iVar4 = FUN_004cde40((int)*(short *)(iVar6 + 0x5d19d4),iVar4);
        fVar9 = (float10)iVar4;
        break;
      case 0xec:
        iVar4 = FUN_0054f470(iVar4,0);
        if (iVar4 < 100) {
          bVar2 = true;
        }
        iVar4 = FUN_004cde40((int)*(short *)(iVar6 + 0x5d19d4),iVar4);
        fVar9 = (float10)iVar4;
      }
      fVar9 = fVar9 + (float10)dStack_80;
      dStack_80 = (double)fVar9;
switchD_004f79d2_caseD_b8:
      iStack_88 = iStack_88 + 1;
    } while (iStack_88 < 3);
    iVar6 = 0x784e78;
    iVar4 = -1;
    do {
      switch(iVar4) {
      case 0:
      case 7:
      case 9:
      case 0xc:
      case 0xe:
      case 0x10:
      case 0x11:
      case 0x12:
      case 0x13:
      case 0x16:
      case 0x17:
      case 0x18:
      case 0x19:
      case 0x1b:
      case 0x1d:
        if (*(int *)(iVar6 + -0x90) != 0) {
                    // WARNING: Could not recover jumptable at 0x004f7cbf. Too many branches
                    // WARNING: Treating indirect jump as call
          fVar9 = (float10)(**(code **)(&UNK_004f7e20 + (uint)(byte)(&UNK_004f7e28)[iVar4] * 4))();
          return fVar9;
        }
      }
      iVar6 = iVar6 + 4;
      iVar7 = iVar4 + 2;
      iVar4 = iVar4 + 1;
    } while (iVar7 < 0x24);
  }
  else {
    fVar9 = (float10)0.0;
    psVar5 = (short *)&DAT_00784de0;
    do {
      if (*psVar5 != 0) {
        fVar9 = (float10)(int)*(short *)(*psVar5 * 0x10 + 0x5d19d4) + fVar9;
      }
      psVar5 = psVar5 + 1;
    } while ((int)psVar5 < 0x784de6);
    piVar8 = (int *)0x784de8;
    iVar4 = -1;
    do {
      switch(iVar4) {
      case 0:
      case 7:
      case 9:
      case 0xc:
      case 0xe:
      case 0x10:
      case 0x11:
      case 0x12:
      case 0x13:
      case 0x16:
      case 0x17:
      case 0x18:
      case 0x19:
      case 0x1b:
      case 0x1d:
        if (*piVar8 != 0) {
                    // WARNING: Could not recover jumptable at 0x004f7935. Too many branches
                    // WARNING: Treating indirect jump as call
          fVar9 = (float10)(**(code **)(&UNK_004f7d60 + (uint)(byte)(&UNK_004f7d68)[iVar4] * 4))();
          return fVar9;
        }
      }
      piVar8 = piVar8 + 1;
      iVar6 = iVar4 + 2;
      iVar4 = iVar4 + 1;
    } while (iVar6 < 0x24);
  }
  fVar9 = SQRT(fVar9) * (float10)6.32 + (float10)0.5;
  if ((bVar2) && (fVar9 = fVar9 - (float10)1.0, fVar9 < (float10)0.0)) {
    fVar9 = (float10)0.0;
  }
  return fVar9;
}
