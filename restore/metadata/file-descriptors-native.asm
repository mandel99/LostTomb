
Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  00566840: push        ebx
  00566841: push        esi
  00566842: push        edi
  00566843: or          ebx,0FFFFFFFFh
  00566846: xor         edi,edi
  00566848: xor         esi,esi
  0056684A: mov         ecx,1266960h
  0056684F: mov         eax,dword ptr [ecx]
  00566851: test        eax,eax
  00566853: je          0056688C
  00566855: lea         edx,[eax+00000100h]
  0056685B: cmp         eax,edx
  0056685D: jae         0056687B
  0056685F: test        byte ptr [eax+4],1
  00566863: je          0056686A
  00566865: add         eax,8
  00566868: jmp         0056685B
  0056686A: or          dword ptr [eax],0FFFFFFFFh
  0056686D: sub         eax,dword ptr [ecx]
  0056686F: sar         eax,3
  00566872: add         eax,esi
  00566874: mov         ebx,eax
  00566876: cmp         ebx,0FFFFFFFFh
  00566879: jne         005668CF
  0056687B: add         ecx,4
  0056687E: inc         edi
  0056687F: add         esi,20h
  00566882: cmp         ecx,1266A60h
  00566888: jl          0056684F
  0056688A: jmp         005668CF
  0056688C: mov         esi,100h
  00566891: push        esi
  00566892: call        00561B68
  00566897: test        eax,eax
  00566899: pop         ecx
  0056689A: je          005668CF
  0056689C: add         dword ptr ds:[01266A60h],20h
  005668A3: lea         ecx,[edi*4+01266960h]
  005668AA: lea         edx,[eax+00000100h]
  005668B0: mov         dword ptr [ecx],eax
  005668B2: cmp         eax,edx
  005668B4: jae         005668CA
  005668B6: and         byte ptr [eax+4],0
  005668BA: or          dword ptr [eax],0FFFFFFFFh
  005668BD: mov         byte ptr [eax+5],0Ah
  005668C1: mov         edx,dword ptr [ecx]
  005668C3: add         eax,8
  005668C6: add         edx,esi
  005668C8: jmp         005668B2
  005668CA: shl         edi,5
  005668CD: mov         ebx,edi
  005668CF: pop         edi
  005668D0: mov         eax,ebx
  005668D2: pop         esi
  005668D3: pop         ebx
  005668D4: ret
  005668D5: mov         eax,dword ptr [esp+4]
  005668D9: push        esi
  005668DA: cmp         eax,dword ptr ds:[01266A60h]
  005668E0: push        edi
  005668E1: jae         00566935
  005668E3: mov         ecx,eax
  005668E5: mov         esi,eax
  005668E7: sar         ecx,5
  005668EA: and         esi,1Fh
  005668ED: lea         edi,[ecx*4+01266960h]
  005668F4: shl         esi,3
  005668F7: mov         ecx,dword ptr [edi]
  005668F9: cmp         dword ptr [ecx+esi],0FFFFFFFFh
  005668FD: jne         00566935
  005668FF: cmp         dword ptr ds:[0060496Ch],1
  00566906: push        ebx
  00566907: mov         ebx,dword ptr [esp+14h]
  0056690B: jne         0056692B
  0056690D: sub         eax,0
  00566910: je          00566922
  00566912: dec         eax
  00566913: je          0056691D
  00566915: dec         eax
  00566916: jne         0056692B
  00566918: push        ebx
  00566919: push        0FFFFFFF4h
  0056691B: jmp         00566925
  0056691D: push        ebx
  0056691E: push        0FFFFFFF5h
  00566920: jmp         00566925
  00566922: push        ebx
  00566923: push        0FFFFFFF6h
  00566925: call        dword ptr ds:[0056F0D4h]
  0056692B: mov         eax,dword ptr [edi]
  0056692D: mov         dword ptr [eax+esi],ebx
  00566930: xor         eax,eax
  00566932: pop         ebx
  00566933: jmp         00566949
  00566935: and         dword ptr ds:[01266434h],0
  0056693C: mov         dword ptr ds:[01266430h],9
  00566946: or          eax,0FFFFFFFFh
  00566949: pop         edi
  0056694A: pop         esi
  0056694B: ret
  0056694C: mov         ecx,dword ptr [esp+4]
  00566950: push        esi
  00566951: cmp         ecx,dword ptr ds:[01266A60h]
  00566957: push        edi
  00566958: jae         005669AF
  0056695A: mov         eax,ecx
  0056695C: mov         esi,ecx
  0056695E: sar         eax,5
  00566961: and         esi,1Fh
  00566964: lea         edi,[eax*4+01266960h]
  0056696B: shl         esi,3
  0056696E: mov         eax,dword ptr [edi]
  00566970: add         eax,esi
  00566972: test        byte ptr [eax+4],1
  00566976: je          005669AF
  00566978: cmp         dword ptr [eax],0FFFFFFFFh
  0056697B: je          005669AF
  0056697D: cmp         dword ptr ds:[0060496Ch],1
  00566984: jne         005669A5
  00566986: xor         eax,eax
  00566988: sub         ecx,eax
  0056698A: je          0056699C
  0056698C: dec         ecx
  0056698D: je          00566997
  0056698F: dec         ecx
  00566990: jne         005669A5
  00566992: push        eax
  00566993: push        0FFFFFFF4h
  00566995: jmp         0056699F
  00566997: push        eax
  00566998: push        0FFFFFFF5h
  0056699A: jmp         0056699F
  0056699C: push        eax
  0056699D: push        0FFFFFFF6h
  0056699F: call        dword ptr ds:[0056F0D4h]
  005669A5: mov         eax,dword ptr [edi]
  005669A7: or          dword ptr [eax+esi],0FFFFFFFFh
  005669AB: xor         eax,eax
  005669AD: jmp         005669C3
  005669AF: and         dword ptr ds:[01266434h],0
  005669B6: mov         dword ptr ds:[01266430h],9
  005669C0: or          eax,0FFFFFFFFh
  005669C3: pop         edi
  005669C4: pop         esi
  005669C5: ret
  005669C6: mov         eax,dword ptr [esp+4]
  005669CA: cmp         eax,dword ptr ds:[01266A60h]
  005669D0: jae         005669EE
  005669D2: mov         ecx,eax
  005669D4: and         eax,1Fh
  005669D7: sar         ecx,5
  005669DA: mov         ecx,dword ptr [ecx*4+01266960h]
  005669E1: test        byte ptr [ecx+eax*8+4],1
  005669E6: lea         eax,[ecx+eax*8]
  005669E9: je          005669EE
  005669EB: mov         eax,dword ptr [eax]
  005669ED: ret

  Summary

      16E000 .text
