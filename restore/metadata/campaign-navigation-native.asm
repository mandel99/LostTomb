
Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  00418220: mov         al,byte ptr ds:[00E38DDAh]
  00418225: push        ebx
  00418226: xor         ebx,ebx
  00418228: cmp         al,36h
  0041822A: mov         dword ptr ds:[007056C4h],ebx
  00418230: mov         dword ptr ds:[007056C8h],ebx
  00418236: mov         dword ptr ds:[007056B8h],ebx
  0041823C: mov         dword ptr ds:[007056C0h],ebx
  00418242: mov         dword ptr ds:[00E94E10h],ebx
  00418248: jne         00418258
  0041824A: call        00516C80
  0041824F: mov         byte ptr ds:[00E38DCAh],1
  00418256: pop         ebx
  00418257: ret
  00418258: cmp         al,34h
  0041825A: jne         004182EB
  00418260: cmp         byte ptr ds:[007027A4h],bl
  00418266: je          00418342
  0041826C: call        004188F0
  00418271: mov         ecx,dword ptr ds:[007027A4h]
  00418277: mov         al,byte ptr ds:[00E38EABh]
  0041827C: and         ecx,0FFh
  00418282: mov         byte ptr ds:[00E38EAAh],al
  00418287: movsx       edx,al
  0041828A: movsx       eax,byte ptr ds:[00E38EA9h]
  00418291: dec         ecx
  00418292: push        ecx
  00418293: push        edx
  00418294: push        eax
  00418295: mov         ecx,6FDD08h
  0041829A: call        0041B920
  0041829F: mov         byte ptr ds:[00E38EABh],al
  004182A4: movsx       eax,al
  004182A7: cmp         eax,ebx
  004182A9: jl          004182D2
  004182AB: cmp         eax,dword ptr ds:[006FF608h]
  004182B1: jge         004182D2
  004182B3: shl         eax,6
  004182B6: add         eax,6FDD08h
  004182BB: mov         cl,byte ptr [eax+0Ch]
  004182BE: mov         byte ptr ds:[00E38EA8h],cl
  004182C4: call        004181E0
  004182C9: mov         byte ptr ds:[00E38DCAh],1
  004182D0: pop         ebx
  004182D1: ret
  004182D2: xor         eax,eax
  004182D4: mov         cl,byte ptr [eax+0Ch]
  004182D7: mov         byte ptr ds:[00E38EA8h],cl
  004182DD: call        004181E0
  004182E2: mov         byte ptr ds:[00E38DCAh],1
  004182E9: pop         ebx
  004182EA: ret
  004182EB: cmp         al,35h
  004182ED: jne         00418301
  004182EF: mov         edx,dword ptr ds:[00EAAC28h]
  004182F5: imul        edx,edx,49D8h
  004182FB: mov         dword ptr [edx+00EA5F8Ch],ebx
  00418301: push        1
  00418303: mov         byte ptr ds:[00E38DDAh],1
  0041830A: mov         byte ptr ds:[00E38DD6h],2
  00418311: call        0053B690
  00418316: mov         eax,dword ptr ds:[00EAAC28h]
  0041831B: mov         ecx,dword ptr ds:[00E39080h]
  00418321: imul        eax,eax,49D8h
  00418327: mov         edx,dword ptr [eax+00EA6018h]
  0041832D: add         esp,4
  00418330: lea         eax,[eax+00EA6018h]
  00418336: cmp         ecx,edx
  00418338: mov         dword ptr ds:[00F55B94h],ebx
  0041833E: jge         00418342
  00418340: mov         dword ptr [eax],ecx
  00418342: mov         byte ptr ds:[00E38DCAh],1
  00418349: pop         ebx
  0041834A: ret
  0041834B: nop
  0041834C: nop
  0041834D: nop
  0041834E: nop
  0041834F: nop
  00418350: cmp         byte ptr ds:[00E38DDAh],35h
  00418357: jne         0041837F
  00418359: mov         al,byte ptr ds:[00E38EAAh]
  0041835E: test        al,al
  00418360: jl          0041837F
  00418362: push        1
  00418364: mov         byte ptr ds:[00E38DDAh],34h
  0041836B: mov         byte ptr ds:[00E38EABh],al
  00418370: mov         byte ptr ds:[00E38EAAh],0FFh
  00418377: call        0051F780
  0041837C: add         esp,4
  0041837F: mov         byte ptr ds:[00E38DCAh],1
  00418386: ret
  00418387: nop
  00418388: nop
  00418389: nop
  0041838A: nop
  0041838B: nop
  0041838C: nop
  0041838D: nop
  0041838E: nop
  0041838F: nop
  00418390: mov         eax,dword ptr ds:[00E38EBCh]
  00418395: push        esi
  00418396: xor         esi,esi
  00418398: cmp         eax,esi
  0041839A: jne         004183CB
  0041839C: push        3E8h
  004183A1: push        3Bh
  004183A3: push        esi
  004183A4: call        0041AA20
  004183A9: add         esp,0Ch
  004183AC: mov         dword ptr ds:[00D3B130h],esi
  004183B2: mov         dword ptr ds:[00D3B138h],esi
  004183B8: mov         dword ptr ds:[00D3A6E4h],esi
  004183BE: mov         dword ptr ds:[00D3B128h],esi
  004183C4: mov         byte ptr ds:[00E38DCAh],1
  004183CB: pop         esi
  004183CC: ret
  004183CD: nop
  004183CE: nop
  004183CF: nop
  004183D0: mov         eax,dword ptr ds:[00E38EBCh]
  004183D5: sub         esp,64h
  004183D8: test        eax,eax
  004183DA: push        ebx
  004183DB: push        esi
  004183DC: push        edi
  004183DD: jne         004184A3
  004183E3: cmp         byte ptr ds:[007027B4h],1
  004183EA: jne         00418491
  004183F0: call        005169C0
  004183F5: mov         edi,eax
  004183F7: or          ecx,0FFFFFFFFh
  004183FA: xor         eax,eax
  004183FC: lea         edx,[esp+0Ch]
  00418400: repne scas  byte ptr es:[edi]
  00418402: not         ecx
  00418404: sub         edi,ecx
  00418406: push        5763BCh
  0041840B: mov         eax,ecx
  0041840D: mov         esi,edi
  0041840F: mov         edi,edx
  00418411: lea         edx,[esp+10h]
  00418415: shr         ecx,2
  00418418: rep movs    dword ptr es:[edi],dword ptr [esi]
  0041841A: mov         ecx,eax
  0041841C: xor         eax,eax
  0041841E: and         ecx,3
  00418421: rep movs    byte ptr es:[edi],byte ptr [esi]
  00418423: mov         edi,5761D0h
  00418428: or          ecx,0FFFFFFFFh
  0041842B: repne scas  byte ptr es:[edi]
  0041842D: not         ecx
  0041842F: sub         edi,ecx
  00418431: mov         esi,edi
  00418433: mov         ebx,ecx
  00418435: mov         edi,edx
  00418437: or          ecx,0FFFFFFFFh
  0041843A: repne scas  byte ptr es:[edi]
  0041843C: mov         ecx,ebx
  0041843E: dec         edi
  0041843F: shr         ecx,2
  00418442: rep movs    dword ptr es:[edi],dword ptr [esi]
  00418444: mov         ecx,ebx
  00418446: lea         edx,[esp+10h]
  0041844A: and         ecx,3
  0041844D: rep movs    byte ptr es:[edi],byte ptr [esi]
  0041844F: mov         edi,57BFC4h
  00418454: or          ecx,0FFFFFFFFh
  00418457: repne scas  byte ptr es:[edi]
  00418459: not         ecx
  0041845B: sub         edi,ecx
  0041845D: mov         esi,edi
  0041845F: mov         ebx,ecx
  00418461: mov         edi,edx
  00418463: or          ecx,0FFFFFFFFh
  00418466: repne scas  byte ptr es:[edi]
  00418468: mov         ecx,ebx
  0041846A: dec         edi
  0041846B: shr         ecx,2
  0041846E: rep movs    dword ptr es:[edi],dword ptr [esi]
  00418470: mov         ecx,ebx
  00418472: lea         eax,[esp+10h]
  00418476: and         ecx,3
  00418479: push        eax
  0041847A: rep movs    byte ptr es:[edi],byte ptr [esi]
  0041847C: call        00561A0A
  00418481: add         esp,8
  00418484: test        eax,eax
  00418486: je          0041849A
  00418488: push        eax
  00418489: call        00561994
  0041848E: add         esp,4
  00418491: pop         edi
  00418492: pop         esi
  00418493: mov         al,1
  00418495: pop         ebx
  00418496: add         esp,64h
  00418499: ret
  0041849A: pop         edi
  0041849B: pop         esi
  0041849C: xor         al,al
  0041849E: pop         ebx
  0041849F: add         esp,64h
  004184A2: ret
  004184A3: mov         edi,57626Ch
  004184A8: or          ecx,0FFFFFFFFh
  004184AB: xor         eax,eax
  004184AD: lea         edx,[esp+0Ch]
  004184B1: repne scas  byte ptr es:[edi]
  004184B3: not         ecx
  004184B5: sub         edi,ecx
  004184B7: mov         eax,ecx
  004184B9: mov         esi,edi
  004184BB: mov         edi,edx
  004184BD: lea         edx,[esp+0Ch]
  004184C1: shr         ecx,2
  004184C4: rep movs    dword ptr es:[edi],dword ptr [esi]
  004184C6: mov         ecx,eax
  004184C8: xor         eax,eax
  004184CA: and         ecx,3
  004184CD: rep movs    byte ptr es:[edi],byte ptr [esi]
  004184CF: mov         edi,0E37078h
  004184D4: or          ecx,0FFFFFFFFh
  004184D7: repne scas  byte ptr es:[edi]
  004184D9: not         ecx
  004184DB: sub         edi,ecx
  004184DD: mov         esi,edi
  004184DF: mov         ebx,ecx
  004184E1: mov         edi,edx
  004184E3: or          ecx,0FFFFFFFFh
  004184E6: repne scas  byte ptr es:[edi]
  004184E8: mov         ecx,ebx
  004184EA: dec         edi
  004184EB: shr         ecx,2
  004184EE: rep movs    dword ptr es:[edi],dword ptr [esi]
  004184F0: mov         ecx,ebx
  004184F2: lea         eax,[esp+0Ch]
  004184F6: and         ecx,3
  004184F9: push        eax
  004184FA: rep movs    byte ptr es:[edi],byte ptr [esi]
  004184FC: call        005409D0
  00418501: add         esp,4
  00418504: test        eax,eax
  00418506: pop         edi
  00418507: pop         esi
  00418508: setne       al
  0041850B: pop         ebx
  0041850C: add         esp,64h
  0041850F: ret
  00418510: mov         eax,dword ptr ds:[00E38EBCh]
  00418515: sub         esp,64h
  00418518: push        ebx
  00418519: xor         ebx,ebx
  0041851B: cmp         eax,ebx
  0041851D: mov         dword ptr ds:[006059E8h],ebx
  00418523: jne         0041860C
  00418529: cmp         byte ptr ds:[007027B4h],1
  00418530: jne         004185FC
  00418536: push        ebp
  00418537: push        esi
  00418538: push        edi
  00418539: call        005169C0
  0041853E: mov         edi,eax
  00418540: or          ecx,0FFFFFFFFh
  00418543: xor         eax,eax
  00418545: lea         edx,[esp+10h]
  00418549: repne scas  byte ptr es:[edi]
  0041854B: not         ecx
  0041854D: sub         edi,ecx
  0041854F: push        5763BCh
  00418554: mov         eax,ecx
  00418556: mov         esi,edi
  00418558: mov         edi,edx
  0041855A: lea         edx,[esp+14h]
  0041855E: shr         ecx,2
  00418561: rep movs    dword ptr es:[edi],dword ptr [esi]
  00418563: mov         ecx,eax
  00418565: xor         eax,eax
  00418567: and         ecx,3
  0041856A: rep movs    byte ptr es:[edi],byte ptr [esi]
  0041856C: mov         edi,5761D0h
  00418571: or          ecx,0FFFFFFFFh
  00418574: repne scas  byte ptr es:[edi]
  00418576: not         ecx
  00418578: sub         edi,ecx
  0041857A: mov         esi,edi
  0041857C: mov         ebp,ecx
  0041857E: mov         edi,edx
  00418580: or          ecx,0FFFFFFFFh
  00418583: repne scas  byte ptr es:[edi]
  00418585: mov         ecx,ebp
  00418587: dec         edi
  00418588: shr         ecx,2
  0041858B: rep movs    dword ptr es:[edi],dword ptr [esi]
  0041858D: mov         ecx,ebp
  0041858F: lea         edx,[esp+14h]
  00418593: and         ecx,3
  00418596: rep movs    byte ptr es:[edi],byte ptr [esi]
  00418598: mov         edi,57BFC4h
  0041859D: or          ecx,0FFFFFFFFh
  004185A0: repne scas  byte ptr es:[edi]
  004185A2: not         ecx
  004185A4: sub         edi,ecx
  004185A6: mov         esi,edi
  004185A8: mov         ebp,ecx
  004185AA: mov         edi,edx
  004185AC: or          ecx,0FFFFFFFFh
  004185AF: repne scas  byte ptr es:[edi]
  004185B1: mov         ecx,ebp
  004185B3: dec         edi
  004185B4: shr         ecx,2
  004185B7: rep movs    dword ptr es:[edi],dword ptr [esi]
  004185B9: mov         ecx,ebp
  004185BB: lea         eax,[esp+14h]
  004185BF: and         ecx,3
  004185C2: push        eax
  004185C3: rep movs    byte ptr es:[edi],byte ptr [esi]
  004185C5: call        00561A0A
  004185CA: add         esp,8
  004185CD: cmp         eax,ebx
  004185CF: pop         edi
  004185D0: pop         esi
  004185D1: pop         ebp
  004185D2: je          004185F2
  004185D4: push        eax
  004185D5: call        00561994
  004185DA: lea         ecx,[esp+8]
  004185DE: mov         dword ptr ds:[00605A3Ch],ebx
  004185E4: push        ecx
  004185E5: mov         byte ptr ds:[006DE515h],bl
  004185EB: call        004D51B0
  004185F0: jmp         0041861D
  004185F2: call        004CB6A0
  004185F7: pop         ebx
  004185F8: add         esp,64h
  004185FB: ret
  004185FC: mov         byte ptr ds:[006FDCB4h],bl
  00418602: call        004181E0
  00418607: pop         ebx
  00418608: add         esp,64h
  0041860B: ret
  0041860C: push        ebx
  0041860D: push        0E37078h
  00418612: mov         byte ptr ds:[007027B4h],bl
  00418618: call        005405B0
  0041861D: add         esp,8
  00418620: mov         byte ptr ds:[00E38DDAh],1
  00418627: mov         byte ptr ds:[00E38DCAh],1
  0041862E: pop         ebx
  0041862F: add         esp,64h
  00418632: ret
  00418633: nop
  00418634: nop
  00418635: nop
  00418636: nop
  00418637: nop
  00418638: nop
  00418639: nop
  0041863A: nop
  0041863B: nop
  0041863C: nop
  0041863D: nop
  0041863E: nop
  0041863F: nop
  00418640: push        ecx
  00418641: mov         eax,dword ptr ds:[00EAAC28h]
  00418646: push        ebx
  00418647: imul        eax,eax,49D8h
  0041864D: push        esi
  0041864E: xor         esi,esi
  00418650: mov         ecx,dword ptr [eax+00EA5B48h]
  00418656: mov         edx,dword ptr [eax+00EA5CC0h]
  0041865C: mov         eax,dword ptr ds:[00E38EBCh]
  00418661: mov         dword ptr ds:[00E38EB0h],ecx
  00418667: mov         ecx,dword ptr ds:[00E38EB8h]
  0041866D: or          ebx,0FFFFFFFFh
  00418670: inc         ecx
  00418671: cmp         eax,esi
  00418673: mov         dword ptr ds:[00E38EB4h],edx
  00418679: mov         dword ptr ds:[00E38EB8h],ecx
  0041867F: mov         byte ptr [esp+0Bh],0
  00418684: jne         004187B2
  0041868A: movsx       eax,byte ptr ds:[00E38EABh]
  00418691: cmp         eax,esi
  00418693: jl          004186BD
  00418695: cmp         eax,dword ptr ds:[006FF608h]
  0041869B: jge         004186BD
  0041869D: shl         eax,6
  004186A0: add         eax,6FDD08h
  004186A5: cmp         eax,esi
  004186A7: je          004186BD
  004186A9: mov         byte ptr [eax+3Ch],1
  004186AD: call        00516A20
  004186B2: push        eax
  004186B3: mov         ecx,6FDD08h
  004186B8: call        0041B8A0
  004186BD: movsx       eax,byte ptr ds:[00E38EABh]
  004186C4: movsx       ecx,byte ptr ds:[00E38EA9h]
  004186CB: push        esi
  004186CC: push        eax
  004186CD: push        ecx
  004186CE: mov         ecx,6FDD08h
  004186D3: call        0041B920
  004186D8: mov         ebx,eax
  004186DA: mov         al,byte ptr ds:[007027B4h]
  004186DF: cmp         al,2
  004186E1: je          004186E7
  004186E3: cmp         al,1
  004186E5: jne         00418762
  004186E7: cmp         ebx,esi
  004186E9: jl          00418762
  004186EB: call        004188F0
  004186F0: mov         dl,byte ptr ds:[00E38EABh]
  004186F6: movsx       eax,dl
  004186F9: cmp         eax,esi
  004186FB: jl          004187B2
  00418701: mov         ecx,dword ptr ds:[006FF608h]
  00418707: cmp         eax,ecx
  00418709: jge         004187B2
  0041870F: shl         eax,6
  00418712: add         eax,6FDD08h
  00418717: cmp         eax,esi
  00418719: je          004187B2
  0041871F: push        edi
  00418720: mov         edi,dword ptr [eax+38h]
  00418723: cmp         edi,2
  00418726: pop         edi
  00418727: jge         0041875B
  00418729: cmp         ebx,ecx
  0041872B: mov         byte ptr ds:[00E38EAAh],dl
  00418731: mov         byte ptr ds:[00E38EABh],bl
  00418737: jge         0041874E
  00418739: mov         eax,ebx
  0041873B: shl         eax,6
  0041873E: add         eax,6FDD08h
  00418743: mov         dl,byte ptr [eax+0Ch]
  00418746: mov         byte ptr ds:[00E38EA8h],dl
  0041874C: jmp         004187B2
  0041874E: xor         eax,eax
  00418750: mov         dl,byte ptr [eax+0Ch]
  00418753: mov         byte ptr ds:[00E38EA8h],dl
  00418759: jmp         004187B2
  0041875B: mov         byte ptr [esp+0Bh],1
  00418760: jmp         004187B2
  00418762: cmp         byte ptr ds:[00E38EA8h],28h
  00418769: jne         004187B2
  0041876B: mov         eax,799AE6h
  00418770: mov         ecx,0EBh
  00418775: cmp         word ptr [eax+0Ah],cx
  00418779: jne         00418796
  0041877B: mov         dx,word ptr [eax]
  0041877E: inc         dx
  00418780: mov         word ptr ds:[00702794h],dx
  00418787: mov         dx,word ptr [eax+2]
  0041878B: add         dx,11h
  0041878F: mov         word ptr ds:[00702796h],dx
  00418796: add         eax,108h
  0041879B: cmp         eax,89B6DEh
  004187A0: jl          00418775
  004187A2: call        00516A20
  004187A7: push        eax
  004187A8: mov         ecx,6FDD08h
  004187AD: call        0041B8A0
  004187B2: mov         eax,dword ptr ds:[00EAAC28h]
  004187B7: imul        eax,eax,49D8h
  004187BD: cmp         ebx,esi
  004187BF: mov         dword ptr [eax+00EA5D40h],esi
  004187C5: mov         dword ptr [eax+00EA5D44h],esi
  004187CB: mov         dword ptr [eax+00EA5D48h],esi
  004187D1: mov         eax,dword ptr ds:[00E38EBCh]
  004187D6: jl          0041880A
  004187D8: cmp         eax,esi
  004187DA: jne         0041880E
  004187DC: cmp         byte ptr ds:[007027B4h],3
  004187E3: je          00418835
  004187E5: mov         al,byte ptr [esp+0Bh]
  004187E9: test        al,al
  004187EB: je          00418800
  004187ED: mov         byte ptr ds:[00E38DDAh],34h
  004187F4: mov         byte ptr ds:[007027A4h],0
  004187FB: jmp         004188C0
  00418800: call        004181E0
  00418805: jmp         004188C0
  0041880A: cmp         eax,esi
  0041880C: je          00418835
  0041880E: mov         dword ptr ds:[00605A3Ch],esi
  00418814: mov         byte ptr ds:[006DE515h],0
  0041881B: mov         dword ptr ds:[00704DB0h],esi
  00418821: mov         dword ptr ds:[00704DACh],3
  0041882B: call        0041F870
  00418830: jmp         004188C0
  00418835: cmp         byte ptr ds:[007027B4h],1
  0041883C: jne         004188BB
  0041883E: movsx       eax,byte ptr ds:[00E38EA9h]
  00418845: inc         eax
  00418846: cmp         eax,5
  00418849: mov         dword ptr ds:[007027B0h],eax
  0041884E: jne         004188D7
  00418854: cmp         dword ptr ds:[00E38EDCh],0E9h
  0041885E: jge         00418889
  00418860: mov         eax,dword ptr ds:[00E38DACh]
  00418865: mov         ecx,dword ptr ds:[00E38DB0h]
  0041886B: add         eax,0FFFFFEE2h
  00418870: cdq
  00418871: sub         eax,edx
  00418873: sar         eax,1
  00418875: push        eax
  00418876: lea         eax,[ecx+FFFFFE20h]
  0041887C: cdq
  0041887D: sub         eax,edx
  0041887F: sar         eax,1
  00418881: push        eax
  00418882: push        57BFF8h
  00418887: jmp         004188B0
  00418889: mov         edx,dword ptr ds:[00E38DACh]
  0041888F: lea         eax,[edx+FFFFFEE2h]
  00418895: cdq
  00418896: sub         eax,edx
  00418898: sar         eax,1
  0041889A: push        eax
  0041889B: mov         eax,dword ptr ds:[00E38DB0h]
  004188A0: add         eax,0FFFFFE20h
  004188A5: cdq
  004188A6: sub         eax,edx
  004188A8: sar         eax,1
  004188AA: push        eax
  004188AB: push        57BFD8h
  004188B0: mov         ecx,dword ptr ds:[006FDC54h]
  004188B6: call        00413580
  004188BB: call        004CB6A0
  004188C0: mov         dword ptr ds:[00BC9694h],esi
  004188C6: mov         dword ptr ds:[00E94E0Ch],esi
  004188CC: pop         esi
  004188CD: mov         byte ptr ds:[00E38DCAh],1
  004188D4: pop         ebx
  004188D5: pop         ecx
  004188D6: ret
  004188D7: cmp         eax,9
  004188DA: jge         004188BB
  004188DC: mov         dword ptr ds:[007027ACh],eax
  004188E1: mov         byte ptr ds:[00E38DDAh],17h
  004188E8: mov         dword ptr ds:[006FDC98h],esi
  004188EE: jmp         004188C0
  004188F0: 83

  Summary

      16E000 .text
