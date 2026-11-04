
Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  004CC110: mov         eax,dword ptr [esp+4]
  004CC114: mov         edx,dword ptr ds:[00E36C70h]
  004CC11A: mov         eax,dword ptr [eax*8+00E34C38h]
  004CC121: add         eax,edx
  004CC123: mov         edx,dword ptr [esp+8]
  004CC127: test        edx,edx
  004CC129: mov         dword ptr ds:[00E20768h],eax
  004CC12E: jle         004CC14C
  004CC130: cmp         byte ptr [eax],0
  004CC133: jne         004CC142
  004CC135: mov         cl,byte ptr [eax-1]
  004CC138: cmp         cl,20h
  004CC13B: jae         004CC141
  004CC13D: test        cl,cl
  004CC13F: jne         004CC142
  004CC141: dec         edx
  004CC142: inc         eax
  004CC143: test        edx,edx
  004CC145: mov         dword ptr ds:[00E20768h],eax
  004CC14A: jg          004CC130
  004CC14C: cmp         byte ptr [eax],20h
  004CC14F: jae         004CC15C
  004CC151: inc         eax
  004CC152: mov         dword ptr ds:[00E20768h],eax
  004CC157: cmp         byte ptr [eax],20h
  004CC15A: jb          004CC151
  004CC15C: ret
  004CC15D: nop
  004CC15E: nop
  004CC15F: nop
  004CC160: mov         eax,dword ptr [esp+4]
  004CC164: mov         edx,dword ptr ds:[00E36C70h]
  004CC16A: mov         eax,dword ptr [eax*8+00E34C38h]
  004CC171: add         eax,edx
  004CC173: mov         edx,dword ptr [esp+8]
  004CC177: test        edx,edx
  004CC179: mov         dword ptr ds:[00E20764h],eax
  004CC17E: jle         004CC19C
  004CC180: cmp         byte ptr [eax],0
  004CC183: jne         004CC192
  004CC185: mov         cl,byte ptr [eax-1]
  004CC188: cmp         cl,20h
  004CC18B: jae         004CC191
  004CC18D: test        cl,cl
  004CC18F: jne         004CC192
  004CC191: dec         edx
  004CC192: inc         eax
  004CC193: test        edx,edx
  004CC195: mov         dword ptr ds:[00E20764h],eax
  004CC19A: jg          004CC180
  004CC19C: cmp         byte ptr [eax],20h
  004CC19F: jae         004CC1AC
  004CC1A1: inc         eax
  004CC1A2: mov         dword ptr ds:[00E20764h],eax
  004CC1A7: cmp         byte ptr [eax],20h
  004CC1AA: jb          004CC1A1
  004CC1AC: ret
  004CC1AD: nop

  Summary

      16E000 .text
