
Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  00563735: mov         eax,dword ptr ds:[01267AA0h]
  0056373A: push        esi
  0056373B: push        14h
  0056373D: test        eax,eax
  0056373F: pop         esi
  00563740: jne         00563749
  00563742: mov         eax,200h
  00563747: jmp         0056374F
  00563749: cmp         eax,esi
  0056374B: jge         00563754
  0056374D: mov         eax,esi
  0056374F: mov         dword ptr ds:[01267AA0h],eax
  00563754: push        4
  00563756: push        eax
  00563757: call        00563129
  0056375C: pop         ecx
  0056375D: mov         dword ptr ds:[01266A90h],eax
  00563762: test        eax,eax
  00563764: pop         ecx
  00563765: jne         00563788
  00563767: push        4
  00563769: push        esi
  0056376A: mov         dword ptr ds:[01267AA0h],esi
  00563770: call        00563129
  00563775: pop         ecx
  00563776: mov         dword ptr ds:[01266A90h],eax
  0056377B: test        eax,eax
  0056377D: pop         ecx
  0056377E: jne         00563788
  00563780: push        1Ah
  00563782: call        005630E0
  00563787: pop         ecx
  00563788: xor         ecx,ecx
  0056378A: mov         eax,6049A0h
  0056378F: mov         edx,dword ptr ds:[01266A90h]
  00563795: mov         dword ptr [ecx+edx],eax
  00563798: add         eax,20h
  0056379B: add         ecx,4
  0056379E: cmp         eax,604C20h
  005637A3: jl          0056378F
  005637A5: xor         edx,edx
  005637A7: mov         ecx,6049B0h
  005637AC: mov         eax,edx
  005637AE: mov         esi,edx
  005637B0: sar         eax,5
  005637B3: and         esi,1Fh
  005637B6: mov         eax,dword ptr [eax*4+01266960h]
  005637BD: mov         eax,dword ptr [eax+esi*8]
  005637C0: cmp         eax,0FFFFFFFFh
  005637C3: je          005637C9
  005637C5: test        eax,eax
  005637C7: jne         005637CC
  005637C9: or          dword ptr [ecx],0FFFFFFFFh
  005637CC: add         ecx,20h
  005637CF: inc         edx
  005637D0: cmp         ecx,604A10h
  005637D6: jl          005637AC
  005637D8: pop         esi
  005637D9: ret

  Summary

      16E000 .text
