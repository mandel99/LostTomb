Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  004DEE10: mov         eax,dword ptr ds:[0070E45Ch]
  004DEE15: sub         esp,34h
  004DEE18: push        ebx
  004DEE19: push        ebp
  004DEE1A: mov         ebp,dword ptr ds:[0070E44Ch]
  004DEE20: push        esi
  004DEE21: push        edi
  004DEE22: lea         ebx,[eax+1]
  004DEE25: xor         edi,edi
  004DEE27: mov         esi,5E2AECh
  004DEE2C: push        0
  004DEE2E: push        0
  004DEE30: lea         ecx,[esp+18h]
  004DEE34: push        edi
  004DEE35: push        ecx
  004DEE36: call        004DED50
  004DEE3B: mov         eax,dword ptr [esp+24h]
  004DEE3F: push        2
  004DEE41: push        2
  004DEE43: push        edi
  004DEE44: lea         edx,[eax+eax*8]
  004DEE47: lea         ecx,[eax+edx*2]
  004DEE4A: lea         edx,[ecx+ecx*2]
  004DEE4D: mov         ecx,dword ptr [esp+2Ch]
  004DEE51: lea         edx,[ecx+edx*4]
  004DEE54: mov         dword ptr [esi-4],edx
  004DEE57: mov         edx,ecx
  004DEE59: sub         edx,eax
  004DEE5B: add         eax,ecx
  004DEE5D: imul        edx,ebx
  004DEE60: imul        eax,ebp
  004DEE63: mov         dword ptr [esi],edx
  004DEE65: mov         dword ptr [esi+10h],eax
  004DEE68: lea         eax,[esp+2Ch]
  004DEE6C: push        eax
  004DEE6D: call        004DED50
  004DEE72: mov         eax,dword ptr [esp+34h]
  004DEE76: mov         ecx,dword ptr [esp+30h]
  004DEE7A: push        4
  004DEE7C: push        4
  004DEE7E: lea         edx,[eax+ecx]
  004DEE81: sub         eax,ecx
  004DEE83: imul        edx,ebx
  004DEE86: imul        eax,ebp
  004DEE89: mov         dword ptr [esi+4],edx
  004DEE8C: mov         dword ptr [esi+14h],eax
  004DEE8F: lea         eax,[esp+38h]
  004DEE93: push        edi
  004DEE94: push        eax
  004DEE95: call        004DED50
  004DEE9A: mov         eax,dword ptr [esp+44h]
  004DEE9E: mov         ecx,dword ptr [esp+40h]
  004DEEA2: mov         edx,eax
  004DEEA4: add         eax,ecx
  004DEEA6: sub         edx,ecx
  004DEEA8: imul        eax,ebp
  004DEEAB: imul        edx,ebx
  004DEEAE: neg         eax
  004DEEB0: mov         dword ptr [esi+8],edx
  004DEEB3: mov         dword ptr [esi+18h],eax
  004DEEB6: push        6
  004DEEB8: push        6
  004DEEBA: lea         eax,[esp+48h]
  004DEEBE: push        edi
  004DEEBF: push        eax
  004DEEC0: call        004DED50
  004DEEC5: mov         ecx,dword ptr [esp+54h]
  004DEEC9: mov         eax,dword ptr [esp+50h]
  004DEECD: add         esp,40h
  004DEED0: add         esi,34h
  004DEED3: lea         edx,[ecx+eax]
  004DEED6: sub         eax,ecx
  004DEED8: imul        edx,ebx
  004DEEDB: imul        eax,ebp
  004DEEDE: neg         edx
  004DEEE0: mov         dword ptr [esi-28h],edx
  004DEEE3: mov         dword ptr [esi-18h],eax
  004DEEE6: inc         edi
  004DEEE7: cmp         esi,5E427Ch
  004DEEED: jl          004DEE2C
  004DEEF3: pop         edi
  004DEEF4: pop         esi
  004DEEF5: pop         ebp
  004DEEF6: pop         ebx
  004DEEF7: add         esp,34h
  004DEEFA: ret
  004DEEFB: nop
  004DEEFC: nop
  004DEEFD: nop
  004DEEFE: nop
  004DEEFF: nop
  004DEF00: sub         esp,4Ch
  004DEF03: mov         ecx,dword ptr [esp+50h]
  004DEF07: push        ebx
  004DEF08: push        ebp
  004DEF09: mov         ebp,dword ptr ds:[00BCA2D8h]
  004DEF0F: sub         ecx,ebp
  004DEF11: push        esi
  004DEF12: mov         eax,ecx
  004DEF14: mov         esi,0E4h
  004DEF19: cdq
  004DEF1A: idiv        eax,esi
  004DEF1C: mov         eax,8FB823EFh
  004DEF21: xor         ebp,ebp
  004DEF23: push        edi
  004DEF24: mov         dword ptr [esp+18h],edx
  004DEF28: imul        ecx
  004DEF2A: add         edx,ecx
  004DEF2C: sar         edx,7
  004DEF2F: mov         eax,edx
  004DEF31: shr         eax,1Fh
  004DEF34: add         edx,eax
  004DEF36: xor         esi,esi
  004DEF38: mov         dword ptr [esp+1Ch],edx
  004DEF3C: mov         dword ptr [esp+14h],esi
  004DEF40: jmp         004DEF44
  004DEF42: xor         ebp,ebp
  004DEF44: mov         eax,dword ptr ds:[00BCA2E0h]
  004DEF49: lea         ecx,[esp+28h]
  004DEF4D: push        eax
  004DEF4E: push        eax
  004DEF4F: push        esi
  004DEF50: push        ecx

  Summary

      16E000 .text
