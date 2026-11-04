Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  0055EB99: push        ebp
  0055EB9A: mov         ebp,esp
  0055EB9C: push        ecx
  0055EB9D: mov         dword ptr [ebp-4],ecx
  0055EBA0: mov         eax,dword ptr [ebp-4]
  0055EBA3: cmp         dword ptr [eax+00000200h],0
  0055EBAA: je          0055EBB3
  0055EBAC: mov         eax,1
  0055EBB1: jmp         0055EBCE
  0055EBB3: mov         ecx,dword ptr [ebp-4]
  0055EBB6: mov         edx,dword ptr [ecx+0000019Ch]
  0055EBBC: mov         eax,dword ptr [ebp+8]
  0055EBBF: mov         ecx,dword ptr [edx+eax*4]
  0055EBC2: push        ecx
  0055EBC3: call        dword ptr ds:[0056F2CCh]
  0055EBC9: mov         eax,1
  0055EBCE: mov         esp,ebp
  0055EBD0: pop         ebp
  0055EBD1: ret         4
  0055EBD4: push        ebp
  0055EBD5: mov         ebp,esp
  0055EBD7: push        ecx
  0055EBD8: mov         dword ptr [ebp-4],ecx
  0055EBDB: mov         eax,dword ptr [ebp-4]
  0055EBDE: cmp         dword ptr [eax+00000200h],0
  0055EBE5: je          0055EBEE
  0055EBE7: mov         eax,1
  0055EBEC: jmp         0055EC09
  0055EBEE: mov         ecx,dword ptr [ebp-4]
  0055EBF1: mov         edx,dword ptr [ecx+000001F0h]
  0055EBF7: mov         eax,dword ptr [ebp+8]
  0055EBFA: mov         ecx,dword ptr [edx+eax*4]
  0055EBFD: push        ecx
  0055EBFE: call        dword ptr ds:[0056F2C8h]
  0055EC04: mov         eax,1
  0055EC09: mov         esp,ebp
  0055EC0B: pop         ebp
  0055EC0C: ret         4
  0055EC0F: push        ebp
  0055EC10: mov         ebp,esp
  0055EC12: sub         esp,8
  0055EC15: mov         dword ptr [ebp-8],ecx
  0055EC18: mov         eax,dword ptr [ebp-8]
  0055EC1B: cmp         dword ptr [eax+00000200h],0
  0055EC22: je          0055EC2B
  0055EC24: mov         eax,1
  0055EC29: jmp         0055EC78
  0055EC2B: mov         dword ptr [ebp-4],0
  0055EC32: jmp         0055EC3D
  0055EC34: mov         ecx,dword ptr [ebp-4]
  0055EC37: add         ecx,1
  0055EC3A: mov         dword ptr [ebp-4],ecx
  0055EC3D: mov         edx,dword ptr [ebp-8]
  0055EC40: mov         eax,dword ptr [ebp-4]
  0055EC43: cmp         eax,dword ptr [edx+00000198h]
  0055EC49: jge         0055EC73
  0055EC4B: mov         ecx,dword ptr [ebp-8]
  0055EC4E: mov         edx,dword ptr [ecx+0000019Ch]
  0055EC54: mov         eax,dword ptr [ebp-4]
  0055EC57: mov         ecx,dword ptr [edx+eax*4]
  0055EC5A: push        ecx
  0055EC5B: call        dword ptr ds:[0056F2C4h]
  0055EC61: test        eax,eax
  0055EC63: jne         0055EC71
  0055EC65: mov         edx,dword ptr [ebp-4]
  0055EC68: push        edx
  0055EC69: mov         ecx,dword ptr [ebp-8]
  0055EC6C: call        0055EB99
  0055EC71: jmp         0055EC34
  0055EC73: mov         eax,1
  0055EC78: mov         esp,ebp
  0055EC7A: pop         ebp
  0055EC7B: ret
  0055EC7C: push        ebp
  0055EC7D: mov         ebp,esp
  0055EC7F: sub         esp,8
  0055EC82: mov         dword ptr [ebp-8],ecx
  0055EC85: mov         eax,dword ptr [ebp-8]
  0055EC88: cmp         dword ptr [eax+00000200h],0
  0055EC8F: je          0055EC93
  0055EC91: jmp         0055ECEF
  0055EC93: mov         dword ptr [ebp-4],0
  0055EC9A: jmp         0055ECA5
  0055EC9C: mov         ecx,dword ptr [ebp-4]
  0055EC9F: add         ecx,1
  0055ECA2: mov         dword ptr [ebp-4],ecx
  0055ECA5: mov         edx,dword ptr [ebp-8]
  0055ECA8: mov         eax,dword ptr [ebp-4]
  0055ECAB: cmp         eax,dword ptr [edx+00000198h]
  0055ECB1: jge         0055ECC1
  0055ECB3: mov         ecx,dword ptr [ebp-4]
  0055ECB6: push        ecx
  0055ECB7: mov         ecx,dword ptr [ebp-8]
  0055ECBA: call        0055EB99
  0055ECBF: jmp         0055EC9C
  0055ECC1: mov         dword ptr [ebp-4],0
  0055ECC8: jmp         0055ECD3
  0055ECCA: mov         edx,dword ptr [ebp-4]
  0055ECCD: add         edx,1
  0055ECD0: mov         dword ptr [ebp-4],edx
  0055ECD3: mov         eax,dword ptr [ebp-8]
  0055ECD6: mov         ecx,dword ptr [ebp-4]
  0055ECD9: cmp         ecx,dword ptr [eax+000001E4h]
  0055ECDF: jge         0055ECEF
  0055ECE1: mov         edx,dword ptr [ebp-4]
  0055ECE4: push        edx
  0055ECE5: mov         ecx,dword ptr [ebp-8]
  0055ECE8: call        0055EBD4
  0055ECED: jmp         0055ECCA
  0055ECEF: mov         esp,ebp
  0055ECF1: pop         ebp
  0055ECF2: ret
  0055ECF3: push        ebp

  Summary

      16E000 .text
