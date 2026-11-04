
Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  00426B40: sub         esp,24h
  00426B43: push        ebx
  00426B44: push        esi
  00426B45: push        edi
  00426B46: push        0
  00426B48: call        dword ptr ds:[0056F1D4h]
  00426B4E: mov         edi,dword ptr ds:[0056F010h]
  00426B54: mov         esi,eax
  00426B56: push        18h
  00426B58: push        esi
  00426B59: call        edi
  00426B5B: push        0Eh
  00426B5D: push        esi
  00426B5E: call        edi
  00426B60: push        0Ch
  00426B62: push        esi
  00426B63: mov         ebx,eax
  00426B65: call        edi
  00426B67: imul        ebx,eax
  00426B6A: push        esi
  00426B6B: push        0
  00426B6D: mov         dword ptr ds:[00E38E58h],ebx
  00426B73: call        dword ptr ds:[0056F1D8h]
  00426B79: mov         esi,dword ptr ds:[0056F1A0h]
  00426B7F: push        0
  00426B81: call        esi
  00426B83: push        1
  00426B85: mov         dword ptr ds:[00E38DB8h],eax
  00426B8A: call        esi
  00426B8C: mov         ecx,dword ptr ds:[00E38DB8h]
  00426B92: mov         dword ptr ds:[00E38DB4h],eax
  00426B97: mov         eax,dword ptr ds:[00E38DB0h]
  00426B9C: cmp         eax,ecx
  00426B9E: mov         eax,dword ptr ds:[00E38E5Ch]
  00426BA3: jle         00426BB9
  00426BA5: test        eax,eax
  00426BA7: jne         00426BE2
  00426BA9: push        0
  00426BAB: push        0
  00426BAD: push        39h
  00426BAF: call        004E2970
  00426BB4: add         esp,0Ch
  00426BB7: jmp         00426BE2
  00426BB9: test        eax,eax
  00426BBB: jne         00426BEC
  00426BBD: cmp         dword ptr ds:[00E38E58h],10h
  00426BC4: je          00426C00
  00426BC6: push        0
  00426BC8: push        0
  00426BCA: push        39h
  00426BCC: call        004E2970
  00426BD1: push        0
  00426BD3: push        0
  00426BD5: push        5D1CD4h
  00426BDA: call        004CE6E0
  00426BDF: add         esp,18h
  00426BE2: mov         dword ptr ds:[00E38E5Ch],1
  00426BEC: push        70D508h
  00426BF1: push        100h
  00426BF6: push        0
  00426BF8: push        0
  00426BFA: call        dword ptr ds:[0056F00Ch]
  00426C00: mov         ecx,dword ptr ds:[00E39148h]
  00426C06: test        ecx,ecx
  00426C08: jne         00426C34
  00426C0A: push        ecx
  00426C0B: lea         ecx,[esp+10h]
  00426C0F: push        ecx
  00426C10: push        0
  00426C12: call        0055C0E0
  00426C17: test        eax,eax
  00426C19: je          00426C3C
  00426C1B: push        eax
  00426C1C: push        0
  00426C1E: push        5D1CB8h
  00426C23: call        004CE6E0
  00426C28: add         esp,0Ch
  00426C2B: xor         eax,eax
  00426C2D: pop         edi
  00426C2E: pop         esi
  00426C2F: pop         ebx
  00426C30: add         esp,24h
  00426C33: ret
  00426C34: mov         eax,ecx
  00426C36: mov         dword ptr [esp+0Ch],eax
  00426C3A: jmp         00426C46
  00426C3C: mov         eax,dword ptr [esp+0Ch]
  00426C40: mov         ecx,dword ptr ds:[00E39148h]
  00426C46: test        ecx,ecx
  00426C48: jne         00426C95
  00426C4A: mov         ecx,dword ptr ds:[00E38E5Ch]
  00426C50: mov         edx,dword ptr [eax]
  00426C52: test        ecx,ecx
  00426C54: je          00426C5A
  00426C56: push        13h
  00426C58: jmp         00426C5C
  00426C5A: push        8
  00426C5C: mov         ecx,dword ptr ds:[00E39140h]
  00426C62: push        ecx
  00426C63: push        eax
  00426C64: call        dword ptr [edx+50h]
  00426C67: test        eax,eax
  00426C69: je          00426C84
  00426C6B: push        eax
  00426C6C: push        0
  00426C6E: push        5D1C94h
  00426C73: call        004CE6E0
  00426C78: add         esp,0Ch
  00426C7B: xor         eax,eax
  00426C7D: pop         edi
  00426C7E: pop         esi
  00426C7F: pop         ebx
  00426C80: add         esp,24h
  00426C83: ret
  00426C84: push        0
  00426C86: push        0
  00426C88: push        5D1C74h
  00426C8D: call        004CE6E0
  00426C92: add         esp,0Ch
  00426C95: mov         eax,dword ptr ds:[00E38E5Ch]
  00426C9A: push        0FFFFFFF0h
  00426C9C: test        eax,eax
  00426C9E: je          00426D73
  00426CA4: mov         edx,dword ptr ds:[00E39140h]
  00426CAA: push        edx
  00426CAB: call        dword ptr ds:[0056F218h]
  00426CB1: and         eax,0FF3BFFFFh
  00426CB6: or          eax,90000000h
  00426CBB: push        eax
  00426CBC: mov         eax,dword ptr ds:[00E39140h]
  00426CC1: push        0FFFFFFF0h
  00426CC3: push        eax
  00426CC4: call        dword ptr ds:[0056F1D0h]
  00426CCA: mov         edx,dword ptr ds:[00E38D98h]
  00426CD0: mov         eax,dword ptr [esp+0Ch]
  00426CD4: push        10h
  00426CD6: push        edx
  00426CD7: mov         edx,dword ptr ds:[00E38D9Ch]
  00426CDD: mov         ecx,dword ptr [eax]
  00426CDF: push        edx
  00426CE0: push        eax
  00426CE1: call        dword ptr [ecx+54h]
  00426CE4: test        eax,eax
  00426CE6: je          00426D65
  00426CE8: push        2
  00426CEA: call        004CFDC0
  00426CEF: add         esp,4
  00426CF2: call        00414BD0
  00426CF7: call        004E2AB0
  00426CFC: mov         edx,dword ptr ds:[00E38D98h]
  00426D02: mov         eax,dword ptr [esp+0Ch]
  00426D06: push        10h
  00426D08: push        edx
  00426D09: mov         edx,dword ptr ds:[00E38D9Ch]
  00426D0F: mov         ecx,dword ptr [eax]
  00426D11: push        edx
  00426D12: push        eax
  00426D13: call        dword ptr [ecx+54h]
  00426D16: test        eax,eax
  00426D18: je          00426D65
  00426D1A: push        1
  00426D1C: call        004CFDC0
  00426D21: add         esp,4
  00426D24: call        00414BD0
  00426D29: call        004E2AB0
  00426D2E: mov         edx,dword ptr ds:[00E38D98h]
  00426D34: mov         eax,dword ptr [esp+0Ch]
  00426D38: push        10h
  00426D3A: push        edx
  00426D3B: mov         edx,dword ptr ds:[00E38D9Ch]
  00426D41: mov         ecx,dword ptr [eax]
  00426D43: push        edx
  00426D44: push        eax
  00426D45: call        dword ptr [ecx+54h]
  00426D48: test        eax,eax
  00426D4A: je          00426D65
  00426D4C: push        eax
  00426D4D: push        0
  00426D4F: push        5D1C58h
  00426D54: call        004CE6E0
  00426D59: add         esp,0Ch
  00426D5C: xor         eax,eax
  00426D5E: pop         edi
  00426D5F: pop         esi
  00426D60: pop         ebx
  00426D61: add         esp,24h
  00426D64: ret
  00426D65: push        0
  00426D67: push        0
  00426D69: push        5D1C40h
  00426D6E: jmp         00426EBE
  00426D73: mov         eax,dword ptr ds:[00E39140h]
  00426D78: mov         esi,dword ptr ds:[0056F218h]
  00426D7E: push        eax
  00426D7F: call        esi
  00426D81: mov         ecx,dword ptr ds:[00E39140h]
  00426D87: and         eax,7F30FFFFh
  00426D8C: or          eax,0CF0000h
  00426D91: push        eax
  00426D92: push        0FFFFFFF0h
  00426D94: push        ecx
  00426D95: call        dword ptr ds:[0056F1D0h]
  00426D9B: mov         edx,dword ptr ds:[00E38D98h]
  00426DA1: mov         eax,dword ptr ds:[00E38D9Ch]
  00426DA6: push        edx
  00426DA7: push        eax
  00426DA8: push        0
  00426DAA: lea         ecx,[esp+1Ch]
  00426DAE: push        0
  00426DB0: push        ecx
  00426DB1: call        dword ptr ds:[0056F208h]
  00426DB7: mov         edx,dword ptr ds:[00E39140h]
  00426DBD: push        0FFFFFFECh
  00426DBF: push        edx
  00426DC0: call        esi
  00426DC2: push        eax
  00426DC3: mov         eax,dword ptr ds:[00E39140h]
  00426DC8: push        eax
  00426DC9: call        dword ptr ds:[0056F17Ch]
  00426DCF: mov         ecx,dword ptr ds:[00E39140h]
  00426DD5: neg         eax
  00426DD7: sbb         eax,eax
  00426DD9: neg         eax
  00426DDB: push        eax
  00426DDC: push        0FFFFFFF0h
  00426DDE: push        ecx
  00426DDF: call        esi
  00426DE1: lea         edx,[esp+18h]
  00426DE5: push        eax
  00426DE6: push        edx
  00426DE7: call        dword ptr ds:[0056F224h]
  00426DED: mov         eax,dword ptr ds:[00E38DB8h]
  00426DF2: mov         ebx,dword ptr ds:[00E38DB0h]
  00426DF8: sub         eax,ebx
  00426DFA: mov         ecx,dword ptr [esp+18h]
  00426DFE: cdq
  00426DFF: sub         eax,edx
  00426E01: mov         edx,dword ptr ds:[00E38DACh]
  00426E07: sar         eax,1
  00426E09: mov         dword ptr ds:[00E390E8h],eax
  00426E0E: mov         edi,eax
  00426E10: mov         eax,dword ptr ds:[00E38DB4h]
  00426E15: push        16h
  00426E17: sub         eax,edx
  00426E19: mov         esi,dword ptr ds:[0056F180h]
  00426E1F: cdq
  00426E20: sub         eax,edx
  00426E22: mov         edx,dword ptr [esp+18h]
  00426E26: sar         eax,1
  00426E28: mov         dword ptr ds:[00E390E4h],eax
  00426E2D: mov         ebx,eax
  00426E2F: mov         eax,dword ptr [esp+20h]
  00426E33: sub         eax,edx
  00426E35: mov         edx,dword ptr ds:[00E39140h]
  00426E3B: push        eax
  00426E3C: mov         eax,dword ptr [esp+18h]
  00426E40: sub         ecx,eax
  00426E42: push        ecx
  00426E43: push        0
  00426E45: push        0
  00426E47: push        0
  00426E49: push        edx
  00426E4A: call        esi
  00426E4C: mov         eax,dword ptr ds:[00E39140h]
  00426E51: push        13h
  00426E53: push        0
  00426E55: push        0
  00426E57: push        0
  00426E59: push        0
  00426E5B: push        0FFFFFFFEh
  00426E5D: push        eax
  00426E5E: call        esi
  00426E60: lea         ecx,[esp+20h]
  00426E64: push        0
  00426E66: push        ecx
  00426E67: push        0
  00426E69: push        30h
  00426E6B: call        dword ptr ds:[0056F1E0h]
  00426E71: mov         eax,dword ptr ds:[00E39140h]
  00426E76: lea         edx,[esp+10h]
  00426E7A: push        edx
  00426E7B: push        eax
  00426E7C: call        dword ptr ds:[0056F1E8h]
  00426E82: mov         eax,dword ptr [esp+20h]
  00426E86: mov         ecx,dword ptr [esp+10h]
  00426E8A: cmp         ecx,eax
  00426E8C: jge         00426E92
  00426E8E: mov         dword ptr [esp+10h],eax
  00426E92: mov         eax,dword ptr [esp+24h]
  00426E96: mov         ecx,dword ptr [esp+14h]
  00426E9A: cmp         ecx,eax
  00426E9C: jge         00426EA2
  00426E9E: mov         dword ptr [esp+14h],eax
  00426EA2: mov         ecx,dword ptr ds:[00E39140h]
  00426EA8: push        15h
  00426EAA: push        0
  00426EAC: push        0
  00426EAE: push        ebx
  00426EAF: push        edi
  00426EB0: push        0
  00426EB2: push        ecx
  00426EB3: call        esi
  00426EB5: push        0
  00426EB7: push        0
  00426EB9: push        5D1C28h
  00426EBE: call        004CE6E0
  00426EC3: mov         edx,dword ptr [esp+18h]
  00426EC7: add         esp,0Ch
  00426ECA: mov         dword ptr ds:[00E39148h],edx
  00426ED0: mov         eax,1
  00426ED5: pop         edi
  00426ED6: pop         esi
  00426ED7: pop         ebx
  00426ED8: add         esp,24h
  00426EDB: ret
  00426EDC: nop
  00426EDD: nop
  00426EDE: nop
  00426EDF: nop
  00426EE0: mov         eax,dword ptr ds:[00E39170h]
  00426EE5: push        esi
  00426EE6: xor         esi,esi
  00426EE8: cmp         eax,esi
  00426EEA: je          00426EF8
  00426EEC: mov         ecx,dword ptr [eax]
  00426EEE: push        eax
  00426EEF: call        dword ptr [ecx+8]
  00426EF2: mov         dword ptr ds:[00E39170h],esi
  00426EF8: mov         eax,dword ptr ds:[00E39174h]
  00426EFD: cmp         eax,esi
  00426EFF: je          00426F0D
  00426F01: mov         edx,dword ptr [eax]
  00426F03: push        eax
  00426F04: call        dword ptr [edx+8]
  00426F07: mov         dword ptr ds:[00E39174h],esi
  00426F0D: mov         eax,dword ptr ds:[00E39178h]
  00426F12: cmp         eax,esi
  00426F14: je          00426F22
  00426F16: mov         ecx,dword ptr [eax]
  00426F18: push        eax
  00426F19: call        dword ptr [ecx+8]
  00426F1C: mov         dword ptr ds:[00E39178h],esi
  00426F22: mov         eax,dword ptr [esp+8]
  00426F26: mov         dword ptr ds:[00E92E18h],esi
  00426F2C: cmp         eax,esi
  00426F2E: je          00426F54
  00426F30: mov         eax,dword ptr ds:[00E39148h]
  00426F35: cmp         eax,esi
  00426F37: je          00426F54
  00426F39: mov         edx,dword ptr [eax]
  00426F3B: push        eax
  00426F3C: call        dword ptr [edx+8]
  00426F3F: push        esi
  00426F40: push        esi
  00426F41: push        5D1D00h
  00426F46: mov         dword ptr ds:[00E39148h],esi
  00426F4C: call        004CE6E0
  00426F51: add         esp,0Ch
  00426F54: mov         eax,1
  00426F59: pop         esi
  00426F5A: ret
  00426F5B: nop
  00426F5C: nop
  00426F5D: nop
  00426F5E: nop
  00426F5F: nop
  00426F60: sub         esp,64h
  00426F63: mov         eax,dword ptr ds:[00E39140h]
  00426F68: push        eax
  00426F69: call        dword ptr ds:[0056F1C8h]
  00426F6F: mov         eax,dword ptr ds:[00E39174h]
  00426F74: lea         edx,[esp]
  00426F78: push        edx
  00426F79: push        1000400h
  00426F7E: push        0
  00426F80: mov         dword ptr [esp+0Ch],64h
  00426F88: mov         dword ptr [esp+5Ch],0
  00426F90: mov         ecx,dword ptr [eax]
  00426F92: push        0
  00426F94: push        0
  00426F96: push        eax
  00426F97: call        dword ptr [ecx+14h]
  00426F9A: test        eax,eax
  00426F9C: je          00426FA2
  00426F9E: push        1
  00426FA0: jmp         00426FC3
  00426FA2: mov         eax,dword ptr ds:[00E39178h]
  00426FA7: lea         edx,[esp]
  00426FAB: push        edx
  00426FAC: push        1000400h
  00426FB1: mov         ecx,dword ptr [eax]
  00426FB3: push        0
  00426FB5: push        0
  00426FB7: push        0
  00426FB9: push        eax
  00426FBA: call        dword ptr [ecx+14h]
  00426FBD: test        eax,eax
  00426FBF: je          00426FD8
  00426FC1: push        2
  00426FC3: push        0
  00426FC5: push        5D1D14h
  00426FCA: call        004CE6E0
  00426FCF: add         esp,0Ch
  00426FD2: xor         eax,eax
  00426FD4: add         esp,64h
  00426FD7: ret
  00426FD8: mov         eax,1
  00426FDD: add         esp,64h
  00426FE0: ret
  00426FE1: nop
  00426FE2: nop
  00426FE3: nop
  00426FE4: nop
  00426FE5: nop
  00426FE6: nop
  00426FE7: nop
  00426FE8: nop
  00426FE9: nop
  00426FEA: nop
  00426FEB: nop
  00426FEC: nop
  00426FED: nop
  00426FEE: nop
  00426FEF: nop
  00426FF0: sub         esp,0D4h
  00426FF6: push        ebx
  00426FF7: push        esi
  00426FF8: push        edi
  00426FF9: mov         ecx,1Bh
  00426FFE: xor         eax,eax
  00427000: lea         edi,[esp+10h]
  00427004: rep stos    dword ptr es:[edi]
  00427006: mov         eax,dword ptr ds:[00E92DFCh]
  0042700B: xor         ebx,ebx
  0042700D: mov         dword ptr [esp+0Ch],eax
  00427011: mov         eax,dword ptr ds:[00E38E5Ch]
  00427016: mov         esi,6Ch
  0042701B: cmp         eax,ebx
  0042701D: push        ebx
  0042701E: mov         dword ptr [esp+14h],esi
  00427022: push        ebx
  00427023: je          00427142
  00427029: push        5D1FBCh
  0042702E: call        004CE6E0
  00427033: mov         eax,dword ptr ds:[00E39148h]
  00427038: add         esp,0Ch
  0042703B: lea         edx,[esp+10h]
  0042703F: mov         dword ptr [esp+14h],1
  00427047: push        ebx
  00427048: mov         dword ptr [esp+7Ch],200h
  00427050: mov         ecx,dword ptr [eax]
  00427052: push        0E39178h
  00427057: push        edx
  00427058: push        eax
  00427059: call        dword ptr [ecx+18h]
  0042705C: cmp         eax,ebx
  0042705E: je          0042707B
  00427060: push        eax
  00427061: push        ebx
  00427062: push        5D1F9Ch
  00427067: call        004CE6E0
  0042706C: add         esp,0Ch
  0042706F: xor         eax,eax
  00427071: pop         edi
  00427072: pop         esi
  00427073: pop         ebx
  00427074: add         esp,0D4h
  0042707A: ret
  0042707B: mov         ecx,1Bh
  00427080: xor         eax,eax
  00427082: lea         edi,[esp+10h]
  00427086: push        ebx
  00427087: rep stos    dword ptr es:[edi]
  00427089: mov         eax,dword ptr ds:[00E39178h]
  0042708E: lea         edx,[esp+14h]
  00427092: push        1
  00427094: mov         dword ptr ds:[00E92E04h],ebx
  0042709A: mov         dword ptr [esp+18h],esi
  0042709E: mov         ecx,dword ptr [eax]
  004270A0: push        edx
  004270A1: push        ebx
  004270A2: push        eax
  004270A3: call        dword ptr [ecx+64h]
  004270A6: cmp         eax,ebx
  004270A8: jne         004270D0
  004270AA: push        ebx
  004270AB: push        ebx
  004270AC: push        5D1F78h
  004270B1: call        004CE6E0
  004270B6: mov         eax,dword ptr ds:[00E39178h]
  004270BB: add         esp,0Ch
  004270BE: mov         dword ptr ds:[00E92E04h],ebx
  004270C4: mov         ecx,dword ptr [eax]
  004270C6: push        ebx
  004270C7: push        eax
  004270C8: call        dword ptr [ecx+00000080h]
  004270CE: jmp         004270E6
  004270D0: push        eax
  004270D1: call        00427510
  004270D6: inc         eax
  004270D7: push        eax
  004270D8: push        ebx
  004270D9: push        5D1F5Ch
  004270DE: call        004CE6E0
  004270E3: add         esp,10h
  004270E6: mov         edx,dword ptr ds:[00E38D98h]
  004270EC: mov         eax,dword ptr ds:[00E38D9Ch]
  004270F1: push        ebx
  004270F2: push        1
  004270F4: push        edx
  004270F5: push        eax
  004270F6: call        00427480
  004270FB: add         esp,10h
  004270FE: cmp         eax,ebx
  00427100: mov         dword ptr ds:[00E39174h],eax
  00427105: je          00427220
  0042710B: mov         ecx,dword ptr [eax]
  0042710D: push        ebx
  0042710E: lea         edx,[esp+14h]
  00427112: push        1
  00427114: push        edx
  00427115: push        ebx
  00427116: push        eax
  00427117: call        dword ptr [ecx+64h]
  0042711A: test        eax,eax
  0042711C: jne         00427136
  0042711E: mov         eax,dword ptr [esp+34h]
  00427122: push        ebx
  00427123: mov         dword ptr ds:[00E92E18h],eax
  00427128: mov         eax,dword ptr ds:[00E39174h]
  0042712D: push        eax
  0042712E: mov         ecx,dword ptr [eax]
  00427130: call        dword ptr [ecx+00000080h]
  00427136: push        ebx
  00427137: push        ebx
  00427138: push        5D1F44h
  0042713D: jmp         00427300
  00427142: push        5D1F10h
  00427147: call        004CE6E0
  0042714C: mov         eax,dword ptr ds:[00E39148h]
  00427151: add         esp,0Ch
  00427154: lea         ecx,[esp+10h]
  00427158: mov         dword ptr [esp+14h],1
  00427160: push        ebx
  00427161: mov         dword ptr [esp+7Ch],200h
  00427169: mov         edx,dword ptr [eax]
  0042716B: push        0E39178h
  00427170: push        ecx
  00427171: push        eax
  00427172: call        dword ptr [edx+18h]
  00427175: cmp         eax,ebx
  00427177: je          00427194
  00427179: push        eax
  0042717A: push        ebx
  0042717B: push        5D1EF0h
  00427180: call        004CE6E0
  00427185: add         esp,0Ch
  00427188: xor         eax,eax
  0042718A: pop         edi
  0042718B: pop         esi
  0042718C: pop         ebx
  0042718D: add         esp,0D4h
  00427193: ret
  00427194: mov         ecx,1Bh
  00427199: xor         eax,eax
  0042719B: lea         edi,[esp+10h]
  0042719F: push        ebx
  004271A0: rep stos    dword ptr es:[edi]
  004271A2: mov         eax,dword ptr ds:[00E39178h]
  004271A7: lea         ecx,[esp+14h]
  004271AB: push        1
  004271AD: mov         dword ptr ds:[00E92E04h],ebx
  004271B3: mov         dword ptr [esp+18h],esi
  004271B7: mov         edx,dword ptr [eax]
  004271B9: push        ecx
  004271BA: push        ebx
  004271BB: push        eax
  004271BC: call        dword ptr [edx+64h]
  004271BF: cmp         eax,ebx
  004271C1: jne         004271E9
  004271C3: push        ebx
  004271C4: push        ebx
  004271C5: push        5D1F78h
  004271CA: call        004CE6E0
  004271CF: mov         eax,dword ptr ds:[00E39178h]
  004271D4: add         esp,0Ch
  004271D7: mov         dword ptr ds:[00E92E04h],ebx
  004271DD: mov         edx,dword ptr [eax]
  004271DF: push        ebx
  004271E0: push        eax
  004271E1: call        dword ptr [edx+00000080h]
  004271E7: jmp         004271FF
  004271E9: push        eax
  004271EA: call        00427510
  004271EF: inc         eax
  004271F0: push        eax
  004271F1: push        ebx
  004271F2: push        5D1F5Ch
  004271F7: call        004CE6E0
  004271FC: add         esp,10h
  004271FF: mov         eax,dword ptr ds:[00E38D98h]
  00427204: mov         ecx,dword ptr ds:[00E38D9Ch]
  0042720A: push        ebx
  0042720B: push        1
  0042720D: push        eax
  0042720E: push        ecx
  0042720F: call        00427480
  00427214: add         esp,10h
  00427217: cmp         eax,ebx
  00427219: mov         dword ptr ds:[00E39174h],eax
  0042721E: jne         0042723B
  00427220: push        ebx
  00427221: push        ebx
  00427222: push        5D1ECCh
  00427227: call        004CE6E0
  0042722C: add         esp,0Ch
  0042722F: xor         eax,eax
  00427231: pop         edi
  00427232: pop         esi
  00427233: pop         ebx
  00427234: add         esp,0D4h
  0042723A: ret
  0042723B: mov         edx,dword ptr [eax]
  0042723D: push        ebx
  0042723E: lea         ecx,[esp+14h]
  00427242: push        1
  00427244: push        ecx
  00427245: push        ebx
  00427246: push        eax
  00427247: call        dword ptr [edx+64h]
  0042724A: test        eax,eax
  0042724C: jne         00427267
  0042724E: mov         edx,dword ptr [esp+34h]
  00427252: mov         eax,dword ptr ds:[00E39174h]
  00427257: mov         dword ptr ds:[00E92E18h],edx
  0042725D: push        ebx
  0042725E: mov         ecx,dword ptr [eax]
  00427260: push        eax
  00427261: call        dword ptr [ecx+00000080h]
  00427267: mov         eax,dword ptr ds:[00E39148h]
  0042726C: push        ebx
  0042726D: push        0E39170h
  00427272: push        ebx
  00427273: mov         edx,dword ptr [eax]
  00427275: push        eax
  00427276: call        dword ptr [edx+10h]
  00427279: test        eax,eax
  0042727B: je          00427298
  0042727D: push        ebx
  0042727E: push        ebx
  0042727F: push        5D1EB0h
  00427284: call        004CE6E0
  00427289: add         esp,0Ch
  0042728C: xor         eax,eax
  0042728E: pop         edi
  0042728F: pop         esi
  00427290: pop         ebx
  00427291: add         esp,0D4h
  00427297: ret
  00427298: mov         eax,dword ptr ds:[00E39170h]
  0042729D: mov         edx,dword ptr ds:[00E39140h]
  004272A3: push        edx
  004272A4: push        ebx
  004272A5: mov         ecx,dword ptr [eax]
  004272A7: push        eax
  004272A8: call        dword ptr [ecx+20h]
  004272AB: test        eax,eax
  004272AD: je          004272CA
  004272AF: push        ebx
  004272B0: push        ebx
  004272B1: push        5D1E88h
  004272B6: call        004CE6E0
  004272BB: add         esp,0Ch
  004272BE: xor         eax,eax
  004272C0: pop         edi
  004272C1: pop         esi
  004272C2: pop         ebx
  004272C3: add         esp,0D4h
  004272C9: ret
  004272CA: mov         eax,dword ptr ds:[00E39178h]
  004272CF: mov         edx,dword ptr ds:[00E39170h]
  004272D5: push        edx
  004272D6: push        eax
  004272D7: mov         ecx,dword ptr [eax]
  004272D9: call        dword ptr [ecx+70h]
  004272DC: test        eax,eax
  004272DE: push        ebx
  004272DF: push        ebx
  004272E0: je          004272FB
  004272E2: push        5D1E5Ch
  004272E7: call        004CE6E0
  004272EC: add         esp,0Ch
  004272EF: xor         eax,eax
  004272F1: pop         edi
  004272F2: pop         esi
  004272F3: pop         ebx
  004272F4: add         esp,0D4h
  004272FA: ret
  004272FB: push        5D1E2Ch
  00427300: call        004CE6E0
  00427305: mov         eax,dword ptr ds:[00E39178h]
  0042730A: add         esp,0Ch
  0042730D: lea         edx,[esp+10h]
  00427311: mov         dword ptr [esp+10h],esi
  00427315: mov         dword ptr [esp+58h],20h
  0042731D: mov         dword ptr [esp+14h],1000h
  00427325: mov         ecx,dword ptr [eax]
  00427327: push        edx
  00427328: push        eax
  00427329: call        dword ptr [ecx+58h]
  0042732C: mov         eax,dword ptr [esp+68h]
  00427330: mov         esi,565h
  00427335: cmp         eax,0FC00h
  0042733A: jne         0042742A
  00427340: push        ebx
  00427341: push        ebx
  00427342: push        5D1E08h
  00427347: mov         dword ptr ds:[00E3914Ch],655h
  00427351: mov         dword ptr ds:[00E92DFCh],3
  0042735B: call        004CE6E0
  00427360: mov         eax,dword ptr ds:[00E3914Ch]
  00427365: add         esp,0Ch
  00427368: cmp         eax,esi
  0042736A: jne         0042737B
  0042736C: push        ebx
  0042736D: push        ebx
  0042736E: push        5D1DE4h
  00427373: call        004CE6E0
  00427378: add         esp,0Ch
  0042737B: cmp         dword ptr ds:[00E3914Ch],556h
  00427385: jne         00427396
  00427387: push        ebx
  00427388: push        ebx
  00427389: push        5D1DC0h
  0042738E: call        004CE6E0
  00427393: add         esp,0Ch
  00427396: cmp         dword ptr ds:[00E3914Ch],555h
  004273A0: jne         004273B1
  004273A2: push        ebx
  004273A3: push        ebx
  004273A4: push        5D1D9Ch
  004273A9: call        004CE6E0
  004273AE: add         esp,0Ch
  004273B1: mov         eax,dword ptr ds:[00E92DFCh]
  004273B6: cmp         eax,1
  004273B9: je          004273C0
  004273BB: cmp         eax,3
  004273BE: jne         004273DF
  004273C0: push        ebx
  004273C1: push        ebx
  004273C2: push        5D1D68h
  004273C7: call        004CE6E0
  004273CC: mov         eax,2
  004273D1: add         esp,0Ch
  004273D4: mov         dword ptr ds:[00E3914Ch],esi
  004273DA: mov         dword ptr ds:[00E92DFCh],eax
  004273DF: cmp         byte ptr ds:[00E38DDAh],bl
  004273E5: je          0042741B
  004273E7: mov         esi,dword ptr [esp+0Ch]
  004273EB: cmp         eax,esi
  004273ED: je          0042741B
  004273EF: push        eax
  004273F0: push        esi
  004273F1: lea         eax,[esp+00000084h]
  004273F8: push        5D1D34h
  004273FD: push        eax
  004273FE: call        00561A36
  00427403: push        ebx
  00427404: lea         ecx,[esp+00000090h]
  0042740B: push        ebx
  0042740C: push        ecx
  0042740D: call        004CE6E0
  00427412: add         esp,1Ch
  00427415: mov         dword ptr ds:[00E92DFCh],esi
  0042741B: pop         edi
  0042741C: pop         esi
  0042741D: mov         eax,1
  00427422: pop         ebx
  00427423: add         esp,0D4h
  00427429: ret
  0042742A: cmp         dword ptr [esp+6Ch],7E0h
  00427432: jne         00427449
  00427434: mov         dword ptr ds:[00E3914Ch],esi
  0042743A: mov         dword ptr ds:[00E92DFCh],2
  00427444: jmp         0042736C
  00427449: cmp         dword ptr [esp+70h],3Fh
  0042744E: jne         00427469
  00427450: mov         dword ptr ds:[00E3914Ch],556h
  0042745A: mov         dword ptr ds:[00E92DFCh],1
  00427464: jmp         00427387
  00427469: mov         dword ptr ds:[00E3914Ch],555h
  00427473: mov         dword ptr ds:[00E92DFCh],ebx
  00427479: jmp         004273A2
  0042747E: nop
  0042747F: nop
  00427480: sub         esp,6Ch
  00427483: push        edi
  00427484: mov         ecx,1Bh
  00427489: xor         eax,eax
  0042748B: lea         edi,[esp+4]
  0042748F: rep stos    dword ptr es:[edi]
  00427491: mov         eax,dword ptr [esp+7Ch]
  00427495: mov         dword ptr [esp+4],6Ch
  0042749D: test        eax,eax
  0042749F: mov         dword ptr [esp+8],7
  004274A7: mov         dword ptr [esp+6Ch],40h
  004274AF: pop         edi
  004274B0: je          004274BA
  004274B2: mov         dword ptr [esp+68h],840h
  004274BA: mov         ecx,dword ptr [esp+70h]
  004274BE: mov         eax,dword ptr [esp+74h]
  004274C2: mov         dword ptr [esp+0Ch],ecx
  004274C6: lea         ecx,[esp+78h]
  004274CA: mov         dword ptr [esp+8],eax
  004274CE: mov         eax,dword ptr ds:[00E39148h]
  004274D3: push        0
  004274D5: push        ecx
  004274D6: mov         edx,dword ptr [eax]
  004274D8: lea         ecx,[esp+8]
  004274DC: push        ecx
  004274DD: push        eax
  004274DE: call        dword ptr [edx+18h]
  004274E1: test        eax,eax
  004274E3: je          004274FC
  004274E5: push        0
  004274E7: push        0
  004274E9: push        5D1FDCh
  004274EE: call        004CE6E0
  004274F3: add         esp,0Ch
  004274F6: xor         eax,eax
  004274F8: add         esp,6Ch
  004274FB: ret
  004274FC: mov         eax,dword ptr [esp+78h]
  00427500: add         esp,6Ch
  00427503: ret
  00427504: nop
  00427505: nop
  00427506: nop
  00427507: nop
  00427508: nop
  00427509: nop
  0042750A: nop
  0042750B: nop
  0042750C: nop
  0042750D: nop
  0042750E: nop
  0042750F: nop
  00427510: 8B

  Summary

      16E000 .text
