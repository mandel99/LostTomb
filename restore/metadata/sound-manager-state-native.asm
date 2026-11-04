Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  00539550: push        ebx
  00539551: push        ebp
  00539552: push        esi
  00539553: mov         esi,ecx
  00539555: xor         ebx,ebx
  00539557: push        edi
  00539558: mov         dword ptr [esi+00059A50h],ebx
  0053955E: mov         dword ptr [esi+00059A54h],ebx
  00539564: mov         dword ptr [esi+00059A58h],ebx
  0053956A: mov         dword ptr [esi+00059A5Ch],ebx
  00539570: mov         dword ptr [esi+00059A60h],ebx
  00539576: mov         dword ptr [esi+00059A64h],ebx
  0053957C: mov         dword ptr [esi+00059A68h],ebx
  00539582: mov         dword ptr [esi+00059A6Ch],ebx
  00539588: mov         dword ptr [esi+00059A70h],ebx
  0053958E: lea         edx,[esi+000015E0h]
  00539594: mov         ebp,35h
  00539599: mov         ecx,578h
  0053959E: xor         eax,eax
  005395A0: lea         edi,[edx+FFFFEA20h]
  005395A6: mov         dword ptr [edx+00000528h],ebx
  005395AC: rep stos    dword ptr es:[edi]
  005395AE: mov         ecx,118h
  005395B3: mov         edi,edx
  005395B5: rep stos    dword ptr es:[edi]
  005395B7: lea         edi,[edx+00000460h]
  005395BD: mov         ecx,32h
  005395C2: add         edx,1B10h
  005395C8: dec         ebp
  005395C9: rep stos    dword ptr es:[edi]
  005395CB: jne         00539599
  005395CD: mov         eax,esi
  005395CF: pop         edi
  005395D0: pop         esi
  005395D1: pop         ebp
  005395D2: pop         ebx
  005395D3: ret
  005395D4: nop
  005395D5: nop
  005395D6: nop
  005395D7: nop
  005395D8: nop
  005395D9: nop
  005395DA: nop
  005395DB: nop
  005395DC: nop
  005395DD: nop
  005395DE: nop
  005395DF: nop
  005395E0: mov         eax,dword ptr [esp+8]
  005395E4: push        ebx
  005395E5: push        ebp
  005395E6: push        esi
  005395E7: cmp         eax,35h
  005395EA: push        edi
  005395EB: mov         ebx,ecx
  005395ED: jle         005395F9
  005395EF: pop         edi
  005395F0: pop         esi
  005395F1: pop         ebp
  005395F2: or          eax,0FFFFFFFFh
  005395F5: pop         ebx
  005395F6: ret         18h
  005395F9: mov         ecx,dword ptr [esp+28h]
  005395FD: test        ecx,ecx
  005395FF: lea         ecx,[eax+eax*2]
  00539602: lea         edx,[ecx+ecx*8]
  00539605: jne         0053968B
  0053960B: shl         edx,4
  0053960E: add         edx,eax
  00539610: shl         edx,4
  00539613: mov         eax,dword ptr [edx+ebx+00001B08h]
  0053961A: cmp         eax,32h
  0053961D: jne         00539629
  0053961F: pop         edi
  00539620: pop         esi
  00539621: pop         ebp
  00539622: or          eax,0FFFFFFFFh
  00539625: pop         ebx
  00539626: ret         18h
  00539629: lea         ecx,[eax*8+00000000h]
  00539630: mov         edi,dword ptr [esp+14h]
  00539634: sub         ecx,eax
  00539636: xor         eax,eax
  00539638: shl         ecx,4
  0053963B: add         ecx,edx
  0053963D: lea         ebp,[ecx+ebx]
  00539640: or          ecx,0FFFFFFFFh
  00539643: repne scas  byte ptr es:[edi]
  00539645: not         ecx
  00539647: sub         edi,ecx
  00539649: mov         eax,ecx
  0053964B: mov         esi,edi
  0053964D: mov         edi,ebp
  0053964F: shr         ecx,2
  00539652: rep movs    dword ptr es:[edi],dword ptr [esi]
  00539654: mov         ecx,eax
  00539656: mov         eax,dword ptr [esp+20h]
  0053965A: and         ecx,3
  0053965D: rep movs    byte ptr es:[edi],byte ptr [esi]
  0053965F: mov         ecx,dword ptr [esp+1Ch]
  00539663: pop         edi
  00539664: mov         dword ptr [ebp+64h],ecx
  00539667: mov         ecx,dword ptr [esp+20h]
  0053966B: mov         dword ptr [ebp+68h],eax
  0053966E: mov         dword ptr [ebp+6Ch],ecx
  00539671: mov         eax,dword ptr [edx+ebx+00001B08h]
  00539678: pop         esi
  00539679: inc         eax
  0053967A: pop         ebp
  0053967B: mov         dword ptr [edx+ebx+00001B08h],eax
  00539682: mov         eax,1
  00539687: pop         ebx
  00539688: ret         18h
  0053968B: shl         edx,4
  0053968E: add         edx,eax
  00539690: shl         edx,4
  00539693: mov         ebp,dword ptr [edx+ebx+00001B0Ch]
  0053969A: cmp         ebp,0Ah
  0053969D: jne         005396A9
  0053969F: pop         edi
  005396A0: pop         esi
  005396A1: pop         ebp
  005396A2: or          eax,0FFFFFFFFh
  005396A5: pop         ebx
  005396A6: ret         18h
  005396A9: lea         eax,[ebp+32h]
  005396AC: mov         edi,dword ptr [esp+14h]
  005396B0: or          ecx,0FFFFFFFFh
  005396B3: lea         esi,[eax*8+00000000h]
  005396BA: sub         esi,eax
  005396BC: xor         eax,eax
  005396BE: shl         esi,4
  005396C1: add         esi,edx
  005396C3: add         esi,ebx
  005396C5: repne scas  byte ptr es:[edi]
  005396C7: not         ecx
  005396C9: sub         edi,ecx
  005396CB: mov         dword ptr [esp+14h],esi
  005396CF: mov         eax,ecx
  005396D1: mov         esi,edi
  005396D3: mov         edi,dword ptr [esp+14h]
  005396D7: shr         ecx,2
  005396DA: rep movs    dword ptr es:[edi],dword ptr [esi]
  005396DC: mov         ecx,eax
  005396DE: and         ecx,3
  005396E1: rep movs    byte ptr es:[edi],byte ptr [esi]
  005396E3: lea         ecx,[ebp*8+00000000h]
  005396EA: pop         edi
  005396EB: sub         ecx,ebp
  005396ED: pop         esi
  005396EE: shl         ecx,4
  005396F1: add         ecx,edx
  005396F3: pop         ebp
  005396F4: lea         eax,[ecx+ebx]
  005396F7: mov         ecx,dword ptr [esp+10h]
  005396FB: mov         dword ptr [eax+00001644h],ecx
  00539701: xor         ecx,ecx
  00539703: mov         dword ptr [eax+00001648h],ecx
  00539709: mov         dword ptr [eax+0000164Ch],ecx
  0053970F: mov         eax,dword ptr [edx+ebx+00001B0Ch]
  00539716: inc         eax
  00539717: mov         dword ptr [edx+ebx+00001B0Ch],eax
  0053971E: mov         eax,1
  00539723: pop         ebx
  00539724: ret         18h
  00539727: nop
  00539728: nop
  00539729: nop
  0053972A: nop
  0053972B: nop
  0053972C: nop
  0053972D: nop
  0053972E: nop
  0053972F: nop
  00539730: mov         eax,dword ptr ds:[00EAAC28h]
  00539735: push        ebx
  00539736: imul        eax,eax,49D8h
  0053973C: push        esi
  0053973D: mov         esi,ecx
  0053973F: movsx       ecx,byte ptr ds:[00E38EA8h]
  00539746: mov         ebx,dword ptr [eax+00EA1894h]
  0053974C: lea         eax,[ecx+ecx*2]
  0053974F: lea         eax,[eax+eax*8]
  00539752: shl         eax,4
  00539755: add         eax,ecx
  00539757: mov         ecx,dword ptr [esi+00059A60h]
  0053975D: shl         eax,4
  00539760: lea         edx,[ecx*8+00000000h]
  00539767: sub         edx,ecx
  00539769: shl         edx,4
  0053976C: add         edx,eax
  0053976E: lea         ecx,[edx+esi]
  00539771: mov         edx,dword ptr [edx+esi+6Ch]
  00539775: cmp         ebx,edx
  00539777: jg          00539786
  00539779: cmp         ebx,dword ptr [ecx+68h]
  0053977C: jl          00539786
  0053977E: mov         ecx,dword ptr [esp+0Ch]
  00539782: test        ecx,ecx
  00539784: je          005397DA
  00539786: mov         edx,dword ptr [eax+esi+00001B08h]
  0053978D: xor         ecx,ecx
  0053978F: push        ebp
  00539790: push        edi
  00539791: test        edx,edx
  00539793: jle         005397AA
  00539795: lea         edi,[eax+esi+6Ch]
  00539799: cmp         ebx,dword ptr [edi-4]
  0053979C: jl          005397A2
  0053979E: cmp         ebx,dword ptr [edi]
  005397A0: jle         005397AA
  005397A2: inc         ecx
  005397A3: add         edi,70h
  005397A6: cmp         ecx,edx
  005397A8: jl          00539799
  005397AA: cmp         ecx,edx
  005397AC: mov         edi,ecx
  005397AE: jge         005397CE
  005397B0: lea         ebp,[ecx*8+00000000h]
  005397B7: sub         ebp,ecx
  005397B9: shl         ebp,4
  005397BC: add         ebp,eax
  005397BE: lea         eax,[esi+ebp+68h]
  005397C2: cmp         dword ptr [eax],ebx
  005397C4: jg          005397CE
  005397C6: inc         ecx
  005397C7: add         eax,70h
  005397CA: cmp         ecx,edx
  005397CC: jl          005397C2
  005397CE: dec         ecx
  005397CF: push        ecx
  005397D0: push        edi
  005397D1: mov         ecx,esi
  005397D3: call        00539A50
  005397D8: pop         edi
  005397D9: pop         ebp
  005397DA: mov         ecx,dword ptr [esi+00059A60h]
  005397E0: mov         eax,dword ptr [esi+00059A54h]
  005397E6: cmp         ecx,eax
  005397E8: jl          005397F2
  005397EA: cmp         ecx,dword ptr [esi+00059A58h]
  005397F0: jle         005397F8
  005397F2: mov         dword ptr [esi+00059A60h],eax
  005397F8: cmp         eax,dword ptr [esi+00059A5Ch]
  005397FE: je          0053981A
  00539800: mov         dword ptr [esi+00059A5Ch],eax
  00539806: mov         eax,dword ptr [esi+00059A6Ch]
  0053980C: test        eax,eax
  0053980E: jne         0053981A
  00539810: mov         dword ptr [esi+00059A64h],1
  0053981A: pop         esi
  0053981B: pop         ebx
  0053981C: ret         4
  0053981F: nop
  00539820: mov         eax,dword ptr ds:[00F55B88h]
  00539825: push        esi
  00539826: test        eax,eax
  00539828: push        edi
  00539829: mov         esi,ecx
  0053982B: je          00539A22
  00539831: mov         al,byte ptr ds:[00E38E64h]
  00539836: test        al,al
  00539838: je          00539A22
  0053983E: mov         eax,dword ptr [esi+00059A64h]
  00539844: test        eax,eax
  00539846: jne         0053984F
  00539848: push        0
  0053984A: call        00539730
  0053984F: mov         eax,dword ptr ds:[00E38EC8h]
  00539854: mov         ecx,0E92BF8h
  00539859: push        eax
  0053985A: call        0055F38D
  0053985F: mov         eax,dword ptr [esi+00059A64h]
  00539865: cmp         eax,3
  00539868: ja          00539A22
  0053986E: jmp         dword ptr [eax*4+00539A28h]
  00539875: movsx       eax,byte ptr ds:[00E38EA8h]
  0053987C: lea         ecx,[eax+eax*2]
  0053987F: lea         ecx,[ecx+ecx*8]
  00539882: shl         ecx,4
  00539885: add         ecx,eax
  00539887: shl         ecx,4
  0053988A: mov         eax,dword ptr [ecx+esi+00001B08h]
  00539891: test        eax,eax
  00539893: jle         00539A22
  00539899: mov         eax,dword ptr [esi+00059A60h]
  0053989F: push        1
  005398A1: push        0
  005398A3: lea         edx,[eax*8+00000000h]
  005398AA: sub         edx,eax
  005398AC: shl         edx,4
  005398AF: add         edx,ecx
  005398B1: mov         ecx,0E92BF8h
  005398B6: add         edx,esi
  005398B8: push        edx
  005398B9: call        0055F449
  005398BE: movsx       eax,byte ptr ds:[00E38EA8h]
  005398C5: lea         ecx,[eax+eax*2]
  005398C8: lea         ecx,[ecx+ecx*8]
  005398CB: shl         ecx,4
  005398CE: add         ecx,eax
  005398D0: mov         eax,dword ptr [esi+00059A60h]
  005398D6: shl         ecx,4
  005398D9: lea         edx,[eax*8+00000000h]
  005398E0: sub         edx,eax
  005398E2: mov         eax,esi
  005398E4: shl         edx,4
  005398E7: add         eax,ecx
  005398E9: mov         eax,dword ptr [edx+eax+64h]
  005398ED: lea         eax,[eax+eax*4]
  005398F0: lea         eax,[eax+eax*4]
  005398F3: lea         ecx,[eax+eax*4]
  005398F6: shl         ecx,3
  005398F9: mov         dword ptr ds:[00F55B98h],ecx
  005398FF: mov         eax,dword ptr [esi+00059A60h]
  00539905: inc         eax
  00539906: mov         dword ptr [esi+00059A6Ch],0
  00539910: mov         dword ptr [esi+00059A60h],eax
  00539916: mov         edx,eax
  00539918: movsx       eax,byte ptr ds:[00E38EA8h]
  0053991F: lea         ecx,[eax+eax*2]
  00539922: lea         ecx,[ecx+ecx*8]
  00539925: shl         ecx,4
  00539928: add         ecx,eax
  0053992A: xor         eax,eax
  0053992C: lea         edx,[edx+ecx*4]
  0053992F: mov         edi,dword ptr [esi+edx*4+00001A40h]
  00539936: cmp         edi,1
  00539939: pop         edi
  0053993A: setne       al
  0053993D: inc         eax
  0053993E: mov         dword ptr [esi+00059A64h],eax
  00539944: pop         esi
  00539945: ret
  00539946: mov         dl,byte ptr ds:[00E38EA8h]
  0053994C: movsx       eax,dl
  0053994F: lea         ecx,[eax+eax*2]
  00539952: lea         ecx,[ecx+ecx*8]
  00539955: shl         ecx,4
  00539958: add         ecx,eax
  0053995A: shl         ecx,4
  0053995D: mov         eax,dword ptr [ecx+esi+00001B0Ch]
  00539964: test        eax,eax
  00539966: jle         005399B4
  00539968: movsx       eax,dl
  0053996B: push        1
  0053996D: push        0
  0053996F: lea         ecx,[eax+eax*2]
  00539972: lea         edi,[ecx+ecx*8]
  00539975: shl         edi,4
  00539978: add         edi,eax
  0053997A: shl         edi,4
  0053997D: call        00562348
  00539982: imul        eax,dword ptr [edi+esi+00001B0Ch]
  0053998A: cdq
  0053998B: and         edx,7FFFh
  00539991: mov         ecx,0E92BF8h
  00539996: add         eax,edx
  00539998: sar         eax,0Fh
  0053999B: add         eax,32h
  0053999E: lea         edx,[eax*8+00000000h]
  005399A5: sub         edx,eax
  005399A7: shl         edx,4
  005399AA: add         edx,edi
  005399AC: add         edx,esi
  005399AE: push        edx
  005399AF: call        0055F449
  005399B4: mov         dword ptr [esi+00059A64h],2
  005399BE: pop         edi
  005399BF: pop         esi
  005399C0: ret
  005399C1: mov         dword ptr [esi+00059A64h],3
  005399CB: call        dword ptr ds:[0056F12Ch]
  005399D1: mov         dword ptr [esi+00059A68h],eax
  005399D7: pop         edi
  005399D8: pop         esi
  005399D9: ret
  005399DA: call        dword ptr ds:[0056F12Ch]
  005399E0: mov         ecx,dword ptr [esi+00059A68h]
  005399E6: mov         edi,dword ptr ds:[00F55B98h]
  005399EC: add         ecx,edi
  005399EE: cmp         eax,ecx
  005399F0: jbe         00539A22
  005399F2: mov         edx,dword ptr [esi+00059A60h]
  005399F8: mov         eax,dword ptr [esi+00059A58h]
  005399FE: cmp         edx,eax
  00539A00: jle         00539A0E
  00539A02: mov         eax,dword ptr [esi+00059A54h]
  00539A08: mov         dword ptr [esi+00059A60h],eax
  00539A0E: mov         dword ptr [esi+00059A64h],0
  00539A18: mov         dword ptr [esi+00059A6Ch],1
  00539A22: pop         edi
  00539A23: pop         esi
  00539A24: ret
  00539A25: lea         ecx,[ecx]
  00539A28: jne         005399C2
  00539A2A: push        ebx
  00539A2B: add         byte ptr [esi-67h],al
  00539A2E: push        ebx
  00539A2F: add         cl,al
  00539A31: cdq
  00539A32: push        ebx
  00539A33: add         dl,bl
  00539A35: cdq
  00539A36: push        ebx
  00539A37: add         byte ptr [eax+90909090h],dl
  00539A3D: nop
  00539A3E: nop
  00539A3F: nop
  00539A40: mov         eax,dword ptr [esp+4]
  00539A44: mov         dword ptr [ecx+00059A50h],eax
  00539A4A: ret         4
  00539A4D: nop
  00539A4E: nop
  00539A4F: nop
  00539A50: mov         eax,dword ptr [esp+4]
  00539A54: mov         edx,dword ptr [esp+8]
  00539A58: mov         dword ptr [ecx+00059A54h],eax
  00539A5E: mov         dword ptr [ecx+00059A58h],edx
  00539A64: ret         8
  00539A67: nop

  Summary

      16E000 .text
