
Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  005404B0: mov         eax,dword ptr [esp+4]
  005404B4: mov         edx,dword ptr [ecx+4]
  005404B7: mov         dword ptr [eax+4],edx
  005404BA: mov         edx,dword ptr [ecx+4]
  005404BD: test        edx,edx
  005404BF: je          005404C3
  005404C1: mov         dword ptr [edx],eax
  005404C3: mov         dword ptr [eax],0
  005404C9: mov         edx,dword ptr [ecx]
  005404CB: test        edx,edx
  005404CD: mov         dword ptr [ecx+4],eax
  005404D0: jne         005404D4
  005404D2: mov         dword ptr [ecx],eax
  005404D4: ret         4
  005404D7: nop
  005404D8: nop
  005404D9: nop
  005404DA: nop
  005404DB: nop
  005404DC: nop
  005404DD: nop
  005404DE: nop
  005404DF: nop
  005404E0: mov         eax,ecx
  005404E2: xor         ecx,ecx
  005404E4: mov         dword ptr [eax+0000CC24h],ecx
  005404EA: mov         byte ptr [eax],cl
  005404EC: mov         dword ptr [eax+0000CC28h],ecx
  005404F2: mov         dword ptr [eax+0000CC2Ch],ecx
  005404F8: mov         dword ptr [eax+0000CC30h],ecx
  005404FE: ret
  005404FF: nop
  00540500: push        ebx
  00540501: push        esi
  00540502: mov         esi,ecx
  00540504: xor         ebx,ebx
  00540506: mov         eax,dword ptr [esi+0000CC24h]
  0054050C: cmp         eax,ebx
  0054050E: je          0054051F
  00540510: push        eax
  00540511: call        00561A2B
  00540516: add         esp,4
  00540519: mov         dword ptr [esi+0000CC24h],ebx
  0054051F: mov         byte ptr [esi],bl
  00540521: mov         dword ptr [esi+0000CC28h],ebx
  00540527: mov         dword ptr [esi+0000CC2Ch],ebx
  0054052D: mov         dword ptr [esi+0000CC30h],ebx
  00540533: pop         esi
  00540534: pop         ebx
  00540535: ret
  00540536: nop
  00540537: nop
  00540538: nop
  00540539: nop
  0054053A: nop
  0054053B: nop
  0054053C: nop
  0054053D: nop
  0054053E: nop
  0054053F: nop
  00540540: mov         eax,dword ptr [esp+4]
  00540544: mov         edx,dword ptr [ecx]
  00540546: mov         dword ptr [eax],edx
  00540548: mov         edx,dword ptr [ecx]
  0054054A: test        edx,edx
  0054054C: je          00540551
  0054054E: mov         dword ptr [edx+4],eax
  00540551: mov         dword ptr [eax+4],0
  00540558: mov         edx,dword ptr [ecx+4]
  0054055B: test        edx,edx
  0054055D: mov         dword ptr [ecx],eax
  0054055F: jne         00540564
  00540561: mov         dword ptr [ecx+4],eax
  00540564: ret         4
  00540567: nop
  00540568: nop
  00540569: nop
  0054056A: nop
  0054056B: nop
  0054056C: nop
  0054056D: nop
  0054056E: nop
  0054056F: nop
  00540570: mov         eax,dword ptr [esp+8]
  00540574: mov         ecx,dword ptr [esp+0Ch]
  00540578: cmp         dword ptr [eax+8],ecx
  0054057B: jge         00540584
  0054057D: mov         eax,dword ptr [eax]
  0054057F: cmp         dword ptr [eax+8],ecx
  00540582: jl          0054057D
  00540584: ret         0Ch
  00540587: nop
  00540588: nop
  00540589: nop
  0054058A: nop
  0054058B: nop
  0054058C: nop
  0054058D: nop
  0054058E: nop
  0054058F: nop
  00540590: mov         ecx,dword ptr [esp+4]
  00540594: mov         eax,dword ptr [esp+8]
  00540598: mov         dword ptr [ecx],eax
  0054059A: mov         edx,dword ptr [eax+4]
  0054059D: mov         dword ptr [ecx+4],edx
  005405A0: mov         edx,dword ptr [eax+4]
  005405A3: mov         dword ptr [edx],ecx
  005405A5: mov         dword ptr [eax+4],ecx
  005405A8: ret         8
  005405AB: nop
  005405AC: nop
  005405AD: nop
  005405AE: nop
  005405AF: nop
  005405B0: sub         esp,70h
  005405B3: push        ebx
  005405B4: push        ebp
  005405B5: push        esi
  005405B6: push        edi
  005405B7: mov         edi,57626Ch
  005405BC: or          ecx,0FFFFFFFFh
  005405BF: xor         eax,eax
  005405C1: lea         edx,[esp+1Ch]
  005405C5: repne scas  byte ptr es:[edi]
  005405C7: not         ecx
  005405C9: sub         edi,ecx
  005405CB: mov         ebp,dword ptr ds:[00E38EB4h]

  Summary

      16E000 .text
