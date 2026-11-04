Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  0041AC00: push        ebx
  0041AC01: push        esi
  0041AC02: xor         ebx,ebx
  0041AC04: or          esi,0FFFFFFFFh
  0041AC07: mov         byte ptr [ecx],0FFh
  0041AC0A: mov         byte ptr [ecx+1],bl
  0041AC0D: mov         dword ptr [ecx+8],esi
  0041AC10: mov         dword ptr [ecx+0Ch],esi
  0041AC13: mov         dword ptr [ecx+10h],esi
  0041AC16: mov         dword ptr [ecx+14h],esi
  0041AC19: mov         dword ptr [ecx+18h],esi
  0041AC1C: mov         dword ptr [ecx+1Ch],esi
  0041AC1F: mov         byte ptr [ecx+7],bl
  0041AC22: mov         edx,32h
  0041AC27: lea         eax,[ecx+24h]
  0041AC2A: mov         dword ptr [eax-4],edx
  0041AC2D: mov         dword ptr [eax],edx
  0041AC2F: mov         word ptr [eax+4],si
  0041AC33: mov         byte ptr [eax+6],bl
  0041AC36: add         edx,32h
  0041AC39: add         eax,0Ch
  0041AC3C: cmp         edx,96h
  0041AC42: jl          0041AC2A
  0041AC44: mov         dword ptr [ecx+38h],ebx
  0041AC47: mov         byte ptr [ecx+3Ch],bl
  0041AC4A: pop         esi
  0041AC4B: pop         ebx
  0041AC4C: ret
  0041AC4D: nop
  0041AC4E: nop
  0041AC4F: nop
  0041AC50: push        ebx
  0041AC51: push        esi
  0041AC52: mov         ebx,ecx
  0041AC54: push        edi
  0041AC55: mov         esi,ebx
  0041AC57: mov         edi,64h
  0041AC5C: mov         ecx,esi
  0041AC5E: call        0041AC00
  0041AC63: add         esi,40h
  0041AC66: dec         edi
  0041AC67: jne         0041AC5C
  0041AC69: xor         eax,eax
  0041AC6B: mov         ecx,10h
  0041AC70: lea         edi,[ebx+00002C90h]
  0041AC76: mov         dword ptr [ebx+00001900h],eax
  0041AC7C: mov         dword ptr [ebx+00002C8Ch],eax
  0041AC82: rep stos    dword ptr es:[edi]
  0041AC84: stos        byte ptr es:[edi]
  0041AC85: pop         edi
  0041AC86: mov         eax,ebx
  0041AC88: pop         esi
  0041AC89: pop         ebx
  0041AC8A: ret
  0041AC8B: nop
  0041AC8C: nop
  0041AC8D: nop
  0041AC8E: nop
  0041AC8F: nop
  0041AC90: mov         eax,ecx
  0041AC92: push        esi
  0041AC93: push        edi
  0041AC94: mov         edi,dword ptr [eax+00001900h]
  0041AC9A: cmp         edi,64h
  0041AC9D: jge         0041ACC8
  0041AC9F: mov         esi,dword ptr [esp+0Ch]
  0041ACA3: cmp         byte ptr [esi],9
  0041ACA6: jae         0041ACC8
  0041ACA8: shl         edi,6
  0041ACAB: add         edi,eax
  0041ACAD: mov         ecx,10h
  0041ACB2: rep movs    dword ptr es:[edi],dword ptr [esi]
  0041ACB4: mov         ecx,dword ptr [eax+00001900h]
  0041ACBA: pop         edi
  0041ACBB: inc         ecx
  0041ACBC: pop         esi
  0041ACBD: mov         dword ptr [eax+00001900h],ecx
  0041ACC3: mov         al,1
  0041ACC5: ret         4
  0041ACC8: pop         edi
  0041ACC9: xor         al,al
  0041ACCB: pop         esi
  0041ACCC: ret         4
  0041ACCF: nop
  0041ACD0: mov         edx,dword ptr [esp+4]
  0041ACD4: push        esi
  0041ACD5: mov         esi,edx
  0041ACD7: mov         cl,byte ptr [edx]
  0041ACD9: test        cl,cl
  0041ACDB: je          0041ACFB
  0041ACDD: cmp         cl,20h
  0041ACE0: ja          0041ACFB
  0041ACE2: test        cl,cl
  0041ACE4: mov         eax,edx
  0041ACE6: je          0041ACF5
  0041ACE8: mov         cl,byte ptr [eax+1]
  0041ACEB: mov         byte ptr [eax],cl
  0041ACED: mov         cl,byte ptr [eax+1]
  0041ACF0: inc         eax
  0041ACF1: test        cl,cl
  0041ACF3: jne         0041ACE8
  0041ACF5: mov         cl,byte ptr [edx]
  0041ACF7: test        cl,cl
  0041ACF9: jne         0041ACDD
  0041ACFB: mov         al,byte ptr [edx]
  0041ACFD: xor         ecx,ecx
  0041ACFF: test        al,al
  0041AD01: je          0041AD18
  0041AD03: cmp         al,20h
  0041AD05: jbe         0041AD09
  0041AD07: mov         ecx,esi
  0041AD09: mov         al,byte ptr [esi+1]
  0041AD0C: inc         esi
  0041AD0D: test        al,al
  0041AD0F: jne         0041AD03
  0041AD11: test        ecx,ecx
  0041AD13: je          0041AD18
  0041AD15: mov         byte ptr [ecx+1],al
  0041AD18: pop         esi
  0041AD19: ret
  0041AD1A: nop
  0041AD1B: nop
  0041AD1C: nop
  0041AD1D: nop
  0041AD1E: nop
  0041AD1F: nop
  0041AD20: mov         edx,dword ptr [esp+4]
  0041AD24: cmp         byte ptr [edx],0
  0041AD27: je          0041AD4B
  0041AD29: mov         cl,byte ptr [edx]
  0041AD2B: cmp         cl,20h
  0041AD2E: jbe         0041AD33
  0041AD30: inc         edx
  0041AD31: jmp         0041AD46
  0041AD33: test        cl,cl
  0041AD35: mov         eax,edx
  0041AD37: je          0041AD46
  0041AD39: mov         cl,byte ptr [eax+1]
  0041AD3C: mov         byte ptr [eax],cl
  0041AD3E: mov         cl,byte ptr [eax+1]
  0041AD41: inc         eax
  0041AD42: test        cl,cl
  0041AD44: jne         0041AD39
  0041AD46: cmp         byte ptr [edx],0
  0041AD49: jne         0041AD29
  0041AD4B: ret
  0041AD4C: nop
  0041AD4D: nop
  0041AD4E: nop
  0041AD4F: nop
  0041AD50: mov         eax,dword ptr ds:[0057BE28h]
  0041AD55: push        esi
  0041AD56: push        edi
  0041AD57: push        eax
  0041AD58: push        0
  0041AD5A: call        00561DB1
  0041AD5F: mov         edi,eax
  0041AD61: add         esp,8
  0041AD64: test        edi,edi
  0041AD66: jne         0041AD6B
  0041AD68: pop         edi
  0041AD69: pop         esi
  0041AD6A: ret
  0041AD6B: mov         al,byte ptr [edi]
  0041AD6D: mov         esi,edi
  0041AD6F: cmp         al,2Dh
  0041AD71: jne         0041AD76
  0041AD73: lea         esi,[edi+1]
  0041AD76: mov         al,byte ptr [esi]
  0041AD78: test        al,al
  0041AD7A: je          0041AD96
  0041AD7C: and         eax,0FFh
  0041AD81: push        eax
  0041AD82: call        00561D89
  0041AD87: add         esp,4
  0041AD8A: test        eax,eax
  0041AD8C: je          0041ADAD
  0041AD8E: mov         al,byte ptr [esi+1]
  0041AD91: inc         esi
  0041AD92: test        al,al
  0041AD94: jne         0041AD7C
  0041AD96: push        edi
  0041AD97: call        00561C96
  0041AD9C: mov         ecx,dword ptr [esp+10h]
  0041ADA0: add         esp,4
  0041ADA3: mov         dword ptr [ecx],eax
  0041ADA5: pop         edi
  0041ADA6: mov         eax,1
  0041ADAB: pop         esi
  0041ADAC: ret
  0041ADAD: pop         edi
  0041ADAE: mov         eax,2
  0041ADB3: pop         esi
  0041ADB4: ret
  0041ADB5: nop
  0041ADB6: nop
  0041ADB7: nop
  0041ADB8: nop
  0041ADB9: nop
  0041ADBA: nop
  0041ADBB: nop
  0041ADBC: nop
  0041ADBD: nop
  0041ADBE: nop
  0041ADBF: nop
  0041ADC0: push        ecx
  0041ADC1: lea         eax,[esp]
  0041ADC5: push        eax
  0041ADC6: call        0041AD50
  0041ADCB: add         esp,4
  0041ADCE: cmp         eax,1
  0041ADD1: jne         0041ADDF
  0041ADD3: mov         ecx,dword ptr [esp+8]
  0041ADD7: mov         dx,word ptr [esp]
  0041ADDC: mov         word ptr [ecx],dx
  0041ADDF: pop         ecx
  0041ADE0: ret
  0041ADE1: nop
  0041ADE2: nop
  0041ADE3: nop
  0041ADE4: nop
  0041ADE5: nop
  0041ADE6: nop
  0041ADE7: nop
  0041ADE8: nop
  0041ADE9: nop
  0041ADEA: nop
  0041ADEB: nop
  0041ADEC: nop
  0041ADED: nop
  0041ADEE: nop
  0041ADEF: nop
  0041ADF0: mov         eax,dword ptr [esp+10h]
  0041ADF4: mov         ecx,dword ptr [esp+0Ch]
  0041ADF8: mov         edx,dword ptr [esp+8]
  0041ADFC: sub         esp,12Ch
  0041AE02: push        eax
  0041AE03: mov         eax,dword ptr [esp+00000134h]
  0041AE0A: push        ecx
  0041AE0B: push        edx
  0041AE0C: push        eax
  0041AE0D: lea         ecx,[esp+10h]
  0041AE11: push        57C04Ch
  0041AE16: push        ecx
  0041AE17: call        00561A36
  0041AE1C: push        0
  0041AE1E: lea         edx,[esp+1Ch]
  0041AE22: push        0
  0041AE24: push        edx
  0041AE25: call        004CE6E0
  0041AE2A: add         esp,150h
  0041AE30: ret
  0041AE31: nop
  0041AE32: nop
  0041AE33: nop
  0041AE34: nop
  0041AE35: nop
  0041AE36: nop
  0041AE37: nop
  0041AE38: nop
  0041AE39: nop
  0041AE3A: nop
  0041AE3B: nop
  0041AE3C: nop
  0041AE3D: nop
  0041AE3E: nop
  0041AE3F: nop
  0041AE40: sub         esp,218h
  0041AE46: push        ebx
  0041AE47: mov         ebx,ecx
  0041AE49: push        ebp
  0041AE4A: push        esi
  0041AE4B: lea         ecx,[esp+20h]
  0041AE4F: mov         dword ptr [esp+18h],ebx
  0041AE53: mov         dword ptr [esp+60h],57C270h
  0041AE5B: mov         dword ptr [esp+64h],57C258h
  0041AE63: mov         dword ptr [esp+68h],57C244h
  0041AE6B: mov         dword ptr [esp+6Ch],57C234h
  0041AE73: mov         dword ptr [esp+70h],57C220h
  0041AE7B: mov         dword ptr [esp+74h],57C210h
  0041AE83: mov         dword ptr [esp+78h],57C1F8h
  0041AE8B: mov         dword ptr [esp+7Ch],57C1ECh
  0041AE93: mov         dword ptr [esp+00000080h],57C1D4h
  0041AE9E: mov         dword ptr [esp+00000084h],57C1BCh
  0041AEA9: mov         dword ptr [esp+00000088h],57C1B4h
  0041AEB4: mov         dword ptr [esp+0000008Ch],57C1A4h
  0041AEBF: mov         dword ptr [esp+00000090h],57C19Ch
  0041AECA: call        0041AC00
  0041AECF: mov         eax,dword ptr [esp+00000228h]
  0041AED6: push        5763BCh
  0041AEDB: push        eax
  0041AEDC: call        00561A0A
  0041AEE1: mov         esi,eax
  0041AEE3: xor         ebp,ebp
  0041AEE5: add         esp,8
  0041AEE8: cmp         esi,ebp
  0041AEEA: mov         dword ptr [esp+1Ch],esi
  0041AEEE: jne         0041AEFE
  0041AEF0: pop         esi
  0041AEF1: pop         ebp
  0041AEF2: xor         al,al
  0041AEF4: pop         ebx
  0041AEF5: add         esp,218h
  0041AEFB: ret         4
  0041AEFE: push        edi
  0041AEFF: mov         byte ptr [esp+13h],0
  0041AF04: mov         edi,6FDD08h
  0041AF09: mov         ecx,edi
  0041AF0B: call        0041AC00
  0041AF10: add         edi,40h
  0041AF13: cmp         edi,6FF608h
  0041AF19: jl          0041AF09
  0041AF1B: mov         ecx,10h
  0041AF20: xor         eax,eax
  0041AF22: lea         edi,[ebx+00002C90h]
  0041AF28: mov         dword ptr [ebx+00001900h],ebp
  0041AF2E: mov         dword ptr [ebx+00002C8Ch],ebp
  0041AF34: push        esi
  0041AF35: rep stos    dword ptr es:[edi]
  0041AF37: lea         ecx,[esp+00000164h]
  0041AF3E: push        0C8h
  0041AF43: push        ecx
  0041AF44: mov         bl,1
  0041AF46: stos        byte ptr es:[edi]
  0041AF47: call        00561E4D
  0041AF4C: add         esp,0Ch
  0041AF4F: test        eax,eax
  0041AF51: je          0041B50D
  0041AF57: lea         edi,[esp+00000160h]
  0041AF5E: or          ecx,0FFFFFFFFh
  0041AF61: xor         eax,eax
  0041AF63: inc         ebp
  0041AF64: repne scas  byte ptr es:[edi]
  0041AF66: not         ecx
  0041AF68: sub         edi,ecx
  0041AF6A: lea         edx,[esp+00000098h]
  0041AF71: mov         eax,ecx
  0041AF73: mov         esi,edi
  0041AF75: mov         edi,edx
  0041AF77: shr         ecx,2
  0041AF7A: rep movs    dword ptr es:[edi],dword ptr [esi]
  0041AF7C: mov         ecx,eax
  0041AF7E: and         ecx,3
  0041AF81: rep movs    byte ptr es:[edi],byte ptr [esi]
  0041AF83: lea         ecx,[esp+00000160h]
  0041AF8A: push        ecx
  0041AF8B: call        0041AD20
  0041AF90: mov         al,byte ptr [esp+00000164h]
  0041AF97: add         esp,4
  0041AF9A: cmp         al,3Bh
  0041AF9C: je          0041B4EB
  0041AFA2: test        al,al
  0041AFA4: je          0041B4EB
  0041AFAA: mov         edx,dword ptr ds:[0057BE28h]
  0041AFB0: lea         eax,[esp+00000160h]
  0041AFB7: push        edx
  0041AFB8: push        eax
  0041AFB9: call        00561DB1
  0041AFBE: mov         esi,eax
  0041AFC0: add         esp,8
  0041AFC3: test        esi,esi
  0041AFC5: je          0041B4EB
  0041AFCB: mov         dword ptr [esp+14h],0
  0041AFD3: lea         edi,[esp+64h]
  0041AFD7: mov         ecx,dword ptr [edi]
  0041AFD9: push        ecx
  0041AFDA: push        esi
  0041AFDB: call        00562370
  0041AFE0: add         esp,8
  0041AFE3: test        eax,eax
  0041AFE5: je          0041AFF8
  0041AFE7: mov         eax,dword ptr [esp+14h]
  0041AFEB: add         edi,4
  0041AFEE: inc         eax
  0041AFEF: cmp         eax,0Dh
  0041AFF2: mov         dword ptr [esp+14h],eax
  0041AFF6: jl          0041AFD7
  0041AFF8: mov         eax,dword ptr [esp+14h]
  0041AFFC: cmp         eax,0Ch
  0041AFFF: ja          0041B419
  0041B005: xor         edx,edx
  0041B007: mov         dl,byte ptr [eax+0041B664h]
  0041B00D: jmp         dword ptr [edx*4+0041B650h]
  0041B014: mov         byte ptr [esp+13h],1
  0041B019: jmp         0041B4EB
  0041B01E: mov         ecx,dword ptr [esp+30h]
  0041B022: mov         byte ptr [esp+13h],0
  0041B027: test        ecx,ecx
  0041B029: jl          0041B07D
  0041B02B: mov         ecx,dword ptr [esp+1Ch]
  0041B02F: lea         eax,[esp+24h]
  0041B033: push        eax
  0041B034: call        0041AC90
  0041B039: test        al,al
  0041B03B: jne         0041B058
  0041B03D: mov         edx,dword ptr [esp+0000022Ch]
  0041B044: lea         ecx,[esp+00000098h]
  0041B04B: push        7027D4h
  0041B050: push        ecx
  0041B051: push        ebp
  0041B052: push        edx
  0041B053: jmp         0041B4E1
  0041B058: or          eax,0FFFFFFFFh
  0041B05B: mov         dword ptr [esp+5Ch],0
  0041B063: mov         dword ptr [esp+30h],eax
  0041B067: mov         dword ptr [esp+2Ch],eax
  0041B06B: mov         dword ptr [esp+34h],eax
  0041B06F: mov         eax,dword ptr [esp+14h]
  0041B073: mov         byte ptr [esp+25h],0
  0041B078: mov         byte ptr [esp+2Bh],0
  0041B07D: dec         al
  0041B07F: mov         byte ptr [esp+24h],al
  0041B083: jmp         0041B4EB
  0041B088: mov         al,byte ptr [esp+13h]
  0041B08C: test        al,al
  0041B08E: je          0041B09A
  0041B090: push        57C188h
  0041B095: jmp         0041B4D0
  0041B09A: mov         eax,dword ptr [esp+30h]
  0041B09E: test        eax,eax
  0041B0A0: jl          0041B0DF
  0041B0A2: mov         ecx,dword ptr [esp+1Ch]
  0041B0A6: lea         edx,[esp+24h]
  0041B0AA: push        edx
  0041B0AB: call        0041AC90
  0041B0B0: test        al,al
  0041B0B2: jne         0041B0BE
  0041B0B4: push        57C16Ch
  0041B0B9: jmp         0041B4D0
  0041B0BE: or          eax,0FFFFFFFFh
  0041B0C1: mov         dword ptr [esp+5Ch],0
  0041B0C9: mov         dword ptr [esp+30h],eax
  0041B0CD: mov         dword ptr [esp+2Ch],eax
  0041B0D1: mov         dword ptr [esp+34h],eax
  0041B0D5: mov         byte ptr [esp+25h],0
  0041B0DA: mov         byte ptr [esp+2Bh],0
  0041B0DF: lea         edx,[esp+30h]
  0041B0E3: push        edx
  0041B0E4: call        0041AD50
  0041B0E9: add         esp,4
  0041B0EC: cmp         eax,1
  0041B0EF: je          0041B0FB
  0041B0F1: push        57C15Ch
  0041B0F6: jmp         0041B4D0
  0041B0FB: mov         eax,dword ptr [esp+30h]
  0041B0FF: test        eax,eax
  0041B101: jl          0041B261
  0041B107: mov         edx,dword ptr [esp+1Ch]
  0041B10B: cmp         eax,dword ptr [edx+00002C8Ch]
  0041B111: jge         0041B261
  0041B117: lea         eax,[esp+2Ch]
  0041B11B: push        eax
  0041B11C: call        0041AD50
  0041B121: add         esp,4
  0041B124: cmp         eax,1
  0041B127: je          0041B144
  0041B129: mov         edx,dword ptr [esp+0000022Ch]
  0041B130: lea         ecx,[esp+00000098h]
  0041B137: push        57C14Ch
  0041B13C: push        ecx
  0041B13D: push        ebp
  0041B13E: push        edx
  0041B13F: jmp         0041B4E1
  0041B144: lea         eax,[esp+34h]
  0041B148: push        eax
  0041B149: call        0041AD50
  0041B14E: add         esp,4
  0041B151: cmp         eax,1
  0041B154: je          0041B171
  0041B156: mov         edx,dword ptr [esp+0000022Ch]
  0041B15D: lea         ecx,[esp+00000098h]
  0041B164: push        57C134h
  0041B169: push        ecx
  0041B16A: push        ebp
  0041B16B: push        edx
  0041B16C: jmp         0041B4E1
  0041B171: lea         eax,[esp+18h]
  0041B175: push        eax
  0041B176: call        0041AD50
  0041B17B: add         esp,4
  0041B17E: cmp         eax,2
  0041B181: jne         0041B19E
  0041B183: mov         edx,dword ptr [esp+0000022Ch]
  0041B18A: lea         ecx,[esp+00000098h]
  0041B191: push        57C128h
  0041B196: push        ecx
  0041B197: push        ebp
  0041B198: push        edx
  0041B199: jmp         0041B4E1
  0041B19E: test        eax,eax
  0041B1A0: je          0041B4EB
  0041B1A6: mov         eax,dword ptr [esp+18h]
  0041B1AA: test        eax,eax
  0041B1AC: jl          0041B40F
  0041B1B2: cmp         eax,0FFh
  0041B1B7: jg          0041B40F
  0041B1BD: mov         byte ptr [esp+25h],al
  0041B1C1: mov         byte ptr [esp+2Bh],0
  0041B1C6: xor         edi,edi
  0041B1C8: push        57BFACh
  0041B1CD: push        0
  0041B1CF: call        00561DB1
  0041B1D4: add         esp,8
  0041B1D7: test        eax,eax
  0041B1D9: je          0041B4EB
  0041B1DF: lea         eax,[esp+18h]
  0041B1E3: push        eax
  0041B1E4: call        0041AD50
  0041B1E9: add         esp,4
  0041B1EC: cmp         eax,2
  0041B1EF: je          0041B22B
  0041B1F1: test        eax,eax
  0041B1F3: je          0041B4EB
  0041B1F9: mov         eax,dword ptr [esp+18h]
  0041B1FD: test        eax,eax
  0041B1FF: jl          0041B246
  0041B201: cmp         eax,0FFh
  0041B206: jg          0041B246
  0041B208: mov         ecx,dword ptr [esp+2Bh]
  0041B20C: and         ecx,0FFh
  0041B212: mov         byte ptr [esp+ecx+26h],al
  0041B216: mov         dl,byte ptr [esp+2Bh]
  0041B21A: inc         dl
  0041B21C: inc         edi
  0041B21D: cmp         edi,5
  0041B220: mov         byte ptr [esp+2Bh],dl
  0041B224: jl          0041B1C8
  0041B226: jmp         0041B4EB
  0041B22B: mov         eax,dword ptr [esp+0000022Ch]
  0041B232: lea         edx,[esp+00000098h]
  0041B239: push        57C118h
  0041B23E: push        edx
  0041B23F: push        ebp
  0041B240: push        eax
  0041B241: jmp         0041B4E1
  0041B246: mov         edx,dword ptr [esp+0000022Ch]
  0041B24D: lea         ecx,[esp+00000098h]
  0041B254: push        57C118h
  0041B259: push        ecx
  0041B25A: push        ebp
  0041B25B: push        edx
  0041B25C: jmp         0041B4E1
  0041B261: mov         eax,dword ptr [esp+0000022Ch]
  0041B268: lea         edx,[esp+00000098h]
  0041B26F: push        57C15Ch
  0041B274: push        edx
  0041B275: push        ebp
  0041B276: push        eax
  0041B277: jmp         0041B4E1
  0041B27C: mov         eax,dword ptr [esp+30h]
  0041B280: test        eax,eax
  0041B282: jge         0041B28C
  0041B284: mov         dword ptr [esp+30h],7Fh
  0041B28C: mov         al,byte ptr [esp+13h]
  0041B290: test        al,al
  0041B292: je          0041B2AF
  0041B294: mov         edx,dword ptr [esp+0000022Ch]
  0041B29B: lea         ecx,[esp+00000098h]
  0041B2A2: push        57C188h
  0041B2A7: push        ecx
  0041B2A8: push        ebp
  0041B2A9: push        edx
  0041B2AA: jmp         0041B4E1
  0041B2AF: lea         eax,[esp+40h]
  0041B2B3: push        eax
  0041B2B4: call        0041AD50
  0041B2B9: add         esp,4
  0041B2BC: cmp         eax,1
  0041B2BF: je          0041B2DC
  0041B2C1: mov         edx,dword ptr [esp+0000022Ch]
  0041B2C8: lea         ecx,[esp+00000098h]
  0041B2CF: push        57C104h
  0041B2D4: push        ecx
  0041B2D5: push        ebp
  0041B2D6: push        edx
  0041B2D7: jmp         0041B4E1
  0041B2DC: lea         eax,[esp+3Ch]
  0041B2E0: push        eax
  0041B2E1: call        0041AD50
  0041B2E6: add         esp,4
  0041B2E9: cmp         eax,1
  0041B2EC: je          0041B4EB
  0041B2F2: jmp         0041B3EB
  0041B2F7: mov         al,byte ptr [esp+13h]
  0041B2FB: test        al,al
  0041B2FD: je          0041B309
  0041B2FF: push        57C188h
  0041B304: jmp         0041B4D0
  0041B309: cmp         dword ptr [esp+5Ch],2
  0041B30E: jl          0041B32B
  0041B310: mov         eax,dword ptr [esp+0000022Ch]
  0041B317: lea         edx,[esp+00000098h]
  0041B31E: push        57C0F0h
  0041B323: push        edx
  0041B324: push        ebp
  0041B325: push        eax
  0041B326: jmp         0041B4E1
  0041B32B: lea         ecx,[esp+18h]
  0041B32F: push        ecx
  0041B330: call        0041AD50
  0041B335: add         esp,4
  0041B338: cmp         eax,1
  0041B33B: je          0041B358
  0041B33D: mov         eax,dword ptr [esp+0000022Ch]
  0041B344: lea         edx,[esp+00000098h]
  0041B34B: push        57C128h
  0041B350: push        edx
  0041B351: push        ebp
  0041B352: push        eax
  0041B353: jmp         0041B4E1
  0041B358: mov         ecx,dword ptr [esp+18h]
  0041B35C: test        ecx,ecx
  0041B35E: jl          0041B40F
  0041B364: cmp         ecx,0FFh
  0041B36A: jg          0041B40F
  0041B370: mov         eax,dword ptr [esp+5Ch]
  0041B374: lea         edx,[eax+eax*2]
  0041B377: mov         byte ptr [esp+edx*4+4Eh],cl
  0041B37B: mov         eax,dword ptr [esp+5Ch]
  0041B37F: lea         eax,[eax+eax*2]
  0041B382: lea         ecx,[esp+eax*4+44h]
  0041B386: push        ecx
  0041B387: call        0041AD50
  0041B38C: add         esp,4
  0041B38F: cmp         eax,1
  0041B392: je          0041B3AF
  0041B394: mov         eax,dword ptr [esp+0000022Ch]
  0041B39B: lea         edx,[esp+00000098h]
  0041B3A2: push        57C0E4h
  0041B3A7: push        edx
  0041B3A8: push        ebp
  0041B3A9: push        eax
  0041B3AA: jmp         0041B4E1
  0041B3AF: mov         eax,dword ptr [esp+5Ch]
  0041B3B3: lea         ecx,[eax+eax*2]
  0041B3B6: lea         edx,[esp+ecx*4+48h]
  0041B3BA: push        edx
  0041B3BB: call        0041AD50
  0041B3C0: add         esp,4
  0041B3C3: cmp         eax,1
  0041B3C6: je          0041B3D2
  0041B3C8: push        57C0D8h
  0041B3CD: jmp         0041B4D0
  0041B3D2: mov         eax,dword ptr [esp+5Ch]
  0041B3D6: lea         edx,[eax+eax*2]
  0041B3D9: lea         eax,[esp+edx*4+4Ch]
  0041B3DD: push        eax
  0041B3DE: call        0041ADC0
  0041B3E3: add         esp,4
  0041B3E6: cmp         eax,1
  0041B3E9: je          0041B406
  0041B3EB: mov         edx,dword ptr [esp+0000022Ch]
  0041B3F2: lea         ecx,[esp+00000098h]
  0041B3F9: push        57C0CCh
  0041B3FE: push        ecx
  0041B3FF: push        ebp
  0041B400: push        edx
  0041B401: jmp         0041B4E1
  0041B406: inc         dword ptr [esp+5Ch]
  0041B40A: jmp         0041B4EB
  0041B40F: push        57C128h
  0041B414: jmp         0041B4D0
  0041B419: mov         al,byte ptr [esp+13h]
  0041B41D: test        al,al
  0041B41F: je          0041B4CB
  0041B425: lea         edi,[esp+00000098h]
  0041B42C: or          ecx,0FFFFFFFFh
  0041B42F: xor         eax,eax
  0041B431: repne scas  byte ptr es:[edi]
  0041B433: not         ecx
  0041B435: dec         ecx
  0041B436: dec         ecx
  0041B437: js          0041B44E
  0041B439: cmp         byte ptr [esp+ecx+00000098h],20h
  0041B441: ja          0041B44E
  0041B443: mov         byte ptr [esp+ecx+00000098h],0
  0041B44B: dec         ecx
  0041B44C: jns         0041B439
  0041B44E: lea         edx,[ecx+2]
  0041B451: cmp         edx,32h
  0041B454: jle         0041B45D
  0041B456: push        57C0B4h
  0041B45B: jmp         0041B4D0
  0041B45D: test        ecx,ecx
  0041B45F: jle         0041B4EB
  0041B465: mov         edx,dword ptr [esp+1Ch]
  0041B469: mov         eax,dword ptr [edx+00002C8Ch]
  0041B46F: cmp         eax,64h
  0041B472: jge         0041B4B3
  0041B474: lea         eax,[eax+eax*4]
  0041B477: lea         edi,[esp+00000098h]
  0041B47E: or          ecx,0FFFFFFFFh
  0041B481: lea         eax,[eax+eax*4]
  0041B484: lea         esi,[edx+eax*2+00001904h]
  0041B48B: xor         eax,eax
  0041B48D: repne scas  byte ptr es:[edi]
  0041B48F: not         ecx
  0041B491: sub         edi,ecx
  0041B493: mov         dword ptr [esp+14h],esi
  0041B497: mov         eax,ecx
  0041B499: mov         esi,edi
  0041B49B: mov         edi,dword ptr [esp+14h]
  0041B49F: shr         ecx,2
  0041B4A2: rep movs    dword ptr es:[edi],dword ptr [esi]
  0041B4A4: mov         ecx,eax
  0041B4A6: and         ecx,3
  0041B4A9: rep movs    byte ptr es:[edi],byte ptr [esi]
  0041B4AB: inc         dword ptr [edx+00002C8Ch]
  0041B4B1: jmp         0041B4EB
  0041B4B3: mov         edx,dword ptr [esp+0000022Ch]
  0041B4BA: lea         ecx,[esp+00000098h]
  0041B4C1: push        57C09Ch
  0041B4C6: push        ecx
  0041B4C7: push        ebp
  0041B4C8: push        edx
  0041B4C9: jmp         0041B4E1
  0041B4CB: push        57C088h
  0041B4D0: mov         ecx,dword ptr [esp+00000230h]
  0041B4D7: lea         eax,[esp+0000009Ch]
  0041B4DE: push        eax
  0041B4DF: push        ebp
  0041B4E0: push        ecx
  0041B4E1: call        0041ADF0
  0041B4E6: add         esp,10h
  0041B4E9: xor         bl,bl
  0041B4EB: mov         edx,dword ptr [esp+20h]
  0041B4EF: lea         eax,[esp+00000160h]
  0041B4F6: push        edx
  0041B4F7: push        0C8h
  0041B4FC: push        eax
  0041B4FD: call        00561E4D
  0041B502: add         esp,0Ch
  0041B505: test        eax,eax
  0041B507: jne         0041AF57
  0041B50D: mov         eax,dword ptr [esp+30h]
  0041B511: mov         esi,dword ptr [esp+1Ch]
  0041B515: xor         edi,edi
  0041B517: cmp         eax,edi
  0041B519: jl          0041B54B
  0041B51B: lea         ecx,[esp+24h]
  0041B51F: push        ecx
  0041B520: mov         ecx,esi
  0041B522: call        0041AC90
  0041B527: test        al,al
  0041B529: jne         0041B54B
  0041B52B: mov         eax,dword ptr [esp+0000022Ch]
  0041B532: lea         edx,[esp+00000098h]
  0041B539: push        57C16Ch
  0041B53E: push        edx
  0041B53F: push        ebp
  0041B540: push        eax
  0041B541: call        0041ADF0
  0041B546: add         esp,10h
  0041B549: xor         bl,bl
  0041B54B: mov         eax,dword ptr [esi+00001900h]
  0041B551: mov         dword ptr [esp+14h],edi
  0041B555: cmp         eax,edi
  0041B557: jle         0041B623
  0041B55D: mov         ecx,dword ptr [edi+006FDD40h]
  0041B563: cmp         ecx,1
  0041B566: jle         0041B609
  0041B56C: xor         eax,eax
  0041B56E: test        ecx,ecx
  0041B570: mov         dword ptr [esp+18h],eax
  0041B574: jle         0041B609
  0041B57A: lea         esi,[edi+006FDD08h]
  0041B580: mov         ecx,dword ptr [esp+14h]
  0041B584: push        eax
  0041B585: mov         eax,esi
  0041B587: xor         edx,edx
  0041B589: push        ecx
  0041B58A: mov         ecx,6FDD08h
  0041B58F: mov         dl,byte ptr [eax]
  0041B591: push        edx
  0041B592: call        0041B920
  0041B597: test        eax,eax
  0041B599: jge         0041B5EE
  0041B59B: mov         eax,dword ptr [esp+18h]
  0041B59F: mov         edx,dword ptr [edi+006FDD14h]
  0041B5A5: xor         ecx,ecx
  0041B5A7: lea         eax,[eax+eax*2]
  0041B5AA: mov         cl,byte ptr [edi+eax*4+006FDD32h]
  0041B5B1: xor         eax,eax
  0041B5B3: mov         al,byte ptr [esi]
  0041B5B5: push        ecx
  0041B5B6: push        edx
  0041B5B7: lea         edx,[esp+00000168h]
  0041B5BE: mov         ecx,dword ptr [esp+eax*4+70h]
  0041B5C2: push        ecx
  0041B5C3: push        57C064h
  0041B5C8: push        edx
  0041B5C9: call        00561A36
  0041B5CE: mov         ecx,dword ptr [esp+00000240h]
  0041B5D5: lea         eax,[esp+00000174h]
  0041B5DC: push        eax
  0041B5DD: push        7027D4h
  0041B5E2: push        ebp
  0041B5E3: push        ecx
  0041B5E4: call        0041ADF0
  0041B5E9: add         esp,24h
  0041B5EC: xor         bl,bl
  0041B5EE: mov         eax,dword ptr [esp+18h]
  0041B5F2: mov         ecx,dword ptr [edi+006FDD40h]
  0041B5F8: inc         eax
  0041B5F9: cmp         eax,ecx
  0041B5FB: mov         dword ptr [esp+18h],eax
  0041B5FF: jl          0041B580
  0041B605: mov         esi,dword ptr [esp+1Ch]
  0041B609: mov         eax,dword ptr [esp+14h]
  0041B60D: mov         ecx,dword ptr [esi+00001900h]
  0041B613: inc         eax
  0041B614: add         edi,40h
  0041B617: cmp         eax,ecx
  0041B619: mov         dword ptr [esp+14h],eax
  0041B61D: jl          0041B55D
  0041B623: mov         edx,dword ptr [esp+20h]
  0041B627: mov         ecx,76Ch
  0041B62C: xor         eax,eax
  0041B62E: lea         edi,[esi+00002CD4h]
  0041B634: rep stos    dword ptr es:[edi]
  0041B636: push        edx
  0041B637: call        00561994
  0041B63C: add         esp,4
  0041B63F: mov         al,bl
  0041B641: pop         edi
  0041B642: pop         esi
  0041B643: pop         ebp
  0041B644: pop         ebx
  0041B645: add         esp,218h
  0041B64B: ret         4
  0041B64E: mov         edi,edi
  0041B650: 14

  Summary

      16E000 .text
