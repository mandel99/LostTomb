Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  00539250: sub         esp,14Ch
  00539256: push        ebp
  00539257: push        esi
  00539258: push        5F90A0h
  0053925D: mov         dword ptr [esp+28h],ecx
  00539261: push        5F9094h
  00539266: call        00561A0A
  0053926B: mov         esi,eax
  0053926D: xor         ebp,ebp
  0053926F: add         esp,8
  00539272: cmp         esi,ebp
  00539274: mov         dword ptr [esp+14h],esi
  00539278: je          005394E4
  0053927E: mov         eax,dword ptr ds:[005F908Ch]
  00539283: mov         cl,byte ptr ds:[005F9090h]
  00539289: push        esi
  0053928A: lea         edx,[esp+00000090h]
  00539291: push        0C8h
  00539296: push        edx
  00539297: mov         dword ptr [esp+14h],eax
  0053929B: mov         byte ptr [esp+18h],cl
  0053929F: mov         dword ptr [esp+2Ch],ebp
  005392A3: mov         dword ptr [esp+28h],1
  005392AB: mov         dword ptr [esp+24h],64h
  005392B3: call        00561E4D
  005392B8: add         esp,0Ch
  005392BB: test        eax,eax
  005392BD: je          005394DB
  005392C3: push        ebx
  005392C4: push        edi
  005392C5: mov         al,byte ptr [esp+00000094h]
  005392CC: cmp         al,3Bh
  005392CE: je          005394BB
  005392D4: test        al,al
  005392D6: je          005394BB
  005392DC: lea         eax,[esp+10h]
  005392E0: lea         ecx,[esp+00000094h]
  005392E7: push        eax
  005392E8: push        ecx
  005392E9: call        00561DB1
  005392EE: mov         edi,eax
  005392F0: add         esp,8
  005392F3: test        edi,edi
  005392F5: je          005394BB
  005392FB: xor         ebx,ebx
  005392FD: mov         esi,5F9084h
  00539302: mov         ecx,edi
  00539304: mov         al,byte ptr [ecx]
  00539306: mov         dl,al
  00539308: cmp         al,byte ptr [esi]
  0053930A: jne         00539328
  0053930C: test        dl,dl
  0053930E: je          00539324
  00539310: mov         al,byte ptr [ecx+1]
  00539313: mov         dl,al
  00539315: cmp         al,byte ptr [esi+1]
  00539318: jne         00539328
  0053931A: add         ecx,2
  0053931D: add         esi,2
  00539320: test        dl,dl
  00539322: jne         00539304
  00539324: xor         ecx,ecx
  00539326: jmp         0053932D
  00539328: sbb         ecx,ecx
  0053932A: sbb         ecx,0FFFFFFFFh
  0053932D: test        ecx,ecx
  0053932F: jne         0053936A
  00539331: lea         eax,[ebp+ebp*2]
  00539335: lea         ecx,[eax+eax*8]
  00539338: lea         eax,[ebp+ebp*2]
  0053933C: shl         ecx,4
  0053933F: add         ecx,ebp
  00539341: lea         edx,[eax+eax*8]
  00539344: mov         eax,dword ptr [esp+2Ch]
  00539348: shl         ecx,4
  0053934B: shl         edx,4
  0053934E: mov         ecx,dword ptr [ecx+eax+00001B08h]
  00539355: add         edx,ebp
  00539357: lea         edx,[ecx+edx*4]
  0053935A: mov         dword ptr [eax+edx*4+00001A40h],1
  00539365: jmp         005394B7
  0053936A: lea         eax,[esp+10h]
  0053936E: push        eax
  0053936F: push        0
  00539371: call        00561DB1
  00539376: add         esp,8
  00539379: mov         esi,5F9080h
  0053937E: mov         dl,byte ptr [eax]
  00539380: mov         cl,dl
  00539382: cmp         dl,byte ptr [esi]
  00539384: jne         005393A2
  00539386: test        cl,cl
  00539388: je          0053939E
  0053938A: mov         dl,byte ptr [eax+1]
  0053938D: mov         cl,dl
  0053938F: cmp         dl,byte ptr [esi+1]
  00539392: jne         005393A2
  00539394: add         eax,2
  00539397: add         esi,2
  0053939A: test        cl,cl
  0053939C: jne         0053937E
  0053939E: xor         eax,eax
  005393A0: jmp         005393A7
  005393A2: sbb         eax,eax
  005393A4: sbb         eax,0FFFFFFFFh
  005393A7: test        eax,eax
  005393A9: jne         005393B0
  005393AB: mov         ebx,1
  005393B0: lea         eax,[esp+10h]
  005393B4: push        eax
  005393B5: push        0
  005393B7: call        00561DB1
  005393BC: add         esp,8
  005393BF: test        eax,eax
  005393C1: je          005393D5
  005393C3: lea         ecx,[esp+18h]
  005393C7: push        0Ah
  005393C9: push        ecx
  005393CA: push        eax
  005393CB: call        00562DCB
  005393D0: add         esp,0Ch
  005393D3: mov         ebp,eax
  005393D5: lea         edx,[esp+10h]
  005393D9: push        edx
  005393DA: push        0
  005393DC: call        00561DB1
  005393E1: add         esp,8
  005393E4: test        eax,eax
  005393E6: je          005393FC
  005393E8: lea         ecx,[esp+18h]
  005393EC: push        0Ah
  005393EE: push        ecx
  005393EF: push        eax
  005393F0: call        00562DCB
  005393F5: add         esp,0Ch
  005393F8: mov         dword ptr [esp+28h],eax
  005393FC: lea         edx,[esp+10h]
  00539400: push        edx
  00539401: push        0
  00539403: call        00561DB1
  00539408: add         esp,8
  0053940B: test        eax,eax
  0053940D: je          00539423
  0053940F: lea         ecx,[esp+18h]
  00539413: push        0Ah
  00539415: push        ecx
  00539416: push        eax
  00539417: call        00562DCB
  0053941C: add         esp,0Ch
  0053941F: mov         dword ptr [esp+24h],eax
  00539423: lea         edx,[esp+10h]
  00539427: push        edx
  00539428: push        0
  0053942A: call        00561DB1
  0053942F: add         esp,8
  00539432: test        eax,eax
  00539434: je          0053944A
  00539436: lea         ecx,[esp+18h]
  0053943A: push        0Ah
  0053943C: push        ecx
  0053943D: push        eax
  0053943E: call        00562DCB
  00539443: add         esp,0Ch
  00539446: mov         dword ptr [esp+20h],eax
  0053944A: push        edi
  0053944B: lea         edx,[esp+34h]
  0053944F: push        5F9070h
  00539454: push        edx
  00539455: call        00561A36
  0053945A: lea         eax,[esp+3Ch]
  0053945E: push        5763BCh
  00539463: push        eax
  00539464: call        00561A0A
  00539469: add         esp,14h
  0053946C: test        eax,eax
  0053946E: jne         0053948E
  00539470: xor         ecx,ecx
  00539472: push        edi
  00539473: mov         cl,byte ptr ds:[00E38E62h]
  00539479: lea         edx,[esp+34h]
  0053947D: push        ecx
  0053947E: push        5F905Ch
  00539483: push        edx
  00539484: call        00561A36
  00539489: add         esp,10h
  0053948C: jmp         00539497
  0053948E: push        eax
  0053948F: call        00561994
  00539494: add         esp,4
  00539497: mov         eax,dword ptr [esp+20h]
  0053949B: mov         ecx,dword ptr [esp+24h]
  0053949F: mov         edx,dword ptr [esp+28h]
  005394A3: push        ebx
  005394A4: push        eax
  005394A5: push        ecx
  005394A6: push        edx
  005394A7: lea         eax,[esp+40h]
  005394AB: push        ebp
  005394AC: push        eax
  005394AD: mov         ecx,0E39180h
  005394B2: call        005395E0
  005394B7: mov         esi,dword ptr [esp+1Ch]
  005394BB: push        esi
  005394BC: lea         ecx,[esp+00000098h]
  005394C3: push        0C8h
  005394C8: push        ecx
  005394C9: call        00561E4D
  005394CE: add         esp,0Ch
  005394D1: test        eax,eax
  005394D3: jne         005392C5
  005394D9: pop         edi
  005394DA: pop         ebx
  005394DB: push        esi
  005394DC: call        00561994
  005394E1: add         esp,4
  005394E4: pop         esi
  005394E5: pop         ebp
  005394E6: add         esp,14Ch
  005394EC: ret
  005394ED: nop
  005394EE: nop
  005394EF: nop
  005394F0: B9

  Summary

      16E000 .text
