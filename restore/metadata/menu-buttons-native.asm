Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  004261D0: push        ecx
  004261D1: mov         eax,dword ptr [esp+14h]
  004261D5: push        ebx
  004261D6: push        ebp
  004261D7: push        esi
  004261D8: test        eax,eax
  004261DA: push        edi
  004261DB: mov         dword ptr [esp+10h],0
  004261E3: jle         00426448
  004261E9: mov         esi,dword ptr [esp+1Ch]
  004261ED: mov         edi,dword ptr [esp+18h]
  004261F1: mov         ecx,dword ptr ds:[00E38E34h]
  004261F7: mov         dl,byte ptr ds:[00E38E03h]
  004261FD: mov         bl,byte ptr ds:[00E38E02h]
  00426203: mov         eax,dword ptr [esp+20h]
  00426207: movsx       ebp,word ptr [eax]
  0042620A: add         ebp,edi
  0042620C: cmp         ebp,ecx
  0042620E: jg          0042632D
  00426214: movsx       ebp,word ptr [eax+4]
  00426218: add         ebp,edi
  0042621A: cmp         ebp,ecx
  0042621C: jle         0042632D
  00426222: movsx       ebp,word ptr [eax+2]
  00426226: mov         ecx,dword ptr ds:[00E38E30h]
  0042622C: add         ebp,esi
  0042622E: cmp         ebp,ecx
  00426230: jg          0042632D
  00426236: movsx       ebp,word ptr [eax+6]
  0042623A: add         ebp,esi
  0042623C: cmp         ebp,ecx
  0042623E: jle         0042632D
  00426244: mov         cl,byte ptr [eax+13h]
  00426247: cmp         cl,1
  0042624A: jne         0042626F
  0042624C: test        dl,dl
  0042624E: jne         00426350
  00426254: mov         cl,byte ptr ds:[00E38DF7h]
  0042625A: test        cl,cl
  0042625C: jne         00426350
  00426262: test        bl,bl
  00426264: jne         00426350
  0042626A: jmp         0042632D
  0042626F: cmp         cl,3
  00426272: jne         00426295
  00426274: mov         cl,byte ptr ds:[00E38E04h]
  0042627A: test        cl,cl
  0042627C: jne         0042639E
  00426282: mov         cl,byte ptr ds:[00E38DF8h]
  00426288: test        cl,cl
  0042628A: jne         0042639E
  00426290: jmp         0042632D
  00426295: cmp         cl,2
  00426298: jne         00426318
  0042629A: mov         cl,byte ptr ds:[00E38E00h]
  004262A0: test        cl,cl
  004262A2: jne         004262DC
  004262A4: test        dl,dl
  004262A6: jne         004263D4
  004262AC: test        bl,bl
  004262AE: jne         004263D4
  004262B4: mov         cl,byte ptr ds:[00E38DF4h]
  004262BA: test        cl,cl
  004262BC: jne         004262EC
  004262BE: mov         cl,byte ptr ds:[00E38DF7h]
  004262C4: test        cl,cl
  004262C6: jne         004263D4
  004262CC: mov         cl,byte ptr ds:[00E38DF6h]
  004262D2: test        cl,cl
  004262D4: jne         004263D4
  004262DA: jmp         0042632D
  004262DC: test        dl,dl
  004262DE: jne         004263D4
  004262E4: test        bl,bl
  004262E6: jne         004263D4
  004262EC: mov         cl,byte ptr ds:[00E38DF7h]
  004262F2: test        cl,cl
  004262F4: jne         004263D4
  004262FA: mov         cl,byte ptr ds:[00E38DF6h]
  00426300: test        cl,cl
  00426302: jne         004263D4
  00426308: mov         ecx,dword ptr ds:[00D3B3ACh]
  0042630E: test        ecx,ecx
  00426310: jne         004263DE
  00426316: jmp         00426330
  00426318: cmp         cl,4
  0042631B: jne         0042632D
  0042631D: test        dl,dl
  0042631F: jne         00426424
  00426325: test        bl,bl
  00426327: jne         00426424
  0042632D: add         eax,1Ch
  00426330: mov         ecx,dword ptr [esp+10h]
  00426334: mov         ebp,dword ptr [esp+24h]
  00426338: inc         ecx
  00426339: cmp         ecx,ebp
  0042633B: mov         dword ptr [esp+10h],ecx
  0042633F: jge         00426448
  00426345: mov         ecx,dword ptr ds:[00E38E34h]
  0042634B: jmp         00426207
  00426350: mov         ecx,dword ptr [eax+14h]
  00426353: mov         byte ptr [eax+11h],1
  00426357: mov         dword ptr ds:[00D3B198h],ecx
  0042635D: mov         cl,byte ptr ds:[00E38E03h]
  00426363: mov         edx,dword ptr [eax+18h]
  00426366: test        cl,cl
  00426368: mov         dword ptr ds:[00D3B194h],edx
  0042636E: jne         0042643A
  00426374: mov         cl,byte ptr ds:[00E38E02h]
  0042637A: test        cl,cl
  0042637C: jne         0042643A
  00426382: mov         cl,byte ptr ds:[00E38DF7h]
  00426388: test        cl,cl
  0042638A: je          0042643D
  00426390: call        dword ptr [eax+0Ch]
  00426393: mov         eax,dword ptr [esp+10h]
  00426397: pop         edi
  00426398: pop         esi
  00426399: pop         ebp
  0042639A: inc         eax
  0042639B: pop         ebx
  0042639C: pop         ecx
  0042639D: ret
  0042639E: mov         ecx,dword ptr [eax+14h]
  004263A1: mov         byte ptr [eax+11h],1
  004263A5: mov         dword ptr ds:[00D3B198h],ecx
  004263AB: mov         cl,byte ptr ds:[00E38E04h]
  004263B1: mov         edx,dword ptr [eax+18h]
  004263B4: test        cl,cl
  004263B6: mov         dword ptr ds:[00D3B194h],edx
  004263BC: je          004263CC
  004263BE: call        dword ptr [eax+8]
  004263C1: mov         eax,dword ptr [esp+10h]
  004263C5: pop         edi
  004263C6: pop         esi
  004263C7: pop         ebp
  004263C8: inc         eax
  004263C9: pop         ebx
  004263CA: pop         ecx
  004263CB: ret
  004263CC: mov         cl,byte ptr ds:[00E38DF8h]
  004263D2: jmp         00426412
  004263D4: mov         dword ptr ds:[00D3B3C8h],0
  004263DE: mov         ecx,dword ptr [eax+14h]
  004263E1: mov         byte ptr [eax+11h],1
  004263E5: mov         dword ptr ds:[00D3B198h],ecx
  004263EB: mov         cl,byte ptr ds:[00E38E00h]
  004263F1: mov         edx,dword ptr [eax+18h]
  004263F4: test        cl,cl
  004263F6: mov         dword ptr ds:[00D3B194h],edx
  004263FC: je          0042640C
  004263FE: call        dword ptr [eax+8]
  00426401: mov         eax,dword ptr [esp+10h]
  00426405: pop         edi
  00426406: pop         esi
  00426407: pop         ebp
  00426408: inc         eax
  00426409: pop         ebx
  0042640A: pop         ecx
  0042640B: ret
  0042640C: mov         cl,byte ptr ds:[00E38DF4h]
  00426412: test        cl,cl
  00426414: je          00426419
  00426416: call        dword ptr [eax+0Ch]
  00426419: mov         eax,dword ptr [esp+10h]
  0042641D: pop         edi
  0042641E: pop         esi
  0042641F: pop         ebp
  00426420: inc         eax
  00426421: pop         ebx
  00426422: pop         ecx
  00426423: ret
  00426424: mov         ecx,dword ptr [eax+14h]
  00426427: mov         byte ptr [eax+11h],1
  0042642B: mov         dword ptr ds:[00D3B198h],ecx
  00426431: mov         edx,dword ptr [eax+18h]
  00426434: mov         dword ptr ds:[00D3B194h],edx
  0042643A: call        dword ptr [eax+8]
  0042643D: mov         eax,dword ptr [esp+10h]
  00426441: pop         edi
  00426442: pop         esi
  00426443: pop         ebp
  00426444: inc         eax
  00426445: pop         ebx
  00426446: pop         ecx
  00426447: ret
  00426448: pop         edi
  00426449: pop         esi
  0042644A: pop         ebp
  0042644B: xor         eax,eax
  0042644D: pop         ebx
  0042644E: pop         ecx
  0042644F: ret
  00426450: mov         eax,dword ptr [esp+10h]
  00426454: push        ebx
  00426455: push        ebp
  00426456: xor         ecx,ecx
  00426458: push        esi
  00426459: push        edi
  0042645A: test        eax,eax
  0042645C: jle         004264A9
  0042645E: mov         edi,dword ptr [esp+18h]
  00426462: mov         ebx,dword ptr [esp+14h]
  00426466: mov         edx,dword ptr ds:[00E38E34h]
  0042646C: mov         esi,dword ptr ds:[00E38E30h]
  00426472: mov         eax,dword ptr [esp+1Ch]
  00426476: movsx       ebp,word ptr [eax]
  00426479: add         ebp,ebx
  0042647B: cmp         ebp,edx
  0042647D: jg          0042649D
  0042647F: movsx       ebp,word ptr [eax+4]
  00426483: add         ebp,ebx
  00426485: cmp         ebp,edx
  00426487: jle         0042649D
  00426489: movsx       ebp,word ptr [eax+2]
  0042648D: add         ebp,edi
  0042648F: cmp         ebp,esi
  00426491: jg          0042649D
  00426493: movsx       ebp,word ptr [eax+6]
  00426497: add         ebp,edi
  00426499: cmp         ebp,esi
  0042649B: jg          004264B0
  0042649D: mov         ebp,dword ptr [esp+20h]
  004264A1: add         eax,1Ch
  004264A4: inc         ecx
  004264A5: cmp         ecx,ebp
  004264A7: jl          00426476
  004264A9: pop         edi
  004264AA: pop         esi
  004264AB: pop         ebp
  004264AC: xor         eax,eax
  004264AE: pop         ebx
  004264AF: ret
  004264B0: mov         dword ptr ds:[00E38E28h],esi
  004264B6: pop         edi
  004264B7: pop         esi
  004264B8: pop         ebp
  004264B9: mov         dword ptr ds:[00E38E2Ch],edx
  004264BF: lea         eax,[ecx+1]
  004264C2: pop         ebx
  004264C3: ret
  004264C4: nop
  004264C5: nop
  004264C6: nop
  004264C7: nop
  004264C8: nop
  004264C9: nop
  004264CA: nop
  004264CB: nop
  004264CC: nop
  004264CD: nop
  004264CE: nop
  004264CF: nop
  004264D0: mov         ecx,dword ptr ds:[00E38DA8h]
  004264D6: mov         edx,dword ptr ds:[00E38DA4h]
  004264DC: xor         eax,eax
  004264DE: add         ecx,50h

  Summary

      16E000 .text
