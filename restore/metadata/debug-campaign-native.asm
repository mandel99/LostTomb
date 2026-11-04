
Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  0041B920: push        ebx
  0041B921: push        ebp
  0041B922: push        esi
  0041B923: mov         esi,dword ptr [esp+14h]
  0041B927: test        esi,esi
  0041B929: push        edi
  0041B92A: jge         0041B933
  0041B92C: or          esi,0FFFFFFFFh
  0041B92F: xor         edi,edi
  0041B931: jmp         0041B962
  0041B933: cmp         esi,dword ptr [ecx+00001900h]
  0041B939: jge         0041B9B9
  0041B93B: mov         eax,esi
  0041B93D: shl         eax,6
  0041B940: mov         edx,dword ptr [eax+ecx+38h]
  0041B944: test        edx,edx
  0041B946: jle         0041B95A
  0041B948: mov         edx,dword ptr [esp+1Ch]
  0041B94C: lea         edx,[edx+edx*2]
  0041B94F: lea         eax,[eax+edx*4]
  0041B952: xor         edx,edx
  0041B954: mov         dl,byte ptr [eax+ecx+2Ah]
  0041B958: jmp         0041B960
  0041B95A: xor         edx,edx
  0041B95C: mov         dl,byte ptr [eax+ecx+1]
  0041B960: mov         edi,edx
  0041B962: mov         eax,dword ptr [ecx+00001900h]
  0041B968: lea         ebp,[esi+1]
  0041B96B: cmp         ebp,eax
  0041B96D: jge         0041B9B9
  0041B96F: mov         edx,ebp
  0041B971: shl         edx,6
  0041B974: lea         edx,[edx+ecx+1]
  0041B978: mov         esi,dword ptr [esp+14h]
  0041B97C: xor         ecx,ecx
  0041B97E: mov         cl,byte ptr [edx-1]
  0041B981: cmp         ecx,esi
  0041B983: jne         0041B9B1
  0041B985: mov         cl,byte ptr [edx]
  0041B987: test        cl,cl
  0041B989: je          0041B9C3
  0041B98B: and         ecx,0FFh
  0041B991: cmp         ecx,edi
  0041B993: je          0041B9C3
  0041B995: xor         ebx,ebx
  0041B997: xor         ecx,ecx
  0041B999: mov         bl,byte ptr [edx+6]
  0041B99C: mov         esi,ebx
  0041B99E: test        esi,esi
  0041B9A0: jle         0041B9B1
  0041B9A2: xor         ebx,ebx
  0041B9A4: mov         bl,byte ptr [edx+ecx+1]
  0041B9A8: cmp         ebx,edi
  0041B9AA: je          0041B9C3
  0041B9AC: inc         ecx
  0041B9AD: cmp         ecx,esi
  0041B9AF: jl          0041B9A2
  0041B9B1: inc         ebp
  0041B9B2: add         edx,40h
  0041B9B5: cmp         ebp,eax
  0041B9B7: jl          0041B978
  0041B9B9: pop         edi
  0041B9BA: pop         esi
  0041B9BB: pop         ebp
  0041B9BC: or          eax,0FFFFFFFFh
  0041B9BF: pop         ebx
  0041B9C0: ret         0Ch
  0041B9C3: pop         edi
  0041B9C4: mov         eax,ebp
  0041B9C6: pop         esi
  0041B9C7: pop         ebp
  0041B9C8: pop         ebx
  0041B9C9: ret         0Ch
  0041B9CC: nop
  0041B9CD: nop
  0041B9CE: nop
  0041B9CF: nop
  0041B9D0: 8B

  Summary

      16E000 .text
