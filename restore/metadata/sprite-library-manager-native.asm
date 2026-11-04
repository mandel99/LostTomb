
Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

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
  0053DE01: call        00562370
  0053DE06: add         esp,8
  0053DE09: test        eax,eax
  0053DE0B: sete        al
  0053DE0E: test        al,al
  0053DE10: mov         byte ptr [esp+74h],al
  0053DE14: jne         0053DE24
  0053DE16: mov         ebx,dword ptr [edi+0000CC34h]
  0053DE1C: mov         ecx,dword ptr [edi+0000CC38h]
  0053DE22: jmp         0053DE28
  0053DE24: mov         ecx,dword ptr [esp+14h]
  0053DE28: mov         edx,dword ptr [edi+0000CC28h]
  0053DE2E: sub         dword ptr [esi+0030F5D0h],edx
  0053DE34: test        al,al
  0053DE36: jne         0053DE9F
  0053DE38: mov         edx,dword ptr [esp+70h]
  0053DE3C: push        ecx
  0053DE3D: lea         ecx,[esi+ecx*8]
  0053DE40: push        ecx
  0053DE41: push        ebx
  0053DE42: shl         ebx,6
  0053DE45: mov         ecx,edi
  0053DE47: lea         eax,[ebx+esi+00180E48h]
  0053DE4E: push        eax
  0053DE4F: push        ebp
  0053DE50: push        edx
  0053DE51: call        0053DFF0
  0053DE56: test        al,al
  0053DE58: mov         byte ptr [esp+74h],al
  0053DE5C: je          0053DE9B
  0053DE5E: mov         al,byte ptr [esp+13h]
  0053DE62: test        al,al
  0053DE64: je          0053DE9B
  0053DE66: mov         eax,dword ptr [edi+0000CC2Ch]
  0053DE6C: mov         edx,dword ptr [esi+0030F5CCh]
  0053DE72: mov         ecx,dword ptr [esi+0030F5C8h]
  0053DE78: add         edx,eax
  0053DE7A: mov         dword ptr [esi+0030F5CCh],edx
  0053DE80: mov         eax,dword ptr [edi+0000CC30h]
  0053DE86: add         ecx,eax
  0053DE88: mov         eax,dword ptr [esi+0030F5D4h]
  0053DE8E: inc         eax
  0053DE8F: mov         dword ptr [esi+0030F5C8h],ecx
  0053DE95: mov         dword ptr [esi+0030F5D4h],eax
  0053DE9B: mov         al,byte ptr [esp+74h]
  0053DE9F: mov         ecx,dword ptr [edi+0000CC28h]
  0053DEA5: mov         ebx,dword ptr [esi+0030F5D0h]
  0053DEAB: add         ebx,ecx
  0053DEAD: mov         dword ptr [esi+0030F5D0h],ebx
  0053DEB3: mov         ebx,dword ptr [esi+ebp*4+0030F548h]
  0053DEBA: dec         ebx
  0053DEBB: test        ebp,ebp
  0053DEBD: jle         0053DEE5
  0053DEBF: mov         esi,dword ptr [esi+ebp*4+0030F544h]
  0053DEC6: dec         esi
  0053DEC7: jmp         0053DEE7
  0053DEC9: mov         eax,dword ptr [esi+0030F5C8h]
  0053DECF: mov         ebx,dword ptr [esi+0030F5CCh]
  0053DED5: mov         byte ptr [esp+13h],1
  0053DEDA: mov         dword ptr [esp+14h],eax
  0053DEDE: mov         ecx,eax
  0053DEE0: jmp         0053DE3C
  0053DEE5: xor         esi,esi
  0053DEE7: mov         ecx,dword ptr [edi+0000CC30h]
  0053DEED: sub         ebx,esi
  0053DEEF: cmp         ebx,ecx
  0053DEF1: pop         ebp
  0053DEF2: je          0053DFAE
  0053DEF8: push        1
  0053DEFA: lea         ecx,[esp+18h]
  0053DEFE: call        005609DA
  0053DF03: mov         edi,dword ptr [edi+0000CC30h]
  0053DF09: mov         ecx,dword ptr [esp+6Ch]
  0053DF0D: push        6018B4h
  0053DF12: push        ebx
  0053DF13: push        60188Ch
  0053DF18: push        edi
  0053DF19: push        601880h
  0053DF1E: push        ecx
  0053DF1F: push        601874h
  0053DF24: lea         ecx,[esp+3Ch]
  0053DF28: mov         dword ptr [esp+00000080h],0
  0053DF33: call        00560CA6
  0053DF38: mov         ecx,eax
  0053DF3A: call        00560CA6
  0053DF3F: mov         ecx,eax
  0053DF41: call        00560CA6
  0053DF46: mov         ecx,eax
  0053DF48: call        00560B4C
  0053DF4D: mov         ecx,eax
  0053DF4F: call        00560CA6
  0053DF54: mov         ecx,eax
  0053DF56: call        00560B4C
  0053DF5B: mov         ecx,eax
  0053DF5D: call        00560CA6
  0053DF62: push        0
  0053DF64: mov         ecx,eax
  0053DF66: call        00560AAE
  0053DF6B: mov         edx,dword ptr [esp+14h]
  0053DF6F: push        0
  0053DF71: push        601858h
  0053DF76: mov         eax,dword ptr [edx+4]
  0053DF79: mov         ecx,dword ptr [esp+eax+20h]
  0053DF7D: call        00560734
  0053DF82: mov         ecx,dword ptr ds:[00E39140h]
  0053DF88: push        eax
  0053DF89: push        ecx
  0053DF8A: call        dword ptr ds:[0056F1DCh]
  0053DF90: lea         ecx,[esp+28h]
  0053DF94: mov         dword ptr [esp+64h],0FFFFFFFFh
  0053DF9C: call        00560A9B
  0053DFA1: lea         ecx,[esp+28h]
  0053DFA5: call        00560648
  0053DFAA: mov         al,byte ptr [esp+70h]
  0053DFAE: mov         ecx,dword ptr [esp+5Ch]
  0053DFB2: pop         edi
  0053DFB3: pop         esi
  0053DFB4: pop         ebx
  0053DFB5: mov         dword ptr fs:[00000000h],ecx
  0053DFBC: add         esp,5Ch

  Summary

      16E000 .text
