
Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  004CFD60: mov         eax,dword ptr [esp+4]
  004CFD64: cmp         eax,1
  004CFD67: jne         004CFD7E
  004CFD69: mov         dword ptr ds:[00E38DB0h],280h
  004CFD73: mov         dword ptr ds:[00E38DACh],1E0h
  004CFD7D: ret
  004CFD7E: cmp         eax,2
  004CFD81: jne         004CFD98
  004CFD83: mov         dword ptr ds:[00E38DB0h],320h
  004CFD8D: mov         dword ptr ds:[00E38DACh],258h
  004CFD97: ret
  004CFD98: cmp         eax,3
  004CFD9B: jne         004CFDB1
  004CFD9D: mov         dword ptr ds:[00E38DB0h],400h
  004CFDA7: mov         dword ptr ds:[00E38DACh],300h
  004CFDB1: ret
  004CFDB2: nop
  004CFDB3: nop
  004CFDB4: nop
  004CFDB5: nop
  004CFDB6: nop
  004CFDB7: nop
  004CFDB8: nop
  004CFDB9: nop
  004CFDBA: nop
  004CFDBB: nop
  004CFDBC: nop
  004CFDBD: nop
  004CFDBE: nop
  004CFDBF: nop
  004CFDC0: sub         esp,10h
  004CFDC3: push        esi
  004CFDC4: mov         esi,dword ptr [esp+18h]
  004CFDC8: push        esi
  004CFDC9: call        004CFD60
  004CFDCE: mov         dword ptr ds:[00E38E68h],esi
  004CFDD4: mov         esi,dword ptr ds:[00E38DB0h]
  004CFDDA: mov         ecx,dword ptr ds:[00E38DACh]
  004CFDE0: add         esp,4
  004CFDE3: lea         eax,[esi+FFFFFD80h]
  004CFDE9: mov         dword ptr ds:[00E38D9Ch],esi
  004CFDEF: cdq
  004CFDF0: sub         eax,edx
  004CFDF2: push        ecx
  004CFDF3: sar         eax,1
  004CFDF5: mov         dword ptr ds:[00E38DA8h],eax
  004CFDFA: lea         eax,[ecx+FFFFFE20h]
  004CFE00: cdq
  004CFE01: sub         eax,edx
  004CFE03: push        esi
  004CFE04: sar         eax,1
  004CFE06: mov         dword ptr ds:[00E38DA4h],eax
  004CFE0B: lea         eax,[esi+esi]
  004CFE0E: mov         dword ptr ds:[00E38D94h],eax
  004CFE13: imul        eax,ecx
  004CFE16: mov         dword ptr ds:[00E38D8Ch],eax
  004CFE1B: push        0
  004CFE1D: lea         eax,[esp+10h]
  004CFE21: push        0
  004CFE23: push        eax
  004CFE24: mov         dword ptr ds:[00E38D98h],ecx
  004CFE2A: mov         dword ptr ds:[00E38D90h],ecx
  004CFE30: mov         dword ptr ds:[00D3B55Ch],0
  004CFE3A: mov         dword ptr ds:[00D3B558h],esi
  004CFE40: mov         dword ptr ds:[00D3B554h],0
  004CFE4A: mov         dword ptr ds:[00D3B550h],ecx
  004CFE50: call        dword ptr ds:[0056F208h]
  004CFE56: mov         ecx,dword ptr ds:[00E39140h]
  004CFE5C: mov         esi,dword ptr ds:[0056F218h]
  004CFE62: push        0FFFFFFECh
  004CFE64: push        ecx
  004CFE65: call        esi
  004CFE67: mov         edx,dword ptr ds:[00E39140h]
  004CFE6D: push        eax
  004CFE6E: push        edx
  004CFE6F: call        dword ptr ds:[0056F17Ch]
  004CFE75: neg         eax
  004CFE77: sbb         eax,eax
  004CFE79: neg         eax
  004CFE7B: push        eax
  004CFE7C: mov         eax,dword ptr ds:[00E39140h]
  004CFE81: push        0FFFFFFF0h
  004CFE83: push        eax
  004CFE84: call        esi
  004CFE86: lea         ecx,[esp+0Ch]
  004CFE8A: push        eax
  004CFE8B: push        ecx
  004CFE8C: call        dword ptr ds:[0056F224h]
  004CFE92: mov         eax,dword ptr ds:[00E38DB8h]
  004CFE97: mov         edx,dword ptr ds:[00E38DB0h]
  004CFE9D: sub         eax,edx
  004CFE9F: mov         ecx,dword ptr [esp+8]
  004CFEA3: cdq
  004CFEA4: sub         eax,edx
  004CFEA6: mov         edx,dword ptr ds:[00E38DACh]
  004CFEAC: sar         eax,1
  004CFEAE: mov         dword ptr ds:[00E390E8h],eax
  004CFEB3: mov         eax,dword ptr ds:[00E38DB4h]
  004CFEB8: sub         eax,edx
  004CFEBA: mov         esi,dword ptr [esp+4]
  004CFEBE: cdq
  004CFEBF: sub         eax,edx
  004CFEC1: mov         edx,dword ptr [esp+10h]
  004CFEC5: sar         eax,1
  004CFEC7: mov         dword ptr ds:[00E390E4h],eax
  004CFECC: mov         eax,dword ptr [esp+0Ch]
  004CFED0: sub         edx,ecx
  004CFED2: push        16h
  004CFED4: sub         eax,esi
  004CFED6: push        edx
  004CFED7: push        eax
  004CFED8: mov         ecx,dword ptr ds:[00E39140h]
  004CFEDE: mov         esi,dword ptr ds:[0056F180h]
  004CFEE4: push        0
  004CFEE6: push        0
  004CFEE8: push        0
  004CFEEA: push        ecx
  004CFEEB: call        esi
  004CFEED: mov         edx,dword ptr ds:[00E39140h]
  004CFEF3: push        13h
  004CFEF5: push        0
  004CFEF7: push        0
  004CFEF9: push        0
  004CFEFB: push        0
  004CFEFD: push        0FFFFFFFEh
  004CFEFF: push        edx
  004CFF00: call        esi
  004CFF02: mov         eax,dword ptr ds:[00E39140h]
  004CFF07: push        eax
  004CFF08: call        dword ptr ds:[0056F1C8h]
  004CFF0E: mov         ecx,dword ptr ds:[00E39140h]
  004CFF14: push        ecx
  004CFF15: call        dword ptr ds:[0056F1C4h]
  004CFF1B: pop         esi
  004CFF1C: add         esp,10h
  004CFF1F: ret
  004CFF20: push        edi
  004CFF21: mov         ecx,8
  004CFF26: xor         eax,eax
  004CFF28: mov         edi,0E39108h
  004CFF2D: rep stos    dword ptr es:[edi]
  004CFF2F: push        0E39108h
  004CFF34: mov         dword ptr ds:[00E39108h],20h
  004CFF3E: call        dword ptr ds:[0056F02Ch]
  004CFF44: mov         eax,dword ptr ds:[00E39114h]
  004CFF49: pop         edi
  004CFF4A: ret

  Summary

      16E000 .text
