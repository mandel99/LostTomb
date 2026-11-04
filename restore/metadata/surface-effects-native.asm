
Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  004D05F0: mov         eax,dword ptr [esp+8]
  004D05F4: push        esi
  004D05F5: push        0Fh
  004D05F7: push        0
  004D05F9: mov         esi,ecx
  004D05FB: push        eax
  004D05FC: call        004D0630
  004D0601: mov         ecx,dword ptr [esp+1Ch]
  004D0605: mov         dl,byte ptr [esp+20h]
  004D0609: add         esp,0Ch
  004D060C: mov         dword ptr [esi+4],ecx
  004D060F: mov         ecx,dword ptr [esp+8]
  004D0613: mov         dword ptr [esi],eax
  004D0615: mov         eax,dword ptr [esp+18h]
  004D0619: push        ecx
  004D061A: mov         ecx,esi
  004D061C: mov         byte ptr [esi+18h],dl
  004D061F: mov         dword ptr [esi+14h],eax
  004D0622: call        004D0650
  004D0627: pop         esi
  004D0628: ret         14h
  004D062B: nop
  004D062C: nop
  004D062D: nop
  004D062E: nop
  004D062F: nop
  004D0630: mov         ecx,dword ptr [esp+4]
  004D0634: mov         eax,dword ptr [esp+8]
  004D0638: cmp         ecx,eax
  004D063A: jl          004D0646
  004D063C: mov         eax,dword ptr [esp+0Ch]
  004D0640: cmp         ecx,eax
  004D0642: jg          004D0646
  004D0644: mov         eax,ecx
  004D0646: ret
  004D0647: nop
  004D0648: nop
  004D0649: nop
  004D064A: nop
  004D064B: nop
  004D064C: nop
  004D064D: nop
  004D064E: nop
  004D064F: nop
  004D0650: sub         esp,14h
  004D0653: push        ebx
  004D0654: push        ebp
  004D0655: push        esi
  004D0656: push        edi
  004D0657: mov         edi,ecx
  004D0659: mov         dword ptr [esp+1Ch],edi
  004D065D: mov         eax,dword ptr [edi]
  004D065F: test        eax,eax
  004D0661: jle         004D0A27
  004D0667: cmp         dword ptr [edi+10h],0FFFFFFFFh
  004D066B: je          004D0A27
  004D0671: mov         ebx,dword ptr [edi+4]
  004D0674: dec         eax
  004D0675: mov         dword ptr [edi],eax
  004D0677: mov         al,byte ptr [edi+18h]
  004D067A: test        al,al
  004D067C: je          004D075B
  004D0682: mov         eax,dword ptr ds:[00D3B1C4h]
  004D0687: mov         ecx,dword ptr ds:[00D3B374h]
  004D068D: mov         dword ptr [eax*4+00D3B1C8h],ecx
  004D0694: inc         eax
  004D0695: cmp         eax,64h
  004D0698: mov         dword ptr ds:[00D3B1C4h],eax
  004D069D: jl          004D06A9
  004D069F: mov         dword ptr ds:[00D3B1C4h],0
  004D06A9: mov         eax,dword ptr ds:[00D3B384h]
  004D06AE: mov         edx,dword ptr ds:[00D3B380h]
  004D06B4: mov         ebp,1Fh
  004D06B9: mov         esi,eax
  004D06BB: mov         ecx,eax
  004D06BD: shr         esi,4
  004D06C0: and         esi,1
  004D06C3: and         ecx,1
  004D06C6: mov         edi,edx
  004D06C8: xor         esi,ecx
  004D06CA: shr         edi,4
  004D06CD: mov         ecx,edx
  004D06CF: and         edi,1
  004D06D2: and         ecx,1
  004D06D5: xor         edi,ecx
  004D06D7: shr         eax,1
  004D06D9: shr         edx,1
  004D06DB: test        esi,esi
  004D06DD: je          004D06E4
  004D06DF: or          eax,40000000h
  004D06E4: test        edi,edi
  004D06E6: je          004D06EE
  004D06E8: or          edx,40000000h
  004D06EE: dec         ebp
  004D06EF: jne         004D06B9
  004D06F1: mov         ecx,eax
  004D06F3: mov         esi,eax
  004D06F5: and         ecx,7Fh
  004D06F8: and         esi,7FFFh
  004D06FE: mov         dword ptr ds:[00D3B374h],ecx
  004D0704: mov         ecx,eax
  004D0706: and         ecx,7
  004D0709: mov         dword ptr ds:[00D3B370h],esi
  004D070F: mov         dword ptr ds:[00D3B378h],ecx
  004D0715: mov         ecx,edx
  004D0717: and         ecx,7FFFh
  004D071D: mov         dword ptr ds:[00D3B380h],edx
  004D0723: mov         dword ptr ds:[00D3B364h],ecx
  004D0729: mov         ecx,edx
  004D072B: and         ecx,7Fh
  004D072E: mov         dword ptr ds:[00D3B384h],eax
  004D0733: mov         dword ptr ds:[00D3B368h],ecx
  004D0739: mov         ecx,edx
  004D073B: and         ecx,7
  004D073E: and         esi,8000000Fh
  004D0744: mov         dword ptr ds:[00D3B36Ch],ecx
  004D074A: jns         004D0751
  004D074C: dec         esi
  004D074D: or          esi,0FFFFFFF0h
  004D0750: inc         esi
  004D0751: mov         edi,dword ptr [esp+1Ch]
  004D0755: lea         ebx,[ebx+esi-8]
  004D0759: jmp         004D0766
  004D075B: mov         eax,dword ptr ds:[00D3B384h]
  004D0760: mov         edx,dword ptr ds:[00D3B380h]
  004D0766: mov         ecx,dword ptr [esp+28h]
  004D076A: mov         ebp,dword ptr [edi+0Ch]
  004D076D: mov         dword ptr [esp+28h],ebp
  004D0771: mov         ebp,dword ptr ds:[00F53F78h]
  004D0777: mov         esi,dword ptr [edi+ecx*4+24h]
  004D077B: mov         ecx,dword ptr [edi+8]
  004D077E: test        ebp,ebp
  004D0780: mov         ebp,dword ptr ds:[00F53F64h]
  004D0786: mov         dword ptr [esp+14h],ecx
  004D078A: je          004D0792
  004D078C: mov         ebp,dword ptr ds:[00F53F60h]
  004D0792: mov         dword ptr [esp+10h],ebp
  004D0796: xor         ebp,ebp
  004D0798: cmp         dword ptr [edi+14h],ebp
  004D079B: jne         004D0891
  004D07A1: mov         eax,dword ptr [edi+10h]
  004D07A4: cmp         eax,1
  004D07A7: jne         004D0819
  004D07A9: mov         eax,dword ptr [esp+28h]
  004D07AD: test        eax,eax
  004D07AF: jle         004D0A27
  004D07B5: xor         edi,edi
  004D07B7: test        ecx,ecx
  004D07B9: jle         004D0806
  004D07BB: cmp         edi,dword ptr [esp+10h]
  004D07BF: jge         004D07FE
  004D07C1: cmp         ebp,1Eh
  004D07C4: jl          004D07FE
  004D07C6: xor         eax,eax
  004D07C8: push        1Fh
  004D07CA: mov         ax,word ptr [esi]
  004D07CD: push        0
  004D07CF: mov         edx,eax
  004D07D1: and         eax,1Fh
  004D07D4: shr         edx,0Bh
  004D07D7: add         edx,eax
  004D07D9: sar         edx,1
  004D07DB: add         edx,ebx
  004D07DD: push        edx
  004D07DE: call        004D0630
  004D07E3: mov         ecx,eax
  004D07E5: add         esp,0Ch
  004D07E8: shl         ecx,5
  004D07EB: or          ecx,eax
  004D07ED: shl         ecx,6
  004D07F0: or          ecx,eax
  004D07F2: mov         word ptr [esi],cx
  004D07F5: mov         ecx,dword ptr [esp+14h]
  004D07F9: add         esi,2
  004D07FC: jmp         004D0801
  004D07FE: add         esi,2
  004D0801: inc         edi
  004D0802: cmp         edi,ecx
  004D0804: jl          004D07BB
  004D0806: mov         eax,dword ptr [esp+28h]
  004D080A: inc         ebp
  004D080B: cmp         ebp,eax
  004D080D: jl          004D07B5
  004D080F: pop         edi
  004D0810: pop         esi
  004D0811: pop         ebp
  004D0812: pop         ebx
  004D0813: add         esp,14h
  004D0816: ret         4
  004D0819: cmp         eax,ebp
  004D081B: jne         004D0A27
  004D0821: mov         eax,dword ptr [esp+28h]
  004D0825: test        eax,eax
  004D0827: jle         004D0A27
  004D082D: xor         edi,edi
  004D082F: test        ecx,ecx
  004D0831: jle         004D087E
  004D0833: cmp         edi,dword ptr [esp+10h]
  004D0837: jge         004D0876
  004D0839: cmp         ebp,1Eh
  004D083C: jl          004D0876
  004D083E: xor         eax,eax
  004D0840: push        1Fh
  004D0842: mov         ax,word ptr [esi]
  004D0845: push        0
  004D0847: mov         edx,eax
  004D0849: and         eax,1Fh
  004D084C: shr         edx,0Bh
  004D084F: add         edx,eax
  004D0851: sar         edx,1
  004D0853: add         edx,ebx
  004D0855: push        edx
  004D0856: call        004D0630
  004D085B: mov         ecx,eax
  004D085D: add         esp,0Ch
  004D0860: shl         ecx,5
  004D0863: or          ecx,eax
  004D0865: shl         ecx,5
  004D0868: or          ecx,eax
  004D086A: mov         word ptr [esi],cx
  004D086D: mov         ecx,dword ptr [esp+14h]
  004D0871: add         esi,2
  004D0874: jmp         004D0879
  004D0876: add         esi,2
  004D0879: inc         edi
  004D087A: cmp         edi,ecx
  004D087C: jl          004D0833
  004D087E: mov         eax,dword ptr [esp+28h]
  004D0882: inc         ebp
  004D0883: cmp         ebp,eax
  004D0885: jl          004D082D
  004D0887: pop         edi
  004D0888: pop         esi
  004D0889: pop         ebp
  004D088A: pop         ebx
  004D088B: add         esp,14h
  004D088E: ret         4
  004D0891: mov         ecx,dword ptr [esp+28h]
  004D0895: mov         dword ptr [esp+18h],ebp
  004D0899: cmp         ecx,ebp
  004D089B: jle         004D0A27
  004D08A1: mov         ecx,dword ptr ds:[00D3B1C4h]
  004D08A7: mov         edi,dword ptr ds:[00D3B374h]
  004D08AD: mov         dword ptr [ecx*4+00D3B1C8h],edi
  004D08B4: inc         ecx
  004D08B5: cmp         ecx,64h
  004D08B8: mov         dword ptr ds:[00D3B1C4h],ecx
  004D08BE: jl          004D08CA
  004D08C0: mov         dword ptr ds:[00D3B1C4h],0
  004D08CA: mov         ebp,1Fh
  004D08CF: mov         edi,eax
  004D08D1: mov         ecx,eax
  004D08D3: shr         edi,4
  004D08D6: and         edi,1
  004D08D9: and         ecx,1
  004D08DC: mov         ebx,edx
  004D08DE: xor         edi,ecx
  004D08E0: shr         ebx,4
  004D08E3: mov         ecx,edx
  004D08E5: and         ebx,1
  004D08E8: and         ecx,1
  004D08EB: xor         ebx,ecx
  004D08ED: shr         eax,1
  004D08EF: shr         edx,1
  004D08F1: test        edi,edi
  004D08F3: je          004D08FA
  004D08F5: or          eax,40000000h
  004D08FA: test        ebx,ebx
  004D08FC: je          004D0904
  004D08FE: or          edx,40000000h
  004D0904: dec         ebp
  004D0905: jne         004D08CF
  004D0907: mov         ecx,eax
  004D0909: mov         ebp,dword ptr [esp+14h]
  004D090D: and         ecx,7Fh
  004D0910: mov         edi,eax
  004D0912: mov         dword ptr ds:[00D3B374h],ecx
  004D0918: mov         ecx,eax
  004D091A: and         ecx,7
  004D091D: and         edi,7FFFh
  004D0923: mov         dword ptr ds:[00D3B378h],ecx
  004D0929: mov         ecx,edx
  004D092B: and         ecx,7FFFh
  004D0931: xor         ebx,ebx
  004D0933: mov         dword ptr ds:[00D3B364h],ecx
  004D0939: mov         ecx,edx
  004D093B: and         ecx,7Fh
  004D093E: mov         dword ptr ds:[00D3B380h],edx
  004D0944: mov         dword ptr ds:[00D3B368h],ecx
  004D094A: mov         ecx,edx
  004D094C: and         ecx,7
  004D094F: mov         dword ptr ds:[00D3B384h],eax
  004D0954: test        ebp,ebp
  004D0956: mov         dword ptr ds:[00D3B370h],edi
  004D095C: mov         dword ptr ds:[00D3B36Ch],ecx
  004D0962: mov         dword ptr [esp+20h],edi
  004D0966: jle         004D0A12
  004D096C: mov         edi,dword ptr [esp+1Ch]
  004D0970: cmp         ebx,dword ptr [esp+10h]
  004D0974: jge         004D0A06
  004D097A: cmp         dword ptr [esp+18h],1Eh
  004D097F: jl          004D0A06
  004D0985: mov         edx,dword ptr [esp+20h]
  004D0989: lea         eax,[ebx+edx]
  004D098C: and         eax,8000007Fh
  004D0991: jns         004D0998
  004D0993: dec         eax
  004D0994: or          eax,0FFFFFF80h
  004D0997: inc         eax
  004D0998: jne         004D09AD
  004D099A: mov         word ptr [esi],0FFFFh
  004D099F: mov         eax,dword ptr [edi+10h]
  004D09A2: test        eax,eax
  004D09A4: jne         004D09FB
  004D09A6: mov         word ptr [esi],7FFFh
  004D09AB: jmp         004D09FB
  004D09AD: mov         eax,dword ptr [edi+10h]
  004D09B0: test        eax,eax
  004D09B2: mov         ax,word ptr [esi]
  004D09B5: jne         004D09C8
  004D09B7: mov         ecx,eax
  004D09B9: shr         ecx,1
  004D09BB: and         ecx,3DEFh
  004D09C1: and         eax,8000h
  004D09C6: jmp         004D09F6
  004D09C8: mov         dl,al
  004D09CA: and         eax,0FFE0h
  004D09CF: shr         dl,1
  004D09D1: and         edx,0Fh
  004D09D4: or          edx,eax
  004D09D6: mov         eax,edx
  004D09D8: and         edx,0F81Fh
  004D09DE: shr         eax,1
  004D09E0: and         eax,3E0h
  004D09E5: or          eax,edx
  004D09E7: mov         ecx,eax
  004D09E9: shr         ecx,1
  004D09EB: and         ecx,7800h
  004D09F1: and         eax,7FFh
  004D09F6: or          ecx,eax
  004D09F8: mov         word ptr [esi],cx
  004D09FB: mov         eax,dword ptr ds:[00D3B384h]
  004D0A00: mov         edx,dword ptr ds:[00D3B380h]
  004D0A06: add         esi,2
  004D0A09: inc         ebx
  004D0A0A: cmp         ebx,ebp
  004D0A0C: jl          004D0970
  004D0A12: mov         ecx,dword ptr [esp+18h]
  004D0A16: mov         edi,dword ptr [esp+28h]
  004D0A1A: inc         ecx
  004D0A1B: cmp         ecx,edi
  004D0A1D: mov         dword ptr [esp+18h],ecx
  004D0A21: jl          004D08A1
  004D0A27: pop         edi
  004D0A28: pop         esi
  004D0A29: pop         ebp
  004D0A2A: pop         ebx
  004D0A2B: add         esp,14h
  004D0A2E: ret         4
  004D0A31: nop
  004D0A32: nop
  004D0A33: nop
  004D0A34: nop
  004D0A35: nop
  004D0A36: nop
  004D0A37: nop
  004D0A38: nop
  004D0A39: nop
  004D0A3A: nop
  004D0A3B: nop
  004D0A3C: nop
  004D0A3D: nop
  004D0A3E: nop
  004D0A3F: nop
  004D0A40: push        ebx

  Summary

      16E000 .text
