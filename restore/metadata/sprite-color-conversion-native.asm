
Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  004CDEB0: push        ebp
  004CDEB1: mov         ebp,esp
  004CDEB3: cmp         dword ptr ds:[00E92DFCh],2
  004CDEBA: jne         004CDF17
  004CDEBC: push        ebx
  004CDEBD: push        esi
  004CDEBE: mov         esi,dword ptr [ebp+8]
  004CDEC1: mov         edx,dword ptr [ebp+0Ch]
  004CDEC4: cmp         edx,0
  004CDEC7: je          004CDF15
  004CDEC9: cmp         edx,0
  004CDECC: jl          004CDF07
  004CDECE: mov         ecx,0
  004CDED3: mov         cl,byte ptr [esi]
  004CDED5: cmp         cl,0FFh
  004CDED8: je          004CDF0D
  004CDEDA: add         esi,1
  004CDEDD: sub         edx,1
  004CDEE0: cmp         cl,0
  004CDEE3: jle         004CDEC4
  004CDEE5: mov         ax,word ptr [esi]
  004CDEE8: mov         bx,ax
  004CDEEB: and         ax,7FE0h
  004CDEEF: and         bx,1Fh
  004CDEF3: shl         ax,1
  004CDEF6: or          ax,bx
  004CDEF9: mov         word ptr [esi],ax
  004CDEFC: add         esi,2
  004CDEFF: sub         edx,2
  004CDF02: sub         cl,1
  004CDF05: jmp         004CDEE0
  004CDF07: mov         ax,0
  004CDF0B: jmp         004CDF15
  004CDF0D: add         esi,2
  004CDF10: sub         edx,2
  004CDF13: jmp         004CDEC4
  004CDF15: pop         esi
  004CDF16: pop         ebx
  004CDF17: pop         ebp
  004CDF18: ret
  004CDF19: nop
  004CDF1A: nop
  004CDF1B: nop
  004CDF1C: nop
  004CDF1D: nop
  004CDF1E: nop
  004CDF1F: nop
  004CDF20: push        ebp
  004CDF21: mov         ebp,esp
  004CDF23: cmp         dword ptr ds:[00E92DFCh],2
  004CDF2A: jne         004CDF65
  004CDF2C: push        ebx
  004CDF2D: push        esi
  004CDF2E: mov         esi,dword ptr [ebp+8]
  004CDF31: mov         edx,dword ptr [ebp+0Ch]
  004CDF34: cmp         edx,0
  004CDF37: jle         004CDF63
  004CDF39: mov         eax,0
  004CDF3E: mov         ax,word ptr [esi]
  004CDF41: cmp         ax,0F81Fh
  004CDF45: je          004CDF5B
  004CDF47: mov         bx,ax
  004CDF4A: and         ax,0FFE0h
  004CDF4E: and         bx,1Fh
  004CDF52: shl         ax,1
  004CDF55: or          ax,bx
  004CDF58: mov         word ptr [esi],ax
  004CDF5B: add         esi,2
  004CDF5E: sub         edx,2
  004CDF61: jmp         004CDF34
  004CDF63: pop         esi
  004CDF64: pop         ebx
  004CDF65: pop         ebp
  004CDF66: ret
  004CDF67: nop
  004CDF68: nop
  004CDF69: nop
  004CDF6A: nop
  004CDF6B: nop
  004CDF6C: nop
  004CDF6D: nop
  004CDF6E: nop
  004CDF6F: nop
  004CDF70: call        004CDF80
  004CDF75: jmp         004CDF90
  004CDF7A: nop
  004CDF7B: nop
  004CDF7C: nop
  004CDF7D: nop
  004CDF7E: nop
  004CDF7F: nop
  004CDF80: mov         ecx,0E92BF8h
  004CDF85: jmp         0055E38B
  004CDF8A: nop
  004CDF8B: nop
  004CDF8C: nop
  004CDF8D: nop
  004CDF8E: nop
  004CDF8F: nop
  004CDF90: push        4CDFA0h
  004CDF95: call        00561953
  004CDF9A: pop         ecx
  004CDF9B: ret
  004CDF9C: nop
  004CDF9D: nop
  004CDF9E: nop
  004CDF9F: nop
  004CDFA0: mov         ecx,0E92BF8h
  004CDFA5: jmp         0055E58A
  004CDFAA: nop
  004CDFAB: nop
  004CDFAC: nop
  004CDFAD: nop
  004CDFAE: nop
  004CDFAF: nop
  004CDFB0: E8

  Summary

      16E000 .text
