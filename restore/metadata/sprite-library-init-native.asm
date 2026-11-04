
Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  0053DC20: add         bh,al
  0053DC22: inc         esp
  0053DC23: and         al,60h
  0053DC25: std
  0053DC26: add         al,byte ptr [eax]
  0053DC28: add         bh,al
  0053DC2A: inc         esp
  0053DC2B: and         al,64h
  0053DC2D: inc         byte ptr [edx]
  0053DC2F: add         byte ptr [eax],al
  0053DC31: mov         dword ptr [esp+68h],2FFh
  0053DC39: mov         dword ptr [esp+6Ch],300h
  0053DC41: mov         dword ptr [esp+70h],301h
  0053DC49: mov         dword ptr [esp+74h],312h
  0053DC51: mov         dword ptr [esp+78h],317h
  0053DC59: mov         dword ptr [esp+7Ch],32Ch
  0053DC61: mov         dword ptr [esp+00000080h],33Ch
  0053DC6C: mov         dword ptr [esp+00000084h],353h
  0053DC77: mov         dword ptr [esp+00000088h],370h
  0053DC82: mov         dword ptr [esp+0000008Ch],393h
  0053DC8D: lea         ebp,[esp+18h]
  0053DC91: mov         dword ptr [esp+10h],edx
  0053DC95: lea         esi,[edi+0000EB64h]
  0053DC9B: mov         dword ptr [esp+14h],1Eh
  0053DCA3: mov         eax,dword ptr [esi]
  0053DCA5: cmp         eax,ebx
  0053DCA7: je          0053DCB4
  0053DCA9: push        eax
  0053DCAA: call        00561A2B
  0053DCAF: add         esp,4
  0053DCB2: mov         dword ptr [esi],ebx
  0053DCB4: mov         eax,dword ptr [esp+10h]
  0053DCB8: mov         ecx,dword ptr [ebp]
  0053DCBB: mov         byte ptr [esi+FFFF33DCh],bl
  0053DCC1: mov         dword ptr [esi+4],ebx
  0053DCC4: mov         dword ptr [esi+8],ebx
  0053DCC7: mov         dword ptr [esi+0Ch],ebx
  0053DCCA: mov         dword ptr [eax],ecx
  0053DCCC: add         eax,4
  0053DCCF: mov         dword ptr [esp+10h],eax
  0053DCD3: mov         eax,dword ptr [esp+14h]
  0053DCD7: add         esi,0CC3Ch
  0053DCDD: add         ebp,4
  0053DCE0: dec         eax
  0053DCE1: mov         dword ptr [esp+14h],eax
  0053DCE5: jne         0053DCA3
  0053DCE7: mov         eax,1
  0053DCEC: mov         dword ptr [edi+0030F5D4h],ebx
  0053DCF2: mov         dword ptr [edi+0030F5C8h],eax
  0053DCF8: mov         dword ptr [edi+0030F5CCh],eax
  0053DCFE: mov         dword ptr [edi+0030F5D0h],ebx
  0053DD04: pop         edi
  0053DD05: pop         esi
  0053DD06: pop         ebp
  0053DD07: pop         ebx
  0053DD08: add         esp,80h
  0053DD0E: ret
  0053DD0F: nop
  0053DD10: mov         edx,dword ptr [ecx+0030F5D4h]
  0053DD16: xor         al,al
  0053DD18: cmp         edx,1Eh
  0053DD1B: jge         0053DD38
  0053DD1D: push        esi
  0053DD1E: mov         esi,dword ptr [esp+8]
  0053DD22: mov         edx,dword ptr [esi+edx*4]
  0053DD25: pop         esi
  0053DD26: test        edx,edx
  0053DD28: je          0053DD38
  0053DD2A: push        ebx
  0053DD2B: mov         bl,byte ptr [edx]
  0053DD2D: test        bl,bl
  0053DD2F: pop         ebx
  0053DD30: je          0053DD38
  0053DD32: push        edx
  0053DD33: call        0053DD80
  0053DD38: ret         4
  0053DD3B: nop
  0053DD3C: nop
  0053DD3D: nop
  0053DD3E: nop
  0053DD3F: nop
  0053DD40: push        esi
  0053DD41: push        edi
  0053DD42: mov         edi,dword ptr [esp+0Ch]
  0053DD46: mov         esi,ecx
  0053DD48: mov         al,1
  0053DD4A: mov         ecx,dword ptr [esi+0030F5D4h]
  0053DD50: cmp         ecx,1Eh
  0053DD53: jge         0053DD6D
  0053DD55: mov         ecx,dword ptr [edi+ecx*4]
  0053DD58: test        ecx,ecx
  0053DD5A: je          0053DD69
  0053DD5C: cmp         byte ptr [ecx],0
  0053DD5F: je          0053DD69
  0053DD61: push        ecx
  0053DD62: mov         ecx,esi
  0053DD64: call        0053DD80
  0053DD69: test        al,al
  0053DD6B: jne         0053DD4A
  0053DD6D: pop         edi
  0053DD6E: pop         esi
  0053DD6F: ret         4
  0053DD72: nop
  0053DD73: nop
  0053DD74: nop
  0053DD75: nop
  0053DD76: nop
  0053DD77: nop
  0053DD78: nop
  0053DD79: nop
  0053DD7A: nop
  0053DD7B: nop
  0053DD7C: nop
  0053DD7D: nop
  0053DD7E: nop
  0053DD7F: nop
  0053DD80: mov         eax,dword ptr [esp+4]
  0053DD84: push        0FFFFFFFFh
  0053DD86: push        eax
  0053DD87: call        0053DD90
  0053DD8C: ret         4
  0053DD8F: nop
  0053DD90: mov         eax,dword ptr fs:[00000000h]
  0053DD96: push        0FFFFFFFFh
  0053DD98: push        56B7D8h
  0053DD9D: mov         edx,dword ptr [esp+0Ch]
  0053DDA1: push        eax
  0053DDA2: xor         al,al
  0053DDA4: mov         dword ptr fs:[00000000h],esp
  0053DDAB: sub         esp,50h
  0053DDAE: push        ebx
  0053DDAF: xor         ebx,ebx
  0053DDB1: push        esi
  0053DDB2: cmp         edx,ebx
  0053DDB4: push        edi
  0053DDB5: mov         esi,ecx
  0053DDB7: je          0053DFAE
  0053DDBD: push        ebp
  0053DDBE: mov         ebp,dword ptr [esp+74h]
  0053DDC2: cmp         ebp,0FFFFFFFFh
  0053DDC5: mov         dword ptr [esp+14h],ebx
  0053DDC9: jne         0053DDD1
  0053DDCB: mov         ebp,dword ptr [esi+0030F5D4h]
  0053DDD1: mov         eax,ebp
  0053DDD3: mov         byte ptr [esp+13h],0
  0053DDD8: shl         eax,4
  0053DDDB: add         eax,ebp
  0053DDDD: shl         eax,6
  0053DDE0: add         eax,ebp
  0053DDE2: lea         eax,[ebp+eax*4]
  0053DDE6: lea         ecx,[eax+eax*2]
  0053DDE9: mov         eax,dword ptr [esi+ecx*4+0000EB6Ch]
  0053DDF0: cmp         eax,ebx
  0053DDF2: lea         edi,[esi+ecx*4+00001F40h]
  0053DDF9: je          0053DEC9
  0053DDFF: push        edi
  0053DE00: push        edx

  Summary

      16E000 .text
