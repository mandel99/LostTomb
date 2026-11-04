
Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  0053DFF0: mov         eax,dword ptr fs:[00000000h]
  0053DFF6: push        0FFFFFFFFh
  0053DFF8: push        56B7FBh
  0053DFFD: push        eax
  0053DFFE: mov         eax,0D8A48h
  0053E003: mov         dword ptr fs:[00000000h],esp
  0053E00A: call        00561F30
  0053E00F: push        ebx
  0053E010: push        ebp
  0053E011: mov         ebp,ecx
  0053E013: xor         ebx,ebx
  0053E015: mov         eax,dword ptr [ebp+0000CC2Ch]
  0053E01B: mov         dword ptr [esp+20h],eax
  0053E01F: mov         eax,dword ptr [ebp+0000CC24h]
  0053E025: cmp         eax,ebx
  0053E027: je          0053E038
  0053E029: push        eax
  0053E02A: call        00561A2B
  0053E02F: add         esp,4
  0053E032: mov         dword ptr [ebp+0000CC24h],ebx
  0053E038: mov         edx,dword ptr [esp+000D8A60h]
  0053E03F: push        esi
  0053E040: push        edi
  0053E041: or          ecx,0FFFFFFFFh
  0053E044: mov         edi,edx
  0053E046: xor         eax,eax
  0053E048: mov         byte ptr [ebp],0
  0053E04C: mov         dword ptr [ebp+0000CC28h],ebx
  0053E052: mov         dword ptr [ebp+0000CC2Ch],ebx
  0053E058: mov         dword ptr [ebp+0000CC30h],ebx
  0053E05E: repne scas  byte ptr es:[edi]
  0053E060: not         ecx
  0053E062: sub         edi,ecx
  0053E064: mov         eax,ecx
  0053E066: mov         esi,edi
  0053E068: shr         ecx,2
  0053E06B: mov         edi,ebp
  0053E06D: rep movs    dword ptr es:[edi],dword ptr [esi]
  0053E06F: mov         ecx,eax
  0053E071: xor         eax,eax
  0053E073: and         ecx,3
  0053E076: rep movs    byte ptr es:[edi],byte ptr [esi]
  0053E078: or          ecx,0FFFFFFFFh
  0053E07B: mov         edi,6018F8h
  0053E080: repne scas  byte ptr es:[edi]
  0053E082: not         ecx
  0053E084: sub         edi,ecx
  0053E086: lea         esi,[esp+2Ch]
  0053E08A: mov         eax,ecx
  0053E08C: mov         dword ptr [esp+14h],esi
  0053E090: mov         esi,edi
  0053E092: mov         edi,dword ptr [esp+14h]
  0053E096: shr         ecx,2
  0053E099: rep movs    dword ptr es:[edi],dword ptr [esi]
  0053E09B: mov         ecx,eax
  0053E09D: xor         eax,eax
  0053E09F: and         ecx,3
  0053E0A2: rep movs    byte ptr es:[edi],byte ptr [esi]
  0053E0A4: mov         edi,edx
  0053E0A6: or          ecx,0FFFFFFFFh
  0053E0A9: repne scas  byte ptr es:[edi]
  0053E0AB: lea         esi,[esp+2Ch]
  0053E0AF: not         ecx
  0053E0B1: mov         eax,esi
  0053E0B3: sub         edi,ecx
  0053E0B5: mov         esi,edi
  0053E0B7: mov         edx,ecx
  0053E0B9: mov         edi,eax
  0053E0BB: or          ecx,0FFFFFFFFh
  0053E0BE: xor         eax,eax
  0053E0C0: repne scas  byte ptr es:[edi]
  0053E0C2: mov         ecx,edx
  0053E0C4: dec         edi
  0053E0C5: shr         ecx,2
  0053E0C8: rep movs    dword ptr es:[edi],dword ptr [esi]
  0053E0CA: mov         ecx,edx
  0053E0CC: lea         edx,[esp+2Ch]
  0053E0D0: and         ecx,3
  0053E0D3: rep movs    byte ptr es:[edi],byte ptr [esi]
  0053E0D5: mov         edi,6018F0h
  0053E0DA: or          ecx,0FFFFFFFFh
  0053E0DD: repne scas  byte ptr es:[edi]
  0053E0DF: not         ecx
  0053E0E1: sub         edi,ecx
  0053E0E3: mov         esi,edi
  0053E0E5: mov         edi,edx
  0053E0E7: mov         edx,ecx
  0053E0E9: or          ecx,0FFFFFFFFh
  0053E0EC: repne scas  byte ptr es:[edi]
  0053E0EE: mov         ecx,edx
  0053E0F0: dec         edi
  0053E0F1: shr         ecx,2
  0053E0F4: rep movs    dword ptr es:[edi],dword ptr [esi]
  0053E0F6: mov         eax,dword ptr [esp+000D8A74h]
  0053E0FD: mov         ecx,edx
  0053E0FF: and         ecx,3
  0053E102: rep movs    byte ptr es:[edi],byte ptr [esi]
  0053E104: mov         ecx,dword ptr [esp+000D8A7Ch]
  0053E10B: mov         dword ptr [ebp+0000CC34h],eax
  0053E111: mov         dword ptr [ebp+0000CC38h],ecx
  0053E117: lea         edx,[esp+00000130h]
  0053E11E: push        0
  0053E120: lea         eax,[esp+30h]
  0053E124: push        edx
  0053E125: push        eax
  0053E126: mov         dword ptr [esp+2Ch],ebx
  0053E12A: mov         dword ptr [esp+20h],0
  0053E132: call        004CEB60
  0053E137: add         esp,0Ch
  0053E13A: test        eax,eax
  0053E13C: setne       al
  0053E13F: cmp         al,1
  0053E141: jne         0053E394
  0053E147: mov         eax,dword ptr [esp+000D8A6Ch]
  0053E14E: test        eax,eax
  0053E150: je          0053E19F
  0053E152: mov         esi,6018E4h
  0053E157: lea         eax,[esp+000003D8h]
  0053E15E: mov         dl,byte ptr [eax]
  0053E160: mov         cl,dl
  0053E162: cmp         dl,byte ptr [esi]
  0053E164: jne         0053E182
  0053E166: test        cl,cl
  0053E168: je          0053E17E
  0053E16A: mov         dl,byte ptr [eax+1]
  0053E16D: mov         cl,dl
  0053E16F: cmp         dl,byte ptr [esi+1]
  0053E172: jne         0053E182
  0053E174: add         eax,2
  0053E177: add         esi,2
  0053E17A: test        cl,cl
  0053E17C: jne         0053E15E
  0053E17E: xor         eax,eax
  0053E180: jmp         0053E187
  0053E182: sbb         eax,eax
  0053E184: sbb         eax,0FFFFFFFFh
  0053E187: test        eax,eax
  0053E189: jne         0053E19F
  0053E18B: mov         dword ptr [esp+20h],0C8h
  0053E193: mov         ebx,dword ptr [esp+20h]
  0053E197: mov         dword ptr [esp+14h],992Eh
  0053E19F: mov         ecx,dword ptr [esp+00000140h]
  0053E1A6: mov         edi,dword ptr [esp+000D8A70h]
  0053E1AD: sub         ecx,ebx
  0053E1AF: mov         eax,ebx
  0053E1B1: mov         dword ptr [ebp+0000CC2Ch],ecx
  0053E1B7: shl         ecx,6
  0053E1BA: shl         eax,6
  0053E1BD: mov         edx,ecx
  0053E1BF: lea         esi,[esp+eax+0000A058h]
  0053E1C6: xor         eax,eax
  0053E1C8: shr         ecx,2
  0053E1CB: rep movs    dword ptr es:[edi],dword ptr [esi]
  0053E1CD: mov         ecx,edx
  0053E1CF: mov         dword ptr [esp+18h],eax
  0053E1D3: and         ecx,3
  0053E1D6: mov         dword ptr [esp+1Ch],eax
  0053E1DA: rep movs    byte ptr es:[edi],byte ptr [esi]
  0053E1DC: lea         ecx,[esp+00000180h]
  0053E1E3: mov         dword ptr [esp+000D8A60h],eax
  0053E1EA: xor         edi,edi
  0053E1EC: mov         dword ptr [esp+10h],ecx
  0053E1F0: mov         edx,dword ptr [esp+10h]
  0053E1F4: movsx       esi,word ptr [edx]
  0053E1F7: sub         esi,ebx
  0053E1F9: test        esi,esi
  0053E1FB: jle         0053E29C
  0053E201: push        8
  0053E203: call        00561A1D
  0053E208: xor         ebx,ebx
  0053E20A: add         esp,4
  0053E20D: cmp         eax,ebx
  0053E20F: je          0053E218
  0053E211: mov         dword ptr [eax+4],ebx
  0053E214: mov         dword ptr [eax],esi
  0053E216: mov         ebx,eax
  0053E218: push        10h
  0053E21A: call        00561A1D
  0053E21F: add         esp,4
  0053E222: test        eax,eax
  0053E224: je          0053E23D
  0053E226: mov         dword ptr [eax+4],0
  0053E22D: mov         dword ptr [eax],0
  0053E233: mov         dword ptr [eax+0Ch],ebx
  0053E236: mov         dword ptr [eax+8],edi
  0053E239: mov         esi,eax
  0053E23B: jmp         0053E23F
  0053E23D: xor         esi,esi
  0053E23F: mov         eax,dword ptr [esp+18h]
  0053E243: test        eax,eax
  0053E245: je          0053E27A
  0053E247: cmp         dword ptr [eax+8],edi
  0053E24A: jge         0053E27A
  0053E24C: mov         ecx,dword ptr [esp+1Ch]
  0053E250: cmp         dword ptr [ecx+8],edi
  0053E253: jg          0053E261
  0053E255: push        esi
  0053E256: lea         ecx,[esp+1Ch]
  0053E25A: call        005404B0
  0053E25F: jmp         0053E284
  0053E261: push        edi
  0053E262: push        eax
  0053E263: push        esi
  0053E264: lea         ecx,[esp+24h]
  0053E268: call        00540570
  0053E26D: push        eax
  0053E26E: push        esi
  0053E26F: lea         ecx,[esp+20h]
  0053E273: call        00540590
  0053E278: jmp         0053E284
  0053E27A: push        esi
  0053E27B: lea         ecx,[esp+1Ch]
  0053E27F: call        00540540
  0053E284: mov         dword ptr [ebx+4],esi
  0053E287: mov         eax,dword ptr [ebp+0000CC30h]
  0053E28D: mov         ebx,dword ptr [esp+20h]
  0053E291: inc         eax
  0053E292: mov         dword ptr [ebp+0000CC30h],eax
  0053E298: mov         eax,dword ptr [esp+18h]
  0053E29C: mov         edx,dword ptr [esp+10h]
  0053E2A0: inc         edi
  0053E2A1: add         edx,2
  0053E2A4: cmp         edi,12Ch
  0053E2AA: mov         dword ptr [esp+10h],edx
  0053E2AE: jl          0053E1F0
  0053E2B4: mov         ecx,eax
  0053E2B6: test        eax,eax
  0053E2B8: mov         dword ptr [esp+24h],ecx
  0053E2BC: jne         0053E2C2
  0053E2BE: xor         esi,esi
  0053E2C0: jmp         0053E2CD
  0053E2C2: mov         edx,dword ptr [eax]
  0053E2C4: mov         esi,dword ptr [eax+0Ch]
  0053E2C7: mov         dword ptr [esp+24h],edx
  0053E2CB: mov         ecx,edx
  0053E2CD: xor         ebx,ebx
  0053E2CF: test        esi,esi
  0053E2D1: je          0053E389
  0053E2D7: mov         edi,dword ptr [esp+000D8A78h]
  0053E2DE: jmp         0053E2E4
  0053E2E0: mov         ecx,dword ptr [esp+24h]
  0053E2E4: test        ecx,ecx
  0053E2E6: jne         0053E2EE
  0053E2E8: mov         dword ptr [esp+10h],ecx
  0053E2EC: jmp         0053E2FB
  0053E2EE: mov         eax,dword ptr [ecx+0Ch]
  0053E2F1: mov         ecx,dword ptr [ecx]
  0053E2F3: mov         dword ptr [esp+24h],ecx
  0053E2F7: mov         dword ptr [esp+10h],eax
  0053E2FB: mov         edx,dword ptr [esp+000D8A6Ch]
  0053E302: mov         ecx,dword ptr [esp+000D8A74h]
  0053E309: mov         dword ptr [edi],edx
  0053E30B: mov         eax,dword ptr [esi]
  0053E30D: add         edi,8
  0053E310: lea         edx,[eax+ecx-1]
  0053E314: mov         eax,dword ptr [esp+000D8A7Ch]
  0053E31B: mov         dword ptr [edi-4],edx
  0053E31E: mov         ecx,dword ptr [esi]
  0053E320: mov         edx,dword ptr [esp+000D8A70h]
  0053E327: add         eax,ebx
  0053E329: shl         ecx,6
  0053E32C: inc         ebx
  0053E32D: mov         word ptr [ecx+edx-24h],ax
  0053E332: mov         eax,dword ptr [esi+4]
  0053E335: mov         ecx,dword ptr [eax+4]
  0053E338: mov         edx,dword ptr [eax]
  0053E33A: test        ecx,ecx
  0053E33C: je          0053E342
  0053E33E: mov         dword ptr [ecx],edx
  0053E340: jmp         0053E346
  0053E342: mov         dword ptr [esp+18h],edx
  0053E346: test        edx,edx
  0053E348: je          0053E34F
  0053E34A: mov         dword ptr [edx+4],ecx
  0053E34D: jmp         0053E353
  0053E34F: mov         dword ptr [esp+1Ch],ecx
  0053E353: push        eax
  0053E354: mov         dword ptr [eax+4],0
  0053E35B: mov         dword ptr [eax],0
  0053E361: call        00561A2B
  0053E366: add         esp,4
  0053E369: mov         dword ptr [esi+4],0
  0053E370: test        esi,esi
  0053E372: je          0053E37D
  0053E374: push        esi
  0053E375: call        00561A2B
  0053E37A: add         esp,4
  0053E37D: mov         esi,dword ptr [esp+10h]
  0053E381: test        esi,esi
  0053E383: jne         0053E2E0
  0053E389: mov         dword ptr [esp+000D8A60h],0FFFFFFFFh
  0053E394: mov         eax,dword ptr [esp+00000144h]
  0053E39B: test        eax,eax
  0053E39D: jle         0053E3E6
  0053E39F: lea         ebx,[esp+000003D8h]
  0053E3A6: lea         edx,[ebp+00000104h]
  0053E3AC: mov         dword ptr [esp+10h],eax
  0053E3B0: mov         edi,ebx
  0053E3B2: or          ecx,0FFFFFFFFh
  0053E3B5: xor         eax,eax
  0053E3B7: add         ebx,0C8h
  0053E3BD: repne scas  byte ptr es:[edi]
  0053E3BF: not         ecx
  0053E3C1: sub         edi,ecx
  0053E3C3: mov         eax,ecx
  0053E3C5: mov         esi,edi
  0053E3C7: mov         edi,edx
  0053E3C9: add         edx,104h
  0053E3CF: shr         ecx,2
  0053E3D2: rep movs    dword ptr es:[edi],dword ptr [esi]
  0053E3D4: mov         ecx,eax
  0053E3D6: mov         eax,dword ptr [esp+10h]
  0053E3DA: and         ecx,3
  0053E3DD: dec         eax
  0053E3DE: rep movs    byte ptr es:[edi],byte ptr [esi]
  0053E3E0: mov         dword ptr [esp+10h],eax
  0053E3E4: jne         0053E3B0
  0053E3E6: mov         ecx,dword ptr [esp+14h]
  0053E3EA: mov         edx,dword ptr [esp+000D8A68h]
  0053E3F1: push        ecx
  0053E3F2: push        0
  0053E3F4: push        0
  0053E3F6: push        0
  0053E3F8: lea         esi,[ebp+0000CC24h]
  0053E3FE: push        0
  0053E400: push        esi
  0053E401: push        edx
  0053E402: call        0053F660
  0053E407: add         esp,1Ch
  0053E40A: mov         dword ptr [ebp+0000CC28h],eax
  0053E410: test        eax,eax
  0053E412: setne       bl
  0053E415: cmp         bl,1
  0053E418: jne         0053E4F4
  0053E41E: mov         eax,dword ptr [esi]
  0053E420: xor         edi,edi
  0053E422: add         eax,4
  0053E425: mov         dword ptr ds:[00D3B5F4h],eax
  0053E42A: mov         eax,dword ptr [ebp+0000CC2Ch]
  0053E430: test        eax,eax
  0053E432: jle         0053E4F4
  0053E438: mov         ecx,dword ptr [esp+000D8A70h]
  0053E43F: lea         esi,[ecx+8]
  0053E442: mov         al,byte ptr [esi+2Ch]
  0053E445: mov         edx,dword ptr [esp+000D8A6Ch]
  0053E44C: test        al,al
  0053E44E: mov         dword ptr [esi+4],edx
  0053E451: je          0053E467
  0053E453: mov         eax,dword ptr [esi-8]
  0053E456: test        eax,eax
  0053E458: jne         0053E4E2
  0053E45E: mov         dword ptr [esi-8],1
  0053E465: jmp         0053E4E2
  0053E467: mov         eax,dword ptr ds:[00D3B5F4h]
  0053E46C: mov         ecx,dword ptr [esi-4]
  0053E46F: mov         dword ptr [esi-8],eax
  0053E472: mov         edx,dword ptr ds:[00D3B5F4h]
  0053E478: add         edx,ecx
  0053E47A: mov         dword ptr ds:[00D3B5F4h],edx
  0053E480: mov         eax,dword ptr [esi-4]
  0053E483: test        eax,eax
  0053E485: jle         0053E4E2
  0053E487: cmp         byte ptr [esi+2Ah],1Eh
  0053E48B: jne         0053E4BF
  0053E48D: mov         cl,byte ptr [esi+2Dh]
  0053E490: test        cl,cl
  0053E492: je          0053E4B8
  0053E494: mov         edx,dword ptr [esi]
  0053E496: mov         eax,dword ptr [esi-8]
  0053E499: push        edx
  0053E49A: push        eax
  0053E49B: call        004CDF20
  0053E4A0: mov         eax,dword ptr [esi]
  0053E4A2: mov         ecx,dword ptr [esi-4]
  0053E4A5: mov         edx,dword ptr [esi-8]
  0053E4A8: sub         ecx,eax
  0053E4AA: add         eax,edx
  0053E4AC: push        ecx
  0053E4AD: push        eax
  0053E4AE: call        004CDEB0
  0053E4B3: add         esp,10h
  0053E4B6: jmp         0053E4E2
  0053E4B8: mov         edx,dword ptr [esi-8]
  0053E4BB: push        eax
  0053E4BC: push        edx
  0053E4BD: jmp         0053E4DA
  0053E4BF: mov         cl,byte ptr [esi+2Bh]
  0053E4C2: mov         byte ptr [esi+2Dh],0
  0053E4C6: test        cl,cl
  0053E4C8: push        eax
  0053E4C9: je          0053E4D6
  0053E4CB: mov         eax,dword ptr [esi-8]
  0053E4CE: push        eax
  0053E4CF: call        004CDEB0
  0053E4D4: jmp         0053E4DF
  0053E4D6: mov         ecx,dword ptr [esi-8]
  0053E4D9: push        ecx
  0053E4DA: call        004CDF20
  0053E4DF: add         esp,8
  0053E4E2: mov         eax,dword ptr [ebp+0000CC2Ch]
  0053E4E8: inc         edi
  0053E4E9: add         esi,40h
  0053E4EC: cmp         edi,eax
  0053E4EE: jl          0053E442
  0053E4F4: mov         eax,dword ptr [ebp+0000CC2Ch]
  0053E4FA: mov         ecx,dword ptr [esp+28h]
  0053E4FE: pop         edi
  0053E4FF: cmp         eax,ecx
  0053E501: pop         esi
  0053E502: jle         0053E50C
  0053E504: mov         dword ptr [ebp+0000CC2Ch],eax
  0053E50A: jmp         0053E512
  0053E50C: mov         dword ptr [ebp+0000CC2Ch],ecx
  0053E512: mov         ecx,dword ptr [esp+000D8A50h]
  0053E519: mov         al,bl
  0053E51B: pop         ebp
  0053E51C: pop         ebx
  0053E51D: mov         dword ptr fs:[00000000h],ecx
  0053E524: add         esp,0D8A54h
  0053E52A: ret         18h
  0053E52D: nop
  0053E52E: nop
  0053E52F: nop
  0053E530: mov         edx,dword ptr [esp+4]
  0053E534: push        ebx
  0053E535: push        esi
  0053E536: mov         esi,dword ptr [esp+14h]
  0053E53A: push        edi
  0053E53B: mov         edi,dword ptr [esp+14h]
  0053E53F: mov         dword ptr [esi],0
  0053E545: mov         eax,edx
  0053E547: mov         dword ptr [edi],0
  0053E54D: mov         ebx,dword ptr [ecx+0030F5CCh]
  0053E553: cmp         edx,ebx
  0053E555: jl          0053E559
  0053E557: xor         eax,eax
  0053E559: shl         eax,6
  0053E55C: test        edx,edx
  0053E55E: lea         eax,[eax+ecx+00180E48h]
  0053E565: jle         0053E587
  0053E567: cmp         edx,639Ch
  0053E56D: jge         0053E587
  0053E56F: test        eax,eax
  0053E571: je          0053E587
  0053E573: movsx       ecx,word ptr [eax+14h]
  0053E577: mov         dword ptr [edi],ecx
  0053E579: pop         edi
  0053E57A: movsx       edx,word ptr [eax+16h]
  0053E57E: mov         dword ptr [esi],edx
  0053E580: pop         esi
  0053E581: mov         al,1
  0053E583: pop         ebx
  0053E584: ret         0Ch
  0053E587: pop         edi
  0053E588: pop         esi
  0053E589: xor         al,al
  0053E58B: pop         ebx
  0053E58C: ret         0Ch
  0053E58F: nop
  0053E590: 81

  Summary

      16E000 .text
