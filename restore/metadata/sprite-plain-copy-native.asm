
Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  0040FB60: push        ebp
  0040FB61: mov         ebp,esp
  0040FB63: push        esi
  0040FB64: push        edi
  0040FB65: mov         esi,dword ptr [ebp+8]
  0040FB68: mov         edi,dword ptr ds:[00E92E18h]
  0040FB6E: mov         eax,dword ptr ds:[00D3B5E0h]
  0040FB73: add         eax,eax
  0040FB75: add         edi,eax
  0040FB77: mov         eax,dword ptr ds:[00D3B5DCh]
  0040FB7C: mov         edx,dword ptr ds:[00E38D94h]
  0040FB82: mul         eax,edx
  0040FB84: add         edi,eax
  0040FB86: movsx       eax,byte ptr ds:[00D3B544h]
  0040FB8D: and         eax,3
  0040FB90: je          0040FBB3
  0040FB92: mov         edx,dword ptr ds:[00D3B5D4h]
  0040FB98: mov         ecx,dword ptr ds:[00D3B544h]
  0040FB9E: add         ecx,ecx
  0040FBA0: rep movs    byte ptr es:[edi],byte ptr [esi]
  0040FBA2: add         esi,dword ptr ds:[00D3B540h]
  0040FBA8: add         edi,dword ptr ds:[00D3B53Ch]
  0040FBAE: dec         edx
  0040FBAF: jg          0040FB98
  0040FBB1: jmp         0040FBD2
  0040FBB3: mov         edx,dword ptr ds:[00D3B5D4h]
  0040FBB9: mov         ecx,dword ptr ds:[00D3B544h]
  0040FBBF: shr         ecx,1
  0040FBC1: rep movs    dword ptr es:[edi],dword ptr [esi]
  0040FBC3: add         esi,dword ptr ds:[00D3B540h]
  0040FBC9: add         edi,dword ptr ds:[00D3B53Ch]
  0040FBCF: dec         edx
  0040FBD0: jg          0040FBB9
  0040FBD2: pop         edi
  0040FBD3: pop         esi
  0040FBD4: pop         ebp
  0040FBD5: ret
  0040FBD6: nop
  0040FBD7: nop
  0040FBD8: nop
  0040FBD9: nop
  0040FBDA: nop
  0040FBDB: nop
  0040FBDC: nop
  0040FBDD: nop
  0040FBDE: nop
  0040FBDF: nop
  0040FBE0: push        ebp
  0040FBE1: mov         ebp,esp
  0040FBE3: push        esi
  0040FBE4: push        edi
  0040FBE5: mov         esi,dword ptr [ebp+8]
  0040FBE8: mov         edi,dword ptr ds:[00E92E18h]
  0040FBEE: mov         eax,dword ptr ds:[00D3B5E0h]
  0040FBF3: add         eax,eax
  0040FBF5: add         edi,eax
  0040FBF7: mov         eax,dword ptr ds:[00D3B5DCh]
  0040FBFC: mov         edx,dword ptr ds:[00E38D94h]
  0040FC02: mul         eax,edx
  0040FC04: add         edi,eax
  0040FC06: mov         eax,dword ptr [esi]
  0040FC08: mov         dword ptr [edi],eax
  0040FC0A: pop         edi
  0040FC0B: pop         esi
  0040FC0C: pop         ebp
  0040FC0D: ret
  0040FC0E: nop
  0040FC0F: nop
  0040FC10: push        ebp
  0040FC11: mov         ebp,esp
  0040FC13: push        ebx
  0040FC14: push        edi
  0040FC15: mov         ebx,dword ptr [ebp+8]
  0040FC18: mov         edi,dword ptr ds:[00E92E18h]
  0040FC1E: mov         eax,dword ptr ds:[00D3B5E0h]
  0040FC23: add         eax,eax
  0040FC25: add         edi,eax
  0040FC27: mov         eax,dword ptr ds:[00D3B5DCh]
  0040FC2C: mov         edx,dword ptr ds:[00E38D94h]
  0040FC32: mul         eax,edx
  0040FC34: add         edi,eax
  0040FC36: mov         dword ptr [edi],ebx
  0040FC38: pop         edi
  0040FC39: pop         ebx
  0040FC3A: pop         ebp
  0040FC3B: ret
  0040FC3C: nop
  0040FC3D: nop
  0040FC3E: nop
  0040FC3F: nop
  0040FC40: 8B

  Summary

      16E000 .text
