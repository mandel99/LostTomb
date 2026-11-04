
Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

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
  0041B014: C6
  0041B015: inc         esp

  Summary

      16E000 .text
