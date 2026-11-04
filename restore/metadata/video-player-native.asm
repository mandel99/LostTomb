
Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  004133D0: mov         eax,ecx
  004133D2: mov         ecx,dword ptr [esp+4]
  004133D6: mov         dword ptr [eax+8],ecx
  004133D9: xor         ecx,ecx
  004133DB: mov         byte ptr [eax+4],1
  004133DF: mov         dword ptr [eax+10h],ecx
  004133E2: mov         dword ptr [eax+0Ch],ecx
  004133E5: ret         4
  004133E8: nop
  004133E9: nop
  004133EA: nop
  004133EB: nop
  004133EC: nop
  004133ED: nop
  004133EE: nop
  004133EF: nop
  004133F0: push        esi
  004133F1: mov         esi,ecx
  004133F3: mov         eax,dword ptr [esi+10h]
  004133F6: test        eax,eax
  004133F8: je          00413408
  004133FA: push        eax
  004133FB: call        dword ptr ds:[0056F25Ch]
  00413401: mov         dword ptr [esi+10h],0
  00413408: mov         eax,dword ptr [esi+0Ch]
  0041340B: test        eax,eax
  0041340D: je          0041341D
  0041340F: push        eax
  00413410: call        dword ptr ds:[0056F260h]
  00413416: mov         dword ptr [esi+0Ch],0
  0041341D: pop         esi
  0041341E: ret
  0041341F: nop
  00413420: push        esi
  00413421: mov         esi,ecx
  00413423: mov         al,byte ptr [esi]
  00413425: mov         byte ptr [esi+4],1
  00413429: mov         byte ptr ds:[00E38DDAh],al
  0041342E: mov         byte ptr ds:[00E38DCAh],1
  00413435: mov         eax,dword ptr [esi+10h]
  00413438: test        eax,eax
  0041343A: je          0041344A
  0041343C: push        eax
  0041343D: call        dword ptr ds:[0056F25Ch]
  00413443: mov         dword ptr [esi+10h],0
  0041344A: mov         eax,dword ptr [esi+0Ch]
  0041344D: test        eax,eax
  0041344F: je          0041345F
  00413451: push        eax
  00413452: call        dword ptr ds:[0056F260h]
  00413458: mov         dword ptr [esi+0Ch],0
  0041345F: mov         al,1
  00413461: pop         esi
  00413462: ret
  00413463: nop
  00413464: nop
  00413465: nop
  00413466: nop
  00413467: nop
  00413468: nop
  00413469: nop
  0041346A: nop
  0041346B: nop
  0041346C: nop
  0041346D: nop
  0041346E: nop
  0041346F: nop
  00413470: push        ebx
  00413471: mov         ebx,ecx
  00413473: mov         eax,dword ptr [ebx+10h]
  00413476: test        eax,eax
  00413478: jne         0041347E
  0041347A: xor         al,al
  0041347C: pop         ebx
  0041347D: ret
  0041347E: mov         al,byte ptr [ebx+4]
  00413481: test        al,al
  00413483: je          00413489
  00413485: xor         al,al
  00413487: pop         ebx
  00413488: ret
  00413489: push        edi
  0041348A: mov         ecx,1Bh
  0041348F: xor         eax,eax
  00413491: mov         edi,605B78h
  00413496: rep stos    dword ptr es:[edi]
  00413498: mov         eax,dword ptr ds:[00E39178h]
  0041349D: push        0
  0041349F: push        1
  004134A1: mov         dword ptr ds:[00605B78h],6Ch
  004134AB: mov         ecx,dword ptr [eax]
  004134AD: push        605B78h
  004134B2: push        0
  004134B4: push        eax
  004134B5: call        dword ptr [ecx+64h]
  004134B8: cmp         eax,887601C2h
  004134BD: jne         004134EF
  004134BF: mov         eax,dword ptr ds:[00E39178h]
  004134C4: push        eax
  004134C5: mov         edx,dword ptr [eax]
  004134C7: call        dword ptr [edx+6Ch]
  004134CA: test        eax,eax
  004134CC: jne         0041357B
  004134D2: mov         eax,dword ptr ds:[00E39178h]
  004134D7: push        0
  004134D9: push        1
  004134DB: push        605B78h
  004134E0: mov         ecx,dword ptr [eax]
  004134E2: push        0
  004134E4: push        eax
  004134E5: call        dword ptr [ecx+64h]
  004134E8: cmp         eax,887601C2h
  004134ED: je          004134BF
  004134EF: test        eax,eax
  004134F1: je          00413519
  004134F3: push        eax
  004134F4: call        00427510
  004134F9: inc         eax
  004134FA: push        eax
  004134FB: push        0
  004134FD: push        576364h
  00413502: call        004CE6E0
  00413507: add         esp,10h
  0041350A: mov         dword ptr ds:[00E92E04h],0
  00413514: mov         al,1
  00413516: pop         edi
  00413517: pop         ebx
  00413518: ret
  00413519: mov         edx,dword ptr ds:[00E39130h]
  0041351F: mov         edi,dword ptr [ebx+18h]
  00413522: mov         ecx,dword ptr ds:[00E39134h]
  00413528: push        esi
  00413529: mov         esi,dword ptr [ebx+14h]
  0041352C: mov         ebx,dword ptr [ebx+10h]
  0041352F: add         esi,edx
  00413531: mov         edx,dword ptr ds:[00E39178h]
  00413537: push        edx
  00413538: add         edi,ecx
  0041353A: call        dword ptr ds:[0056F254h]
  00413540: push        eax
  00413541: mov         eax,dword ptr [ebx+4]
  00413544: push        0
  00413546: push        0
  00413548: push        eax
  00413549: mov         eax,dword ptr ds:[00605B88h]
  0041354E: push        eax
  0041354F: imul        eax,edi
  00413552: mov         ecx,dword ptr ds:[00605B9Ch]
  00413558: add         ecx,eax
  0041355A: lea         edx,[ecx+esi*2]
  0041355D: push        edx
  0041355E: push        ebx
  0041355F: call        dword ptr ds:[0056F258h]
  00413565: mov         eax,dword ptr ds:[00E39178h]
  0041356A: mov         edx,dword ptr ds:[00605B9Ch]
  00413570: push        edx
  00413571: push        eax
  00413572: mov         ecx,dword ptr [eax]
  00413574: call        dword ptr [ecx+00000080h]
  0041357A: pop         esi
  0041357B: pop         edi
  0041357C: mov         al,1
  0041357E: pop         ebx
  0041357F: ret
  00413580: sub         esp,64h
  00413583: push        esi
  00413584: mov         esi,ecx
  00413586: mov         eax,dword ptr [esi+10h]
  00413589: test        eax,eax
  0041358B: je          00413594
  0041358D: push        eax
  0041358E: call        dword ptr ds:[0056F25Ch]
  00413594: mov         eax,dword ptr ds:[012663ACh]
  00413599: test        eax,eax
  0041359B: je          004135DA
  0041359D: push        0
  0041359F: mov         ecx,0E92BF8h
  004135A4: call        0055F561
  004135A9: push        2
  004135AB: mov         ecx,0E92BF8h
  004135B0: call        0055F561
  004135B5: push        1
  004135B7: mov         ecx,0E92BF8h
  004135BC: call        0055F561
  004135C1: mov         eax,dword ptr ds:[012663ACh]
  004135C6: mov         edx,dword ptr ds:[0056F23Ch]
  004135CC: mov         ecx,dword ptr [eax+00000178h]
  004135D2: push        ecx
  004135D3: push        edx
  004135D4: call        dword ptr ds:[0056F238h]
  004135DA: push        edi
  004135DB: mov         edi,dword ptr [esp+70h]
  004135DF: push        edi
  004135E0: lea         eax,[esp+0Ch]
  004135E4: push        5763C0h
  004135E9: push        eax
  004135EA: call        00561A36
  004135EF: lea         ecx,[esp+14h]
  004135F3: push        5763BCh
  004135F8: push        ecx
  004135F9: call        00561A0A
  004135FE: add         esp,14h
  00413601: test        eax,eax
  00413603: jne         00413623
  00413605: xor         edx,edx
  00413607: push        edi
  00413608: mov         dl,byte ptr ds:[00E38E62h]
  0041360E: lea         eax,[esp+0Ch]
  00413612: push        edx
  00413613: push        5763B4h
  00413618: push        eax
  00413619: call        00561A36
  0041361E: add         esp,10h
  00413621: jmp         0041362C
  00413623: push        eax
  00413624: call        00561994
  00413629: add         esp,4
  0041362C: lea         ecx,[esp+8]
  00413630: push        0
  00413632: push        ecx
  00413633: call        dword ptr ds:[0056F24Ch]
  00413639: test        eax,eax
  0041363B: mov         dword ptr [esi+10h],eax
  0041363E: pop         edi
  0041363F: jne         00413655
  00413641: push        57639Ch
  00413646: call        dword ptr ds:[0056F0F8h]
  0041364C: xor         al,al
  0041364E: pop         esi
  0041364F: add         esp,64h
  00413652: ret         0Ch
  00413655: mov         ecx,dword ptr [esp+74h]
  00413659: xor         edx,edx
  0041365B: mov         dl,byte ptr ds:[00E38DDAh]
  00413661: mov         eax,dword ptr [esp+70h]
  00413665: mov         dword ptr [esi],edx
  00413667: mov         byte ptr ds:[00E38DDAh],4Ah
  0041366E: mov         byte ptr [esi+4],0
  00413672: mov         byte ptr ds:[00E38DCAh],0
  00413679: mov         dword ptr [esi+18h],ecx
  0041367C: mov         ecx,esi
  0041367E: mov         dword ptr [esi+14h],eax
  00413681: call        00413690
  00413686: mov         al,1
  00413688: pop         esi
  00413689: add         esp,64h
  0041368C: ret         0Ch
  0041368F: nop
  00413690: sub         esp,24h
  00413693: mov         eax,dword ptr ds:[006FDC54h]
  00413698: push        ebx
  00413699: push        ebp
  0041369A: push        esi
  0041369B: push        edi
  0041369C: xor         edi,edi
  0041369E: cmp         eax,edi
  004136A0: mov         esi,ecx
  004136A2: mov         dword ptr [esp+10h],edi
  004136A6: je          0041387A
  004136AC: call        00426F60
  004136B1: cmp         dword ptr ds:[00E38E5Ch],edi
  004136B7: je          004136C1
  004136B9: mov         dword ptr [esp+10h],1
  004136C1: mov         eax,dword ptr ds:[00E38E68h]
  004136C6: mov         dword ptr [esp+14h],eax
  004136CA: mov         eax,dword ptr [esp+10h]
  004136CE: cmp         eax,edi
  004136D0: je          0041370E
  004136D2: push        1
  004136D4: mov         dword ptr ds:[005E06B0h],1
  004136DE: call        004D01E0
  004136E3: push        1
  004136E5: call        004CFDC0
  004136EA: add         esp,8
  004136ED: call        00414BD0
  004136F2: call        004E2AB0
  004136F7: call        004D0220
  004136FC: mov         dword ptr ds:[005E06B0h],edi
  00413702: call        00402DA0
  00413707: mov         byte ptr ds:[00E38DCAh],1
  0041370E: mov         eax,dword ptr [esi+10h]
  00413711: mov         ecx,dword ptr ds:[00E38DB0h]
  00413717: push        edi
  00413718: sub         ecx,dword ptr [eax]
  0041371A: shr         ecx,1
  0041371C: mov         dword ptr [esi+14h],ecx
  0041371F: mov         edx,dword ptr ds:[00E38DACh]
  00413725: sub         edx,dword ptr [eax+4]
  00413728: shr         edx,1
  0041372A: mov         dword ptr [esi+18h],edx
  0041372D: call        dword ptr ds:[0056F194h]
  00413733: mov         eax,dword ptr [esi+10h]
  00413736: mov         ecx,dword ptr [eax+14h]
  00413739: mov         edx,dword ptr [eax+10h]
  0041373C: cmp         ecx,edx
  0041373E: jae         00413826
  00413744: mov         ebp,dword ptr ds:[0056F184h]
  0041374A: mov         ebx,dword ptr ds:[0056F250h]
  00413750: call        dword ptr ds:[0056F12Ch]
  00413756: mov         edx,dword ptr [esi+10h]
  00413759: push        edx
  0041375A: call        dword ptr ds:[0056F234h]
  00413760: mov         eax,dword ptr [esi+10h]
  00413763: push        eax
  00413764: call        dword ptr ds:[0056F248h]
  0041376A: mov         ecx,esi
  0041376C: call        00413470
  00413771: call        dword ptr ds:[0056F244h]
  00413777: mov         edx,eax
  00413779: or          ecx,0FFFFFFFFh
  0041377C: mov         edi,edx
  0041377E: xor         eax,eax
  00413780: repne scas  byte ptr es:[edi]
  00413782: not         ecx
  00413784: dec         ecx
  00413785: je          00413796
  00413787: push        eax
  00413788: push        edx
  00413789: push        5763C4h
  0041378E: call        004CE6E0
  00413793: add         esp,0Ch
  00413796: push        1
  00413798: push        0
  0041379A: push        0
  0041379C: lea         ecx,[esp+24h]
  004137A0: push        0
  004137A2: push        ecx
  004137A3: call        ebp
  004137A5: test        eax,eax
  004137A7: je          004137FB
  004137A9: mov         eax,dword ptr [esp+1Ch]
  004137AD: sub         eax,100h
  004137B2: je          00413882
  004137B8: sub         eax,4
  004137BB: je          00413882
  004137C1: sub         eax,0FDh
  004137C6: je          00413882
  004137CC: lea         edx,[esp+18h]
  004137D0: push        edx
  004137D1: call        dword ptr ds:[0056F18Ch]
  004137D7: lea         eax,[esp+18h]
  004137DB: push        eax
  004137DC: call        dword ptr ds:[0056F188h]
  004137E2: mov         ecx,dword ptr [esi+10h]
  004137E5: push        ecx
  004137E6: call        ebx
  004137E8: push        1
  004137EA: push        0
  004137EC: push        0
  004137EE: lea         edx,[esp+24h]
  004137F2: push        0
  004137F4: push        edx
  004137F5: call        ebp
  004137F7: test        eax,eax
  004137F9: jne         004137A9
  004137FB: mov         eax,dword ptr [esi+10h]
  004137FE: push        eax
  004137FF: call        ebx
  00413801: mov         ecx,dword ptr [esi+10h]
  00413804: push        ecx
  00413805: call        dword ptr ds:[0056F240h]
  0041380B: test        eax,eax
  0041380D: jne         00413771
  00413813: mov         eax,dword ptr [esi+10h]
  00413816: mov         edx,dword ptr [eax+14h]
  00413819: mov         ecx,dword ptr [eax+10h]
  0041381C: cmp         edx,ecx
  0041381E: jb          00413750
  00413824: xor         edi,edi
  00413826: mov         ecx,esi
  00413828: call        00413420
  0041382D: cmp         dword ptr [esp+10h],edi
  00413831: je          00413872
  00413833: push        1
  00413835: mov         dword ptr ds:[005E06B0h],1
  0041383F: call        004D01E0
  00413844: mov         ecx,dword ptr [esp+18h]
  00413848: push        ecx
  00413849: call        004CFDC0
  0041384E: add         esp,8
  00413851: call        00414BD0
  00413856: call        004E2AB0
  0041385B: call        004D0220
  00413860: mov         dword ptr ds:[005E06B0h],edi
  00413866: call        00402DA0
  0041386B: mov         byte ptr ds:[00E38DCAh],1
  00413872: push        1
  00413874: call        dword ptr ds:[0056F194h]
  0041387A: pop         edi
  0041387B: pop         esi
  0041387C: pop         ebp
  0041387D: pop         ebx
  0041387E: add         esp,24h
  00413881: ret
  00413882: mov         ecx,esi
  00413884: call        00413420
  00413889: mov         eax,dword ptr [esp+10h]
  0041388D: test        eax,eax
  0041388F: je          00413872
  00413891: push        1
  00413893: mov         dword ptr ds:[005E06B0h],1
  0041389D: call        004D01E0
  004138A2: mov         eax,dword ptr [esp+18h]
  004138A6: push        eax
  004138A7: call        004CFDC0
  004138AC: add         esp,8
  004138AF: call        00414BD0
  004138B4: call        004E2AB0
  004138B9: call        004D0220
  004138BE: mov         dword ptr ds:[005E06B0h],0
  004138C8: jmp         00413866
  004138CA: nop
  004138CB: nop
  004138CC: nop
  004138CD: nop
  004138CE: nop
  004138CF: nop

  Summary

      16E000 .text
