Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  00417FF0: mov         eax,dword ptr ds:[00D3B198h]
  00417FF5: push        ebx
  00417FF6: xor         ebx,ebx
  00417FF8: mov         ecx,1
  00417FFD: cmp         eax,4
  00418000: mov         byte ptr ds:[00E38EA8h],bl
  00418006: mov         dword ptr ds:[005D2A2Ch],ecx
  0041800C: ja          004180A7
  00418012: jmp         dword ptr [eax*4+004180ACh]
  00418019: call        00516BB0
  0041801E: mov         dword ptr ds:[00F53F78h],ebx
  00418024: pop         ebx
  00418025: ret
  00418026: mov         eax,dword ptr ds:[00E39140h]
  0041802B: push        6
  0041802D: push        eax
  0041802E: call        dword ptr ds:[0056F178h]
  00418034: mov         ecx,dword ptr ds:[00E39140h]
  0041803A: push        5
  0041803C: push        ebx
  0041803D: push        ebx
  0041803E: push        57BFB0h
  00418043: push        5761D8h
  00418048: push        ecx
  00418049: call        dword ptr ds:[0056F16Ch]
  0041804F: pop         ebx
  00418050: ret
  00418051: mov         dword ptr ds:[00E38EBCh],2
  0041805B: mov         dword ptr ds:[00605A3Ch],ecx
  00418061: mov         byte ptr ds:[00E38E60h],bl
  00418067: mov         dword ptr ds:[00F53F78h],ebx
  0041806D: mov         dword ptr ds:[00704DB0h],ebx
  00418073: mov         dword ptr ds:[00704DACh],3
  0041807D: call        0041F870
  00418082: pop         ebx
  00418083: ret
  00418084: call        004C5920
  00418089: mov         byte ptr ds:[00E38DDAh],53h
  00418090: pop         ebx
  00418091: ret
  00418092: mov         edx,dword ptr ds:[00E39140h]
  00418098: push        ebx
  00418099: push        1Bh
  0041809B: push        100h
  004180A0: push        edx
  004180A1: call        dword ptr ds:[0056F174h]
  004180A7: pop         ebx
  004180A8: ret
  004180A9: lea         ecx,[ecx]
  004180AC: sbb         dword ptr [eax+80260041h],eax
  004180B2: inc         ecx
  004180B3: add         byte ptr [ecx-80h],dl
  004180B6: inc         ecx
  004180B7: add         byte ptr [eax+eax*4+80920041h],al
  004180BE: inc         ecx
  004180BF: 00
  004180C0: push        ebx

  Summary

      16E000 .text
