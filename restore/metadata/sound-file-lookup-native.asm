Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  0055DD3F: push        ebp
  0055DD40: mov         ebp,esp
  0055DD42: push        0FFFFFFFFh
  0055DD44: push        56E15Eh
  0055DD49: mov         eax,dword ptr fs:[00000000h]
  0055DD4F: push        eax
  0055DD50: mov         dword ptr fs:[00000000h],esp
  0055DD57: sub         esp,24h
  0055DD5A: mov         dword ptr [ebp-30h],ecx
  0055DD5D: lea         ecx,[ebp-14h]
  0055DD60: call        00560280
  0055DD65: mov         dword ptr [ebp-4],0
  0055DD6C: mov         eax,dword ptr [ebp-30h]
  0055DD6F: mov         ecx,dword ptr [eax+4]
  0055DD72: mov         dword ptr [ebp-18h],ecx
  0055DD75: mov         edx,dword ptr [ebp-30h]
  0055DD78: mov         eax,dword ptr [ebp-30h]
  0055DD7B: mov         ecx,dword ptr [edx+4]
  0055DD7E: cmp         ecx,dword ptr [eax]
  0055DD80: jl          0055DDA0
  0055DD82: mov         dword ptr [ebp-20h],0FFFFFFFFh
  0055DD89: mov         dword ptr [ebp-4],0FFFFFFFFh
  0055DD90: lea         ecx,[ebp-14h]
  0055DD93: call        005602B0
  0055DD98: mov         eax,dword ptr [ebp-20h]
  0055DD9B: jmp         0055DE92
  0055DDA0: mov         edx,dword ptr [ebp-30h]
  0055DDA3: cmp         dword ptr [edx+8],0
  0055DDA7: jne         0055DDC7
  0055DDA9: mov         dword ptr [ebp-24h],0FFFFFFFFh
  0055DDB0: mov         dword ptr [ebp-4],0FFFFFFFFh
  0055DDB7: lea         ecx,[ebp-14h]
  0055DDBA: call        005602B0
  0055DDBF: mov         eax,dword ptr [ebp-24h]
  0055DDC2: jmp         0055DE92
  0055DDC7: mov         dword ptr [ebp-1Ch],0
  0055DDCE: jmp         0055DDD9
  0055DDD0: mov         eax,dword ptr [ebp-1Ch]
  0055DDD3: add         eax,1
  0055DDD6: mov         dword ptr [ebp-1Ch],eax
  0055DDD9: mov         ecx,dword ptr [ebp-30h]
  0055DDDC: mov         edx,dword ptr [ebp-1Ch]
  0055DDDF: cmp         edx,dword ptr [ecx+4]
  0055DDE2: jge         0055DE2C
  0055DDE4: mov         eax,dword ptr [ebp-30h]
  0055DDE7: mov         ecx,dword ptr [eax+8]
  0055DDEA: mov         edx,dword ptr [ebp-1Ch]
  0055DDED: cmp         dword ptr [ecx+edx*8],0
  0055DDF1: je          0055DE2A
  0055DDF3: mov         eax,dword ptr [ebp+8]
  0055DDF6: push        eax
  0055DDF7: mov         ecx,dword ptr [ebp-30h]
  0055DDFA: mov         edx,dword ptr [ecx+8]
  0055DDFD: mov         eax,dword ptr [ebp-1Ch]
  0055DE00: mov         ecx,dword ptr [edx+eax*8]
  0055DE03: push        ecx
  0055DE04: call        005631B0
  0055DE09: add         esp,8
  0055DE0C: test        eax,eax
  0055DE0E: jne         0055DE2A
  0055DE10: mov         edx,dword ptr [ebp-1Ch]
  0055DE13: mov         dword ptr [ebp-28h],edx
  0055DE16: mov         dword ptr [ebp-4],0FFFFFFFFh
  0055DE1D: lea         ecx,[ebp-14h]
  0055DE20: call        005602B0
  0055DE25: mov         eax,dword ptr [ebp-28h]
  0055DE28: jmp         0055DE92
  0055DE2A: jmp         0055DDD0
  0055DE2C: cmp         dword ptr [ebp+0Ch],0
  0055DE30: je          0055DE4C
  0055DE32: mov         eax,dword ptr [ebp+8]
  0055DE35: push        eax
  0055DE36: call        0056B177
  0055DE3B: add         esp,4
  0055DE3E: mov         ecx,dword ptr [ebp-30h]
  0055DE41: mov         edx,dword ptr [ecx+8]
  0055DE44: mov         ecx,dword ptr [ebp-18h]
  0055DE47: mov         dword ptr [edx+ecx*8],eax
  0055DE4A: jmp         0055DE5B
  0055DE4C: mov         edx,dword ptr [ebp-30h]
  0055DE4F: mov         eax,dword ptr [edx+8]
  0055DE52: mov         ecx,dword ptr [ebp-18h]
  0055DE55: mov         edx,dword ptr [ebp+8]
  0055DE58: mov         dword ptr [eax+ecx*8],edx
  0055DE5B: mov         eax,dword ptr [ebp-30h]
  0055DE5E: mov         ecx,dword ptr [eax+8]
  0055DE61: mov         edx,dword ptr [ebp-18h]
  0055DE64: mov         eax,dword ptr [ebp+0Ch]
  0055DE67: mov         dword ptr [ecx+edx*8+4],eax
  0055DE6B: mov         ecx,dword ptr [ebp-30h]
  0055DE6E: mov         edx,dword ptr [ecx+4]
  0055DE71: add         edx,1
  0055DE74: mov         eax,dword ptr [ebp-30h]
  0055DE77: mov         dword ptr [eax+4],edx
  0055DE7A: mov         ecx,dword ptr [ebp-18h]
  0055DE7D: mov         dword ptr [ebp-2Ch],ecx
  0055DE80: mov         dword ptr [ebp-4],0FFFFFFFFh
  0055DE87: lea         ecx,[ebp-14h]
  0055DE8A: call        005602B0
  0055DE8F: mov         eax,dword ptr [ebp-2Ch]
  0055DE92: mov         ecx,dword ptr [ebp-0Ch]
  0055DE95: mov         dword ptr fs:[00000000h],ecx
  0055DE9C: mov         esp,ebp
  0055DE9E: pop         ebp
  0055DE9F: ret         8
  0055DEA2: push        ebp

  Summary

      16E000 .text
