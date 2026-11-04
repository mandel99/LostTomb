
Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  004CF960: mov         eax,dword ptr fs:[00000000h]
  004CF966: push        0FFFFFFFFh
  004CF968: push        56B6BBh
  004CF96D: push        eax
  004CF96E: mov         dword ptr fs:[00000000h],esp
  004CF975: sub         esp,28h
  004CF978: push        ebx
  004CF979: push        ebp
  004CF97A: push        esi
  004CF97B: push        edi
  004CF97C: push        64h
  004CF97E: push        0E36B94h
  004CF983: call        005623FC
  004CF988: mov         eax,dword ptr ds:[00E38E68h]
  004CF98D: add         esp,8
  004CF990: xor         ebx,ebx
  004CF992: cmp         eax,2
  004CF995: mov         dword ptr ds:[00D3B384h],54657687h
  004CF99F: mov         dword ptr ds:[00D3B380h],72641663h
  004CF9A9: mov         dword ptr ds:[00D3B37Ch],34518632h
  004CF9B3: mov         byte ptr ds:[00D3B494h],bl
  004CF9B9: mov         byte ptr ds:[00D3B495h],bl
  004CF9BF: mov         byte ptr ds:[00D3B496h],bl
  004CF9C5: mov         byte ptr ds:[00D3B497h],bl
  004CF9CB: mov         byte ptr ds:[00E38E4Ch],bl
  004CF9D1: mov         dword ptr ds:[00D3B518h],ebx
  004CF9D7: mov         dword ptr ds:[00D3B508h],ebx
  004CF9DD: mov         dword ptr ds:[00D3B510h],ebx
  004CF9E3: mov         dword ptr ds:[00E38E14h],ebx
  004CF9E9: mov         byte ptr ds:[00E38DA1h],1
  004CF9F0: jne         004CFA04
  004CF9F2: mov         esi,320h
  004CF9F7: mov         ecx,258h
  004CF9FC: mov         dword ptr ds:[00E38DB0h],esi
  004CFA02: jmp         004CFA2B
  004CFA04: cmp         eax,3
  004CFA07: jne         004CFA1B
  004CFA09: mov         esi,400h
  004CFA0E: mov         ecx,300h
  004CFA13: mov         dword ptr ds:[00E38DB0h],esi
  004CFA19: jmp         004CFA2B
  004CFA1B: mov         esi,280h
  004CFA20: mov         ecx,1E0h
  004CFA25: mov         dword ptr ds:[00E38DB0h],esi
  004CFA2B: lea         eax,[esi+FFFFFD80h]
  004CFA31: mov         ebp,dword ptr ds:[0056F1A0h]
  004CFA37: cdq
  004CFA38: sub         eax,edx
  004CFA3A: push        ebx
  004CFA3B: sar         eax,1
  004CFA3D: mov         dword ptr ds:[00E38DA8h],eax
  004CFA42: lea         eax,[ecx+FFFFFE20h]
  004CFA48: cdq
  004CFA49: sub         eax,edx
  004CFA4B: mov         dword ptr ds:[00E38DACh],ecx
  004CFA51: sar         eax,1
  004CFA53: mov         dword ptr ds:[00E38DA4h],eax
  004CFA58: lea         eax,[esi+esi]
  004CFA5B: mov         dword ptr ds:[00E38D94h],eax
  004CFA60: imul        eax,ecx
  004CFA63: mov         dword ptr ds:[00E38D9Ch],esi
  004CFA69: mov         dword ptr ds:[00E38D98h],ecx
  004CFA6F: mov         dword ptr ds:[00E38D90h],ecx
  004CFA75: mov         dword ptr ds:[00E38D8Ch],eax
  004CFA7A: mov         dword ptr ds:[00D3B55Ch],ebx
  004CFA80: mov         dword ptr ds:[00D3B558h],esi
  004CFA86: mov         dword ptr ds:[00D3B554h],ebx
  004CFA8C: mov         dword ptr ds:[00D3B550h],ecx
  004CFA92: call        ebp
  004CFA94: push        1
  004CFA96: mov         dword ptr ds:[00E38DB8h],eax
  004CFA9B: call        ebp
  004CFA9D: push        ebx
  004CFA9E: push        ebx
  004CFA9F: push        5E0730h
  004CFAA4: mov         dword ptr ds:[00E38DB4h],eax
  004CFAA9: call        004CE6E0
  004CFAAE: add         esp,0Ch
  004CFAB1: push        ebx
  004CFAB2: call        dword ptr ds:[0056F310h]
  004CFAB8: push        ebx
  004CFAB9: push        9
  004CFABB: call        004CC110
  004CFAC0: mov         eax,dword ptr ds:[00E20768h]
  004CFAC5: add         esp,8
  004CFAC8: push        eax
  004CFAC9: push        5E0724h
  004CFACE: call        dword ptr ds:[0056F1A8h]
  004CFAD4: mov         esi,eax
  004CFAD6: cmp         esi,ebx
  004CFAD8: je          004CFB25
  004CFADA: push        esi
  004CFADB: call        dword ptr ds:[0056F204h]
  004CFAE1: test        eax,eax
  004CFAE3: je          004CFAEC
  004CFAE5: push        esi
  004CFAE6: call        dword ptr ds:[0056F1ACh]
  004CFAEC: push        esi
  004CFAED: call        dword ptr ds:[0056F1F4h]
  004CFAF3: mov         edi,dword ptr ds:[0056F1A4h]
  004CFAF9: push        ebx
  004CFAFA: push        ebx
  004CFAFB: push        7
  004CFAFD: push        esi
  004CFAFE: call        edi
  004CFB00: push        ebx
  004CFB01: push        1
  004CFB03: push        18h
  004CFB05: push        esi
  004CFB06: call        edi
  004CFB08: push        ebx
  004CFB09: push        1
  004CFB0B: push        1Ch
  004CFB0D: push        esi
  004CFB0E: call        edi
  004CFB10: pop         edi
  004CFB11: pop         esi
  004CFB12: pop         ebp
  004CFB13: xor         eax,eax
  004CFB15: pop         ebx
  004CFB16: mov         ecx,dword ptr [esp+28h]
  004CFB1A: mov         dword ptr fs:[00000000h],ecx
  004CFB21: add         esp,34h
  004CFB24: ret
  004CFB25: mov         esi,dword ptr [esp+48h]
  004CFB29: push        8Dh
  004CFB2E: push        esi
  004CFB2F: mov         dword ptr [esp+18h],ebx
  004CFB33: mov         dword ptr [esp+1Ch],414CD0h
  004CFB3B: mov         dword ptr [esp+20h],ebx
  004CFB3F: mov         dword ptr [esp+24h],ebx
  004CFB43: mov         dword ptr [esp+28h],esi
  004CFB47: call        dword ptr ds:[0056F1B4h]
  004CFB4D: push        4
  004CFB4F: mov         dword ptr [esp+28h],eax
  004CFB53: mov         dword ptr [esp+2Ch],ebx
  004CFB57: call        dword ptr ds:[0056F01Ch]
  004CFB5D: lea         ecx,[esp+10h]
  004CFB61: mov         dword ptr [esp+2Ch],eax
  004CFB65: push        ecx
  004CFB66: mov         dword ptr [esp+34h],ebx
  004CFB6A: mov         dword ptr [esp+38h],5E0724h
  004CFB72: call        dword ptr ds:[0056F1B8h]
  004CFB78: and         eax,0FFFFh
  004CFB7D: mov         dword ptr [esp+48h],eax
  004CFB81: jne         004CFB98
  004CFB83: pop         edi
  004CFB84: pop         esi
  004CFB85: pop         ebp
  004CFB86: xor         eax,eax
  004CFB88: pop         ebx
  004CFB89: mov         ecx,dword ptr [esp+28h]
  004CFB8D: mov         dword ptr fs:[00000000h],ecx
  004CFB94: add         esp,34h
  004CFB97: ret
  004CFB98: mov         edi,dword ptr ds:[0056F1B0h]
  004CFB9E: push        66h
  004CFBA0: push        esi
  004CFBA1: call        edi
  004CFBA3: push        88h
  004CFBA8: push        esi
  004CFBA9: mov         dword ptr ds:[00E390D8h],eax
  004CFBAE: call        edi
  004CFBB0: push        82h
  004CFBB5: push        esi
  004CFBB6: mov         dword ptr ds:[00E390D4h],eax
  004CFBBB: call        edi
  004CFBBD: push        93h
  004CFBC2: push        esi
  004CFBC3: mov         dword ptr ds:[00E390D0h],eax
  004CFBC8: call        edi
  004CFBCA: push        90h
  004CFBCF: push        esi
  004CFBD0: mov         dword ptr ds:[00E390BCh],eax
  004CFBD5: call        edi
  004CFBD7: push        91h
  004CFBDC: push        esi
  004CFBDD: mov         dword ptr ds:[00E390C4h],eax
  004CFBE2: call        edi
  004CFBE4: push        92h
  004CFBE9: push        esi
  004CFBEA: mov         dword ptr ds:[00E390C8h],eax
  004CFBEF: call        edi
  004CFBF1: push        8Eh
  004CFBF6: push        esi
  004CFBF7: mov         dword ptr ds:[00E39094h],eax
  004CFBFC: call        edi
  004CFBFE: push        96h
  004CFC03: push        esi
  004CFC04: mov         dword ptr ds:[00E390CCh],eax
  004CFC09: call        edi
  004CFC0B: push        95h
  004CFC10: push        esi
  004CFC11: mov         dword ptr ds:[00E390B8h],eax
  004CFC16: call        edi
  004CFC18: push        94h
  004CFC1D: push        esi
  004CFC1E: mov         dword ptr ds:[00E390B0h],eax
  004CFC23: call        edi
  004CFC25: push        99h
  004CFC2A: push        esi
  004CFC2B: mov         dword ptr ds:[00E390B4h],eax
  004CFC30: call        edi
  004CFC32: push        98h
  004CFC37: push        esi
  004CFC38: mov         dword ptr ds:[00E390ACh],eax
  004CFC3D: call        edi
  004CFC3F: push        9Bh
  004CFC44: push        esi
  004CFC45: mov         dword ptr ds:[00E390A8h],eax
  004CFC4A: call        edi
  004CFC4C: push        9Ah
  004CFC51: push        esi
  004CFC52: mov         dword ptr ds:[00E390A4h],eax
  004CFC57: call        edi
  004CFC59: push        9Ch
  004CFC5E: push        esi
  004CFC5F: mov         dword ptr ds:[00E390A0h],eax
  004CFC64: call        edi
  004CFC66: push        9Dh
  004CFC6B: push        esi
  004CFC6C: mov         dword ptr ds:[00E3909Ch],eax
  004CFC71: call        edi
  004CFC73: push        9Fh
  004CFC78: push        esi
  004CFC79: mov         dword ptr ds:[00E39098h],eax
  004CFC7E: call        edi
  004CFC80: push        0A0h
  004CFC85: push        esi
  004CFC86: mov         dword ptr ds:[00E390C0h],eax
  004CFC8B: call        edi
  004CFC8D: push        ebx
  004CFC8E: push        esi
  004CFC8F: mov         dword ptr ds:[00E39090h],eax
  004CFC94: mov         dword ptr ds:[00E39144h],esi
  004CFC9A: push        ebx
  004CFC9B: push        ebx
  004CFC9C: push        1
  004CFC9E: call        ebp
  004CFCA0: push        eax
  004CFCA1: push        ebx
  004CFCA2: call        ebp
  004CFCA4: mov         edx,dword ptr ds:[00E20768h]
  004CFCAA: push        eax
  004CFCAB: push        ebx
  004CFCAC: push        ebx
  004CFCAD: push        90080000h
  004CFCB2: push        edx
  004CFCB3: push        5E0724h
  004CFCB8: push        40000h
  004CFCBD: call        dword ptr ds:[0056F1C0h]
  004CFCC3: cmp         eax,ebx
  004CFCC5: mov         dword ptr ds:[00E39140h],eax
  004CFCCA: jne         004CFCE1
  004CFCCC: pop         edi
  004CFCCD: pop         esi
  004CFCCE: pop         ebp
  004CFCCF: xor         eax,eax
  004CFCD1: pop         ebx
  004CFCD2: mov         ecx,dword ptr [esp+28h]
  004CFCD6: mov         dword ptr fs:[00000000h],ecx
  004CFCDD: add         esp,34h
  004CFCE0: ret
  004CFCE1: push        eax
  004CFCE2: call        dword ptr ds:[0056F1C8h]
  004CFCE8: mov         eax,dword ptr ds:[00E39140h]
  004CFCED: push        eax
  004CFCEE: call        dword ptr ds:[0056F1C4h]
  004CFCF4: push        1Ch
  004CFCF6: mov         dword ptr ds:[00E39178h],ebx
  004CFCFC: mov         dword ptr ds:[00E39174h],ebx
  004CFD02: mov         dword ptr ds:[00E39148h],ebx
  004CFD08: mov         dword ptr ds:[00E39170h],ebx
  004CFD0E: call        00561A1D
  004CFD13: add         esp,4
  004CFD16: mov         dword ptr [esp+48h],eax
  004CFD1A: cmp         eax,ebx
  004CFD1C: mov         dword ptr [esp+40h],ebx
  004CFD20: je          004CFD32
  004CFD22: mov         ecx,dword ptr ds:[00E39140h]
  004CFD28: push        ecx
  004CFD29: mov         ecx,eax
  004CFD2B: call        004133D0
  004CFD30: jmp         004CFD34
  004CFD32: xor         eax,eax
  004CFD34: mov         dword ptr [esp+40h],0FFFFFFFFh
  004CFD3C: mov         dword ptr ds:[006FDC54h],eax
  004CFD41: call        004D0220
  004CFD46: mov         ecx,dword ptr [esp+38h]
  004CFD4A: pop         edi
  004CFD4B: pop         esi
  004CFD4C: pop         ebp
  004CFD4D: pop         ebx
  004CFD4E: mov         dword ptr fs:[00000000h],ecx
  004CFD55: add         esp,34h
  004CFD58: ret
  004CFD59: nop
  004CFD5A: nop
  004CFD5B: nop
  004CFD5C: nop
  004CFD5D: nop
  004CFD5E: nop
  004CFD5F: nop
  004CFD60: mov         eax,dword ptr [esp+4]
  004CFD64: cmp         eax,1
  004CFD67: jne         004CFD7E
  004CFD69: mov         dword ptr ds:[00E38DB0h],280h
  004CFD73: mov         dword ptr ds:[00E38DACh],1E0h
  004CFD7D: ret
  004CFD7E: cmp         eax,2

  Summary

      16E000 .text
