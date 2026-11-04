
Dump of file build\c-only-debug\Debug\restore_link_check.exe

File Type: EXECUTABLE IMAGE

_FUN_00426b40:
  00276CC0: push        ebp
  00276CC1: mov         ebp,esp
  00276CC3: sub         esp,78h
  00276CC6: push        esi
  00276CC7: push        edi
  00276CC8: lea         edi,[ebp-78h]
  00276CCB: mov         ecx,1Eh
  00276CD0: mov         eax,0CCCCCCCCh
  00276CD5: rep stos    dword ptr es:[edi]
  00276CD7: mov         eax,dword ptr [___security_cookie]
  00276CDC: xor         eax,ebp
  00276CDE: mov         dword ptr [ebp-4],eax
  00276CE1: call        @ILT+28745(_restore_graphics_platform)
  00276CE6: mov         dword ptr [ebp-8],eax
  00276CE9: mov         esi,esp
  00276CEB: push        0
  00276CED: mov         eax,dword ptr [ebp-8]
  00276CF0: mov         ecx,dword ptr [eax]
  00276CF2: call        ecx
  00276CF4: cmp         esi,esp
  00276CF6: call        @ILT+18365(__RTC_CheckEsp)
  00276CFB: mov         dword ptr [ebp-0Ch],eax
  00276CFE: xor         edx,edx
  00276D00: mov         dword ptr [ebp-4Ch],edx
  00276D03: mov         dword ptr [ebp-48h],edx
  00276D06: mov         dword ptr [ebp-44h],edx
  00276D09: mov         dword ptr [ebp-40h],edx
  00276D0C: mov         esi,esp
  00276D0E: push        18h
  00276D10: mov         eax,dword ptr [ebp-0Ch]
  00276D13: push        eax
  00276D14: mov         ecx,dword ptr [ebp-8]
  00276D17: mov         edx,dword ptr [ecx+4]
  00276D1A: call        edx
  00276D1C: cmp         esi,esp
  00276D1E: call        @ILT+18365(__RTC_CheckEsp)
  00276D23: mov         esi,esp
  00276D25: push        0Eh
  00276D27: mov         eax,dword ptr [ebp-0Ch]
  00276D2A: push        eax
  00276D2B: mov         ecx,dword ptr [ebp-8]
  00276D2E: mov         edx,dword ptr [ecx+4]
  00276D31: call        edx
  00276D33: cmp         esi,esp
  00276D35: call        @ILT+18365(__RTC_CheckEsp)
  00276D3A: mov         dword ptr [ebp-58h],eax
  00276D3D: mov         esi,esp
  00276D3F: push        0Ch
  00276D41: mov         eax,dword ptr [ebp-0Ch]
  00276D44: push        eax
  00276D45: mov         ecx,dword ptr [ebp-8]
  00276D48: mov         edx,dword ptr [ecx+4]
  00276D4B: call        edx
  00276D4D: cmp         esi,esp
  00276D4F: call        @ILT+18365(__RTC_CheckEsp)
  00276D54: mov         dword ptr [ebp-5Ch],eax
  00276D57: mov         eax,dword ptr [ebp-58h]
  00276D5A: imul        eax,dword ptr [ebp-5Ch]
  00276D5E: mov         dword ptr ds:[00E38E58h],eax
  00276D63: mov         esi,esp
  00276D65: mov         ecx,dword ptr [ebp-0Ch]
  00276D68: push        ecx
  00276D69: push        0
  00276D6B: mov         edx,dword ptr [ebp-8]
  00276D6E: mov         eax,dword ptr [edx+8]
  00276D71: call        eax
  00276D73: cmp         esi,esp
  00276D75: call        @ILT+18365(__RTC_CheckEsp)
  00276D7A: mov         esi,esp
  00276D7C: push        0
  00276D7E: mov         ecx,dword ptr [ebp-8]
  00276D81: mov         edx,dword ptr [ecx+0Ch]
  00276D84: call        edx
  00276D86: cmp         esi,esp
  00276D88: call        @ILT+18365(__RTC_CheckEsp)
  00276D8D: mov         dword ptr ds:[00E38DB8h],eax
  00276D92: mov         esi,esp
  00276D94: push        1
  00276D96: mov         eax,dword ptr [ebp-8]
  00276D99: mov         ecx,dword ptr [eax+0Ch]
  00276D9C: call        ecx
  00276D9E: cmp         esi,esp
  00276DA0: call        @ILT+18365(__RTC_CheckEsp)
  00276DA5: mov         dword ptr ds:[00E38DB4h],eax
  00276DAA: mov         edx,dword ptr ds:[0E38DB8h]
  00276DB0: cmp         edx,dword ptr ds:[0E38DB0h]
  00276DB6: jge         00276DDB
  00276DB8: cmp         dword ptr ds:[0E38E5Ch],0
  00276DBF: jne         00276DCF
  00276DC1: push        0
  00276DC3: push        0
  00276DC5: push        39h
  00276DC7: call        @ILT+14700(_FUN_004e2970)
  00276DCC: add         esp,0Ch
  00276DCF: mov         dword ptr ds:[0E38E5Ch],1
  00276DD9: jmp         00276E16
  00276DDB: cmp         dword ptr ds:[0E38E5Ch],0
  00276DE2: jne         00276E16
  00276DE4: cmp         dword ptr ds:[0E38E58h],10h
  00276DEB: je          00276E16
  00276DED: push        0
  00276DEF: push        0
  00276DF1: push        39h
  00276DF3: call        @ILT+14700(_FUN_004e2970)
  00276DF8: add         esp,0Ch
  00276DFB: push        0
  00276DFD: push        0
  00276DFF: push        5D1CD4h
  00276E04: call        @ILT+16925(_FUN_004ce6e0)
  00276E09: add         esp,0Ch
  00276E0C: mov         dword ptr ds:[0E38E5Ch],1
  00276E16: cmp         dword ptr ds:[0E38E5Ch],0
  00276E1D: je          00276E3F
  00276E1F: mov         esi,esp
  00276E21: push        70D508h
  00276E26: push        100h
  00276E2B: push        0
  00276E2D: push        0
  00276E2F: mov         eax,dword ptr [ebp-8]
  00276E32: mov         ecx,dword ptr [eax+10h]
  00276E35: call        ecx
  00276E37: cmp         esi,esp
  00276E39: call        @ILT+18365(__RTC_CheckEsp)
  00276E3E: nop
  00276E3F: mov         edx,dword ptr ds:[0E39148h]
  00276E45: mov         dword ptr [ebp-14h],edx
  00276E48: cmp         dword ptr [ebp-14h],0
  00276E4C: jne         00276E8A
  00276E4E: mov         esi,esp
  00276E50: push        0
  00276E52: lea         eax,[ebp-14h]
  00276E55: push        eax
  00276E56: push        0
  00276E58: mov         ecx,dword ptr [ebp-8]
  00276E5B: mov         edx,dword ptr [ecx+14h]
  00276E5E: call        edx
  00276E60: cmp         esi,esp
  00276E62: call        @ILT+18365(__RTC_CheckEsp)
  00276E67: mov         dword ptr [ebp-1Ch],eax
  00276E6A: cmp         dword ptr [ebp-1Ch],0
  00276E6E: je          00276E8A
  00276E70: mov         eax,dword ptr [ebp-1Ch]
  00276E73: push        eax
  00276E74: push        0
  00276E76: push        5D1CB8h
  00276E7B: call        @ILT+16925(_FUN_004ce6e0)
  00276E80: add         esp,0Ch
  00276E83: xor         eax,eax
  00276E85: jmp         00277223
  00276E8A: cmp         dword ptr ds:[0E39148h],0
  00276E91: jne         00276F02
  00276E93: cmp         dword ptr ds:[0E38E5Ch],0
  00276E9A: je          00276EA5
  00276E9C: mov         dword ptr [ebp-74h],13h
  00276EA3: jmp         00276EAC
  00276EA5: mov         dword ptr [ebp-74h],8
  00276EAC: mov         esi,esp
  00276EAE: mov         ecx,dword ptr [ebp-74h]
  00276EB1: push        ecx
  00276EB2: mov         edx,dword ptr ds:[0E39140h]
  00276EB8: push        edx
  00276EB9: mov         eax,dword ptr [ebp-14h]
  00276EBC: push        eax
  00276EBD: mov         ecx,dword ptr [ebp-14h]
  00276EC0: mov         edx,dword ptr [ecx]
  00276EC2: mov         eax,dword ptr [edx+50h]
  00276EC5: call        eax
  00276EC7: cmp         esi,esp
  00276EC9: call        @ILT+18365(__RTC_CheckEsp)
  00276ECE: mov         dword ptr [ebp-1Ch],eax
  00276ED1: cmp         dword ptr [ebp-1Ch],0
  00276ED5: je          00276EF1
  00276ED7: mov         ecx,dword ptr [ebp-1Ch]
  00276EDA: push        ecx
  00276EDB: push        0
  00276EDD: push        5D1C94h
  00276EE2: call        @ILT+16925(_FUN_004ce6e0)
  00276EE7: add         esp,0Ch
  00276EEA: xor         eax,eax
  00276EEC: jmp         00277223
  00276EF1: push        0
  00276EF3: push        0
  00276EF5: push        5D1C74h
  00276EFA: call        @ILT+16925(_FUN_004ce6e0)
  00276EFF: add         esp,0Ch
  00276F02: mov         edx,dword ptr ds:[0E39140h]
  00276F08: mov         dword ptr [ebp-20h],edx
  00276F0B: mov         esi,esp
  00276F0D: push        0FFFFFFF0h
  00276F0F: mov         eax,dword ptr [ebp-20h]
  00276F12: push        eax
  00276F13: mov         ecx,dword ptr [ebp-8]
  00276F16: mov         edx,dword ptr [ecx+18h]
  00276F19: call        edx
  00276F1B: cmp         esi,esp
  00276F1D: call        @ILT+18365(__RTC_CheckEsp)
  00276F22: mov         dword ptr [ebp-54h],eax
  00276F25: cmp         dword ptr ds:[0E38E5Ch],0
  00276F2C: je          00277043
  00276F32: mov         eax,dword ptr [ebp-54h]
  00276F35: and         eax,0FF3BFFFFh
  00276F3A: or          eax,90000000h
  00276F3F: mov         esi,esp
  00276F41: push        eax
  00276F42: push        0FFFFFFF0h
  00276F44: mov         ecx,dword ptr [ebp-20h]
  00276F47: push        ecx
  00276F48: mov         edx,dword ptr [ebp-8]
  00276F4B: mov         eax,dword ptr [edx+1Ch]
  00276F4E: call        eax
  00276F50: cmp         esi,esp
  00276F52: call        @ILT+18365(__RTC_CheckEsp)
  00276F57: mov         esi,esp
  00276F59: push        10h
  00276F5B: mov         ecx,dword ptr ds:[0E38D98h]
  00276F61: push        ecx
  00276F62: mov         edx,dword ptr ds:[0E38D9Ch]
  00276F68: push        edx
  00276F69: mov         eax,dword ptr [ebp-14h]
  00276F6C: push        eax
  00276F6D: mov         ecx,dword ptr [ebp-14h]
  00276F70: mov         edx,dword ptr [ecx]
  00276F72: mov         eax,dword ptr [edx+54h]
  00276F75: call        eax
  00276F77: cmp         esi,esp
  00276F79: call        @ILT+18365(__RTC_CheckEsp)
  00276F7E: mov         dword ptr [ebp-1Ch],eax
  00276F81: cmp         dword ptr [ebp-1Ch],0
  00276F85: je          0027702D
  00276F8B: push        2
  00276F8D: call        @ILT+29925(_FUN_004cfdc0)
  00276F92: add         esp,4
  00276F95: call        @ILT+18375(_FUN_00414bd0)
  00276F9A: call        @ILT+11895(_FUN_004e2ab0)
  00276F9F: mov         esi,esp
  00276FA1: push        10h
  00276FA3: mov         ecx,dword ptr ds:[0E38D98h]
  00276FA9: push        ecx
  00276FAA: mov         edx,dword ptr ds:[0E38D9Ch]
  00276FB0: push        edx
  00276FB1: mov         eax,dword ptr [ebp-14h]
  00276FB4: push        eax
  00276FB5: mov         ecx,dword ptr [ebp-14h]
  00276FB8: mov         edx,dword ptr [ecx]
  00276FBA: mov         eax,dword ptr [edx+54h]
  00276FBD: call        eax
  00276FBF: cmp         esi,esp
  00276FC1: call        @ILT+18365(__RTC_CheckEsp)
  00276FC6: mov         dword ptr [ebp-1Ch],eax
  00276FC9: cmp         dword ptr [ebp-1Ch],0
  00276FCD: je          0027702D
  00276FCF: push        1
  00276FD1: call        @ILT+29925(_FUN_004cfdc0)
  00276FD6: add         esp,4
  00276FD9: call        @ILT+18375(_FUN_00414bd0)
  00276FDE: call        @ILT+11895(_FUN_004e2ab0)
  00276FE3: mov         esi,esp
  00276FE5: push        10h
  00276FE7: mov         ecx,dword ptr ds:[0E38D98h]
  00276FED: push        ecx
  00276FEE: mov         edx,dword ptr ds:[0E38D9Ch]
  00276FF4: push        edx
  00276FF5: mov         eax,dword ptr [ebp-14h]
  00276FF8: push        eax
  00276FF9: mov         ecx,dword ptr [ebp-14h]
  00276FFC: mov         edx,dword ptr [ecx]
  00276FFE: mov         eax,dword ptr [edx+54h]
  00277001: call        eax
  00277003: cmp         esi,esp
  00277005: call        @ILT+18365(__RTC_CheckEsp)
  0027700A: mov         dword ptr [ebp-1Ch],eax
  0027700D: cmp         dword ptr [ebp-1Ch],0
  00277011: je          0027702D
  00277013: mov         ecx,dword ptr [ebp-1Ch]
  00277016: push        ecx
  00277017: push        0
  00277019: push        5D1C58h
  0027701E: call        @ILT+16925(_FUN_004ce6e0)
  00277023: add         esp,0Ch
  00277026: xor         eax,eax
  00277028: jmp         00277223
  0027702D: push        0
  0027702F: push        0
  00277031: push        5D1C40h
  00277036: call        @ILT+16925(_FUN_004ce6e0)
  0027703B: add         esp,0Ch
  0027703E: jmp         00277215
  00277043: mov         edx,dword ptr [ebp-54h]
  00277046: and         edx,7F30FFFFh
  0027704C: or          edx,0CF0000h
  00277052: mov         esi,esp
  00277054: push        edx
  00277055: push        0FFFFFFF0h
  00277057: mov         eax,dword ptr [ebp-20h]
  0027705A: push        eax
  0027705B: mov         ecx,dword ptr [ebp-8]
  0027705E: mov         edx,dword ptr [ecx+1Ch]
  00277061: call        edx
  00277063: cmp         esi,esp
  00277065: call        @ILT+18365(__RTC_CheckEsp)
  0027706A: mov         dword ptr [ebp-30h],0
  00277071: mov         eax,dword ptr [ebp-30h]
  00277074: mov         dword ptr [ebp-34h],eax
  00277077: mov         ecx,dword ptr ds:[0E38D9Ch]
  0027707D: mov         dword ptr [ebp-2Ch],ecx
  00277080: mov         edx,dword ptr ds:[0E38D98h]
  00277086: mov         dword ptr [ebp-28h],edx
  00277089: mov         esi,esp
  0027708B: push        0FFFFFFECh
  0027708D: mov         eax,dword ptr [ebp-20h]
  00277090: push        eax
  00277091: mov         ecx,dword ptr [ebp-8]
  00277094: mov         edx,dword ptr [ecx+18h]
  00277097: call        edx
  00277099: cmp         esi,esp
  0027709B: call        @ILT+18365(__RTC_CheckEsp)
  002770A0: mov         dword ptr [ebp-68h],eax
  002770A3: mov         esi,esp
  002770A5: mov         eax,dword ptr [ebp-20h]
  002770A8: push        eax
  002770A9: mov         ecx,dword ptr [ebp-8]
  002770AC: mov         edx,dword ptr [ecx+20h]
  002770AF: call        edx
  002770B1: cmp         esi,esp
  002770B3: call        @ILT+18365(__RTC_CheckEsp)
  002770B8: mov         dword ptr [ebp-70h],eax
  002770BB: mov         esi,esp
  002770BD: push        0FFFFFFF0h
  002770BF: mov         eax,dword ptr [ebp-20h]
  002770C2: push        eax
  002770C3: mov         ecx,dword ptr [ebp-8]
  002770C6: mov         edx,dword ptr [ecx+18h]
  002770C9: call        edx
  002770CB: cmp         esi,esp
  002770CD: call        @ILT+18365(__RTC_CheckEsp)
  002770D2: mov         dword ptr [ebp-6Ch],eax
  002770D5: cmp         dword ptr [ebp-70h],0
  002770D9: je          002770E4
  002770DB: mov         dword ptr [ebp-78h],1
  002770E2: jmp         002770EB
  002770E4: mov         dword ptr [ebp-78h],0
  002770EB: mov         esi,esp
  002770ED: mov         eax,dword ptr [ebp-68h]
  002770F0: push        eax
  002770F1: mov         ecx,dword ptr [ebp-78h]
  002770F4: push        ecx
  002770F5: mov         edx,dword ptr [ebp-6Ch]
  002770F8: push        edx
  002770F9: lea         eax,[ebp-34h]
  002770FC: push        eax
  002770FD: mov         ecx,dword ptr [ebp-8]
  00277100: mov         edx,dword ptr [ecx+24h]
  00277103: call        edx
  00277105: cmp         esi,esp
  00277107: call        @ILT+18365(__RTC_CheckEsp)
  0027710C: mov         eax,dword ptr ds:[00E38DB8h]
  00277111: sub         eax,dword ptr ds:[0E38DB0h]
  00277117: cdq
  00277118: sub         eax,edx
  0027711A: sar         eax,1
  0027711C: mov         dword ptr [ebp-60h],eax
  0027711F: mov         eax,dword ptr ds:[00E38DB4h]
  00277124: sub         eax,dword ptr ds:[0E38DACh]
  0027712A: cdq
  0027712B: sub         eax,edx
  0027712D: sar         eax,1
  0027712F: mov         dword ptr [ebp-64h],eax
  00277132: mov         eax,dword ptr [ebp-60h]
  00277135: mov         dword ptr ds:[00E390E8h],eax
  0027713A: mov         ecx,dword ptr [ebp-64h]
  0027713D: mov         dword ptr ds:[0E390E4h],ecx
  00277143: mov         esi,esp
  00277145: push        16h
  00277147: mov         edx,dword ptr [ebp-28h]
  0027714A: sub         edx,dword ptr [ebp-30h]
  0027714D: push        edx
  0027714E: mov         eax,dword ptr [ebp-2Ch]
  00277151: sub         eax,dword ptr [ebp-34h]
  00277154: push        eax
  00277155: push        0
  00277157: push        0
  00277159: push        0
  0027715B: mov         ecx,dword ptr [ebp-20h]
  0027715E: push        ecx
  0027715F: mov         edx,dword ptr [ebp-8]
  00277162: mov         eax,dword ptr [edx+28h]
  00277165: call        eax
  00277167: cmp         esi,esp
  00277169: call        @ILT+18365(__RTC_CheckEsp)
  0027716E: mov         esi,esp
  00277170: push        13h
  00277172: push        0
  00277174: push        0
  00277176: push        0
  00277178: push        0
  0027717A: push        0FFFFFFFEh
  0027717C: mov         ecx,dword ptr [ebp-20h]
  0027717F: push        ecx
  00277180: mov         edx,dword ptr [ebp-8]
  00277183: mov         eax,dword ptr [edx+28h]
  00277186: call        eax
  00277188: cmp         esi,esp
  0027718A: call        @ILT+18365(__RTC_CheckEsp)
  0027718F: mov         esi,esp
  00277191: push        0
  00277193: lea         ecx,[ebp-4Ch]
  00277196: push        ecx
  00277197: push        0
  00277199: push        30h
  0027719B: mov         edx,dword ptr [ebp-8]
  0027719E: mov         eax,dword ptr [edx+2Ch]
  002771A1: call        eax
  002771A3: cmp         esi,esp
  002771A5: call        @ILT+18365(__RTC_CheckEsp)
  002771AA: mov         esi,esp
  002771AC: lea         ecx,[ebp-34h]
  002771AF: push        ecx
  002771B0: mov         edx,dword ptr [ebp-20h]
  002771B3: push        edx
  002771B4: mov         eax,dword ptr [ebp-8]
  002771B7: mov         ecx,dword ptr [eax+30h]
  002771BA: call        ecx
  002771BC: cmp         esi,esp
  002771BE: call        @ILT+18365(__RTC_CheckEsp)
  002771C3: mov         edx,dword ptr [ebp-34h]
  002771C6: cmp         edx,dword ptr [ebp-4Ch]
  002771C9: jge         002771D1
  002771CB: mov         eax,dword ptr [ebp-4Ch]
  002771CE: mov         dword ptr [ebp-34h],eax
  002771D1: mov         ecx,dword ptr [ebp-30h]
  002771D4: cmp         ecx,dword ptr [ebp-48h]
  002771D7: jge         002771DF
  002771D9: mov         edx,dword ptr [ebp-48h]
  002771DC: mov         dword ptr [ebp-30h],edx
  002771DF: mov         esi,esp
  002771E1: push        15h
  002771E3: push        0
  002771E5: push        0
  002771E7: mov         eax,dword ptr [ebp-64h]
  002771EA: push        eax
  002771EB: mov         ecx,dword ptr [ebp-60h]
  002771EE: push        ecx
  002771EF: push        0
  002771F1: mov         edx,dword ptr [ebp-20h]
  002771F4: push        edx
  002771F5: mov         eax,dword ptr [ebp-8]
  002771F8: mov         ecx,dword ptr [eax+28h]
  002771FB: call        ecx
  002771FD: cmp         esi,esp
  002771FF: call        @ILT+18365(__RTC_CheckEsp)
  00277204: push        0
  00277206: push        0
  00277208: push        5D1C28h
  0027720D: call        @ILT+16925(_FUN_004ce6e0)
  00277212: add         esp,0Ch
  00277215: mov         edx,dword ptr [ebp-14h]
  00277218: mov         dword ptr ds:[0E39148h],edx
  0027721E: mov         eax,1
  00277223: push        edx
  00277224: mov         ecx,ebp
  00277226: push        eax
  00277227: lea         edx,ds:[277250h]
  0027722D: call        @ILT+135(@_RTC_CheckStackVars@8)
  00277232: pop         eax
  00277233: pop         edx
  00277234: pop         edi
  00277235: pop         esi
  00277236: mov         ecx,dword ptr [ebp-4]
  00277239: xor         ecx,ebp
  0027723B: call        @ILT+13165(@__security_check_cookie@4)
  00277240: add         esp,78h
  00277243: cmp         ebp,esp
  00277245: call        @ILT+18365(__RTC_CheckEsp)
  0027724A: mov         esp,ebp
  0027724C: pop         ebp
  0027724D: ret
  0027724E: nop
  00277250: add         eax,dword ptr [eax]

  Summary

      36A000 .text
