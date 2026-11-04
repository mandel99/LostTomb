
Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  00414CD0: mov         eax,dword ptr ds:[006FDC40h]
  00414CD5: sub         esp,7Ch
  00414CD8: test        eax,eax
  00414CDA: push        ebx
  00414CDB: mov         ebx,dword ptr [esp+00000084h]
  00414CE2: push        ebp
  00414CE3: mov         ebp,dword ptr [esp+00000094h]
  00414CEA: push        esi
  00414CEB: mov         esi,dword ptr [esp+00000090h]
  00414CF2: push        edi
  00414CF3: mov         edi,dword ptr [esp+00000098h]
  00414CFA: je          00414D58
  00414CFC: mov         dword ptr [esp+30h],ebx
  00414D00: mov         dword ptr [esp+34h],esi
  00414D04: mov         dword ptr [esp+38h],edi
  00414D08: mov         dword ptr [esp+3Ch],ebp
  00414D0C: call        dword ptr ds:[0056F1F0h]
  00414D12: mov         dword ptr [esp+40h],eax
  00414D16: call        dword ptr ds:[0056F1ECh]
  00414D1C: mov         ecx,eax
  00414D1E: lea         edx,[esp+20h]
  00414D22: shr         eax,10h
  00414D25: mov         dword ptr [esp+48h],eax
  00414D29: lea         eax,[esp+30h]
  00414D2D: and         ecx,0FFFFh
  00414D33: push        edx
  00414D34: push        eax
  00414D35: mov         dword ptr [esp+4Ch],ecx
  00414D39: call        00560330
  00414D3E: mov         eax,dword ptr ds:[006FDC3Ch]
  00414D43: add         esp,8
  00414D46: test        eax,eax
  00414D48: je          00414D58
  00414D4A: lea         ecx,[esp+20h]
  00414D4E: push        ecx
  00414D4F: push        eax
  00414D50: call        005602F0
  00414D55: add         esp,8
  00414D58: mov         eax,dword ptr ds:[006FDC54h]
  00414D5D: test        eax,eax
  00414D5F: je          00414D68
  00414D61: mov         cl,byte ptr [eax+4]
  00414D64: test        cl,cl
  00414D66: je          00414DB3
  00414D68: cmp         byte ptr ds:[00E38DDAh],54h
  00414D6F: jne         00414DC7
  00414D71: cmp         esi,20h
  00414D74: je          00414DC7
  00414D76: cmp         esi,0Fh
  00414D79: je          00414DC7
  00414D7B: cmp         esi,200h
  00414D81: je          00414DC7
  00414D83: cmp         esi,205h
  00414D89: je          00414DC7
  00414D8B: cmp         esi,204h
  00414D91: je          00414DC7
  00414D93: cmp         esi,206h
  00414D99: je          00414DC7
  00414D9B: cmp         esi,202h
  00414DA1: je          00414DC7
  00414DA3: cmp         esi,201h
  00414DA9: je          00414DC7
  00414DAB: cmp         esi,203h
  00414DB1: je          00414DC7
  00414DB3: push        ebp
  00414DB4: push        edi
  00414DB5: push        esi
  00414DB6: push        ebx
  00414DB7: call        dword ptr ds:[0056F1F8h]
  00414DBD: pop         edi
  00414DBE: pop         esi
  00414DBF: pop         ebp
  00414DC0: pop         ebx
  00414DC1: add         esp,7Ch
  00414DC4: ret         10h
  00414DC7: push        edi
  00414DC8: push        esi
  00414DC9: call        0041D110
  00414DCE: add         esp,8
  00414DD1: cmp         esi,1Ch
  00414DD4: ja          00415022
  00414DDA: je          00414F86
  00414DE0: lea         eax,[esi-1]
  00414DE3: cmp         eax,0Fh
  00414DE6: ja          0041635D
  00414DEC: xor         edx,edx
  00414DEE: mov         dl,byte ptr [eax+004163ACh]
  00414DF4: jmp         dword ptr [edx*4+00416390h]
  00414DFB: shr         edi,10h
  00414DFE: je          00414E33
  00414E00: push        0
  00414E02: mov         ecx,0E92BF8h
  00414E07: call        0055F561
  00414E0C: push        2
  00414E0E: mov         ecx,0E92BF8h
  00414E13: call        0055F561
  00414E18: push        1
  00414E1A: mov         ecx,0E92BF8h
  00414E1F: call        0055F561
  00414E24: mov         dword ptr ds:[006FDC48h],1
  00414E2E: jmp         0041635D
  00414E33: mov         eax,dword ptr ds:[006FDC48h]
  00414E38: test        eax,eax
  00414E3A: je          0041635D
  00414E40: mov         eax,dword ptr ds:[00E38E5Ch]
  00414E45: test        eax,eax
  00414E47: je          0041635D
  00414E4D: mov         esi,dword ptr ds:[0056F1FCh]
  00414E53: push        0
  00414E55: call        esi
  00414E57: mov         eax,dword ptr ds:[00E39140h]
  00414E5C: push        eax
  00414E5D: call        esi
  00414E5F: mov         ecx,dword ptr ds:[00E39140h]
  00414E65: push        ecx
  00414E66: call        dword ptr ds:[0056F1F4h]
  00414E6C: mov         edx,dword ptr ds:[00E39140h]
  00414E72: push        3
  00414E74: push        edx
  00414E75: call        dword ptr ds:[0056F178h]
  00414E7B: mov         dword ptr ds:[006FDC48h],0
  00414E85: jmp         0041635D
  00414E8A: push        ebx
  00414E8B: call        dword ptr ds:[0056F204h]
  00414E91: test        eax,eax
  00414E93: je          00414E9C
  00414E95: call        004CFF90
  00414E9A: jmp         00414EAB
  00414E9C: call        004CFFC0
  00414EA1: mov         dword ptr ds:[00E3912Ch],1
  00414EAB: mov         eax,dword ptr ds:[00E38E5Ch]
  00414EB0: test        eax,eax
  00414EB2: je          00414ED5
  00414EB4: mov         esi,dword ptr ds:[0056F1A0h]
  00414EBA: push        1
  00414EBC: call        esi
  00414EBE: push        eax
  00414EBF: push        0
  00414EC1: call        esi
  00414EC3: push        eax
  00414EC4: push        0
  00414EC6: push        0
  00414EC8: push        0E39130h
  00414ECD: call        dword ptr ds:[0056F208h]
  00414ED3: jmp         00414EF7
  00414ED5: push        0E39130h
  00414EDA: push        ebx
  00414EDB: call        dword ptr ds:[0056F200h]
  00414EE1: mov         esi,dword ptr ds:[0056F210h]
  00414EE7: push        0E39130h
  00414EEC: push        ebx
  00414EED: call        esi
  00414EEF: push        0E39138h
  00414EF4: push        ebx
  00414EF5: call        esi
  00414EF7: push        1
  00414EF9: call        004D0280
  00414EFE: add         esp,4
  00414F01: mov         byte ptr ds:[00E38DCAh],1
  00414F08: mov         byte ptr ds:[00E38DC8h],1
  00414F0F: jmp         0041635D
  00414F14: mov         dword ptr ds:[006FDC44h],ebx
  00414F1A: jmp         0041635D
  00414F1F: mov         byte ptr ds:[00D3B43Dh],3
  00414F26: jmp         0041635D
  00414F2B: lea         eax,[esp+4Ch]
  00414F2F: push        eax
  00414F30: push        ebx
  00414F31: call        dword ptr ds:[0056F214h]
  00414F37: lea         ecx,[esp+4Ch]
  00414F3B: push        ecx
  00414F3C: push        ebx
  00414F3D: call        dword ptr ds:[0056F20Ch]
  00414F43: push        1
  00414F45: call        004D0280
  00414F4A: add         esp,4
  00414F4D: mov         byte ptr ds:[00E38DC8h],1
  00414F54: mov         byte ptr ds:[00E38DCAh],1
  00414F5B: jmp         0041635D
  00414F60: call        005391E0
  00414F65: push        1
  00414F67: mov         dword ptr ds:[00E39140h],0
  00414F71: call        004D01E0
  00414F76: add         esp,4
  00414F79: push        0
  00414F7B: call        dword ptr ds:[0056F21Ch]
  00414F81: jmp         0041635D
  00414F86: mov         esi,dword ptr ds:[0056F220h]
  00414F8C: test        edi,edi
  00414F8E: je          00414FB4
  00414F90: call        esi
  00414F92: cmp         eax,ebx
  00414F94: jne         00414FB4
  00414F96: push        ebx
  00414F97: call        dword ptr ds:[0056F204h]
  00414F9D: test        eax,eax
  00414F9F: jne         00414FB4
  00414FA1: push        eax
  00414FA2: push        eax
  00414FA3: mov         dword ptr ds:[00E3912Ch],1
  00414FAD: push        5792E8h
  00414FB2: jmp         00414FC7
  00414FB4: push        0
  00414FB6: push        0
  00414FB8: mov         dword ptr ds:[00E3912Ch],0
  00414FC2: push        5792D4h
  00414FC7: call        004CE6E0
  00414FCC: mov         eax,dword ptr ds:[00E39128h]
  00414FD1: add         esp,0Ch
  00414FD4: test        eax,eax
  00414FD6: je          0041500F
  00414FD8: mov         eax,dword ptr ds:[00E3912Ch]
  00414FDD: test        eax,eax
  00414FDF: je          0041500F
  00414FE1: call        004CFFE0
  00414FE6: test        eax,eax
  00414FE8: jne         0041500A
  00414FEA: call        esi
  00414FEC: cmp         eax,ebx
  00414FEE: jne         0041500F
  00414FF0: push        0
  00414FF2: push        0
  00414FF4: push        5792B0h
  00414FF9: call        004CE6E0
  00414FFE: add         esp,0Ch
  00415001: call        004D0220
  00415006: test        eax,eax
  00415008: je          0041500F
  0041500A: call        004CFFC0
  0041500F: mov         byte ptr ds:[00E38DCAh],1
  00415016: mov         byte ptr ds:[00E38DC8h],1
  0041501D: jmp         0041635D
  00415022: cmp         esi,200h
  00415028: ja          004162F1
  0041502E: je          0041634C
  00415034: lea         eax,[esi-7Eh]
  00415037: cmp         eax,86h
  0041503C: ja          0041635D
  00415042: xor         edx,edx
  00415044: mov         dl,byte ptr [eax+004163D4h]
  0041504A: jmp         dword ptr [edx*4+004163BCh]
  00415051: mov         eax,ebp
  00415053: mov         dword ptr ds:[00E38DBCh],edi
  00415059: and         eax,0FFFFh
  0041505E: shr         ebp,10h
  00415061: mov         dword ptr ds:[00E38DB8h],eax
  00415066: mov         dword ptr ds:[00E38DB4h],ebp
  0041506C: jmp         0041635D
  00415071: lea         eax,[edi-44h]
  00415074: cmp         eax,36h
  00415077: ja          0041635D
  0041507D: xor         ecx,ecx
  0041507F: mov         cl,byte ptr [eax+0041646Ch]
  00415085: jmp         dword ptr [ecx*4+0041645Ch]
  0041508C: push        0
  0041508E: push        1Bh
  00415090: push        100h
  00415095: push        ebx
  00415096: call        dword ptr ds:[0056F174h]
  0041509C: jmp         0041635D
  004150A1: call        00406090
  004150A6: jmp         0041635D
  004150AB: mov         eax,dword ptr ds:[006FDC50h]
  004150B0: xor         edx,edx
  004150B2: test        eax,eax
  004150B4: sete        dl
  004150B7: mov         dword ptr ds:[006FDC50h],edx
  004150BD: jmp         0041635D
  004150C2: push        edi
  004150C3: call        004CC250
  004150C8: add         esp,4
  004150CB: cmp         edi,5Bh
  004150CE: mov         ebp,1
  004150D3: jne         004150E9
  004150D5: push        ebp
  004150D6: call        00416BA0
  004150DB: mov         cl,byte ptr ds:[00E38DDAh]
  004150E1: add         esp,4
  004150E4: jmp         004151A8
  004150E9: cmp         edi,5Dh
  004150EC: jne         00415103
  004150EE: push        0
  004150F0: call        00416BA0
  004150F5: mov         cl,byte ptr ds:[00E38DDAh]
  004150FB: add         esp,4
  004150FE: jmp         004151A8
  00415103: cmp         edi,50h
  00415106: je          0041510D
  00415108: cmp         edi,70h
  0041510B: jne         00415175
  0041510D: mov         al,byte ptr ds:[00E38DDAh]
  00415112: xor         ebx,ebx
  00415114: cmp         al,1Eh
  00415116: mov         bl,al
  00415118: jne         00415123
  0041511A: mov         byte ptr ds:[00E38DDAh],1
  00415121: jmp         0041514E
  00415123: cmp         al,49h
  00415125: jne         00415130
  00415127: mov         byte ptr ds:[00E38DDAh],1
  0041512E: jmp         0041514E
  00415130: cmp         al,1Fh
  00415132: jne         0041513D
  00415134: mov         byte ptr ds:[00E38DDAh],1
  0041513B: jmp         0041514E
  0041513D: cmp         al,20h
  0041513F: jne         0041514A
  00415141: mov         byte ptr ds:[00E38DDAh],1
  00415148: jmp         0041514E
  0041514A: cmp         al,1
  0041514C: jne         0041516B
  0041514E: mov         eax,dword ptr ds:[00605A3Ch]
  00415153: test        eax,eax
  00415155: jne         0041516B
  00415157: mov         al,byte ptr ds:[00E38E60h]
  0041515C: test        al,al
  0041515E: sete        al
  00415161: mov         byte ptr ds:[00E38E60h],al
  00415166: call        004E2AB0
  0041516B: mov         cl,bl
  0041516D: mov         byte ptr ds:[00E38DDAh],cl
  00415173: jmp         0041517B
  00415175: mov         cl,byte ptr ds:[00E38DDAh]
  0041517B: cmp         edi,4Dh
  0041517E: je          00415185
  00415180: cmp         edi,6Dh
  00415183: jne         004151A8
  00415185: mov         eax,dword ptr ds:[00605A3Ch]
  0041518A: test        eax,eax
  0041518C: jne         004151A8
  0041518E: mov         al,byte ptr ds:[00605A5Ch]
  00415193: mov         dword ptr ds:[006FDC50h],0
  0041519D: test        al,al
  0041519F: sete        dl
  004151A2: mov         byte ptr ds:[00605A5Ch],dl
  004151A8: mov         eax,dword ptr ds:[00BCB9C0h]
  004151AD: cmp         cl,1Eh
  004151B0: je          004157B8
  004151B6: cmp         cl,49h
  004151B9: mov         edi,4
  004151BE: jne         004151ED
  004151C0: test        eax,eax
  004151C2: jne         004157AA
  004151C8: mov         esi,dword ptr [esp+00000098h]
  004151CF: cmp         esi,20h
  004151D2: jne         004155AC
  004151D8: cmp         cl,49h
  004151DB: jne         00415569
  004151E1: mov         byte ptr ds:[00E38DDAh],1
  004151E8: jmp         0041558A
  004151ED: cmp         cl,1Fh
  004151F0: jne         004152C7
  004151F6: mov         eax,dword ptr ds:[00D05708h]
  004151FB: test        eax,eax
  004151FD: je          004151C8
  004151FF: mov         eax,dword ptr ds:[00D05700h]
  00415204: mov         ecx,dword ptr [esp+00000098h]
  0041520B: mov         dword ptr ds:[00EADCC8h],eax
  00415210: lea         eax,[ecx-45h]
  00415213: cmp         eax,32h
  00415216: ja          0041635D
  0041521C: xor         edx,edx
  0041521E: mov         dl,byte ptr [eax+004164BCh]
  00415224: jmp         dword ptr [edx*4+004164A4h]
  0041522B: mov         esi,dword ptr ds:[00D3B198h]
  00415231: mov         dword ptr ds:[00D3B198h],0
  0041523B: call        00406B40
  00415240: mov         dword ptr ds:[00D3B198h],esi
  00415246: jmp         0041635D
  0041524B: mov         esi,dword ptr ds:[00D3B198h]
  00415251: mov         dword ptr ds:[00D3B198h],ebp
  00415257: call        00406B40
  0041525C: mov         dword ptr ds:[00D3B198h],esi
  00415262: jmp         0041635D
  00415267: mov         esi,dword ptr ds:[00D3B198h]
  0041526D: mov         dword ptr ds:[00D3B198h],2
  00415277: call        00406B40
  0041527C: mov         dword ptr ds:[00D3B198h],esi
  00415282: jmp         0041635D
  00415287: mov         esi,dword ptr ds:[00D3B198h]
  0041528D: mov         dword ptr ds:[00D3B198h],3
  00415297: call        00406B40
  0041529C: mov         dword ptr ds:[00D3B198h],esi
  004152A2: jmp         0041635D
  004152A7: mov         esi,dword ptr ds:[00D3B198h]
  004152AD: mov         dword ptr ds:[00D3B198h],4
  004152B7: call        00406E80
  004152BC: mov         dword ptr ds:[00D3B198h],esi
  004152C2: jmp         0041635D
  004152C7: cmp         cl,20h
  004152CA: jne         004153A5
  004152D0: mov         eax,dword ptr ds:[00D0570Ch]
  004152D5: test        eax,eax
  004152D7: je          004151C8
  004152DD: mov         eax,dword ptr ds:[00D05704h]
  004152E2: mov         ecx,dword ptr [esp+00000098h]
  004152E9: mov         dword ptr ds:[00EADCC4h],eax
  004152EE: lea         eax,[ecx-41h]
  004152F1: cmp         eax,36h
  004152F4: ja          0041635D
  004152FA: xor         edx,edx
  004152FC: mov         dl,byte ptr [eax+00416508h]
  00415302: jmp         dword ptr [edx*4+004164F0h]
  00415309: mov         esi,dword ptr ds:[00D3B198h]
  0041530F: mov         dword ptr ds:[00D3B198h],0
  00415319: call        00406C90
  0041531E: mov         dword ptr ds:[00D3B198h],esi
  00415324: jmp         0041635D
  00415329: mov         esi,dword ptr ds:[00D3B198h]
  0041532F: mov         dword ptr ds:[00D3B198h],ebp
  00415335: call        00406C90
  0041533A: mov         dword ptr ds:[00D3B198h],esi
  00415340: jmp         0041635D
  00415345: mov         esi,dword ptr ds:[00D3B198h]
  0041534B: mov         dword ptr ds:[00D3B198h],2
  00415355: call        00406C90
  0041535A: mov         dword ptr ds:[00D3B198h],esi
  00415360: jmp         0041635D
  00415365: mov         esi,dword ptr ds:[00D3B198h]
  0041536B: mov         dword ptr ds:[00D3B198h],3
  00415375: call        00406C90
  0041537A: mov         dword ptr ds:[00D3B198h],esi
  00415380: jmp         0041635D
  00415385: mov         esi,dword ptr ds:[00D3B198h]
  0041538B: mov         dword ptr ds:[00D3B198h],4
  00415395: call        00406DE0
  0041539A: mov         dword ptr ds:[00D3B198h],esi
  004153A0: jmp         0041635D
  004153A5: cmp         cl,9
  004153A8: jne         00415532
  004153AE: mov         eax,dword ptr ds:[00EADD50h]
  004153B3: cmp         eax,edi
  004153B5: jne         00415490
  004153BB: mov         eax,dword ptr [esp+00000098h]
  004153C2: mov         esi,dword ptr ds:[00BCB9C0h]
  004153C8: add         eax,0FFFFFFBDh
  004153CB: cmp         eax,31h
  004153CE: ja          0041635D
  004153D4: xor         ecx,ecx
  004153D6: mov         cl,byte ptr [eax+00416560h]
  004153DC: jmp         dword ptr [ecx*4+00416540h]
  004153E3: mov         dword ptr ds:[00D3B198h],0
  004153ED: call        00406720
  004153F2: jmp         0041635D
  004153F7: lea         edx,[esi+esi*8]
  004153FA: shl         edx,4
  004153FD: cmp         word ptr [edx+00BCB9CCh],0Dh
  00415405: je          0041635D
  0041540B: mov         dword ptr ds:[00D3B198h],ebp
  00415411: call        00406720
  00415416: jmp         0041635D
  0041541B: mov         dword ptr ds:[00D3B198h],2
  00415425: call        00406720
  0041542A: jmp         0041635D
  0041542F: mov         dword ptr ds:[00D3B198h],3
  00415439: call        00406720
  0041543E: lea         eax,[esi+esi*8]
  00415441: shl         eax,4
  00415444: cmp         word ptr [eax+00BCB9F2h],6
  0041544C: jne         0041635D
  00415452: mov         byte ptr ds:[00E38DDAh],1
  00415459: jmp         0041635D
  0041545E: lea         eax,[esi+esi*8]
  00415461: shl         eax,4
  00415464: cmp         word ptr [eax+00BCB9CCh],0Dh
  0041546C: jne         0041635D
  00415472: cmp         word ptr [eax+00BCBA46h],0
  0041547A: jne         0041635D
  00415480: mov         dword ptr ds:[00D3B198h],ebp
  00415486: call        00406720
  0041548B: jmp         0041635D
  00415490: cmp         eax,5
  00415493: jne         004154DF
  00415495: mov         edx,dword ptr [esp+00000098h]
  0041549C: mov         ecx,dword ptr ds:[00D05704h]
  004154A2: mov         dword ptr ds:[00EADCC4h],ecx
  004154A8: lea         eax,[edx-41h]
  004154AB: cmp         eax,36h
  004154AE: ja          0041635D
  004154B4: xor         ecx,ecx
  004154B6: mov         cl,byte ptr [eax+004165ACh]
  004154BC: jmp         dword ptr [ecx*4+00416594h]
  004154C3: mov         esi,dword ptr ds:[00D3B198h]
  004154C9: mov         dword ptr ds:[00D3B198h],edi
  004154CF: call        00406DE0
  004154D4: mov         dword ptr ds:[00D3B198h],esi
  004154DA: jmp         0041635D
  004154DF: cmp         eax,6
  004154E2: jne         004151C8
  004154E8: mov         eax,dword ptr [esp+00000098h]
  004154EF: mov         edx,dword ptr ds:[00D05700h]
  004154F5: add         eax,0FFFFFFBBh
  004154F8: mov         dword ptr ds:[00EADCC4h],edx
  004154FE: cmp         eax,32h
  00415501: ja          0041635D
  00415507: xor         ecx,ecx
  00415509: mov         cl,byte ptr [eax+004165FCh]
  0041550F: jmp         dword ptr [ecx*4+004165E4h]
  00415516: mov         esi,dword ptr ds:[00D3B198h]
  0041551C: mov         dword ptr ds:[00D3B198h],edi
  00415522: call        00406E80
  00415527: mov         dword ptr ds:[00D3B198h],esi
  0041552D: jmp         0041635D
  00415532: cmp         cl,0Eh
  00415535: jne         004151C8
  0041553B: mov         eax,dword ptr [esp+00000098h]
  00415542: cmp         eax,30h
  00415545: jb          0041635D
  0041554B: cmp         eax,39h
  0041554E: ja          0041635D
  00415554: mov         edx,eax
  00415556: add         edx,0FFFFFFD0h
  00415559: mov         dword ptr ds:[00D3B198h],edx
  0041555F: call        004035A0
  00415564: jmp         0041635D
  00415569: cmp         cl,1Fh
  0041556C: jne         00415577
  0041556E: mov         byte ptr ds:[00E38DDAh],1
  00415575: jmp         0041558A
  00415577: cmp         cl,20h
  0041557A: jne         00415585
  0041557C: mov         byte ptr ds:[00E38DDAh],1
  00415583: jmp         0041558A
  00415585: cmp         cl,1
  00415588: jne         004155AC
  0041558A: mov         eax,dword ptr ds:[00E94E10h]
  0041558F: mov         ecx,dword ptr ds:[00E94E0Ch]
  00415595: mov         dword ptr ds:[00E94E10h],ecx
  0041559B: mov         dword ptr ds:[00E94E0Ch],eax
  004155A0: mov         byte ptr ds:[00E38DCAh],1
  004155A7: call        00547A60
  004155AC: mov         eax,dword ptr ds:[00605A3Ch]
  004155B1: test        eax,eax
  004155B3: jne         00415787
  004155B9: cmp         esi,46h
  004155BC: je          004155C3
  004155BE: cmp         esi,66h
  004155C1: jne         004155CD
  004155C3: push        8
  004155C5: call        004168E0
  004155CA: add         esp,4
  004155CD: cmp         esi,44h
  004155D0: je          004155D7
  004155D2: cmp         esi,64h
  004155D5: jne         004155E1
  004155D7: push        9
  004155D9: call        004168E0
  004155DE: add         esp,4
  004155E1: cmp         esi,43h
  004155E4: je          004155EB
  004155E6: cmp         esi,63h
  004155E9: jne         004155F5
  004155EB: push        0Ah
  004155ED: call        004168E0
  004155F2: add         esp,4
  004155F5: cmp         esi,54h
  004155F8: je          004155FF
  004155FA: cmp         esi,74h
  004155FD: jne         00415609
  004155FF: push        1Fh
  00415601: call        004168E0
  00415606: add         esp,4
  00415609: cmp         esi,57h
  0041560C: je          00415613
  0041560E: cmp         esi,77h
  00415611: jne         0041561D
  00415613: push        2Dh
  00415615: call        004168E0
  0041561A: add         esp,4
  0041561D: cmp         esi,48h
  00415620: je          00415627
  00415622: cmp         esi,68h
  00415625: jne         00415631
  00415627: push        33h
  00415629: call        004168E0
  0041562E: add         esp,4
  00415631: cmp         esi,58h
  00415634: je          0041563B
  00415636: cmp         esi,78h
  00415639: jne         00415645
  0041563B: push        19h
  0041563D: call        004168E0
  00415642: add         esp,4
  00415645: cmp         esi,59h
  00415648: je          0041564F
  0041564A: cmp         esi,79h
  0041564D: jne         00415659
  0041564F: push        30h
  00415651: call        004168E0
  00415656: add         esp,4
  00415659: cmp         esi,5Ah
  0041565C: je          00415663
  0041565E: cmp         esi,7Ah
  00415661: jne         0041566D
  00415663: push        2Bh
  00415665: call        004168E0
  0041566A: add         esp,4
  0041566D: cmp         esi,60h
  00415670: jne         00415680
  00415672: push        ebp
  00415673: call        00416990
  00415678: add         esp,4
  0041567B: jmp         0041635D
  00415680: cmp         esi,31h
  00415683: jne         00415694
  00415685: push        2
  00415687: call        00416990
  0041568C: add         esp,4
  0041568F: jmp         0041635D
  00415694: cmp         esi,32h
  00415697: jne         004156A7
  00415699: push        edi
  0041569A: call        00416990
  0041569F: add         esp,4
  004156A2: jmp         0041635D
  004156A7: cmp         esi,33h
  004156AA: jne         004156BB
  004156AC: push        5
  004156AE: call        00416990
  004156B3: add         esp,4
  004156B6: jmp         0041635D
  004156BB: cmp         esi,34h
  004156BE: jne         004156CF
  004156C0: push        6
  004156C2: call        00416990
  004156C7: add         esp,4
  004156CA: jmp         0041635D
  004156CF: cmp         esi,35h
  004156D2: jne         004156E3
  004156D4: push        7
  004156D6: call        00416990
  004156DB: add         esp,4
  004156DE: jmp         0041635D
  004156E3: cmp         esi,36h
  004156E6: jne         004156F7
  004156E8: push        8
  004156EA: call        00416990
  004156EF: add         esp,4
  004156F2: jmp         0041635D
  004156F7: cmp         esi,37h
  004156FA: jne         0041570B
  004156FC: push        9
  004156FE: call        00416990
  00415703: add         esp,4
  00415706: jmp         0041635D
  0041570B: cmp         esi,38h
  0041570E: jne         0041571F
  00415710: push        0Ah
  00415712: call        00416990
  00415717: add         esp,4
  0041571A: jmp         0041635D
  0041571F: cmp         esi,39h
  00415722: jne         00415733
  00415724: push        0Bh
  00415726: call        00416990
  0041572B: add         esp,4
  0041572E: jmp         0041635D
  00415733: cmp         esi,30h
  00415736: jne         00415747
  00415738: push        0Ch
  0041573A: call        00416990
  0041573F: add         esp,4
  00415742: jmp         0041635D
  00415747: cmp         esi,2Dh
  0041574A: jne         0041575B
  0041574C: push        0Dh
  0041574E: call        00416990
  00415753: add         esp,4
  00415756: jmp         0041635D
  0041575B: cmp         esi,3Dh
  0041575E: jne         0041576F
  00415760: push        0Eh
  00415762: call        00416990
  00415767: add         esp,4
  0041576A: jmp         0041635D
  0041576F: cmp         esi,4Ch
  00415772: je          0041577D
  00415774: cmp         esi,6Ch
  00415777: jne         0041635D
  0041577D: call        00416A20
  00415782: jmp         0041635D
  00415787: cmp         esi,48h
  0041578A: je          00415795
  0041578C: cmp         esi,68h
  0041578F: jne         0041635D
  00415795: mov         al,byte ptr ds:[006DE515h]
  0041579A: test        al,al
  0041579C: sete        dl
  0041579F: mov         byte ptr ds:[006DE515h],dl
  004157A5: jmp         0041635D
  004157AA: mov         edi,dword ptr [esp+00000098h]
  004157B1: mov         byte ptr ds:[00E38DDAh],1Eh
  004157B8: lea         ecx,[edi-43h]
  004157BB: cmp         ecx,31h
  004157BE: ja          0041635D
  004157C4: xor         edx,edx
  004157C6: mov         dl,byte ptr [ecx+00416650h]
  004157CC: jmp         dword ptr [edx*4+00416630h]
  004157D3: lea         eax,[eax+eax*8]
  004157D6: shl         eax,4
  004157D9: mov         cl,byte ptr [eax+00BCBA3Fh]
  004157DF: test        cl,cl
  004157E1: je          004157F1
  004157E3: mov         word ptr [eax+00BCB9F2h],2
  004157EC: jmp         0041635D
  004157F1: mov         word ptr [eax+00BCB9F2h],bp
  004157F8: jmp         0041635D
  004157FD: lea         eax,[eax+eax*8]
  00415800: shl         eax,4
  00415803: cmp         word ptr [eax+00BCB9CCh],0Dh
  0041580B: je          0041635D
  00415811: mov         cl,byte ptr [eax+00BCBA3Fh]
  00415817: test        cl,cl
  00415819: je          00415829
  0041581B: mov         word ptr [eax+00BCB9F2h],4
  00415824: jmp         0041635D
  00415829: mov         word ptr [eax+00BCB9F2h],3
  00415832: jmp         0041635D
  00415837: mov         dword ptr ds:[00D3B194h],ebp
  0041583D: call        00407110
  00415842: jmp         0041635D
  00415847: lea         eax,[eax+eax*8]
  0041584A: shl         eax,4
  0041584D: mov         word ptr [eax+00BCB9F2h],0Fh
  00415856: jmp         0041635D
  0041585B: lea         eax,[eax+eax*8]
  0041585E: shl         eax,4
  00415861: mov         cx,word ptr [eax+00BCB9F2h]
  00415868: mov         word ptr [eax+00BCBA2Ah],cx
  0041586F: mov         word ptr [eax+00BCB9F2h],6
  00415878: jmp         0041635D
  0041587D: mov         dword ptr ds:[00EADCCCh],eax
  00415882: call        00406FB0
  00415887: jmp         0041635D
  0041588C: lea         eax,[eax+eax*8]
  0041588F: shl         eax,4
  00415892: cmp         word ptr [eax+00BCB9CCh],0Dh
  0041589A: jne         0041635D
  004158A0: cmp         word ptr [eax+00BCBA46h],0
  004158A8: jne         0041635D
  004158AE: mov         byte ptr ds:[00E38DDAh],49h
  004158B5: jmp         0041635D
  004158BA: lea         eax,[edi-8]
  004158BD: mov         esi,1
  004158C2: cmp         eax,73h
  004158C5: mov         byte ptr ds:[00E38E4Eh],1
  004158CC: ja          0041635D
  004158D2: xor         edx,edx
  004158D4: mov         dl,byte ptr [eax+00416708h]
  004158DA: jmp         dword ptr [edx*4+00416684h]
  004158E1: mov         dword ptr ds:[00E92E20h],esi
  004158E7: jmp         0041635D
  004158EC: mov         dword ptr ds:[00E92E24h],esi
  004158F2: jmp         0041635D
  004158F7: mov         al,byte ptr ds:[00775680h]
  004158FC: test        al,al
  004158FE: sete        al
  00415901: mov         byte ptr ds:[00775680h],al
  00415906: jmp         0041635D
  0041590B: mov         al,byte ptr ds:[00578C68h]
  00415910: test        al,al
  00415912: jne         0041635D
  00415918: mov         eax,dword ptr ds:[00E92E20h]
  0041591D: test        eax,eax
  0041591F: je          0041635D
  00415925: cmp         byte ptr ds:[00E38DDAh],1
  0041592C: jne         0041635D
  00415932: mov         eax,dword ptr ds:[00605A08h]
  00415937: inc         eax
  00415938: cmp         eax,4
  0041593B: mov         dword ptr ds:[00605A08h],eax
  00415940: jle         0041635D
  00415946: mov         dword ptr ds:[00605A08h],0
  00415950: jmp         0041635D
  00415955: mov         al,byte ptr ds:[00578C68h]
  0041595A: test        al,al
  0041595C: jne         0041635D
  00415962: mov         eax,dword ptr ds:[00E92E20h]
  00415967: test        eax,eax
  00415969: je          0041635D
  0041596F: cmp         byte ptr ds:[00E38DDAh],1
  00415976: jne         0041635D
  0041597C: mov         eax,dword ptr ds:[00605A04h]
  00415981: inc         eax
  00415982: cmp         eax,3
  00415985: mov         dword ptr ds:[00605A04h],eax
  0041598A: jle         0041635D
  00415990: mov         dword ptr ds:[00605A04h],esi
  00415996: jmp         0041635D
  0041599B: mov         al,byte ptr ds:[00578C68h]
  004159A0: test        al,al
  004159A2: jne         0041635D
  004159A8: mov         eax,dword ptr ds:[00E92E20h]
  004159AD: test        eax,eax
  004159AF: je          0041635D
  004159B5: cmp         byte ptr ds:[00E38DDAh],1
  004159BC: jne         0041635D
  004159C2: mov         eax,dword ptr ds:[00605A00h]
  004159C7: inc         eax
  004159C8: cmp         eax,4
  004159CB: mov         dword ptr ds:[00605A00h],eax
  004159D0: jle         0041635D
  004159D6: mov         dword ptr ds:[00605A00h],0
  004159E0: jmp         0041635D
  004159E5: mov         al,byte ptr ds:[00578C68h]
  004159EA: test        al,al
  004159EC: jne         0041635D
  004159F2: mov         eax,dword ptr ds:[00E92E20h]
  004159F7: test        eax,eax
  004159F9: je          0041635D
  004159FF: cmp         byte ptr ds:[00E38DDAh],1
  00415A06: jne         0041635D
  00415A0C: mov         eax,dword ptr ds:[00605A00h]
  00415A11: test        eax,eax
  00415A13: je          0041635D
  00415A19: mov         ecx,dword ptr ds:[00605A04h]
  00415A1F: mov         edx,dword ptr ds:[00605A08h]
  00415A25: push        eax
  00415A26: push        ecx
  00415A27: push        edx
  00415A28: call        004C47A0
  00415A2D: add         esp,0Ch
  00415A30: jmp         0041635D
  00415A35: call        004CC3C0
  00415A3A: cmp         byte ptr ds:[00E38DDAh],1
  00415A41: jne         0041635D
  00415A47: mov         dword ptr ds:[00D3B198h],0
  00415A51: call        00409EB0
  00415A56: jmp         0041635D
  00415A5B: call        004CC3E0
  00415A60: jmp         0041635D
  00415A65: call        004CC390
  00415A6A: mov         eax,dword ptr ds:[00E92E20h]
  00415A6F: test        eax,eax
  00415A71: je          00415A77
  00415A73: push        8
  00415A75: jmp         00415A78
  00415A77: push        esi
  00415A78: push        2
  00415A7A: call        00407C10
  00415A7F: add         esp,8
  00415A82: mov         dword ptr ds:[00E92E2Ch],esi
  00415A88: jmp         0041635D
  00415A8D: call        004CC370
  00415A92: mov         eax,dword ptr ds:[00E92E20h]
  00415A97: test        eax,eax
  00415A99: je          00415A9F
  00415A9B: push        8
  00415A9D: jmp         00415AA0
  00415A9F: push        esi
  00415AA0: push        6
  00415AA2: call        00407C10
  00415AA7: add         esp,8
  00415AAA: mov         dword ptr ds:[00E92E30h],esi
  00415AB0: jmp         0041635D
  00415AB5: xor         eax,eax
  00415AB7: mov         al,byte ptr ds:[00E38DDAh]
  00415ABC: add         eax,0FFFFFFF9h
  00415ABF: cmp         eax,36h
  00415AC2: ja          00415B95
  00415AC8: xor         ecx,ecx
  00415ACA: mov         cl,byte ptr [eax+00416794h]
  00415AD0: jmp         dword ptr [ecx*4+0041677Ch]
  00415AD7: cmp         dword ptr ds:[00605A44h],6
  00415ADE: jne         00415BB2
  00415AE4: mov         dword ptr ds:[00D3B198h],esi
  00415AEA: call        0040C760
  00415AEF: mov         dword ptr ds:[00E92E34h],esi
  00415AF5: jmp         0041635D
  00415AFA: mov         eax,dword ptr ds:[00EADD6Ch]
  00415AFF: mov         edx,eax
  00415B01: shl         edx,5
  00415B04: add         edx,eax
  00415B06: mov         ax,word ptr [edx*8+007999E8h]
  00415B0E: cmp         ax,48h
  00415B12: je          00415B24
  00415B14: cmp         ax,0C1h
  00415B18: je          00415B24
  00415B1A: cmp         ax,49h
  00415B1E: jne         00415BB2
  00415B24: mov         eax,dword ptr ds:[00EADCC0h]
  00415B29: mov         dword ptr ds:[00D3B198h],esi
  00415B2F: test        eax,eax
  00415B31: je          00415B43
  00415B33: call        0040C6B0
  00415B38: mov         dword ptr ds:[00E92E34h],esi
  00415B3E: jmp         0041635D
  00415B43: call        0040C630
  00415B48: mov         dword ptr ds:[00E92E34h],esi
  00415B4E: jmp         0041635D
  00415B53: mov         dword ptr ds:[00D3B198h],esi
  00415B59: call        00402980
  00415B5E: mov         dword ptr ds:[00E92E34h],esi
  00415B64: jmp         0041635D
  00415B69: mov         dword ptr ds:[00D3B198h],esi
  00415B6F: call        0041F460
  00415B74: mov         dword ptr ds:[00E92E34h],esi
  00415B7A: jmp         0041635D
  00415B7F: mov         dword ptr ds:[00D3B198h],esi
  00415B85: call        00516890
  00415B8A: mov         dword ptr ds:[00E92E34h],esi
  00415B90: jmp         0041635D
  00415B95: call        004CC390
  00415B9A: mov         eax,dword ptr ds:[00E92E20h]
  00415B9F: test        eax,eax
  00415BA1: je          00415BA7
  00415BA3: push        8
  00415BA5: jmp         00415BA8
  00415BA7: push        esi
  00415BA8: push        4
  00415BAA: call        00407C10
  00415BAF: add         esp,8
  00415BB2: mov         dword ptr ds:[00E92E34h],esi
  00415BB8: jmp         0041635D
  00415BBD: xor         eax,eax
  00415BBF: mov         al,byte ptr ds:[00E38DDAh]
  00415BC4: add         eax,0FFFFFFF9h
  00415BC7: cmp         eax,36h
  00415BCA: ja          00415CB1
  00415BD0: xor         ecx,ecx
  00415BD2: mov         cl,byte ptr [eax+004167E4h]
  00415BD8: jmp         dword ptr [ecx*4+004167CCh]
  00415BDF: cmp         dword ptr ds:[00605A44h],6
  00415BE6: jne         00415CCE
  00415BEC: mov         dword ptr ds:[00D3B198h],0
  00415BF6: call        0040C760
  00415BFB: mov         dword ptr ds:[00E92E38h],esi
  00415C01: jmp         0041635D
  00415C06: mov         eax,dword ptr ds:[00EADD6Ch]
  00415C0B: mov         edx,eax
  00415C0D: shl         edx,5
  00415C10: add         edx,eax
  00415C12: mov         ax,word ptr [edx*8+007999E8h]
  00415C1A: cmp         ax,48h
  00415C1E: je          00415C30
  00415C20: cmp         ax,0C1h
  00415C24: je          00415C30
  00415C26: cmp         ax,49h
  00415C2A: jne         00415CCE
  00415C30: mov         eax,dword ptr ds:[00EADCC0h]
  00415C35: mov         dword ptr ds:[00D3B198h],0
  00415C3F: test        eax,eax
  00415C41: je          00415C53
  00415C43: call        0040C6B0
  00415C48: mov         dword ptr ds:[00E92E38h],esi
  00415C4E: jmp         0041635D
  00415C53: call        0040C630
  00415C58: mov         dword ptr ds:[00E92E38h],esi
  00415C5E: jmp         0041635D
  00415C63: mov         dword ptr ds:[00D3B198h],0
  00415C6D: call        00402980
  00415C72: mov         dword ptr ds:[00E92E38h],esi
  00415C78: jmp         0041635D
  00415C7D: mov         dword ptr ds:[00D3B198h],0
  00415C87: call        0041F460
  00415C8C: mov         dword ptr ds:[00E92E38h],esi
  00415C92: jmp         0041635D
  00415C97: mov         dword ptr ds:[00D3B198h],0
  00415CA1: call        00516890
  00415CA6: mov         dword ptr ds:[00E92E38h],esi
  00415CAC: jmp         0041635D
  00415CB1: call        004CC370
  00415CB6: mov         eax,dword ptr ds:[00E92E20h]
  00415CBB: test        eax,eax
  00415CBD: je          00415CC3
  00415CBF: push        8
  00415CC1: jmp         00415CC4
  00415CC3: push        esi
  00415CC4: push        0
  00415CC6: call        00407C10
  00415CCB: add         esp,8
  00415CCE: mov         dword ptr ds:[00E92E38h],esi
  00415CD4: jmp         0041635D
  00415CD9: xor         eax,eax
  00415CDB: mov         al,byte ptr ds:[00E38DDAh]
  00415CE0: dec         eax
  00415CE1: cmp         eax,3Ch
  00415CE4: ja          0041635D
  00415CEA: xor         ecx,ecx
  00415CEC: mov         cl,byte ptr [eax+00416838h]
  00415CF2: jmp         dword ptr [ecx*4+0041681Ch]
  00415CF9: mov         dword ptr ds:[00D3B198h],2
  00415D03: call        00409EB0
  00415D08: jmp         0041635D
  00415D0D: mov         dword ptr ds:[00D3B198h],0
  00415D17: mov         esi,0Ch
  00415D1C: call        00402980
  00415D21: dec         esi
  00415D22: jne         00415D1C
  00415D24: mov         byte ptr ds:[00E38DCAh],1
  00415D2B: jmp         0041635D
  00415D30: mov         dword ptr ds:[00D3B198h],0
  00415D3A: mov         esi,0Fh
  00415D3F: call        0041F460
  00415D44: dec         esi
  00415D45: jne         00415D3F
  00415D47: mov         byte ptr ds:[00E38DCAh],1
  00415D4E: jmp         0041635D
  00415D53: mov         dword ptr ds:[00D3B198h],0
  00415D5D: mov         esi,0Ch
  00415D62: call        00516890
  00415D67: dec         esi
  00415D68: jne         00415D62
  00415D6A: mov         byte ptr ds:[00E38DCAh],1
  00415D71: jmp         0041635D
  00415D76: cmp         dword ptr ds:[00605A44h],6
  00415D7D: jne         0041635D
  00415D83: mov         dword ptr ds:[00D3B198h],0
  00415D8D: mov         esi,0Ch
  00415D92: call        0040C760
  00415D97: dec         esi
  00415D98: jne         00415D92
  00415D9A: mov         byte ptr ds:[00E38DCAh],1
  00415DA1: jmp         0041635D
  00415DA6: mov         eax,dword ptr ds:[00EADD6Ch]
  00415DAB: mov         edx,eax
  00415DAD: shl         edx,5
  00415DB0: add         edx,eax
  00415DB2: mov         ax,word ptr [edx*8+007999E8h]
  00415DBA: cmp         ax,48h
  00415DBE: je          00415DD0
  00415DC0: cmp         ax,0C1h
  00415DC4: je          00415DD0
  00415DC6: cmp         ax,49h
  00415DCA: jne         0041635D
  00415DD0: xor         esi,esi
  00415DD2: mov         dword ptr ds:[00D3B198h],esi
  00415DD8: call        00502780
  00415DDD: test        eax,eax
  00415DDF: jle         00415DF0
  00415DE1: call        0040C6B0
  00415DE6: inc         esi
  00415DE7: call        00502780
  00415DEC: cmp         esi,eax
  00415DEE: jl          00415DE1
  00415DF0: mov         byte ptr ds:[00E38DCAh],1
  00415DF7: jmp         0041635D
  00415DFC: xor         eax,eax
  00415DFE: mov         al,byte ptr ds:[00E38DDAh]
  00415E03: dec         eax
  00415E04: cmp         eax,3Ch
  00415E07: ja          0041635D
  00415E0D: xor         ecx,ecx
  00415E0F: mov         cl,byte ptr [eax+00416894h]
  00415E15: jmp         dword ptr [ecx*4+00416878h]
  00415E1C: mov         dword ptr ds:[00D3B198h],esi
  00415E22: call        00409EB0
  00415E27: jmp         0041635D
  00415E2C: mov         dword ptr ds:[00D3B198h],esi
  00415E32: mov         esi,0Ch
  00415E37: call        00402980
  00415E3C: dec         esi
  00415E3D: jne         00415E37
  00415E3F: mov         byte ptr ds:[00E38DCAh],1
  00415E46: jmp         0041635D
  00415E4B: mov         dword ptr ds:[00D3B198h],esi
  00415E51: mov         esi,0Fh
  00415E56: call        0041F460
  00415E5B: dec         esi
  00415E5C: jne         00415E56
  00415E5E: mov         byte ptr ds:[00E38DCAh],1
  00415E65: jmp         0041635D
  00415E6A: mov         dword ptr ds:[00D3B198h],esi
  00415E70: mov         esi,0Ch
  00415E75: call        00516890
  00415E7A: dec         esi
  00415E7B: jne         00415E75
  00415E7D: mov         byte ptr ds:[00E38DCAh],1
  00415E84: jmp         0041635D
  00415E89: cmp         dword ptr ds:[00605A44h],6
  00415E90: jne         0041635D
  00415E96: mov         dword ptr ds:[00D3B198h],esi
  00415E9C: mov         esi,0Ch
  00415EA1: call        0040C760
  00415EA6: dec         esi
  00415EA7: jne         00415EA1
  00415EA9: mov         byte ptr ds:[00E38DCAh],1
  00415EB0: jmp         0041635D
  00415EB5: mov         eax,dword ptr ds:[00EADD6Ch]
  00415EBA: mov         edx,eax
  00415EBC: shl         edx,5
  00415EBF: add         edx,eax
  00415EC1: mov         ax,word ptr [edx*8+007999E8h]
  00415EC9: cmp         ax,48h
  00415ECD: je          00415EDF
  00415ECF: cmp         ax,0C1h
  00415ED3: je          00415EDF
  00415ED5: cmp         ax,49h
  00415ED9: jne         0041635D
  00415EDF: mov         dword ptr ds:[00D3B198h],esi
  00415EE5: xor         esi,esi
  00415EE7: call        00502780
  00415EEC: test        eax,eax
  00415EEE: jle         00415EFF
  00415EF0: call        0040C6B0
  00415EF5: inc         esi
  00415EF6: call        00502780
  00415EFB: cmp         esi,eax
  00415EFD: jl          00415EF0
  00415EFF: mov         byte ptr ds:[00E38DCAh],1
  00415F06: jmp         0041635D
  00415F0B: call        004CC360
  00415F10: jmp         0041635D
  00415F15: cmp         byte ptr ds:[00E38DDAh],0Eh
  00415F1C: jne         00415F28
  00415F1E: call        00403770
  00415F23: jmp         0041635D
  00415F28: call        004CC350
  00415F2D: jmp         0041635D
  00415F32: call        004CC330
  00415F37: jmp         0041635D
  00415F3C: cmp         byte ptr ds:[00E38DDAh],0Eh
  00415F43: jne         00415F54
  00415F45: call        004035F0
  00415F4A: call        004CC320
  00415F4F: jmp         0041635D
  00415F54: call        004CC350
  00415F59: call        004CC320
  00415F5E: jmp         0041635D
  00415F63: mov         al,byte ptr ds:[00578C68h]
  00415F68: test        al,al
  00415F6A: jne         0041635D
  00415F70: mov         eax,dword ptr ds:[00E92E20h]
  00415F75: test        eax,eax
  00415F77: je          00415F88
  00415F79: cmp         byte ptr ds:[00E38DDAh],1
  00415F80: jne         00415F88
  00415F82: xor         dword ptr ds:[00605A0Ch],esi
  00415F88: call        00404350
  00415F8D: call        004051A0
  00415F92: mov         eax,dword ptr ds:[00E92E20h]
  00415F97: test        eax,eax
  00415F99: je          0041635D
  00415F9F: mov         al,byte ptr ds:[00E38E74h]
  00415FA4: test        al,al
  00415FA6: je          0041635D
  00415FAC: cmp         byte ptr ds:[00E38DDAh],36h
  00415FB3: je          0041635D
  00415FB9: mov         ecx,6FDD08h
  00415FBE: call        0041BAE0
  00415FC3: call        00516A20
  00415FC8: push        eax
  00415FC9: mov         ecx,6FDD08h
  00415FCE: call        0041B8A0
  00415FD3: jmp         0041635D
  00415FD8: mov         al,byte ptr ds:[00578C68h]
  00415FDD: test        al,al
  00415FDF: jne         0041635D
  00415FE5: mov         eax,dword ptr ds:[00E38E94h]
  00415FEA: dec         eax
  00415FEB: mov         dword ptr ds:[00E38E94h],eax
  00415FF0: jns         0041635D
  00415FF6: mov         dword ptr ds:[00E38E94h],1Bh
  00416000: jmp         0041635D
  00416005: cmp         byte ptr ds:[00E38DDAh],0Eh
  0041600C: jne         0041635D
  00416012: mov         al,byte ptr ds:[00578C68h]
  00416017: test        al,al
  00416019: je          0041635D
  0041601F: mov         dword ptr ds:[00D3B198h],3
  00416029: call        004035A0
  0041602E: jmp         0041635D
  00416033: mov         eax,dword ptr ds:[00E92E20h]
  00416038: push        0
  0041603A: test        eax,eax
  0041603C: je          0041604B
  0041603E: call        00416B10
  00416043: add         esp,4
  00416046: jmp         0041635D
  0041604B: call        00416AC0
  00416050: add         esp,4
  00416053: jmp         0041635D
  00416058: mov         eax,dword ptr ds:[00E92E20h]
  0041605D: push        esi
  0041605E: test        eax,eax
  00416060: je          0041604B
  00416062: call        00416B10
  00416067: add         esp,4
  0041606A: jmp         0041635D
  0041606F: mov         eax,dword ptr ds:[00E92E20h]
  00416074: push        2
  00416076: test        eax,eax
  00416078: je          0041604B
  0041607A: call        00416B10
  0041607F: add         esp,4
  00416082: jmp         0041635D
  00416087: mov         eax,dword ptr ds:[00E92E20h]
  0041608C: push        3
  0041608E: test        eax,eax
  00416090: je          0041604B
  00416092: call        00416B10
  00416097: add         esp,4
  0041609A: jmp         0041635D
  0041609F: mov         eax,dword ptr ds:[00E38E5Ch]
  004160A4: test        eax,eax
  004160A6: jne         0041635D
  004160AC: lea         eax,[esp+10h]
  004160B0: push        eax
  004160B1: push        ebx
  004160B2: call        dword ptr ds:[0056F200h]
  004160B8: mov         ecx,dword ptr ds:[00E38D98h]
  004160BE: mov         edx,dword ptr ds:[00E38D9Ch]
  004160C4: push        ecx
  004160C5: push        edx
  004160C6: push        0
  004160C8: lea         eax,[esp+1Ch]
  004160CC: push        0
  004160CE: push        eax
  004160CF: call        dword ptr ds:[0056F208h]
  004160D5: mov         esi,dword ptr ds:[0056F218h]
  004160DB: push        0FFFFFFECh
  004160DD: push        ebx
  004160DE: call        esi
  004160E0: push        eax
  004160E1: push        ebx
  004160E2: call        dword ptr ds:[0056F17Ch]
  004160E8: neg         eax
  004160EA: sbb         eax,eax
  004160EC: neg         eax
  004160EE: push        eax
  004160EF: push        0FFFFFFF0h
  004160F1: push        ebx
  004160F2: call        esi
  004160F4: lea         ecx,[esp+18h]
  004160F8: push        eax
  004160F9: push        ecx
  004160FA: call        dword ptr ds:[0056F224h]
  00416100: mov         edi,dword ptr [esp+14h]
  00416104: push        14h
  00416106: mov         edx,dword ptr [esp+20h]
  0041610A: mov         eax,dword ptr [esp+1Ch]
  0041610E: mov         ecx,dword ptr [esp+14h]
  00416112: sub         edx,edi
  00416114: mov         edi,dword ptr ds:[00E38DACh]
  0041611A: sub         eax,ecx
  0041611C: push        edx
  0041611D: push        eax
  0041611E: mov         eax,dword ptr ds:[00E38DB4h]
  00416123: mov         ebp,dword ptr ds:[00E38DB0h]
  00416129: sub         eax,edi
  0041612B: cdq
  0041612C: sub         eax,edx
  0041612E: sar         eax,1
  00416130: push        eax
  00416131: mov         eax,dword ptr ds:[00E38DB8h]
  00416136: sub         eax,ebp
  00416138: cdq
  00416139: sub         eax,edx
  0041613B: sar         eax,1
  0041613D: push        eax
  0041613E: push        0
  00416140: push        ebx
  00416141: call        dword ptr ds:[0056F180h]
  00416147: jmp         0041635D
  0041614C: mov         edx,dword ptr ds:[00E38E5Ch]
  00416152: xor         ecx,ecx
  00416154: test        edx,edx
  00416156: sete        cl
  00416159: push        esi
  0041615A: mov         dword ptr ds:[005E06B0h],esi
  00416160: mov         dword ptr ds:[00E38E5Ch],ecx
  00416166: call        004D01E0
  0041616B: add         esp,4
  0041616E: call        004D0220
  00416173: mov         esi,dword ptr ds:[0056F1FCh]
  00416179: push        0
  0041617B: mov         dword ptr ds:[005E06B0h],0
  00416185: call        esi
  00416187: mov         edx,dword ptr ds:[00E39140h]
  0041618D: push        edx
  0041618E: call        esi
  00416190: mov         eax,dword ptr ds:[00E39140h]
  00416195: push        eax
  00416196: call        dword ptr ds:[0056F1F4h]
  0041619C: mov         ecx,dword ptr ds:[00E39140h]
  004161A2: push        3
  004161A4: push        ecx
  004161A5: call        dword ptr ds:[0056F178h]
  004161AB: mov         byte ptr ds:[00E38DCAh],1
  004161B2: jmp         0041635D
  004161B7: mov         al,byte ptr ds:[00E38DDAh]
  004161BC: cmp         al,0Fh
  004161BE: je          0041635D
  004161C4: cmp         al,48h
  004161C6: je          0041635D
  004161CC: cmp         al,15h
  004161CE: je          0041635D
  004161D4: cmp         al,0Eh
  004161D6: je          0041635D
  004161DC: push        esi
  004161DD: mov         dword ptr ds:[005E06B0h],esi
  004161E3: call        004D01E0
  004161E8: push        esi
  004161E9: call        004CFDC0
  004161EE: add         esp,8
  004161F1: call        00414BD0
  004161F6: call        004E2AB0
  004161FB: call        004D0220
  00416200: mov         dword ptr ds:[005E06B0h],0
  0041620A: call        00402DA0
  0041620F: mov         byte ptr ds:[00E38DCAh],1
  00416216: jmp         0041635D
  0041621B: mov         al,byte ptr ds:[00E38DDAh]
  00416220: cmp         al,0Fh
  00416222: je          0041635D
  00416228: cmp         al,48h
  0041622A: je          0041635D
  00416230: cmp         al,0Eh
  00416232: je          0041635D
  00416238: push        esi
  00416239: mov         dword ptr ds:[005E06B0h],esi
  0041623F: call        004D01E0
  00416244: push        2
  00416246: jmp         004161E9
  00416248: mov         al,byte ptr ds:[00E38DDAh]
  0041624D: cmp         al,0Fh
  0041624F: je          0041635D
  00416255: cmp         al,48h
  00416257: je          0041635D
  0041625D: cmp         al,0Eh
  0041625F: je          0041635D
  00416265: push        esi
  00416266: mov         dword ptr ds:[005E06B0h],esi
  0041626C: call        004D01E0
  00416271: push        3
  00416273: jmp         004161E9
  00416278: call        00539170
  0041627D: push        0
  0041627F: push        5
  00416281: call        004CC110
  00416286: push        esi
  00416287: push        5
  00416289: call        004CC160
  0041628E: mov         edx,dword ptr ds:[00E20768h]
  00416294: mov         eax,dword ptr ds:[00E20764h]
  00416299: push        4
  0041629B: push        edx
  0041629C: push        eax
  0041629D: call        00538CD0
  004162A2: add         esp,1Ch
  004162A5: cmp         eax,6
  004162A8: mov         byte ptr ds:[00E38E07h],0
  004162AF: jne         004162BF
  004162B1: mov         byte ptr ds:[00D3B43Dh],1
  004162B8: mov         byte ptr ds:[00E38DCAh],1
  004162BF: pop         edi
  004162C0: pop         esi
  004162C1: pop         ebp
  004162C2: xor         eax,eax
  004162C4: pop         ebx
  004162C5: add         esp,7Ch
  004162C8: ret         10h
  004162CB: mov         eax,edi
  004162CD: sub         eax,10h
  004162D0: je          004162E5
  004162D2: dec         eax
  004162D3: jne         0041635D
  004162D9: mov         dword ptr ds:[00E92E20h],0
  004162E3: jmp         0041635D
  004162E5: mov         dword ptr ds:[00E92E24h],0
  004162EF: jmp         0041635D
  004162F1: cmp         esi,204h
  004162F7: ja          00416335
  004162F9: je          0041632A
  004162FB: mov         eax,esi
  004162FD: sub         eax,201h
  00416302: je          00416312
  00416304: dec         eax
  00416305: jne         0041635D
  00416307: mov         cl,byte ptr ds:[00E38E07h]
  0041630D: and         cl,0FEh
  00416310: jmp         00416346
  00416312: mov         cl,byte ptr ds:[00E38E07h]
  00416318: mov         byte ptr ds:[00E38E02h],1
  0041631F: or          cl,1
  00416322: mov         byte ptr ds:[00E38E07h],cl
  00416328: jmp         0041634C
  0041632A: mov         cl,byte ptr ds:[00E38E07h]
  00416330: or          cl,2
  00416333: jmp         00416346
  00416335: cmp         esi,205h
  0041633B: jne         0041635D
  0041633D: mov         cl,byte ptr ds:[00E38E07h]
  00416343: and         cl,0FDh
  00416346: mov         byte ptr ds:[00E38E07h],cl
  0041634C: mov         word ptr ds:[00E38E0Ah],bp
  00416353: shr         ebp,10h
  00416356: mov         word ptr ds:[00E38E08h],bp
  0041635D: mov         ecx,dword ptr [esp+0000009Ch]
  00416364: mov         edx,dword ptr [esp+00000098h]
  0041636B: mov         eax,dword ptr [esp+00000094h]
  00416372: push        ecx
  00416373: mov         ecx,dword ptr [esp+00000094h]
  0041637A: push        edx
  0041637B: push        eax
  0041637C: push        ecx
  0041637D: call        dword ptr ds:[0056F1F8h]
  00416383: pop         edi
  00416384: pop         esi
  00416385: pop         ebp
  00416386: pop         ebx
  00416387: add         esp,7Ch
  0041638A: ret         10h
  0041638D: lea         ecx,[ecx]
  00416390: adc         al,4Fh
  00416392: inc         ecx
  00416393: add         byte ptr [eax+4Fh],ah
  00416396: inc         ecx
  00416397: add         byte ptr [edx+FB00414Eh],cl
  0041639D: dec         ebp
  0041639E: inc         ecx
  0041639F: add         byte ptr [ebx],ch
  004163A1: dec         edi
  004163A2: inc         ecx
  004163A3: add         byte ptr [edi],bl
  004163A5: dec         edi
  004163A6: inc         ecx
  004163A7: add         byte ptr [ebp+63h],bl
  004163AA: inc         ecx
  004163AB: add         byte ptr [eax],al
  004163AD: add         dword ptr [edx],eax
  004163AF: push        es
  004163B0: add         al,byte ptr [ebx]
  004163B2: push        es
  004163B3: push        es
  004163B4: push        es
  004163B5: push        es
  004163B6: push        es
  004163B7: push        es
  004163B8: push        es
  004163B9: push        es
  004163BA: add         al,5
  004163BC: push        ecx
  004163BD: push        eax
  004163BE: inc         ecx
  004163BF: add         byte ptr [edx+CB004158h],bh
  004163C5: bound       eax,qword ptr [ecx]
  004163C8: ret         4150h
  004163CB: add         byte ptr [ecx+50h],dh
  004163CE: inc         ecx
  004163CF: add         byte ptr [ebp+63h],bl
  004163D2: inc         ecx
  004163D3: add         byte ptr [eax],al
  004163D5: add         eax,5050505h
  004163DA: add         eax,5050505h
  004163DF: add         eax,5050505h
  004163E4: add         eax,5050505h
  004163E9: add         eax,5050505h
  004163EE: add         eax,5050505h
  004163F3: add         eax,5050505h
  004163F8: add         eax,5050505h
  004163FD: add         eax,5050505h
  00416402: add         eax,5050505h
  00416407: add         eax,5050505h
  0041640C: add         eax,5050505h
  00416411: add         eax,5050505h
  00416416: add         eax,5050505h
  0041641B: add         eax,5050505h
  00416420: add         eax,5050505h
  00416425: add         eax,5050505h
  0041642A: add         eax,5050505h
  0041642F: add         eax,5050505h
  00416434: add         eax,5050505h
  00416439: add         eax,5050505h
  0041643E: add         eax,5050505h
  00416443: add         eax,5050505h
  00416448: add         eax,5050505h
  0041644D: add         eax,5050505h
  00416452: add         eax,1050505h
  00416457: add         al,byte ptr [ebx]
  00416459: add         eax,50AB9004h
  0041645E: inc         ecx
  0041645F: add         byte ptr [eax+edx*2+50A10041h],cl
  00416466: inc         ecx
  00416467: add         byte ptr [ebp+63h],bl
  0041646A: inc         ecx
  0041646B: add         byte ptr [eax],al
  0041646D: add         eax,dword ptr [ebx]
  0041646F: add         eax,dword ptr [ebx]
  00416471: add         eax,dword ptr [ebx]
  00416473: add         eax,dword ptr [ebx]
  00416475: add         eax,dword ptr [ebx]
  00416477: add         eax,dword ptr [ebx]
  00416479: add         eax,dword ptr [ebx]
  0041647B: add         eax,dword ptr [ebx]
  0041647D: add         eax,dword ptr [ebx]
  0041647F: add         eax,dword ptr [ecx]
  00416481: add         eax,dword ptr [edx]
  00416483: add         eax,dword ptr [ebx]
  00416485: add         eax,dword ptr [ebx]
  00416487: add         eax,dword ptr [ebx]
  00416489: add         eax,dword ptr [ebx]
  0041648B: add         eax,dword ptr [eax]
  0041648D: add         eax,dword ptr [ebx]
  0041648F: add         eax,dword ptr [ebx]
  00416491: add         eax,dword ptr [ebx]
  00416493: add         eax,dword ptr [ebx]
  00416495: add         eax,dword ptr [ebx]
  00416497: add         eax,dword ptr [ebx]
  00416499: add         eax,dword ptr [ebx]
  0041649B: add         eax,dword ptr [ebx]
  0041649D: add         eax,dword ptr [ebx]
  0041649F: add         eax,dword ptr [ebx]
  004164A1: add         eax,dword ptr [edx]
  004164A3: nop
  004164A4: dec         ebx
  004164A5: push        edx
  004164A6: inc         ecx
  004164A7: add         byte ptr [ebx],ch
  004164A9: push        edx
  004164AA: inc         ecx
  004164AB: add         byte ptr [edi+52h],ah
  004164AE: inc         ecx
  004164AF: add         byte ptr [edi+A7004152h],al
  004164B5: push        edx
  004164B6: inc         ecx
  004164B7: add         byte ptr [ebp+63h],bl
  004164BA: inc         ecx
  004164BB: add         byte ptr [eax],al
  004164BD: add         eax,5050105h
  004164C2: add         al,byte ptr ds:[05050505h]
  004164C8: add         eax,5050503h
  004164CD: add         eax,5050504h
  004164D2: add         eax,5050505h
  004164D7: add         eax,5050505h
  004164DC: add         byte ptr ds:[05050105h],al
  004164E2: add         al,byte ptr ds:[05050505h]
  004164E8: add         eax,5050503h
  004164ED: add         eax,53459004h
  004164F2: inc         ecx
  004164F3: add         byte ptr [ecx],cl
  004164F5: push        ebx
  004164F6: inc         ecx
  004164F7: add         byte ptr [ecx],ch
  004164F9: push        ebx
  004164FA: inc         ecx
  004164FB: add         byte ptr [ebp+53h],ah
  004164FE: inc         ecx
  004164FF: add         byte ptr [ebp+5D004153h],al
  00416505: arpl        word ptr [ecx],ax
  00416508: add         byte ptr ds:[05050505h],al
  0041650E: add         eax,5050501h
  00416513: add         eax,5050205h
  00416518: add         eax,5050503h
  0041651D: add         eax,5050504h
  00416522: add         eax,5050505h
  00416527: add         eax,5050500h
  0041652C: add         eax,5010505h
  00416531: add         eax,2050505h
  00416536: add         eax,5030505h
  0041653B: add         eax,90040505h
  00416540: pop         esi
  00416541: push        esp
  00416542: inc         ecx
  00416543: add         byte ptr [edx+F7004158h],al
  00416549: push        ebx
  0041654A: inc         ecx
  0041654B: add         byte ptr [edi],ch
  0041654D: push        esp
  0041654E: inc         ecx
  0041654F: add         byte ptr [ebx],bl
  00416551: push        esp
  00416552: inc         ecx
  00416553: add         byte ptr ds:[E3004158h],bh
  00416559: push        ebx
  0041655A: inc         ecx
  0041655B: add         byte ptr [ebp+63h],bl
  0041655E: inc         ecx
  0041655F: add         byte ptr [eax],al
  00416561: pop         es
  00416562: pop         es
  00416563: add         dword ptr [edi],eax
  00416565: pop         es
  00416566: pop         es
  00416567: pop         es
  00416568: pop         es
  00416569: add         al,byte ptr [ebx]
  0041656B: add         al,7
  0041656D: pop         es
  0041656E: pop         es
  0041656F: add         eax,7070607h
  00416574: pop         es
  00416575: pop         es
  00416576: pop         es
  00416577: pop         es
  00416578: pop         es
  00416579: pop         es
  0041657A: pop         es
  0041657B: pop         es
  0041657C: pop         es
  0041657D: pop         es
  0041657E: pop         es
  0041657F: pop         es
  00416580: add         byte ptr [edi],al
  00416582: pop         es
  00416583: add         dword ptr [edi],eax
  00416585: pop         es
  00416586: pop         es
  00416587: pop         es
  00416588: pop         es
  00416589: add         al,byte ptr [ebx]
  0041658B: add         al,7
  0041658D: pop         es
  0041658E: pop         es
  0041658F: add         eax,0FF8B0607h
  00416594: inc         ebp
  00416595: push        ebx
  00416596: inc         ecx
  00416597: add         byte ptr [ecx],cl
  00416599: push        ebx
  0041659A: inc         ecx
  0041659B: add         byte ptr [ecx],ch
  0041659D: push        ebx
  0041659E: inc         ecx
  0041659F: add         byte ptr [ebp+53h],ah
  004165A2: inc         ecx
  004165A3: add         bl,al
  004165A5: push        esp
  004165A6: inc         ecx
  004165A7: add         byte ptr [ebp+63h],bl
  004165AA: inc         ecx
  004165AB: add         byte ptr [eax],al
  004165AD: add         eax,5050505h
  004165B2: add         eax,5050501h
  004165B7: add         eax,5050205h
  004165BC: add         eax,5050503h
  004165C1: add         eax,5050504h
  004165C6: add         eax,5050505h
  004165CB: add         eax,5050500h
  004165D0: add         eax,5010505h
  004165D5: add         eax,2050505h
  004165DA: add         eax,5030505h
  004165DF: add         eax,90040505h
  004165E4: dec         ebx
  004165E5: push        edx
  004165E6: inc         ecx
  004165E7: add         byte ptr [ebx],ch
  004165E9: push        edx
  004165EA: inc         ecx
  004165EB: add         byte ptr [edi+52h],ah
  004165EE: inc         ecx
  004165EF: add         byte ptr [edi+16004152h],al
  004165F5: push        ebp
  004165F6: inc         ecx
  004165F7: add         byte ptr [ebp+63h],bl
  004165FA: inc         ecx
  004165FB: add         byte ptr [eax],al
  004165FD: add         eax,5050105h
  00416602: add         al,byte ptr ds:[05050505h]
  00416608: add         eax,5050503h
  0041660D: add         eax,5050504h
  00416612: add         eax,5050505h
  00416617: add         eax,5050505h
  0041661C: add         byte ptr ds:[05050105h],al
  00416622: add         al,byte ptr ds:[05050505h]
  00416628: add         eax,5050503h
  0041662D: add         eax,588C9004h
  00416632: inc         ecx
  00416633: add         byte ptr [ebp+58h],bh
  00416636: inc         ecx
  00416637: add         ch,bh
  00416639: push        edi
  0041663A: inc         ecx
  0041663B: add         byte ptr [ebx+58h],bl
  0041663E: inc         ecx
  0041663F: add         byte ptr [edi+58h],al
  00416642: inc         ecx
  00416643: add         byte ptr [edi],dh
  00416645: pop         eax
  00416646: inc         ecx
  00416647: add         bl,dl
  00416649: push        edi
  0041664A: inc         ecx
  0041664B: add         byte ptr [ebp+63h],bl
  0041664E: inc         ecx
  0041664F: add         byte ptr [eax],al
  00416651: pop         es
  00416652: pop         es
  00416653: add         dword ptr [edi],eax
  00416655: pop         es
  00416656: pop         es
  00416657: pop         es
  00416658: pop         es
  00416659: add         al,byte ptr [ebx]
  0041665B: add         al,7
  0041665D: pop         es
  0041665E: pop         es
  0041665F: add         eax,7070607h
  00416664: pop         es
  00416665: pop         es
  00416666: pop         es
  00416667: pop         es
  00416668: pop         es
  00416669: pop         es
  0041666A: pop         es
  0041666B: pop         es
  0041666C: pop         es
  0041666D: pop         es
  0041666E: pop         es
  0041666F: pop         es
  00416670: add         byte ptr [edi],al
  00416672: pop         es
  00416673: add         dword ptr [edi],eax
  00416675: pop         es
  00416676: pop         es
  00416677: pop         es
  00416678: pop         es
  00416679: add         al,byte ptr [ebx]
  0041667B: add         al,7
  0041667D: pop         es
  0041667E: pop         es
  0041667F: add         eax,0FF8B0607h
  00416684: xor         bl,byte ptr [edi+41h]
  00416687: add         byte ptr [edi+ebx*2],bh
  0041668A: inc         ecx
  0041668B: add         ah,ch
  0041668D: pop         eax
  0041668E: inc         ecx
  0041668F: add         cl,ah
  00416691: pop         eax
  00416692: inc         ecx
  00416693: add         byte ptr [eax+62h],bh
  00416696: inc         ecx
  00416697: add         cl,bl
  00416699: pop         esp
  0041669A: inc         ecx
  0041669B: add         ah,bh
  0041669D: pop         ebp
  0041669E: inc         ecx
  0041669F: add         byte ptr [ebx+5Ah],bl
  004166A2: inc         ecx
  004166A3: add         byte ptr ds:[8D00415Ah],dh
  004166A9: pop         edx
  004166AA: inc         ecx
  004166AB: add         byte ptr [ebp+6500415Bh],bh
  004166B1: pop         edx
  004166B2: inc         ecx
  004166B3: add         byte ptr [ebp+0B00415Ah],dh
  004166B9: pop         edi
  004166BA: inc         ecx
  004166BB: add         byte ptr ds:[6300415Fh],dl
  004166C1: pop         edi
  004166C2: inc         ecx
  004166C3: add         byte ptr [ebx],cl
  004166C5: pop         ecx
  004166C6: inc         ecx
  004166C7: add         byte ptr [ebp+59h],dl
  004166CA: inc         ecx
  004166CB: add         byte ptr [ebx+E5004159h],bl
  004166D1: pop         ecx
  004166D2: inc         ecx
  004166D3: add         al,bl
  004166D5: pop         edi
  004166D6: inc         ecx
  004166D7: add         byte ptr ds:[33004160h],al
  004166DD: pushad
  004166DE: inc         ecx
  004166DF: add         byte ptr [eax+60h],bl
  004166E2: inc         ecx
  004166E3: add         byte ptr [edi+60h],ch
  004166E6: inc         ecx
  004166E7: add         byte ptr [edi+9F004160h],al
  004166ED: pushad
  004166EE: inc         ecx
  004166EF: add         byte ptr [ecx+41h],cl
  004166F3: add         byte ptr [edi+1B004161h],dh
  004166F9: bound       eax,qword ptr [ecx]
  004166FC: dec         eax
  004166FD: bound       eax,qword ptr [ecx]
  00416700: neg         dword ptr [eax+41h]
  00416703: add         byte ptr [ebp+63h],bl
  00416706: inc         ecx
  00416707: add         byte ptr [eax],al
  00416709: and         byte ptr [eax],ah
  0041670B: and         byte ptr [eax],ah
  0041670D: add         dword ptr [eax],esp
  0041670F: and         byte ptr [edx],al
  00416711: add         esp,dword ptr [eax]
  00416713: and         byte ptr [eax],ah
  00416715: and         byte ptr [eax],ah
  00416717: and         byte ptr [eax],ah
  00416719: and         byte ptr [eax],ah
  0041671B: add         al,20h
  0041671D: and         byte ptr [eax],ah
  0041671F: and         byte ptr [eax],ah
  00416721: add         eax,9080706h
  00416726: or          cl,byte ptr [ebx]
  00416728: or          al,20h
  0041672A: and         byte ptr [eax],ah
  0041672C: and         byte ptr ds:[2020200Eh],cl
  00416732: and         byte ptr [eax],ah
  00416734: and         byte ptr [eax],ah
  00416736: and         byte ptr [eax],ah
  00416738: and         byte ptr [eax],ah
  0041673A: and         byte ptr [eax],ah
  0041673C: and         byte ptr [eax],ah
  0041673E: and         byte ptr [eax],ah
  00416740: and         byte ptr [edi],cl
  00416742: and         byte ptr [eax],ah
  00416744: and         byte ptr [eax],ah
  00416746: and         byte ptr [eax],dl
  00416748: adc         dword ptr [eax],esp
  0041674A: adc         dl,byte ptr [ebx]
  0041674C: and         byte ptr [eax],ah
  0041674E: and         byte ptr [eax],ah
  00416750: and         byte ptr [eax],ah
  00416752: and         byte ptr [eax],ah
  00416754: and         byte ptr [eax],ah
  00416756: and         byte ptr [eax],ah
  00416758: and         byte ptr [eax],ah
  0041675A: and         byte ptr [eax],ah
  0041675C: and         byte ptr [eax],ah
  0041675E: and         byte ptr [eax],ah
  00416760: and         byte ptr [edi],cl
  00416762: adc         al,15h
  00416764: and         byte ptr [eax],ah
  00416766: and         byte ptr [eax],dl
  00416768: adc         dword ptr [eax],esp
  0041676A: adc         dl,byte ptr [ebx]
  0041676C: and         byte ptr [eax],ah
  0041676E: and         byte ptr [eax],ah
  00416770: push        ss
  00416771: pop         ss
  00416772: sbb         byte ptr [ecx],bl
  00416774: sbb         bl,byte ptr [ebx]
  00416776: sbb         al,1Dh
  00416778: push        ds
  00416779: and         byte ptr [eax],ah
  0041677B: pop         ds
  0041677C: xlat        byte ptr [ebx]
  0041677D: pop         edx
  0041677E: inc         ecx
  0041677F: add         dl,bh
  00416781: pop         edx
  00416782: inc         ecx
  00416783: add         byte ptr [ebx+5Bh],dl
  00416786: inc         ecx
  00416787: add         byte ptr [edi+5Bh],bh
  0041678A: inc         ecx
  0041678B: add         byte ptr [ecx+5Bh],ch
  0041678E: inc         ecx
  0041678F: add         byte ptr [ebp+0000415Bh],dl
  00416795: add         eax,5050501h
  0041679A: add         al,byte ptr ds:[05050205h]
  004167A0: add         eax,5050505h
  004167A5: add         eax,5050505h
  004167AA: add         eax,5050505h
  004167AF: add         eax,5050505h
  004167B4: add         eax,5050505h
  004167B9: add         eax,5030505h
  004167BE: add         eax,5050505h
  004167C3: add         eax,5050505h
  004167C8: add         eax,0DF900405h
  004167CD: pop         ebx
  004167CE: inc         ecx
  004167CF: add         byte ptr [esi],al
  004167D1: pop         esp
  004167D2: inc         ecx
  004167D3: add         byte ptr [ebx+5Ch],ah
  004167D6: inc         ecx
  004167D7: add         byte ptr [edi+7D00415Ch],dl
  004167DD: pop         esp
  004167DE: inc         ecx
  004167DF: add         byte ptr [ecx+0000415Ch],dh
  004167E5: add         eax,5050501h
  004167EA: add         al,byte ptr ds:[05050205h]
  004167F0: add         eax,5050505h
  004167F5: add         eax,5050505h
  004167FA: add         eax,5050505h
  004167FF: add         eax,5050505h
  00416804: add         eax,5050505h
  00416809: add         eax,5030505h
  0041680E: add         eax,5050505h
  00416813: add         eax,5050505h
  00416818: add         eax,0F9900405h
  0041681D: pop         esp
  0041681E: inc         ecx
  0041681F: add         byte ptr [esi+5Dh],dh
  00416822: inc         ecx
  00416823: add         byte ptr [esi+0D00415Dh],ah
  00416829: pop         ebp
  0041682A: inc         ecx
  0041682B: add         byte ptr [ebx+5Dh],dl
  0041682E: inc         ecx
  0041682F: add         byte ptr [eax],dh
  00416831: pop         ebp
  00416832: inc         ecx
  00416833: add         byte ptr [ebp+63h],bl
  00416836: inc         ecx
  00416837: add         byte ptr [eax],al
  00416839: push        es
  0041683A: push        es
  0041683B: push        es
  0041683C: push        es
  0041683D: push        es
  0041683E: add         dword ptr [esi],eax
  00416840: add         al,byte ptr [esi]
  00416842: push        es
  00416843: push        es
  00416844: add         eax,dword ptr [esi]
  00416846: push        es
  00416847: add         eax,dword ptr [esi]
  00416849: push        es
  0041684A: push        es
  0041684B: push        es
  0041684C: push        es
  0041684D: push        es
  0041684E: push        es
  0041684F: push        es
  00416850: push        es
  00416851: push        es
  00416852: push        es
  00416853: push        es
  00416854: push        es
  00416855: push        es
  00416856: push        es
  00416857: push        es
  00416858: push        es
  00416859: push        es
  0041685A: push        es
  0041685B: push        es
  0041685C: push        es
  0041685D: push        es
  0041685E: push        es
  0041685F: push        es
  00416860: push        es
  00416861: push        es
  00416862: push        es
  00416863: push        es
  00416864: push        es
  00416865: push        es
  00416866: add         al,6
  00416868: push        es
  00416869: push        es
  0041686A: push        es
  0041686B: push        es
  0041686C: push        es
  0041686D: push        es
  0041686E: push        es
  0041686F: push        es
  00416870: push        es
  00416871: push        es
  00416872: push        es
  00416873: push        es
  00416874: add         eax,1C00498Dh
  00416879: pop         esi
  0041687A: inc         ecx
  0041687B: add         byte ptr [ecx+B500415Eh],cl
  00416881: pop         esi
  00416882: inc         ecx
  00416883: add         byte ptr [esi+ebx*2],ch
  00416886: inc         ecx
  00416887: add         byte ptr [edx+5Eh],ch
  0041688A: inc         ecx
  0041688B: add         byte ptr [ebx+5Eh],cl
  0041688E: inc         ecx
  0041688F: add         byte ptr [ebp+63h],bl
  00416892: inc         ecx
  00416893: add         byte ptr [eax],al
  00416895: push        es
  00416896: push        es
  00416897: push        es
  00416898: push        es
  00416899: push        es
  0041689A: add         dword ptr [esi],eax
  0041689C: add         al,byte ptr [esi]
  0041689E: push        es
  0041689F: push        es
  004168A0: add         eax,dword ptr [esi]
  004168A2: push        es
  004168A3: add         eax,dword ptr [esi]
  004168A5: push        es
  004168A6: push        es
  004168A7: push        es
  004168A8: push        es
  004168A9: push        es
  004168AA: push        es
  004168AB: push        es
  004168AC: push        es
  004168AD: push        es
  004168AE: push        es
  004168AF: push        es
  004168B0: push        es
  004168B1: push        es
  004168B2: push        es
  004168B3: push        es
  004168B4: push        es
  004168B5: push        es
  004168B6: push        es
  004168B7: push        es
  004168B8: push        es
  004168B9: push        es
  004168BA: push        es
  004168BB: push        es
  004168BC: push        es
  004168BD: push        es
  004168BE: push        es
  004168BF: push        es
  004168C0: push        es
  004168C1: push        es
  004168C2: add         al,6
  004168C4: push        es
  004168C5: push        es
  004168C6: push        es
  004168C7: push        es
  004168C8: push        es
  004168C9: push        es
  004168CA: push        es
  004168CB: push        es
  004168CC: push        es
  004168CD: push        es
  004168CE: push        es
  004168CF: push        es
  004168D0: add         eax,90909090h
  004168D5: nop
  004168D6: nop
  004168D7: nop
  004168D8: nop
  004168D9: nop
  004168DA: nop
  004168DB: nop
  004168DC: nop
  004168DD: nop
  004168DE: nop
  004168DF: nop
  004168E0: mov         al,byte ptr ds:[00E38DDAh]
  004168E5: cmp         al,1Eh
  004168E7: jne         004168F6
  004168E9: mov         ecx,1
  004168EE: mov         byte ptr ds:[00E38DDAh],cl
  004168F4: jmp         00416949
  004168F6: cmp         al,49h
  004168F8: jne         00416907
  004168FA: mov         ecx,1
  004168FF: mov         byte ptr ds:[00E38DDAh],cl
  00416905: jmp         00416949
  00416907: cmp         al,1Fh
  00416909: jne         00416918
  0041690B: mov         ecx,1
  00416910: mov         byte ptr ds:[00E38DDAh],cl
  00416916: jmp         00416949
  00416918: cmp         al,20h
  0041691A: jne         00416929
  0041691C: mov         ecx,1
  00416921: mov         byte ptr ds:[00E38DDAh],cl
  00416927: jmp         00416949
  00416929: cmp         al,12h
  0041692B: je          0041693B
  0041692D: cmp         al,23h
  0041692F: je          0041693B
  00416931: mov         ecx,1
  00416936: cmp         al,cl
  00416938: je          00416949
  0041693A: ret
  0041693B: mov         eax,dword ptr ds:[00D3A6C4h]
  00416940: mov         ecx,1
  00416945: cmp         eax,ecx
  00416947: jne         0041693A
  00416949: mov         eax,dword ptr [esp+4]
  0041694D: mov         edx,dword ptr ds:[00E94E0Ch]
  00416953: cmp         edx,eax
  00416955: jne         00416971
  00416957: mov         dword ptr ds:[00E94E10h],eax
  0041695C: mov         dword ptr ds:[00E94E0Ch],0
  00416966: mov         byte ptr ds:[00E38DCAh],cl
  0041696C: jmp         00547A60
  00416971: mov         dword ptr ds:[00E94E10h],0
  0041697B: mov         dword ptr ds:[00E94E0Ch],eax
  00416980: mov         byte ptr ds:[00E38DCAh],cl
  00416986: jmp         00547A60
  0041698B: nop
  0041698C: nop
  0041698D: nop
  0041698E: nop
  0041698F: nop
  00416990: mov         al,byte ptr ds:[00E38DDAh]
  00416995: cmp         al,1Eh
  00416997: jne         004169A2
  00416999: mov         byte ptr ds:[00E38DDAh],1
  004169A0: jmp         00416A08
  004169A2: cmp         al,49h
  004169A4: jne         004169AF
  004169A6: mov         byte ptr ds:[00E38DDAh],1
  004169AD: jmp         00416A08
  004169AF: cmp         al,1Fh
  004169B1: jne         004169BC
  004169B3: mov         byte ptr ds:[00E38DDAh],1
  004169BA: jmp         00416A08
  004169BC: cmp         al,20h
  004169BE: jne         004169C9
  004169C0: mov         byte ptr ds:[00E38DDAh],1
  004169C7: jmp         00416A08
  004169C9: cmp         al,7
  004169CB: jne         00416A04
  004169CD: mov         eax,dword ptr [esp+4]
  004169D1: mov         ecx,dword ptr ds:[00605A44h]
  004169D7: cmp         ecx,eax
  004169D9: jne         004169FA
  004169DB: mov         al,1
  004169DD: mov         byte ptr ds:[00E38DDAh],al
  004169E2: mov         byte ptr ds:[00E38DCAh],al
  004169E7: mov         eax,dword ptr ds:[00EAAC28h]
  004169EC: imul        eax,eax,49D8h
  004169F2: mov         byte ptr [eax+00EA623Ch],0
  004169F9: ret
  004169FA: mov         dword ptr ds:[00D3B198h],eax
  004169FF: jmp         0040A0E0
  00416A04: cmp         al,1
  00416A06: jne         004169F9
  00416A08: mov         ecx,dword ptr [esp+4]
  00416A0C: mov         dword ptr ds:[00D3B198h],ecx
  00416A12: jmp         0040A0E0
  00416A17: nop
  00416A18: nop
  00416A19: nop
  00416A1A: nop
  00416A1B: nop
  00416A1C: nop
  00416A1D: nop
  00416A1E: nop
  00416A1F: nop
  00416A20: mov         al,byte ptr ds:[00E38DDAh]
  00416A25: push        ebx
  00416A26: mov         ebx,1
  00416A2B: cmp         al,bl
  00416A2D: jne         00416AB9
  00416A33: mov         ecx,dword ptr ds:[00578C6Ch]
  00416A39: xor         edx,edx
  00416A3B: push        esi
  00416A3C: mov         dword ptr ds:[00578C6Ch],edx
  00416A42: mov         esi,6
  00416A47: inc         ecx
  00416A48: cmp         ecx,6
  00416A4B: jle         00416A4F
  00416A4D: mov         ecx,ebx
  00416A4F: lea         eax,[ecx+ecx*8]
  00416A52: shl         eax,4
  00416A55: cmp         byte ptr [eax+00BCB9C8h],bl
  00416A5B: jne         00416A75
  00416A5D: cmp         byte ptr [eax+00BCBA31h],0
  00416A64: jne         00416A75
  00416A66: cmp         byte ptr [eax+00BCBA0Ah],0
  00416A6D: je          00416A75
  00416A6F: test        edx,edx
  00416A71: jne         00416A75
  00416A73: mov         edx,ecx
  00416A75: dec         esi
  00416A76: jne         00416A47
  00416A78: test        edx,edx
  00416A7A: mov         dword ptr ds:[00578C6Ch],edx
  00416A80: pop         esi
  00416A81: jle         00416AB9
  00416A83: lea         eax,[edx+edx*8]
  00416A86: shl         eax,4
  00416A89: movsx       ecx,word ptr [eax+00BCB9F8h]
  00416A90: lea         edx,[ecx+ecx*8]
  00416A93: lea         ecx,[ecx+edx*2]
  00416A96: movsx       edx,word ptr [eax+00BCB9F6h]
  00416A9D: mov         eax,dword ptr ds:[00BCA2D8h]
  00416AA2: lea         ecx,[ecx+ecx*2]
  00416AA5: add         eax,edx
  00416AA7: lea         ecx,[eax+ecx*4]
  00416AAA: push        ecx
  00416AAB: call        00430CD0
  00416AB0: add         esp,4
  00416AB3: mov         byte ptr ds:[00E38DCAh],bl
  00416AB9: pop         ebx
  00416ABA: ret
  00416ABB: nop
  00416ABC: nop
  00416ABD: nop
  00416ABE: nop
  00416ABF: nop
  00416AC0: mov         al,byte ptr ds:[00E38DDAh]
  00416AC5: cmp         al,1Eh
  00416AC7: jne         00416AD2
  00416AC9: mov         byte ptr ds:[00E38DDAh],1
  00416AD0: jmp         00416AE3
  00416AD2: cmp         al,49h
  00416AD4: jne         00416ADF
  00416AD6: mov         byte ptr ds:[00E38DDAh],1
  00416ADD: jmp         00416AE3
  00416ADF: cmp         al,1
  00416AE1: jne         00416B0C
  00416AE3: mov         eax,dword ptr [esp+4]
  00416AE7: test        eax,eax
  00416AE9: jl          00416B0C
  00416AEB: cmp         eax,4
  00416AEE: jg          00416B0C
  00416AF0: mov         eax,dword ptr [eax*8+00E94DDCh]
  00416AF7: cmp         eax,0FFFFFFFFh
  00416AFA: jle         00416B0C
  00416AFC: push        eax
  00416AFD: call        00430CD0
  00416B02: add         esp,4
  00416B05: mov         byte ptr ds:[00E38DCAh],1
  00416B0C: ret
  00416B0D: nop
  00416B0E: nop
  00416B0F: nop
  00416B10: mov         al,byte ptr ds:[00E38DDAh]
  00416B15: push        esi
  00416B16: cmp         al,1Eh
  00416B18: jne         00416B23
  00416B1A: mov         byte ptr ds:[00E38DDAh],1
  00416B21: jmp         00416B4E
  00416B23: cmp         al,49h
  00416B25: jne         00416B30
  00416B27: mov         byte ptr ds:[00E38DDAh],1
  00416B2E: jmp         00416B4E
  00416B30: cmp         al,1Fh
  00416B32: jne         00416B3D
  00416B34: mov         byte ptr ds:[00E38DDAh],1
  00416B3B: jmp         00416B4E
  00416B3D: cmp         al,20h
  00416B3F: jne         00416B4A
  00416B41: mov         byte ptr ds:[00E38DDAh],1
  00416B48: jmp         00416B4E
  00416B4A: cmp         al,1
  00416B4C: jne         00416B9D
  00416B4E: mov         esi,dword ptr [esp+8]
  00416B52: test        esi,esi
  00416B54: jl          00416B9D
  00416B56: cmp         esi,4
  00416B59: jg          00416B9D
  00416B5B: mov         eax,dword ptr ds:[0070E460h]
  00416B60: mov         ecx,dword ptr ds:[00BCA2C0h]
  00416B66: cdq
  00416B67: sub         eax,edx
  00416B69: push        edi
  00416B6A: sar         eax,1
  00416B6C: add         eax,ecx
  00416B6E: lea         ecx,[eax+eax*8]
  00416B71: lea         edx,[eax+ecx*2]
  00416B74: lea         ecx,[eax+edx*4]
  00416B77: mov         eax,dword ptr ds:[00BCA2BCh]
  00416B7C: lea         edi,[ecx+eax]
  00416B7F: mov         eax,dword ptr ds:[0070E464h]
  00416B84: cdq
  00416B85: sub         eax,edx
  00416B87: lea         ecx,[edi+ecx*2]
  00416B8A: sar         eax,1
  00416B8C: add         eax,ecx
  00416B8E: pop         edi
  00416B8F: mov         edx,dword ptr [eax*4+0070E494h]
  00416B96: mov         dword ptr [esi*8+00E94DDCh],edx
  00416B9D: pop         esi
  00416B9E: ret
  00416B9F: nop
  00416BA0: cmp         byte ptr ds:[00E38DDAh],1
  00416BA7: jne         00416BB7
  00416BA9: mov         eax,dword ptr [esp+4]
  00416BAD: mov         dword ptr ds:[00D3B198h],eax
  00416BB2: jmp         00403320
  00416BB7: ret
  00416BB8: nop
  00416BB9: nop
  00416BBA: nop
  00416BBB: nop
  00416BBC: nop
  00416BBD: nop
  00416BBE: nop
  00416BBF: nop
  00416BC0: mov         eax,dword ptr ds:[006FDC38h]
  00416BC5: test        eax,eax
  00416BC7: je          00416C10
  00416BC9: push        eax
  00416BCA: call        00560560
  00416BCF: mov         eax,dword ptr ds:[006FDC3Ch]
  00416BD4: add         esp,4
  00416BD7: test        eax,eax
  00416BD9: mov         dword ptr ds:[006FDC40h],0
  00416BE3: je          00416BEE
  00416BE5: push        eax
  00416BE6: call        005605C0
  00416BEB: add         esp,4
  00416BEE: mov         eax,dword ptr ds:[006FDC38h]
  00416BF3: test        eax,eax
  00416BF5: je          00416C00
  00416BF7: push        eax
  00416BF8: call        00560580
  00416BFD: add         esp,4
  00416C00: push        edi
  00416C01: mov         ecx,44h
  00416C06: xor         eax,eax
  00416C08: mov         edi,6FDB38h
  00416C0D: rep stos    dword ptr es:[edi]
  00416C0F: pop         edi
  00416C10: ret
  00416C11: nop
  00416C12: nop
  00416C13: nop
  00416C14: nop
  00416C15: nop
  00416C16: nop
  00416C17: nop
  00416C18: nop
  00416C19: nop
  00416C1A: nop
  00416C1B: nop
  00416C1C: nop
  00416C1D: nop
  00416C1E: nop
  00416C1F: nop
  00416C20: push        0FFFFFFFFh
  00416C22: push        56B626h
  00416C27: mov         eax,dword ptr fs:[00000000h]
  00416C2D: push        eax
  00416C2E: mov         dword ptr fs:[00000000h],esp
  00416C35: sub         esp,38h
  00416C38: push        ebx
  00416C39: push        esi
  00416C3A: mov         esi,ecx
  00416C3C: push        edi
  00416C3D: mov         dword ptr [esp+0Ch],esi
  00416C41: call        004E5210
  00416C46: push        51F780h
  00416C4B: push        4E50F0h
  00416C50: lea         edi,[esi+4Ch]
  00416C53: push        9
  00416C55: push        34h
  00416C57: push        edi
  00416C58: mov         dword ptr [esp+60h],0
  00416C60: call        00561441
  00416C65: push        0
  00416C67: push        5
  00416C69: mov         ebx,1
  00416C6E: push        0FFFFFFF9h
  00416C70: push        0FFFFFFF9h
  00416C72: lea         ecx,[esp+20h]
  00416C76: mov         byte ptr [esp+5Ch],bl
  00416C7A: mov         dword ptr [esi],56F4DCh
  00416C80: mov         dword ptr [esi+00000220h],5793DCh
  00416C8A: mov         dword ptr [esi+00000224h],5793D0h
  00416C94: mov         dword ptr [esi+00000228h],5793C4h
  00416C9E: mov         dword ptr [esi+0000022Ch],5793B8h
  00416CA8: mov         dword ptr [esi+00000230h],5793ACh
  00416CB2: mov         dword ptr [esi+00000234h],5793A0h
  00416CBC: mov         dword ptr [esi+00000238h],579394h
  00416CC6: mov         dword ptr [esi+0000023Ch],579388h
  00416CD0: mov         dword ptr [esi+00000240h],57937Ch
  00416CDA: mov         dword ptr [esi+00000244h],579370h
  00416CE4: mov         dword ptr [esi+00000248h],579364h
  00416CEE: mov         dword ptr [esi+0000024Ch],579358h
  00416CF8: mov         dword ptr [esi+00000250h],57934Ch
  00416D02: mov         dword ptr [esi+00000254h],579340h
  00416D0C: mov         dword ptr [esi+00000258h],579334h
  00416D16: mov         dword ptr [esi+0000025Ch],579328h
  00416D20: mov         dword ptr [esi+00000260h],57931Ch
  00416D2A: mov         dword ptr [esi+00000264h],579310h
  00416D34: mov         dword ptr [esi+00000268h],579304h
  00416D3E: mov         dword ptr [esi+0000026Ch],5792F8h
  00416D48: call        004E5120
  00416D4D: push        eax
  00416D4E: mov         ecx,edi
  00416D50: mov         byte ptr [esp+50h],2
  00416D55: call        004E5140
  00416D5A: lea         ecx,[esp+10h]
  00416D5E: mov         byte ptr [esp+4Ch],bl
  00416D62: call        0051F780
  00416D67: push        ebx
  00416D68: push        5
  00416D6A: push        0FFFFFFFEh
  00416D6C: push        0FFFFFFF9h
  00416D6E: lea         ecx,[esp+20h]
  00416D72: call        004E5120
  00416D77: push        eax
  00416D78: lea         ecx,[esi+00000080h]
  00416D7E: mov         byte ptr [esp+50h],3
  00416D83: call        004E5140
  00416D88: lea         ecx,[esp+10h]
  00416D8C: mov         byte ptr [esp+4Ch],bl
  00416D90: call        0051F780
  00416D95: push        2
  00416D97: push        5
  00416D99: push        3
  00416D9B: push        0FFFFFFF9h
  00416D9D: lea         ecx,[esp+20h]
  00416DA1: call        004E5120
  00416DA6: push        eax
  00416DA7: lea         ecx,[esi+000000B4h]
  00416DAD: mov         byte ptr [esp+50h],4
  00416DB2: call        004E5140
  00416DB7: lea         ecx,[esp+10h]
  00416DBB: mov         byte ptr [esp+4Ch],bl
  00416DBF: call        0051F780
  00416DC4: push        3
  00416DC6: push        5
  00416DC8: push        0FFFFFFF9h
  00416DCA: push        0FFFFFFFEh
  00416DCC: lea         ecx,[esp+20h]
  00416DD0: call        004E5120
  00416DD5: push        eax
  00416DD6: lea         ecx,[esi+000000E8h]
  00416DDC: mov         byte ptr [esp+50h],5
  00416DE1: call        004E5140
  00416DE6: lea         ecx,[esp+10h]
  00416DEA: mov         byte ptr [esp+4Ch],bl
  00416DEE: call        0051F780
  00416DF3: push        4
  00416DF5: push        5
  00416DF7: push        0FFFFFFFEh
  00416DF9: push        0FFFFFFFEh
  00416DFB: lea         ecx,[esp+20h]
  00416DFF: call        004E5120
  00416E04: push        eax
  00416E05: lea         ecx,[esi+0000011Ch]
  00416E0B: mov         byte ptr [esp+50h],6
  00416E10: call        004E5140
  00416E15: lea         ecx,[esp+10h]
  00416E19: mov         byte ptr [esp+4Ch],bl
  00416E1D: call        0051F780
  00416E22: push        5
  00416E24: push        5
  00416E26: push        3
  00416E28: push        0FFFFFFFEh
  00416E2A: lea         ecx,[esp+20h]
  00416E2E: call        004E5120
  00416E33: push        eax
  00416E34: lea         ecx,[esi+00000150h]
  00416E3A: mov         byte ptr [esp+50h],7
  00416E3F: call        004E5140
  00416E44: lea         ecx,[esp+10h]
  00416E48: mov         byte ptr [esp+4Ch],bl
  00416E4C: call        0051F780
  00416E51: push        6
  00416E53: push        5
  00416E55: push        0FFFFFFF9h
  00416E57: push        3
  00416E59: lea         ecx,[esp+20h]
  00416E5D: call        004E5120
  00416E62: push        eax
  00416E63: lea         ecx,[esi+00000184h]
  00416E69: mov         byte ptr [esp+50h],8
  00416E6E: call        004E5140
  00416E73: lea         ecx,[esp+10h]
  00416E77: mov         byte ptr [esp+4Ch],bl
  00416E7B: call        0051F780
  00416E80: push        7
  00416E82: push        5
  00416E84: push        0FFFFFFFEh
  00416E86: push        3
  00416E88: lea         ecx,[esp+20h]
  00416E8C: call        004E5120
  00416E91: push        eax
  00416E92: lea         ecx,[esi+000001B8h]
  00416E98: mov         byte ptr [esp+50h],9
  00416E9D: call        004E5140
  00416EA2: lea         ecx,[esp+10h]
  00416EA6: mov         byte ptr [esp+4Ch],bl
  00416EAA: call        0051F780
  00416EAF: push        8
  00416EB1: push        5
  00416EB3: push        3
  00416EB5: push        3
  00416EB7: lea         ecx,[esp+20h]
  00416EBB: call        004E5120
  00416EC0: push        eax
  00416EC1: lea         ecx,[esi+000001ECh]
  00416EC7: mov         byte ptr [esp+50h],0Ah
  00416ECC: call        004E5140
  00416ED1: lea         ecx,[esp+10h]
  00416ED5: mov         byte ptr [esp+4Ch],bl
  00416ED9: call        0051F780
  00416EDE: mov         ecx,dword ptr [esp+44h]
  00416EE2: mov         dword ptr [esi+4],2
  00416EE9: mov         eax,esi
  00416EEB: pop         edi
  00416EEC: pop         esi
  00416EED: pop         ebx
  00416EEE: mov         dword ptr fs:[00000000h],ecx
  00416EF5: add         esp,44h
  00416EF8: ret
  00416EF9: nop
  00416EFA: nop
  00416EFB: nop
  00416EFC: nop
  00416EFD: nop
  00416EFE: nop
  00416EFF: nop
  00416F00: push        0FFFFFFFFh
  00416F02: push        56B638h
  00416F07: mov         eax,dword ptr fs:[00000000h]
  00416F0D: push        eax
  00416F0E: mov         dword ptr fs:[00000000h],esp
  00416F15: push        ecx
  00416F16: push        esi
  00416F17: mov         esi,ecx
  00416F19: mov         dword ptr [esp+4],esi
  00416F1D: mov         dword ptr [esi],56F4DCh
  00416F23: push        51F780h
  00416F28: push        9
  00416F2A: lea         eax,[esi+4Ch]
  00416F2D: push        34h
  00416F2F: push        eax
  00416F30: mov         dword ptr [esp+20h],0
  00416F38: call        00561363
  00416F3D: mov         ecx,esi
  00416F3F: mov         dword ptr [esp+10h],0FFFFFFFFh
  00416F47: call        004E5290
  00416F4C: mov         ecx,dword ptr [esp+8]
  00416F50: pop         esi
  00416F51: mov         dword ptr fs:[00000000h],ecx
  00416F58: add         esp,10h
  00416F5B: ret
  00416F5C: nop
  00416F5D: nop
  00416F5E: nop
  00416F5F: nop

  Summary

      16E000 .text
