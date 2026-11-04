
Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  004303A0: mov         ecx,dword ptr [esp+18h]
  004303A4: mov         eax,dword ptr [esp+8]
  004303A8: mov         edx,dword ptr [esp+10h]
  004303AC: mov         dword ptr ds:[0070E460h],ecx
  004303B2: mov         ecx,dword ptr [esp+1Ch]
  004303B6: mov         dword ptr ds:[0070E46Ch],eax
  004303BB: mov         eax,ecx
  004303BD: mov         dword ptr ds:[0070E48Ch],edx
  004303C3: cdq
  004303C4: sub         eax,edx
  004303C6: push        ebx
  004303C7: sar         eax,1
  004303C9: mov         dword ptr ds:[0070E45Ch],eax
  004303CE: add         eax,ecx
  004303D0: push        ebp
  004303D1: mov         dword ptr ds:[0070E454h],eax
  004303D6: push        esi
  004303D7: mov         esi,dword ptr [esp+2Ch]
  004303DB: lea         eax,[ecx+ecx]
  004303DE: mov         ebx,dword ptr [esp+20h]
  004303E2: mov         dword ptr ds:[0070E450h],eax
  004303E7: mov         eax,esi
  004303E9: cdq
  004303EA: sub         eax,edx
  004303EC: push        edi
  004303ED: mov         edi,dword ptr [esp+14h]
  004303F1: mov         dword ptr ds:[0070E458h],ecx
  004303F7: sar         eax,1
  004303F9: add         ecx,2
  004303FC: mov         ebp,dword ptr [esp+1Ch]
  00430400: lea         edx,[eax+esi]
  00430403: imul        ecx,ebx
  00430406: mov         dword ptr ds:[0070E444h],edx
  0043040C: lea         edx,[esi+esi]
  0043040F: mov         dword ptr ds:[0070E440h],edx
  00430415: mov         edx,edi
  00430417: sub         edx,ebx
  00430419: lea         ecx,[ecx+ebp-2]
  0043041D: mov         dword ptr ds:[0070E468h],edx
  00430423: mov         edx,dword ptr [esp+28h]
  00430427: dec         edx
  00430428: mov         dword ptr ds:[0070E438h],ecx
  0043042E: imul        edx,eax
  00430431: mov         ecx,dword ptr [esp+20h]
  00430435: mov         dword ptr ds:[0070E44Ch],eax
  0043043A: add         edx,ecx
  0043043C: mov         eax,edi
  0043043E: mov         dword ptr ds:[0070E430h],edx
  00430444: mov         dword ptr ds:[0070E470h],edi
  0043044A: cdq
  0043044B: sub         eax,edx
  0043044D: mov         dword ptr ds:[0070E448h],esi
  00430453: sar         eax,1
  00430455: mov         dword ptr ds:[0070E478h],eax
  0043045A: mov         eax,dword ptr [esp+18h]
  0043045E: pop         edi
  0043045F: mov         dword ptr ds:[0070E490h],ebp
  00430465: mov         dword ptr ds:[0070E43Ch],ebp
  0043046B: pop         esi
  0043046C: mov         dword ptr ds:[0070E464h],ebx
  00430472: pop         ebp
  00430473: mov         dword ptr ds:[0070E434h],ecx
  00430479: mov         dword ptr ds:[0070E474h],eax
  0043047E: pop         ebx
  0043047F: ret
  00430480: mov         eax,dword ptr ds:[00F56B44h]
  00430485: push        edi
  00430486: mov         ecx,19C5Fh
  0043048B: add         eax,0FFF0000h
  00430490: mov         edi,70E494h
  00430495: mov         dword ptr ds:[0070E424h],0E6h
  0043049F: rep stos    dword ptr es:[edi]
  004304A1: xor         eax,eax
  004304A3: mov         dword ptr ds:[0070E420h],1C8h
  004304AD: mov         dword ptr ds:[0070E42Ch],eax
  004304B2: mov         dword ptr ds:[0070E428h],eax
  004304B7: pop         edi
  004304B8: ret
  004304B9: nop
  004304BA: nop
  004304BB: nop
  004304BC: nop
  004304BD: nop
  004304BE: nop
  004304BF: nop
  004304C0: push        ebx
  004304C1: mov         ebx,dword ptr [esp+8]
  004304C5: push        esi
  004304C6: push        edi
  004304C7: mov         dword ptr ds:[00D3B4E0h],0

  Summary

      16E000 .text
