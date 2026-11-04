
Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  0041BA20: push        ebx
  0041BA21: push        ebp
  0041BA22: mov         ebp,dword ptr [esp+0Ch]
  0041BA26: push        esi
  0041BA27: push        edi
  0041BA28: cmp         ebp,5
  0041BA2B: mov         edi,ecx
  0041BA2D: jl          0041BA38
  0041BA2F: pop         edi
  0041BA30: pop         esi
  0041BA31: pop         ebp
  0041BA32: mov         al,1
  0041BA34: pop         ebx
  0041BA35: ret         4
  0041BA38: mov         eax,dword ptr [edi+00001900h]
  0041BA3E: xor         esi,esi
  0041BA40: test        eax,eax
  0041BA42: jle         0041BA72
  0041BA44: mov         ebx,edi
  0041BA46: xor         eax,eax
  0041BA48: mov         al,byte ptr [ebx]
  0041BA4A: cmp         eax,ebp
  0041BA4C: jne         0041BA64
  0041BA4E: mov         al,byte ptr [ebx+3Ch]
  0041BA51: test        al,al
  0041BA53: je          0041BA64
  0041BA55: push        0
  0041BA57: push        esi
  0041BA58: push        ebp
  0041BA59: mov         ecx,edi
  0041BA5B: call        0041B920
  0041BA60: test        eax,eax
  0041BA62: jl          0041BA2F
  0041BA64: mov         eax,dword ptr [edi+00001900h]
  0041BA6A: inc         esi
  0041BA6B: add         ebx,40h
  0041BA6E: cmp         esi,eax
  0041BA70: jl          0041BA46
  0041BA72: pop         edi
  0041BA73: pop         esi
  0041BA74: pop         ebp
  0041BA75: xor         al,al
  0041BA77: pop         ebx
  0041BA78: ret         4
  0041BA7B: nop
  0041BA7C: nop
  0041BA7D: nop
  0041BA7E: nop
  0041BA7F: nop
  0041BA80: mov         edx,dword ptr [ecx+00001900h]
  0041BA86: xor         eax,eax
  0041BA88: test        edx,edx
  0041BA8A: jle         0041BA9C
  0041BA8C: add         ecx,3Ch
  0041BA8F: cmp         byte ptr [ecx],0
  0041BA92: jne         0041BA9F
  0041BA94: inc         eax
  0041BA95: add         ecx,40h
  0041BA98: cmp         eax,edx
  0041BA9A: jl          0041BA8F
  0041BA9C: xor         al,al
  0041BA9E: ret
  0041BA9F: mov         al,1
  0041BAA1: ret
  0041BAA2: nop
  0041BAA3: nop
  0041BAA4: nop
  0041BAA5: nop
  0041BAA6: nop
  0041BAA7: nop
  0041BAA8: nop
  0041BAA9: nop
  0041BAAA: nop
  0041BAAB: nop
  0041BAAC: nop
  0041BAAD: nop
  0041BAAE: nop
  0041BAAF: nop
  0041BAB0: push        esi
  0041BAB1: push        edi
  0041BAB2: mov         edi,ecx
  0041BAB4: xor         esi,esi
  0041BAB6: push        esi
  0041BAB7: mov         ecx,edi
  0041BAB9: call        0041BA20
  0041BABE: test        al,al
  0041BAC0: jne         0041BACB
  0041BAC2: inc         esi
  0041BAC3: cmp         esi,9
  0041BAC6: jl          0041BAB6
  0041BAC8: pop         edi
  0041BAC9: pop         esi
  0041BACA: ret
  0041BACB: pop         edi
  0041BACC: mov         al,1
  0041BACE: pop         esi
  0041BACF: ret
  0041BAD0: 8B

  Summary

      16E000 .text
