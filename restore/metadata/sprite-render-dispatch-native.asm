Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  0053E590: sub         esp,10Ch
  0053E596: mov         edx,dword ptr [esp+00000118h]
  0053E59D: push        ebx
  0053E59E: push        ebp
  0053E59F: mov         ebp,ecx
  0053E5A1: mov         ecx,dword ptr [esp+0000011Ch]
  0053E5A8: push        esi
  0053E5A9: mov         esi,dword ptr [esp+0000011Ch]
  0053E5B0: mov         dword ptr ds:[00D3B57Ch],ecx
  0053E5B6: mov         dword ptr ds:[00D3B580h],esi
  0053E5BC: mov         dword ptr ds:[00D3B578h],edx
  0053E5C2: mov         eax,dword ptr [ebp+0030F5CCh]
  0053E5C8: push        edi
  0053E5C9: cmp         esi,eax
  0053E5CB: mov         dword ptr [esp+14h],0
  0053E5D3: mov         byte ptr [esp+13h],0
  0053E5D8: mov         ebx,esi
  0053E5DA: jl          0053E5DE
  0053E5DC: xor         ebx,ebx
  0053E5DE: shl         ebx,6
  0053E5E1: test        esi,esi
  0053E5E3: lea         ebx,[ebx+ebp+00180E48h]
  0053E5EA: jg          0053E5FB
  0053E5EC: pop         edi
  0053E5ED: pop         esi
  0053E5EE: pop         ebp
  0053E5EF: xor         eax,eax
  0053E5F1: pop         ebx
  0053E5F2: add         esp,10Ch
  0053E5F8: ret         0Ch
  0053E5FB: cmp         esi,639Ch
  0053E601: jl          0053E612
  0053E603: pop         edi
  0053E604: pop         esi
  0053E605: pop         ebp
  0053E606: xor         eax,eax
  0053E608: pop         ebx
  0053E609: add         esp,10Ch
  0053E60F: ret         0Ch
  0053E612: test        ebx,ebx
  0053E614: jne         0053E625
  0053E616: pop         edi
  0053E617: pop         esi
  0053E618: pop         ebp
  0053E619: xor         eax,eax
  0053E61B: pop         ebx
  0053E61C: add         esp,10Ch
  0053E622: ret         0Ch
  0053E625: cmp         byte ptr [ebx+3Eh],0
  0053E629: je          0053E658
  0053E62B: mov         ebx,dword ptr [ebx+10h]
  0053E62E: add         ebx,esi
  0053E630: cmp         ebx,eax
  0053E632: jl          0053E636
  0053E634: xor         ebx,ebx
  0053E636: shl         ebx,6
  0053E639: mov         byte ptr [esp+13h],1
  0053E63E: lea         ebx,[ebx+ebp+00180E48h]
  0053E645: test        ebx,ebx
  0053E647: jne         0053E658
  0053E649: pop         edi
  0053E64A: pop         esi
  0053E64B: pop         ebp
  0053E64C: xor         eax,eax
  0053E64E: pop         ebx
  0053E64F: add         esp,10Ch
  0053E655: ret         0Ch
  0053E658: mov         eax,dword ptr [ebx]
  0053E65A: test        eax,eax
  0053E65C: mov         dword ptr ds:[00D3B5B8h],eax
  0053E661: jg          0053E672
  0053E663: pop         edi
  0053E664: pop         esi
  0053E665: pop         ebp
  0053E666: xor         eax,eax
  0053E668: pop         ebx
  0053E669: add         esp,10Ch
  0053E66F: ret         0Ch
  0053E672: mov         di,word ptr [ebx+14h]
  0053E676: mov         word ptr ds:[00D3B5BEh],di
  0053E67D: mov         ax,word ptr [ebx+16h]
  0053E681: mov         word ptr ds:[00D3B5BCh],ax
  0053E687: mov         al,byte ptr [ebx+32h]
  0053E68A: mov         dword ptr ds:[00D3B5E0h],ecx
  0053E690: mov         byte ptr ds:[00D3B5B4h],al
  0053E695: movsx       ecx,di
  0053E698: mov         dword ptr ds:[00D3B5DCh],edx
  0053E69E: mov         edx,dword ptr ds:[00E38D9Ch]
  0053E6A4: mov         dword ptr ds:[00D3B544h],ecx
  0053E6AA: mov         al,byte ptr [ebx+34h]
  0053E6AD: test        al,al
  0053E6AF: je          0053EA7E
  0053E6B5: cmp         ecx,edx
  0053E6B7: jg          0053EA78
  0053E6BD: mov         ecx,dword ptr ds:[006018F8h]
  0053E6C3: mov         dx,word ptr ds:[006018FCh]
  0053E6CA: mov         dword ptr [esp+18h],ecx
  0053E6CE: mov         ecx,3Fh
  0053E6D3: xor         eax,eax
  0053E6D5: lea         edi,[esp+1Eh]
  0053E6D9: mov         word ptr [esp+1Ch],dx
  0053E6DE: rep stos    dword ptr es:[edi]
  0053E6E0: mov         ecx,dword ptr [ebp+0030F5CCh]
  0053E6E6: stos        word ptr es:[edi]
  0053E6E8: cmp         esi,ecx
  0053E6EA: mov         eax,esi
  0053E6EC: jl          0053E6F6
  0053E6EE: xor         eax,eax
  0053E6F0: cmp         esi,ecx
  0053E6F2: jl          0053E6F6
  0053E6F4: xor         esi,esi
  0053E6F6: add         eax,603Ah
  0053E6FB: xor         ecx,ecx
  0053E6FD: shl         eax,6
  0053E700: shl         esi,6
  0053E703: mov         cl,byte ptr [eax+ebp]
  0053E706: push        5E06E0h
  0053E70B: mov         eax,ecx
  0053E70D: mov         edx,eax
  0053E70F: shl         edx,6
  0053E712: add         edx,eax
  0053E714: mov         eax,dword ptr [esi+ebp+00180E54h]
  0053E71B: mov         ecx,eax
  0053E71D: shl         ecx,4
  0053E720: add         ecx,eax
  0053E722: shl         ecx,6
  0053E725: add         ecx,eax
  0053E727: lea         eax,[eax+ecx*4]
  0053E72A: lea         eax,[eax+eax*2]
  0053E72D: lea         ecx,[ebp+eax*4]
  0053E731: xor         eax,eax
  0053E733: lea         edi,[ecx+edx*4+00002044h]
  0053E73A: or          ecx,0FFFFFFFFh
  0053E73D: repne scas  byte ptr es:[edi]
  0053E73F: not         ecx
  0053E741: sub         edi,ecx
  0053E743: lea         edx,[esp+1Ch]
  0053E747: mov         esi,edi
  0053E749: mov         edi,edx
  0053E74B: mov         edx,ecx
  0053E74D: or          ecx,0FFFFFFFFh
  0053E750: repne scas  byte ptr es:[edi]
  0053E752: mov         ecx,edx
  0053E754: dec         edi
  0053E755: shr         ecx,2
  0053E758: rep movs    dword ptr es:[edi],dword ptr [esi]
  0053E75A: mov         ecx,edx
  0053E75C: lea         eax,[esp+1Ch]
  0053E760: and         ecx,3
  0053E763: push        eax
  0053E764: rep movs    byte ptr es:[edi],byte ptr [esi]
  0053E766: call        004CE310
  0053E76B: lea         esi,[ebp+0030F5C0h]
  0053E771: push        0
  0053E773: push        esi
  0053E774: call        0053F790
  0053E779: mov         ecx,dword ptr ds:[00E92E1Ch]
  0053E77F: mov         dword ptr [esi],ecx
  0053E781: mov         edx,dword ptr [ebx]
  0053E783: mov         eax,dword ptr [ebx+4]
  0053E786: dec         edx
  0053E787: push        edx
  0053E788: push        eax
  0053E789: lea         edx,[esp+30h]
  0053E78D: push        ecx
  0053E78E: push        edx
  0053E78F: call        004CE480
  0053E794: add         esp,20h
  0053E797: test        eax,eax
  0053E799: jne         0053E7A8
  0053E79B: pop         edi
  0053E79C: pop         esi
  0053E79D: pop         ebp
  0053E79E: pop         ebx
  0053E79F: add         esp,10Ch
  0053E7A5: ret         0Ch
  0053E7A8: mov         al,byte ptr [ebx+33h]
  0053E7AB: test        al,al
  0053E7AD: je          0053E7BD
  0053E7AF: mov         eax,dword ptr [ebx+4]
  0053E7B2: mov         ecx,dword ptr [esi]
  0053E7B4: push        eax
  0053E7B5: push        ecx
  0053E7B6: call        004CDEB0
  0053E7BB: jmp         0053E7C9
  0053E7BD: mov         edx,dword ptr [ebx+4]
  0053E7C0: mov         eax,dword ptr [esi]
  0053E7C2: push        edx
  0053E7C3: push        eax
  0053E7C4: call        004CDF20
  0053E7C9: add         esp,8
  0053E7CC: call        004CDAD0
  0053E7D1: call        004CDB80
  0053E7D6: mov         al,byte ptr ds:[00D3B530h]
  0053E7DB: movsx       edi,word ptr ds:[00D3B5BEh]
  0053E7E2: cmp         al,5
  0053E7E4: je          0053EA67
  0053E7EA: mov         al,byte ptr ds:[00D3B5B4h]
  0053E7EF: test        al,al
  0053E7F1: jne         0053E985
  0053E7F7: mov         al,byte ptr [ebx+33h]
  0053E7FA: test        al,al
  0053E7FC: je          0053E8BB
  0053E802: mov         al,byte ptr [esp+13h]
  0053E806: test        al,al
  0053E808: mov         al,byte ptr ds:[00D3B531h]
  0053E80D: je          0053E865
  0053E80F: cmp         al,1
  0053E811: jne         0053E82D
  0053E813: mov         ecx,dword ptr [esi]
  0053E815: push        ecx
  0053E816: call        0040FDD0
  0053E81B: add         esp,4
  0053E81E: mov         eax,edi
  0053E820: pop         edi
  0053E821: pop         esi
  0053E822: pop         ebp
  0053E823: pop         ebx
  0053E824: add         esp,10Ch
  0053E82A: ret         0Ch
  0053E82D: cmp         al,2
  0053E82F: jne         0053E84B
  0053E831: mov         edx,dword ptr [esi]
  0053E833: push        edx
  0053E834: call        0040FCF0
  0053E839: add         esp,4
  0053E83C: mov         eax,edi
  0053E83E: pop         edi
  0053E83F: pop         esi
  0053E840: pop         ebp
  0053E841: pop         ebx
  0053E842: add         esp,10Ch
  0053E848: ret         0Ch
  0053E84B: mov         eax,dword ptr [esi]
  0053E84D: push        eax
  0053E84E: call        0040FC40
  0053E853: add         esp,4
  0053E856: mov         eax,edi
  0053E858: pop         edi
  0053E859: pop         esi
  0053E85A: pop         ebp
  0053E85B: pop         ebx
  0053E85C: add         esp,10Ch
  0053E862: ret         0Ch
  0053E865: cmp         al,1
  0053E867: jne         0053E883
  0053E869: mov         ecx,dword ptr [esi]
  0053E86B: push        ecx
  0053E86C: call        004105C0
  0053E871: add         esp,4
  0053E874: mov         eax,edi
  0053E876: pop         edi
  0053E877: pop         esi
  0053E878: pop         ebp
  0053E879: pop         ebx
  0053E87A: add         esp,10Ch
  0053E880: ret         0Ch
  0053E883: cmp         al,2
  0053E885: jne         0053E8A1
  0053E887: mov         edx,dword ptr [esi]
  0053E889: push        edx
  0053E88A: call        00410660
  0053E88F: add         esp,4
  0053E892: mov         eax,edi
  0053E894: pop         edi
  0053E895: pop         esi
  0053E896: pop         ebp
  0053E897: pop         ebx
  0053E898: add         esp,10Ch
  0053E89E: ret         0Ch
  0053E8A1: mov         eax,dword ptr [esi]
  0053E8A3: push        eax
  0053E8A4: call        00410230
  0053E8A9: add         esp,4
  0053E8AC: mov         eax,edi
  0053E8AE: pop         edi
  0053E8AF: pop         esi
  0053E8B0: pop         ebp
  0053E8B1: pop         ebx
  0053E8B2: add         esp,10Ch
  0053E8B8: ret         0Ch
  0053E8BB: mov         eax,edi
  0053E8BD: mov         ebx,dword ptr ds:[00D3B54Ch]
  0053E8C3: imul        eax,dword ptr ds:[00D3B538h]
  0053E8CA: mov         cl,byte ptr [esp+13h]
  0053E8CE: add         eax,ebx
  0053E8D0: shl         eax,1
  0053E8D2: test        cl,cl
  0053E8D4: mov         cl,byte ptr ds:[00D3B531h]
  0053E8DA: mov         dword ptr ds:[00D3B5B8h],eax
  0053E8DF: je          0053E927
  0053E8E1: cmp         cl,1
  0053E8E4: jne         0053E902
  0053E8E6: mov         ecx,dword ptr [esi]
  0053E8E8: add         ecx,eax
  0053E8EA: push        ecx
  0053E8EB: call        0040FF50
  0053E8F0: add         esp,4
  0053E8F3: mov         eax,edi
  0053E8F5: pop         edi
  0053E8F6: pop         esi
  0053E8F7: pop         ebp
  0053E8F8: pop         ebx
  0053E8F9: add         esp,10Ch
  0053E8FF: ret         0Ch
  0053E902: cmp         cl,2
  0053E905: jne         0053E9ED
  0053E90B: mov         edx,dword ptr [esi]
  0053E90D: add         edx,eax
  0053E90F: push        edx
  0053E910: call        0040FF50
  0053E915: add         esp,4
  0053E918: mov         eax,edi
  0053E91A: pop         edi
  0053E91B: pop         esi
  0053E91C: pop         ebp
  0053E91D: pop         ebx
  0053E91E: add         esp,10Ch
  0053E924: ret         0Ch
  0053E927: cmp         cl,1
  0053E92A: jne         0053E948
  0053E92C: mov         edx,dword ptr [esi]
  0053E92E: add         edx,eax
  0053E930: push        edx
  0053E931: call        00410090
  0053E936: add         esp,4
  0053E939: mov         eax,edi
  0053E93B: pop         edi
  0053E93C: pop         esi
  0053E93D: pop         ebp
  0053E93E: pop         ebx
  0053E93F: add         esp,10Ch
  0053E945: ret         0Ch
  0053E948: cmp         cl,2
  0053E94B: jne         0053E969
  0053E94D: mov         ecx,dword ptr [esi]
  0053E94F: add         ecx,eax
  0053E951: push        ecx
  0053E952: call        00410090
  0053E957: add         esp,4
  0053E95A: mov         eax,edi
  0053E95C: pop         edi
  0053E95D: pop         esi
  0053E95E: pop         ebp
  0053E95F: pop         ebx
  0053E960: add         esp,10Ch
  0053E966: ret         0Ch
  0053E969: mov         edx,dword ptr [esi]
  0053E96B: add         edx,eax
  0053E96D: push        edx
  0053E96E: call        0040FFD0
  0053E973: add         esp,4
  0053E976: mov         eax,edi
  0053E978: pop         edi
  0053E979: pop         esi
  0053E97A: pop         ebp
  0053E97B: pop         ebx
  0053E97C: add         esp,10Ch
  0053E982: ret         0Ch
  0053E985: mov         eax,edi
  0053E987: mov         ebx,dword ptr ds:[00D3B54Ch]
  0053E98D: imul        eax,dword ptr ds:[00D3B538h]
  0053E994: mov         cl,byte ptr [esp+13h]
  0053E998: add         eax,ebx
  0053E99A: shl         eax,1
  0053E99C: test        cl,cl
  0053E99E: mov         cl,byte ptr ds:[00D3B531h]
  0053E9A4: mov         dword ptr ds:[00D3B5B8h],eax
  0053E9A9: je          0053EA09
  0053E9AB: cmp         cl,1
  0053E9AE: jne         0053E9CC
  0053E9B0: mov         ecx,dword ptr [esi]
  0053E9B2: add         ecx,eax
  0053E9B4: push        ecx
  0053E9B5: call        0040FF50
  0053E9BA: add         esp,4
  0053E9BD: mov         eax,edi
  0053E9BF: pop         edi
  0053E9C0: pop         esi
  0053E9C1: pop         ebp
  0053E9C2: pop         ebx
  0053E9C3: add         esp,10Ch
  0053E9C9: ret         0Ch
  0053E9CC: cmp         cl,2
  0053E9CF: jne         0053E9ED
  0053E9D1: mov         edx,dword ptr [esi]
  0053E9D3: add         edx,eax
  0053E9D5: push        edx
  0053E9D6: call        0040FF50
  0053E9DB: add         esp,4
  0053E9DE: mov         eax,edi
  0053E9E0: pop         edi
  0053E9E1: pop         esi
  0053E9E2: pop         ebp
  0053E9E3: pop         ebx
  0053E9E4: add         esp,10Ch
  0053E9EA: ret         0Ch
  0053E9ED: mov         ecx,dword ptr [esi]
  0053E9EF: add         ecx,eax
  0053E9F1: push        ecx
  0053E9F2: call        0040FED0
  0053E9F7: add         esp,4
  0053E9FA: mov         eax,edi
  0053E9FC: pop         edi
  0053E9FD: pop         esi
  0053E9FE: pop         ebp
  0053E9FF: pop         ebx
  0053EA00: add         esp,10Ch
  0053EA06: ret         0Ch
  0053EA09: cmp         cl,1
  0053EA0C: jne         0053EA2A
  0053EA0E: mov         edx,dword ptr [esi]
  0053EA10: add         edx,eax
  0053EA12: push        edx
  0053EA13: call        00410090
  0053EA18: add         esp,4
  0053EA1B: mov         eax,edi
  0053EA1D: pop         edi
  0053EA1E: pop         esi
  0053EA1F: pop         ebp
  0053EA20: pop         ebx
  0053EA21: add         esp,10Ch
  0053EA27: ret         0Ch
  0053EA2A: cmp         cl,2
  0053EA2D: jne         0053EA4B
  0053EA2F: mov         ecx,dword ptr [esi]
  0053EA31: add         ecx,eax
  0053EA33: push        ecx
  0053EA34: call        00410090
  0053EA39: add         esp,4
  0053EA3C: mov         eax,edi
  0053EA3E: pop         edi
  0053EA3F: pop         esi
  0053EA40: pop         ebp
  0053EA41: pop         ebx
  0053EA42: add         esp,10Ch
  0053EA48: ret         0Ch
  0053EA4B: mov         edx,dword ptr [esi]
  0053EA4D: add         edx,eax
  0053EA4F: push        edx
  0053EA50: call        0040FFD0
  0053EA55: add         esp,4
  0053EA58: mov         eax,edi
  0053EA5A: pop         edi
  0053EA5B: pop         esi
  0053EA5C: pop         ebp
  0053EA5D: pop         ebx
  0053EA5E: add         esp,10Ch
  0053EA64: ret         0Ch
  0053EA67: xor         edi,edi
  0053EA69: mov         eax,edi
  0053EA6B: pop         edi
  0053EA6C: pop         esi
  0053EA6D: pop         ebp
  0053EA6E: pop         ebx
  0053EA6F: add         esp,10Ch
  0053EA75: ret         0Ch
  0053EA78: mov         edx,dword ptr ds:[00E38D9Ch]
  0053EA7E: test        al,al
  0053EA80: je          0053EBC3
  0053EA86: mov         ebx,dword ptr ds:[00E38DB0h]
  0053EA8C: mov         eax,400h
  0053EA91: sub         eax,ebx
  0053EA93: mov         dword ptr ds:[00D3B574h],ebx
  0053EA99: cdq
  0053EA9A: sub         eax,edx
  0053EA9C: mov         edx,dword ptr ds:[00E38DACh]
  0053EAA2: mov         edi,eax
  0053EAA4: mov         eax,300h
  0053EAA9: sub         eax,edx
  0053EAAB: mov         dword ptr ds:[00D3B544h],ebx
  0053EAB1: cdq
  0053EAB2: sub         eax,edx
  0053EAB4: mov         edx,dword ptr ds:[00E38DACh]
  0053EABA: sar         eax,1
  0053EABC: imul        ecx,eax
  0053EABF: sar         edi,1
  0053EAC1: add         ecx,edi
  0053EAC3: mov         dword ptr ds:[00D3B56Ch],edi
  0053EAC9: mov         dword ptr ds:[00D3B568h],eax
  0053EACE: mov         dword ptr ds:[00D3B570h],edx
  0053EAD4: mov         dword ptr ds:[00D3B5D4h],edx
  0053EADA: mov         eax,dword ptr [ebp+0030F5CCh]
  0053EAE0: shl         ecx,1
  0053EAE2: mov         dword ptr [esp+14h],ecx
  0053EAE6: cmp         esi,eax
  0053EAE8: mov         ecx,esi
  0053EAEA: jl          0053EAF4
  0053EAEC: xor         ecx,ecx
  0053EAEE: cmp         esi,eax
  0053EAF0: jl          0053EAF4
  0053EAF2: xor         esi,esi
  0053EAF4: add         ecx,603Ah
  0053EAFA: xor         eax,eax
  0053EAFC: shl         ecx,6
  0053EAFF: shl         esi,6
  0053EB02: mov         al,byte ptr [ecx+ebp]
  0053EB05: mov         ecx,eax
  0053EB07: shl         ecx,6
  0053EB0A: add         ecx,eax
  0053EB0C: mov         eax,dword ptr [esi+ebp+00180E54h]
  0053EB13: mov         edx,eax
  0053EB15: shl         edx,4
  0053EB18: add         edx,eax
  0053EB1A: shl         edx,6
  0053EB1D: add         edx,eax
  0053EB1F: lea         eax,[eax+edx*4]
  0053EB22: lea         eax,[eax+eax*2]
  0053EB25: lea         edx,[ebp+eax*4]
  0053EB29: xor         eax,eax
  0053EB2B: lea         edi,[edx+ecx*4+00002044h]
  0053EB32: or          ecx,0FFFFFFFFh
  0053EB35: repne scas  byte ptr es:[edi]
  0053EB37: not         ecx
  0053EB39: sub         edi,ecx
  0053EB3B: mov         eax,ecx
  0053EB3D: mov         esi,edi
  0053EB3F: mov         edi,0E38AA8h
  0053EB44: shr         ecx,2
  0053EB47: rep movs    dword ptr es:[edi],dword ptr [esi]
  0053EB49: mov         ecx,eax
  0053EB4B: and         ecx,3
  0053EB4E: rep movs    byte ptr es:[edi],byte ptr [esi]
  0053EB50: xor         edi,edi
  0053EB52: lea         esi,[ebp+0030F5C0h]
  0053EB58: push        edi
  0053EB59: push        esi
  0053EB5A: call        0053F790
  0053EB5F: mov         ecx,dword ptr ds:[00E92E1Ch]
  0053EB65: mov         edx,dword ptr [esp+1Ch]
  0053EB69: add         esp,8
  0053EB6C: mov         dword ptr [esi],ecx
  0053EB6E: mov         ecx,dword ptr ds:[00D3B544h]
  0053EB74: mov         eax,dword ptr ds:[00D3B5D4h]
  0053EB79: push        edx
  0053EB7A: mov         dword ptr ds:[00D3B5DCh],edi
  0053EB80: mov         dword ptr ds:[00D3B5E0h],edi
  0053EB86: push        eax
  0053EB87: mov         eax,dword ptr [esi]
  0053EB89: lea         edx,[ecx+ecx]
  0053EB8C: push        edx
  0053EB8D: push        eax
  0053EB8E: push        0E38AA8h
  0053EB93: mov         ecx,ebp
  0053EB95: call        0053F7D0
  0053EB9A: mov         dword ptr ds:[00D3B540h],edi
  0053EBA0: mov         dword ptr ds:[00D3B53Ch],edi
  0053EBA6: mov         ecx,dword ptr [esi]
  0053EBA8: push        ecx
  0053EBA9: call        0040FB60
  0053EBAE: mov         eax,dword ptr ds:[00D3B544h]
  0053EBB3: add         esp,4
  0053EBB6: pop         edi
  0053EBB7: pop         esi
  0053EBB8: pop         ebp
  0053EBB9: pop         ebx
  0053EBBA: add         esp,10Ch
  0053EBC0: ret         0Ch
  0053EBC3: mov         al,byte ptr ds:[00E38DDAh]
  0053EBC8: cmp         al,15h
  0053EBCA: je          0053EBD0
  0053EBCC: cmp         al,14h
  0053EBCE: jne         0053EC37
  0053EBD0: cmp         esi,dword ptr ds:[00F56DB4h]
  0053EBD6: jne         0053EC37
  0053EBD8: mov         esi,dword ptr ds:[00D3B570h]
  0053EBDE: mov         eax,dword ptr ds:[00D3B574h]
  0053EBE3: mov         dword ptr ds:[00D3B5D4h],esi
  0053EBE9: mov         esi,ecx
  0053EBEB: imul        ecx,dword ptr ds:[00D3B568h]
  0053EBF2: sub         edx,eax
  0053EBF4: sub         esi,eax
  0053EBF6: shl         edx,1
  0053EBF8: mov         dword ptr ds:[00D3B53Ch],edx
  0053EBFE: mov         edx,dword ptr ds:[00D3B56Ch]
  0053EC04: shl         esi,1
  0053EC06: add         ecx,edx
  0053EC08: mov         edx,dword ptr ds:[00D3B5B8h]
  0053EC0E: mov         dword ptr ds:[00D3B544h],eax
  0053EC13: mov         dword ptr ds:[00D3B540h],esi
  0053EC19: lea         eax,[edx+ecx*2]
  0053EC1C: push        eax
  0053EC1D: call        0040FB60
  0053EC22: add         esp,4
  0053EC25: mov         eax,dword ptr ds:[00D3B544h]
  0053EC2A: pop         edi
  0053EC2B: pop         esi
  0053EC2C: pop         ebp
  0053EC2D: pop         ebx
  0053EC2E: add         esp,10Ch
  0053EC34: ret         0Ch
  0053EC37: cmp         byte ptr ds:[00D3B5B4h],1Eh
  0053EC3E: jne         0053EC4F
  0053EC40: cmp         di,3Ah
  0053EC44: jne         0053ECAA
  0053EC46: mov         word ptr ds:[00D3B5BCh],1Eh
  0053EC4F: call        004CDAD0
  0053EC54: call        004CDB80
  0053EC59: mov         dl,byte ptr ds:[00D3B530h]
  0053EC5F: cmp         dl,5
  0053EC62: je          0053F232
  0053EC68: mov         al,byte ptr ds:[00D3B5B4h]
  0053EC6D: test        al,al
  0053EC6F: jne         0053EF14
  0053EC75: mov         al,byte ptr [ebx+33h]
  0053EC78: test        al,al
  0053EC7A: je          0053EE1E
  0053EC80: mov         al,byte ptr [esp+13h]
  0053EC84: test        al,al
  0053EC86: mov         al,byte ptr ds:[00D3B531h]
  0053EC8B: je          0053ED6B
  0053EC91: cmp         al,1
  0053EC93: jne         0053ED4C
  0053EC99: mov         ecx,dword ptr ds:[00D3B5B8h]
  0053EC9F: push        ecx
  0053ECA0: call        0040FDD0
  0053ECA5: jmp         0053ED9A
  0053ECAA: cmp         di,1Ah
  0053ECAE: jne         0053ECBB
  0053ECB0: mov         word ptr ds:[00D3B5BCh],0Eh
  0053ECB9: jmp         0053EC4F
  0053ECBB: cmp         di,0Ah
  0053ECBF: jne         0053ECCC
  0053ECC1: mov         word ptr ds:[00D3B5BCh],6
  0053ECCA: jmp         0053EC4F
  0053ECCC: cmp         di,76h
  0053ECD0: jne         0053ECE4
  0053ECD2: call        0042C740
  0053ECD7: pop         edi
  0053ECD8: pop         esi
  0053ECD9: pop         ebp
  0053ECDA: pop         ebx
  0053ECDB: add         esp,10Ch
  0053ECE1: ret         0Ch
  0053ECE4: cmp         di,0B2h
  0053ECE9: jne         0053ECFD
  0053ECEB: call        0042C9A0
  0053ECF0: pop         edi
  0053ECF1: pop         esi
  0053ECF2: pop         ebp
  0053ECF3: pop         ebx
  0053ECF4: add         esp,10Ch
  0053ECFA: ret         0Ch
  0053ECFD: cmp         di,0EEh
  0053ED02: jne         0053ED16
  0053ED04: call        0042CEE0
  0053ED09: pop         edi
  0053ED0A: pop         esi
  0053ED0B: pop         ebp
  0053ED0C: pop         ebx
  0053ED0D: add         esp,10Ch
  0053ED13: ret         0Ch
  0053ED16: cmp         di,12Ah
  0053ED1B: jne         0053ED2F
  0053ED1D: call        0042D810
  0053ED22: pop         edi
  0053ED23: pop         esi
  0053ED24: pop         ebp
  0053ED25: pop         ebx
  0053ED26: add         esp,10Ch
  0053ED2C: ret         0Ch
  0053ED2F: cmp         di,166h
  0053ED34: jne         0053EC4F
  0053ED3A: call        0042E680
  0053ED3F: pop         edi
  0053ED40: pop         esi
  0053ED41: pop         ebp
  0053ED42: pop         ebx
  0053ED43: add         esp,10Ch
  0053ED49: ret         0Ch
  0053ED4C: cmp         al,2
  0053ED4E: jne         0053ED5E
  0053ED50: mov         edx,dword ptr ds:[00D3B5B8h]
  0053ED56: push        edx
  0053ED57: call        0040FCF0
  0053ED5C: jmp         0053ED9A
  0053ED5E: mov         eax,dword ptr ds:[00D3B5B8h]
  0053ED63: push        eax
  0053ED64: call        0040FC40
  0053ED69: jmp         0053ED9A
  0053ED6B: cmp         al,1
  0053ED6D: jne         0053ED7D
  0053ED6F: mov         ecx,dword ptr ds:[00D3B5B8h]
  0053ED75: push        ecx
  0053ED76: call        004105C0
  0053ED7B: jmp         0053ED9A
  0053ED7D: cmp         al,2
  0053ED7F: jne         0053ED8F
  0053ED81: mov         edx,dword ptr ds:[00D3B5B8h]
  0053ED87: push        edx
  0053ED88: call        00410660
  0053ED8D: jmp         0053ED9A
  0053ED8F: mov         eax,dword ptr ds:[00D3B5B8h]
  0053ED94: push        eax
  0053ED95: call        00410230
  0053ED9A: mov         cx,word ptr ds:[0070E120h]
  0053EDA1: add         esp,4
  0053EDA4: test        cx,cx
  0053EDA7: je          0053EC25
  0053EDAD: mov         al,byte ptr ds:[00D3B531h]
  0053EDB2: cmp         al,1
  0053EDB4: jne         0053EDD8
  0053EDB6: push        ecx
  0053EDB7: mov         ecx,dword ptr ds:[00D3B5B8h]
  0053EDBD: push        ecx
  0053EDBE: call        004109D0
  0053EDC3: mov         eax,dword ptr ds:[00D3B544h]
  0053EDC8: add         esp,8
  0053EDCB: pop         edi
  0053EDCC: pop         esi
  0053EDCD: pop         ebp
  0053EDCE: pop         ebx
  0053EDCF: add         esp,10Ch
  0053EDD5: ret         0Ch
  0053EDD8: cmp         al,2
  0053EDDA: push        ecx
  0053EDDB: jne         0053EDFE
  0053EDDD: mov         edx,dword ptr ds:[00D3B5B8h]
  0053EDE3: push        edx
  0053EDE4: call        00410A70
  0053EDE9: mov         eax,dword ptr ds:[00D3B544h]
  0053EDEE: add         esp,8
  0053EDF1: pop         edi
  0053EDF2: pop         esi
  0053EDF3: pop         ebp
  0053EDF4: pop         ebx
  0053EDF5: add         esp,10Ch
  0053EDFB: ret         0Ch
  0053EDFE: mov         eax,dword ptr ds:[00D3B5B8h]
  0053EE03: push        eax
  0053EE04: call        00410700
  0053EE09: mov         eax,dword ptr ds:[00D3B544h]
  0053EE0E: add         esp,8
  0053EE11: pop         edi
  0053EE12: pop         esi
  0053EE13: pop         ebp
  0053EE14: pop         ebx
  0053EE15: add         esp,10Ch
  0053EE1B: ret         0Ch
  0053EE1E: movsx       ecx,word ptr ds:[00D3B5BEh]
  0053EE25: imul        ecx,dword ptr ds:[00D3B538h]
  0053EE2C: mov         edx,dword ptr ds:[00D3B54Ch]
  0053EE32: add         ecx,edx
  0053EE34: mov         edx,dword ptr ds:[00D3B5B8h]
  0053EE3A: lea         eax,[edx+ecx*2]
  0053EE3D: mov         cl,byte ptr [esp+13h]
  0053EE41: test        cl,cl
  0053EE43: mov         cl,byte ptr ds:[00D3B531h]
  0053EE49: mov         dword ptr ds:[00D3B5B8h],eax
  0053EE4E: je          0053EE71
  0053EE50: cmp         cl,1
  0053EE53: jne         0053EE5D
  0053EE55: push        eax
  0053EE56: call        0040FF50
  0053EE5B: jmp         0053EE90
  0053EE5D: cmp         cl,2
  0053EE60: push        eax
  0053EE61: jne         0053EE6A
  0053EE63: call        0040FF50
  0053EE68: jmp         0053EE90
  0053EE6A: call        0040FED0
  0053EE6F: jmp         0053EE90
  0053EE71: cmp         cl,1
  0053EE74: jne         0053EE7E
  0053EE76: push        eax
  0053EE77: call        00410090
  0053EE7C: jmp         0053EE90
  0053EE7E: cmp         cl,2
  0053EE81: push        eax
  0053EE82: jne         0053EE8B
  0053EE84: call        00410090
  0053EE89: jmp         0053EE90
  0053EE8B: call        0040FFD0
  0053EE90: mov         cx,word ptr ds:[0070E120h]
  0053EE97: add         esp,4
  0053EE9A: test        cx,cx
  0053EE9D: je          0053EC25
  0053EEA3: mov         al,byte ptr ds:[00D3B531h]
  0053EEA8: cmp         al,1
  0053EEAA: jne         0053EECD
  0053EEAC: mov         eax,dword ptr ds:[00D3B5B8h]
  0053EEB1: push        ecx
  0053EEB2: push        eax
  0053EEB3: call        004101C0
  0053EEB8: mov         eax,dword ptr ds:[00D3B544h]
  0053EEBD: add         esp,8
  0053EEC0: pop         edi
  0053EEC1: pop         esi
  0053EEC2: pop         ebp
  0053EEC3: pop         ebx
  0053EEC4: add         esp,10Ch
  0053EECA: ret         0Ch
  0053EECD: cmp         al,2
  0053EECF: push        ecx
  0053EED0: jne         0053EEF3
  0053EED2: mov         ecx,dword ptr ds:[00D3B5B8h]
  0053EED8: push        ecx
  0053EED9: call        004101C0
  0053EEDE: mov         eax,dword ptr ds:[00D3B544h]
  0053EEE3: add         esp,8
  0053EEE6: pop         edi
  0053EEE7: pop         esi
  0053EEE8: pop         ebp
  0053EEE9: pop         ebx
  0053EEEA: add         esp,10Ch
  0053EEF0: ret         0Ch
  0053EEF3: mov         edx,dword ptr ds:[00D3B5B8h]
  0053EEF9: push        edx
  0053EEFA: call        004100F0
  0053EEFF: mov         eax,dword ptr ds:[00D3B544h]
  0053EF04: add         esp,8
  0053EF07: pop         edi
  0053EF08: pop         esi
  0053EF09: pop         ebp
  0053EF0A: pop         ebx
  0053EF0B: add         esp,10Ch
  0053EF11: ret         0Ch
  0053EF14: cmp         al,1Eh
  0053EF16: jne         0053F0FC
  0053EF1C: mov         al,byte ptr ds:[00D3B531h]
  0053EF21: cmp         al,1
  0053EF23: jne         0053EFC4
  0053EF29: mov         ax,word ptr ds:[00D3B5BEh]
  0053EF2F: cmp         ax,3Ah
  0053EF33: jne         0053EF5B
  0053EF35: mov         ecx,dword ptr ds:[00D3B5B8h]
  0053EF3B: xor         eax,eax
  0053EF3D: mov         al,dl
  0053EF3F: push        eax
  0053EF40: push        ecx
  0053EF41: call        0042A160
  0053EF46: mov         eax,dword ptr ds:[00D3B544h]
  0053EF4B: add         esp,8
  0053EF4E: pop         edi
  0053EF4F: pop         esi
  0053EF50: pop         ebp
  0053EF51: pop         ebx
  0053EF52: add         esp,10Ch
  0053EF58: ret         0Ch
  0053EF5B: cmp         ax,1Ah
  0053EF5F: jne         0053EF87
  0053EF61: mov         ecx,dword ptr ds:[00D3B5B8h]
  0053EF67: xor         eax,eax
  0053EF69: mov         al,dl
  0053EF6B: push        eax
  0053EF6C: push        ecx
  0053EF6D: call        0051F780
  0053EF72: mov         eax,dword ptr ds:[00D3B544h]
  0053EF77: add         esp,8
  0053EF7A: pop         edi
  0053EF7B: pop         esi
  0053EF7C: pop         ebp
  0053EF7D: pop         ebx
  0053EF7E: add         esp,10Ch
  0053EF84: ret         0Ch
  0053EF87: cmp         ax,0Ah
  0053EF8B: jne         0053EFB3
  0053EF8D: mov         ecx,dword ptr ds:[00D3B5B8h]
  0053EF93: xor         eax,eax
  0053EF95: mov         al,dl
  0053EF97: push        eax
  0053EF98: push        ecx
  0053EF99: call        0051F780
  0053EF9E: mov         eax,dword ptr ds:[00D3B544h]
  0053EFA3: add         esp,8
  0053EFA6: pop         edi
  0053EFA7: pop         esi
  0053EFA8: pop         ebp
  0053EFA9: pop         ebx
  0053EFAA: add         esp,10Ch
  0053EFB0: ret         0Ch
  0053EFB3: mov         edx,dword ptr ds:[00D3B5B8h]
  0053EFB9: push        edx
  0053EFBA: call        00410090
  0053EFBF: jmp         0053EC22
  0053EFC4: cmp         al,2
  0053EFC6: mov         ax,word ptr ds:[00D3B5BEh]
  0053EFCC: jne         0053F067
  0053EFD2: cmp         ax,3Ah
  0053EFD6: jne         0053EFFE
  0053EFD8: mov         ecx,dword ptr ds:[00D3B5B8h]
  0053EFDE: xor         eax,eax
  0053EFE0: mov         al,dl
  0053EFE2: push        eax
  0053EFE3: push        ecx
  0053EFE4: call        0042B440
  0053EFE9: mov         eax,dword ptr ds:[00D3B544h]
  0053EFEE: add         esp,8
  0053EFF1: pop         edi
  0053EFF2: pop         esi
  0053EFF3: pop         ebp
  0053EFF4: pop         ebx
  0053EFF5: add         esp,10Ch
  0053EFFB: ret         0Ch
  0053EFFE: cmp         ax,1Ah
  0053F002: jne         0053F02A
  0053F004: mov         ecx,dword ptr ds:[00D3B5B8h]
  0053F00A: xor         eax,eax
  0053F00C: mov         al,dl
  0053F00E: push        eax
  0053F00F: push        ecx
  0053F010: call        0051F780
  0053F015: mov         eax,dword ptr ds:[00D3B544h]
  0053F01A: add         esp,8
  0053F01D: pop         edi
  0053F01E: pop         esi
  0053F01F: pop         ebp
  0053F020: pop         ebx
  0053F021: add         esp,10Ch
  0053F027: ret         0Ch
  0053F02A: cmp         ax,0Ah
  0053F02E: jne         0053F056
  0053F030: mov         ecx,dword ptr ds:[00D3B5B8h]
  0053F036: xor         eax,eax
  0053F038: mov         al,dl
  0053F03A: push        eax
  0053F03B: push        ecx
  0053F03C: call        0051F780
  0053F041: mov         eax,dword ptr ds:[00D3B544h]
  0053F046: add         esp,8
  0053F049: pop         edi
  0053F04A: pop         esi
  0053F04B: pop         ebp
  0053F04C: pop         ebx
  0053F04D: add         esp,10Ch
  0053F053: ret         0Ch
  0053F056: mov         edx,dword ptr ds:[00D3B5B8h]
  0053F05C: push        edx
  0053F05D: call        0040FF50
  0053F062: jmp         0053EC22
  0053F067: cmp         ax,3Ah
  0053F06B: jne         0053F093
  0053F06D: mov         ecx,dword ptr ds:[00D3B5B8h]
  0053F073: xor         eax,eax
  0053F075: mov         al,dl
  0053F077: push        eax
  0053F078: push        ecx
  0053F079: call        00427A20
  0053F07E: mov         eax,dword ptr ds:[00D3B544h]
  0053F083: add         esp,8
  0053F086: pop         edi
  0053F087: pop         esi
  0053F088: pop         ebp
  0053F089: pop         ebx
  0053F08A: add         esp,10Ch
  0053F090: ret         0Ch
  0053F093: cmp         ax,1Ah
  0053F097: jne         0053F0BF
  0053F099: mov         ecx,dword ptr ds:[00D3B5B8h]
  0053F09F: xor         eax,eax
  0053F0A1: mov         al,dl
  0053F0A3: push        eax
  0053F0A4: push        ecx
  0053F0A5: call        0051F780
  0053F0AA: mov         eax,dword ptr ds:[00D3B544h]
  0053F0AF: add         esp,8
  0053F0B2: pop         edi
  0053F0B3: pop         esi
  0053F0B4: pop         ebp
  0053F0B5: pop         ebx
  0053F0B6: add         esp,10Ch
  0053F0BC: ret         0Ch
  0053F0BF: cmp         ax,0Ah
  0053F0C3: jne         0053F0EB
  0053F0C5: mov         ecx,dword ptr ds:[00D3B5B8h]
  0053F0CB: xor         eax,eax
  0053F0CD: mov         al,dl
  0053F0CF: push        eax
  0053F0D0: push        ecx
  0053F0D1: call        0051F780
  0053F0D6: mov         eax,dword ptr ds:[00D3B544h]
  0053F0DB: add         esp,8
  0053F0DE: pop         edi
  0053F0DF: pop         esi
  0053F0E0: pop         ebp
  0053F0E1: pop         ebx
  0053F0E2: add         esp,10Ch
  0053F0E8: ret         0Ch
  0053F0EB: mov         edx,dword ptr ds:[00D3B5B8h]
  0053F0F1: push        edx
  0053F0F2: call        0040FED0
  0053F0F7: jmp         0053EC22
  0053F0FC: mov         cl,byte ptr ds:[00D3B531h]
  0053F102: cmp         al,0Dh
  0053F104: jne         0053F11E
  0053F106: test        cl,cl
  0053F108: je          0053F1AF
  0053F10E: mov         eax,dword ptr ds:[00D3B5B8h]
  0053F113: push        eax
  0053F114: call        00411FD0
  0053F119: jmp         0053EC22
  0053F11E: cmp         al,0Ch
  0053F120: jne         0053F13B
  0053F122: test        cl,cl
  0053F124: je          0053F1AF
  0053F12A: mov         ecx,dword ptr ds:[00D3B5B8h]
  0053F130: push        ecx
  0053F131: call        004114A0
  0053F136: jmp         0053EC22
  0053F13B: cmp         al,0Ah
  0053F13D: jne         0053F154
  0053F13F: test        cl,cl
  0053F141: je          0053F1AF
  0053F143: mov         edx,dword ptr ds:[00D3B5B8h]
  0053F149: push        edx
  0053F14A: call        00410FB0
  0053F14F: jmp         0053EC22
  0053F154: cmp         al,2
  0053F156: jne         0053F16C
  0053F158: test        cl,cl
  0053F15A: je          0053F1AF
  0053F15C: mov         eax,dword ptr ds:[00D3B5B8h]
  0053F161: push        eax
  0053F162: call        00410F70
  0053F167: jmp         0053EC22
  0053F16C: cmp         al,14h
  0053F16E: jne         0053F1AF
  0053F170: mov         al,byte ptr ds:[00D3B531h]
  0053F175: cmp         al,1
  0053F177: jne         0053F18A
  0053F179: mov         ecx,dword ptr ds:[00D3B5B8h]
  0053F17F: push        ecx
  0053F180: call        00410DE0
  0053F185: jmp         0053EC22
  0053F18A: cmp         al,2
  0053F18C: jne         0053F19F
  0053F18E: mov         edx,dword ptr ds:[00D3B5B8h]
  0053F194: push        edx
  0053F195: call        00410E80
  0053F19A: jmp         0053EC22
  0053F19F: mov         eax,dword ptr ds:[00D3B5B8h]
  0053F1A4: push        eax
  0053F1A5: call        00410B10
  0053F1AA: jmp         0053EC22
  0053F1AF: movsx       eax,word ptr ds:[00D3B5BEh]
  0053F1B6: imul        eax,dword ptr ds:[00D3B538h]
  0053F1BD: mov         ebp,dword ptr ds:[00D3B54Ch]
  0053F1C3: mov         esi,dword ptr ds:[00D3B5B8h]
  0053F1C9: mov         bl,byte ptr [esp+13h]
  0053F1CD: add         eax,ebp
  0053F1CF: test        bl,bl
  0053F1D1: lea         eax,[esi+eax*2]
  0053F1D4: mov         dword ptr ds:[00D3B5B8h],eax
  0053F1D9: je          0053F1FF
  0053F1DB: cmp         cl,1
  0053F1DE: jne         0053F1EB
  0053F1E0: push        eax
  0053F1E1: call        0040FF50
  0053F1E6: jmp         0053EC22
  0053F1EB: cmp         cl,2
  0053F1EE: push        eax
  0053F1EF: jne         0053F0F2
  0053F1F5: call        0040FF50
  0053F1FA: jmp         0053EC22
  0053F1FF: cmp         cl,1
  0053F202: jne         0053F20F
  0053F204: push        eax
  0053F205: call        00410090
  0053F20A: jmp         0053EC22
  0053F20F: cmp         cl,2
  0053F212: jne         0053F21F
  0053F214: push        eax
  0053F215: call        00410090
  0053F21A: jmp         0053EC22
  0053F21F: test        dl,dl
  0053F221: push        eax
  0053F222: je          0053EC1D
  0053F228: call        0040FFD0
  0053F22D: jmp         0053EC22
  0053F232: mov         eax,dword ptr [esp+14h]
  0053F236: pop         edi
  0053F237: pop         esi
  0053F238: pop         ebp
  0053F239: pop         ebx
  0053F23A: add         esp,10Ch
  0053F240: ret         0Ch
  0053F243: nop

  Summary

      16E000 .text
