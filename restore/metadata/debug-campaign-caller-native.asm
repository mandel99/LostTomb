
Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  00417DD0: mov         edx,dword ptr ds:[00EAAC28h]
  00417DD6: mov         eax,dword ptr ds:[00E39080h]
  00417DDB: imul        edx,edx,49D8h
  00417DE1: mov         ecx,dword ptr [eax*4+005D1988h]
  00417DE8: mov         al,byte ptr [esp+4]
  00417DEC: push        ebx
  00417DED: mov         byte ptr ds:[00E38EA9h],al
  00417DF2: xor         ebx,ebx
  00417DF4: mov         dword ptr ds:[00E38EB0h],ecx
  00417DFA: movsx       eax,al
  00417DFD: push        ebx
  00417DFE: push        0FFFFFFFFh
  00417E00: push        eax
  00417E01: mov         ecx,6FDD08h
  00417E06: mov         dword ptr ds:[00E38EB4h],ebx
  00417E0C: mov         dword ptr ds:[00E38EB8h],ebx
  00417E12: mov         dword ptr [edx+00EA5CC0h],ebx
  00417E18: call        0041B920
  00417E1D: mov         byte ptr ds:[00E38EABh],al
  00417E22: movsx       eax,al
  00417E25: cmp         eax,ebx
  00417E27: jl          00417E55
  00417E29: cmp         eax,dword ptr ds:[006FF608h]
  00417E2F: jge         00417E55
  00417E31: shl         eax,6
  00417E34: add         eax,6FDD08h
  00417E39: cmp         eax,ebx
  00417E3B: je          00417E55
  00417E3D: mov         cl,byte ptr [eax+0Ch]
  00417E40: mov         byte ptr ds:[007027A4h],bl
  00417E46: mov         byte ptr ds:[00E38EA8h],cl
  00417E4C: mov         byte ptr ds:[00E38EAAh],0FFh
  00417E53: pop         ebx
  00417E54: ret
  00417E55: mov         byte ptr ds:[00E38EA8h],bl
  00417E5B: mov         byte ptr ds:[007027A4h],bl
  00417E61: mov         byte ptr ds:[00E38EAAh],0FFh
  00417E68: pop         ebx
  00417E69: ret
  00417E6A: nop
  00417E6B: nop
  00417E6C: nop
  00417E6D: nop
  00417E6E: nop
  00417E6F: nop
  00417E70: C7

  Summary

      16E000 .text
