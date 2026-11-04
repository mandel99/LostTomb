
Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  00564A39: mov         edx,dword ptr ds:[01267AA0h]
  00564A3F: push        ebx
  00564A40: push        ebp
  00564A41: push        esi
  00564A42: xor         ebp,ebp
  00564A44: xor         esi,esi
  00564A46: xor         eax,eax
  00564A48: cmp         edx,ebp
  00564A4A: push        edi
  00564A4B: jle         00564AAA
  00564A4D: mov         ebx,dword ptr ds:[01266A90h]
  00564A53: mov         edi,ebx
  00564A55: mov         ecx,dword ptr [edi]
  00564A57: cmp         ecx,ebp
  00564A59: je          00564A70
  00564A5B: test        byte ptr [ecx+0Ch],83h
  00564A5F: je          00564A6B
  00564A61: inc         eax
  00564A62: add         edi,4
  00564A65: cmp         eax,edx
  00564A67: jl          00564A55
  00564A69: jmp         00564AAA
  00564A6B: mov         esi,dword ptr [ebx+eax*4]
  00564A6E: jmp         00564A94
  00564A70: mov         edi,eax
  00564A72: push        20h
  00564A74: shl         edi,2
  00564A77: call        00561B68
  00564A7C: pop         ecx
  00564A7D: mov         ecx,dword ptr ds:[01266A90h]
  00564A83: mov         dword ptr [edi+ecx],eax
  00564A86: mov         eax,dword ptr ds:[01266A90h]
  00564A8B: mov         edi,dword ptr [edi+eax]
  00564A8E: cmp         edi,ebp
  00564A90: je          00564AAA
  00564A92: mov         esi,edi
  00564A94: cmp         esi,ebp
  00564A96: je          00564AAA
  00564A98: or          dword ptr [esi+10h],0FFFFFFFFh
  00564A9C: mov         dword ptr [esi+4],ebp
  00564A9F: mov         dword ptr [esi+0Ch],ebp
  00564AA2: mov         dword ptr [esi+8],ebp
  00564AA5: mov         dword ptr [esi],ebp
  00564AA7: mov         dword ptr [esi+1Ch],ebp
  00564AAA: mov         eax,esi
  00564AAC: pop         edi
  00564AAD: pop         esi
  00564AAE: pop         ebp
  00564AAF: pop         ebx
  00564AB0: ret

  Summary

      16E000 .text
