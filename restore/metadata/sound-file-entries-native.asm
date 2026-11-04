Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  00560210: push        ebp
  00560211: mov         ebp,esp
  00560213: push        ecx
  00560214: mov         dword ptr [ebp-4],ecx
  00560217: mov         eax,dword ptr [ebp+8]
  0056021A: and         eax,2
  0056021D: test        eax,eax
  0056021F: je          00560259
  00560221: push        5602B0h
  00560226: mov         ecx,dword ptr [ebp-4]
  00560229: mov         edx,dword ptr [ecx-4]
  0056022C: push        edx
  0056022D: push        8
  0056022F: mov         eax,dword ptr [ebp-4]
  00560232: push        eax
  00560233: call        00561363
  00560238: mov         ecx,dword ptr [ebp+8]
  0056023B: and         ecx,1
  0056023E: test        ecx,ecx
  00560240: je          00560251
  00560242: mov         edx,dword ptr [ebp-4]
  00560245: sub         edx,4
  00560248: push        edx
  00560249: call        00561A2B
  0056024E: add         esp,4
  00560251: mov         eax,dword ptr [ebp-4]
  00560254: sub         eax,4
  00560257: jmp         0056027A
  00560259: mov         ecx,dword ptr [ebp-4]
  0056025C: call        005602B0
  00560261: mov         eax,dword ptr [ebp+8]
  00560264: and         eax,1
  00560267: test        eax,eax
  00560269: je          00560277
  0056026B: mov         ecx,dword ptr [ebp-4]
  0056026E: push        ecx
  0056026F: call        00561A2B
  00560274: add         esp,4
  00560277: mov         eax,dword ptr [ebp-4]
  0056027A: mov         esp,ebp
  0056027C: pop         ebp
  0056027D: ret         4
  00560280: push        ebp
  00560281: mov         ebp,esp
  00560283: push        ecx
  00560284: mov         dword ptr [ebp-4],ecx
  00560287: mov         eax,dword ptr [ebp-4]
  0056028A: mov         dword ptr [eax+4],0
  00560291: mov         ecx,dword ptr [ebp-4]
  00560294: mov         dword ptr [ecx],0
  0056029A: mov         eax,dword ptr [ebp-4]
  0056029D: mov         esp,ebp
  0056029F: pop         ebp
  005602A0: ret
  005602A1: int         3
  005602A2: int         3
  005602A3: int         3
  005602A4: int         3
  005602A5: int         3
  005602A6: int         3
  005602A7: int         3
  005602A8: int         3
  005602A9: int         3
  005602AA: int         3
  005602AB: int         3
  005602AC: int         3
  005602AD: int         3
  005602AE: int         3
  005602AF: int         3
  005602B0: push        ebp
  005602B1: mov         ebp,esp
  005602B3: push        ecx
  005602B4: mov         dword ptr [ebp-4],ecx
  005602B7: mov         eax,dword ptr [ebp-4]
  005602BA: cmp         dword ptr [eax+4],0
  005602BE: je          005602DF
  005602C0: mov         ecx,dword ptr [ebp-4]
  005602C3: cmp         dword ptr [ecx],0
  005602C6: je          005602DF
  005602C8: mov         edx,dword ptr [ebp-4]
  005602CB: mov         eax,dword ptr [edx]
  005602CD: push        eax
  005602CE: call        00561BDC
  005602D3: add         esp,4
  005602D6: mov         ecx,dword ptr [ebp-4]
  005602D9: mov         dword ptr [ecx],0
  005602DF: mov         esp,ebp

  Summary

      16E000 .text
